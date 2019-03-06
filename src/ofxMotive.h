#pragma once

#include "ofMain.h"

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NPTrackingTools.h"

#include "ofxRemoteUIServer.h"


enum MotiveState {
	MOTIVE_DISCONNECTED = 0,
	MOTIVE_CONNECTING,
	MOTIVE_CONNECTED,
};

class ofxMotive : private ofThread {
public:

	/// \brief Setup the RUI Params associated with this addon
	void setupParams();

	/// \brief Begin streaming and reconstructing information from the cameras
	void start();

	/// \brief Stop streaming and reconstructing
	void stop();

	/// \brief Get the state of this addon
	MotiveState getState();

	/// \brief Get the number of connected cameras
	void getNumCameras();
	
	/// \brief Get a pointer to all camera objects
	CameraList* getCameras();

	/// \brief Get the observed 2D points from a specific camera feed
	vector<glm::vec2> get2DPoints(int camID);

	/// \brief Get the reconstructed 3D points
	vector<glm::vec3> get3DPoints();


private:

	CameraList cams;




};