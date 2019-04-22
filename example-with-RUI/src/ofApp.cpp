#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);

	RUI_SETUP();
	motive.setupParams();
	RUI_LOAD_FROM_XML();
	
	// Add a listener for new data 
	ofAddListener(motive.newDataReceived, this, &ofApp::newDataReceived);

	// Setup and start motive
	motive.setCalibrationPath(ofToDataPath("6c_20190418.cal"));
	motive.setProfilePath(ofToDataPath("6c_20190418.motive"));
	motive.start();
}

//--------------------------------------------------------------
void ofApp::update(){

	// We can also call it to receive information ...
	//vector<MotiveOutput> out = motive.get3DPoints();
	//stringstream ss;
	//ss << "Motive Output\n";
	//for (int i = 0; i < out.size(); i++) {
	//	ss << "\t" << out[i].position << "\n";
	//}
	//if (out.size() > 0) cout << ss.str() << endl;

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::newDataReceived(MotiveEventArgs& args) {

	// Receive data from the listener.

	stringstream ss;
	ss << "Motive Output\n";
	for (int i = 0; i < args.markers.size(); i++) {
		ss << "\t" << args.markers[i].position << "\n";
	}
	if (args.markers.size() > 0) cout << ss.str() << endl;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
