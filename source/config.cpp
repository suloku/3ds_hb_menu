#include "config.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int caseSetting = 0;
int mixSetting = 0;
int totalfavs = 0;
int remembermenu = 0;
int lastFolder = 0;
int lastEntry = 0;
char favorites[MAX_FAVS][1024];

void loadConfig(hbfolder* folder){

	if(!folder)return;

    XMLDocument doc;
    if(doc.LoadFile(FOLDER_FILE))return;
	
	XMLNode * pRoot = doc.FirstChild();
	if (pRoot == NULL)return;

	int i;

	XMLElement * pElement = pRoot->FirstChildElement("folders");
	XMLElement * subElement = pElement->FirstChildElement("path");
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
	subElement->QueryIntText(&mixSetting);
	
	subElement = pElement->FirstChildElement("case_sensitive");
	subElement->QueryIntText(&caseSetting);

	subElement = pElement->FirstChildElement("disable_RF");
	subElement->QueryIntText(&disableRF);

	subElement = pElement->FirstChildElement("menu_pos");
	if (subElement != nullptr)
	{
		//Values
		subElement->QueryIntAttribute("remember_menu", &remembermenu);
		subElement->QueryIntAttribute("folder", &lastFolder);
		subElement->QueryIntAttribute("entry", &lastEntry);
	}

	pElement = pRoot->FirstChildElement("favorites");
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

	subElement = xmlDoc.NewElement("remember_menu");
	subElement->SetText(remembermenu);
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
