#include "folders.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int caseSetting = 0;
int mixSetting = 0;
int totalfavs = 0;
char favorites[MAX_FAVS][1024];

void loadFolders(hbfolder* folder){

	if(!folder)return;

    XMLDocument doc;
    if(doc.LoadFile(FOLDER_FILE))return;
	
	XMLNode * pRoot = doc.FirstChild();
	if (pRoot == NULL)return;

	int i;

	XMLElement * pElement = pRoot->FirstChildElement("folders");
	XMLElement * subElement = pElement->FirstChildElement("path");
	for (i=1; i < MAX_FOLDER; i++){
		if (subElement == nullptr) break;
		const char* str = subElement->GetText();
		sprintf(folder->dir[i], "%s", str);
		folder->max = i;
		subElement = subElement->NextSiblingElement("path");
	}
	subElement = pElement->FirstChildElement("mix_files");
	subElement->QueryIntText(&mixSetting);
	
	subElement = pElement->FirstChildElement("case_sensitive");
	subElement->QueryIntText(&caseSetting);

	subElement = pElement->FirstChildElement("disable_RF");
	subElement->QueryIntText(&disableRF);
	
	pElement = pRoot->FirstChildElement("favorites");
	subElement = pElement->FirstChildElement("fav");
	for (i=0; i < MAX_FAVS; i++){
		if (subElement == nullptr) break;
		const char* str = subElement->GetText();
		sprintf(favorites[i], "%s", str);
		totalfavs = i;
		subElement = subElement->NextSiblingElement("fav");
	}

}
