#include "LEDController.h"

LEDController* LEDController::GetInstance() {
	if (instance == nullptr) {
			instance = new LEDController();
	}
	return instance;
}

LEDController::LEDController() {
	auto pref = Preferences::GetInstance();
	std::string port = pref->GetString("leds.port", "onboard");
	int count = pref->GetInt("leds.count", 255);
	int addr = pref->GetInt("leds.addr", 67);
	controller = new I2C((port == "mxp" ? I2C::kMXP : I2C::kOnboard), addr);
	printf("[LEDController] Created controller to addr %d on port %s for %d LEDs\n", addr, port.c_str(), count);
	leds.resize(count);
}

void LEDController::SetBrightness(uint8_t newBrightness) {
	if (brightness != newBrightness) {
		uint8_t buf[2];
		buf[0] = 'b';
		buf[1] = newBrightness;
		Write(buf, 2);
		brightness = newBrightness;
	}
}

uint8_t LEDController::GetBrightness() {
	return brightness;
}

void LEDController::Set(Util::Color led, int idx, bool show /*= false*/) {
	std::vector<Util::Color> tmp;
	tmp.push_back(led);
	Set(tmp, idx, show);
}

Util::Color LEDController::Get(int idx) {
	return leds[idx];
}

void LEDController::Set(std::vector<Util::Color> values, int idx /*= 0*/, bool show /*= false*/) {
	uint8_t buf[3];
	buf[0] = 'l';
	buf[1] = idx;
	buf[2] = values.size();
	Write(buf, 3);
	//for (auto led = values.begin(); led != values.end(); led++) {
	for (uint16_t i = 0; i < values.size(); i++) {
		Util::RGB led = values[i];
		leds[idx + i] = led;
		buf[0] = led.r;
		buf[1] = led.g;
		buf[2] = led.b;
		Write(buf, 3);
	}
}

void LEDController::Clear() {
	uint8_t buf[1];
	buf[0] = 'c';
	Write(buf, 1);
}

void LEDController::Show(bool refresh /*= false*/) {
	if (refresh) {
		Refresh();
	}
	uint8_t buf[1];
	buf[0] = 's';
	Write(buf, 1);
}

void LEDController::Refresh() {
	Clear();
	Set(leds, 0, true); // Set all LEDs to themselves
	Show();
}

void LEDController::Write(uint8_t* buf, int length) {
	//printf("Writing datas: ");
	//for (int i = 0; i < length; i++) {
		//printf("%02x ", buf[i]);
	//}
	//printf("\n");
	controller->WriteBulk(buf, length);
}

LEDController* LEDController::instance = nullptr;
