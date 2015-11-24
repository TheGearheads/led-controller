#include "LEDStrip.h"

LEDStrip::LEDStrip(int idx, int count) : start_idx(idx), count(count) { }

void LEDStrip::Set(Util::Color led, int idx, bool show /*= false*/) {
	LEDController::GetInstance()->Set(led, start_idx + idx, show);
}

void LEDStrip::Set(std::vector<Util::Color> values, int idx /*= 0*/, bool show /*= false*/) {
	LEDController::GetInstance()->Set(values, start_idx + idx, show);
}

Util::Color LEDStrip::Get(int idx) {
	return LEDController::GetInstance()->Get(start_idx + idx);
}

void LEDStrip::Clear() {
	std::vector<Util::Color> tmp;
	tmp.resize(count);
	Set(tmp, start_idx, true);
}

void LEDStrip::Show() {
	LEDController::GetInstance()->Show();
}

