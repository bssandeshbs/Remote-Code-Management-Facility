#ifndef FILEMGR_H
#define FILEMGR_H
////////////////////////////////////////////////////////////////////////////////
//  FileManager.h - Responsible for searching files and directories           //
//  ver 2.0																	  //
//  Language:      Visual C++ 2013, Update 2								  //
//  Platform:      ASUS TP300L, Windows 8.1									  //
//  Application:   CSE 687 Project 1							              //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University			  //
//                 (315) 751-4826, sbellurs@syr.edu		                      //
////////////////////////////////////////////////////////////////////////////////

/*
*
* Module Operations:
* ===================
* The purpose of this module is to handle all the file related tasks such as getting files from directories
* and iterating sub directories recursively and invokes the data store to save the data.
* This module will utilize all the services of File System. File
*
* Required Files:
* ===============
* DataStore.h, DataStore.cpp, FileSystem.cpp, FileSystem.h, FileManager.h, FIleManager.cpp
*
* Build Process:
* ==============
* cl /DTEST_FILEMANAGER ../DataStore/DataStore.cpp ../FileSystem/FileSystem.cpp FileManager.cpp
*
* Public Interface:
* =================
* constructor () :  FileManager(DataStore& ds, int argc, char* argv[])
*			Input : DataStore& ds - data store object to save the file details
* void processInput() : parses the command line input and stores into option and command vectors
* void search(); - check the command option and invokes searchFiles() and SearchDirectories
* string getSearchString(); - returns the search string
*
* Usage :
* ==================
* FileManager fm(ds, argc, argv);
* fm.processInput();
* fm.search();
* fm.getDirCount()
*
* Maintenance History:
* ====================
* ver 2.0 : 27 Apr 15
*  fixed bug in search method to save the initial path
* ver 1.0 : 10 Feb 15
* - first release
*/

#include "../FileSystem/FileSystem.h"
#include "../DataStore/DataStore.h"
#include <iostream>
#include <string>

class FileManager
{

public:

	//----< constructor>-------------------
	FileManager(DataStore& ds, int argc, char* argv[]) : store_(ds), argc_(argc), argv_(argv) {
		patterns_.push_back("*.*");
	}

	FileManager(DataStore& ds, int argc, std::vector<std::string> r) : store_(ds), argc_(argc), res_(r){
		patterns_.push_back("*.*");
	}

	using patterns = std::vector < std::string > ;
	using options = std::vector < std::string > ;
	using optionIterator = options::iterator;
	using patternIterator = patterns::iterator;

	void processInput();
	void search();
	std::string getSearchString();
	int getDirCount();

	//----< returns Iterator pointing to first element of the option vector>-------------------
	inline patternIterator optionBegin() { return options_.begin(); }

	//----< returns Iterator pointing to first element of the option vector>-------------------
	inline patternIterator optionEnd() { return options_.end(); }

	//----< returns Iterator pointing to first element of the pattern vector>-------------------
	inline optionIterator patternBegin() { return patterns_.begin(); }

	//----< returns Iterator pointing to first element of the pattern vector>-------------------
	inline optionIterator patternEnd() { return patterns_.end(); }
	void processVectorInput();
private:
	std::string path_;
	DataStore & store_;
	patterns patterns_;
	options options_;
	char** argv_;
	int argc_;
	std::string searchString;
	int dirCounter = 1;
	int fileCount;
	std::vector<std::string> res_;

	void searchFiles(const std::string& path);
	void addPattern(const std::string& patt);
	void addOption(const std::string& patt);
	void searchDirectories(const std::string& path);

};

#endif