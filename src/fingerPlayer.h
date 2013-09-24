//
//  fingerPlayer.h
//  Fingerbaby
//
//  Created by Hansi on 20.09.13.
//
//

#pragma once

#include "ofMain.h" 
#include "fingerShape.h" 
#include "tones.h"
#include "constants.h" 

class fingerPlayer{
	
public:
	fingerPlayer() : nextShape(fingerShape()), currShape(fingerShape()), lastShape(fingerShape()){};
	void setShape( const fingerShape &shape );
	void audioOut(float * output, int bufferSize, int nChannels, bool chromatic );
	void draw( float ofWidth, float ofHeight );

	fingerShape nextShape, currShape, lastShape;
	float phase;
	float phaseAdder = 0;
	float phaseAdderTarget = 0;
};