#ifndef FOLDERS_H
#define FOLDERS_H

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_FOLDER 21
#define FOLDER_FILE "sdmc:/3ds/config.xml"

#define MAX_FAVS 20

extern int caseSetting;
extern int mixSetting;
extern int totalfavs;
extern int remembermenu;
extern int lastFolder;
extern int lastEntry;
extern int disableRF;
extern char favorites[MAX_FAVS][1024];

typedef struct
{
	char dir[MAX_FOLDER][1024];
	int current;
	int max;
} hbfolder;

void loadFolders(hbfolder* folder);
void writeFolders(hbfolder* folder);

#ifdef __cplusplus
}
#endif

#endif
