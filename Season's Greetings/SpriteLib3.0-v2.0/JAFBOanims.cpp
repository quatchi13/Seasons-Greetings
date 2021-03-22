#include "JAFBOanims.h"

void jAnims::updateJAnim(int entity, int width, int height) {
	frameCount++;
	if (isEvenlyDistributed) {
		if (frameCount >= (totalFrames / files.size())) {
			currentImage++;
			if (currentImage == files.size()) {
				currentImage = 0;
			}
			ECS::GetComponent<Sprite>(entity).LoadSprite(files[currentImage], width, height);
			frameCount = 0;
		}
	}
	else {
		if (frameCount >= framePauses[currentImage]) {
			currentImage++;
			if (currentImage == files.size()) {
				currentImage = 0;
			}
			ECS::GetComponent<Sprite>(entity).LoadSprite(files[currentImage], width, height);
			frameCount = 0;
		}
	}
}

void jAnims::nextFrame(int entity, int width, int height) {
	ECS::GetComponent<Sprite>(entity).LoadSprite(files[currentImage], width, height);
}