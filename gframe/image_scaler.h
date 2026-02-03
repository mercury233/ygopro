#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <irrlicht.h>

namespace ygo {

// Use stb_image_resize2 when possible, fallback to NNAA.
void imageScale(irr::video::IImage* src, irr::video::IImage* dest);

}

#endif // IMAGESCALER_H