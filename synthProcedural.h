#pragma once
#include "ofMain.h"
#include "ofxMaxim.h"
class synthProcedural
{
public:
	synthProcedural();
	~synthProcedural();

	// audio out
	double myOutputSample;

	//	generators
	maxiOsc myNoise;
	maxiOsc sine1;
	maxiOsc sine2;
	maxiOsc sine3;
	float noiseGain;
	float sine1freq;
	float sine2freq;
	float sine3freq;

	//	filters & LFO
	maxiFilter myFilter;
	maxiOsc LFO1;
	float filterCutoff;
	maxiOsc LFO2;
	float LFOvalue;

};

