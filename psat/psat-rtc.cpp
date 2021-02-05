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
		DateTime compileTime = DateTime(F(__DATE__), F(__TIME__));

		LOG_INFO("RTC", "Setting clock to " __DATE__ " " __TIME__);
		setTime(compileTime);
	}

	LOG_INFO("RTC", "Enabled RTC");

	valid = true;
}

void RtcModule::setTime(DateTime const &dateTime) {
	if (!valid) {
		return;
	}

	LOG_INFO("RTC", "Setting the RTC time");
	rtc.adjust(dateTime);
}

void RtcModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	data.time = rtc.now().unixtime();
}

}