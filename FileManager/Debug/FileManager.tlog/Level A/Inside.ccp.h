#ifndef FILEMGR_H
#define FILEMGR_H
// DataStore.h- ...........

#include "FileSystem.h"
#include "../DataStore/DataStore.h"
#include <iostream>
#include <string>

class FileMgr
{
public:
	//using iterator = datastore::iterator;
	using patterns = std::vector <std::string>;
	FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds) {
		patterns_.push_back("*.*");
	}

	void addPattern(const std::string& patt)
	{
	//	std::cout << "Size" << patterns_.size();
		if (patterns_.size() == 1 && patterns_[0] == "*.*") {
			patterns_.pop_back();
		}

		patterns_.push_back(patt);

	}

	void search()
	{
		find(path_);
	}

	void find(const std::string& path)
	{
		
		sendPatternsAndFilesToMap(path);
			
		iterateDirectories(); 
			
			//for (auto f : files)
			//{
			//	cout << "Add Path \t" << f;
			//	std::string p = FileSystem::Path::getFullFileSpec(f);
			//	cout << "Path of the file " << f << ":: Path" << p <<"\n";
			////	cout << "Add Path \n" << f;
			//	
			//}
			//store_.addPath("C://test/hi.cpp");
			//store_.addPath("D://test/hi.cpp");
				
	}

	void sendPatternsAndFilesToMap(const std::string& path) {
		 for (auto patt : patterns_)
		 {
			 cout << "Path :" << path << "\n";
			 std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
			 for (auto f : files)
			 {
				 //				cout << "Add Path \t" << f;
				 std::string p = FileSystem::Path::getFullFileSpec(f);
				 cout << "Path of the file " << f << ":: Path" << p << "\n";
				 //				cout << "Add Path \n" << f;
				 store_.addPath(p);
			 }
		 }
		
	}

	void iterateDirectories() {
		std::vector<std::string> directories = FileSystem::Directory::getDirectories();
		std::vector<std::string> newDir; 
		std::string test = FileSystem::Directory::getCurrentDirectory();
		for (auto f : directories)
		{
			std::cout << "\nDirectories Now Iterating::" << f;
			if (!((f == ".") || (f == ".."))) {				
				string res = test + "\\" + f;
				FileSystem::Directory::setCurrentDirectory(res);
				cout << "New Dir :\n" << FileSystem::Directory::getCurrentDirectory();
				sendPatternsAndFilesToMap(res);
				newDir = FileSystem::Directory::getDirectories();
				cout << "\nSize \n" << newDir.size();
				if (newDir.size() >= 1) {
					std::cout << "Greater than 1";
					iterateDirectories();
				}
				else {
					std::cout << "Not Calling";
				}
				
				//f = "";
			}
		}
	}
private:
	std::string path_;
	DataStore & store_;
	patterns patterns_;
};

#endif