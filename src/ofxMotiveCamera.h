#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"

#include "NPTrackingTools.h"

enum MotiveCameraMode {
	MOTIVE_CAMERA_MODE_INVALID = -1,
	MOTIVE_CAMERA_MODE_SEGMENT, // 0
	MOTIVE_CAMERA_MODE_RAW_GRAYSCALE, // 1
	MOTIVE_CAMERA_MODE_OBJECT, // 2
	MOTIVE_CAMERA_MODE_PRECISION, // 4
	MOTIVE_CAMERA_MODE_MJPEG // 6
};

class MotiveCamera {
public:

	MotiveCamera();
	~MotiveCamera();

	// Setup params for the RUI per camera
	void setupParams();


	// Identifiers for each camera
	int index = -1;				// index in the array of active cameras from TT_CameraCount()
	int ID = -1;				// unique ID assigned to every camera (displayed on cameras)
	int serial = -1;			// unique for a camera
	string name = "";			// name and serial number

	// Is the camera currently online?
	bool bConnected = false;
	// Is the camera enabled for reconstruction?
	eCameraStates camState = Camera_Enabled;


	// SETTINGS THAT CAN CHANGE:

	// Capture params
	int exposure = 0; // in microseconds (max: 480)
	int threshold = 0;
	int intensity = 0; // intensity of IR light
	int frameRate;
	int imagerGain;
	int imagerGainLevels; // available gain levels

	// Image Quality
	// initialized to arbitrary number
	// [0, 100] (low --> high quality)
	int mjpegQuality = 50;

	// This pushes video type, exposure, thresh and intensity to the camera
	bool pushSettings();
	bool bPushSettings = false;

	// Location Params
	glm::vec3 position;		// this could change if continuous calibration is enabled
	glm::quat orientation;

	// Flag whether this camera may be misaligned.
	bool flagPossibleMisalignment = false;
	float avgFlagPossibleMisalignment = 0.0;
	uint64_t lastPossibleMisalignmentTimeMS = 0;


	// SETTINGS THAT CANNOT CHANGE:

	// Image Params
	glm::ivec2 resolution;

	// Camera temperature
	float temperature; // celcius

	
	// Set the video/camera mode
	void setVideoType(int _type);
	void setCameraMode(MotiveCameraMode _mode);
	int getVideoType();
	MotiveCameraMode getCameraMode();


	// call this after disconnecting
	void clearFrameData();

	// Frame Data:
	// Markers
	vector<glm::vec2> markers2DRaw; // unfiltered by size, roundness


	// Drawing 
	//bool bDraw = false;


	// TODO: 2D FILTER SETTINGS

private:

	// What is the video mode?
	MotiveCameraMode camMode = MOTIVE_CAMERA_MODE_INVALID;


	// also: intrinsic opencv parameters

	// Get this camera's fingerprint
	string fpt();

};

