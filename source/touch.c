#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#include "touch.h"

extern int toolbar_pos;

Button TL_entry;
Button TL_prev;
Button TL_next;
Button TL_demo;
Button TL_apps;
Button TL_system;
Button TL_all;
Button TL_shortcut_create;
Button TL_shortcut_delete;
Button TL_launchr4;
Button TL_left;
Button TL_right;

void initButtons(){

	TL_entry.x = 240-ENTRY_WIDTH;
	TL_entry.y = 9;
	TL_entry.width = ENTRY_WIDTH;
	TL_entry.height = ENTRY_HEIGHT;

	TL_prev.x = 130;
	TL_prev.y = 10;
	TL_prev.width = 29;
	TL_prev.height = 20;

	TL_next.x = 130;
	TL_next.y = 320-10-20;
	TL_next.width = 20;
	TL_next.height = 29;

	TL_demo.x = 130;
	TL_demo.y = 40;
	TL_demo.width = 32;
	TL_demo.height = 75;
	strcpy(TL_demo.title, "   Demo");

	TL_apps.x = 130;
	TL_apps.y = 120;
	TL_apps.width = 32;
	TL_apps.height = 75;
	strcpy(TL_apps.title, "    Apps");

	TL_system.x = 130;
	TL_system.y = 200;
	TL_system.width = 32;
	TL_system.height = 75;
	strcpy(TL_system.title, "  System");

	TL_all.x = 90;
	TL_all.y = 200;
	TL_all.width = 32;
	TL_all.height = 75;
	strcpy(TL_all.title, "     All");

	TL_shortcut_create.x = 50;
	TL_shortcut_create.y = 10;
	TL_shortcut_create.width = 32;
	TL_shortcut_create.height = 110;
	strcpy(TL_shortcut_create.title, "Create Shortcut");

	TL_shortcut_delete.x = 10;
	TL_shortcut_delete.y = 10;
	TL_shortcut_delete.width = 32;
	TL_shortcut_delete.height = 110;
	strcpy(TL_shortcut_delete.title, "Delete Shortcut");

	TL_launchr4.x = 50;
	TL_launchr4.y = 160;
	TL_launchr4.width = 32;
	TL_launchr4.height = 150;
	strcpy(TL_launchr4.title, "Launch with Region 4");

}

bool touchInBox(int x1, int x2, int y1, int y2, touchPosition touch){
	//Corner buttons
	if (touch.px > x1 && touch.px < x2 && touch.py > y1 && touch.py < y2)
		return true;
	return false;
}

bool touchInButton(Button button, touchPosition touch){

	if (touch.py < 240-button.x && touch.py > 240-button.x-button.width && touch.px > button.y && touch.px < button.y+button.height)
		return true;
	return false;

/*
	if (touch.px > button.x && touch.px < button.x+button.width && touch.py > button.y && touch.py < button.y+button.height)
		return true;
	return false;
*/
}

bool touchInFavBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	//return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	//return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	//return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
	
	if (!toolbar_pos)
		return touchInBox(320-36, 320, 240-39, 240, touch); //Bottom Right
	
	return touchInBox(320-29, 320, 240-3+(-3-30)*3, 240-3+(-3-30)*2, touch); //Bottom Right
}

bool touchInFolderBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	//return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	//return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	//return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
	
	if (!toolbar_pos)
		return touchInBox(320-66, 320-36, 240-39, 240, touch); //Bottom Right
	return touchInBox(320-29, 320, 240-3+(-3-30)*2, 240-3+(-30-3), touch); //Bottom Right
}

bool touchInHomeBut(touchPosition touch){
	//return touchInBox(0, 39, 0, 39, touch); //Top Left
	//return touchInBox(320-39, 320, 0, 39, touch); //Top Right
	//return touchInBox(320-39, 320, 240-39, 240, touch); //Bottom Right
	//return touchInBox(0, 39, 240-39, 240, touch); //Bottom Left
	
	if (!toolbar_pos)
		return touchInBox(320-99, 320-69, 240-39, 240, touch); //Bottom Right
	
	return touchInBox(320-29, 320, 240-3-30, 240-3, touch); //Bottom Right
}

bool touchInBackBut(touchPosition touch){
	return touchInBox(320-33, 320, 240-28, 240, touch); //Bottom Right
}