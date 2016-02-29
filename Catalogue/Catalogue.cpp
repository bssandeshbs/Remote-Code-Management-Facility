///////////////////////////////////////////////////////////////////////////////////
//  Catalogue.cpp - Searches for text in the file locations stored in data store //
//  ver 1.0																	     //
//  Language:      Visual C++ 2013, Update 2                                     //
//  Platform:      ASUS TP300L, Windows 8.1									     //
//  Application:   File Catalogue Project									     //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University			     //
//                 (315) 751-4826, sbellurs@syr.edu							     //
///////////////////////////////////////////////////////////////////////////////////
#include "Catalogue.h"

//----< process input from command prompt >--------------------------
void Catalogue::processTextInput(std::string inputString) {

	fileSet.clear();
	std::string searchStr;
	std::string patternStr;
	std::string ip;
	// finding the first 2 characters are /f
	if (inputString.find("/f") != std::string::npos) {
		ip = inputString.substr(2, inputString.length());
	}
	else {
		ip = inputString;
	}
	//finding if the string is having ""
	std::size_t first = ip.find("\"");
	if (first != std::string::npos) {
		std::size_t second = ip.find("\"", first + 1, 1);
		if (second != std::string::npos) {
			searchStr = ip.substr(first + 1, second - 1);
			patternStr = ip.substr(second + 1);
		}
	}
	//splitting the string with " "
	else {
		std::size_t second = ip.find(" ");
		if (second != std::string::npos) {
			searchStr = ip.substr(0, 4);
			patternStr = ip.substr(second + 1);
		}
		else {
			searchStr = ip;
		}
	}

	std::string buf; // Have a buffer string
	std::stringstream ss(patternStr); // Insert the string into a stream
	std::vector<std::string> tokens;
	// Inserting the patterns to vector 
	while (ss >> buf)
		tokens.push_back(buf);

	if (tokens.size() > 0)
		searchCatalog(searchStr, tokens);
	else
		searchCatalog(searchStr);
}

//----< search catalog based on key>--------------------------
void Catalogue::searchCatalog(const std::string key) {
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
			searchAndSaveFile(absolutePath, key);
		}
	}
}

//----< search the catalog for specific patterns >--------------------------
void Catalogue::searchCatalog(const std::string key, std::vector<std::string> list){
	DataStore::iterator iter = store_.begin();
	for (iter = store_.begin(); iter != store_.end(); iter++)
	{
		std::string filename = iter->first;
		std::string extension = FileSystem::Path::getExt(filename);

		for (auto patterns : list) {
			std::string temp = patterns;
			std::size_t found = patterns.find("*");
			if (found != std::string::npos) {
				if (patterns.size() > 2) {
					std::string patt = patterns.substr(found + 2);
					if (patt.find("*") != std::string::npos) {
						findPaths(iter->second, filename, key);
					}
					else if (patt == extension) {
						findPaths(iter->second, filename, key);
					}
				}
			}
			else if (filename == patterns) {
				findPaths(iter->second, filename, key);
			}
		}
	}
}

//----< search the path for specific file name >--------------------------
void Catalogue::findPaths(std::list<pathref> itss, const std::string filename, const std::string key) {
	std::list<pathref>::iterator itssr;
	for (itssr = itss.begin(); itssr != itss.end(); itssr++)
	{
		std::string path = (*(*itssr));
		std::string absolutePath = path + "\\" + filename;
		searchAndSaveFile(absolutePath, key);
	}
}

//----< search the key in file and store  >--------------------------
void Catalogue::searchAndSaveFile(const std::string absolutePath, const std::string key) {

	FileSystem::File in(absolutePath);
	in.open(FileSystem::File::in);

	boolean found = false;
	while (in.isGood()) {
		std::string filetxt = in.readAll();
		if (filetxt.find(key) != std::string::npos) {
			found = true;
			break;
		}
	}
	if (found) {
		fileSet.insert(absolutePath);
	}
}

//----< gets the file list with full name and path  >--------------------------
std::set<std::string> Catalogue::getFileSet() {
	return fileSet;
}

#ifdef TEST_CATALOGUE
int main() {
	DataStore ds;

	int argc = 4;
	char* argv[] = { "C://filename.cpp", "/s", "*.cpp", "*.h" };

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
	Catalogue c(ds);
	c.processTextInput("include");
	std::set<std::string> inputSet = c.getFileSet();
	for (auto element : inputSet) {
		unsigned found = element.find_last_of("/\\");
		std::string path = element.substr(0, found);
		std::string file = element.substr(found + 1);
		std::cout << "File : " << file << "\n";
		std::cout << "Path : " << path << "\n\n";
	}
	std::cout << "\n\n";
}
#endif