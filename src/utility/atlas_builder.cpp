#include "utility/atlas_builder.h"

#include <cstring>
#include <stdexcept>

#define STB_RECT_PACK_IMPLEMENTATION
#include "3rdparty/stb_rect_pack.h"

namespace zenithstgv {
AtlasBuilder::AtlasBuilder(int width, int height)
    : width_(width), height_(height) {
	atlas_.resize(width_ * height_ * 4, 0);
}

void AtlasBuilder::add(const std::string &name, ImageData &&image) {
	items_.push_back({name, std::move(image)});
}

void AtlasBuilder::build() {
	std::vector<stbrp_rect> rects(items_.size());
	std::vector<stbrp_node> nodes(width_);

	stbrp_context ctx;
	stbrp_init_target(&ctx, width_, height_, nodes.data(), nodes.size());

	for (size_t i = 0; i < items_.size(); i++) {
		rects[i].id = i;
		rects[i].w = items_[i].image.width;
		rects[i].h = items_[i].image.height;
	}

	stbrp_pack_rects(&ctx, rects.data(), rects.size());
	for (auto &r : rects) {
		if (!r.was_packed) {
			throw std::runtime_error("atlas packing failed: " +
			                         items_[r.id].name);
		}
	}

	for (size_t i = 0; i < items_.size(); i++) {
		auto &r = rects[i];
		auto &it = items_[r.id];

		it.x = r.x;
		it.y = r.y;

		for (int y = 0; y < r.h; y++) {
			std::memcpy(&atlas_[((r.y + y) * width_ + r.x) * 4],
			            &it.image.pixels[y * r.w * 4], r.w * 4);
		}

		AtlasEntry entry;
		entry.x = r.x;
		entry.y = r.y;
		entry.w = r.w;
		entry.h = r.h;

		entry.uv.u0 = (float)r.x / width_;
		entry.uv.v0 = (float)r.y / height_;
		entry.uv.u1 = (float)(r.x + r.w) / width_;
		entry.uv.v1 = (float)(r.y + r.h) / height_;

		entries_[it.name] = entry;
	}
}

const std::vector<unsigned char> &AtlasBuilder::getAtlasData() const {
	return atlas_;
}

const std::unordered_map<std::string, AtlasEntry> &
AtlasBuilder::getEntries() const {
	return entries_;
}

ImageData AtlasBuilder::getImageData() const {
	ImageData img;
	img.width = width_;
	img.height = height_;
	img.channels = 4;
	img.pixels = atlas_;
	return img;
}

} // namespace zenithstgv
