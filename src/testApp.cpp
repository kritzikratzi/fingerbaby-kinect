#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);

	kinect.setRegistration(true);
	//kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 255;
	farThreshold = 150;
	triggerThreshold = 150;
	step = 50;
	drawEllipses = false;
	maxY = kinect.height;
	minFreq = 50;
	maxFreq = 2000;
	offY = 0; 
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
	
	
	OFX_REMOTEUI_SERVER_SETUP(10000); 	//start server
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(ofColor(0,255,0,100));
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR(); // set a bg color for the upcoming params
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("preprocess"); //make a new group
	OFX_REMOTEUI_SERVER_SHARE_PARAM(nearThreshold, 0, 255 ); //add an "x" float param to this group.
	OFX_REMOTEUI_SERVER_SHARE_PARAM(farThreshold, 0, 255);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(triggerThreshold, 0, 255);
	
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(ofColor(255,0,0,100));
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("analyze");
	OFX_REMOTEUI_SERVER_SHARE_PARAM(step, 1, 100);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(maxY, 1, kinect.height );
	
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(ofColor(0,150,255,100));
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("display"); //make a new group
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawEllipses);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(offY, 0, ofGetWidth()/2 );
	OFX_REMOTEUI_SERVER_LOAD_FROM_XML();	//load values from XML, if you want to do so

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_COLOR(ofColor(255,255,0,100));
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("synthesis"); //make a new group
	OFX_REMOTEUI_SERVER_SHARE_PARAM(minFreq, 20, 500);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(maxFreq, 500, 5000);
	
	

	ofSoundStreamSetup( 2, 0, SAMPLE_RATE, BUFFER_SIZE, NUM_BUFFERS ); 
}

//--------------------------------------------------------------
void testApp::update() {
	OFX_REMOTEUI_SERVER_UPDATE(0.016);
	
	ofBackground(100, 100, 100);
	
	kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {
		
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
		// or we do it ourselves - show people how they can work with the pixels
		unsigned char * pix = grayImage.getPixels();
		
		int numPixels = grayImage.getWidth() * grayImage.getHeight();
		for(int i = 0; i < numPixels; i++) {
			if(pix[i] < nearThreshold && pix[i] > farThreshold) {
				pix[i] = 255;
			} else {
				pix[i] = 0;
			}
		}
		
		coords.clear();
		float S = ofGetWidth()/grayImage.getWidth();
		for( int x = 0; x < grayImage.getWidth(); x ++ ){
			for( int y = 0; y < grayImage.getHeight() && y < maxY; y ++ ){
				int i = x + y*grayImage.getWidth();
				if( pix[i] > triggerThreshold ){
					
					coords.push_back( ofPoint( x*S, y*S ) );
					x += step;
					y = grayImage.getHeight();
				}
			}
		}
		
		fingerShape shape( coords, ofGetWidth(), ofGetHeight(), minFreq, maxFreq );
		player.setShape( shape ); 
		
		// update the cv images
		grayImage.flagImageChanged();
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground( 0 );
	ofSetColor(255, 255, 255);
	
//		kinect.drawDepth(10, 10, 400, 300);
//		kinect.draw(420, 10, 400, 300);
//		
//		grayImage.draw(10, 320, 400, 300);
//		contourFinder.draw(10, 320, 400, 300);

	ofTranslate( 0, offY );
	vector<ofPoint>::iterator it = coords.begin();
	ofSetColor( 255 );
	ofFill();
	if( drawEllipses ){
//		ofPushMatrix();
//		ofScale( ofGetWidth()/grayImage.getWidth(),ofGetWidth()/grayImage.getWidth() );
		while( it != coords.end() ){
			ofCircle( (*it).x, (*it).y, 4 );
			++it;
		}
//		ofPopMatrix();
	}
	
	player.draw( ofGetWidth(), ofGetHeight() );
}


//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
	OFX_REMOTEUI_SERVER_CLOSE();		//setop the server
	OFX_REMOTEUI_SERVER_SAVE_TO_XML();	//save values to XML
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case '1':
			kinect.setLed(ofxKinect::LED_GREEN);
			break;
			
		case '2':
			kinect.setLed(ofxKinect::LED_YELLOW);
			break;
			
		case '3':
			kinect.setLed(ofxKinect::LED_RED);
			break;
			
		case '4':
			kinect.setLed(ofxKinect::LED_BLINK_GREEN);
			break;
			
		case '5':
			kinect.setLed(ofxKinect::LED_BLINK_YELLOW_RED);
			break;
			
		case '0':
			kinect.setLed(ofxKinect::LED_OFF);
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

void testApp::audioOut(float * output, int bufferSize, int nChannels ){
	memset( output, 0, nChannels*BUFFER_SIZE*sizeof(float) );
	player.audioOut( output, bufferSize, nChannels, false );
}
