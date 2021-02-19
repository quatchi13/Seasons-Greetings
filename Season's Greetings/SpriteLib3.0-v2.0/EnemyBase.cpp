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


