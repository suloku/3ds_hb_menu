#include <stdlib.h>
#include <string.h>
#include <3ds.h>

bool touchInBox(int x1, int x2, int y1, int y2, touchPosition touch){
	//Corner buttons
	if (touch.px > x1 && touch.px < x2 && touch.py > y1 && touch.py < y2)
		return true;
	return false;
}

bool touchInFolderBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	//return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	//return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
}

bool touchInFavBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	//return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	//return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
}

bool touchInHomeBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	//return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	//return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
}
