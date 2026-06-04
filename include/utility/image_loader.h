#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_IMAGE_LOADER_H_
#define ZENITHSTGV_INCLUDE_UTILITY_IMAGE_LOADER_H_

#include <string>
#include <vector>

namespace zenithstgv {
struct ImageData {
	std::vector<unsigned char> pixels;
	int width = 0;
	int height = 0;
	int channels = 0;

	ImageData() = default;
	ImageData(const ImageData &) = default;
	ImageData &operator=(const ImageData &) = default;
	ImageData(ImageData &&) noexcept = default;
	ImageData &operator=(ImageData &&) noexcept = default;
	~ImageData() = default;
};

ImageData loadImage(const std::string &path);
} // namespace zenithstgv

#endif
