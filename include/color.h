#pragma once
#ifndef ZENITHSTG_SRC_COLOR_H_
#define ZENITHSTG_SRC_COLOR_H_

// SIMPLE

#define kColorRed 255, 0, 0
#define kColorGreen 0, 255, 0
#define kColorBlue 0, 0, 255
#define kColorYellow 255, 255, 0
#define kColorAqua 0, 255, 255
#define kColorCyan 0, 255, 255
#define kColorPurple 255, 0, 255
#define kColorOrange 255, 127, 0
#define kColorWhite 255, 255, 255
#define kColorGray 127, 127, 127
#define kColorBlack 0, 0, 0

// NOT-SIMPLE

#define kColorAliceBlue 240, 248, 255
#define kColorAntiqueWhite 250, 235, 215
#define kColorAquamarine 127, 255, 212
#define kColorAzure 240, 255, 255
#define kColorBeige 245, 245, 220
#define kColorBisque 255, 228, 196
#define kColorBlanchedalmond 255, 235, 205
#define kColorBlueViolet 138, 43, 226
#define kColorBrown 165, 42, 42
#define kColorBurlyWood 222, 184, 135

#include <algorithm>
#include <array>
#include <cmath>

#include <emmintrin.h>
#include <immintrin.h>
#include <pmmintrin.h>

namespace zenithstgv {
class alignas(16) Color {
  public:
	Color() noexcept : rgba_(_mm_set1_ps(1.0f)) {}

	Color(float r, float g, float b, float a = 1.0f) noexcept {
		rgba_ = _mm_setr_ps(r, g, b, a);
		Saturate();
	}

	explicit Color(__m128 rgba) noexcept : rgba_(rgba) { Saturate(); }

	float GetR255() const noexcept { return rgba_array()[0] * 255.0f; }
	float GetG255() const noexcept { return rgba_array()[1] * 255.0f; }
	float GetB255() const noexcept { return rgba_array()[2] * 255.0f; }
	float GetA255() const noexcept { return rgba_array()[3] * 255.0f; }
	float GetR() const noexcept { return _mm_cvtss_f32(rgba_); }
	float GetG() const noexcept {
		return _mm_cvtss_f32(
		    _mm_shuffle_ps(rgba_, rgba_, _MM_SHUFFLE(3, 2, 1, 1)));
	}
	float GetB() const noexcept {
		return _mm_cvtss_f32(
		    _mm_shuffle_ps(rgba_, rgba_, _MM_SHUFFLE(3, 2, 1, 2)));
	}
	float GetA() const noexcept {
		return _mm_cvtss_f32(
		    _mm_shuffle_ps(rgba_, rgba_, _MM_SHUFFLE(3, 2, 1, 3)));
	}

	__m128 Get() const noexcept { return rgba_; }

	void SetR(float r) noexcept {
		alignas(16) float tmp[4];
		_mm_store_ps(tmp, rgba_);
		tmp[0] = r;
		rgba_ = _mm_load_ps(tmp);
		Saturate();
	}
	void SetG(float g) noexcept {
		alignas(16) float tmp[4];
		_mm_store_ps(tmp, rgba_);
		tmp[1] = g;
		rgba_ = _mm_load_ps(tmp);
		Saturate();
	}
	void SetB(float b) noexcept {
		alignas(16) float tmp[4];
		_mm_store_ps(tmp, rgba_);
		tmp[2] = b;
		rgba_ = _mm_load_ps(tmp);
		Saturate();
	}
	void SetA(float a) noexcept {
		alignas(16) float tmp[4];
		_mm_store_ps(tmp, rgba_);
		tmp[3] = a;
		rgba_ = _mm_load_ps(tmp);
		Saturate();
	}
	void SetR255(float r) noexcept { SetR(r / 255.0f); }
	void SetG255(float g) noexcept { SetG(g / 255.0f); }
	void SetB255(float b) noexcept { SetB(b / 255.0f); }
	void SetA255(float a) noexcept { SetA(a / 255.0f); }

	void Set(float r, float g, float b, float a = 1.0f) noexcept {
		rgba_ = _mm_setr_ps(r, g, b, a);
		Saturate();
	}

	void Set255(float r, float g, float b, float a = 255.0f) noexcept {
		rgba_ = _mm_setr_ps(r, g, b, a);
		rgba_ = _mm_div_ps(rgba_, _mm_set1_ps(255.0f));
		Saturate();
	}

	Color operator+(const Color &rhs) const noexcept {
		return Color(_mm_add_ps(rgba_, rhs.rgba_));
	}
	Color operator+(float s) const noexcept {
		return Color(_mm_add_ps(rgba_, _mm_set1_ps(s)));
	}
	Color operator-(const Color &rhs) const noexcept {
		return Color(_mm_sub_ps(rgba_, rhs.rgba_));
	}
	Color operator-(float s) const noexcept {
		return Color(_mm_sub_ps(rgba_, _mm_set1_ps(s)));
	}
	Color operator*(const Color &rhs) const noexcept {
		return Color(_mm_mul_ps(rgba_, rhs.rgba_));
	}
	Color operator*(float s) const noexcept {
		return Color(_mm_mul_ps(rgba_, _mm_set1_ps(s)));
	}
	Color operator/(const Color &rhs) const noexcept {
		return Color(_mm_div_ps(rgba_, rhs.rgba_));
	}
	Color operator/(float s) const noexcept {
		return Color(_mm_div_ps(rgba_, _mm_set1_ps(s)));
	}

	Color &operator+=(const Color &rhs) noexcept {
		rgba_ = _mm_add_ps(rgba_, rhs.rgba_);
		Saturate();
		return *this;
	}
	Color &operator-=(const Color &rhs) noexcept {
		rgba_ = _mm_sub_ps(rgba_, rhs.rgba_);
		Saturate();
		return *this;
	}
	Color &operator*=(float s) noexcept {
		rgba_ = _mm_mul_ps(rgba_, _mm_set1_ps(s));
		Saturate();
		return *this;
	}

  private:
	__m128 rgba_;

	void Saturate() noexcept {
		rgba_ =
		    _mm_max_ps(_mm_min_ps(rgba_, _mm_set1_ps(1.0f)), _mm_set1_ps(0.0f));
	}

	inline std::array<float, 4> rgba_array() const noexcept {
		alignas(16) float tmp[4];
		_mm_store_ps(tmp, rgba_);
		return {tmp[0], tmp[1], tmp[2], tmp[3]};
	}
};

inline Color Color255(float r, float g, float b, float a = 255.0f) noexcept {
	return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

inline static Color Lerp(const Color &c1, const Color &c2, float t) noexcept {
	t = std::clamp(t, 0.0f, 1.0f);
	__m128 diff = _mm_sub_ps(c2.Get(), c1.Get());
	__m128 t_vec = _mm_set1_ps(t);
	return Color(_mm_add_ps(c1.Get(), _mm_mul_ps(diff, t_vec)));
}

Color GetColorHsv(float h, float s, float v);

Color GamingColor(long long t, long long offset = 0, float mul = 1.0);
} // namespace zenithstgv

#endif
