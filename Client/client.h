#ifndef CLIENT_H
#define CLIENT_H
/////////////////////////////////////////////////////////////////////
//  Client.h - Enquees the upload request and spaws client and     //
//             and server threads                                  //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//				   Remote Code Managemenet						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This class starts the client sender and client receiver threads. The client sender reads the queue 
* and starts to perform the upload/download based on the command. The client receiver receives the 
* acknowledgement from server sender
*
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h Message.cpp Sender.h Sender.cpp
* Receiver.h Receiver.cpp Client.h Client.cpp
* FileSystem.cpp FileSystem.h DataStore.cpp DataStore.h Catalogue.cpp Catalogue.h
*
* Build Process:
* ==============
* cl /DTEST_CLIENT Client.cpp ../ApplicationHelpers/AppHelpers.cpp ../Message/Message.cpp
* ../sockets/sockets.cpp ../Receiver/Receiver.cpp ../QServer/QServer/QServer.cpp ../QServer/Cpp11-
* BlockingQueue ../Sender/Sender.cpp /Cpp11-BlockingQueue.cpp ../Display/Display.cpp
* ../FileManager/FileManager.cpp ../FileSystem/FileSystem.cpp  ../Catalogue/Cataloge.cpp ../DataStore/DataStore.cpp
*
* Public Interface:
* =================
*
*   void startReceiver() - start the receiver thread
*	void startSender() - start the sender thread
*   void readFile(std::string path) - read the file from the path specified
*   void start() - Start sender and receiver threads
*   void parseline(std::string line) - reads the line 
* Usage :
* ==================
* Server server(address, port, serverPath);
* std::thread receiver(&Server::startReceiver, server);
* std::thread sender(&Server::startSender, server);
* 
*
* Maintenance History:
* ====================
* ver 2.0 : 27 Apr 15
*  added new implementation to support file and text searches
* ver 1.0 : 05 Apr 15
* - first release
*/
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <set>

#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"

class Client{
public:
	Client(std::string address = "localhost", int port = 9081,std::string path="") {
		this->serverAddress = address;
		this->port = port;
		this->clientPath = path;
	}
	void startReceiver();
	void startSender();
	void readFile(std::string path);
	void start();
	void parseline(std::string line);
private:
	bool connectadd(SocketConnecter& si, std::string address, int port);
	void readQueue();
	std::vector<std::string> getDetails(const std::string str, const char delim);
	void enQuee(std::string operation, std::string address, int port, std::string fileName);
	std::string dequee();
	BlockingQueue<std::string> &q2 = QServer<std::string, 50>().get();
	std::string serverAddress;
	int port;
	std::string clientPath;
	bool isConnected = false;
	std::vector<std::string> addressSet;
	void connect(SocketConnecter& si);
	bool isShutdown = false;
};
#endif
