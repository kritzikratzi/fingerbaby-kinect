#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxRemoteUIServer.h"
#include "constants.h"
#include "fingerShape.h"
#include "fingerPlayer.h" 


// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void drawPointCloud();
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void audioOut(float * output, int bufferSize, int nChannels ); 
	
	ofxKinect kinect;
	
#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
	
	ofxCvContourFinder contourFinder;
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	int triggerThreshold;
	int step; 
	int angle;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
	
	float minFreq, maxFreq; 
	
	vector<ofPoint> coords;
	fingerPlayer player;
	fingerShape shape;
	
	bool drawEllipses;
	int maxY;
	
	float offY; 
};
