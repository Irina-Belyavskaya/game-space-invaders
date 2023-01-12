#pragma once

#include "GameSettings.h"
#include "GameObject.h"

class GameObjectFactory
{
	public:
		GameSettings gameSettings;
		GameObjectFactory(GameSettings gameSettings);
		virtual GameObject getGameObject(GameObjectPlace objectPlace) = 0;
};

