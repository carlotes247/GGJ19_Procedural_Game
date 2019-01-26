#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// Start the game when the app begins
	game_state = "start";
	score = 0;
	// Set up player with radius of 20
	player_1.setup(20);
}

//--------------------------------------------------------------
void ofApp::update() {
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
