#ifndef TOUCH_H
#define TOUCH_H

#include <3ds.h>
#include "menu.h"

typedef struct Button
{
   u8 enabled;
   u16 x;
   u16 y;
   u16 width;
   u16 height;
   char title[32];
   char body[32];
}Button;

extern Button TL_entry;
extern Button TL_prev;
extern Button TL_next;
extern Button TL_demo;
extern Button TL_apps;
extern Button TL_system;
extern Button TL_all;
extern Button TL_shortcut_create;
extern Button TL_shortcut_delete;
extern Button TL_launchr4;

extern Button CNF_themeprev;
extern Button CNF_themenext;
extern Button CNF_themerand;
extern Button CNF_remembermenu;
extern Button CNF_sorting;
extern Button CNF_mixfiles;
extern Button CNF_rememberRF;
extern Button CNF_toolbar;
extern Button CNF_title_boot;

void initButtons();

bool touchInBox(int x1, int x2, int y1, int y2, touchPosition touch);
bool touchInButton(Button button, touchPosition touch);

bool touchInFavBut(touchPosition touch);
bool touchInFolderBut(touchPosition touch);
bool touchInHomeBut(touchPosition touch);
bool touchInBackBut(touchPosition touch);

#endif
