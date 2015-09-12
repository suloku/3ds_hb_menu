#include "config.h"
#include "font.h"
#include "menu.h"
#include "background.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int caseSetting = 0;
int mixSetting = 0;
int totalfavs = 0;
int remembermenu = 0;
int rememberbrew = 0;
int lastFolder = 0;
int lastEntry = 0;
char favorites[MAX_FAVS][1024];
int current_theme = 0;
int totalThemes = 0;
int first_theme = 0;

void loadConfig(hbfolder* folder){

	if(!folder)return;

    XMLDocument doc;
    if(doc.LoadFile(FOLDER_FILE))return;
	
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
				folder->max = i;
			}
			subElement = subElement->NextSiblingElement("path");
		}
		subElement = pElement->FirstChildElement("mix_files");
		if (subElement != nullptr) subElement->QueryIntText(&mixSetting);
		
		subElement = pElement->FirstChildElement("case_sensitive");
		if (subElement != nullptr) subElement->QueryIntText(&caseSetting);

		subElement = pElement->FirstChildElement("disable_RF");
		if (subElement != nullptr) subElement->QueryIntText(&disableRF);

		subElement = pElement->FirstChildElement("theBrew");
		if (subElement != nullptr) subElement->QueryIntText(&rememberbrew);

		subElement = pElement->FirstChildElement("menu_pos");
		if (subElement != nullptr)
		{
			//Values
			subElement->QueryIntAttribute("remember_menu", &remembermenu);
			subElement->QueryIntAttribute("folder", &lastFolder);
			subElement->QueryIntAttribute("entry", &lastEntry);
		}
		if (!first_theme){ //Don't reload theme when changing themes
			subElement = pElement->FirstChildElement("current_theme");
			if (subElement != nullptr) subElement->QueryIntText(&current_theme);
			first_theme = 1;
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

		char theme[32];
		totalThemes = 0;
		for (i=1; i <= MAX_THEMES; i++){
			sprintf(theme, "theme%d", i);
			pElement = pRoot->FirstChildElement(theme);
			if (pElement != nullptr)
				totalThemes++;
		}

		sprintf(theme, "theme%d", current_theme);
		pElement = pRoot->FirstChildElement(theme);
		if (pElement != nullptr)
		{
			int temp;
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
			}
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

	subElement = xmlDoc.NewElement("mix_files");
	subElement->SetText(mixSetting);
	pElement->InsertEndChild(subElement);//close
	
	subElement = xmlDoc.NewElement("case_sensitive");
	subElement->SetText(caseSetting);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("disable_RF");
	subElement->SetText(disableRF);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("theBrew");
	subElement->SetText(rememberbrew);
	pElement->InsertEndChild(subElement);//close

	subElement = xmlDoc.NewElement("current_theme");
	subElement->SetText(current_theme);
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

	xmlDoc.SaveFile(FOLDER_FILE);
}

int isFavorite(char* path){
	int i;
	for (i=0; i<totalfavs ; i++){
		if (strcmp(path, favorites[i]) == 0) return i+1;//+1 to use return value as boolean
	}
	return 0;
}
