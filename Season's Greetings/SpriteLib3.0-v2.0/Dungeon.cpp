#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#include "Dungeon.h"


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
					randNum = rand() % 16;
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
}



void Dungeon::getPosition(int x) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (map[i][j] == x) {
				position[0] = i;
				startPos[0] = i;
				position[1] = j;
				startPos[1] = j;
			}
		}
	}
	currentRoom = x - 1;

}




void Dungeon::setDoors() {
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
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
			if (map[i][j] > 0) {
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
	std::cout << "done";
}

//randomly selects one dungeon map to use as game map
void Dungeon::drawMapFromPool() {
	index = rand() % dungeonPool.size();

	storeAsArray(dungeonPool[index]);
}

//converts line to vector and adds to dungeon pool
void Dungeon::addToDPool() {
	for (int i = 0; i < 37; i++) {
		int j;
		char x = readD[i];
		switch (x) {
		case('A'):
			j = 10;
			break;
		case('B'):
			j = 11;
			break;
		case('C'):
			j = 12;
			break;
		default:
			j = x- '0';
		}
		
		currentLine.push_back(j);
	}

	dungeonPool.push_back(currentLine);
	std::cout << "dank";
	currentLine.clear();
}

//reads dungeon layouts from file and stores in dungeon pool
void Dungeon::getDungeonPool() {
	std::string sourceFile;
	if (level == 1) {
		sourceFile = "./SGData/dungeonDatabase.txt";
	}
	else if (level == 2) {
		sourceFile = "./SGData/dungeonDatabase2.txt";
	}
	else {
		sourceFile = "./SGData/dungeonDatabase3.txt";
	}
	iStream.open(sourceFile);
	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readD, 37);
			addToDPool();
		}

		iStream.close();
	}
	else {
		std::cout << "Could not find " << sourceFile;
		exit(1);
	}
}

//defines map array
void Dungeon::selectMap() {
	getDungeonPool();
	drawMapFromPool();
}


Dungeon::Dungeon(int l)
{
	level = l;
	srand(time(0));
	isTutorial = false;
	selectMap();
	selectRooms();
	getPosition(5);
	populateDungeon();
	std::cout << "success";
}

//tutorial overload
Dungeon::Dungeon(bool t) {
	level = 0;
	isTutorial = t;
	nOfRooms = 7;
	storeAsArray(tutorialMap);
	loadTutorialRooms();
	setDoors();
	getPosition(6);
	populateTutorial();
}

void Dungeon::loadTutorialRooms() {
	iStream.open("./SGData/tutorialRoomDatabase.txt");

	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream.getline(readR, 64);
			storeRoom();
		}
		iStream.close();
	}
	else {
		std::cout << "Could not find tutorialRoomDatabase.txt";
		exit(1);
	}
	
}

void Dungeon::storeRoom() {
	for (int i = 0; i < 64; i++) {
		currentLine.push_back(readR[i] - '0');
	}
	rooms.push_back(currentLine);
	currentLine.clear(); 
	
}

void Dungeon::populateTutorial() {
	iStream.open("./SGData/tutorialEnemies.txt");
	int line;
	if (iStream.is_open()) {
		while (iStream.good()) {
			iStream >> line;
			currRoomEnemies.push_back(line);
			if (line) {
				for (int i = 0; i < currRoomEnemies[0]; i++) {
					iStream >> line;
					currRoomEnemies.push_back(line);
				}
			}

			enemiesInRooms.push_back(currRoomEnemies);
			currRoomEnemies.clear();
		}

		iStream.close();


		for (int i = 0; i < enemiesInRooms.size(); i++) {
			for (int j = 0; j < enemiesInRooms[i].size(); j++) {
				std::cout << enemiesInRooms[i][j] << ' ';
			}
			std::cout << '\n';
		}




	}
	else {
		std::cout << "Could not find tutorialEnemies.txt";
		exit(1);
	}
}


//this function deletes every data value that does not get overwritten by the constructors
void Dungeon::WipeDungeon() {
	mapCleared = false;
	enemiesInRooms.clear();
	rooms.clear();
	dungeonPool.clear();
	roomPool.clear();
	indexes.clear();
}



//this function is called when frostbite enters a room containing enemies
//it sets the locations of all room enemies, based on which door the room was entered from
//i deeply apologize to anyone who has to read this wretched function
void Dungeon::setPositions(int direction) {
	switch (direction) {
	case 0:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][10] == 1) {
					enemiesInRooms[currentRoom].push_back(10);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(15);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][46] == 1) {
					enemiesInRooms[currentRoom].push_back(46);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][31] == 1) {
					enemiesInRooms[currentRoom].push_back(31);
				}
				else {
					enemiesInRooms[currentRoom].push_back(51);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else {
					enemiesInRooms[currentRoom].push_back(24);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else {
					enemiesInRooms[currentRoom].push_back(42);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][11] == 1) {
					enemiesInRooms[currentRoom].push_back(11);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else {
					enemiesInRooms[currentRoom].push_back(16);
				}
			}
			else {
				if (rooms[currentRoom][47] == 1) {
					enemiesInRooms[currentRoom].push_back(47);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else {
					enemiesInRooms[currentRoom].push_back(52);
				}
			}
		}
		break;

	case 1:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][16] == 1) {
					enemiesInRooms[currentRoom].push_back(16);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(11);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][52] == 1) {
					enemiesInRooms[currentRoom].push_back(52);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][31] == 1) {
					enemiesInRooms[currentRoom].push_back(31);
				}
				else {
					enemiesInRooms[currentRoom].push_back(47);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else {
					enemiesInRooms[currentRoom].push_back(20);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else {
					enemiesInRooms[currentRoom].push_back(38);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][15] == 1) {
					enemiesInRooms[currentRoom].push_back(15);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else {
					enemiesInRooms[currentRoom].push_back(10);
				}
			}
			else {
				if (rooms[currentRoom][51] == 1) {
					enemiesInRooms[currentRoom].push_back(51);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else {
					enemiesInRooms[currentRoom].push_back(46);
				}
			}
		}
		break;
	case 2:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][46] == 1) {
					enemiesInRooms[currentRoom].push_back(46);
				}
				else if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else if (rooms[currentRoom][49] == 1) {
					enemiesInRooms[currentRoom].push_back(49);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(21);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][52] == 1) {
					enemiesInRooms[currentRoom].push_back(52);
				}
				else if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else if (rooms[currentRoom][40] == 1) {
					enemiesInRooms[currentRoom].push_back(40);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(23);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][47] == 1) {
					enemiesInRooms[currentRoom].push_back(47);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else {
					enemiesInRooms[currentRoom].push_back(10);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][51] == 1) {
					enemiesInRooms[currentRoom].push_back(51);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else {
					enemiesInRooms[currentRoom].push_back(16);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][48] == 1) {
					enemiesInRooms[currentRoom].push_back(48);
				}
				else if (rooms[currentRoom][39] == 1) {
					enemiesInRooms[currentRoom].push_back(39);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else {
					enemiesInRooms[currentRoom].push_back(11);
				}
			}
			else {
				if (rooms[currentRoom][50] == 1) {
					enemiesInRooms[currentRoom].push_back(50);
				}
				else if (rooms[currentRoom][41] == 1) {
					enemiesInRooms[currentRoom].push_back(41);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else {
					enemiesInRooms[currentRoom].push_back(15);
				}
			}
		}
		break;

	default:
		for (int j = 0; j < (enemiesInRooms[currentRoom][0]); j++) {
			if (j == 0) {
				if (rooms[currentRoom][10] == 1) {
					enemiesInRooms[currentRoom].push_back(10);
				}
				else if (rooms[currentRoom][19] == 1) {
					enemiesInRooms[currentRoom].push_back(19);
				}
				else if (rooms[currentRoom][13] == 1) {
					enemiesInRooms[currentRoom].push_back(13);
				}
				else if (rooms[currentRoom][30] == 1) {
					enemiesInRooms[currentRoom].push_back(30);
				}
				else {
					enemiesInRooms[currentRoom].push_back(39);
				}
			}
			else if (j == 1) {
				if (rooms[currentRoom][16] == 1) {
					enemiesInRooms[currentRoom].push_back(16);
				}
				else if (rooms[currentRoom][25] == 1) {
					enemiesInRooms[currentRoom].push_back(25);
				}
				else if (rooms[currentRoom][22] == 1) {
					enemiesInRooms[currentRoom].push_back(22);
				}
				else if (rooms[currentRoom][32] == 1) {
					enemiesInRooms[currentRoom].push_back(32);
				}
				else {
					enemiesInRooms[currentRoom].push_back(41);
				}
			}
			else if (j == 2) {
				if (rooms[currentRoom][11] == 1) {
					enemiesInRooms[currentRoom].push_back(11);
				}
				else if (rooms[currentRoom][20] == 1) {
					enemiesInRooms[currentRoom].push_back(20);
				}
				else if (rooms[currentRoom][28] == 1) {
					enemiesInRooms[currentRoom].push_back(28);
				}
				else if (rooms[currentRoom][37] == 1) {
					enemiesInRooms[currentRoom].push_back(37);
				}
				else {
					enemiesInRooms[currentRoom].push_back(46);
				}
			}
			else if (j == 3) {
				if (rooms[currentRoom][15] == 1) {
					enemiesInRooms[currentRoom].push_back(15);
				}
				else if (rooms[currentRoom][24] == 1) {
					enemiesInRooms[currentRoom].push_back(24);
				}
				else if (rooms[currentRoom][34] == 1) {
					enemiesInRooms[currentRoom].push_back(34);
				}
				else if (rooms[currentRoom][43] == 1) {
					enemiesInRooms[currentRoom].push_back(43);
				}
				else {
					enemiesInRooms[currentRoom].push_back(52);
				}
			}
			else if (j == 4) {
				if (rooms[currentRoom][12] == 1) {
					enemiesInRooms[currentRoom].push_back(12);
				}
				else if (rooms[currentRoom][21] == 1) {
					enemiesInRooms[currentRoom].push_back(21);
				}
				else if (rooms[currentRoom][29] == 1) {
					enemiesInRooms[currentRoom].push_back(29);
				}
				else if (rooms[currentRoom][38] == 1) {
					enemiesInRooms[currentRoom].push_back(38);
				}
				else {
					enemiesInRooms[currentRoom].push_back(47);
				}
			}
			else {
				if (rooms[currentRoom][14] == 1) {
					enemiesInRooms[currentRoom].push_back(14);
				}
				else if (rooms[currentRoom][23] == 1) {
					enemiesInRooms[currentRoom].push_back(23);
				}
				else if (rooms[currentRoom][33] == 1) {
					enemiesInRooms[currentRoom].push_back(33);
				}
				else if (rooms[currentRoom][42] == 1) {
					enemiesInRooms[currentRoom].push_back(42);
				}
				else {
					enemiesInRooms[currentRoom].push_back(51);
				}
			}
		}
	}
}