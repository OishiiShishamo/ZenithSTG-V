#include "texture_init.h"

#include "utility/atlas_builder.h"

namespace zenithstgv {
void TextureInit::TexturesInit(AtlasBuilder &b) {
	b.add("bullet_normal", loadImage("textures/bullets/bullet_normal.png"));
	b.add("bullet_middle", loadImage("textures/bullets/bullet_medium.png"));
	b.add("bullet_big", loadImage("textures/bullets/bullet_big.png"));
	b.add("bullet_scale", loadImage("textures/bullets/bullet_scale.png"));
	b.add("bullet_light", loadImage("textures/bullets/bullet_light.png"));

	b.add("laser_normal", loadImage("textures/lasers/laser_normal.png"));

	b.add("player", loadImage("textures/player.png"));

	b.build();
}
} // namespace zenithstgv
