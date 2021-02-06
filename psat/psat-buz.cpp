#include "psat-buz.h"
#include "psat-log.h"

/* How to use:
* - Initialise with setup()
* - In main PSat loop, run buzLoop()
* - Should magically know when and how to play
*/

namespace {
	constexpr int buzzer_pin = 6;

	constexpr int all_star_verse_1[][2] = {
		{370, 600}, 
		{554, 300}, 
		{466, 300}, 
		{466, 600}, 
		{415, 300}, 
		{370, 300}, 
		{370, 300}, 
		{494, 600}, 
		{466, 300}, 
		{466, 300}, 
		{415, 300},
		{415, 300},
		{370, 600},
		{370, 300},
		{554, 300},
		{466, 300},
		{466, 600},
		{415, 300},
		{370, 300},
		{370, 300},
		{311, 600},
		{277, 900},
		{370, 300},
		{370, 300},
		{554, 300},
		{0, 500}
	};

	constexpr int all_star_length = sizeof(all_star_verse_1) / sizeof(all_star_verse_1[0]);
}

namespace psat {

void BuzModule::selectSong(int song) {
	buzzer_on = true;

	switch (song) {
	case 0:
		buzzer_on = false;
		break;
	case 1:
		music = all_star_verse_1;
		num_notes = all_star_length;
		break;
	case 2:
		break;
	}

	buzzer_on = true;
	note_index = 0;
}

void BuzModule::setup() {
}

void BuzModule::loop() {
	if (!buzzer_on){
		return;
	}

	int current_millis = millis();
	if ((current_millis - interval) > previous_millis) {
		previous_millis = millis();
		note_index++;
		if (note_index >= num_notes){
			note_index = 0;
		}
		
		interval = all_star_verse_1[note_index][1];
		tone(buzzer_pin, all_star_verse_1[note_index][0], interval);
	}
}

}
