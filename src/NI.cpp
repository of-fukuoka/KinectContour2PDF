//
//  NI.cpp
//  Capture
//
//  Created by Iwatani Nariaki on 2014/11/13.
//
//

#include "NI.h"

namespace {
	int WIDTH = 640;
	int HEIGHT = 480;
}

void CameraDevice::setup()
{
	if(device_.setup()) {
		if(depth_.setup(device_)) {
			depth_.setSize(WIDTH, HEIGHT);
			depth_.start();
			depth_.setMirror();
			depth_.setupShader<Threshold>();
		}
	}
	contour_.setSortBySize(true);
	contour_.setSimplify(false);
	contour_.setInvert(false);
	contour_.setFindHoles(true);
	frame_buffer_.allocate(WIDTH, HEIGHT, GL_RGB);
	grayscale_image_.allocate(WIDTH, HEIGHT);
}
void CameraDevice::update()
{
	ofPtr<Threshold> shader = depth_.getShader<Threshold>();
	shader->setNear(setting_.near);
	shader->setFar(setting_.far);
	shader->setLeft(setting_.left);
	shader->setRight(setting_.right);
	shader->setTop(setting_.top);
	shader->setBottom(setting_.bottom);
	device_.update();
	depth_.updateTextureIfNeeded();

	frame_buffer_.begin();
	ofClear(0);
	depth_.draw();
	frame_buffer_.end();
	
	ofTexture &depth_texture = frame_buffer_.getTextureReference();
	depth_texture.readToPixels(depth_pixels_);
	frame_buffer_.begin();
	ofClear(0);
	ofPath path;
	color_image_.setFromPixels(depth_pixels_);
	grayscale_image_ = color_image_;
	contour_finder_.findContours(grayscale_image_, setting_.min_area, setting_.max_area, 128, true);
	int contour_size = contour_finder_.blobs.size();
	for(int i = 0; i < contour_size; ++i) {
		ofPolyline poly;
		ofxCvBlob &blob = contour_finder_.blobs[i];
		if(blob.nPts < 3) continue;
		for(int j = 0; j < blob.nPts; ++j) {
			poly.addVertex(blob.pts[j]);
		}
		poly.simplify(setting_.simplify);
		poly = poly.getSmoothed(setting_.smoothing_size, setting_.smoothing_shape);
		if(poly.size() < 3) continue;
		path.moveTo(poly[0]);
		for(int j = 1; j < blob.nPts; ++j) {
			path.lineTo(poly[j]);
		}
		path.close();
	}
	ofPushStyle();
	ofSetColor(ofColor::white);
	path.draw();
	ofFill();
	ofRect(setting_.stage);
	ofPopStyle();
	frame_buffer_.end();
}
void CameraDevice::drawDepth()
{
	depth_.draw();
}
void CameraDevice::draw()
{
	frame_buffer_.draw(0,0);
}

void CameraDevice::drawOutline()
{
	ofTexture &depth_texture = frame_buffer_.getTextureReference();
	depth_texture.readToPixels(depth_pixels_);
	contour_.resetMinArea();
	contour_.resetMaxArea();
	contour_.findContours(depth_pixels_);
	ofPath path;
	path.setFilled(true);
	path.setFillColor(ofColor::black);
	path.setStrokeColor(ofColor::black);
	const vector<ofPolyline> &polylines = contour_.getPolylines();
	for(vector<ofPolyline>::const_iterator it = polylines.begin(); it != polylines.end(); ++it) {
		const ofPolyline &poly = *it;
		int poly_size = poly.size();
		if(poly_size < 3) continue;
		path.moveTo(poly[0]);
		for(int j = 1; j < poly_size; ++j) {
			path.lineTo(poly[j]);
		}
		path.close();
	}
	path.draw();
}

int CameraDevice::getWidth()
{
	return WIDTH;
}
int CameraDevice::getHeight()
{
	return HEIGHT;
}

string Threshold::getShaderCode() const
{
#define _S(src) #src
	
	const char *fs = _S(
						uniform sampler2DRect tex;
						uniform float near_value;
						uniform float far_value;
						uniform float left;
						uniform float right;
						uniform float top;
						uniform float bottom;
						
						void main()
						{
							vec2 coord = gl_TexCoord[0].st;
							if(coord.x < left || right <= coord.x || coord.y < top || bottom <= coord.y) {
								discard;
							}
							float c = texture2DRect(tex, coord).r;
							if(c < near_value || far_value <= c) {
								discard;
							}
							gl_FragColor = vec4(1.);
						}
						);
#undef _S
	
	return fs;
}

void Threshold::begin()
{
	DepthShader::begin();
	setUniform1f("near_value", near_value_);
	setUniform1f("far_value", far_value_);
	setUniform1f("left", left_);
	setUniform1f("right", right_);
	setUniform1f("top", top_);
	setUniform1f("bottom", bottom_);
}
