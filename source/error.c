#include <3ds.h>
#include <string.h>
#include <stdio.h>

#include "error.h"
#include "menu.h"
#include "text.h"
#include "arrowdown_bin.h"
#include "arrowup_bin.h"

u8 roundLutError[]={8, 5, 4, 3, 2, 1, 1, 1, 0};

int countLines(char* str)
{
	if(!str)return 0;
	int cnt; for(cnt=1;*str=='\n'?++cnt:*str;str++);
	return cnt;
}

void initErrors()
{
	//placeholder
}

void drawError(gfxScreen_t screen, char* title, char* body, int offset)
{
	int i;

	int numLines=countLines(body);

	int width=numLines*8+32;
	int height=300;
	int x=240-width-12+offset, y=4;

	//main frame
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);

	//content
	gfxDrawText(screen, GFX_LEFT, &fontTitle, title, x+width-6-16, y+6);
	gfxDrawText(screen, GFX_LEFT, &fontDescription, body, x+width-5-16-13, y+8);
}

void drawPanel(gfxScreen_t screen, int x, int y, int width, int height, bool border)
{
	drawPanelAlphaBlend(screen, x, y, width, height, border, 255);
}

void drawPanelAlphaBlend(gfxScreen_t screen, int x, int y, int width, int height, bool border, u8 alpha)
{
	int i;
	//main frame
	for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor, alpha, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor, alpha, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor, alpha, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);
	if (border){
		//Border
		for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x+roundLutError[i], y+i, 1, 1);
		for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x-roundLutError[i]+width, y+i, 1, 1);
		gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x-1+9-2, y+9-1-9+1, width+2-9*2+4+1, 1);
		gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x-1+9-2, y+9-1-9+height, width+2-9*2+4+1, 1);
		gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x, y+9-2, 1, height-9*2+2);
		gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x+width, y+9-2, 1, height-9*2+2);
		for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x+roundLutError[i], y+height-1-i, 1, 1);
		for(i=0; i<9; i++)gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, entry_bgcolor_shadow, alpha, x-roundLutError[i]+width, y+height-1-i, 1, 1);
	}
}

void drawFolders(char* current, char* previous, char* next, int offset)
{
	int i;

	int numLines=countLines("\n\n");

	int width=numLines*8+32;
	int height=175;
	int x=240-width-12+offset, y=4;

	//main frame
	for(i=0; i<9; i++)gfxDrawRectangle(GFX_TOP, GFX_LEFT, folder_bgcolor, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(GFX_TOP, GFX_LEFT, folder_bgcolor, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(GFX_TOP, GFX_LEFT, folder_bgcolor, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);

	//content
	
	//Get folder name
	char * pch;
	char temp[1024];

	strcpy (temp, previous);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontDescriptionFolder, pch,  x+width-16, y+38);

	strcpy (temp, current);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontTitleFolder, pch, x+width-5-16-13, y+26);

	strcpy (temp, next);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontDescriptionFolder, pch, x+width-5-16-13-16, y+38);

	gfxDrawSpriteAlphaBlend(GFX_TOP, GFX_LEFT, (u8*)arrowup_bin, 9, 16, 46, 8);
	gfxDrawSpriteAlphaBlend(GFX_TOP, GFX_LEFT, (u8*)arrowdown_bin, 9, 16, 6, 8);
}

void drawButton(gfxScreen_t screen, char* title, char* body, int x, int y, int height)
{
	int i;

	int numLines=countLines(body);

	int width=numLines*8+32;
	if (body == NULL) width = 32;
	else if (strlen(body) < 1) width = 32;

	//Drop shadow
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);
	//main frame
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);


	//content
	gfxDrawText(screen, GFX_LEFT, &fontTitle, title, x+width-6-16, y+6);
	gfxDrawText(screen, GFX_LEFT, &fontDescription, body, x+width-5-16-13, y+8);
}

void drawButton2(gfxScreen_t screen, Button button)
{
	int i;
	int x = button.x;
	int y = button.y;
	int height = button.height;

	int numLines=countLines(button.body);

	int width=numLines*8+32;
	if (button.body == NULL) width = 32;
	else if (strlen(button.body) < 1) width = button.width;
/*
	//Drop shadow
	if (button.enabled){
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4, y+9, width, height-9*2);
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);
	}
	//Border
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x+roundLutError[i]-1, y+i-1, width+2-roundLutError[i]*2, 1+2);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-1, y+9-1, width+2, height-9*2+2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x+roundLutError[i]-1, y+height-1-i-1, width+2-roundLutError[i]*2, 1+2);
*/
	//main frame
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);

		//Border
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x+roundLutError[i], y+i, 1, 1);
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-roundLutError[i]+width, y+i, 1, 1);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-1+9-2, y+9-1-9+1, width+2-9*2+4+1, 1);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-1+9-2, y+9-1-9+height, width+2-9*2+4+1, 1);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x, y+9-2, 1, height-9*2+2);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x+width, y+9-2, 1, height-9*2+2);
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x+roundLutError[i], y+height-1-i, 1, 1);
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-roundLutError[i]+width, y+height-1-i, 1, 1);

	//Button state indicator
	u8 color[]={0,255,0};
	if (button.enabled){
		gfxDrawRectangleAlphaBlend(screen, GFX_LEFT, color, 175, x+1, y+height/2-(height/6)*2/2, 4, (height/6)*2);
	}

	//content
	gfxDrawText(screen, GFX_LEFT, &fontTitle, button.title, x+width-6-16, y+6);
	gfxDrawText(screen, GFX_LEFT, &fontDescription, button.body, x+width-5-16-13, y+8);
}
