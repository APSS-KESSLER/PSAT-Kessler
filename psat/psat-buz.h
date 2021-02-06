#ifndef PSAT_BUZ_H
#define PSAT_BUZ_H

#include <Arduino.h>

namespace psat {

class BuzModule {
	int num_notes;
	int const (*music) [2];

	int note_index = 0;
	int interval = 0;
	int previous_millis = 0;
	bool buzzer_on = false;

public:
	void setup();
	void loop();
	void selectSong(int song);
};

}

#endif
