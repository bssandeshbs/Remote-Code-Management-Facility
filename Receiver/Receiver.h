#ifndef RECEIVER_H
#define RECEIVER_H
/////////////////////////////////////////////////////////////////////
//  Receiver.h - Receives the header and data from Client Sender   //
//				   and Server receiver							   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//				   Remote Code Management Facility                 //	
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This class provides the receiver functionalities to receive the data from both client and server
* packages, the receiver in server side is responsible for receiving the data, the receiver in client 
* side is responsible for receving the acknowldegment 
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h Message.cpp Receiver.h Receiver.cpp
* QServer.cpp cpp11-BlockingQueue.coo Display.cpp FileManager.cpp FileSystem.cpp DataStore.cpp DataStore.h
* Caatalogue.cpp Catalogue.h
*
* Build Process:
* ==============
* cl /DTEST_RECEIVER ../ApplicationHelpers/AppHelpers.cpp ../Message/Message.cpp
* ../sockets/sockets.cpp Receiver.cpp ../QServer/QServer/QServer.cpp ../QServer/Cpp11-BlockingQueue
* /Cpp11-BlockingQueue.cpp ../Display/Display.cpp ../FileManager/FileManager.cpp ../FileSystem/FileSystem.cpp 
* ../Catalogue/Cataloge.cpp ../DataStore/DataStore.cpp
*
* Public Interface:
* =================
* void operator()(Socket& socket_);
*  - handles the core logic of Receiver, new thread is created and the functor is called, taking 
*    socket as input
*   Input - Socket - socket to perform send and recv operations
*   
* Usage :
* =================
* SocketListener sl(9080, Socket::IP6);
* Receiver cp("output/");
* sl.start(cp);
* std::thread clientThread(std::ref(co), std::move(clientSocket));
*
* Maintenance History:
* ====================
* ver 2.0 : 27th Apr 15
*  updated the reciever code for download,text and file searches
* ver 1.0 : 05 Apr 15
* - first release
*/
#include <iostream>
#include <string>
#include <vector>
#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../QServer/QServer/QServer.h"
#include "../QServer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Display/Display.h"
#include <thread>
#include <fstream>
#include <mutex>
#include <vector>
#include "../Message/Message.h"

#include "../Catalogue//Catalogue.h"
#include "../DataStore/DataStore.h"
#include "../FileManager/FileManager.h"
#include "../FileSystem/FileSystem.h"

using namespace ApplicationHelpers;

class Receiver {
public:
	Receiver(std::string path) {
		this->path = path;
	}
	void operator()(Socket& socket_);

private:
	void stopClientHandlerThread(Socket& socket_);
	bool uploadFile(Socket& socket_, std::string fileName);
	void stopReceiverThread(Socket& socket_, Message msg);
	std::vector<std::string> getDetails(const std::string str, const char delim);
	void fileUploadServer(Socket& socket_, Message msg);
	void fileUpload(Socket& socket_, Message msg, std::string headerInfo, std::string fileName);
	void serverFileUploadReq(Socket& socket_, Message msg, std::string temp);
	void fileUploadConfirm(Socket& socket_, Message msg);
	void determineType(std::string command, Socket& socket_, Message msg, std::string temp);
	void textSearch(Socket& socket_, Message msg, std::string headerInfo);
	BlockingQueue<std::string> &q1 = QServer<std::string, 5>().get();
	std::string path;
	void performTextSearch(std::string textString, std::vector<std::string> patterns, Socket& socket_);
	void sendSearchResults(std::string inputString, Socket& socket_);
	std::string receiveResult(Socket& socket_, std::string fileName);
	void displayTextSearchResults(Socket& socket_, Message msg, std::string headerInfo);
	void fileSearch(Socket& socket_, Message msg, std::string headerInfo);
	void displayFileSearchResults(Socket& socket_, Message msg, std::string headerInfo);
};

#endif