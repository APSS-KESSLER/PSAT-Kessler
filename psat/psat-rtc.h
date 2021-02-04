#ifndef PSAT_RTC_H
#define PSAT_RTC_H

#include <RTClib.h>

namespace psat {

struct Data;

class RtcModule {
	RTC_PCF8523 rtc;

public:
	void setup();
	void setTime(DateTime const &dateTime);
	void writeData(psat::Data &data);
};

}

#endif