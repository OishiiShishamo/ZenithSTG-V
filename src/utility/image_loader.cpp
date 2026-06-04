#define STB_IMAGE_IMPLEMENTATION
#include "utility/image_loader.h"

#include "3rdparty/stb_image.h"

#include <stdexcept>

namespace zenithstgv {
ImageData loadImage(const std::string &path) {
	ImageData data;
	int w, h, c;
	unsigned char *raw = stbi_load(path.c_str(), &w, &h, &c, STBI_rgb_alpha);
	if (!raw) {
		throw std::runtime_error("failed to load texture: " + path);
	}

	data.width = w;
	data.height = h;
	data.channels = 4;
	data.pixels.assign(raw, raw + w * h * 4);
	stbi_image_free(raw);
	return data;
}
} // namespace zenithstgv
