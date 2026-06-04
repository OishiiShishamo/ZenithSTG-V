#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_ATLAS_BUILDER_H_
#define ZENITHSTGV_INCLUDE_UTILITY_ATLAS_BUILDER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include "utility/image_loader.h"

namespace zenithstgv {

struct UVRect {
	float u0, v0;
	float u1, v1;
};

struct AtlasEntry {
	int x, y, w, h;
	UVRect uv;
};

class AtlasBuilder {
  public:
	AtlasBuilder(int width, int height);

	void add(const std::string &name, ImageData &&image);

	void build();

	const std::vector<unsigned char> &getAtlasData() const;
	const std::unordered_map<std::string, AtlasEntry> &getEntries() const;
	ImageData getImageData() const;

  private:
	int width_;
	int height_;

	struct Item {
		std::string name;
		ImageData image;
		int x = 0, y = 0;
	};

	std::vector<Item> items_;

	std::vector<unsigned char> atlas_;
	std::unordered_map<std::string, AtlasEntry> entries_;
};

} // namespace zenithstgv

#endif
