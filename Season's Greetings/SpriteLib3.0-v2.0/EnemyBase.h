#pragma once
#include "IsInactive.h"
#include "ECS.h"
class Enemy {
public:

	int health;
	int damage;
	int speed = 0;
	int count = 1;
	bool shoots = false;
	bool moves = false;
	bool ded = false;
	bool active = false;
	bool collided = false;
	bool movesClockwise = false;
	bool chases = false;
	bool animated = true;

	std::string sprite;
	std::vector<std::string>files;

	int totalFrames = 0;
	int frameCount = 0;
	int currentFile = 0;

	vec3 eVelo = (vec3(0, 0, 0));
	vec3 ePosi = (vec3(0, 0, 0));

	bool readyToShoot(IsInactive eB);

	void setVel(int, int, int);
	void changeDirection();
	void changeClockwiseDirection();

	void updateAnim(int);
};