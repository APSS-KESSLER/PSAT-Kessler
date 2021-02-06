#include "psat-buz.h"
#include "psat-log.h"

/* How to use:
* - Initialise with setup()
* - In main PSat loop, run buzLoop()
* - Should magically know when and how to play
*/

namespace {
	constexpr int buzzer_pin = 6;

	constexpr int c = 261;
	constexpr int d = 294;
	constexpr int e = 329;
	constexpr int f = 349;
	constexpr int g = 391;
	constexpr int gS = 415;
	constexpr int a = 440;
	constexpr int aS = 455;
	constexpr int b = 466;
	constexpr int cH = 523;
	constexpr int cSH = 554;
	constexpr int dH = 587;
	constexpr int dSH = 622;
	constexpr int eH = 659;
	constexpr int fH = 698;
	constexpr int fSH = 740;
	constexpr int gH = 784;
	constexpr int gSH = 830;
	constexpr int aH = 880;

	constexpr int all_star_verse[][2] = {
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

	constexpr int all_star_length = sizeof(all_star_verse) / sizeof(all_star_verse[0]);

	constexpr int star_wars_verse[][2] = {
		{a, 500}, {a, 500}, {a, 500}, {f, 350}, {cH, 150}, {a, 500}, {f, 350}, {cH, 150}, {a, 650}, 
    	{0,500},
    	{eH, 500}, {eH, 500}, {eH, 500}, {fH, 350}, {cH, 150}, {gS, 500}, {f, 350}, {cH, 150}, {a, 650},
    	{0, 500},
    	{aH, 500}, {a, 300}, {a, 150}, {aH, 500}, {gSH, 325}, {gH, 175}, {fSH, 125}, {fH, 125}, {fSH, 250}, 
    	{0, 325},
    	{aS, 250}, {dSH, 500}, {dH, 325}, {cSH, 175}, {cH, 125}, {b, 125}, {cH, 250}, 
    	{0, 350},
      	{f, 250}, {gS, 500}, {f, 350}, {a, 125}, {cH, 500}, {a, 375}, {cH, 125}, {eH, 650},
    	{aH, 500}, {a, 300}, {a, 150}, {aH, 500}, {gSH, 325}, {gH, 175}, {fSH, 125}, {fH, 125}, {fSH, 250}, 
    	{0, 325},
    	{aS, 250}, {dSH, 500}, {dH, 325}, {cSH, 175}, {cH, 125}, {b, 125}, {cH, 250}, 
    	{0, 350},
    	{f, 250}, {gS, 500}, {f, 375}, {cH, 125}, {a, 500}, {f, 375}, {cH, 125}, {a, 650},
    	{0, 650}
	};
	
	constexpr int star_wars_length = sizeof(star_wars_verse) / sizeof(star_wars_verse[0]);
}

namespace psat {

void BuzModule::selectSong(int song) {
	switch (song) {
	case 0:
		buzzer_on = false;
		return;
	case 1:
		music = all_star_verse;
		num_notes = all_star_length;
		break;
	case 2:
		music = star_wars_verse;
		num_notes = star_wars_length;
		break;
	}

	buzzer_on = true;
	note_index = 0;
  
	previous_millis = millis();
	note_index++;
	if (note_index >= num_notes){
		note_index = 0;
	}

	tone(buzzer_pin, music[0][0], music[0][1]);
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
		
		interval = music[note_index][1];
		tone(buzzer_pin, music[note_index][0], interval);
	}
}

}
