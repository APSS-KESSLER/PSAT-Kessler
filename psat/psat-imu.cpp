#include "psat-imu.h"

#include "psat-log.h"
#include "psat-data.h"

namespace psat {

void ImuModule::setup() {
	if (!lsm.begin_I2C()) {
		LOG_ERROR("ISM", "Unable to find LSM6DS33");
		return;
	}

	LOG_INFO("ISM", "Found LSM module");

	lsm.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
	lsm.setGyroRange(LSM6DS_GYRO_RANGE_125_DPS);
	lsm.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
	lsm.setGyroDataRate(LSM6DS_RATE_12_5_HZ);

	valid = true;
}

void ImuModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	sensors_event_t accel, gyro, temp;

	if (!lsm.getEvent(&accel, &gyro, &temp)) {
		return;
	}
	
	data.acceleration.x = accel.acceleration.x;
	data.acceleration.y = accel.acceleration.y;
	data.acceleration.z = accel.acceleration.z;

	data.gyro.x = gyro.gyro.x;
	data.gyro.y = gyro.gyro.y;
	data.gyro.z = gyro.gyro.z;

	data.temperature = temp.temperature;
}

}