#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"

// beta distribution
float dbeta(float alpha, int numA, float beta, int numB);

// Predicted ID
class PredictedID {
public:
	PredictedID();
	~PredictedID();

	// Add an observation (0 or 1) to the bit # bitIndex
	void addObservation(int bitIndex, int observation);
	void predict();
	bool isValidPrediction();
	uint64_t getIDPrediction();
	float getIDLikelihood();

private:
	int nBits = 12;
	vector<int> nZeros; // for each bit
	vector<int> nOnes;
	float alpha = 1;
	float beta = 1;
	uint64_t predictedID = 0;
	float likelihood = 0;
	bool bValidPrediction = false;

};
