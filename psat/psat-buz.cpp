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

	constexpr int NOTE_B0 = 31;
	constexpr int NOTE_C1  = 33;
	constexpr int NOTE_CS1 = 35;
	constexpr int NOTE_D1  = 37;
	constexpr int NOTE_DS1 = 39;
	constexpr int NOTE_E1  = 41;
	constexpr int NOTE_F1  = 44;
	constexpr int NOTE_FS1 = 46;
	constexpr int NOTE_G1  = 49;
	constexpr int NOTE_GS1 = 52;
	constexpr int NOTE_A1  = 55;
	constexpr int NOTE_AS1 = 58;
	constexpr int NOTE_B1  = 62;
	constexpr int NOTE_C2  = 65;
	constexpr int NOTE_CS2 = 69;
	constexpr int NOTE_D2  = 73;
	constexpr int NOTE_DS2 = 78;
	constexpr int NOTE_E2  = 82;
	constexpr int NOTE_F2  = 87;
	constexpr int NOTE_FS2 = 93;
	constexpr int NOTE_G2  = 98;
	constexpr int NOTE_GS2 = 104;
	constexpr int NOTE_A2  = 110;
	constexpr int NOTE_AS2 = 117;
	constexpr int NOTE_B2  = 123;
	constexpr int NOTE_C3  = 131;
	constexpr int NOTE_CS3 = 139;
	constexpr int NOTE_D3  = 147;
	constexpr int NOTE_DS3 = 156;
	constexpr int NOTE_E3  = 165;
	constexpr int NOTE_F3  = 175;
	constexpr int NOTE_FS3 = 185;
	constexpr int NOTE_G3  = 196;
	constexpr int NOTE_GS3 = 208;
	constexpr int NOTE_A3  = 220;
	constexpr int NOTE_AS3 = 233;
	constexpr int NOTE_B3  = 247;
	constexpr int NOTE_C4  = 262;
	constexpr int NOTE_CS4 = 277;
	constexpr int NOTE_D4  = 294;
	constexpr int NOTE_DS4 = 311;
	constexpr int NOTE_E4  = 330;
	constexpr int NOTE_F4  = 349;
	constexpr int NOTE_FS4 = 370;
	constexpr int NOTE_G4  = 392;
	constexpr int NOTE_GS4 = 415;
	constexpr int NOTE_A4  = 440;
	constexpr int NOTE_AS4 = 466;
	constexpr int NOTE_B4  = 494;
	constexpr int NOTE_C5  = 523;
	constexpr int NOTE_CS5 = 554;
	constexpr int NOTE_D5  = 587;
	constexpr int NOTE_DS5 = 622;
	constexpr int NOTE_E5  = 659;
	constexpr int NOTE_F5  = 698;
	constexpr int NOTE_FS5 = 740;
	constexpr int NOTE_G5  = 784;
	constexpr int NOTE_GS5 = 831;
	constexpr int NOTE_A5  = 880;
	constexpr int NOTE_AS5 = 932;
	constexpr int NOTE_B5  = 988;
	constexpr int NOTE_C6  = 1047;
	constexpr int NOTE_CS6 = 1109;
	constexpr int NOTE_D6  = 1175;
	constexpr int NOTE_DS6 = 1245;
	constexpr int NOTE_E6  = 1319;
	constexpr int NOTE_F6  = 1397;
	constexpr int NOTE_FS6 = 1480;
	constexpr int NOTE_G6  = 1568;
	constexpr int NOTE_GS6 = 1661;
	constexpr int NOTE_A6  = 1760;
	constexpr int NOTE_AS6 = 1865;
	constexpr int NOTE_B6  = 1976;
	constexpr int NOTE_C7  = 2093;
	constexpr int NOTE_CS7 = 2217;
	constexpr int NOTE_D7  = 2349;
	constexpr int NOTE_DS7 = 2489;
	constexpr int NOTE_E7  = 2637;
	constexpr int NOTE_F7  = 2794;
	constexpr int NOTE_FS7 = 2960;
	constexpr int NOTE_G7  = 3136;
	constexpr int NOTE_GS7 = 3322;
	constexpr int NOTE_A7  = 3520;
	constexpr int NOTE_AS7 = 3729;
	constexpr int NOTE_B7  = 3951;
	constexpr int NOTE_C8  = 4186;
	constexpr int NOTE_CS8 = 4435;
	constexpr int NOTE_D8  = 4699;
	constexpr int NOTE_DS8 = 4978;
	constexpr int REST      = 0;

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

	constexpr int rick_roll_verse[][2] = {
	{NOTE_D5, 900}, {NOTE_E5,900}, {NOTE_A4,600}, {NOTE_E5,900}, {NOTE_FS5,900}, {NOTE_A5, 150}, {NOTE_G5, 150}, {NOTE_FS5,300}, {NOTE_D5,900}, {NOTE_E5,900}, 
	{NOTE_A4, 1200}, {NOTE_A4, 150}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 300}, {NOTE_D5,150}, {NOTE_D5,900}, {NOTE_E5,900}, {NOTE_A4,600}, {NOTE_E5,900}, {NOTE_FS5,900}, {NOTE_A5, 150}, {NOTE_G5, 150}, 
	{NOTE_FS5, 300}, {NOTE_D5,900}, {NOTE_E5,900}, {NOTE_A4,1200}, {NOTE_A4, 150}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 300}, {NOTE_D5,150}, {REST, 600}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, 
	{NOTE_D5, 300}, {NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4,1200}, {REST,600}, {REST, 300}, {NOTE_B4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4, 600}, {NOTE_A4,300}, 
	{NOTE_A5, 300}, {REST, 300}, {NOTE_A5, 300}, {NOTE_E5,900}, {REST,600}, {NOTE_B4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4, 300}, {NOTE_D5, 300}, {NOTE_E5, 300}, {REST,300}, {REST, 300}, 
	{NOTE_CS5, 300}, {NOTE_B4, 300}, {NOTE_A4,900}, {REST,600}, {REST, 300}, {NOTE_B4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4, 300}, {NOTE_A4,600}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_E5, 300}, 
	{NOTE_FS5, 300}, {NOTE_E5, 600}, {REST,600}, {NOTE_D5,1200}, {NOTE_E5, 300}, {NOTE_FS5, 300}, {NOTE_D5,300}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_FS5, 300}, {NOTE_E5, 600}, {NOTE_A4,600}, {REST,1200}, 
	{NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4,300}, {REST, 300}, {NOTE_E5, 300}, {NOTE_FS5, 300}, {NOTE_E5,900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_FS5, 450}, {NOTE_FS5, 450}, {NOTE_E5, 900},
	{NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_E5,450}, {NOTE_E5,450}, {NOTE_D5,450}, {NOTE_CS5, 150}, {NOTE_B4,450}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_D5, 600}, 
	{NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 300}, {NOTE_A4, 300}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_FS5,450}, {NOTE_FS5,450}, {NOTE_E5,900}, 
	{NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_A5, 600}, {NOTE_CS5, 300}, {NOTE_D5,450}, {NOTE_CS5, 150}, {NOTE_B4, 300}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_D5, 600}, 
	{NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 600}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, {REST,600}, {REST, 300}, {NOTE_B4, 300}, {NOTE_D5, 300}, {NOTE_B4, 300}, {NOTE_D5, 300}, {NOTE_E5, 600}, {REST,300}, 
	{REST, 300}, {NOTE_CS5, 300}, {NOTE_B4, 300}, {NOTE_A4,900}, {REST,600}, {REST, 300}, {NOTE_B4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4, 300}, {NOTE_A4,600}, {REST, 300}, {NOTE_A5, 300}, {NOTE_A5, 300}, 
	{NOTE_E5, 300}, {NOTE_FS5, 300}, {NOTE_E5, 300}, {NOTE_D5,300}, {REST, 300}, {NOTE_A4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4,300}, {REST, 300}, {NOTE_CS5, 300}, {NOTE_B4, 300}, {NOTE_A4,900}, {REST,600}, 
	{NOTE_B4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4, 300}, {NOTE_A4, 600}, {REST,300}, {REST, 300}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_FS5, 600}, {NOTE_E5,900}, {NOTE_D5,1200}, {NOTE_D5, 300}, {NOTE_E5, 300}, 
	{NOTE_FS5, 300}, {NOTE_E5,600}, {NOTE_E5, 300}, {NOTE_E5, 300}, {NOTE_FS5, 300}, {NOTE_E5, 300}, {NOTE_A4, 300}, {NOTE_A4,600}, {REST,900}, {NOTE_A4, 300}, {NOTE_B4, 300}, {NOTE_CS5, 300}, {NOTE_D5, 300}, {NOTE_B4,300}, {REST, 300}, {NOTE_E5, 300}, 
	{NOTE_FS5, 300}, {NOTE_E5,900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_FS5,450}, {NOTE_FS5,450}, {NOTE_E5,900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_E5,450}, {NOTE_E5,450}, 
	{NOTE_D5,450}, {NOTE_CS5, 150}, {NOTE_B4, 300}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_D5, 600}, {NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 600}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, 
	{NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_FS5,450}, {NOTE_FS5,450}, {NOTE_E5,900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_A5, 600}, {NOTE_CS5, 300}, {NOTE_D5,450}, {NOTE_CS5, 150}, 
	{NOTE_B4, 300}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_D5, 600}, {NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 600}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, {NOTE_A4, 150}, {NOTE_B4, 150}, 
	{NOTE_D5, 150}, {NOTE_B4, 150}, {NOTE_FS5, 450}, {NOTE_FS5,450}, {NOTE_E5,900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, {NOTE_A5, 600}, {NOTE_CS5, 300}, {NOTE_D5,450}, {NOTE_CS5, 150}, {NOTE_B4, 300}, {NOTE_A4, 150}, 
	{NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4, 150}, {NOTE_D5, 600}, {NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 600}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,150}, 
	{NOTE_FS5,450}, {NOTE_FS5, 450}, {NOTE_E5, 900}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, {NOTE_B4,1}, {NOTE_A5, 600}, {NOTE_CS5, 300}, {NOTE_D5,450}, {NOTE_CS5, 150}, {NOTE_B4, 300}, {NOTE_A4, 150}, {NOTE_B4, 150}, {NOTE_D5, 150}, 
	{NOTE_B4, 150}, {NOTE_D5, 600}, {NOTE_E5, 300}, {NOTE_CS5,450}, {NOTE_B4, 150}, {NOTE_A4, 600}, {NOTE_A4,300}, {NOTE_E5, 600}, {NOTE_D5,1200}, {REST,600}
	};
	constexpr int rick_roll_length = sizeof(rick_roll_verse) / sizeof(rick_roll_verse[0]);
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
	case 3:
		music = rick_roll_verse;
		num_notes = rick_roll_length;
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
