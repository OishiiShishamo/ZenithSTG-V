#include "font_renderer.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "3rdparty/stb_truetype.h"

#include "color.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {
std::unordered_map<std::string, FontRenderer::FontData> FontRenderer::fonts_;

bool FontRenderer::LoadFont(AtlasBuilder &atlas, const std::string &font_id,
                            const std::string &path, float font_size,
                            const std::vector<int> &codepoints) {
	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	if (!ifs.is_open()) {
		std::cerr << "[FontRenderer] Cannot open font: " << path << "\n";
		return false;
	}
	std::streamsize size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<unsigned char> buf(size);
	if (!ifs.read(reinterpret_cast<char *>(buf.data()), size)) {
		std::cerr << "[FontRenderer] Failed to read font: " << path << "\n";
		return false;
	}

	stbtt_fontinfo info;
	if (!stbtt_InitFont(&info, buf.data(), 0)) {
		std::cerr << "[FontRenderer] stbtt_InitFont failed: " << path << "\n";
		return false;
	}

	float scale = stbtt_ScaleForPixelHeight(&info, font_size);

	int ascent_raw, descent_raw, line_gap_raw;
	stbtt_GetFontVMetrics(&info, &ascent_raw, &descent_raw, &line_gap_raw);

	FontData fd;
	fd.font_id = font_id;
	fd.font_size = font_size;
	fd.ascent = static_cast<int>(ascent_raw * scale);
	fd.descent = static_cast<int>(descent_raw * scale);
	fd.line_gap = static_cast<int>(line_gap_raw * scale);

	for (int cp : codepoints) {
		const int glyph_idx = stbtt_FindGlyphIndex(&info, cp);
		if (glyph_idx == 0 && cp != 32)
			continue;

		int adv, lsb;
		stbtt_GetGlyphHMetrics(&info, glyph_idx, &adv, &lsb);

		FontRenderer::GlyphInfo gi{};
		gi.codepoint = cp;
		gi.advance_x = adv * scale;

		if (cp == 32) {
			fd.glyphs[cp] = gi;
			continue;
		}

		int bw, bh;
		int padding = 4;
		unsigned char onedge_value = 180;
		float pixel_dist_scale = 36.0f;

		int xoff, yoff;
		unsigned char *sdf =
		    stbtt_GetGlyphSDF(&info, scale, glyph_idx, padding, onedge_value,
		                      pixel_dist_scale, &bw, &bh, &xoff, &yoff);

		if (!sdf || bw <= 0 || bh <= 0)
			continue;

		ImageData img;
		img.width = bw;
		img.height = bh;
		img.channels = 4;
		img.pixels.resize(bw * bh * 4);
		for (int i = 0; i < bw * bh; i++) {
			img.pixels[i * 4 + 0] = sdf[i];
			img.pixels[i * 4 + 1] = sdf[i];
			img.pixels[i * 4 + 2] = sdf[i];
			img.pixels[i * 4 + 3] = 255;
		}

		gi.bearing_x = lsb * scale;
		gi.bearing_y = 0;
		gi.xoff = xoff;
		gi.yoff = yoff;
		gi.width = bw;
		gi.height = bh;
		gi.atlas_key = font_id + "_cp" + std::to_string(cp);

		atlas.add(gi.atlas_key, std::move(img));
		fd.glyphs[cp] = gi;
		stbtt_FreeSDF(sdf, nullptr);
	}

	fonts_[font_id] = std::move(fd);
	std::cout << "[FontRenderer] Loaded font: " << path << " id=" << font_id
	          << " size=" << font_size << "\n";
	return true;
}

bool FontRenderer::HasFont(const std::string &font_id) {
	return fonts_.find(font_id) != fonts_.end();
}

void FontRenderer::DrawText(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 5> &instance_lists,
    const std::string &font_id, const std::string &text, Vec2D pos,
    float font_size, Color color, TextAlign align) {
	auto it = fonts_.find(font_id);
	if (it == fonts_.end()) {
		return;
	}
	const FontData &fd = it->second;
	const auto &entries = atlas.getEntries();

	const float scale =
	    (fd.font_size > 0.0f) ? (font_size / fd.font_size) : 1.0f;
	const float s = scale;

	float total_w = 0.0f;
	for (size_t i = 0; i < text.size();) {
		unsigned char c = static_cast<unsigned char>(text[i]);
		int cp;
		if (c < 0x80) {
			cp = c;
			i += 1;
		} else if (c < 0xE0) {
			cp = (c & 0x1F) << 6 | (text[i + 1] & 0x3F);
			i += 2;
		} else if (c < 0xF0) {
			cp = (c & 0x0F) << 12 | (text[i + 1] & 0x3F) << 6 |
			     (text[i + 2] & 0x3F);
			i += 3;
		} else {
			cp = (c & 0x07) << 18 | (text[i + 1] & 0x3F) << 12 |
			     (text[i + 2] & 0x3F) << 6 | (text[i + 3] & 0x3F);
			i += 4;
		}
		auto git = fd.glyphs.find(cp);
		if (git == fd.glyphs.end())
			continue;
		total_w += static_cast<float>(git->second.advance_x) * scale;
	}
	float cursor_x = static_cast<float>(pos.GetX());
	float cursor_y = static_cast<float>(pos.GetY());
	if (align == TextAlign::kAlignCenter)
		cursor_x -= total_w * 0.5f;
	else if (align == TextAlign::kAlignRight)
		cursor_x -= total_w;
	const ScreenSize screen(1920.0, 1080.0);
	for (size_t i = 0; i < text.size();) {
		unsigned char c = static_cast<unsigned char>(text[i]);
		int cp;
		if (c < 0x80) {
			cp = c;
			i += 1;
		} else if (c < 0xE0) {
			cp = (c & 0x1F) << 6 | (text[i + 1] & 0x3F);
			i += 2;
		} else if (c < 0xF0) {
			cp = (c & 0x0F) << 12 | (text[i + 1] & 0x3F) << 6 |
			     (text[i + 2] & 0x3F);
			i += 3;
		} else {
			cp = (c & 0x07) << 18 | (text[i + 1] & 0x3F) << 12 |
			     (text[i + 2] & 0x3F) << 6 | (text[i + 3] & 0x3F);
			i += 4;
		}
		auto git = fd.glyphs.find(cp);
		if (git == fd.glyphs.end()) {
			cursor_x += 8.0f * scale;
			continue;
		}
		const GlyphInfo &gi = git->second;
		if (gi.atlas_key.empty() || gi.width == 0) {
			cursor_x += static_cast<float>(gi.advance_x);
			continue;
		}
		auto eit = entries.find(gi.atlas_key);
		if (eit == entries.end()) {
			cursor_x += static_cast<float>(gi.advance_x);
			continue;
		}
		const AtlasEntry &ae = eit->second;
		const float baseline_y =
		    cursor_y + static_cast<float>(fd.ascent) * scale;

		const float gx0 = cursor_x + (gi.bearing_x) + (gi.xoff * s);
		const float gy0 = baseline_y + (gi.bearing_y) + (gi.yoff * s);
		const float gx1 = gx0 + (gi.width);
		const float gy1 = gy0 + (gi.height);
		Vec2D tl = Vec2D(gx0, gy0).ToNDC(screen);
		Vec2D tr = Vec2D(gx1, gy0).ToNDC(screen);
		Vec2D br = Vec2D(gx1, gy1).ToNDC(screen);
		Vec2D bl = Vec2D(gx0, gy1).ToNDC(screen);
		InstanceData d{};
		d.pos[0] = {static_cast<float>(tl.GetX()),
		            static_cast<float>(tl.GetY())};
		d.pos[1] = {static_cast<float>(tr.GetX()),
		            static_cast<float>(tr.GetY())};
		d.pos[2] = {static_cast<float>(br.GetX()),
		            static_cast<float>(br.GetY())};
		d.pos[3] = {static_cast<float>(bl.GetX()),
		            static_cast<float>(bl.GetY())};
		d.color = {color.GetR(), color.GetG(), color.GetB(), color.GetA()};
		d.uv = {ae.uv.u0, ae.uv.v0, ae.uv.u1, ae.uv.v1};
		d.blend_pal = 1.0;
		instance_lists[4].push_back(d);
		cursor_x += static_cast<float>(gi.advance_x);
	}
}

std::vector<int> FontRenderer::Codepoints::Ascii() {
	std::vector<int> v;
	for (int c = 0x0020; c <= 0x007E; c++)
		v.push_back(c);
	return v;
}

std::vector<int> FontRenderer::Codepoints::Hiragana() {
	std::vector<int> v;
	for (int c = 0x3041; c <= 0x3096; c++)
		v.push_back(c);
	for (int c = 0x309D; c <= 0x309F; c++)
		v.push_back(c);

	v.push_back(0x30FB);
	v.push_back(0x30FC);
	return v;
}

std::vector<int> FontRenderer::Codepoints::Katakana() {
	std::vector<int> v;
	for (int c = 0x30A0; c <= 0x30FF; c++)
		v.push_back(c);
	for (int c = 0xFF65; c <= 0xFF9F; c++)
		v.push_back(c);
	return v;
}

std::vector<int> FontRenderer::Codepoints::JouyoKanji() {
	std::vector<int> v;

	static const char32_t block1[] =
	    U"一丁七万丈三上下不与丑且世丘丙両並中串丸丹主乃久之乍乏乗乙九乞也乱乳"
	    U"乾亀了予争事二互五井亜亡交亥亨享京亭亮人仁今介仕他付仙代令以仮仰仲件"
	    U"任企伊伏伐休会伝伯估伴伸伺似但位低住佐体何余作佳使供依侍侮侯侵便係促"
	    U"俊俗保信修俳俵俸倉個候借倍倒値倣健側偉偏傍傑傘備催債傷傾僕僚像僧儀億"
	    U"儚儛允入全八公六兵典兼冊再冒冗冬冠冶准凄凡処凶凸凹刀刃分切刈刊刑列初"
	    U"判別利到刷刻則削前剖剛剣剤副剰割創劇力功加助努労効励勅勇勉動勤勧勲務"
	    U"勾包化升午卒卓協南単博占印危却卵厄厘厚原厳去参又及友双反収取叔受口古"
	    U"句叫召可台史右号司各合同名后吏吐向君吟否含吸吹吻吾告呈呉命周味呼和咲"
	    U"哀品哲唆唇唐唯唱嗚嘆噴";
	for (char32_t c : block1)
		if (c)
			v.push_back(static_cast<int>(c));

	static const char32_t block2[] =
	    U"囚図固国圏園囲土圧在地坂均坊坑坪垂型垣埋城域執培基堂堅堪報場塀塁塊塑"
	    U"塗塚塩填墓墜墨壁壇壊壌壊声壱売変夏夕外多夜夢大天太夫央失奇奈奉奏契奥"
	    U"奨奪奮女奴好如妙妥妨姉始姿威娘婆婚媒嫁嫌嫡子孔孤季孫学宅宇守安宗官宙"
	    U"定宜宝実家容宮宴宵害宿寂察寛寝寡寧審寮寺対寿封射将尊尋導小少尚尽尾尿"
	    U"局居屈届屋展属層履峠峡峰島崩崇嵐巡巻市布帆帝帥師帰帳幅幕幣干平幼庄床"
	    U"底庁度座庫庭廃廊延建弊式引弐弓弦弧弱張強弾当形彩影役征径待律後徐徒得"
	    U"御復循微必忙忠念怒怖思急恩息恵悔悟悲悼情惨惜想惰愁愚愛感慈態慌慢慣慮"
	    U"慰憂憎憤憩憶憾懸成我戦戒戯戸房所扇手才打払托扶批承技抗折抵押拡拝拠拡"
	    U"拷招拾持指捜捨掃採探接控推措描提握援搭摘撃操支放改攻救敗散敬数斎斗料"
	    U"斜斬断新方施旅旋旗既日旧早旬旺昇明昔映昭晩晴暇暖暗曇曜曲更書朗月有服"
	    U"朝期木末本札朴束条来松析枠枯架柔某染柱柳格桁桃案梅械棄棋棒棚森楼楽業"
	    U"極概構様模権横歓歌止正歩步歯歴";
	for (char32_t c : block2)
		if (c)
			v.push_back(static_cast<int>(c));

	static const char32_t block3[] =
	    U"汚江池決況泉泊泌泥波泰泳洗洞津浅浮浴海消涙涯液深混清渇済渡渦温港湖湯"
	    U"満源準溶滅滑演漁漠漢漫潔潮澄激瀬火灰災炉炊炎焦然煙燃爆爵父片版牛牧物"
	    U"特獄獣王珍現球理瓦産用田由男界留略番異疑痛発登白百的皆皮益盛盟盲相目"
	    U"省眼着瞬矛石砕砲破硬確磁礎示社祈祉票祖祝神票禁禍福禽私秀秋移程積穀穂"
	    U"穏空突窓竹笑筆筋策答等筒簡簿籍米粉粗粘粟精糖紀約紅納紙純紋組細終絵結"
	    U"絶絹継続維網緊総緑線編縁縦繁織繊縮缶置罪罰";
	for (char32_t c : block3)
		if (c)
			v.push_back(static_cast<int>(c));

	static const char32_t block4[] =
	    U"脅脆脱腐腸膚臓臣臨自至致興舌舎舗舞船良色芸芽花若苦茂苗茶草荒荷菊菌菜"
	    U"落著蒸蓄蔵薄薬藤行街衛衝衣表袋裁裂装裏複褒見規視覚観角解言訂計討訓記"
	    U"訪設許訳証評試詩話誇誠語誤説読課調談論諮議謝謡識警護豊貝負財貫責貸費"
	    U"賛賞賢賦購贈赤走超越趣足跡路踏躍身車軍軒転軽輝輸辛辞農近逃逆速造連週"
	    U"道達遠適選遺還郡部都里重野量金針釣録長間関附際雑難";
	for (char32_t c : block4)
		if (c)
			v.push_back(static_cast<int>(c));

	static const char32_t block5[] =
	    U"非面革音頂頭顔風飛食飲養騎験骨高鬼魚鳥鳴麦黄黒鼻齢";
	for (char32_t c : block5)
		if (c)
			v.push_back(static_cast<int>(c));

	static const char32_t punct[] = U"、。「」『』【】〔〕・〜…―";
	for (char32_t c : punct)
		if (c)
			v.push_back(static_cast<int>(c));

	for (int c = 0xFF01; c <= 0xFF5E; c++)
		v.push_back(c);

	static const char32_t block_addition[] = U"運";
	for (char32_t c : block_addition)
		if (c)
			v.push_back(static_cast<int>(c));

	return v;
}

std::vector<int> FontRenderer::Codepoints::Japanese() {
	auto v = Ascii();
	auto h = Hiragana();
	auto k = Katakana();
	auto j = JouyoKanji();
	v.insert(v.end(), h.begin(), h.end());
	v.insert(v.end(), k.begin(), k.end());
	v.insert(v.end(), j.begin(), j.end());
	return v;
}

void FontRenderer::FontInit(AtlasBuilder &b) {
	LoadFont(b, "default", "fonts/NotoSansJP-Regular.ttf", 48,
	         Codepoints::Japanese());
	b.build();
}
} // namespace zenithstgv
