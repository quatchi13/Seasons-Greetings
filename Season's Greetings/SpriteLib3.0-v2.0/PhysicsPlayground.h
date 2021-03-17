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
	void makeCamFocus();
	void makeWall(int index);
	void makeLockedDoor();

	void makeSpike(int index);
	void makeShooter(int index);
	void makeEnemy(int index);
	void makeClockwiseEnemy(int index);
	void makeChaser(int index);

	void makeBullet(int index);
	void makeHostileBullet();

	void fireBullet();

	void newRoom(int, int);

	void makeDoor(int);

	void fireEnemyBullet(int, int);

	void applyKnockBack();

	void setupRoomEnemies(int);

protected:
	enum gameState {
		STARTSCREEN,
		MENU,
		PLAY,
		END
	};

	int stateOfGame = STARTSCREEN;


	PhysicsPlaygroundListener listener;
	int healthBar;
	int attackFrames = 0;
	bool needToAdd = false;

	bool hasStarted = false;
	bool hasEnded = false;

	int spawnCount = 5;
	int totalEnt = 117;
	int tempBullet;

	std::vector <int> enemies;
	std::vector<int> activeEnemies;
	std::vector<int> hostileBullets;
	std::vector <int> activeBullets{ 0 };
	std::vector <int> bulletHold{ 0 };
	std::vector <int> walls;
	std::vector<int> spikes;
	std::vector<int>allTiles;
	std::vector<int>doors;
	std::vector<int>blockedDoors;

	int groundTile;
	int block;

	int screen;

	
};
