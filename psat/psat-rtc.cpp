#include "psat-rtc.h"
#include "psat-log.h"
#include "psat-data.h"

namespace psat {

void RtcModule::setup() {
	if (!rtc.begin()) {
		LOG_ERROR("RTC", "No RTC module found");
		return;
	}

	if (!rtc.initialized() || rtc.lostPower()) {
		setTime(DateTime(F(__DATE__), F(__TIME__)));
	}
}

void RtcModule::setTime(DateTime const &dateTime) {
	LOG_INFO("RTC", "Setting the RTC time");
	rtc.adjust(dateTime);
}

void RtcModule::writeData(psat::Data &data) {
	data.time = rtc.now().unixtime();
}

}