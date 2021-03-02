#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "Dungeon.h"

//KILL AFTER REVIEW
void Dungeon::tempPopulateDungeon() {
	indexes.clear();
	for (int i = 0; i < nOfRooms; i++) {
		while (unsuccessful) {
			randNum = rand() % 12;
			if (i == 0) {
				indexes.push_back(randNum);
				unsuccessful = false;
			}
			else if (i == 4) {
				indexes.push_back(0);
				unsuccessful = false;
			}
			else {
				for (int j = 0; j < i; j++) {
					if (indexes[j] == randNum) {
						recurring = true;
					}
				}
				if (!recurring) {
					indexes.push_back(randNum);
					unsuccessful = false;
				}
				else {
					recurring = false;
					unsuccessful = true;
				}
			}

		}
		unsuccessful = true;
	}

	for (int i = 0; i < nOfRooms; i++) {
		if (i == 4) {
			currRoomEnemies.push_back(0);
		}
		else {
			currRoomEnemies.push_back(1);
			currRoomEnemies.push_back(indexes[i]);
		}

		enemiesInRooms.push_back(currRoomEnemies);
		currRoomEnemies.clear();
	}
		
}

void Dungeon::setEnemyCount() {
	remainingEnemies = 24 - ((8 - (nOfRooms - 1)) * 3);
}

void Dungeon::populateRoom() {
	
}

void Dungeon::populateDungeon() {
	setEnemyCount();
	for (index = 0; index < nOfRooms; index++) {
		if (index == 4) {
			currRoomEnemies.push_back(0);
			enemiesInRooms.push_back(currRoomEnemies);
			currRoomEnemies.clear();
		}
		else {
			populateRoom();
		}
	}
}



void Dungeon::getPosition() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (map[i][j] == 5) {
				position[0] = i;
				position[1] = j;
			}
		}
	}
	currentRoom = 4;
}




void Dungeon::setDoors() {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			index = map[i][j];
			if (index) {
				index--;

				if (map[i-1][j]) {
					rooms[index][4] = 1;
				}

				if (map[i+1][j]) {
					rooms[index][58] = 1;
				}

				if (map[i][j-1]) {
					rooms[index][27] = 1;
				}

				if (map[i][j+1]) {
					rooms[index][35] = 1;
				}
			}
		}
	}
}

void Dungeon::generateIndexes(){
	for (int i = 0; i < nOfRooms; i++) {
		while (unsuccessful) {
			randNum = 1 + rand() % (roomPool.size() - 1);
			if (i == 0) {
				indexes.push_back(randNum);
				unsuccessful = false;
			}
			else if (i == 4) {
				indexes.push_back(0);
				unsuccessful = false;
			}
			else {
				for (int j = 0; j < i; j++) {
					if (indexes[j] == randNum) {
						recurring = true;
					}
				}
				if (!recurring) {
					indexes.push_back(randNum);
					unsuccessful = false;
				}
				else {
					recurring = false;
					unsuccessful = true;
				}
			}

		}
		unsuccessful = true;
	}
}

void Dungeon::drawRoomsFromPool() {
	generateIndexes();

	for (int i = 0; i < nOfRooms; i++) {
		index = indexes[i];
		rooms.push_back(roomPool[index]);
	}
}

void Dungeon::addToRPool() {
	for (int i = 0; i < 64; i++) {
		currentLine.push_back(readR[i] - '0');
	}
	roomPool.push_back(currentLine);
	currentLine.clear();
}

void Dungeon::getRoomPool() {
	iStream.open("./SGData/roomDatabase.txt");

	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readR, 64);
			addToRPool();
		}
		iStream.close();
	}
	else {
		std::cout << "Could not find roomDatabase.txt";
		exit(1);
	}
}




void Dungeon::getNofRooms() {
	nOfRooms = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (map[i][j]) {
				nOfRooms++;
			}
		}
	}
}




void Dungeon::selectRooms() {
	getNofRooms();
	getRoomPool();
	drawRoomsFromPool();
	setDoors();
}
//stores selected map as map array
void Dungeon::storeAsArray(std::vector<int> m) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			map[i][j] = m[j + (i*6)];
		}
	}
}

//randomly selects one dungeon map to use as game map
void Dungeon::drawMapFromPool() {
	index = rand() % dungeonPool.size();

	storeAsArray(dungeonPool[index]);
}

//converts line to vector and adds to dungeon pool
void Dungeon::addToDPool() {
	for (int i = 0; i < 37; i++) {
		currentLine.push_back(readD[i] - '0');
	}

	dungeonPool.push_back(currentLine);

	currentLine.clear();
}

//reads dungeon layouts from file and stores in dungeon pool
void Dungeon::getDungeonPool() {
	iStream.open("./SGData/dungeonDatabase.txt");
	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readD, 37);
			addToDPool();
		}

		iStream.close();
	}
	else {
		std::cout << "Could not find dungeonDatabase.txt";
		exit(1);
	}
}

//defines map array
void Dungeon::selectMap() {
	getDungeonPool();
	drawMapFromPool();
}




Dungeon::Dungeon()
{
	srand(time(0));

	selectMap();
	selectRooms();
	getPosition();

	//KILL AFTER REVIEW
	tempPopulateDungeon();
}