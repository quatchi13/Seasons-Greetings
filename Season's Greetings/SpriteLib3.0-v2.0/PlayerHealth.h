#pragma once
class PlayerHealth
{
public:
	bool hasBeenDamaged = false;
	int health = 10;
	bool deceased = false;
	void CheckPlayerStatus();
};

