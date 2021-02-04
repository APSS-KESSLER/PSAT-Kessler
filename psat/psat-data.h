#ifndef PSAT_DATA_H
#define PSAT_DATA_H

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

	float temperature;

	size_t writeToString(char *string) const {
		return static_cast<size_t>(sprintf(string,
		"{"
			"\"millis\":%lu,"
			"\"time\":%u,"
			"\"rssi\":%d,"
			"\"acc\":[%f,%f,%f],"
			"\"gyro\":[%f,%f,%f],"
			"\"temp\":%f"
		"}", millis, time, rssi, acceleration.x, acceleration.y, acceleration.z, gyro.x, gyro.y, gyro.z, temperature));
	};
};

}

#endif