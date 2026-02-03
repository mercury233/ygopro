#pragma once

#include <irrlicht.h>

namespace ygo {

// Reference scaler (existing NNAA implementation).
void imageScaleNNAA(irr::video::IImage* src, irr::video::IImage* dest);

} // namespace ygo
