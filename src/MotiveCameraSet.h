#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"

#include "NPTrackingTools.h"

#include "MotiveCamera.h"


class MotiveCameraSet {
public:

	MotiveCameraSet();
	~MotiveCameraSet();

	void setupParams();

	void update();

	int getNumActiveCameras() { return nActiveCams; };
	int getNumObservedCameras() { return cams.size(); };

	bool camExists(int serial);

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
};

