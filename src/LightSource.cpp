#include "LightSource.h"

// ----------------------------------------------------
LightSource::LightSource(Core::cUID _ID) {
	ID = _ID;
}

// ----------------------------------------------------
LightSource::~LightSource() {

}

// ----------------------------------------------------
void LightSource::addSample(bool _sample, float _timeStamp) {
	samples.push_back(_sample);
	if (samples.size() > nMaxSamples) samples.erase(samples.begin());
	lastSampleUpdate = _timeStamp;
}

// ----------------------------------------------------
float LightSource::getLastSampleUpdateTimestamp() {
	return lastSampleUpdate;
}

// ----------------------------------------------------
bool LightSource::wasRecentlySeen() {
	return find(samples.begin(), samples.end(), true) != samples.end();
}

// ----------------------------------------------------
void LightSource::updateBits() {
	if (!bitJustReceived()) return;

	// get the value of the last bit
	int nHiVals = 0;
	for (int i = samples.size() - 2; i >= 0; i--) {
		if (samples[i]) nHiVals++;
		else break;
	}
	highValues.push_back(nHiVals);

	// Parse the number of high values into a meaningful value
	if (nHiVals >= 10) { // start message threshold
		bits.push_back(-1); // start
	}
	else if (nHiVals <= 4) { // 0 bit threshold
		bits.push_back(0); // 0
	}
	else {
		bits.push_back(1); // 1
	}
}

// ----------------------------------------------------
void LightSource::updateID() {

	// Update the ID with any new bit information
	// If there are two start messages, process the information in between those messages
	vector<int>::iterator it1 = find(bits.begin(), bits.end(), -1);
	if (it1 != bits.end()) {
		vector<int>::iterator it2 = find(it1 + 1, bits.end(), -1);
		if (it2 != bits.end()) {

			// Check if there are enough bits
			if (std::distance(it1, it2) == (nIDBits + 1)) {
				// Correct number of bits

				// Parse into an ID
				int bitCounter = 11;
				for (auto it = (it1 + 1); it != it2; it++) {
					if (*it == -1) {
						// This shouldn't happen
						ofLogError("LightSource") << "Start value should not be here";
					}
					// Store these bits in a container that records their history
					lampID.addObservation(bitCounter, *it);
					//ofLogNotice("LS " + ofToString(ID.HighBits())) << *it << "\t" << highValues[std::distance(bits.begin(), it)];
					bitCounter--;
				}
				// Predict the ID
				lampID.predict();
			}
			else {
				// Incorrect number of bits
				// Ideally, record as an uncertain ID
			}

			// Delete all of the bits leading up to the second start instance
			highValues.erase(highValues.begin(), highValues.begin() + std::distance(bits.begin(), it2-1));
			bits.erase(bits.begin(), it2 - 1); // -1 or 0?
		}
	}
}

// ----------------------------------------------------
PredictedID& LightSource::getPredictedID() {
	return lampID;
}

// ----------------------------------------------------
bool LightSource::bitJustReceived() {
	// If there was just a recent transition from true to false (HI to LOW)
	return !(samples.size() < 3 || samples.back() || !samples[samples.size() - 2]);
}
// ----------------------------------------------------

// ----------------------------------------------------

// ----------------------------------------------------
