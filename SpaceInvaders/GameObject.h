#pragma once
#include "GameObjectType.h"
#include <Windows.h>

using namespace std;

class GameObject
{
	protected:
		const wchar_t* imagePath;
		const wchar_t* maskPath;

		GameObjectType objectType;

		HANDLE objectPictureHandler;
		HANDLE objectMaskPictureHandler;

		BITMAP objectBmp;
		BITMAP objectMaskBmp;

		int objectSpeed;
	public:
		RECT objectCoordinates;

		virtual void setObjectCoordinates(int x, int y);

		virtual GameObjectType getObjectType();
		
		virtual void loadImage();
		virtual void drawObject(HDC* hdcPicture, HDC* hdcBuffer);
		virtual int getSpeed();
};
