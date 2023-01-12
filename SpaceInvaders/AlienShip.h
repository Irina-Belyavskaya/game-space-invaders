#pragma once
#include "GameObject.h"
#include "GameObjectType.h"

using namespace std;

class AlienShip : public GameObject
{
	public:
		AlienShip(const wchar_t* pathToImage, const wchar_t* pathToMask, int speed) {
			imagePath = pathToImage;
			maskPath = pathToMask;
			objectSpeed = speed;
			objectType = GameObjectType::alienShip;
		};
};

