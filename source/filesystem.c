#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <3ds.h>

#include "installerIcon_bin.h"
#include "folder_bin.h"

#include "filesystem.h"
#include "smdh.h"
#include "utils.h"

extern hbfolder Folders;

FS_archive sdmcArchive;

void initFilesystem(void)
{
	fsInit();
	sdmcInit();
}

void exitFilesystem(void)
{
	sdmcExit();
	fsExit();
}

void openSDArchive()
{
	sdmcArchive=(FS_archive){0x00000009, (FS_path){PATH_EMPTY, 1, (u8*)""}};
	FSUSER_OpenArchive(NULL, &sdmcArchive);
}

void closeSDArchive()
{
	FSUSER_CloseArchive(NULL, &sdmcArchive);
}

int loadFile(char* path, void* dst, FS_archive* archive, u64 maxSize)
{
	if(!path || !dst || !archive)return -1;

	u64 size;
	u32 bytesRead;
	Result ret;
	Handle fileHandle;

	ret=FSUSER_OpenFile(NULL, &fileHandle, *archive, FS_makePath(PATH_CHAR, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if(ret!=0)return ret;

	ret=FSFILE_GetSize(fileHandle, &size);
	if(ret!=0)goto loadFileExit;
	if(size>maxSize){ret=-2; goto loadFileExit;}

	ret=FSFILE_Read(fileHandle, &bytesRead, 0x0, dst, size);
	if(ret!=0)goto loadFileExit;
	if(bytesRead<size){ret=-3; goto loadFileExit;}

	loadFileExit:
	FSFILE_Close(fileHandle);
	return ret;
}

bool fileExists(char* path, FS_archive* archive)
{
	if(!path || !archive)return false;

	Result ret;
	Handle fileHandle;

	ret=FSUSER_OpenFile(NULL, &fileHandle, *archive, FS_makePath(PATH_CHAR, path), FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	if(ret!=0)return false;

	ret=FSFILE_Close(fileHandle);
	if(ret!=0)return false;

	return true;
}

extern int debugValues[100];

void addExecutableToMenu(menu_s* m, char* execPath)
{
	if(!m || !execPath)return;

	static menuEntry_s tmpEntry;

	if(!fileExists(execPath, &sdmcArchive))return;

	int i, l=-1; for(i=0; execPath[i]; i++) if(execPath[i]=='/')l=i;

	char execPathFav[1024];
	sprintf(execPathFav, "%s%s", (isFavorite(execPath)?FAVORITE_MARKER:""), execPath);
	initMenuEntry(&tmpEntry, execPath, &execPath[l+1], execPathFav, "Unknown Publisher", (u8*)installerIcon_bin);

	static char xmlPath[128];
	snprintf(xmlPath, 128, "%s", execPath);
	l = strlen(xmlPath);
	xmlPath[l-1] = 0;
	xmlPath[l-2] = 'l';
	xmlPath[l-3] = 'm';
	xmlPath[l-4] = 'x';
	if(fileExists(xmlPath, &sdmcArchive)){
		loadDescriptor(&tmpEntry.descriptor, xmlPath);
	}else{
		strcpy(xmlPath, "descriptor.xml");
		if(fileExists(xmlPath, &sdmcArchive)) loadDescriptor(&tmpEntry.descriptor, xmlPath);
	}

	addMenuEntryCopy(m, &tmpEntry);
}

void addDirectoryToMenu(menu_s* m, char* path)
{
	if(!m || !path)return;

	static menuEntry_s tmpEntry;
	static smdh_s tmpSmdh;
	static char execPath[128];
	static char iconPath[128];
	static char xmlPath[128];

	int i, l=-1; for(i=0; path[i]; i++) if(path[i]=='/') l=i;

	snprintf(execPath, 128, "%s/boot.3dsx", path);
	if(!fileExists(execPath, &sdmcArchive))
	{
		snprintf(execPath, 128, "%s/%s.3dsx", path, &path[l+1]);
		if(!fileExists(execPath, &sdmcArchive))return;
	}

	bool icon=false;
	snprintf(iconPath, 128, "%s/icon.bin", path);
	if(!icon && !(icon=fileExists(iconPath, &sdmcArchive)))snprintf(iconPath, 128, "%s/icon.smdh", path);
	if(!icon && !(icon=fileExists(iconPath, &sdmcArchive)))snprintf(iconPath, 128, "%s/icon.icn", path);
	if(!icon && !(icon=fileExists(iconPath, &sdmcArchive)))snprintf(iconPath, 128, "%s/%s.smdh", path, &path[l+1]);
	if(!icon && !(icon=fileExists(iconPath, &sdmcArchive)))snprintf(iconPath, 128, "%s/%s.icn", path, &path[l+1]);

	int ret=loadFile(iconPath, &tmpSmdh, &sdmcArchive, sizeof(smdh_s));
	
	if(!ret)
	{
		initEmptyMenuEntry(&tmpEntry);
		ret=extractSmdhData(&tmpSmdh, tmpEntry.name, tmpEntry.description, tmpEntry.author, tmpEntry.iconData);
		strncpy(tmpEntry.executablePath, execPath, ENTRY_PATHLENGTH);
		if (isFavorite(path)){
			char descriptionFav[ENTRY_DESCLENGTH+1];
			sprintf(descriptionFav, "%s%s", (isFavorite(path)?FAVORITE_MARKER:""), tmpEntry.description);
			strcpy(tmpEntry.description, descriptionFav);
		}
	}

	if(ret){
		char descriptionFav[ENTRY_DESCLENGTH+1];
		sprintf(descriptionFav, "%s%s", (isFavorite(path)?FAVORITE_MARKER:""), execPath);
		initMenuEntry(&tmpEntry, execPath, &path[l+1], descriptionFav, "Unknown publisher", (u8*)installerIcon_bin);
	}

	snprintf(xmlPath, 128, "%s/descriptor.xml", path);
	if(!fileExists(xmlPath, &sdmcArchive))snprintf(xmlPath, 128, "%s/%s.xml", path, &path[l+1]);
	loadDescriptor(&tmpEntry.descriptor, xmlPath);


	addMenuEntryCopy(m, &tmpEntry);
}

int cmp(const void *a, const void *b) { 
	if (!caseSetting){
		int r = strcasecmp(a, b);
		if (r) return r;
		/* if equal ignoring case, use opposite of strcmp() result to get
		 * lower before upper */
		return -strcmp(a, b); /* aka: return strcmp(b, a); */
	}else{
		return strcmp(a, b);
	}
}

// should go in menu.c ?
void createMenuEntryShortcut(menu_s* m, shortcut_s* s, char* shortcutPath, int priority)
{
	if(!m || !s)return;

	static menuEntry_s tmpEntry;
	static smdh_s tmpSmdh;

	char* execPath = s->executable;

	if(!fileExists(execPath, &sdmcArchive))return;

	int i, l=-1; for(i=0; execPath[i]; i++) if(execPath[i]=='/') l=i;

	char* iconPath = s->icon;
	int ret = loadFile(iconPath, &tmpSmdh, &sdmcArchive, sizeof(smdh_s));

	if(!ret)
	{
		initEmptyMenuEntry(&tmpEntry);
		ret = extractSmdhData(&tmpSmdh, tmpEntry.name, tmpEntry.description, tmpEntry.author, tmpEntry.iconData);
		strncpy(tmpEntry.executablePath, execPath, ENTRY_PATHLENGTH);
		if (isFavorite(shortcutPath)){
			char descriptionFav[ENTRY_DESCLENGTH+1];
			sprintf(descriptionFav, "%s%s", (isFavorite(shortcutPath)?FAVORITE_MARKER:""), tmpEntry.description);
			strcpy(tmpEntry.description, descriptionFav);
		}
	}

	if(ret){
		char descriptionFav[ENTRY_DESCLENGTH+1];
		sprintf(descriptionFav, "%s%s", (isFavorite(shortcutPath)?FAVORITE_MARKER:""), execPath);
		initMenuEntry(&tmpEntry, execPath, &execPath[l+1], descriptionFav, "Unknown publisher", (u8*)installerIcon_bin);
	}

	if(s->name) strncpy(tmpEntry.name, s->name, ENTRY_NAMELENGTH);
	if(s->description) strncpy(tmpEntry.description, s->description, ENTRY_DESCLENGTH);
	if(s->author) strncpy(tmpEntry.author, s->author, ENTRY_AUTHORLENGTH);

	if(s->arg)
	{
		strncpy(tmpEntry.arg, s->arg, ENTRY_ARGLENGTH);
	}

	if(fileExists(s->descriptor, &sdmcArchive)) loadDescriptor(&tmpEntry.descriptor, s->descriptor);
	
	strncpy(tmpEntry.shortcutPath, shortcutPath, ENTRY_PATHLENGTH);

	if (priority){
		addMenuEntryCopyAt(m, &tmpEntry, 0);
	}else{
		addMenuEntryCopy(m, &tmpEntry);
	}

}

void addShortcutToMenu(menu_s* m, char* shortcutPath, int priority)
{
	if(!m || !shortcutPath)return;

	static shortcut_s tmpShortcut;

	Result ret = createShortcut(&tmpShortcut, shortcutPath);
	if(!ret) createMenuEntryShortcut(m, &tmpShortcut, shortcutPath, priority);

	freeShortcut(&tmpShortcut);
}

void scanHomebrewDirectory(menu_s* m, char* path)
{
	if(!path)return;

	Handle dirHandle;
	FS_path dirPath=FS_makePath(PATH_CHAR, path);
	FSUSER_OpenDirectory(NULL, &dirHandle, sdmcArchive, dirPath);
	
	static char fullPath[1024][1024];
	u32 entriesRead;
	int totalentries = 0;
	do
	{
		static FS_dirent entry;
		memset(&entry,0,sizeof(FS_dirent));
		entriesRead=0;
		FSDIR_Read(dirHandle, &entriesRead, 1, &entry);
		if(entriesRead)
		{
			strncpy(fullPath[totalentries], path, 1024);
			int n=strlen(fullPath[totalentries]);
			unicodeToChar(&fullPath[totalentries][n], entry.name, 1024-n);
			if(entry.isDirectory) //directories
			{
				//addDirectoryToMenu(m, fullPath[totalentries]);
				totalentries++;
			}else{ //stray executables and shortcuts
				n=strlen(fullPath[totalentries]);
				if(n>5 && !strcmp(".3dsx", &fullPath[totalentries][n-5])){
					//addExecutableToMenu(m, fullPath[totalentries]);
					totalentries++;
				}
				if(n>4 && !strcmp(".xml", &fullPath[totalentries][n-4])){
					//addShortcutToMenu(m, fullPath[totalentries]);
					totalentries++;
				}
			}
		}
	}while(entriesRead);

    int i, j;
    qsort (fullPath, totalentries, 1024, cmp);
	for (j=0; j<2; j++){
		for (i = 0; i < totalentries; i++) {
			int n=strlen(fullPath[i]);
			if(n>5 && !strcmp(".3dsx", &fullPath[i][n-5])){
				if (!j && !mixSetting){
					//skip stray files in first pass
				}else {
					addExecutableToMenu(m, fullPath[i]);
				}
			}else if(n>4 && !strcmp(".xml", &fullPath[i][n-4])){
				if (!j){
					addShortcutToMenu(m, fullPath[i], 1);
				}else{
					//Skip shortcuts in second pass
				}
			}else{
				if (strncmp(".", fullPath[i], 1) != 0){
					if (!j){
						addDirectoryToMenu(m, fullPath[i]);
					}else{
						//Skip dirs in second pass
					}
				}
			}
		}
		if (mixSetting) break;
	}

	FSDIR_Close(dirHandle);
}

void addFavorites(menu_s* m)
{
	int i;
	if (totalfavs>0){
		for (i = 0; i < totalfavs; i++)
		{
			int n=strlen(favorites[i]);
			if(n>5 && !strcmp(".3dsx", &favorites[i][n-5]))	{
				addExecutableToMenu(m, favorites[i]);
			}else if(n>4 && !strcmp(".xml", &favorites[i][n-4])){
				addShortcutToMenu(m, favorites[i], 0);
			}else {
				addDirectoryToMenu(m, favorites[i]);
			}
		}
	}
}

void addFolderToMenu(menu_s* m, char* folderPath)
{
	if(!m || !folderPath)return;

	static menuEntry_s tmpEntry;

	//Check if dir exists
	Handle dirHandle;
	FS_path dirPath=FS_makePath(PATH_CHAR, folderPath);
	Result ret = FSUSER_OpenDirectory(NULL, &dirHandle, sdmcArchive, dirPath);
	FSDIR_Close(dirHandle);
	if (ret != 0) return;
	
	char tmp[ENTRY_PATHLENGTH+1];
	strcpy(tmp, folderPath);

	if (tmp[strlen(tmp)-1] == '/') tmp[strlen(tmp)-1] = '\0';
	int i, l=-1; for(i=0; tmp[i]; i++) if(tmp[i]=='/')l=i;

	initMenuEntry(&tmpEntry, FOLDERMAGIC, &tmp[l+1], tmp, "Folder", (u8*)folder_bin);

	addMenuEntryCopy(m, &tmpEntry);
}

void addFolders(menu_s* m)
{
	int i;
	for (i = 0; i < Folders.max+1; i++){
		addFolderToMenu(m, Folders.dir[i]);
	}
}
