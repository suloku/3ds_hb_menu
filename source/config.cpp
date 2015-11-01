#include <time.h>
#include "config.h"
#include "font.h"
#include "menu.h"
#include "background.h"
#include "tinyxml2.h"

using namespace tinyxml2;

extern u8 backbutton_fade;

int caseSetting = 0;
int mixSetting = 0;
int totalfavs = 0;
int remembermenu = 0;
int rememberbrew = 0;
int lastFolder = 0;
int lastEntry = 0;
char favorites[MAX_FAVS][1024];
int current_theme = 0;
int random_theme = 0;
int totalThemes = 0;
int rememberRF = 0;
int RFatboot = 0;
int config_dir = 0; //0: /3ds/ 1: /3ds/.hbl/
int swipesens = 40;
int theme_alpha = 255;
int title_boot = 0; // 0: HANS, 1: R4

int toolbar_pos = 0; //0: horizontal, 1: vertical

//Function from K. Biermann (http://stackoverflow.com/questions/2509679/how-to-generate-a-random-number-from-within-a-range)
uint32_t getRandInterval(uint32_t begin, uint32_t end) {
    uint32_t range = 1 + end - begin;
    uint32_t limit = RAND_MAX - (RAND_MAX % range); 

    uint32_t randVal;
    do {
        randVal = rand();
    } while (randVal >= limit);

    return (randVal % range) + begin;
}

void loadTheme(){

    XMLDocument doc;
    if(doc.LoadFile(THEMES_FILE)){
		if(doc.LoadFile(THEMES_FILE2)){
			return;
		}else{
			config_dir = 1;
		}
	}
	
	XMLNode * pRoot = doc.FirstChild();
	if (pRoot == nullptr)return;

	int i;
	XMLElement * pElement;
	XMLElement * subElement;

	char theme[32];
	totalThemes = 0;
	for (i=1; i <= MAX_THEMES; i++){
		sprintf(theme, "theme%d", i);
		pElement = pRoot->FirstChildElement(theme);
		if (pElement != nullptr)
			totalThemes++;
	}
	if (random_theme && totalThemes > 0){
		current_theme = getRandInterval(1, totalThemes);
	}

	sprintf(theme, "theme%d", current_theme);
	pElement = pRoot->FirstChildElement(theme);
	if (pElement != nullptr)
	{
		subElement = pElement->FirstChildElement("THEME_ALPHA");
		if (subElement != nullptr) subElement->QueryIntText(&theme_alpha);
		else theme_alpha = 255;

		int temp;
		subElement = pElement->FirstChildElement("ENTRY_BGCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			entry_bgcolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			entry_bgcolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			entry_bgcolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FOLDER_BGCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			folder_bgcolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			folder_bgcolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			folder_bgcolor[2] = (short)temp;
		}else{
			folder_bgcolor[0] = entry_bgcolor[0];
			folder_bgcolor[1] = entry_bgcolor[1];
			folder_bgcolor[2] = entry_bgcolor[2];
		}
		subElement = pElement->FirstChildElement("ENTRY_BGCOLOR_SHADOW");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			entry_bgcolor_shadow[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			entry_bgcolor_shadow[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			entry_bgcolor_shadow[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("BUTTON_BORDERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			button_bordercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			button_bordercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			button_bordercolor[2] = (short)temp;
		}else{
			button_bordercolor[0] = 0;
			button_bordercolor[1] = 0;
			button_bordercolor[2] = 0;
		}
		subElement = pElement->FirstChildElement("BUTTON_STATECOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			colorButtonEnabled[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			colorButtonEnabled[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			colorButtonEnabled[2] = (short)temp;
		}else{
			memcpy(colorButtonEnabled, entry_bgcolor_shadow, 3);
		}
		subElement = pElement->FirstChildElement("BEERBORDERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			beerbordercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			beerbordercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			beerbordercolor[2] = (short)temp;
		}else{ //This is supposed to be an easter egg, even for themes
			beerbordercolor[0] = 240;
			beerbordercolor[1] = 240;
			beerbordercolor[2] = 240;
		}
		subElement = pElement->FirstChildElement("BEERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			beercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			beercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			beercolor[2] = (short)temp;
		}else{ //Same as above
			beercolor[0] = 188;
			beercolor[1] = 157;
			beercolor[2] = 75;
		}
		subElement = pElement->FirstChildElement("WATERBORDERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			waterbordercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			waterbordercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			waterbordercolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("WATERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			watercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			watercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			watercolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("BGCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			bgcolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			bgcolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			bgcolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FAV_WATERBORDERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fav_waterbordercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fav_waterbordercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fav_waterbordercolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FAV_WATERCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fav_watercolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fav_watercolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fav_watercolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FAV_BGCOLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fav_bgcolor[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fav_bgcolor[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fav_bgcolor[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("SCROLL_COLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			scroll_colorBg[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			scroll_colorBg[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			scroll_colorBg[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("SCROLLFRONT_COLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			scrollfront_color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			scrollfront_color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			scrollfront_color[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("ICON_COLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			colorIcon[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			colorIcon[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			colorIcon[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("ICONSHORT_COLOR");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			colorIconShort[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			colorIconShort[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			colorIconShort[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FONT_DEFAULT");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fontDefault.color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fontDefault.color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fontDefault.color[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FONT_TITLE");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fontTitle.color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fontTitle.color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fontTitle.color[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FONT_DESCRIPTION");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fontDescription.color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fontDescription.color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fontDescription.color[2] = (short)temp;
		}
		subElement = pElement->FirstChildElement("FONT_TITLEFOLDER");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fontTitleFolder.color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fontTitleFolder.color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fontTitleFolder.color[2] = (short)temp;
		}else{
			fontTitleFolder.color[0] = fontTitle.color[0];
			fontTitleFolder.color[2] = fontTitle.color[1];
			fontTitleFolder.color[1] = fontTitle.color[2];
		}
		subElement = pElement->FirstChildElement("FONT_DESCRIPTIONFOLDER");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("r", &temp);
			fontDescriptionFolder.color[0] = (short)temp;
			subElement->QueryIntAttribute("g", &temp);
			fontDescriptionFolder.color[1] = (short)temp;
			subElement->QueryIntAttribute("b", &temp);
			fontDescriptionFolder.color[2] = (short)temp;
		}else{
			fontDescriptionFolder.color[0] = fontDescription.color[0];
			fontDescriptionFolder.color[1] = fontDescription.color[1];
			fontDescriptionFolder.color[2] = fontDescription.color[2];
		}
	}
}

void loadConfig(hbfolder* folder){

	if(!folder)return;

    XMLDocument doc;
    if(doc.LoadFile(FOLDER_FILE)){
		if(doc.LoadFile(FOLDER_FILE2)){
			return;
		}else{
			config_dir = 1;
		}
	}
	
	XMLNode * pRoot = doc.FirstChild();
	if (pRoot == nullptr)return;

	int i;
	XMLElement * pElement;
	XMLElement * subElement;

	pElement = pRoot->FirstChildElement("folders");
	if (pElement != nullptr)
	{
		subElement = pElement->FirstChildElement("path");
		//Start on 1 because we already made the first element /3ds/
		for (i=1; i < MAX_FOLDER; i++){
			if (subElement == nullptr) break;
			if (subElement && subElement->GetText()){
				const char* str = subElement->GetText();
				sprintf(folder->dir[i], "%s", str);
				if ( folder->dir[i][strlen(folder->dir[i])-1] != '/' ){
					strcat(folder->dir[i], "/");
				}
				folder->max = i;
			}
			subElement = subElement->NextSiblingElement("path");
		}
		subElement = pElement->FirstChildElement("title_boot");
		if (subElement != nullptr) subElement->QueryIntText(&title_boot);

		subElement = pElement->FirstChildElement("toolbar");
		if (subElement != nullptr) subElement->QueryIntText(&toolbar_pos);

		subElement = pElement->FirstChildElement("swipe_sens");
		if (subElement != nullptr) subElement->QueryIntText(&swipesens);

		subElement = pElement->FirstChildElement("mix_files");
		if (subElement != nullptr) subElement->QueryIntText(&mixSetting);
		
		subElement = pElement->FirstChildElement("case_sensitive");
		if (subElement != nullptr) subElement->QueryIntText(&caseSetting);

		subElement = pElement->FirstChildElement("disable_RF");
		if (subElement != nullptr) subElement->QueryIntText(&disableRF);

		subElement = pElement->FirstChildElement("remember_RF");
		if (subElement != nullptr) subElement->QueryIntText(&rememberRF);

		subElement = pElement->FirstChildElement("theBrew");
		if (subElement != nullptr) subElement->QueryIntText(&rememberbrew);

		subElement = pElement->FirstChildElement("current_theme");
		if (subElement != nullptr) subElement->QueryIntText(&current_theme);

		srand (time(NULL)); //We only call loadConfig() once, so let's put this here
		subElement = pElement->FirstChildElement("random_theme");
		if (subElement != nullptr) subElement->QueryIntText(&random_theme);

		subElement = pElement->FirstChildElement("menu_pos");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("remember_menu", &remembermenu);
			subElement->QueryIntAttribute("folder", &lastFolder);
			subElement->QueryIntAttribute("entry", &lastEntry);
		}
	}

	pElement = pRoot->FirstChildElement("favorites");
	if (pElement != nullptr)
	{
		subElement = pElement->FirstChildElement("fav");
		for (i=0; i < MAX_FAVS; i++){
			if (subElement == nullptr) break;
			if (subElement && subElement->GetText()){
				const char* str = subElement->GetText();
				sprintf(favorites[i], "%s", str);
				totalfavs++;
			}
			subElement = subElement->NextSiblingElement("fav");
		}
	}
}

void writeConfig(hbfolder* folder){

	if(!folder)return;

    XMLDocument xmlDoc;
	
	XMLNode * pRoot = xmlDoc.NewElement("root");
	xmlDoc.InsertFirstChild(pRoot);

	XMLElement * pElement;
	XMLElement * subElement;

//Folders
	pElement = xmlDoc.NewElement("folders");
	int i;
	for (i=1; i<=folder->max; i++){
		subElement = xmlDoc.NewElement("path");
		char str[1024];
		sprintf (str, "%s", folder->dir[i]);
		subElement->SetText(str);
		pElement->InsertEndChild(subElement);//close each
	}

	subElement = xmlDoc.NewElement("title_boot");
	subElement->SetText(title_boot);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("toolbar");
	subElement->SetText(toolbar_pos);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("swipe_sens");
	subElement->SetText(swipesens);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("mix_files");
	subElement->SetText(mixSetting);
	pElement->InsertEndChild(subElement);//close
	
	subElement = xmlDoc.NewElement("case_sensitive");
	subElement->SetText(caseSetting);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("disable_RF");
	if (rememberRF){
		subElement->SetText(disableRF);
	}else{
		subElement->SetText(RFatboot);
	}
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("remember_RF");
	subElement->SetText(rememberRF);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("theBrew");
	subElement->SetText(rememberbrew);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("current_theme");
	subElement->SetText(current_theme);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("random_theme");
	subElement->SetText(random_theme);
	pElement->InsertEndChild(subElement);//close

//Remember menu
	subElement = xmlDoc.NewElement("menu_pos");
		subElement->SetAttribute("remember_menu", remembermenu);
		subElement->SetAttribute("folder", lastFolder);
		subElement->SetAttribute("entry", lastEntry);
	pElement->InsertEndChild(subElement);//close menu_pos

	pRoot->InsertEndChild(pElement);//close folders
//End folders
//Favs
	pElement = xmlDoc.NewElement("favorites");
	for (i=0; i<totalfavs; i++){
		subElement = xmlDoc.NewElement("fav");
		subElement->SetText(favorites[i]);
		pElement->InsertEndChild(subElement);
	}
	pRoot->InsertEndChild(pElement);//close favorites
//End favs

	if (!config_dir){
		xmlDoc.SaveFile(FOLDER_FILE);
	}else{
		xmlDoc.SaveFile(FOLDER_FILE2);
	}
}

void writeShortcut(char* ShortcutPath, char* HansPath, char* iconPath, char* arg, u64 title_id, u8 mediatype){

    XMLDocument xmlDoc;
	
	XMLElement * pElement;
	XMLElement * subElement;

//Shortcut
	pElement = xmlDoc.NewElement("shortcut");
		subElement = xmlDoc.NewElement("executable");
		subElement->SetText(HansPath);
		pElement->InsertEndChild(subElement);//close

		subElement = xmlDoc.NewElement("icon");
		subElement->SetText(iconPath);
		pElement->InsertEndChild(subElement);//close
/*
		subElement = xmlDoc.NewElement("arg");
		subElement->SetText(arg);
		pElement->InsertEndChild(subElement);//close
*/
		subElement = xmlDoc.NewElement("author");
		if(title_boot){
			subElement->SetText("R4 Shortcut");
		}else{
			subElement->SetText("HANS Shortcut");
		}
		pElement->InsertEndChild(subElement);//close
	xmlDoc.LinkEndChild(pElement);//close shortcut

//targets
	pElement = xmlDoc.NewElement("targets");
	pElement->SetAttribute("selectable", false);
		subElement = xmlDoc.NewElement("title");
		subElement->SetAttribute("mediatype", mediatype);
		char tmp[32];
		sprintf (tmp, "%016llX", title_id);
		subElement->SetText(tmp);
		pElement->InsertEndChild(subElement);//close
		xmlDoc.LinkEndChild(pElement);//close targets

	xmlDoc.SaveFile(ShortcutPath);
}

int isFavorite(char* path){
	int i;
	for (i=0; i<totalfavs ; i++){
		if (strcmp(path, favorites[i]) == 0) return i+1;//+1 to use return value as boolean
	}
	return 0;
}
