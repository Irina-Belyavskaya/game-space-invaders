#include "GameObject.h"

GameObjectType GameObject::getObjectType() {
	return objectType;
}

void GameObject::setObjectCoordinates(int x, int y) {
	objectCoordinates.left = x;
	objectCoordinates.right = x + objectBmp.bmWidth;
	objectCoordinates.top = y;
	objectCoordinates.bottom = y + objectBmp.bmHeight;
}

void GameObject::loadImage() {
	objectPictureHandler = LoadImage(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(objectPictureHandler, sizeof(objectBmp), &objectBmp);
	objectMaskPictureHandler = LoadImage(NULL, maskPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(objectMaskPictureHandler, sizeof(objectMaskBmp), &objectMaskBmp);
}

void GameObject::drawObject(HDC* hdcPicture, HDC* hdcBuffer) {
	HBITMAP objectmaskBmp = (HBITMAP)SelectObject(*hdcPicture, objectMaskPictureHandler);
	BitBlt(*hdcBuffer, objectCoordinates.left, objectCoordinates.top, objectMaskBmp.bmWidth, objectMaskBmp.bmHeight, *hdcPicture, 0, 0, SRCPAINT);

	HBITMAP objectpictureBmp = (HBITMAP)SelectObject(*hdcPicture, objectPictureHandler);
	BitBlt(*hdcBuffer, objectCoordinates.left, objectCoordinates.top, objectBmp.bmWidth, objectBmp.bmHeight, *hdcPicture, 0, 0, SRCAND);
}

int GameObject::getSpeed() {
	return objectSpeed;
}
