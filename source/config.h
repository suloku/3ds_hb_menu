#ifndef CONFIG_H
#define CONFIG_H

#include <3ds.h>

#ifdef __cplusplus
extern "C" {
#endif


#define MAX_FOLDER 21
#define FOLDER_FILE "sdmc:/3ds/config.xml"
#define FOLDER_FILE2 "sdmc:/3ds/.hbl/config.xml"

#define MAX_THEMES 50
#define THEMES_FILE "sdmc:/3ds/themes.xml"
#define THEMES_FILE2 "sdmc:/3ds/.hbl/themes.xml"

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
extern int toolbar_pos;
extern int swipesens;

typedef struct
{
	char dir[MAX_FOLDER][1024];
	int current;
	int max;
} hbfolder;

void loadTheme();
void loadConfig(hbfolder* folder);
void writeConfig(hbfolder* folder);
void writeShortcut(char* ShortcutPath, char* HansPath, char* iconPath, char* arg, u64 title_id, u8 mediatype);
int isFavorite(char* path);

#ifdef __cplusplus
}
#endif

#endif
