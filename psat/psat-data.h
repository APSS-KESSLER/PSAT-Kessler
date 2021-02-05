#ifndef PSAT_DATA_H
#define PSAT_DATA_H

#include <Adafruit_GPS.h>

namespace psat {

struct Data {
	unsigned long millis;
	uint32_t time;
	int32_t rssi;

	struct {
		float x;
		float y;
		float z;
	} acceleration;

	struct {
		float x;
		float y;
		float z;
	} gyro;

	struct {
		float x;
		float y;
		float z;
	} mag;

	struct {
		struct {
			uint8_t y;
			uint8_t mo;
			uint8_t d;
			uint8_t h;
			uint8_t m;
			uint8_t s;
		} time;

		bool fix;

		struct {
			nmea_float_t latitude;
			char lat;
			nmea_float_t longitude;
			char lon;
			nmea_float_t altitude;
		} location;

		nmea_float_t speed;
		uint8_t satellites;
	} gps;

	float temperature;
	float pressure;
	float pressureAltitude;

	uint32_t numberOfPhotosTaken;

	size_t writeToString(char *string) const {
		return static_cast<size_t>(sprintf(string,
		"{"
			"\"millis\":%lu,"
			"\"time\":%u,"
			"\"rssi\":%d,"
			"\"acc\":[%f,%f,%f],"
			"\"gyro\":[%f,%f,%f],"
			"\"mag\":[%f,%f,%f],"
			"\"temp\":%f,"
			"\"pressure\":%f,"
			"\"pressureAltitude\":%f,"
			"\"gps\":{"
				"\"time\":[%u,%u,%u,%u,%u,%u],"
				"\"fix\":%d,"
				"\"latitude\":%f,"
				"\"longitude\":%f,"
				"\"speed\":%f,"
				"\"altitude\":%f,"
				"\"satellites\":%u"
			"},"
			"\"photoNumber\":%u"
		"}", 
			millis, time, 
			
			rssi, 
			
			acceleration.x, acceleration.y, acceleration.z,
			gyro.x, gyro.y, gyro.z, 
			mag.x, mag.y, mag.z, 
			
			temperature, pressure, pressureAltitude,

			gps.time.y, gps.time.mo, gps.time.d, gps.time.h, gps.time.m, gps.time.s,
			gps.fix,
			gps.location.latitude, gps.location.longitude,
			gps.speed,
			gps.location.altitude,
			gps.satellites,
			numberOfPhotosTaken
		));
	};
};

}

#endif