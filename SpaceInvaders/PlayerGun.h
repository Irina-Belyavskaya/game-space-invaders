#pragma once
#include "GameObject.h"
class PlayerGun : public GameObject
{
	public: 
		PlayerGun() {};
		PlayerGun(const wchar_t* pathToImage, const wchar_t* pathToMask, int speed) {
			imagePath = pathToImage;
			maskPath = pathToMask;
			objectSpeed = speed;
			objectType = GameObjectType::playerGun;
		};
};

