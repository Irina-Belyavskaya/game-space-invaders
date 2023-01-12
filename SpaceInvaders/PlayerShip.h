#pragma once
#include "GameObject.h"
#include "GameObjectType.h"

class PlayerShip : public GameObject
{
	public:
		PlayerShip() {};
		PlayerShip(const wchar_t* pathToImage, const wchar_t* pathToMask, int speed) {
			imagePath = pathToImage;
			maskPath = pathToMask;
			objectSpeed = speed;
			objectType = GameObjectType::playerShip;
		};
		
};

