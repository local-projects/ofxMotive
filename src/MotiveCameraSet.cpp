#include "MotiveCameraSet.h"

// --------------------------------------------------------
MotiveCameraSet::MotiveCameraSet() {

}

// --------------------------------------------------------
MotiveCameraSet::~MotiveCameraSet() {
	//delete all cameras
}

// --------------------------------------------------------
void MotiveCameraSet::setupParams() {
	map<int, MotiveCamera*>::iterator it;
	for (it = cams.begin(); it != cams.end(); it++) {
		it->second->setupParams();
	}
}

// --------------------------------------------------------
bool MotiveCameraSet::camExists(int serial) {
	return (cams.find(serial) != cams.end());
}

// --------------------------------------------------------
MotiveCamera* MotiveCameraSet::getCamera(int serial) {
	if (!camExists(serial)) {
		cams[serial] = new MotiveCamera();
		cams[serial]->serial = serial;
	}
	return cams[serial];
}

// --------------------------------------------------------
void MotiveCameraSet::update() {

	// Update all cameras

	// First, get the set of current serial numbers
	nActiveCams = TT_CameraCount();
	thisSerials.clear();
	serial2Index.clear();
	for (int index = 0; index < nActiveCams; index++) {
		// Get this serial number
		int serial = TT_CameraSerial(index);
		thisSerials.push_back(serial);
		serial2Index[serial] = index;
	}
	sort(thisSerials.begin(), thisSerials.end());
	// Compare these new serial numbers with the old ones to find how cameras have changed state
	newCams.clear(); 
	newCams.resize(lastSerials.size() + thisSerials.size());
	lostCams.clear();
	lostCams.resize(lastSerials.size() + thisSerials.size());
	activeCams = thisSerials;
	// Get all new cameras
	vector<int>::iterator it = set_difference(
		thisSerials.begin(),
		thisSerials.end(),
		lastSerials.begin(),
		lastSerials.end(),
		newCams.begin());
	newCams.resize(it - newCams.begin());
	// Get all lost cameras
	it = set_difference(
		lastSerials.begin(),
		lastSerials.end(),
		thisSerials.begin(),
		thisSerials.end(),
		lostCams.begin());
	lostCams.resize(it - lostCams.begin());
	// Save these cams
	lastSerials = thisSerials;

	// Print information about cameras
	// ... if we've lost connection with any
	if (lostCams.size() > 0) {
		stringstream ss;
		ss << "Disconnected from cameras: ";
		for (int i = 0; i < lostCams.size(); i++) {
			if (i != 0) ss << ", ";
			ss << lostCams[i];
		}
		ofLogNotice("Motive Camera Set") << ss.str();
	}
	// ... if we've connected with any new ones
	if (newCams.size() > 0) {
		stringstream ss;
		ss << "Connected to cameras: ";
		for (int i = 0; i < newCams.size(); i++) {
			if (i != 0) ss << ", ";
			ss << newCams[i];
		}
		ofLogNotice("Motive Camera Set") << ss.str();
	}

	// Update the connection status of disconnected cameras
	for (int i = 0; i < lostCams.size(); i++) {
		getCamera(lostCams[i])->bConnected = false;
	}

	// Add any new cameras and set their status to connected
	for (int i = 0; i < newCams.size(); i++) {
		MotiveCamera* cam = getCamera(newCams[i]);
		cam->bConnected = true;
		cam->serial = newCams[i]; // redundant
		// Get all relevant information for this camera that isn't going to change every frame
		cam->index = serial2Index[cam->serial];
		cam->name = TT_CameraName(cam->index);
		// location
		cam->position.x = TT_CameraXLocation(cam->index);
		cam->position.y = TT_CameraYLocation(cam->index);
		cam->position.z = TT_CameraZLocation(cam->index);
		// orientation
		glm::mat3 mat;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				mat[i][j] = TT_CameraOrientationMatrix(cam->index, i * 3 + j);
			}
		}
		cam->orientation = getQuaternion(mat);
		// pixel resolution
		// exposure
		// threshold
		// intensity
		// imager gain
		// mask settings
		// camera id

		// Create new parameter groups

	}



	// Now, process all current cameras
	for (int i = 0; i < activeCams.size(); i++) {

	}


	// Update their active index
	// Update all information that changes every frame
	// Set all new information

	// marker count, markers, projected location
	// frame rate
	// video type
	// temperature
	// camera state






	// push changes to this camera
	// camera settings
	// camera frame rate
	// camera video type
	// automatic gain control 
	// automatic exposure control
	// mjpeg quality
	// set imager gain
	// set mask
	// set camera state
	// save a frame


		// Get this camera
		//int serial = TT_CameraSerial(index);
		//if (!camExists(serial)) {
		//	ofLogNotice("Motive Camera Set") << "Adding camera with serial number " << ofToString(serial);
		//}



}

// --------------------------------------------------------
glm::quat getQuaternion(glm::mat3& rotMat) {

	// Implementation from:
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

	glm::quat q;
	float trace = rotMat[0][0] + rotMat[1][1] + rotMat[2][2];
	if (trace > 0) {
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (rotMat[2][1] - rotMat[1][2]) * s;
		q.y = (rotMat[0][2] - rotMat[2][0]) * s;
		q.z = (rotMat[1][0] - rotMat[0][1]) * s;
	}
	else {
		if (rotMat[0][0] > rotMat[1][1] && rotMat[0][0] > rotMat[2][2]) {
			float s = 2.0f * sqrtf(1.0f + rotMat[0][0] - rotMat[1][1] - rotMat[2][2]);
			q.w = (rotMat[2][1] - rotMat[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (rotMat[0][1] + rotMat[1][0]) / s;
			q.z = (rotMat[0][2] + rotMat[2][0]) / s;
		}
		else if (rotMat[1][1] > rotMat[2][2]) {
			float s = 2.0f * sqrtf(1.0f + rotMat[1][1] - rotMat[0][0] - rotMat[2][2]);
			q.w = (rotMat[0][2] - rotMat[2][0]) / s;
			q.x = (rotMat[0][1] + rotMat[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (rotMat[1][2] + rotMat[2][1]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + rotMat[2][2] - rotMat[0][0] - rotMat[1][1]);
			q.w = (rotMat[1][0] - rotMat[0][1]) / s;
			q.x = (rotMat[0][2] + rotMat[2][0]) / s;
			q.y = (rotMat[1][2] + rotMat[2][1]) / s;
			q.z = 0.25f * s;
		}
	}
}

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

