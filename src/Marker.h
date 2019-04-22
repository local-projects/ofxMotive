#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"

struct MarkerContribution {
	int cameraIndex;
	int cameraSerial;
	glm::vec2 centroid; // 2d location of the contributing point (raw?)
};

class Marker {
public:

	Marker();
	~Marker();

	glm::vec3 position;
	Core::cUID ID;
	float residual;
	vector<MarkerContribution> contributions;
	double timestamp;

};

