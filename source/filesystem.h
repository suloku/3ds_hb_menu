#pragma once
#include <3ds.h>
#include "menu.h"
#include "config.h"
#include "shortcut.h"

#define FAVORITE_MARKER "* "
#define FOLDERMAGIC "@FOLDER"

extern FS_Archive sdmcArchive;

//system stuff
void initFilesystem(void);
void exitFilesystem(void);

void openSDArchive();
void closeSDArchive();

//general fs stuff
int loadFile(char* path, void* dst, FS_Archive* archive, u64 maxSize);
bool fileExists(char* path, FS_Archive* archive);

//menu fs stuff
void addDirectoryToMenu(menu_s* m, char* path);
void scanHomebrewDirectory(menu_s* m, char* path);

//shortcut menu stuff
void createMenuEntryShortcut(menu_s* m, shortcut_s* s, char* shortcutPath, int priority);

void addFavorites(menu_s* m);
void addFolders(menu_s* m);
