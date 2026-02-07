// CPU-only image resize benchmark (no GUI, no GPU requirement).

#define _CRT_SECURE_NO_WARNINGS

// This repo builds Irrlicht as a static library (see gframe/config.h).
// Define it here too, otherwise <irrlicht.h> declares dllimport symbols on Windows.
#define _IRR_STATIC_LIB_

#include <irrlicht.h>

#include "image_resizer.h"
#include "myfilesystem.h"

#include <algorithm>
#include <chrono>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

namespace {

enum class Algo { Stb, Nnaa, Both };

struct Args {
	std::string input_dir = "pics";
	std::string output_dir = "imgbench_out";
	uint32_t width = 55;
	uint32_t height = 80;
	int count = 500;
	Algo algo = Algo::Both;
	bool threading = true;
};

static void print_usage() {
	printf("ImgBench (CPU-only)\n");
	printf("Usage: ImgBench [--input DIR] [--output DIR] [--width W] [--height H] [--count N] [--algo stb|nnaa|both] [--threading 0|1]\n");
}

static bool parse_args(int argc, char** argv, Args& a) {
	for(int i = 1; i < argc; i++) {
		std::string k = argv[i];
		auto need = [&](const char* key) -> const char* {
			if(i + 1 >= argc) {
				printf("Missing value for %s\n", key);
				return nullptr;
			}
			return argv[++i];
		};
		if(k == "--help" || k == "-h") {
			print_usage();
			return false;
		} else if(k == "--input") {
			const char* v = need("--input");
			if(!v) return false;
			a.input_dir = v;
		} else if(k == "--output") {
			const char* v = need("--output");
			if(!v) return false;
			a.output_dir = v;
		} else if(k == "--width") {
			const char* v = need("--width");
			if(!v) return false;
			a.width = (uint32_t)atoi(v);
		} else if(k == "--height") {
			const char* v = need("--height");
			if(!v) return false;
			a.height = (uint32_t)atoi(v);
		} else if(k == "--count") {
			const char* v = need("--count");
			if(!v) return false;
			a.count = atoi(v);
		} else if(k == "--algo") {
			const char* v = need("--algo");
			if(!v) return false;
			std::string s = v;
			std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
			if(s == "stb") a.algo = Algo::Stb;
			else if(s == "nnaa") a.algo = Algo::Nnaa;
			else if(s == "both") a.algo = Algo::Both;
			else {
				printf("Unknown algo: %s\n", v);
				return false;
			}
		} else if(k == "--threading") {
			const char* v = need("--threading");
			if(!v) return false;
			a.threading = atoi(v) != 0;
		} else {
			printf("Unknown option: %s\n", k.c_str());
			print_usage();
			return false;
		}
	}
	if(a.width == 0 || a.height == 0) {
		printf("Invalid size.\n");
		return false;
	}
	if(a.count <= 0) a.count = 1;
	return true;
}

static bool ends_with_i(const std::string& s, const char* suffix) {
	size_t sl = s.size();
	size_t tl = strlen(suffix);
	if(sl < tl) return false;
	for(size_t i = 0; i < tl; ++i) {
		char a = (char)tolower((unsigned char)s[sl - tl + i]);
		char b = (char)tolower((unsigned char)suffix[i]);
		if(a != b) return false;
	}
	return true;
}

static bool is_image_file(const std::string& path) {
	return ends_with_i(path, ".jpg") || ends_with_i(path, ".jpeg") || ends_with_i(path, ".png") || ends_with_i(path, ".bmp") || ends_with_i(path, ".tga");
}

static char path_sep() {
#ifdef _WIN32
	return '\\';
#else
	return '/';
#endif
}

static std::string join_path(const std::string& a, const std::string& b) {
	if(a.empty()) return b;
	if(b.empty()) return a;
	char last = a.back();
	if(last == '/' || last == '\\') return a + b;
	return a + path_sep() + b;
}

static std::string file_name_only(const std::string& p) {
	size_t s = p.find_last_of("/\\");
	return (s == std::string::npos) ? p : p.substr(s + 1);
}

static std::string strip_ext(const std::string& p) {
	size_t d = p.find_last_of('.');
	if(d == std::string::npos) return p;
	return p.substr(0, d);
}

static bool ensure_dir_recursive(const std::string& dir_utf8) {
	if(dir_utf8.empty())
		return false;
	if(FileSystem::IsDirExists(dir_utf8.c_str()))
		return true;

	std::string cur;
	cur.reserve(dir_utf8.size());
	for(size_t i = 0; i < dir_utf8.size(); ++i) {
		char c = dir_utf8[i];
		cur.push_back(c);
		if(c == '/' || c == '\\') {
			if(cur.size() == 1) continue;
			// Skip drive like C:
			if(cur.size() == 3 && isalpha((unsigned char)cur[0]) && cur[1] == ':' )
				continue;
			if(!FileSystem::IsDirExists(cur.c_str()))
				FileSystem::MakeDir(cur.c_str());
		}
	}
	if(!FileSystem::IsDirExists(dir_utf8.c_str()))
		FileSystem::MakeDir(dir_utf8.c_str());
	return FileSystem::IsDirExists(dir_utf8.c_str());
}

static void list_files_recursive(const std::string& root, std::vector<std::string>& out) {
	FileSystem::TraversalDir(root.c_str(), [&](const char* name, bool isdir) {
		std::string full = join_path(root, name);
		if(isdir) {
			list_files_recursive(full, out);
		} else {
			if(is_image_file(full))
				out.push_back(full);
		}
	});
}

static void print_cwd() {
#ifdef _WIN32
	wchar_t wcwd[2048];
	DWORD n = GetCurrentDirectoryW((DWORD)(sizeof(wcwd) / sizeof(wcwd[0])), wcwd);
	if(n == 0 || n >= (DWORD)(sizeof(wcwd) / sizeof(wcwd[0]))) {
		printf("CWD: <unknown>\n");
		return;
	}
	char cwd_utf8[4096];
	BufferIO::EncodeUTF8(wcwd, cwd_utf8);
	printf("CWD: %s\n", cwd_utf8);
#else
	char buf[2048];
	if(getcwd(buf, sizeof(buf)) == nullptr) {
		printf("CWD: <unknown>\n");
		return;
	}
	printf("CWD: %s\n", buf);
#endif
}

static irr::io::path to_irr_path(const std::string& s) {
#ifdef _IRR_WCHAR_FILESYSTEM
	wchar_t ws[2048];
	BufferIO::DecodeUTF8(s.c_str(), ws);
	return irr::io::path(ws);
#else
	return irr::io::path(s.c_str());
#endif
}

} // namespace

int main(int argc, char** argv) {
	Args args;
	if(!parse_args(argc, argv, args))
		return 1;

	irr::SIrrlichtCreationParameters params{};
	params.DriverType = irr::video::EDT_NULL;
	params.WindowSize = irr::core::dimension2d<irr::u32>(1, 1);
	params.AntiAlias = 0;
	params.Fullscreen = false;
	params.Stencilbuffer = false;
	params.Vsync = false;
	params.Doublebuffer = false;
	params.IgnoreInput = true;
	irr::IrrlichtDevice* device = irr::createDeviceEx(params);
	if(!device) {
		printf("Failed to create Irrlicht device (EDT_NULL).\n");
		return 2;
	}
	device->getLogger()->setLogLevel(irr::ELL_ERROR);
	irr::video::IVideoDriver* driver = device->getVideoDriver();
	if(!driver) {
		printf("Failed to get Irrlicht video driver.\n");
		device->drop();
		return 2;
	}

	std::vector<std::string> files;
	if(!FileSystem::IsDirExists(args.input_dir.c_str())) {
		printf("Input dir not found: %s\n", args.input_dir.c_str());
		print_cwd();
		device->drop();
		return 3;
	}
	list_files_recursive(args.input_dir, files);
	if(files.empty()) {
		printf("No images found under: %s\n", args.input_dir.c_str());
		print_cwd();
		device->drop();
		return 3;
	}
	if((int)files.size() > args.count)
		files.resize(args.count);

	if(!ensure_dir_recursive(args.output_dir)) {
		printf("Failed to create output dir: %s\n", args.output_dir.c_str());
		device->drop();
		return 4;
	}

	std::vector<irr::video::IImage*> images;
	images.reserve(files.size());
	std::vector<std::string> loaded_files;
	loaded_files.reserve(files.size());

	using clock = std::chrono::steady_clock;
	auto t0 = clock::now();
	for(const auto& f : files) {
		irr::video::IImage* img = driver->createImageFromFile(to_irr_path(f));
		if(!img)
			continue;
		images.push_back(img);
		loaded_files.push_back(f);
	}
	auto t1 = clock::now();
	double decode_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
	printf("Loaded %zu/%zu images in %.3f ms\n", images.size(), files.size(), decode_ms);
	if(images.empty()) {
		device->drop();
		return 5;
	}

	auto run_one = [&](Algo algo) {
		long long resize_ns = 0;
		long long save_ns = 0;
		int saved = 0;
		for(size_t i = 0; i < images.size(); i++) {
			irr::video::IImage* src = images[i];
			irr::video::IImage* dest = driver->createImage(src->getColorFormat(), irr::core::dimension2d<irr::u32>(args.width, args.height));
			if(!dest)
				continue;

			auto rs0 = clock::now();
			if(algo == Algo::Stb) {
				ygo::imageResizer.resizeSTB(src, dest);
			} else {
				ygo::imageResizer.resizeNNAA(src, dest, args.threading);
			}
			auto rs1 = clock::now();
			resize_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(rs1 - rs0).count();

			std::string base = strip_ext(file_name_only(loaded_files[i]));
			const char* tag = (algo == Algo::Stb) ? "stb" : "nnaa";
			std::string outPath = join_path(args.output_dir, base + "_" + tag + ".png");

			auto ws0 = clock::now();
			bool ok = driver->writeImageToFile(dest, to_irr_path(outPath));
			auto ws1 = clock::now();
			save_ns += std::chrono::duration_cast<std::chrono::nanoseconds>(ws1 - ws0).count();
			if(ok)
				saved++;
			dest->drop();
		}
		const double resize_ms = (double)resize_ns / 1e6;
		const double save_ms = (double)save_ns / 1e6;
		const double per_img_us = images.empty() ? 0.0 : ((double)resize_ns / 1e3) / (double)images.size();
		printf("%s: resize=%.3f ms (%.3f us/img), save=%.3f ms, saved=%d\n",
				(algo == Algo::Stb) ? "STB" : "NNAA", resize_ms, per_img_us, save_ms, saved);
	};

	if(args.algo == Algo::Stb || args.algo == Algo::Both)
		run_one(Algo::Stb);
	if(args.algo == Algo::Nnaa || args.algo == Algo::Both)
		run_one(Algo::Nnaa);

	for(auto* img : images)
		img->drop();
	device->drop();
	return 0;
}
