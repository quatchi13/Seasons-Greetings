#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "Dungeon.h"

void Dungeon::setPositions() {
	for (int i = 0; i < nOfRooms; i++) {
		if (enemiesInRooms[i][0]) {
			for (int j = 0; j < (enemiesInRooms[i][0]); j++) {
				if (j == 0) {
					if (rooms[i][22]) {
						enemiesInRooms[i].push_back(22);
					}else if (rooms[i][21]) {
						enemiesInRooms[i].push_back(21);
					}
					else if (rooms[i][23]) {
						enemiesInRooms[i].push_back(23);
					}
					else if (rooms[i][12]) {
						enemiesInRooms[i].push_back(12);
					}
					else {
						enemiesInRooms[i].push_back(14);
					}
				}
				else if (j == 1) {
					if (rooms[i][40]) {
						enemiesInRooms[i].push_back(40);
					}
					else if (rooms[i][41]) {
						enemiesInRooms[i].push_back(41);
					}
					else if (rooms[i][39]) {
						enemiesInRooms[i].push_back(39);
					}
					else if (rooms[i][50]) {
						enemiesInRooms[i].push_back(50);
					}
					else {
						enemiesInRooms[i].push_back(48);
					}
				}
				else if (j == 2) {
					if (rooms[i][30]) {
						enemiesInRooms[i].push_back(30);
					}
					else if (rooms[i][20]) {
						enemiesInRooms[i].push_back(20);
					}
					else if (rooms[i][11]) {
						enemiesInRooms[i].push_back(11);
					}
					else if (rooms[i][10]) {
						enemiesInRooms[i].push_back(10);
					}
					else {
						enemiesInRooms[i].push_back(19);
					}
				}
				else if (j == 3) {
					if (rooms[i][29]) {
						enemiesInRooms[i].push_back(29);
					}
					else if (rooms[i][38]) {
						enemiesInRooms[i].push_back(38);
					}
					else if (rooms[i][47]) {
						enemiesInRooms[i].push_back(47);
					}
					else if (rooms[i][46]) {
						enemiesInRooms[i].push_back(46);
					}
					else {
						enemiesInRooms[i].push_back(37);
					}
				}
				else if (j == 4) {
					if (rooms[i][32]) {
						enemiesInRooms[i].push_back(32);
					}
					else if (rooms[i][24]) {
						enemiesInRooms[i].push_back(24);
					}
					else if (rooms[i][15]) {
						enemiesInRooms[i].push_back(15);
					}
					else if (rooms[i][16]) {
						enemiesInRooms[i].push_back(16);
					}
					else {
						enemiesInRooms[i].push_back(25);
					}
				}
				else {
					if (rooms[i][33]) {
						enemiesInRooms[i].push_back(33);
					}
					else if (rooms[i][42]) {
						enemiesInRooms[i].push_back(42);
					}
					else if (rooms[i][51]) {
						enemiesInRooms[i].push_back(51);
					}
					else if (rooms[i][52]) {
						enemiesInRooms[i].push_back(52);
					}
					else {
						enemiesInRooms[i].push_back(43);
					}
				}
			}
		}
	}
}

void Dungeon::setEnemyCount() {
	indexes.clear();

	for (int i = 0; i < nOfRooms; i++) {
		if (i == 4) {
			indexes.push_back(0);
		}
		else {
			indexes.push_back(3);
		}
	}

	int iterations = (4 + (rand() % 3));

	for (int i = 0; i < iterations; i++) {
		int numA = 0;

		do {
			numA = rand() % nOfRooms;
		} while (indexes[numA] < 3 || numA == 4);

		int numB = numA;

		do {
			numB = rand()% nOfRooms;
		} while (numB == numA || (numB == 4 || indexes[numB] > 5));

		indexes[numA]--;
		indexes[numB]++;

		unsuccessful = true;
	}
}

void Dungeon::populateRooms() {
	for (int i = 0; i < nOfRooms; i++) {
		currRoomEnemies.push_back(indexes[i]);
		if (indexes[i]) {
			for (int j = 0; j < indexes[i]; j++) {
				while (unsuccessful) {
					randNum = rand() % 12;
					if (j == 0) {
						currRoomEnemies.push_back(randNum);
						unsuccessful = false;
					}else {
						for (int k = 1; k <= j; k++) {
							if (currRoomEnemies[k] == randNum) {
								recurring = true;
							}
						}
						if (!recurring) {
							currRoomEnemies.push_back(randNum);
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

		enemiesInRooms.push_back(currRoomEnemies);
		currRoomEnemies.clear();
	}
}

void Dungeon::populateDungeon() {
	setEnemyCount();
	populateRooms();
	setPositions();
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
	populateDungeon();
}