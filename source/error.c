#include <3ds.h>
#include <string.h>
#include <stdio.h>

#include "error.h"
#include "menu.h"
#include "text.h"
#include "arrowdown_bgr.h"
#include "arrowup_bgr.h"

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
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, ENTRY_BGCOLOR, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(screen, GFX_LEFT, ENTRY_BGCOLOR, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(screen, GFX_LEFT, ENTRY_BGCOLOR, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);

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
	for(i=0; i<9; i++)gfxDrawRectangle(GFX_TOP, GFX_LEFT, ENTRY_BGCOLOR, x+roundLutError[i], y+i, width-roundLutError[i]*2, 1);
	gfxDrawRectangle(GFX_TOP, GFX_LEFT, ENTRY_BGCOLOR, x, y+9, width, height-9*2);
	for(i=0; i<9; i++)gfxDrawRectangle(GFX_TOP, GFX_LEFT, ENTRY_BGCOLOR, x+roundLutError[i], y+height-1-i, width-roundLutError[i]*2, 1);

	//content
	
	//Get folder name
	char * pch;
	char temp[1024];

	strcpy (temp, previous);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontDescription, pch,  x+width-16, y+38);

	strcpy (temp, current);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontTitle, pch, x+width-5-16-13, y+26);

	strcpy (temp, next);
	pch=strrchr(temp,'/');
	*pch='\0';
	pch=strrchr(temp,'/')+1;
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontDescription, pch, x+width-5-16-13-16, y+38);
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)arrowdown_bgr, 9, 14, 6, 8);
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)arrowup_bgr, 9, 14, 46, 8);


}
