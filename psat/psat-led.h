#ifndef PSAT_LED_H
#define PSAT_LED_H

#include <FastLED.h>

namespace psat {

class LedModule {
	CRGBPalette16 currentPalette;
	TBlendType currentBlending;
	CRGB leds[20];

public:
	void setup();
	void showLEDs();
	void setLED(int mode);
};

}

#endif