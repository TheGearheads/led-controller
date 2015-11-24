#include "FastLED.h"
#include "Wire.h"

#define NUM_LEDS 255
#define DATA_PIN 2
#define BUFSIZE 255

/*#define USE_SERIAL*/

CRGB leds[NUM_LEDS];

char buffer[BUFSIZE];
int readIdx = 0;
int writeIdx = 0;

enum ModeEnum { kRGB, kHSV };
ModeEnum mode = kRGB;

void eatBuf(int i) {
	readIdx = (readIdx + i) % BUFSIZE;
}

char getBuf(int i) {
	return buffer[(readIdx + i) % BUFSIZE];
}

void setup() {

#ifdef USE_SERIAL
	Serial.begin(9600);
	delay(1000);
	Serial.println("Hallo");
#else
	Wire.begin(167);
	/*Wire.onReceive(handler);*/
#endif

	FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
	FastLED.setBrightness(100);
}

void loop() {
	int count = 0;
	int idx = 0;
	int offset = 0;
	while (1) {

#ifdef USE_SERIAL
		while(Serial.available()) {
			char c = Serial.read();
#else
		while(Wire.available()) {
			char c = Wire.read();
#endif
			buffer[writeIdx] = c;
			writeIdx = (writeIdx + 1) % BUFSIZE;
		}

		int bufferSize = ((writeIdx < readIdx) ? (writeIdx + BUFSIZE) : writeIdx) - readIdx;

		if (count > 0) {
			if (bufferSize >= 3) {
#ifdef USE_SERIAL
				Serial.print("Setting LED to: ");
				Serial.print(getBuf(0), DEC);
				Serial.print(", ");
				Serial.print(getBuf(1), DEC);
				Serial.print(", ");
				Serial.print(getBuf(2), DEC);
				Serial.println();
#endif
				if (mode == kRGB) {
					leds[idx + offset].setRGB(getBuf(0), getBuf(1), getBuf(2));
				} else {
					leds[idx + offset].setHSV(getBuf(0), getBuf(1), getBuf(2));
				}
				offset++;
				if (offset == count) {
					offset = idx = count = 0;
				}
				eatBuf(3);
			}
		} else if (bufferSize > 0){
			switch (getBuf(0)) {
				case 'l':
					if (bufferSize >= 3) {
						idx = getBuf(1);
						count = getBuf(2);
#ifdef USE_SERIAL
						Serial.print("Setting ");
						Serial.print(count, DEC);
						Serial.print(" LEDs starting at ");
						Serial.print(idx, DEC);
						Serial.println();
#endif
						eatBuf(3);
					}
					break;
				case 'r':
#ifdef USE_SERIAL
					Serial.println("Setting mode to RGB");
#endif
					mode = kRGB;
					eatBuf(1);
					break;
				case 'h':
#ifdef USE_SERIAL
					Serial.println("Setting mode to HSV");
#endif
					mode = kHSV;
					eatBuf(1);
					break;
				case 'c':
#ifdef USE_SERIAL
					Serial.println("Clearing LEDs");
#endif
					FastLED.clear();
					eatBuf(1);
					break;
				case 's':
#ifdef USE_SERIAL
					Serial.println("Showing LEDs");
#endif
					FastLED.show();
					eatBuf(1);
					break;
				case 'b':
					if (bufferSize >= 2) {
#ifdef USE_SERIAL
						Serial.print("Setting brightness to: ");
						Serial.println(getBuf(1), DEC);
#endif
						FastLED.setBrightness(getBuf(1));
						eatBuf(2);
					}
					break;
				default:
#ifdef USE_SERIAL
					Serial.print("Read in junk: ");
					Serial.println(getBuf(0), DEC);
#endif
					eatBuf(1);
			}
		}
	}
}
