#include "Identification.h"

// ----------------------------------------------------
Identification::Identification() {

}

// ----------------------------------------------------
Identification::~Identification() {

}

// ----------------------------------------------------
void Identification::setupParams() {

	RUI_NEW_GROUP("Identification");
	RUI_SHARE_PARAM_WCN("Number of Bits", nCodeBits, 1, 16);
	RUI_SHARE_PARAM_WCN("Interleave", bInterleaved);
	RUI_SHARE_PARAM_WCN("Interleave Smart", bSmartInterleave);
	RUI_SHARE_PARAM_WCN("Interleave Pattern", interleavePattern);
	RUI_SHARE_PARAM_WCN("Interleave Start", bInterleaveStart);
	RUI_SHARE_PARAM_WCN("Interleave Start Value", interleaveStartValue, 0, 1);
	RUI_SHARE_PARAM_WCN("Interleave End", bInterleaveEnd);
	RUI_SHARE_PARAM_WCN("Interleave End Value", interleaveEndValue, 0, 1);
	RUI_SHARE_PARAM_WCN("Prefix", bPrefix);
	RUI_SHARE_PARAM_WCN("Prefix Length", nPrefixBits, 0, 24);
	RUI_SHARE_PARAM_WCN("Prefix Pattern", prefixPattern);
	RUI_SHARE_PARAM_WCN("Push Updates to Pattern", bPushPatternUpdates);

}

// ----------------------------------------------------
void Identification::update(vector<Marker>& markers) {

	if (firstTimeLoading || bPushPatternUpdates) {
		firstTimeLoading = false;
		bPushPatternUpdates = false;
		RUI_PUSH_TO_CLIENT();
		updatePatternTemplate();
	}

	// print debug information
	//stringstream ss;
	//ss << "There are " << markers.size() << " markers:\t";
	//for (int i = 0; i < markers.size(); i++) {
	//	ss << markers[i].getIDString();
	//}
	//ofLogNotice("ofxMotive") << ss.str();


	// Determine which dots exhibit persistence
	// Assign temporary IDs to continue tracking their pulse codes

	// Save this pulse

	// Look at the last pulse sequence and extract the code

	// Save this code for this pulsing dot

	// Update the likelihoods of specific codes




	// Set a parameter for each pulse to indicate how long it has been tracking
	// and how reliable its ID is



}

// ----------------------------------------------------
void Identification::updatePatternTemplate() {

	// Determine the total length of a message
	nTotalBits = 0
		+ (bPrefix ? nPrefixBits : 0)
		+ (bInterleaveStart ? 1 : 0)
		+ nCodeBits
		+ (bInterleaved ? nCodeBits - 1 : 0)
		+ (bInterleaveEnd ? 1 : 0);
	ofLogNotice("ID") << "There are " << nTotalBits << " total bits";

	// The template will contain the ID for a 0 ID
	patternTemplate = 0;
	int loc = nTotalBits - 1;
	for (int i = 0; i < nPrefixBits; i++) {
		patternTemplate |= getPatternBit(prefixPattern, i) << loc;
		loc--;
	}
	if (bInterleaveStart) {
		patternTemplate |= uint64_t(interleaveStartValue) << loc;
		loc--;
	}
	for (int i = 0; i < nCodeBits; i++) {
		// Add the code bit
		uint64_t codeBit = 0;
		patternTemplate |= codeBit << loc;
		loc--;
		if (i != (nCodeBits - 1)) {
			// Add the interleaved value -- this is not smart
			patternTemplate |= getPatternBit(interleavePattern, i) << loc;
			loc--;
		}
	}
	if (bInterleaveEnd) {
		patternTemplate |= uint64_t(interleaveEndValue) << loc;
		loc--;
	}

	ofLogNotice("Identification") << getDebugString(getPattern(888), nTotalBits) << " " << getDebugString(getPattern(1325), nTotalBits);
	bSmartInterleave = true;
	ofLogNotice("Identification") << getDebugString(getPattern(888), nTotalBits) << " " << getDebugString(getPattern(1325), nTotalBits);


}

// ----------------------------------------------------
uint64_t Identification::getPattern(uint64_t ID) {

	// Add the code bits to the template
	uint64_t outPattern = patternTemplate;
	for (int i = (nCodeBits-1); i >= 0; i--) {
		outPattern |= (((uint64_t(1) << i) & ID) >> i) << (i * (bInterleaved ? 2 : 1) + (bInterleaveEnd ? 1 : 0));
	}

	// smart interleaving
	if (bSmartInterleave) {

		for (int i = (nCodeBits - 1); i >= 1; i--) {
			uint64_t thisCodeBit = (((uint64_t(1) << i) & ID) >> i);
			uint64_t nextCodeBit = (((uint64_t(1) << (i - 1)) & ID) >> (i - 1));
			uint64_t interleavedBit = ((thisCodeBit + nextCodeBit) == 2 ? 0 : 1);

			outPattern |= interleavedBit << ((i * (bInterleaved ? 2 : 1) + (bInterleaveEnd ? 1 : 0)) - 1);
		}
	}

	return outPattern;
}

// ----------------------------------------------------
uint64_t Identification::getPatternBit(string pattern, int index) {
	if (pattern == "") return 0;
	return (pattern[index % pattern.length()] == '0' ? 0 : 1);
}

// ----------------------------------------------------
string Identification::getDebugString(uint64_t pattern, int length) {

	string out = "";
	for (int i = (length-1); i >= 0; i--) {
		out += (((pattern & (uint64_t(1) << i)) >> i) == 0 ? "0" : "1");
	}
	return out;
}

// ----------------------------------------------------
uint64_t Identification::decodeID(uint64_t pattern) {



	return 0; // TODO
}


// ----------------------------------------------------

// ----------------------------------------------------
