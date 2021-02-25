#pragma once
#include <vector>
class Dungeon {
public:
	int map[6][6];
	int position[2];
	int nOfRooms;
	int currentRoom;
	bool mapCleared;
	std::vector<std::vector<int>>enemiesInRooms;
	std::vector<std::vector<int>>rooms;
	
	Dungeon();
	
	void setToClear();

private:
	std::ifstream iStream;
	char readD[37];
	char readR[64];
	std::vector<int>currentLine;
	int index;
	bool unsuccessful = true;
	bool recurring = false;
	int randNum;
	int eCountForRoom;
	std::vector<int>currRoomEnemies;
	int remainingEnemies;

	std::vector<std::vector<int>>dungeonPool;
	std::vector<std::vector<int>>roomPool;
	std::vector<int>indexes;

	void generateIndexes();
	void getDungeonPool();
	void getRoomPool();

	void selectMap();
	void getPosition();
	void getNofRooms();
	void selectRooms();

	void addToDPool();
	void addToRPool();
	void drawMapFromPool();
	void drawRoomsFromPool();
	void storeAsArray(std::vector<int>);
	void setDoors();

	void populateDungeon();
	void populateRoom();
	void setEnemyCount();
	
};