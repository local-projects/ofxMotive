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
		// orientation
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

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

// --------------------------------------------------------

