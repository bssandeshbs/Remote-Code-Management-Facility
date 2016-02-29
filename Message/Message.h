#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////
//  Message.h - Constructs the Message header                      //
//                before sending data		                       //
//								  								   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* ===================
* This class provides serveral wrapper methods to get the header information, complete message 
* and also set the header content. 
*
* Required Files:
* ===============
* Message.h, Message.cpp
*
* Build Process:
* ==============
* cl /DTEST_MESSAGE Message.cpp
*
* Public Interface:
* =================
* void setMessage(int contentLength, std::string command, std::string sendAddr,std::string recvAddr, 
* std::string body) - Set the properties of the message,
*         Input - contentLength - length of body, command - message to perform action
*	      senderAddr - address of the sender, recv - address of the receiver, body - body content 
* char* getMessage() - get properties of the message and returns a char pointer
*        output - returns the char pointer containing all the information
* void setHeader(int contentLength, std::string command, std::string sendAddr,std::string recvAddr); 
*  - Set only the message header properties
*         Input - contentLength - length of body, command - message to perform action
*	      senderAddr - address of the sender, recv - address of the receiver
* char* getHeader() - returns only the header information 
* int getSize() - returns the complete size of the message object
* void setDetails(char * details) - set the message objects from the input char pointer
* std::string getBody()  - returns the body information
* std::string getSendAdd() - returns the sender address 
* std::string getRecvAdd() - returns the receiver address
* std::string getCommand() - returns the command 
* int getContentLength() - returns the content length
* int getHeaderSize() - returns the header size
* void setCommand(std::string string) - setter function to set command
* void setSenderAddr(std::string addr) - settter function to set sender address
* void setRecvAddr(std::string addr) - setter function to set receiver adddress
* void setBody(std::string body) - setter function to set the body
*
* Usage :
* ==================
*  Message m;
*  m.setMessage(20, "TEST_SENDFILE", "localhost:8090", "localhost:8080", "hello.txt");
*  char* res = m.getMessage();
*  int contentLen = m.getContentLength()
*
* Maintenance History:
* ====================
* ver 1.0 : 05 Apr 14
* - first release
*/
#include <iostream>
#include <string>
#include <vector>

class Message {
public :
	void setMessage(int contentLength, std::string command, std::string sendAddr,
		std::string recvAddr, std::string body);
	char* getMessage();
	void setHeader(int contentLength, std::string command, std::string sendAddr,
		std::string recvAddr);
	char* getHeader();
	int getSize();
	void setDetails(char * details);
	std::string getBody() ;
	std::string getSendAdd() ;
	std::string getRecvAdd() ;
	std::string getCommand() ;
	int getContentLength();
	int getHeaderSize();
	void setCommand(std::string string);
	void setSenderAddr(std::string addr);
	void setRecvAddr(std::string addr);
	void setBody(std::string body);

private:
	std::vector<std::string> split(std::string str, char delim);
	std::string contentLength;
	std::string command;
	std::string sendAddr;
	std::string recAddr;
	std::string body;
	std::string delimiter = "\n";
	std::string result;
};

#endif