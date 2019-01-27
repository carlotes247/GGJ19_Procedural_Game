#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// Start the game when the app begins
	game_state = "start";
	score = 0;
	max_score = 200;
	score_to_add = 10;
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


	/* CODE FOR BOX 2D */
	img.allocate(40, 30, OF_IMAGE_GRAYSCALE);
	color = ofColor(255, 0, 0, 60);

	box2d.init();
	box2d.enableEvents();   // <-- turn on the event listener
	box2d.setGravity(0, 0);
	box2d.setFPS(60.0);
	box2d.registerGrabbing();

	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);

	//Make some random circles on Setup

	for (int i = 0; i < 20; i++) {
		shared_ptr <ofxBox2dCircle> c = shared_ptr <ofxBox2dCircle>(new ofxBox2dCircle);
		c.get()->setPhysics(1, 0.5, 0.9);
		c.get()->setup(box2d.getWorld(), (ofGetWidth() / 2) + ofRandom(-300, 300), (ofGetHeight() / 2) + ofRandom(-300, 300), 30);

		c.get()->setData(new HitData());
		HitData * sd = (HitData*)c.get()->getData();
		sd->bHit = false;
		sd->bIsPlayer = false;
		sd->bRend = true;
		sd->refCirc = c;
		sd->colourID = rand() % 5;
		circles.push_back(c);
	}

	//Make player circle on Setup
	playerCircle = shared_ptr <ofxBox2dCircle>(new ofxBox2dCircle);
	playerCircle.get()->setPhysics(1, 0.5, 0.9);
	playerCircle.get()->setup(box2d.getWorld(), (ofGetWidth() / 2), (ofGetHeight() / 2), 30);//ofRandom(20, 50)
	playerCircle.get()->setData(new HitData());
	HitData * hd = (HitData*)playerCircle.get()->getData();
	hd->bHit = false;
	hd->bIsPlayer = true;
	hd->bRend = true;
	hd->refCirc = playerCircle;
	hd->colourID = rand() % 5;
	circles.push_back(playerCircle);

}

//--------------------------------------------------------------
void ofApp::contactStart(ofxBox2dContactArgs &e) {
	if (e.a != NULL && e.b != NULL) {

		// we can change the shape here if we want to.

		if (e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {

			HitData * aData = (HitData*)e.a->GetBody()->GetUserData();
			HitData * bData = (HitData*)e.b->GetBody()->GetUserData();
			if (aData) {
				//check to see if bdata is player and check if it is touching me.
				if (bData && bData->bIsPlayer &&aData->bRend) {
					shared_ptr<ofxBox2dCircle> c = bData->refCirc;
					c->setRadius(c->getRadius() + (float)10);
					aData->bHit = true;
					aData->bRend = false;

					score += score_to_add;
				}
			}

			if (bData) {
				//ditto, but for adata.
				if (aData && aData->bIsPlayer && bData->bRend) {
					shared_ptr<ofxBox2dCircle>  c = aData->refCirc;
					c->setRadius(c->getRadius() + (float)10);
					bData->bHit = true;
					bData->bRend = false;
				}
			}

		}
	}
}


//--------------------------------------------------------------
void ofApp::contactEnd(ofxBox2dContactArgs &e) {
	/*
	if (e.a != NULL && e.b != NULL) {
		if (e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {

			HitData * aData = (HitData*)e.a->GetBody()->GetUserData();
			HitData * bData = (HitData*)e.b->GetBody()->GetUserData();

			if (aData) {
				aData->bHit = false;
			}

			if (bData) {
				bData->bHit = false;
			}

		}
	}
	*/
}


//--------------------------------------------------------------
void ofApp::update() {

	if (game_state == "start")
	{
		score = 0;

	}
	else if (game_state == "game")
	{
		player_1.update();

		// We check if the score is above the max value
		if (score >= max_score)
		{
			game_state = "end";
		}

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


		/* BOX 2D CODE */

	//playerCircle.get()->addAttractionPoint(mouseX, mouseY, 10.0F);
		playerCircle.get()->setPosition(mouseX, mouseY);
		box2d.update();

		float noiseScale = 0.5; //ofMap(mouseX, 0, ofGetWidth(), 0, 0.1);
		float noiseVel = ofGetElapsedTimef();

		//ofPixels & pixels = img.getPixels();

		int w = img.getWidth();
		int h = img.getHeight();
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int i = y * w + x;
				float noiseVelue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
				img.getPixels()[i] = 255 * noiseVelue;
			}
		}
		img.update();
		/*
		int i = 0;
		while (i < img.getPixels().size()) {
			img.getPixels()[i] = ofNoise(rand() % ofGetWidth(),rand() % ofGetHeight()) * 255.f; // make some op-art
			i++;
		}
		img.update();
		*/


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
		player_1.draw();

		// synth draw
		ofBackground(ofColor(100 * SynthEngine.noiseGain + 5, 100 * SynthEngine.noiseGain + 5, 100 * SynthEngine.noiseGain + 5, SynthEngine.noiseGain));
		ofSetColor(ofColor::purple);
		ofSetLineWidth(1 + (rms * SynthEngine.noiseGain) * 0.1);
		waveform.draw();

		/* BOX 2D CODE */
		ofSetColor(color);
		//ofBackgroundGradient(ofColor::white, ofColor::black, OF_GRADIENT_CIRCULAR);
		img.draw(0, 0, ofGetWidth(), ofGetHeight());

		for (size_t i = 0; i < circles.size(); i++) {
			ofFill();
			HitData * data = (HitData*)circles[i].get()->getData();
			//int random = rand() % 5;
			int col = 0;
			switch (data->colourID) {
			case 0:
				col = 0x9b42f4;
				break;
			case 1:
				col = 0xd32828;
				break;
			case 2:
				col = 0x66e228;
				break;
			case 3:
				col = 0xefdd15;
				break;
			case 4:
				col = 0x14ef85;
				break;
			case 5:
				col = 0x4368ad;
				break;
			default:
				col = 0xffffff;
				break;
			}
			if (data && data->bHit) ofSetHexColor(0x000000);
			else ofSetHexColor(col); //0x4ccae9

			//if we don't want to show circles, just don't draw them. collision and everything else (should) still works.
			if (data->bRend) {
				circles[i].get()->draw();
			}
			//circles[i].get()->draw();
		}

		stringstream ss;
		ss << "FPS: " << ofToString(ofGetFrameRate(), 0) << endl << endl;
		ofSetHexColor(0xfffff);
		ofDrawBitmapString(ss.str().c_str(), 100, 100);

	}
	else if (game_state == "end")
	{

	}


}


// audio callback
void ofApp::audioOut(ofSoundBuffer &outBuffer) {

	if (game_state == "game")
	{
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

	if (game_state == "game")
	{
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

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	/*	if (button = 0) {
	int r = rand() % 255;
	int g = rand() % 255;
	int b = rand() % 255;
	color = ofColor(r, g, b, 255);
	}*/

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
