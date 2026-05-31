#define STB_IMAGE_IMPLEMENTATION
#include "utility/image_loader.h"

#include "3rdparty/stb_image.h"

#include <stdexcept>

namespace zenithstgv {
ImageData loadImage(const std::string &path) {
	ImageData data;
	data.pixels = stbi_load(path.c_str(), &data.width, &data.height,
	                        &data.channels, STBI_rgb_alpha);
	if (!data.pixels) {
		throw std::runtime_error("failed to load texture: " + path);
	}
	return data;
}

void ImageData::free() {
	if (pixels) {
		stbi_image_free(pixels);
		pixels = nullptr;
	}
}
} // namespace zenithstgv
