#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"
#include "PredictedID.h"

// A persistent source of light, possibly identified
class LightSource {
public:
	// Light sources are defined by a CUID (from Motive)
	LightSource(Core::cUID _ID);
	~LightSource();

	// Add a new sample to the vector of raw data 
	void addSample(bool _sample, float _timeStamp);

	// Get the last time (in seconds) this was updated
	float getLastSampleUpdateTimestamp();

	// Was this light source recently seen?
	bool wasRecentlySeen();
	
	// Update the extracted bits from the raw data
	void updateBits();

	// Refresh the ID prediction
	void updateID();

	// Get the predicted ID
	PredictedID& getPredictedID();

	// position of the light source
	glm::vec3 position;

private:
	// Number of Bits in ID
	int nIDBits = 12;

	Core::cUID ID;
	vector<bool> samples;	// raw data
	int nMaxSamples = 200;
	float lastSampleUpdate = 0; // timestamp in seconds
	vector<int> bits;		// processed data: -1 = start bit, 0 = 0 bit, 1 = 1 bit
	vector<int> highValues; // for reference; becomes the bits
	PredictedID lampID;
	
	bool bitJustReceived();

};

