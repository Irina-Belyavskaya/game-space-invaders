#pragma once
#include "GameObject.h"

class AlienGun : public GameObject
{
	public:
		AlienGun() {};
		AlienGun(const wchar_t* pathToImage, const wchar_t* pathToMask, int speed) {
			imagePath = pathToImage;
			maskPath = pathToMask;
			objectSpeed = speed;
			objectType = GameObjectType::alienGun;
		};
};

