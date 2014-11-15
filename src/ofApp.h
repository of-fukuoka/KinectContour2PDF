#pragma once

#include "ofMain.h"
#include "NI.h"
#include "ofxParamEdit.h"
#include "ofxCairo.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
private:
	void exportImage();
	CameraDevice camera_;
	ofxParamEdit param_;
	ofxCairo pdf_;
};
