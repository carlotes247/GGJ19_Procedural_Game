#pragma once
#include "ofMain.h"
class Player
{
public:
	// 2D vector x,y for position
	ofPoint pos;
	// Used for collision detection and movement
	float width, height, speed;
	// Finite num of lives
	int lives;

	// Methods
	void setup(float radius);
	void update();
	void draw();
	

	void calculate_movement();

	Player();
	~Player();
};

