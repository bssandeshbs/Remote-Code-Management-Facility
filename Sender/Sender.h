#ifndef SENDER_H
#define SENDER_H

/////////////////////////////////////////////////////////////////////
//  Sender.h - Responsible for sending the header and data         //
//				 to Receiver									   //
//																   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This class provides the sender functionalities to send the data for both client and server 
* packages, the sender in server side is responsible for provide acknowldegment , in client 
* side its responsible for uploading the data
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h Message.cpp Sender.h Sender.cpp
* FileSystem.cpp FileSystem.h DataStore.cpp DataStore.h Catalogue.cpp Catalogue.h
*
* Build Process:
* ==============
* cl /DTEST_SENDER ../ApplicationHelpers/AppHelpers.cpp ../Message/Message.h 
* ../sockets/sockets.cpp Sender.cpp ../FileManager/FileManager.cpp ../FileSystem/FileSystem.cpp 
* ../Catalogue/Cataloge.cpp ../DataStore/DataStore.cpp
*
* Public Interface:
* =================
* void clientFileHandling(Socket& si, std::vector<std::string> messageDetails,std::string path)
* - function to upload a binary/text file from client sender to server receiver
*   Input - Socket to send, messageDetails - Vector of client and server address, path to read
*	void clientFileDownloadHandling(Socket& si, std::vector<std::string> messageDetails)
*        - Sends a request to server to request file
*   Input - Socket to send, messageDetails - Vector of client and server address
*	void clientAckowledgement(Socket& si, Message message);
*       - Sends a acknowledgement of file upload from server sender
*   Input - Socket to send, Message - header information
*	void serverClientUpload(Socket& si, Message message, std::string path);
*        - Responsible for sending the file from server to client 
*    Input - Socket, message - header information, path to store
*   void performSearch(Socket& si, Message message, bool searchType, std::string serverPath);
*          - functions which reads the header message and search type and performs the appropriat
*            search operation
*   void sendTextSearch(Socket& si, std::vector<std::string> messageDetails);
*			- functions which sends a message to perform text search,
*           Input - socket, vector of message Details
*   void sendFileSearch(Socket& si, std::vector<std::string> messageDetails);
*         Input - socket, vector of message details
*         function which performs file search
* Usage :
* ==================
*  Sender s;
*  s.clientTestFileHandling(si, messageDetails, "input/");
*  s.performSearch(si, message, true, "input/");
*
*
* Maintenance History:
* ====================
* ver 2.0 : 27th Apr 15
*  added new methods for download, text search and file searches
* ver 1.0 : 05 Apr 15
* - first release
*/
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include <iostream>
#include "../Message/Message.h"
#include "../Display/Display.h"

#include "../Catalogue//Catalogue.h"
#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"
#include "../FileSystem/FileSystem.h"
#include <string>
#include <fstream>

using namespace ApplicationHelpers;

class Sender {
public:
	void clientFileHandling(Socket& si, std::vector<std::string> messageDetails,std::string path);
	void clientFileDownloadHandling(Socket& si, std::vector<std::string> messageDetails);
	void clientAckowledgement(Socket& si, Message message);
	void serverClientUpload(Socket& si, Message message, std::string path);
	void demoMessage(Socket& si, std::vector<std::string> messageDetails);
	void performSearch(Socket& si, Message message, bool searchType, std::string serverPath);
	void sendTextSearch(Socket& si, std::vector<std::string> messageDetails);
	void sendFileSearch(Socket& si, std::vector<std::string> messageDetails);
private:
	std::vector<std::string> getDetails(const std::string str, const char delim);	
	void performTextSearch(std::string textString, std::vector<std::string> patterns, Socket& socket_,
		Message message, std::string serverPath);
	void sendSearchResults(std::string inputString, Socket& si, Message message);
	void performFileSearch(std::string textString, std::vector<std::string> patterns, Socket& socket_,
		Message message, std::string serverPath);
	void sendFileSearchResults(std::string inputString, Socket& si, Message message);
};

#endif