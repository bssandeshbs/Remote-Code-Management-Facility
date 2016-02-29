#ifndef SERVER_H
#define SERVER_H
/////////////////////////////////////////////////////////////////////
//  Server.h -   Accepts the request from client, processes        //
//				   the header and data, provides ack to client	   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//				   Remote Code Management						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ===================
* This class starts the server sender and server receiver threads. The server sender receives the buffer
* data from the client sender and then stores in the server, the server receiver reads the queue and 
* performs the required operation
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h Message.cpp Sender.h Sender.cpp
* Receiver.h Receiver.cpp Server.h Server.cpp
* FileSystem.cpp FileSystem.h DataStore.cpp DataStore.h Catalogue.cpp Catalogue.h
*
* Build Process:
* ==============
* cl /DTEST_SERVER_1 Server.cpp ../ApplicationHelpers/AppHelpers.cpp ../Message/Message.cpp
* ../sockets/sockets.cpp ../Receiver/Receiver.cpp ../QServer/QServer/QServer.cpp ../QServer/Cpp11-
* BlockingQueue ../Sender/Sender.cpp /Cpp11-BlockingQueue.cpp ../Display/Display.cpp
* ../FileManager/FileManager.cpp ../FileSystem/FileSystem.cpp  ../Catalogue/Cataloge.cpp ../DataStore/DataStore.cpp
*
* Public Interface:
* =================
*
* void startReceiver() - start the receiver thread
* void startSender() -  start the sender thread
*
* Usage :
* ==================
* Client client(address, port, clientPath);
* client.readFile(clientPath + inputFiles);
* std::thread receiver(&Client::startReceiver, client);
* std::thread sender(&Client::startSender, client);
*
* Maintenance History:
* ====================
* ver 2.0 : 27 Apr 15
*           added new methods for file and text search
* ver 1.0 : 05 Apr 15
* - first release
*/
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../Receiver/Receiver.h"
#include "../Sender/Sender.h"

class Server {
public:
	Server(std::string address = "localhost", int port = 9080,std::string path ="output\\") {
		this->serverAddress = address;
		this->port = port;
		this->serverPath = path;
	}
	void startReceiver();
	void startSender();
private:
	std::vector<std::string> getDetails(const std::string str, const char delim);
	BlockingQueue<std::string> &q1 = QServer<std::string, 5>().get();
	std::string serverAddress;
	int port;
	std::string serverPath;
};
#endif
