//
//  fingerShape.h
//  Fingerbaby
//
//  Created by Hansi on 20.09.13.
//
//

#pragma once
#include "ofMain.h"


class fingerShape{
public:
	vector<ofPoint> c;
	float offX;
	float fingerDist;
	float freq;
	bool empty = true;
	
	fingerShape(){
		freq = 0;
		empty = true;
		fingerDist = 1;
		offX = 0;
	}
	
	fingerShape( const fingerShape& other ){
		c = other.c;
		freq = other.freq;
		empty = other.empty;
		offX = other.offX;
		fingerDist = other.fingerDist;
	};
	
	
	fingerShape( vector<ofPoint> coords, float ofWidth, float ofHeight, float minFreq, float maxFreq ){
		// wtf magic
		if( coords.size() < 2 ) return;
		empty = false;
		offX = coords[0].x;
		fingerDist = coords.back().x - coords[0].x;
		float dy = coords[0].y + coords.back().y;
		if( dy == 0 ) dy = 1;
		for( int i = 0; i < coords.size()-1; i++ ){
			c.push_back( ofPoint((coords[i].x-offX)/fingerDist/2, 2*(coords[i].y-ofHeight/2)/ofHeight ) );
		}
		for( int i = 0; i < coords.size()-1; i++ ){
			c.push_back( ofPoint( (fingerDist + coords[i].x - offX)/fingerDist/2, (dy - coords[i].y-ofHeight/2)*2/ofHeight ) );
		}
		
		// scale this thing accordingly
		freq = ofMap( fingerDist, 0, ofWidth, maxFreq, minFreq, true );
		
		// hackedy hack
		c.push_back( c[0] + ofPoint( 1,0 ) );
	}
	
	float linear( float t ){
		t = fmod( t, 1 );
		
		for( int i = 1; i < c.size(); i++ ){
			if( c[i].x >= t ){
				return ofMap( t, c[i-1].x, c[i].x, c[i-1].y, c[i].y );
			}
		}
		
		return 0;
	}
};
