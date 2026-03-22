#ifndef IMAGE_UTILITY_H
#define IMAGE_UTILITY_H

#include <irrlicht.h>

namespace ygo {

class ImageUtility {
private:
	static bool imageScaleSTB(irr::video::IImage* src, irr::video::IImage* dest);
	static void imageScaleNNAA(irr::video::IImage* src, irr::video::IImage* dest, bool use_threading);
public:
	static void Resize(irr::video::IImage* src, irr::video::IImage* dest, bool use_threading);
	/**
	 * Decode a JPEG file using libjpeg with optional DCT-domain downscaling (1/2, 1/4, 1/8).
	 * When targetWidth / targetHeight are provided the largest scale denominator that keeps
	 * the decoded dimensions >= target is chosen, saving memory and CPU time.
	 * The reader is not dropped by this function.
	 * @return Image pointer. Must be dropped after use.
	 */
	static irr::video::IImage* LoadJpegImageDownsampled(irr::video::IVideoDriver* driver, irr::io::IReadFile* reader, irr::s32 targetWidth, irr::s32 targetHeight);
};

} // namespace ygo

#endif // IMAGE_UTILITY_H
