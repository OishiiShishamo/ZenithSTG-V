#pragma once
#ifndef ZENITHSTGV_INCLUDE_VEC2D_H_
#define ZENITHSTGV_INCLUDE_VEC2D_H_

#include <array>
#include <cmath>
#include <emmintrin.h>

namespace zenithstgv {
struct ScreenSize {
	double w;
	double h;

	template <typename T, typename U>
	ScreenSize(T w, U h)
	    : w(static_cast<double>(w)), h(static_cast<double>(h)) {}
};

class alignas(16) Vec2D {
  public:
	Vec2D(double x = 0, double y = 0) { xy_ = _mm_set_pd(y, x); }

	explicit Vec2D(__m128d reg) : xy_(reg) {}

	Vec2D operator+(const Vec2D &rhs) const {
		return Vec2D(_mm_add_pd(xy_, rhs.xy_));
	}
	Vec2D operator-(const Vec2D &rhs) const {
		return Vec2D(_mm_sub_pd(xy_, rhs.xy_));
	}
	Vec2D operator*(double scalar) const {
		return Vec2D(_mm_mul_pd(xy_, _mm_set1_pd(scalar)));
	}
	Vec2D operator*(const Vec2D &rhs) const {
		return Vec2D(_mm_mul_pd(xy_, rhs.xy_));
	}
	Vec2D operator/(double scalar) const {
		return Vec2D(_mm_div_pd(xy_, _mm_set1_pd(scalar)));
	}
	Vec2D operator/(const Vec2D &rhs) const {
		return Vec2D(_mm_div_pd(xy_, rhs.xy_));
	}

	Vec2D &operator=(const Vec2D &rhs) {
		if (this != &rhs) {
			xy_ = rhs.xy_;
			return *this;
		}
		return *this;
	}
	Vec2D &operator+=(const Vec2D &rhs) {
		xy_ = _mm_add_pd(xy_, rhs.xy_);
		return *this;
	}
	Vec2D &operator-=(const Vec2D &rhs) {
		xy_ = _mm_sub_pd(xy_, rhs.xy_);
		return *this;
	}
	Vec2D &operator*=(double scalar) {
		xy_ = _mm_mul_pd(xy_, _mm_set1_pd(scalar));
		return *this;
	}
	Vec2D &operator*=(const Vec2D &rhs) {
		xy_ = _mm_mul_pd(xy_, rhs.xy_);
		return *this;
	}
	Vec2D &operator/=(double scalar) {
		xy_ = _mm_div_pd(xy_, _mm_set1_pd(scalar));
		return *this;
	}
	Vec2D &operator/=(const Vec2D &rhs) {
		xy_ = _mm_div_pd(xy_, rhs.xy_);
		return *this;
	}

	bool operator==(const Vec2D &rhs) const {
		__m128d cmp = _mm_cmpeq_pd(xy_, rhs.xy_);
		return _mm_movemask_pd(cmp) == 0b11;
	}
	bool operator!=(const Vec2D &rhs) const { return !(*this == rhs); }

	double GetX() const { return _mm_cvtsd_f64(xy_); }
	double GetY() const { return _mm_cvtsd_f64(_mm_unpackhi_pd(xy_, xy_)); }
	__m128d GetXY() const { return xy_; }

	void SetX(double x) {
		xy_ = _mm_unpacklo_pd(_mm_set_sd(x), _mm_unpackhi_pd(xy_, xy_));
	}
	void SetY(double y) {
		xy_ = _mm_unpacklo_pd(_mm_unpacklo_pd(xy_, xy_), _mm_set_sd(y));
	}
	void SetXY(double x, double y) { xy_ = _mm_set_pd(y, x); }
	void SetXY(__m128d xy) { xy_ = xy; }

	void Normalize() {
		__m128d mul = _mm_mul_pd(xy_, xy_);
		__m128d temp = _mm_unpackhi_pd(mul, mul);
		__m128d sum = _mm_add_pd(mul, temp);
		__m128d length = _mm_sqrt_pd(sum);

		double len = _mm_cvtsd_f64(length);
		if (len != 0.0) {
			xy_ = _mm_div_pd(xy_, _mm_set1_pd(len));
		}
	}

	[[nodiscard]] Vec2D ToNDC(const ScreenSize &s) const noexcept {
		__m128d inv_size = _mm_set_pd(1.0 / s.h, 1.0 / s.w);
		__m128d two = _mm_set1_pd(2.0);
		__m128d one = _mm_set1_pd(1.0);
		return Vec2D(
		    _mm_sub_pd(_mm_mul_pd(_mm_mul_pd(xy_, inv_size), two), one));
	}

	[[nodiscard]] Vec2D FromNDC(const ScreenSize &s) const noexcept {
		__m128d half_size = _mm_set_pd(s.h * 0.5, s.w * 0.5);
		__m128d one = _mm_set1_pd(1.0);
		return Vec2D(_mm_mul_pd(_mm_add_pd(xy_, one), half_size));
	}

	[[nodiscard]] Vec2D Clamp(const Vec2D &min_v,
	                          const Vec2D &max_v) const noexcept {
		return Vec2D(_mm_min_pd(_mm_max_pd(xy_, min_v.GetXY()), max_v.GetXY()));
	}

	[[nodiscard]] bool IsInScreen(const ScreenSize &s) const noexcept {
		__m128d lo = _mm_setzero_pd();
		__m128d hi = _mm_set_pd(s.h, s.w);
		int ge = _mm_movemask_pd(_mm_cmpge_pd(xy_, lo));
		int le = _mm_movemask_pd(_mm_cmple_pd(xy_, hi));
		return (ge & le) == 0b11;
	}

	[[nodiscard]] bool IsInScreen(const ScreenSize &s,
	                              double margin) const noexcept {
		__m128d lo = _mm_set1_pd(-margin);
		__m128d hi = _mm_set_pd(s.h + margin, s.w + margin);
		int ge = _mm_movemask_pd(_mm_cmpge_pd(xy_, lo));
		int le = _mm_movemask_pd(_mm_cmple_pd(xy_, hi));
		return (ge & le) == 0b11;
	}

  private:
	__m128d xy_;
};

double CrossProduct(const Vec2D &v1, const Vec2D &v2, const Vec2D &v3);
double Range(const Vec2D &v1, const Vec2D &v2);
double Vec2DToAngle(const Vec2D &v1, const Vec2D &v2);
Vec2D AngleToVec2D(double angle);
Vec2D RotatePoint(const Vec2D &pt, double angle);
} // namespace zenithstgv

#endif
