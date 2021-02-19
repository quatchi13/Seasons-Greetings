#pragma once

#include "Trigger.h"

class EnemyShotTrigger : public Trigger
{
public:
	void OnEnter() override;
	void OnExit() override;


	bool defaultSet = true;

	int floor1[63]
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 1, 1, 1, 1, 1, 1, 1, 1,
	 0, 1, 1, 1, 1, 1, 1, 1, 0,
	 0, 0, 1, 1, 1, 1, 1, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int floor2[63]
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0,
	 0, 1, 1, 1, 1, 1, 0, 1, 0,
	 0, 0, 1, 1, 1, 1, 1, 1, 0,
	 1, 1, 1, 0, 1, 1, 1, 1, 0,
	 0, 1, 1, 0, 1, 1, 1, 1, 0,
	 0, 0, 1, 1, 1, 1, 0, 0, 0,
	 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int block;

protected:

};
