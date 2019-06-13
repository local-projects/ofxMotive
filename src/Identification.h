#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"
#include "Marker.h"

enum LampIDMode {
	ID_CODE = 0,
	ID_CODE_INTERLEAVED
};

// beta distribution
float dbeta(int alpha, int numA, int beta, int numB);

// Predicted ID
class PredictedID {
public:
	PredictedID() {
		nZeros.resize(nBits, 0);
		nOnes.resize(nBits, 0);
	}
	// Add an observation (0 or 1) to the bit # bitIndex
	void addObservation(int bitIndex, int observation) {
		if (bitIndex < 0 || bitIndex >= nBits) ofLogError("Identification") << "Bit index out of range";
		if (observation == 0) {
			nZeros[bitIndex]++;
		}
		else if (observation == 1) {
			nOnes[bitIndex]++;
		}
		else {
			ofLogError("Identification") << "Bit observation out of range";
		}
	}
	void predict() {
		// Use our observations to create a prediction of the ID
		uint64_t tmpID = 0;
		float thisLike = 1;
		for (int i = 0; i < nBits; i++) {
			// calc probability
			float probability = dbeta(alpha, nZeros[i], beta, nOnes[i]);
			// Update likelihood
			thisLike *= probability;
			// predict
			uint64_t bit = (probability <= 0.5) ? 0 : 1;
			// store the bit
			tmpID |= bit << (nBits - i - 1);
		}
		// Store the predicted ID
		predictedID = tmpID;
		// Store the likelihood
		likelihood = thisLike;
	}
	bool isValidPrediction() {
		return predictedID != -1;
	}
	uint64_t getIDPrediction() {
		return predictedID;
	}
	uint64_t getIDLikelihood() {
		return likelihood;
	}

private:
	int nBits = 12;
	vector<int> nZeros; // for each bit
	vector<int> nOnes;
	int alpha = 1;
	int beta = 1;
	uint64_t predictedID = -1;
	float likelihood = 0;

};

class LightSource {
public:
	LightSource(Core::cUID _ID) {
		ID = _ID;
	}
	void addSample(bool _sample, float _timeStamp) {
		samples.push_back(_sample);
		if (samples.size() > nMaxSamples) samples.erase(samples.begin());
		lastSampleUpdate = _timeStamp;
	}
	float getLastSampleUpdateTimestamp() {
		return lastSampleUpdate;
	}
	bool wasRecentlySeen() {
		return find(samples.begin(), samples.end(), true) != samples.end();
	}
	void updateBits() {
		if (!bitJustReceived()) return;
		// get the value of the last bit
		int nHiVals = 0;
		for (int i = samples.size() - 2; i >= 0; i--) {
			if (samples[i]) nHiVals++;
			else break;
		}
		// Parse the number of high values into a meaningful value
		if (nHiVals >= 10) { // start message threshold
			bits.push_back(-1); // start
		}
		else if (nHiVals <= 3) { // 0 bit threshold
			bits.push_back(0); // 0
		}
		else {
			bits.push_back(1); // 1
		}
	}
	void updateID() {
		// Number of Bits in ID
		int nIDBits = 12;

		// Update the ID with any new bit information
		// If there are two start messages, process the information in between those messages
		vector<int>::iterator it1 = find(bits.begin(), bits.end(), -1);
		if (it1 != bits.end()) {
			vector<int>::iterator it2 = find(it1 + 1, bits.end(), -1);
			if (it2 != bits.end()) {

				// Check if there are enough bits
				if (std::distance(it1, it2) == (nIDBits+1)) {
					// Correct number of bits

					// Parse into an ID
					int bitCounter = 0;
					for (auto it = (it1+1); it != it2; it++) {
						if (*it == -1) {
							// This shouldn't happen
							ofLogError("Identification") << "Start value should not be here";
						}
						// Store these bits in a container that records their history
						lampID.addObservation(bitCounter, *it);
						bitCounter++;
					}
					// Predict the ID
					lampID.predict();
				}
				else {
					// Incorrect number of bits
					// Ideally, record as an uncertain ID
				}
				
				// Delete all of the bits leading up to the second start instance
				bits.erase(bits.begin(), it2 - 1); // -1 or 0?
			}
		}
	}
	PredictedID& getPredictedID() {
		return lampID;
	}


private:
	Core::cUID ID;
	vector<bool> samples;	// raw data
	int nMaxSamples = 200;
	float lastSampleUpdate = 0; // timestamp in seconds
	vector<int> bits;		// processed data: -1 = start bit, 0 = 0 bit, 1 = 1 bit
	PredictedID lampID;

	bool bitJustReceived() {
		// If there was just a recent transition from true to false (HI to LOW)
		return !(samples.size() < 3 || samples.back() || !samples[samples.size() - 2]);
	}

};

class Identification {
public:

	Identification();
	~Identification();

	void setupParams();

	void update(vector<Marker>& markers);


private:

	int frameRate = 100;

	// How many bits will it take to define this many lamps?
	int nCodeBits = 12; // 4096 lamps

	bool bInterleaved = true;
	bool bSmartInterleave = false;
	string interleavePattern = "10"; // "10";
	bool bInterleaveStart = true;
	int interleaveStartValue = 0;
	bool bInterleaveEnd = true; // whether to fix the last interleaved value
	int interleaveEndValue = 0;

	bool bPrefix = true;
	int nPrefixBits = 6;
	string prefixPattern = "1";

	bool bPushPatternUpdates = false;


	void updatePatternTemplate();
	// Evaluated attributes:
	int nTotalBits = 0;
	uint64_t patternTemplate = 0;
	// The bits in this pattern look like this:


	// Get a pattern with a specific ID
	uint64_t getPattern(uint64_t ID);

	// Try to decode an ID with a pattern of nTotalBits length.
	// This does not perform any time warping nor any 
	// gaussian distribution fitting / matching.
	// This also assumes that it begins with the prefix and that the 
	// prefix is the correct length.
	uint64_t decodeID(uint64_t pattern);


	bool firstTimeLoading = true;
	uint64_t getPatternBit(string pattern, int index);

	string getDebugString(uint64_t pattern, int length);


	// New mode as of Jun 2019:
	map<Core::cUID, LightSource*> sources;

};

