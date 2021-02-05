#ifndef PSAT_CAMERA_H
#define PSAT_CAMERA_H

#include <Arduino.h>

namespace psat {

struct Data;

class CameraModule {
	const int cameraLED = 13;
	const int cameraTrigger = 5;
	const int maxPhotos = 999;

	bool recordingVideo = false;
	uint32_t photoCount = 0;

	unsigned long nextPhotoMs, stopVideoMs, nextPhotoIntervalMs;
	size_t photosToTake = 0;

public:
	void setup();
	void writeData(psat::Data &data);
	void runCameraTasks();
	void takeVideo(unsigned long lengthMs = 10000u);
	void takePicture(size_t numberOfPhotos = 1, unsigned long interval = 1000u);
	size_t numberOfPhotosRemaining();

private:
	void toggleVideoState();

};

}

#endif
