#pragma once
#ifndef ZENITHSTGV_INCLUDE_FONT_RENDERER_H_
#define ZENITHSTGV_INCLUDE_FONT_RENDERER_H_

#include <array>
#include <string>
#include <unordered_map>
#include <vector>

#include "color.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {

enum class TextAlign {
	kAlignLeft,
	kAlignCenter,
	kAlignRight,
};

class FontRenderer {
  public:
	struct Codepoints {
		static std::vector<int> Ascii();
		static std::vector<int> Hiragana();
		static std::vector<int> Katakana();
		static std::vector<int> JouyoKanji();
		static std::vector<int> Japanese();
	};

	static bool LoadFont(AtlasBuilder &atlas, const std::string &font_id,
	                     const std::string &path, float font_size,
	                     const std::vector<int> &codepoints);

	static void
	DrawText(const AtlasBuilder &atlas,
	         std::array<std::vector<InstanceData>, 5> &instance_lists,
	         const std::string &font_id, const std::string &text, Vec2D pos,
	         float font_size, Color color,
	         TextAlign align = TextAlign::kAlignLeft);

	static bool HasFont(const std::string &font_id);

	static void FontInit(AtlasBuilder &b);

  private:
	struct GlyphInfo {
		int codepoint;
		int advance_x;
		int bearing_x;
		int bearing_y;
		int xoff;
		int yoff;
		int width;
		int height;
		std::string atlas_key;
	};

	struct FontData {
		std::string font_id;
		float font_size;
		int ascent;
		int descent;
		int line_gap;
		std::unordered_map<int, GlyphInfo> glyphs;
	};

	static std::unordered_map<std::string, FontData> fonts_;
};

} // namespace zenithstgv

#endif
