#pragma once
#include "Vector.h"
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"

class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground();

	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	void makeImage(std::string filename, int width, int height, float opacity, float x, float y, float z);

	void makeWall(int index);
	void makeGround(int index);

	void makeShotTrigger(int index);

	int makeEnemy();

	void makeBullet(int index);

	void makeSword();

	void slash();

	void fireBullet();


protected:
	PhysicsPlaygroundListener listener;
	int endCard;

	bool hasStarted = false;
	bool hasEnded = false;

	int spawnCount = 5;
	int totalEnt = 117;
	int tempBullet;

	std::vector <int> enemies{ 0 };
	std::vector <int> activeBullets{ 0 };
	std::vector <int> bulletHold{ 0 };
	std::vector <int> shotTriggers{ 0 };
	std::vector <int> walls;
	std::vector <int> groundTiles;

	int block;

	int backBarrier;
	int frontBarrier;
	int melee;
	int score = 0;

	int positionArray[5][2]{
		{440, 0},
		{860, 20},
		{1280, -30},
		{1690, -30},
		{2120, 70}
	};

	int floor[63]
	{0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 0, 1, 1, 1, 1, 1, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0};



};
