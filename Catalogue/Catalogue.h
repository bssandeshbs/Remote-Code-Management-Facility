#ifndef CATALOG_H
#define CATALOG_H
/////////////////////////////////////////////////////////////////////////////////
//  Catalogue.h - Searches for text in the file locations stored in data store //
//  ver 1.0																	   //
//  Language:      Visual C++ 2013, Update 2                                   //
//  Platform:      ASUS TP300L, Windows 8.1									   //
//  Application:   File Catalogue Project									   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University			   //
//                 (315) 751-4826, sbellurs@syr.edu							   //
/////////////////////////////////////////////////////////////////////////////////
/*
*
* Module Operations:
* ===================
* The purpose of this module is to search the text specifications within the files stored in the catalogue
*
* Required Files:
* ===============
* DataStore.h, DataStore.cpp, Catalogue.h, Catalogue.cpp
*
* Build Process:
* ==============
* cl /DTEST_CATALOGUE /EHsc ../DataStore/DataStore.cpp ../FileSystem/FileSystem.cpp ../FileManager
* /FileManager.cpp Catalogue.cpp
*
* Public Interface:
* =================
* constructor () :  Catalogue(DataStore& ds)
*			Input : DataStore& ds - data store object to navigate into file system
* boolean processTextInput(std::string); : process the text input entered from the command prompt
* void searchCatalog(const std::string); - search catalogue directly
* void searchCatalog(const std::string, std::vector<std::string>); - Search catalog with tokens
* void findPaths(std::list<pathref> itss, const std::string filename, const std::string key)
*					- Constructs absolute path from the input and invokes searchAndSaveFile()
* std::set<std::string> getFileSet(); - Returns the file set 
* Usage :
* ==================
* Catalogue c(DataStore &);
* c.processTextInput(inputString);
* if (tokens.size() > 0)
* searchCatalog(searchStr, tokens);
* else
* searchCatalog(searchStr);
* c.searchAndSaveFile(filepath,key)
*
* Maintenance History:
* ====================
* ver 1.0 : 10 Feb 14
* - first release
*/

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include "../DataStore/DataStore.h"
#include "../FileSystem/FileSystem.h"
#include "../FileManager/FileManager.h"

class Catalogue {

public:
	//----< constructor>-------------------
	Catalogue(DataStore& ds) :store_(ds){
	}
	using pathref = DataStore::pathIterator;
	using setIterator = std::set<std::string>::iterator;

	//----< returns Iterator pointing to first element of the set>--------------------
	inline setIterator begin() { return fileSet.begin(); }

	//----< returns Iterator pointing to last element of the set>--------------------
	inline setIterator end() { return fileSet.end(); }

	void searchCatalog(const std::string);
	void searchCatalog(const std::string, std::vector<std::string>);
	void processTextInput(std::string);
	std::set<std::string> getFileSet();

private:
	DataStore& store_;
	std::set<std::string> fileSet;
	void findPaths(std::list<pathref> itss, const std::string filename, const std::string key);
	void searchAndSaveFile(std::string absolutePath, std::string key);
};

#endif