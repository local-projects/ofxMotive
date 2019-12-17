#pragma once

#include "ofMain.h"

// If we're simulating motive, don't load 
//#ifndef SIMULATE_MOTIVE
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "NPTrackingTools.h"
//#endif

#include "ofxRemoteUIServer.h"
#include "MotiveCameraSet.h"
#include "Reconstruction.h"

enum MotiveState {
	MOTIVE_DISCONNECTED = 0,
	MOTIVE_TRY_CONNECT,
	MOTIVE_CONNECTED,
};

// The information output from this addon
struct MotiveOutput {
	glm::vec3 position;
	Core::cUID cuid;
};

// The event args output
class MotiveEventArgs : public ofEventArgs {
public:
	vector<MotiveOutput> markers;
};

class ofxMotive : private ofThread {
public:

	/// \brief Create an object to connect with motive's cameras. There should only be one per program.
	ofxMotive();
	~ofxMotive();

	/// \brief Setup the RUI Params associated with this addon
	void setupParams();
	void setCalibrationPath(string _calibrationPath);
	void setProfilePath(string _profilePath);

	/// \brief Begin streaming and reconstructing information from the cameras
	void start();

	/// \brief Check if we're connected to the cameras and streaming info
	bool isConnected();

	bool isState(MotiveState _state);

	/// \brief Stop streaming and reconstructing
	void stop();

	/// \brief Get the state of this addon
	MotiveState getState();

	/// \brief Get the number of connected cameras
	int getNumCameras();
	
	/// \brief Get a pointer to all camera objects
	MotiveCameraSet* getCameras();

	/// \brief Get the observed 2D points from a specific camera feed
	vector<glm::vec2> get2DPoints(int camID);

	/// \brief Get the reconstructed 3D points
	vector<MotiveOutput> get3DPoints(); // this is the output

	/// \brief Draw any debug info
	//void drawDebugInfo();

	// Event that occurs when new data is received
	ofEvent< MotiveEventArgs > newDataReceived;

private:

	/// \brief Try to connect after some time of being disconnected
	void connect();

	/// \brief Disconnect the system and disable all cameras.
	void disconnect();

	bool isSuccess(NPRESULT result, string fnctName = "");
	//void logResult(NPRESULT result, string functionName = "");

	MotiveState state = MOTIVE_DISCONNECTED;
	void setState(MotiveState _state);

	void threadedFunction();
	bool bBlocking = true;

	//CameraList cams;

	void startTryConnect();
	long lastTryConnectTime = 0;
	int tryConnectTimestep = 3000; // every 3 seconds
	int tryConnectCounter = 0;
	int maxTryConnectAttempts = 5;
	bool bForceTryConnect = false;

	bool bForceDisconnect = false;

	string profilePath = ofToDataPath("profile.motive");; // should be a .motive file (actually xml)
	bool bForceLoadProfile = false;
	bool loadProfile();
	bool bSaveProfile = false;
	bool saveProfile();

	string calibrationPath = ofToDataPath("calibration.cal");; // should be a .cal file
	bool bForceLoadCalibration = false;
	bool loadCalibration();

	bool initialize();
	bool update(bool bSingleFrame = false);
	bool shutdown();

	void processNewData();
	bool bProcessAllFrames = true;

	MotiveCameraSet cameras;

	bool bFlushCameraQueues = false;

	Reconstruction reconstruction;

};