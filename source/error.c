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
	else if (strlen(button.body) < 1) width = 32;

	//Drop shadow
	if (button.enabled){
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
		gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4, y+9, width, height-9*2);
		for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor_shadow, x-4+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);
	}
	//main frame
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, entry_bgcolor, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);


	//content
	gfxDrawText(screen, GFX_LEFT, &fontTitle, button.title, x+width-6-16, y+6);
	gfxDrawText(screen, GFX_LEFT, &fontDescription, button.body, x+width-5-16-13, y+8);
}
