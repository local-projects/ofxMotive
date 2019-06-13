#pragma once

#include "ofMain.h"
#include "ofxRemoteUIServer.h"
#include "NPTrackingTools.h"
#include "Marker.h"

enum LampIDMode {
	ID_CODE = 0,
	ID_CODE_INTERLEAVED
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

};

