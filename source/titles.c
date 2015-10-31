#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <3ds.h>

#include "titles.h"
#include "error.h"
#include "config.h"
#include "touch.h"
#include "arrowleft_bin.h"
#include "arrowright_bin.h"
#include "arrowback_bin.h"

extern int debugValues[100];
extern bool regionFreeGamecardIn;

//Touch buttons
extern touchPosition previousTouch, firstTouch;
extern u16 touchTimer;
extern bool button_touched(Button button);
extern int backbutton_fade;

//0 all, 1 user, 2 demos, 3 system
int filterID = 0;

void titlesInit()
{
	amInit();
}

void titlesExit()
{
	amExit();
}

void initTitleInfo(titleInfo_s* ti, u8 mediatype, u64 title_id)
{
	if(!ti)return;

	ti->mediatype = mediatype;
	ti->title_id = title_id;
	ti->icon = NULL;
}

void freeTitleInfo(titleInfo_s* ti)
{
	if(!ti)return;

	ti->title_id = 0x0;
	if(ti->icon)free(ti->icon);
	ti->icon = NULL;
}

Result loadTitleInfoIcon(titleInfo_s* ti)
{
	if(!ti || ti->icon)return -1;

	ti->icon = malloc(sizeof(smdh_s));
	if(!ti->icon)return -1;

	Handle fileHandle;
	u32 archivePath[] = {ti->title_id & 0xFFFFFFFF, (ti->title_id >> 32) & 0xFFFFFFFF, ti->mediatype, 0x00000000};
	static const u32 filePath[] = {0x00000000, 0x00000000, 0x00000002, 0x6E6F6369, 0x00000000};	
	Result ret = FSUSER_OpenFileDirectly(NULL, &fileHandle, (FS_archive){0x2345678a, (FS_path){PATH_BINARY, 0x10, (u8*)archivePath}}, (FS_path){PATH_BINARY, 0x14, (u8*)filePath}, FS_OPEN_READ, FS_ATTRIBUTE_NONE);

	if(ret)
	{
		free(ti->icon);
		ti->icon = NULL;
		return ret;
	}

	u32 bytesRead;	
	ret = FSFILE_Read(fileHandle, &bytesRead, 0x0, ti->icon, sizeof(smdh_s));

	if(ret)
	{
		free(ti->icon);
		ti->icon = NULL;
	}

	FSFILE_Close(fileHandle);

	return ret;
}

bool application_filter(u64 tid)
{
	u32 tid_high = tid >> 32;
	switch (filterID){
		case 1:
			return (tid_high == 0x00040000);
			break;
		case 2:
			return (tid_high == 0x00040002);
			break;
		case 3:
			return (tid_high == 0x00040010);
			break;
	}
	return (tid_high == 0x00040010 || tid_high == 0x00040000 || tid_high == 0x00040002);
}

void initTitleList(titleList_s* tl, titleFilter_callback filter, u8 mediatype)
{
	if(!tl)return;

	tl->mediatype = mediatype;

	if(tl->filter) tl->filter = filter;
	else tl->filter = &application_filter;

	tl->num = 0;
	tl->titles = NULL;

	populateTitleList(tl);
}

void freeTitleList(titleList_s* tl)
{
	if(!tl)return;

	int i;
	for(i=0; i<tl->num; i++)freeTitleInfo(&tl->titles[i]);

	tl->num = 0;
	if(tl->titles)free(tl->titles);
	tl->titles = NULL;
}

int populateTitleList(titleList_s* tl)
{
	if(!tl)return 0;
	Result ret;

	u32 old_num = tl->num;

	u32 num;
	ret = AM_GetTitleCount(tl->mediatype, &num);

	if(ret)
	{
		freeTitleList(tl);
	}else if(num){
		// temp buffer is not ideal, but i like modularity
		u64* tmp = (u64*)malloc(sizeof(u64) * num);

		if(!tmp)
		{
			tl->num = 0;
			return 1;
		}

		ret = AM_GetTitleIdList(tl->mediatype, num, tmp);

		if(!ret)
		{
			int i;

			// apply tid filter
			for(i=0; i<num; i++)
			{
				if(!tl->filter(tmp[i]))
				{
					num--;
					tmp[i] = tmp[num];
					i--;
				}
			}

			if(tl->num != num || tl->mediatype == 2)
			{
				freeTitleList(tl);

				tl->num = num;

				if(tl->num) tl->titles = malloc(sizeof(titleInfo_s) * tl->num);

				if(tl->titles)
				{
					for(i=0; i<tl->num; i++)
					{
						initTitleInfo(&tl->titles[i], tl->mediatype, tmp[i]);
					}
				}
			}
		}else tl->num = 0;

		free(tmp);
	}

	return old_num != tl->num;
}

titleInfo_s* findTitleList(titleList_s* tl, u64 tid)
{
	if(!tl)return NULL;

	// special case : gamecard mediatype with 0 tid
	if(!tid && tl->mediatype == 2 && tl->num)return &tl->titles[0];

	int i;
	for(i=0; i<tl->num; i++)
	{
		if(tl->titles[i].title_id == tid)return &tl->titles[i];
	}

	return NULL;
}

titleInfo_s* findTitleBrowser(titleBrowser_s* tb, u8 mediatype, u64 tid)
{
	if(!tb || mediatype > 2)return NULL;

	return findTitleList(&tb->lists[mediatype], tid);
}

void initTitleBrowser(titleBrowser_s* tb, titleFilter_callback filter)
{
	if(!tb)return;

	int i;
	for(i=0; i<3; i++)
	{
		initTitleList(&tb->lists[i], filter, (u8)i);
	}

	tb->total = 0;
	tb->nextCheck = 0;
	tb->selectedId = 0;
	tb->selected = NULL;
}

void updateTitleBrowser(titleBrowser_s* tb)
{
	if(!tb)return;

	int i;

	if (osGetTime() > tb->nextCheck)
	{
		bool updated = false;

		tb->total = 0;

		for(i=0; i<3; i++)
		{
			updated = populateTitleList(&tb->lists[i]) || updated;
			tb->total += tb->lists[i].num;
		}

		if(updated)
		{
			tb->selectedId = 0;
			if(regionFreeGamecardIn) tb->selectedId -= 1;
		}

		tb->nextCheck = osGetTime() + 250;
	}

	tb->selected = NULL;

	if(!tb->total)return;

	u32 padDown = hidKeysDown();

	int move = 0;
	if(padDown & KEY_LEFT || button_touched(TL_prev))move--;
	if(padDown & KEY_RIGHT || button_touched(TL_next))move++;

	tb->selectedId += move;

	while(tb->selectedId < 0) tb->selectedId += tb->total;
	while(tb->selectedId >= tb->total) tb->selectedId -= tb->total;

	int id = tb->selectedId;
	for(i=0; i<3; i++)
	{
		const titleList_s* tl = &tb->lists[i];
		if(id >= 0 && id < tl->num)
		{
			tb->selected = &tl->titles[id];
			break;
		}else id -= tl->num;
	}

	if(tb->selected)
	{
		if(!tb->selected->icon)loadTitleInfoIcon(tb->selected);
		if(tb->selected->icon)extractSmdhData(tb->selected->icon, tb->selectedEntry.name, tb->selectedEntry.description, tb->selectedEntry.author, tb->selectedEntry.iconData);
		else
		{
			tb->selected = NULL;
			if(!move)tb->selectedId++;
			else tb->selectedId += move;
		}
	}
}

void drawTitleBrowser(titleBrowser_s* tb, bool titlelaunch)
{
	if(!tb)return;

	char mode[32];
	switch (filterID){
		case 0:
			strcpy (mode, "All titles");
			break;
		case 1:
			strcpy (mode, "Application titles");
			break;
		case 2:
			strcpy (mode, "Demo titles");
			break;
		case 3:
			strcpy (mode, "System titles");
			break;
	}
/*
	if(tb->selected && tb->selected->icon)
	{
		drawError(GFX_BOTTOM,
			mode,
			"    Press LEFT or RIGHT to select a title.                                 \n"
			"    Press L or R to change title mode.                                     \n"
			"    Press X to show all titles.                                            \n\n"
			"                                                                                                        A : Select target\n"
			"                                                                                                        B : Cancel\n",
			10-drawMenuEntry(&tb->selectedEntry, GFX_BOTTOM, 240, 6, true));
	}else{
		drawError(GFX_BOTTOM,
			mode,
			"    The application you're trying to run requires that you select a target.\n"
			"    No adequate target title could be found. :(\n\n"
			"                                                                                            B : Cancel\n",
			0);
	}
	drawError(GFX_BOTTOM,
		"                        Title Launcher                                          \n",
		"\nPress SELECT to create shortcut                                                 "
		"\nPress START to delete shortcut (if present)                                     "
		"\nPress Y to launch using region four                                             ",
		-160);	
*/
	drawMenuEntry(&tb->selectedEntry, GFX_BOTTOM, 240, (320-ENTRY_HEIGHT)/2, false);
	
	//Back pannel
	//drawPanel(GFX_BOTTOM, 84, 4, 84, 320-8, true);
	drawPanelAlphaBlend(GFX_BOTTOM, 84, 4, 84, 320-8, false, theme_alpha);
	//Arrows
	gfxDrawSpriteAlphaBlend(GFX_BOTTOM, GFX_LEFT, (u8*)arrowleft_bin, 29, 20, TL_prev.x, TL_prev.y);
	gfxDrawSpriteAlphaBlend(GFX_BOTTOM, GFX_LEFT, (u8*)arrowright_bin, 29, 20, TL_next.x, TL_next.y);
		
	drawButton2(GFX_BOTTOM, TL_demo);
	drawButton2(GFX_BOTTOM, TL_apps);
	drawButton2(GFX_BOTTOM, TL_system);
	drawButton2(GFX_BOTTOM, TL_all);
	if (titlelaunch){
		drawButton2(GFX_BOTTOM, TL_shortcut_create);
		drawButton2(GFX_BOTTOM, TL_shortcut_delete);
		drawButton2(GFX_BOTTOM, TL_launchr4);
	}
/*	
	drawButton(GFX_BOTTOM, "   Demo", NULL, 130, 40, 75);
	drawButton(GFX_BOTTOM, "    Apps", NULL, 130, 120, 75);
	drawButton(GFX_BOTTOM, "  System", NULL, 130, 200, 75);
	drawButton(GFX_BOTTOM, "     All", NULL, 90, 200, 75);
	
	drawButton(GFX_BOTTOM, "Create Shortcut", NULL, 50, 10, 110);
	drawButton(GFX_BOTTOM, "Delete Shortcut", NULL, 10, 10, 110);
	drawButton(GFX_BOTTOM, "Launch with Region 4", NULL, 50, 160, 150);
*/
	gfxDrawSpriteAlphaBlendFade(GFX_BOTTOM, GFX_LEFT, (u8*)arrowback_bin, 28, 33, 0, 320-33, backbutton_fade);
}
