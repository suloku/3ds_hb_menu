#ifndef TOUCH_H
#define TOUCH_H

#include <3ds.h>

bool touchInBox(int x1, int x2, int y1, int y2, touchPosition touch);

bool touchInFolderBut(touchPosition touch);
bool touchInFavBut(touchPosition touch);
bool touchInHomeBut(touchPosition touch);

#endif
