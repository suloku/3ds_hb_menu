#ifndef FOLDERS_H
#define FOLDERS_H

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_FOLDER 21
#define FOLDER_FILE "sdmc:/3ds/config.xml"

#define MAX_THEMES 50
#define THEMES_FILE "sdmc:/3ds/themes.xml"

#define MAX_FAVS 20

extern int caseSetting;
extern int mixSetting;
extern int totalfavs;
extern int remembermenu;
extern int rememberbrew;
extern int lastFolder;
extern int lastEntry;
extern int disableRF;
extern char favorites[MAX_FAVS][1024];
extern int current_theme;
extern int random_theme;
extern int totalThemes;
extern int rememberRF;
extern int RFatboot;

typedef struct
{
	char dir[MAX_FOLDER][1024];
	int current;
	int max;
} hbfolder;

void loadTheme();
void loadConfig(hbfolder* folder);
void writeConfig(hbfolder* folder);
int isFavorite(char* path);

#ifdef __cplusplus
}
#endif

#endif
