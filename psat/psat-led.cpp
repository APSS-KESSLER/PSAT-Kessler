#include "psat-led.h"

namespace {

constexpr int ledPin = 9;

}

namespace psat {

void LedModule::setup() {
	FastLED.addLeds<WS2811, ledPin, GRB>(leds, 20).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(64);

	setLED(0);
}

void LedModule::showLEDs() {
	static uint8_t startIndex = 0;
	startIndex = startIndex + 1;

	uint8_t brightness = 255;
	uint8_t colourIndex = startIndex;

	for( int i = 0; i < 20; ++i) {
		leds[i] = ColorFromPalette(currentPalette, colourIndex, brightness, currentBlending);
		colourIndex += 3;
    }

	FastLED.show();
}

void LedModule::setLED(int mode) {
	switch(mode) {
	case 0:
		currentPalette = RainbowStripeColors_p;
		currentBlending = LINEARBLEND;
		break;
	case 1:
		fill_solid(currentPalette, 16, CRGB::Black);
		currentPalette[0] = CRGB::White;
		currentPalette[4] = CRGB::White;
		currentPalette[8] = CRGB::White;
		currentPalette[12] = CRGB::White;
		currentBlending = LINEARBLEND;
		break;
	}
}

}