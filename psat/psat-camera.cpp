#include "psat-camera.h"
#include "psat-data.h"
#include "psat-log.h"

#include <Arduino.h>

namespace {

constexpr unsigned long overflowDetect = ~0u / 2;

}

namespace psat {

void CameraModule::setup() {
	// initialize the digital pins as output.
	pinMode(cameraLED, OUTPUT);
	pinMode(cameraTrigger, OUTPUT);
	digitalWrite(cameraLED, HIGH);  
	digitalWrite(cameraTrigger, HIGH);

	LOG_INFO("CAMERA", "Setup camera");
}

void CameraModule::writeData(psat::Data &data) {
	data.numberOfPhotosTaken = photoCount;
}

void CameraModule::runCameraTasks() {
	auto time = millis();

	if (recordingVideo && time - stopVideoMs < overflowDetect) {
		toggleVideoState();
	}

	if (photosToTake != 0 && time - nextPhotoMs < overflowDetect) {
		takePicture(photosToTake, nextPhotoIntervalMs);
	}
}

void CameraModule::takeVideo(unsigned long lengthMs) {
	toggleVideoState();

	stopVideoMs = millis() + lengthMs;
}

void CameraModule::takePicture(size_t numberOfPhotos, unsigned long interval) {
	LOG_INFO("CAMERA", "Taking picture");

	digitalWrite(cameraTrigger, LOW);
	delay(50);
	digitalWrite(cameraTrigger, HIGH);

	photoCount++;
	
	photosToTake = numberOfPhotos - 1;
	nextPhotoIntervalMs = interval;
	nextPhotoMs = millis() + interval;
}

size_t CameraModule::numberOfPhotosRemaining() {
	return maxPhotos - photoCount;
}

void CameraModule::toggleVideoState() {
	LOG_INFO_F("CAMERA", "%s video", recordingVideo ? "Stopping" : "Starting");

	digitalWrite(cameraTrigger, LOW);
	delay(1500);
	digitalWrite(cameraTrigger, HIGH);

	recordingVideo = !recordingVideo;
}

}
