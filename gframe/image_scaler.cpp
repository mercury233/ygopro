#include "image_scaler.h"
#include "game.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace ygo {

/** Scale image using nearest neighbor anti-aliasing.
 * Function by Warr1024, from https://github.com/minetest/minetest/issues/2419, modified. */
void imageScaleNNAA(irr::video::IImage* src, irr::video::IImage* dest) {
	const auto& srcDim = src->getDimension();
	const auto& destDim = dest->getDimension();

	// Cache scale ratios.
	const double rx = (double)srcDim.Width / destDim.Width;
	const double ry = (double)srcDim.Height / destDim.Height;

#pragma omp parallel if(mainGame->gameConf.use_image_scale_multi_thread)
	{
		double sx, sy, minsx, maxsx, minsy, maxsy, area, ra, ga, ba, aa, pw, ph, pa;
		irr::video::SColor pxl, npxl;

		// Walk each destination image pixel.
#pragma omp for schedule(dynamic)
		for(irr::s32 dy = 0; dy < (irr::s32)destDim.Height; dy++) {
			for(irr::s32 dx = 0; dx < (irr::s32)destDim.Width; dx++) {
				// Calculate floating-point source rectangle bounds.
				minsx = dx * rx;
				maxsx = minsx + rx;
				minsy = dy * ry;
				maxsy = minsy + ry;

				// Total area, and integral of r, g, b values over that area,
				// initialized to zero, to be summed up in next loops.
				area = 0;
				ra = 0;
				ga = 0;
				ba = 0;
				aa = 0;

				// Loop over the integral pixel positions described by those bounds.
				for(sy = floor(minsy); sy < maxsy; sy++) {
					for(sx = floor(minsx); sx < maxsx; sx++) {
						// Calculate width, height, then area of dest pixel
						// that's covered by this source pixel.
						pw = 1;
						if(minsx > sx)
							pw += sx - minsx;
						if(maxsx < (sx + 1))
							pw += maxsx - sx - 1;
						ph = 1;
						if(minsy > sy)
							ph += sy - minsy;
						if(maxsy < (sy + 1))
							ph += maxsy - sy - 1;
						pa = pw * ph;

						// Get source pixel and add it to totals, weighted
						// by covered area and alpha.
						pxl = src->getPixel((irr::u32)sx, (irr::u32)sy);
						area += pa;
						ra += pa * pxl.getRed();
						ga += pa * pxl.getGreen();
						ba += pa * pxl.getBlue();
						aa += pa * pxl.getAlpha();
					}
				}
				// Set the destination image pixel to the average color.
				if(area > 0) {
					npxl.set((irr::u32)(aa / area + 0.5),
							 (irr::u32)(ra / area + 0.5),
							 (irr::u32)(ga / area + 0.5),
							 (irr::u32)(ba / area + 0.5));
				} else {
					npxl.set(0);
				}
				dest->setPixel(dx, dy, npxl);
			}
		}
	} // end of parallel region
}

} // namespace ygo
