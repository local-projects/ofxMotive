#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"
#include "Marker.h"

class Reconstruction {
public:

	Reconstruction();
	~Reconstruction();

	void setupParams();

	void update();

	vector<Marker> markers;

private:


};

