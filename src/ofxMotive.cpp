#include "ofxMotive.h"

// --------------------------------------------------------------
ofxMotive::ofxMotive() {

}

// --------------------------------------------------------------
ofxMotive::~ofxMotive() {
	stop();
	waitForThread(false, 3000);
}

// --------------------------------------------------------------
void ofxMotive::setupParams() {

	RUI_NEW_GROUP("ofxMotive");
	RUI_SHARE_PARAM_WCN("Force Try Connect", bForceTryConnect);
	RUI_SHARE_PARAM_WCN("Try Connect Timestep", tryConnectTimestep, 0, 10000);
	RUI_SHARE_PARAM_WCN("Max Try Connect Attempts", maxTryConnectAttempts, 1, 10);
	RUI_SHARE_PARAM_WCN("Profile Path", profilePath);
	RUI_SHARE_PARAM_WCN("Reload Profile", bForceLoadProfile);
	RUI_SHARE_PARAM_WCN("Force Disconnect", bForceDisconnect);
	RUI_SHARE_PARAM_WCN("Process All Frames", bProcessAllFrames);

	// flush the queues


}

// --------------------------------------------------------------
void ofxMotive::setCalibrationPath(string _calibrationPath) {
	calibrationPath = _calibrationPath;
	if (isState(MOTIVE_CONNECTED)) loadCalibration();
}

// --------------------------------------------------------------
void ofxMotive::setProfilePath(string _profilePath) {
	profilePath = _profilePath;
		if (isState(MOTIVE_CONNECTED)) loadProfile();
}

// --------------------------------------------------------------
void ofxMotive::start() {

	// Begin the thread that will connect.
	// If blocking is true, then any lock on this thread will wait until its acquired. Otherwise, 
	// attempting to lock it will either return true or false.
	startThread(bBlocking);
	connect();
}

// --------------------------------------------------------------
MotiveState ofxMotive::getState() {
	return state;
}

// --------------------------------------------------------------
bool ofxMotive::isState(MotiveState _state) {
	return state == _state;
}

// --------------------------------------------------------------
bool ofxMotive::isConnected() {
	return isState(MOTIVE_CONNECTED);
}

// --------------------------------------------------------------
void ofxMotive::connect() {
	// Change the state of this thread
	if (isConnected()) {
		ofLogVerbose("ofxMotive") << "ofxMotive is already connected.";
	}
	else if (isState(MOTIVE_TRY_CONNECT)) {
		ofLogVerbose("ofxMotive") << "ofxMotive is already trying to connect";
	}
	else {
		startTryConnect();
	}
}

// --------------------------------------------------------------
void ofxMotive::startTryConnect() {
	setState(MOTIVE_TRY_CONNECT);
	lastTryConnectTime = -tryConnectTimestep;
	tryConnectCounter = 0;
}

// --------------------------------------------------------------
void ofxMotive::stop() {
	// Disconnect the system and cameras
	disconnect();

	// Stop the main thread
	stopThread();
}

// --------------------------------------------------------------
void ofxMotive::disconnect() {
	// If we're connected, disconnect all cameras first
	if (!isState(MOTIVE_DISCONNECTED)) {
		setState(MOTIVE_DISCONNECTED);
		shutdown();
	}
}

// --------------------------------------------------------------
void ofxMotive::setState(MotiveState _state) {
	state = _state;
};

// --------------------------------------------------------------
bool ofxMotive::isSuccess(NPRESULT result, string fnctName) {
	if (result == NPRESULT_SUCCESS) return true;
	string resultString(TT_GetResultString(result));
	ofLogError("ofxMotive" + (fnctName.empty() ? "" : (" : " + fnctName))) << resultString;
	return false;
}

// --------------------------------------------------------------
bool ofxMotive::initialize() {
	return isSuccess(TT_Initialize(), "Initialize");
}

// --------------------------------------------------------------
bool ofxMotive::shutdown() {
	// this automatically saves the system calibration file "System Calibration.cal"
	return isSuccess(TT_Shutdown(), "Shutdown");
}

// --------------------------------------------------------------
bool ofxMotive::loadProfile() {
	if (profilePath == "") return false;
	return isSuccess(TT_LoadProfile(profilePath.c_str()), "Load Profile");
}

// --------------------------------------------------------------
bool ofxMotive::loadCalibration() {
	if (calibrationPath == "") return false;
	return isSuccess(TT_LoadCalibration(calibrationPath.c_str()), "Load Calibration");
}

// --------------------------------------------------------------
void ofxMotive::threadedFunction() {

	while (isThreadRunning()) {

		// Update parameters if any have changed
		if (bForceTryConnect) {
			bForceTryConnect = false;
			RUI_PUSH_TO_CLIENT();
			startTryConnect();
		}
		if (bForceLoadProfile) {
			bForceLoadProfile = false;
			RUI_PUSH_TO_CLIENT();
			loadProfile();
		}
		if (bForceDisconnect) {
			bForceDisconnect = false;
			RUI_PUSH_TO_CLIENT();
			disconnect();
		}

		// Do something different depending on the state
		switch (state) {
		case MOTIVE_TRY_CONNECT: {

			// Try to connect every timestep
			unsigned long thisTime = ofGetElapsedTimeMillis();
			if (thisTime - lastTryConnectTime >= tryConnectTimestep) {
				lastTryConnectTime = thisTime;
				tryConnectCounter++;

				// Try to initialize the system
				if (initialize()) {
					ofLogNotice("ofxMotive") << "Motive has connected";
					setState(MOTIVE_CONNECTED);
					loadProfile(); // load profile
					loadCalibration();
					update(); // init camera list
				}
			}

			// If we've surpassed the limit, then stop trying to connect
			if (tryConnectCounter >= maxTryConnectAttempts) {
				ofLogNotice("ofxMotive") << "Exceeded max number of connection attempts. Exiting.";
				setState(MOTIVE_DISCONNECTED);
			}

		}; break;
		case MOTIVE_CONNECTED: {

			// Get all new information from the cameras
			processNewData();

		}; break;
		case MOTIVE_DISCONNECTED: default: {

			// Don't run this thread too much if disconnected
			ofSleepMillis(200);

		}; break;
		}
	}
}

// --------------------------------------------------------------
bool ofxMotive::update(bool bSingleFrame) {
	if (bSingleFrame) {
		return TT_UpdateSingleFrame() == NPRESULT_SUCCESS;
	}
	else {
		return TT_Update() == NPRESULT_SUCCESS;
	}
}

// --------------------------------------------------------------
void ofxMotive::processNewData() {

	// While we can update the camera feed, update the information we're receiving
	while (update(bProcessAllFrames)) {

		// Update the camera information (fps, serial numbers, etc.)



		// Get the 3D information
		// TT_FrameMarkerCount()
		// x,y,z,residual,label
		// which cameras are contributing?


		// lock
		// store the 3D info in a queue
		// unlock


	}
}

// --------------------------------------------------------------
void ofxMotive::updateCameraInfo() {
	
	// Get the total number of cameras
	//int nCameras = 
	//for (int index = 0; index < )


	// If we have a different number of cameras, update the list we're storing

	// Once a second, match the serial numbers


	// Iterate through all cameras



}

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------