#include "texture_init.h"

#include "utility/atlas_builder.h"

namespace zenithstgv {
void TextureInit::TexturesInit(AtlasBuilder &b) {
	b.add("bullet_1", loadImage("textures/bullet_1.png"));

	b.build();
}
} // namespace zenithstgv
