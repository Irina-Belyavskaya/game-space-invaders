#pragma once
#include "LevelsTypes.h"
#include <thread>

using namespace std;

class GameSettings
{
	public:

		/*       Images       */
		const wchar_t* playerShipPic = L"images/playerShip.bmp";
		const wchar_t* playerShipMask = L"images/playerShipMask.bmp";

		const wchar_t* alienShipPic = L"images/alien.bmp";
		const wchar_t* alienShipMask = L"images/alienMask.bmp";

		const wchar_t* playerGunPic = L"images/playerGun.bmp";
		const wchar_t* playerGunMask = L"images/gunMask.bmp";

		const wchar_t* alienGunPic = L"images/alienGun.bmp";
		const wchar_t* alienGunMask = L"images/gunMask.bmp";
		/*       Start coordinates    */
		const int alienShipStartX = 500;
		const int alienShipStartY = 0;

		const int playerShipStartX = 500;
		const int playerShipStartY = 400;

		/*       Speed       */
		int alienShipSpeed = 5;
		int playerGunSpeed = 20;

		int alienGunSpeed = 20;
		int playerShipSpeed = 10;

		int gameSpeed = 100;

		chrono::duration<long long, milli> creationSpeed = 2000ms;
		int movementTimerOfAlien = 300;
		int movementTimerOfAlienGun = 100;

		void setLevelSettings(LevelsTypes level);
};

