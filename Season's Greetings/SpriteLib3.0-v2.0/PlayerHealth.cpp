#include "PlayerHealth.h"
#include <iostream>

void PlayerHealth::CheckPlayerStatus()
{

	if (health > 0)
	{

	}
	else
	{
		std::cout << "i'm dead";
		deceased = true;
	}

	if (health < 0)
	{
		health = 0;
	}

}