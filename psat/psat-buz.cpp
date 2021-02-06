#include "psat-buz.h"
#include "psat-log.h"

#define buzzer_pin 10

/* How to use:
* - Initialise with setup()
* - In main PSat loop, run buzLoop()
* - Should magically know when and how to play
*/

namespace {
  int all_star_verse_1[][2] = {{370,600},{554,300},{466,300},{466,600},{415,300},{370,300},{370,300},{494,600},{466,300},{466,300} ,{415,300} ,
{415,300} ,{370,600} ,{370,300} ,{554,300} ,{466,300} ,{466,600} ,{415,300} ,{370,300} ,{370,300} ,{311,600} ,{277,900},{370,300} ,{370,300} ,{554,300},
{0, 500}};
  int num_notes = sizeof(all_star_verse_1) / sizeof(all_star_verse_1[0]);

  int note_index = 0;
  int interval = 0;
  int current_millis = 0;
  int previous_millis = 0;

  bool buzzer_on = 0;
 
}


namespace psat {

void BuzModule::setup() {
  startBuz();	
}

void BuzModule::startBuz() {
  buzzer_on = 1;
  note_index = 0;
  
  previous_millis = millis();
  note_index++;
  if (note_index >= num_notes){
    note_index = 0;
  }
  tone(buzzer_pin, all_star_verse_1[0][0], all_star_verse_1[0][1]);
}

void BuzModule::buzLoop() {
  if (buzzer_on == 0){
      return;
   }

   int current_millis = millis();
   if((current_millis - interval) > previous_millis) {
      previous_millis = millis();
      note_index++;
      if (note_index >= num_notes){
        note_index = 0;
      }
      interval = all_star_verse_1[note_index][1];
      tone(buzzer_pin, all_star_verse_1[note_index][0], interval);
   }
}

void BuzModule::buzStop() {
  buzzer_on = 0;
}

}
