#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// Start the game when the app begins
	game_state = "start";
	score = 0;
	// Set up player with radius of 20
	player_1.setup(20);

	//	synth

	ofSoundStreamSettings settings;
	settings.numOutputChannels = 2;
	settings.sampleRate = 48000;
	settings.bufferSize = 512;
	settings.numBuffers = 4;
	settings.setOutListener(this);
	soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update() {

	//	synth audio thread
	unique_lock<mutex> lock(audioMutex);

	//	synth geometry drawing
	waveform.clear();
	for (size_t i = 0; i < lastBuffer.getNumFrames(); i++) {
		float sample = lastBuffer.getSample(i, 0);
		float r = ((2 - 2 * sin(i) + sin(i)*sqrt(abs(cos(i))) / (sin(i) + 1.4)) * -80 * (100 * SynthEngine.noiseGain));
		float x = ofGetWidth() / 2 + cos(i) * r * sample * (1 - (SynthEngine.noiseGain + 0.02));
		float y = ofGetHeight() / 2 + sin(i) * r * sample;
		waveform.addVertex(ofVec3f(x, y, 0));
		waveform.close();
	}
	rms = lastBuffer.getRMSAmplitude();

	if (game_state == "start")
	{

	}
	else if (game_state == "game")
	{
		//player_1.update();
	}
	else if (game_state == "end")
	{
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (game_state == "start")
	{

	}
	else if (game_state == "game")
	{
		//player_1.draw();
	}
	else if (game_state == "end")
	{

	}


	// synth draw
	ofBackground(ofColor(100 * SynthEngine.noiseGain + 5, 100 * SynthEngine.noiseGain + 5, 100 * SynthEngine.noiseGain + 5, SynthEngine.noiseGain));
	ofSetColor(ofColor::purple);
	ofSetLineWidth(1 + (rms * SynthEngine.noiseGain) * 0.1);
	waveform.draw();
}


// audio callback
void ofApp::audioOut(ofSoundBuffer &outBuffer) {

	for (size_t i = 0; i < outBuffer.getNumFrames(); i++) {

		SynthEngine.myOutputSample = SynthEngine.myNoise.noise() * (SynthEngine.noiseGain * SynthEngine.LFO1.phasor(4)) * 0.7;
		SynthEngine.myOutputSample = SynthEngine.myOutputSample + (SynthEngine.sine1.sinewave(SynthEngine.sine1freq) * 0.25);
		SynthEngine.myOutputSample = SynthEngine.myOutputSample + ((SynthEngine.sine2.triangle(SynthEngine.sine2freq)*SynthEngine.LFO2.sinewave(SynthEngine.LFOvalue * 10))* 0.25);
		SynthEngine.myOutputSample = SynthEngine.myOutputSample + ((SynthEngine.sine3.saw(SynthEngine.sine3freq)*SynthEngine.LFO2.sinewave(SynthEngine.LFOvalue)) * 0.25);
		SynthEngine.myOutputSample = SynthEngine.myFilter.lores(SynthEngine.myOutputSample, SynthEngine.filterCutoff, 4) * 0.5;
		outBuffer.getSample(i, 0) = SynthEngine.myOutputSample;
		outBuffer.getSample(i, 1) = SynthEngine.myOutputSample;
	}

	unique_lock<mutex> lock(audioMutex);
	lastBuffer = outBuffer;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	// When any key is released...
	if (game_state == "start")
	{
		// We go to the game if we are on the start screen
		game_state = "game";
	}
	else if (game_state == "game")
	{
		// blank for now
		// game logic
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

	//	synth parameter control
	int width = ofGetWidth();
	float height = (float)ofGetHeight();
	float heightPct = ((height - y) / height);
	SynthEngine.noiseGain = heightPct;
	SynthEngine.filterCutoff = 4000 * heightPct + 200;
	SynthEngine.sine1freq = 150 + (10 * heightPct);
	SynthEngine.sine2freq = 50 + (150 * (heightPct * 2));
	SynthEngine.sine3freq = 148 + 10 * heightPct;
	SynthEngine.LFOvalue = 100 + 2000 * heightPct;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
