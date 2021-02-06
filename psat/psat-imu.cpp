#include "psat-imu.h"

#include "psat-log.h"
#include "psat-data.h"

namespace {

const float magXOffset = 26.34f;
const float magYOffset = 27.61f;
const float magZOffset = 127.30f;

const float gyroXOffset = 0.0696f;
const float gyroYOffset = -0.1140f;
const float gyroZOffset = -0.1139f;

}

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

	if (!lis.begin_I2C()) {
		LOG_ERROR("ISM", "Unable to find LIS3MDL");
		return;
	}

	lis.setDataRate(LIS3MDL_DATARATE_155_HZ);
	lis.setOperationMode(LIS3MDL_CONTINUOUSMODE);
	lis.setPerformanceMode(LIS3MDL_MEDIUMMODE);
	lis.setRange(LIS3MDL_RANGE_4_GAUSS);

	valid = true;
}

void ImuModule::writeData(psat::Data &data) {
	if (!valid) {
		return;
	}

	sensors_event_t accel, gyro, temp, mag;

	if (lsm.getEvent(&accel, &gyro, &temp)) {
		data.acceleration.x = accel.acceleration.x;
		data.acceleration.y = accel.acceleration.y;
		data.acceleration.z = accel.acceleration.z;

		data.gyro.x = gyro.gyro.x - gyroXOffset;
		data.gyro.y = gyro.gyro.y - gyroYOffset;
		data.gyro.z = gyro.gyro.z - gyroZOffset;

		data.temperature = temp.temperature;
	}

	if (lis.getEvent(&mag)) {
		data.mag.x = mag.magnetic.x - magXOffset;
		data.mag.y = mag.magnetic.y - magYOffset;
		data.mag.z = mag.magnetic.z - magZOffset;
	}
}

}