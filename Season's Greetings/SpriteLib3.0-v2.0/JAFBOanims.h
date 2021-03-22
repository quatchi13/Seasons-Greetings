#pragma once
#include "ECS.h"
#include <vector>

//this method of animation is pretty stupid
//it should only be used in extreme instances where the regular animation controller isnt working
//it completely sidesteps all JSON stuff and loads new sprites repeatedly onto an object
//dont use it
class jAnims {
public:
	std::vector<std::string>files;
	std::vector<int>framePauses;
	int totalFrames;
	int currentImage = 0;
	int frameCount = 0;
	bool isEvenlyDistributed = true;

	void updateJAnim(int entity, int w, int h);
	void nextFrame(int entity, int w, int h);
};
