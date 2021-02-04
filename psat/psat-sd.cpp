#include <RTClib.h>

#include "psat-sd.h"
#include "psat-data.h"

namespace psat {

void CardModule::setup() {
	if (!SD.begin(chipSelect)) {
		LOG_ERROR("SD", "Failed to init card");
		return;
	}

	char fileName [32];
	DateTime compileTime(F(__DATE__), F(__TIME__));

	sprintf(fileName, "%08u.csv", compileTime.unixtime() % 100000000u);
	
	LOG_INFO_F("SD", "Creating log file '%s'", fileName);
	SD.remove(fileName);
	fileOutput = SD.open(fileName, FILE_WRITE);

	if (!fileOutput) {
		LOG_ERROR("SD", "Failed to open log file");
		return;
	}

	writeHeader();
}

void CardModule::writeHeader() {
	if (!fileOutput) {
		return;
	}
	
	fileOutput.println("Millis [ms],Time [s since 1 Jan 1970],WiFi Strength [dBm]");
	fileOutput.flush();
}

void CardModule::writeToCard(psat::Data const &data) {
	if (!fileOutput) {
		return;
	}
	
	fileOutput.printf("%lu,%u,%d\n", data.millis, data.time, data.rssi);
	fileOutput.flush();
}

}