#pragma once
#ifndef ZENITHSTGV_INCLUDE_DRAW_TEXTURE_H_
#define ZENITHSTGV_INCLUDE_DRAW_TEXTURE_H_

#include <array>
#include <string>
#include <vector>

#include "color.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {
class DrawTexture {
  public:
	static void
	DrawTex(const AtlasBuilder &atlas,
	        std::array<std::vector<InstanceData>, 4> &instance_lists, Vec2D pos,
	        double size, double angle_rad, std::string key, Color color,
	        BlendMode blend, double blend_pal);
};
} // namespace zenithstgv

#endif
