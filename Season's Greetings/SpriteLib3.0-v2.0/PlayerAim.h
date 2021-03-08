#pragma once
#include <vector>

class PlayerAim
{
public:

	bool meleeAttackOn = true;
	
	bool m_isMoving = false;
	char m_dirFacing = 'S';

	std::vector<std::vector<int>> aimingCoords{
		{100, 0},     //right
		{0, 100},     //up
		{-100, 0},    //left
		{0, -100} };  //down


	bool m_hasHit = false;
	bool m_isActive = false;
	int m_curDeg = 0;
	int startDeg = 0;

private:

};