//
//  fingerPlayer.mm
//  Fingerbaby
//
//  Created by Hansi on 20.09.13.
//
//

#include "fingerPlayer.h"

void fingerPlayer::setShape( const fingerShape & shape ){
	nextShape = fingerShape( shape );
}

//--------------------------------------------------------------
void fingerPlayer::audioOut(float * output, int bufferSize, int nChannels, bool chromatic ){
	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-TWO_PI like this:
	while(phase > 1){
		phase -= 1;
	}

	currShape = nextShape;
	//	phaseAdderTarget = nearestChromatic(MAX(1,audioShape.freq)) / (float)SAMPLE_RATE ;
	phaseAdderTarget = MAX(1,chromatic?nearestChromatic(currShape.freq):currShape.freq) / (float)SAMPLE_RATE ;
	//	vector<bezierCurve> segments = cmrKeyframeCurve( audioShape.c, 0.5, false );
	float N = bufferSize;
	if( currShape.freq==0 && currShape.freq==0 ){
		return;
	}
	
	
	//	bool D = debug->selected;
	//	if( D ) cout << "{";
	for(int i = 0; i < N; i++){
		phaseAdder = 0.99f * phaseAdder + 0.001f * phaseAdderTarget;
		phase += phaseAdder;
		if( phase > 1 ) phase -= 1;
		
		float sample = currShape.linear( phase )*i/N + lastShape.linear( phase )*(N-i)/N;
		//cout << sample << endl;
		output[i * nChannels] += sample;
		output[i * nChannels + 1] += sample;
//		if( D && i != N-1 ) cout << sample << ",";
	}
//	if( D ) cout << "}, " << endl;
	
	lastShape = currShape;
}


//--------------------------------------------------------------
void fingerPlayer::draw( float ofWidth, float ofHeight ){
	if( nextShape.empty ) return;
//	float fingerDist = nextShape.c.back().x - nextShape.c[0].x;
//	float dt = coords[0].x/fingerDist;
	ofNoFill(); 
	ofBeginShape();
	for( int x = 0; x < ofWidth; x += 2 ){
		float t = fmod( fmod( (x-nextShape.offX)/nextShape.fingerDist/2, 1 ) + 1, 1 );
		float y = nextShape.linear( t )*ofHeight/2 + ofHeight/2;
		//			float y = shape.linear( t )*ofHeight/2 + ofHeight/2;
		ofVertex( x, y );
	}
	ofEndShape();
}