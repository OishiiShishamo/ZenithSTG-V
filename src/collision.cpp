#include "collision.h"

#include <array>
#include <cmath>

#include "global.h"
#include "vec2d.h"

namespace zenithstgv {
int ColPointAndCircle(const Vec2D &v1, const Vec2D &v2, double r) {
	Vec2D d = v1 - v2;
	return d.GetX() * d.GetX() + d.GetY() * d.GetY() <= r * r;
}

int ColPointAndTriangle(const Vec2D &v, const Vec2D &v1, const Vec2D &v2,
                        const Vec2D &v3) {
	double cross1 = CrossProduct(v1, v2, v);
	double cross2 = CrossProduct(v2, v3, v);
	double cross3 = CrossProduct(v3, v1, v);

	return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0) ||
	       (cross1 <= 0 && cross2 <= 0 && cross3 <= 0);
}

int ColPointAndRect(const Vec2D &v, const Vec2D &v1, const Vec2D &v2,
                    const Vec2D &v3, const Vec2D &v4) {
	double cross1 = CrossProduct(v1, v2, v);
	double cross2 = CrossProduct(v2, v3, v);
	double cross3 = CrossProduct(v3, v4, v);
	double cross4 = CrossProduct(v4, v1, v);

	return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0 && cross4 >= 0) ||
	       (cross1 <= 0 && cross2 <= 0 && cross3 <= 0 && cross4 <= 0);
}

int ColPointAndCircleAdv(const Vec2D &v1, const Vec2D &v2, const Vec2D &v3,
                         double r) {
	if (ColPointAndCircle(v1, v2, r) || ColPointAndCircle(v1, v3, r))
		return 1;
	double rad = Vec2DToAngle(v2, v3);
	return ColPointAndRect(v1, v2 + AngleToVec2D(rad + kPi / 2) * r,
	                       v2 + AngleToVec2D(rad - kPi / 2) * r,
	                       v3 + AngleToVec2D(rad - kPi / 2) * r,
	                       v3 + AngleToVec2D(rad + kPi / 2) * r);
}
} // namespace zenithstgv
