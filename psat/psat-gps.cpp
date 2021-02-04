#include "psat-gps.h"
#include "psat-data.h"
#include "psat-log.h"

namespace psat {

void GpsModule::setup() {
	if (!gps.begin(9600)) {
		LOG_ERROR("GPS", "Failed to start GPS");
		return;
	}

	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
}

void GpsModule::writeData(psat::Data &data) {
	while (!gps.newNMEAreceived()) {
		readTenCharacters();
	}
	
	if (gps.newNMEAreceived()) {

	}
}

}