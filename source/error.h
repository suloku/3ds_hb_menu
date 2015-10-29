#ifndef ERROR_H
#define ERROR_H

#include <3ds.h>
#include "touch.h"

void initErrors();
void drawError(gfxScreen_t screen, char* title, char* body, int offset);
void drawPanel(gfxScreen_t screen, int x, int y, int width, int height, bool border);
void drawFolders(char* current, char* previous, char* next, int offset);
void drawButton(gfxScreen_t screen, char* title, char* body, int x, int y, int height);
void drawButton2(gfxScreen_t screen, Button button);

#endif
