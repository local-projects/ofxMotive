#include "PredictedID.h"

float dbeta(float alpha, int numA, float beta, int numB) {
	return (alpha + float(numA)) / (alpha + float(numA) + beta + float(numB));
}

// ----------------------------------------------------
PredictedID::PredictedID() {
	nZeros.resize(nBits, 0);
	nOnes.resize(nBits, 0);
}

// ----------------------------------------------------
PredictedID::~PredictedID() {

}

// ----------------------------------------------------
void PredictedID::addObservation(int bitIndex, int observation) {
	if (bitIndex < 0 || bitIndex >= nBits) ofLogError("PredictedID") << "Bit index out of range";
	if (observation == 0) {
		nZeros[bitIndex]++;
	}
	else if (observation == 1) {
		nOnes[bitIndex]++;
	}
	else {
		ofLogError("PredictedID") << "Bit observation out of range";
	}
}

// ----------------------------------------------------
void PredictedID::predict() {
	// Use our observations to create a prediction of the ID
	uint64_t tmpID = 0;
	float thisLike = 1.0;
	float avgLike = 0;
	int nArbitraryDecisions = 0; // when prob = 0.5
	for (int i = 0; i < nBits; i++) {
		// calc probability
		float probability = dbeta(alpha, nZeros[i], beta, nOnes[i]);
		if (probability == 0.5) nArbitraryDecisions++;
		// Update likelihood
		float like = (probability <= 0.5) ? (1.0 - probability) : probability;
		thisLike *= like;
		avgLike += like;
		// predict
		uint64_t bit = (probability >= 0.5) ? 0 : 1;
		// store the bit
		tmpID |= bit << i; // (nBits - i - 1);
	}
	avgLike /= float(nBits);
	// Store the predicted ID
	predictedID = tmpID;
	// Store the likelihood
	likelihood = thisLike;
	//likelihood = avgLike;
	// If not all decisions were arbitrary, mark as valid prediction
	if (!bValidPrediction && nArbitraryDecisions != nBits) {
		bValidPrediction = true;
	}
}

// ----------------------------------------------------
bool PredictedID::isValidPrediction() {
	return bValidPrediction;
}

// ----------------------------------------------------
uint64_t PredictedID::getIDPrediction() {
	return predictedID;
}

// ----------------------------------------------------
float PredictedID::getIDLikelihood() {
	return likelihood;
}

// ----------------------------------------------------

// ----------------------------------------------------

// ----------------------------------------------------

// ----------------------------------------------------
