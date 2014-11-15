#pragma once

#include "ofxNI2.h"
#include "ofxCv.h"
#include "ofxCvColorImage.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvContourFinder.h"

class CameraDevice
{
public:
	struct Setting {
		int left, right, top, bottom;
		float near, far;
		ofRectangle stage;
		float min_area, max_area;
		float simplify;
		int smoothing_size;
		float smoothing_shape;
	};
	Setting& getSetting() { return setting_; }
	void setup();
	void update();
	void drawDepth();
	void drawOutline();
	void draw();
	int getWidth();
	int getHeight();
private:
	ofxNI2::Device device_;
	ofxNI2::DepthStream depth_;
	Setting setting_;
	
	ofPixels depth_pixels_;
	ofxCvColorImage color_image_;
	ofxCvGrayscaleImage grayscale_image_;
	
	ofFbo frame_buffer_;
	ofxCv::ContourFinder contour_;
	ofxCvContourFinder contour_finder_;
};

class Threshold : public ofxNI2::DepthShader
{
public:
	
	Threshold() : near_value_(50), far_value_(10000)
	,left_(0),right_(640),top_(0),bottom_(480){}
	
	void begin();
	
	void setNear(float val) { near_value_ = val; }
	void setFar(float val) { far_value_ = val; }
	void setLeft(int val) { left_ = val; }
	void setRight(int val) { right_ = val; }
	void setTop(int val) { top_ = val; }
	void setBottom(int val) { bottom_ = val; }
	
	
protected:
	float near_value_, far_value_;
	int left_, right_, top_, bottom_;
	string getShaderCode() const;
};

