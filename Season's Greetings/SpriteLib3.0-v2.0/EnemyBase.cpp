#include "EnemyBase.h"
#include "ECS.h"

bool Enemy::readyToShoot(IsInactive eb) {
	if (eb.m_notInUse) {
		return true;
	}
	else {
		return false;
	}
}

void Enemy::setVel(int x, int y, int z)
{
	eVelo = (vec3(x, y, z));
}

void Enemy::changeDirection() {
	eVelo.x *= -1;
	eVelo.y *= -1;
}

void Enemy::changeClockwiseDirection()
{
    int rando = 10 * (1 + rand() % 5);

    if (count == 1)
    {
        eVelo.x = -rando;
        eVelo.y = -50;
        count++;
    }
    else if (count == 2)
    {
        eVelo.y = -rando;
        eVelo.x = 50;
        count++;
    }
    else if (count == 3)
    {
        eVelo.x = rando;
        eVelo.y = 50;
        count++;
    }
    else
    {
        eVelo.y = rando;
        eVelo.x = -50;
        count = 1;
    }
}


