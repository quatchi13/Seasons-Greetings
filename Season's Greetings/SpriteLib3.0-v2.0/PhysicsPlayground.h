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
	void makeFrame(std::string fileName, int width, int height, vec3 pos);
	void makeJAnimFrame(std::vector<std::string>frames, std::vector<int>pauses, int width, int height, vec3 pos);
	void makeLockedDoor();
	void makeNonDoor();

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
	void initEnemy(int, Enemy);
	void pushBackAnimations(Enemy, int);

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
	std::vector<int>nonDoors;
	std::vector<int>jAnimatedEntities;

	int groundTile;
	int block;

	int screen;
	int nonEnemyJanims = 0;

	std::vector<std::vector<std::string>>enemyAnimations{
		{"aSprites/Bean Shooter/Bean shooter1.png"},
		{"aSprites/Corn knight/CornKnight1.png", "aSprites/Corn knight/CornKnight2.png", "aSprites/Corn knight/CornKnight3.png", "aSprites/Corn knight/CornKnight4.png", "aSprites/Corn knight/CornKnight5.png", "aSprites/Corn knight/CornKnight6.png", "aSprites/Corn knight/CornKnight7.png", "aSprites/Corn knight/CornKnight8.png", "aSprites/Corn knight/CornKnight9.png"},
		{"aSprites/wisp/WilloWisp1.png", "aSprites/wisp/WilloWisp2.png", "aSprites/wisp/WilloWisp3.png", "aSprites/wisp/WilloWisp4.png", "aSprites/wisp/WilloWisp5.png", "aSprites/wisp/WilloWisp6.png", "aSprites/wisp/WilloWisp7.png", "aSprites/wisp/WilloWisp8.png", "aSprites/wisp/WilloWisp9.png", "aSprites/wisp/WilloWisp10.png"},
		{"aSprites/Spectre/Spectre1.png", "aSprites/Spectre/Spectre2.png", "aSprites/Spectre/Spectre.png", "aSprites/Spectre/Spectre4.png"}
	};

	
};
