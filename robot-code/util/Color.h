#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include <stdint.h>
#include <math.h>

namespace Util {

struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct HSV {
	float h;
	float s;
	float v;
};

class Color {
	public:
		Color();
		Color(HSV newColor);
		Color(float h, float s, float v);
		Color(RGB newColor);
		Color(uint8_t r, uint8_t g, uint8_t b);
		Color(int newColor);
		void FromHSV(HSV newColor);
		void FromHSV(float h, float s, float v);
		void FromRGB(RGB newColor);
		void FromRGB(uint8_t r, uint8_t g, uint8_t b);
		HSV ToHSV();
		RGB ToRGB();
		operator HSV() { return ToHSV(); }
		operator RGB() { return ToRGB(); }
		operator int() { return raw; }
	private:
		uint32_t raw;
};

}

#endif /* SRC_COLOR_H_ */
