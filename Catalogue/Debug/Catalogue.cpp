/////////////////////////////////////////////////////////////////////
//  Catalogue.h - Searches text in the file locations			   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   File Catalogue Project		                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
#include "Catalogue.h"
#include "../FileSystem/FileSystem.h"

std::list<std::string> Catalogue::searchCatalog(const std::string key) {
	DataStore::iterator iter = store_.begin();
	for (iter = store_.begin(); iter != store_.end(); iter++)
	{
		std::list<pathref> itss = iter->second;
		std::list<pathref>::iterator itssr;
		for (itssr = itss.begin(); itssr != itss.end(); itssr++)
		{
			std::string filename = iter->first;
			std::string path = (*(*itssr));
			std::string absolutePath = path + "\\" + filename;

			FileSystem::File in(absolutePath);
			in.open(FileSystem::File::in);

			boolean fnd = false;
			
			while (in.isGood()) {
					
						std::string temp = in.readAll();
						if (temp.find(key) != std::string::npos) {
							fnd = true;
							break;
						}

					}
				if (fnd) {
				fileList.push_back(absolutePath);
			}
		}
	}
	return fileList;
}

void Catalogue::searchCatalogWithPatterns(const std::string key, std::vector<std::string> list){
	DataStore::iterator iter = store_.begin();
	for (iter = store_.begin(); iter != store_.end(); iter++)
	{
		std::string filename = iter->first;
		std::string extension = FileSystem::Path::getExt(filename);
		std::cout << "\n extension" << extension;
		
		for (auto patterns : list) {
			std::cout << "\n Iterating patterns";
			std::size_t found = patterns.find("*");
			if (found != std::string::npos) {
				std::string patt= patterns.substr(found+2);
				std::cout << "splitting patern" << patt;
				if (patt == extension) {
					std::cout << "\n Checking Files with pattern match";
					checkfile(iter->second, filename, key);
				}
			}
			
			else if (filename == key) {
				std::cout << "Checking Files with exact key";
				checkfile(iter->second, filename, "include");
			}
		}
			
	}
}

void Catalogue::processInput(std::string inputString) {

	std::string searchStr;
	std::string patternStr;
	std::size_t first = inputString.find("\"");

	if (first != std::string::npos) {
		std::size_t second = inputString.find("\"", first + 1, 1);
		if (second != std::string::npos)
			std::cout << "second 'needle' found at: " << second << '\n'; 
		searchStr = inputString.substr(first + 1, second - 1);	
		patternStr = inputString.substr(second + 1);
	}
	else {
		std::size_t second = inputString.find(" ");
		if (second != std::string::npos) {
			searchStr = inputString.substr(0, 4);
			patternStr = inputString.substr(second + 1);
		}		
	}

	std::string buf; // Have a buffer string
	std::stringstream ss(patternStr); // Insert the string into a stream

	std::vector<std::string> tokens; // Create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);

	searchCatalogWithPatterns(searchStr, tokens);
}

void Catalogue::checkfile(std::list<pathref> itss,std::string filename,std::string key) {
	
	std::cout << "filename" << filename << "Key :" << key;
	std::list<pathref>::iterator itssr;
	for (itssr = itss.begin(); itssr != itss.end(); itssr++)
	{
		//handle case of *.txt & *txt
		//std::string filename = iter->first;
		std::string path = (*(*itssr));
		std::string absolutePath = path + "\\" + filename;

		FileSystem::File in(absolutePath);
		in.open(FileSystem::File::in);

		boolean fnd = false;

		while (in.isGood()) {
			std::string temp = in.readAll();
			if (temp.find(key) != std::string::npos) {
				fnd = true;
				break;
			}
			else {
				std::cout << "Failure";
			}
		}
		if (fnd) {
			fileList.push_back(absolutePath);
			std::cout << "Size" << fileList.size();
		}
	}
}