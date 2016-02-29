////////////////////////////////////////////////////////////////////////////////
//  FileManager.cpp - Responsible for searching files and directories         //
//  ver 2.0																	  //
//  Language:      Visual C++ 2013, Update 2								  //
//  Platform:      ASUS TP300L, Windows 8.1									  //
//  Application:   CSE 687 Project 1							              //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University			  //
//                 (315) 751-4826, sbellurs@syr.edu		                      //
////////////////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include <iostream>
#include <regex>

//----< process command line arguments >------------------------------------------
void FileManager::processInput(){
	try {
		path_ = ".";
		if (argc_ >= 2) {
			std::string arg = argv_[1];
			if (arg[0] != '/' && (arg.find("*") == std::string::npos)) {
				path_ = argv_[1];
			}
		}
		for (int i = 1; i < argc_; ++i)
		{
			std::string arg = argv_[i];

			//If options add to option vector
			if (arg == "/s" || arg == "/d" || std::regex_match(arg, std::regex("(/f)(.*)")))
			{
				if (std::regex_match(arg, std::regex("(/f)(.*)"))) {
					searchString = arg.erase(0, 2);
					addOption("/f");
				}
				else {
					addOption(arg);
				}
			}
			// else If string has *, then add to pattern vector
			else if ((arg.find("*") != std::string::npos)) {
				addPattern(arg);
			}
			//else add everything except first argument to option pattern vector
			else {
				if (i != 1) {
					addPattern(arg);
				}
			}
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}

//----< process command line arguments >------------------------------------------
void FileManager::processVectorInput(){
	try {
		path_ = ".";
	
		for (int i = 0; i < argc_; ++i)
		{
			if (i == 0) {
				std::string arg = res_[0];
				if (arg[0] != '/' && (arg.find("*") == std::string::npos)) {
					path_ = res_[0];
					continue;
				}
			}

			std::string arg = res_[i];

			//If options add to option vector
			if (arg == "/s" || arg == "/d" || std::regex_match(arg, std::regex("(/f)(.*)")))
			{
				if (std::regex_match(arg, std::regex("(/f)(.*)"))) {
					searchString = arg.erase(0, 2);
					addOption("/f");
				}
				else {
					addOption(arg);
				}
			}
			// else If string has *, then add to pattern vector
			else if ((arg.find("*") != std::string::npos)) {
				addPattern(arg);
			}
			//else add everything except first argument to option pattern vector
			else {
				if (i != 1) {
					addPattern(arg);
				}
			}
		}
	}
	catch (std::exception e) {
		std::cout << e.what();
	}
}


//----< store patterns from command line>----------------------------------
void FileManager::addPattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*") {
		patterns_.pop_back();
	}
	patterns_.push_back(patt);
}

//----< store options from command line>------------------------------------------
void FileManager::addOption(const std::string& option) {
	options_.push_back(option);
}

//----< search input path >--------------------------------------
void FileManager::search()
{
	std::string initPath = FileSystem::Directory::getCurrentDirectory();
	searchFiles(path_);
	bool is_in = std::find(options_.begin(), options_.end(), "/s") != options_.end();
	if (is_in)
	{
		searchDirectories(path_);
	}
	FileSystem::Directory::setCurrentDirectory(initPath);
	std::string initPath1 = FileSystem::Directory::getCurrentDirectory();
}

//----< search files in the path >--------------------------------
void FileManager::searchFiles(const std::string& path) {
	std::string initPath = FileSystem::Directory::getCurrentDirectory();
	bool res = FileSystem::Directory::setCurrentDirectory(path);
	for (auto patt : patterns_)
	{
		std::vector<std::string> files;
		if (res) {
			files = FileSystem::Directory::getFiles(FileSystem::Directory::getCurrentDirectory(), patt);
			for (auto f : files)
			{
				std::string p = FileSystem::Path::getFullFileSpec(f);
				store_.save(p);
			}
		}
	}
}

//----< search directories in the path >--------------------------------
void FileManager::searchDirectories(const std::string& path) {
	std::string curDir = FileSystem::Directory::getCurrentDirectory();
	std::vector<std::string> directories = FileSystem::Directory::getDirectories(curDir);
	std::vector<std::string> newDir;
	std::string test = FileSystem::Directory::getCurrentDirectory();
	for (auto f : directories)
	{
		if (!((f == ".") || (f == "..")))
		{
			dirCounter++;
			std::string res = test + "\\" + f;
			FileSystem::Directory::setCurrentDirectory(res);
			searchFiles(res);
			newDir = FileSystem::Directory::getDirectories();
			if (newDir.size() >= 1)
			{
				searchDirectories(".");
			}
		}
	}
}

//----< return search string >--------------------------------
std::string FileManager::getSearchString()
{
	return searchString;
}

//----< returns the total directory count >----------------------------------
int FileManager::getDirCount() {
	return dirCounter;
}

#ifdef TEST_FILEMANAGER
int main() {
	DataStore ds;
	int argc = 4;
	char* argv[] = { "C://filename. cpp", "/s", "*.cpp", "*.h" };

	FileManager fm(ds, argc, argv);
	fm.processInput();
	fm.search();

	DataStore::iterator iter = ds.begin();
	using pathref = DataStore::pathIterator;

	std::cout << "\n FILE NAME \t\t PATH \n";
	std::cout << "------------------------------------------------\n\n";
	for (iter = ds.begin(); iter != ds.end(); iter++)
	{
		std::cout << iter->first << "\n";
		std::list<pathref> itss = iter->second;
		std::list<pathref>::iterator itssr;
		for (itssr = itss.begin(); itssr != itss.end(); itssr++)
		{
			std::cout << "\t\t\t" << (*(*itssr)) << "\n";
		}
	}
	std::cout << "\n\n";
	fm.getDirCount();
}
#endif