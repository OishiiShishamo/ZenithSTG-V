#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_IMAGE_LOADER_H_
#define ZENITHSTGV_INCLUDE_UTILITY_IMAGE_LOADER_H_

#include <string>

namespace zenithstgv {
struct ImageData {
	unsigned char *pixels = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;

	void free();
};

ImageData loadImage(const std::string &path);
} // namespace zenithstgv

#endif
