#include "vec2d.h"

#include <cmath>

#include "global.h"

namespace zenithstgv {
	double CrossProduct(const Vec2D& v1, const Vec2D& v2, const Vec2D& v3) {
		__m128d v1_xy = v1.GetXY();
		__m128d v2_xy = v2.GetXY();
		__m128d v3_xy = v3.GetXY();

		__m128d diff1 = _mm_sub_pd(v2_xy, v1_xy);
		__m128d diff2 = _mm_sub_pd(v3_xy, v1_xy);

		__m128d mul1 = _mm_mul_pd(diff1, _mm_shuffle_pd(diff2, diff2, 0b01));
		__m128d result = _mm_sub_pd(mul1, _mm_shuffle_pd(mul1, mul1, 0b01));

		alignas(16) std::array<double, 2> res;
		_mm_store_pd(res.data(), result);
		return res[0];
	}

	double Range(const Vec2D& v1, const Vec2D& v2) {
		__m128d v1_xy = v1.GetXY();
		__m128d v2_xy = v2.GetXY();

		__m128d diff = _mm_sub_pd(v1_xy, v2_xy);

		__m128d sqr = _mm_mul_pd(diff, diff);

		__m128d sum = _mm_add_pd(sqr, _mm_shuffle_pd(sqr, sqr, 1));

		__m128d length = _mm_sqrt_pd(sum);

		alignas(16) std::array<double, 2> len;
		_mm_store_pd(len.data(), length);
		return len[0];
	}

	double Vec2DToAngle(const Vec2D& v1, const Vec2D& v2) {
		return std::atan2(-v1.GetY() - v2.GetY(), v1.GetX() - v2.GetX());
	}

	Vec2D AngleToVec2D(double angle) {
		return {
			std::cos(angle),
			-std::sin(angle)
		};
	}

	Vec2D RotatePoint(const Vec2D& pt, double angle) {
		double cos_a = std::cos(angle);
		double sin_a = -std::sin(angle);

		__m128d vec = pt.GetXY();
		__m128d rot = _mm_set_pd(cos_a, cos_a);

		__m128d xy_cos = _mm_mul_pd(vec, rot);

		__m128d sin_vec = _mm_set_pd(sin_a, -sin_a);
		__m128d swapped = _mm_shuffle_pd(vec, vec, 0b01);

		__m128d xy_sin = _mm_mul_pd(swapped, sin_vec);

		__m128d result = _mm_add_pd(xy_cos, xy_sin);

		return Vec2D(result);
	}
}
