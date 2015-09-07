#include "folders.h"
#include "tinyxml2.h"

using namespace tinyxml2;

void loadFolders(hbfolder* folder){

	if(!folder)return;

    XMLDocument doc;
    if(doc.LoadFile(FOLDER_FILE))return;
	
	XMLNode * pRoot = doc.FirstChild();
	if (pRoot == NULL)return;

	XMLElement * pElement = pRoot->FirstChildElement("path");
	int i;
	for (i=1; i < MAX_FOLDER; i++){
		if (pElement == nullptr) break;
		const char* str = pElement->GetText();
		sprintf(folder->dir[i], "%s", str);
		folder->max = i;
		pElement = pElement->NextSiblingElement("path");
	}
/*
	XMLElement * pElement = pRoot->FirstChildElement("path");
	if (pElement == nullptr) return;
	const char* str = pElement->GetText();
	sprintf(folder->dir[1], "%s", str);
	folder->max = 1;
*/
/*	
	int i = 1;

	XMLElement* folderpath = doc.FirstChildElement( "FOLDERS" )->FirstChildElement( "path" );
	sprintf(folder->dir[1], "%s", folderpath->GetText());
	folder->max = 1;

	if (folderpath)
	{
		for(i=1; i < MAX_FOLDER; i++){
			folderpath = folderpath->NextSiblingElement();
			if (!folderpath) break;
			strncpy(folder->dir[i], folderpath->GetText(), 1023);
			folder->dir[i][1023-1]='\0';
		}
		folder->max = i;
	}
*/
}
