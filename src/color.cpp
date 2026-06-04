#include "color.h"

namespace zenithstgv {
Color GetColorHsv(float h, float s, float v) {
	h = std::fmod(h, 360.0f);
	if (h < 0.0f)
		h += 360.0f;

	float c = v * s;
	float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
	float m = v - c;

	float r, g, b;

	if (h < 60.0f) {
		r = c;
		g = x;
		b = 0.0f;
	} else if (h < 120.0f) {
		r = x;
		g = c;
		b = 0.0f;
	} else if (h < 180.0f) {
		r = 0.0f;
		g = c;
		b = x;
	} else if (h < 240.0f) {
		r = 0.0f;
		g = x;
		b = c;
	} else if (h < 300.0f) {
		r = x;
		g = 0.0f;
		b = c;
	} else {
		r = c;
		g = 0.0f;
		b = x;
	}

	return Color(r + m, g + m, b + m);
}

Color GamingColor(long long t, long long offset, float mul) {
	return GetColorHsv(std::fmod((t + offset) * mul, 360), 1, 1);
}
} // namespace zenithstgv
