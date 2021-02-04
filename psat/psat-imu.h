#ifndef PSAT_IMU_H
#define PSAT_IMU_H

#include <Adafruit_LSM6DS33.h>

namespace psat {

struct Data;

class ImuModule {
	bool valid = false;
	Adafruit_LSM6DS33 lsm;

public:
	void setup();
	void writeData(psat::Data &data);
};

}

#endif