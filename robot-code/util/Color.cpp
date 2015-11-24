#include "Color.h"
//#include <stdio.h>

namespace Util {

Color::Color() {
	raw = 0;
}

Color::Color(HSV newColor) {
	FromHSV(newColor);
}

Color::Color(float h, float s, float v) {
	FromHSV(h, s, v);
}

Color::Color(RGB newColor) {
	FromRGB(newColor);
}

Color::Color(uint8_t r, uint8_t g, uint8_t b) {
	FromRGB(r, g, b);
}

Color::Color(int newColor) {
	raw = newColor;
}

void Color::FromHSV(HSV newColor) {
	FromHSV(newColor.h, newColor.s, newColor.v);
}

void Color::FromHSV(float h, float s, float v) {
	h = fmod(h, 360);
	float chroma = v * s;
	float hPrime = h / 60.0;
	float X = chroma * (1.0 - fabs(fmod(hPrime, 2) - 1));

	float r1, g1, b1;

	if (0 <= hPrime && hPrime < 1) {
		r1 = chroma;
		g1 = X;
		b1 = 0;
	} else if (1 <= hPrime && hPrime < 2) {
		r1 = X;
		g1 = chroma;
		b1 = 0;
	} else if (2 <= hPrime && hPrime < 3) {
		r1 = 0;
		g1 = chroma;
		b1 = X;
	} else if (3 <= hPrime && hPrime < 4) {
		r1 = 0;
		g1 = X;
		b1 = chroma;
	} else if (4 <= hPrime && hPrime < 5) {
		r1 = X;
		g1 = 0;
		b1 = chroma;
	} else if (5 <= hPrime && hPrime < 6) {
		r1 = chroma;
		g1 = 0;
		b1 = X;
	}

	float m = v - chroma;
	FromRGB(255 * (r1 + m), 255 * (g1 + m), 255 * (b1 + m));
}

void Color::FromRGB(RGB newColor) {
	FromRGB(newColor.r, newColor.g, newColor.b);
}

void Color::FromRGB(uint8_t r, uint8_t g, uint8_t b) {
	raw = r << 16 | g << 8 | b;
}

HSV Color::ToHSV() {
	RGB color = ToRGB();
	float r = (float)(color.r) / 255;
	float g = (float)(color.g) / 255;
	float b = (float)(color.b) / 255;
	float M = fmax(r, fmax(g, b));
	float m = fmin(r, fmin(g, b));
	float C = M - m;
	float hPrime;
	HSV tmp;

	tmp.v = M;

	if (M == 0) {
		tmp.s = 0;
		tmp.h = -1;
		return tmp;
	} else {
		tmp.s = C / M;
	}

	if (C == 0) { // Undefined, but we set to 0
		hPrime = 0;
	} else if (M == r) {
		hPrime = (g - b) / C;
	} else if (M == g) {
		hPrime = (b - r) / C + 2;
	} else if (M == b) {
		hPrime = (r - g) / C + 4;
	}
	tmp.h = fmod(hPrime * 60 + 360, 360);

	return tmp;
}

RGB Color::ToRGB() {
	RGB tmp;
	tmp.r = raw >> 16;
	tmp.g = (raw >> 8) & 0xff;
	tmp.b = raw & 0xff;
	return tmp;
}

}
