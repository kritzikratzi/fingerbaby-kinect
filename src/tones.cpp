//
//  tones.h
//  ios-0073
//
//  Created by Hansi on 15.08.13.
//
//

// from soundythingie ...
static float toneFrequencies[] = { 33, 34.38, 37.13, 39.6, 41.25, 44, 45.83, 49.5, 52.8, 55, 59.4, 61.88, 66, 68.75, 74.25, 79.2, 82.5, 88, 91.67, 99, 105.6, 110, 118.8, 123.75, 132, 137.5, 148.5, 158.4, 165, 176, 183.33, 198, 211.2, 220, 237.6, 247.5, 264, 275, 297, 316.8, 330, 352, 366.67, 396, 422.4, 440, 475.2, 495, 528, 550, 594, 633.6, 660, 704, 733.33, 792, 844.8, 880, 950.4, 990, 1056, 1100, 1188, 1267.2, 1320, 1408, 1466.67, 1584, 1689.6, 1760, 1900.8, 1980, 2112, 2200, 2376, 2534.4, 2640, 2816, 2933.33, 3168, 3379.2, 3520, 3801.6, 3960, 4224, 4400, 4752, 5068.8, 5280, 5632, 5866.67, 6336, 6758.4, 7040, 7603.2, 7920, 8448, 8800, 9504 };
float nearestChromatic( float val ){
	int lower = 0;
	int upper = sizeof( toneFrequencies )/sizeof(*toneFrequencies) - 1;
	int i = 0, nextI = ( lower + upper )/2;
	while( i != nextI ){
		i = nextI;
		if( val < toneFrequencies[i] ) upper = i;
		else lower = i;
		nextI = ( lower + upper )/2;
	}
	
	return toneFrequencies[i];
}
