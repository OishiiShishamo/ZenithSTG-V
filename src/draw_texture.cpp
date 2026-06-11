#include "draw_texture.h"

#include <array>
#include <string>
#include <vector>

#include "color.h"
#include "global.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"
namespace zenithstgv {
void DrawTexture::DrawTex(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 4> &instance_lists, Vec2D pos,
    double size, double angle_rad, std::string key, Color color,
    BlendMode blend, double blend_pal) {
	std::array<Vec2D, 4> world;
	double half = size / 2;
	std::array<Vec2D, 4> local = {Vec2D(-half, -half), Vec2D(half, -half),
	                              Vec2D(half, half), Vec2D(-half, half)};

	for (int i = 0; i < 4; ++i) {
		Vec2D rot = RotatePoint(local[i], angle_rad + kPi / 2);
		world[i] = pos + rot;
	}
	Sprite spr;
	spr.pos[0] = world[0].ToNDC(ScreenSize(1920, 1080));
	spr.pos[1] = world[1].ToNDC(ScreenSize(1920, 1080));
	spr.pos[2] = world[2].ToNDC(ScreenSize(1920, 1080));
	spr.pos[3] = world[3].ToNDC(ScreenSize(1920, 1080));
	spr.color = color;
	spr.blend_mode = blend;
	spr.blend_pal = blend_pal;
	spr.uv_key = key;
	instance_lists[static_cast<int>(blend)].push_back(
	    spr.toInstanceData(atlas.getEntries().at(key)));
}
} // namespace zenithstgv
