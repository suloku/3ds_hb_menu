#include "config.h"
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

void writeFolders(hbfolder* folder){

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
		pElement->InsertEndChild(subElement);
	}

	subElement = xmlDoc.NewElement("mix_files");
	subElement->SetText(mixSetting);
	pElement->InsertEndChild(subElement);
	
	subElement = xmlDoc.NewElement("case_sensitive");
	subElement->SetText(caseSetting);
	pElement->InsertEndChild(subElement);

	subElement = xmlDoc.NewElement("disable_RF");
	subElement->SetText(disableRF);
	pElement->InsertEndChild(subElement);

	pRoot->InsertEndChild(pElement);
//End folders
//Favs
	pElement = xmlDoc.NewElement("favorites");
	for (i=0; i<totalfavs; i++){
		subElement = xmlDoc.NewElement("fav");
		subElement->SetText(favorites[i]);
		pElement->InsertEndChild(subElement);
	}
	pRoot->InsertEndChild(pElement);
//End favs

	xmlDoc.SaveFile(FOLDER_FILE);
}
