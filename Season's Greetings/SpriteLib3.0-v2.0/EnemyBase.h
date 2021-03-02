#pragma once
#include "IsInactive.h"
#include "ECS.h"
class Enemy {
public:

	int health;
	int damage;
	int speed = 0;
	bool shoots = false;
	bool moves = false;
	bool ded = false;
	bool active = false;
	bool collided = false;

	vec3 eVelo = (vec3(0, 0, 0));

	bool readyToShoot(IsInactive eB);

	void setVel(int, int, int);
	void changeDirection();
};