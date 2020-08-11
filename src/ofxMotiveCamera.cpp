#include "ofxMotiveCamera.h"

// -----------------------------------------------------------
MotiveCamera::MotiveCamera() {

}

// -----------------------------------------------------------
MotiveCamera::~MotiveCamera() {

}

// -----------------------------------------------------------
void MotiveCamera::setupParams() {

	RUI_NEW_GROUP("Camera " + ofToString(serial));
	RUI_SHARE_PARAM_WCN("ID "+fpt(), ID, -1, 1000);
	RUI_SHARE_PARAM_WCN("Connected "+fpt(), bConnected);
	string cameraStateNames[] = { "Enabled","Disabled for Reconstruction", "Disabled","States Count" };
	RUI_SHARE_ENUM_PARAM_WCN("State "+fpt(),camState, eCameraStates::Camera_Enabled, eCameraStates::CameraStatesCount, cameraStateNames);
	string cameraModeNames[] = {"Invalid", "Segment", "Raw Grayscale", "Object", "Precision", "MJPEG"};
	RUI_SHARE_ENUM_PARAM_WCN("Mode "+fpt(),camMode, MOTIVE_CAMERA_MODE_INVALID, MOTIVE_CAMERA_MODE_MJPEG, cameraModeNames);
	RUI_SHARE_PARAM_WCN("Exposure "+fpt(), exposure, 1, 480);
	RUI_SHARE_PARAM_WCN("Threshold "+fpt(), threshold, 0, 255);
	RUI_SHARE_PARAM_WCN("IR Intensity "+fpt(), intensity, 0, 15);
	RUI_SHARE_PARAM_WCN("Frame Rate "+fpt(), frameRate, 1, 240);
	RUI_SHARE_PARAM_WCN("Imager Gain "+fpt(), imagerGain, 1, 100); // not sure what this scale is
	RUI_SHARE_PARAM_WCN("MJPEG Quality "+fpt(), mjpegQuality, 0, 100);
	// Push settings to this camera
	RUI_SHARE_PARAM_WCN("Push camera settings "+fpt(), bPushSettings);

	// Draw a camera feed
	// Set a mask
}

// -----------------------------------------------------------
void MotiveCamera::clearFrameData() {
	markers2DRaw.clear();
}

// -----------------------------------------------------------
bool MotiveCamera::pushSettings() {

	bool success = true;
	if (!TT_SetCameraSettings(
		index,
		getVideoType(),
		CLAMP(exposure, 1, 480),
		CLAMP(threshold, 0, 255),
		CLAMP(intensity, 0, 15))) {
		ofLogNotice("ofxMotive : Camera " + ofToString(serial)) << "Could not set camera video type, exposure, threshold or intensity";
		success &= false;
	}
	if (!TT_SetCameraFrameRate(index, frameRate)) {
		ofLogNotice("ofxMotive : Camera " + ofToString(serial)) << "Could not set frame rate";
		success &= false;
	}
	if (!TT_SetCameraMJPEGHighQuality(index, CLAMP(mjpegQuality, 0, 100))) {
		ofLogNotice("ofxMotive : Camera " + ofToString(serial)) << "Could not set MJPEG quality";
		success &= false;
	}
	TT_SetCameraImagerGain(index, imagerGain);
	if (!TT_SetCameraState(index, camState)) {
		ofLogNotice("ofxMotive : Camera " + ofToString(serial)) << "Could not set camera state";
		success &= false;
	}

	// set mask
	// save a frame

	return success;
}

// -----------------------------------------------------------
void MotiveCamera::setCameraMode(MotiveCameraMode _mode) {
	camMode = _mode;
}

// -----------------------------------------------------------
void MotiveCamera::setVideoType(int _type) {

	switch (_type) {
	case -1: setCameraMode(MOTIVE_CAMERA_MODE_INVALID); break;
	case 0: setCameraMode(MOTIVE_CAMERA_MODE_SEGMENT); break;
	case 1: setCameraMode(MOTIVE_CAMERA_MODE_RAW_GRAYSCALE); break;
	case 4: setCameraMode(MOTIVE_CAMERA_MODE_PRECISION); break;
	case 6: setCameraMode(MOTIVE_CAMERA_MODE_MJPEG); break;
	case 2: default: setCameraMode(MOTIVE_CAMERA_MODE_OBJECT); break;
	}
}

// -----------------------------------------------------------
int MotiveCamera::getVideoType() {

	switch (camMode) {
	case MOTIVE_CAMERA_MODE_SEGMENT: return 0; break;
	case MOTIVE_CAMERA_MODE_RAW_GRAYSCALE: return 1; break;
	case MOTIVE_CAMERA_MODE_PRECISION: return 4; break;
	case MOTIVE_CAMERA_MODE_MJPEG: return 6; break;
	case MOTIVE_CAMERA_MODE_OBJECT: default: return 2; break;
	}
}

// -----------------------------------------------------------
MotiveCameraMode MotiveCamera::getCameraMode() {
	return camMode;
}

// -----------------------------------------------------------

string MotiveCamera::fpt() {
	return "[" + ofToString(serial) + "]";
}

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------