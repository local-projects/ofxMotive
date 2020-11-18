#include "ofxMotiveCameraSettings.h"

// -----------------------------------------------------------
MotiveCameraSettings::MotiveCameraSettings() {

}

// -----------------------------------------------------------
MotiveCameraSettings::~MotiveCameraSettings() {

}

// -----------------------------------------------------------
void MotiveCameraSettings::shareParams(string fpt) {

	string cameraStateNames[] = { "Enabled","Disabled for Reconstruction", "Disabled","States Count" };
	RUI_SHARE_ENUM_PARAM_WCN("State "+fpt,camState, eCameraStates::Camera_Enabled, eCameraStates::CameraStatesCount, cameraStateNames);
	string cameraModeNames[] = {"Invalid", "Segment", "Raw Grayscale", "Object", "Precision", "MJPEG"};
	RUI_SHARE_ENUM_PARAM_WCN("Mode "+fpt,camMode, MOTIVE_CAMERA_MODE_INVALID, MOTIVE_CAMERA_MODE_MJPEG, cameraModeNames);
	RUI_SHARE_PARAM_WCN("Exposure "+fpt, exposure, 1, 10000); // arbitrary upper bound (upper bounds are tied to specific camera models)
	RUI_SHARE_PARAM_WCN("Threshold "+fpt, threshold, 0, 255);
	RUI_SHARE_PARAM_WCN("IR Intensity "+fpt, intensity, 0, 15);
	RUI_SHARE_PARAM_WCN("Frame Rate "+fpt, frameRate, 1, 480);
	RUI_SHARE_PARAM_WCN("Imager Gain "+fpt, imagerGain, 1, 100); // not sure what this scale is (1-8?)
	RUI_SHARE_PARAM_WCN("MJPEG Quality "+fpt, mjpegQuality, 0, 100);
}

// -----------------------------------------------------------
void MotiveCameraSettings::setCameraMode(MotiveCameraMode _mode) {
	camMode = _mode;
}

// -----------------------------------------------------------
void MotiveCameraSettings::setVideoType(int _type) {

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
int MotiveCameraSettings::getVideoType() {

	switch (camMode) {
	case MOTIVE_CAMERA_MODE_SEGMENT: return 0; break;
	case MOTIVE_CAMERA_MODE_RAW_GRAYSCALE: return 1; break;
	case MOTIVE_CAMERA_MODE_PRECISION: return 4; break;
	case MOTIVE_CAMERA_MODE_MJPEG: return 6; break;
	case MOTIVE_CAMERA_MODE_OBJECT: default: return 2; break;
	}
}

// -----------------------------------------------------------
MotiveCameraMode MotiveCameraSettings::getCameraMode() {
	return camMode;
}

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------

// -----------------------------------------------------------