#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"

#include "NPTrackingTools.h"

#include "ofxMotiveCamera.h"


class MotiveCameraSet {
public:

	MotiveCameraSet();
	~MotiveCameraSet();

	void setupParams();

	void update();

	//int getNumActiveCameras() { return nActiveCams; };
	int getNumCameras() { return cams.size(); };

	bool camExists(int serial);

	// Remove Accumulated latency
	void flushQueues();

	// Get the list of all active cameras
	vector<MotiveCamera*> getActiveCameras();

	// Get a specific camera, and don't create if new
	MotiveCamera* getCameraFromSerial(int serial);

	// TODO: Set settings of all cameras at once

	// Get the maximum frame rate across all cameras
	int getMaxFPS();


private:

	// Map of serial number to camera pointer
	map<int, MotiveCamera*> cams;
	int nActiveCams = 0;
	
	// get a camera or create if new
	MotiveCamera* getCamera(int serial);

	// These are used for sorting
	vector<int> thisSerials;
	vector<int> lastSerials;
	map<int, int> serial2Index;
	vector<int> lostCams;		// no longer observed
	vector<int> newCams;		// newly observed
	vector<int> activeCams;		// all current (active) cameras

	// Utility functions
	glm::quat getQuaternion(glm::mat3& rotationMatrix);
	
	// Global/Template camera settings
	MotiveCameraSettings defaultCameraSettings;
	// Should new cameras be initialized with these default settings?
	bool bInitWithDefaults = false;
	// Which default settings should be used? Use an integer (binary) mask
	// that is the sum of the following powers of 2:
	// 1 = init camera state
	// 2 = init exposure
	// 4 = init threshold
	// 8 = init intensity
	// 16 = init framerate
	// 32 = init imager gain
	// 64 = init camera mode
	// 128 = mjpeg quality
	int bInitWithDefaultsMask = 255;
	// Should we push default settings to all cams at any point in time?
	bool bPushDefaults = false;
};

