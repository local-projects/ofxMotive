#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"

#include "NPTrackingTools.h"

class MotiveCamera {
public:

	MotiveCamera();
	~MotiveCamera();

	int index = -1;				// index in the array of active cameras
	int ID = -1;				// unique ID assigned at startup for every camera
	int serial = -1;			// unique for a camera
	string name = "";			// name and serial number

	bool bConnected = false;

	int videoType = 0;
	int exposure = 0;
	int threshold = 0;
	int intensity = 0;
	

	/// \brief Setup params for the RUI per camera
	void setupParams();




};

