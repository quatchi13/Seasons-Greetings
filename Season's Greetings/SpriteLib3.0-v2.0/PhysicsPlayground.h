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
	void makeGround();

	void makeSpike(int index);
	void makeEnemy(int index);

	void makeBullet(int index);
	void makeHostileBullet();

	void makeSword();

	void slash();

	void fireBullet();

	void newRoom(int, int);

	void makeDoor(int);

	void fireEnemyBullet(int, int);

	void applyKnockBack();


protected:
	PhysicsPlaygroundListener listener;
	int endCard;
	int attackFrames = 0;
	bool needToAdd = false;

	bool hasStarted = false;
	bool hasEnded = false;

	int spawnCount = 5;
	int totalEnt = 117;
	int tempBullet;

	std::vector <int> enemies;
	std::vector<int> hostileBullets;
	std::vector <int> activeBullets{ 0 };
	std::vector <int> bulletHold{ 0 };
	std::vector <int> shotTriggers{ 0 };
	std::vector <int> walls;
	std::vector<int> spikes;
	std::vector<int>allTiles;
	std::vector<int>doors;

	int groundTile;
	int block;
	int camFocus;

	int melee;
	int score = 0;

	
};
