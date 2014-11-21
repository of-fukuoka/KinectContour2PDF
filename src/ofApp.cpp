#include "ofApp.h"

namespace {
	float SCREEN_X = 0;
	float SCREEN_Y = 0;
	float SCREEN_SCALE_X = 1;
	float SCREEN_SCALE_Y = 1;
}

//--------------------------------------------------------------
void ofApp::setup(){
	camera_.setup();
	CameraDevice::Setting &setting = camera_.getSetting();
	
	param_.setup("setting");
	param_.beginGroup("screen");
	param_.addSlider("x", SCREEN_X, 0.f, (float)ofGetWidth());
	param_.addSlider("y", SCREEN_Y, 0.f, (float)ofGetHeight());
	param_.addSlider("scale x", SCREEN_SCALE_X, -2.f, 2.f);
	param_.addSlider("scale y", SCREEN_SCALE_Y, -2.f, 2.f);
	param_.endGroup();
	param_.beginGroup("camera");
	param_.addSlider("left", setting.left, 0, camera_.getWidth());
	param_.addSlider("right", setting.right, 0, camera_.getWidth());
	param_.addSlider("top", setting.top, 0, camera_.getHeight());
	param_.addSlider("bottom", setting.bottom, 0, camera_.getHeight());
	param_.addSlider("near", setting.near, 0.f, 0.2f);
	param_.addSlider("far", setting.far, 0.f, 0.2f);
	param_.addSlider("min area", setting.min_area, 0.f, 10000.f);
	param_.addSlider("max area", setting.max_area, 0.f, 1000000.f);
	param_.addSlider("simplify", setting.simplify, 0.f, 1.f);
	param_.addSlider("smoothing size", setting.smoothing_size, 0, 100);
	param_.addSlider("smoothing shape", setting.smoothing_shape, 0.f, 1.f);
	param_.endGroup();
	param_.beginGroup("stage");
	param_.addSlider("x", setting.stage.x, 0.f, (float)camera_.getWidth());
	param_.addSlider("y", setting.stage.y, 0.f, (float)camera_.getWidth());
	param_.addSlider("width", setting.stage.width, 0.f, (float)camera_.getHeight());
	param_.addSlider("height", setting.stage.height, 0.f, (float)camera_.getHeight());
	param_.endGroup();
	param_.load();
}

//--------------------------------------------------------------
void ofApp::update(){
	camera_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(128);
	ofPushMatrix();
	ofTranslate(SCREEN_X, SCREEN_Y);
	ofScale(SCREEN_SCALE_X, SCREEN_SCALE_Y);
	camera_.draw();
	ofPopMatrix();
	if(param_.isOpen()) {
		param_.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'f':
			ofSetFullscreen(true);
			break;
		case 'F':
			ofSetFullscreen(false);
			break;
		case OF_KEY_RETURN:
			param_.toggleOpen();
			break;
		case ' ':
			exportImage();
			break;
	}
}
void ofApp::exportImage()
{
	camera_.drawOutline();		// don't know why but it's the way
	pdf_.begin(true, "hoge.pdf");
	ofClear(0);
	camera_.drawOutline();
	pdf_.end();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
