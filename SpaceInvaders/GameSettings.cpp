#include "GameSettings.h"

void GameSettings::setLevelSettings(LevelsTypes level) {
	switch (level)
	{
	case Level_2:
		alienShipPic = L"images/alien2.bmp";
		alienShipMask = L"images/alien2Mask.bmp";
		creationSpeed = 1500ms;
		movementTimerOfAlien = 250;
		movementTimerOfAlienGun = 50;
		break;
	case Level_3:
		alienShipPic = L"images/alien3.bmp";
		alienShipMask = L"images/alien3Mask.bmp";
		creationSpeed = 1000ms;
		movementTimerOfAlien = 200;
		movementTimerOfAlienGun = 50;
		break;
	case Level_4:
		alienShipPic = L"images/alien4.bmp";
		alienShipMask = L"images/alien4Mask.bmp";
		creationSpeed = 500ms;
		movementTimerOfAlien = 100;
		movementTimerOfAlienGun = 25;
		break;
	default:
		break;
	}
}
