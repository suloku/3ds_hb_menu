#ifndef FOLDERS_H
#define FOLDERS_H

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_FOLDER 21
#define FOLDER_FILE "sdmc:/3ds/folders.xml"

typedef struct
{
	char dir[MAX_FOLDER][1024];
	int current;
	int max;
} hbfolder;

void loadFolders(hbfolder* folder);

#ifdef __cplusplus
}
#endif

#endif
