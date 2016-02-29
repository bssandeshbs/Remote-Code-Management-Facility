/////////////////////////////////////////////////////////////////////
//  MockChannelTest.cpp - Test Stub for MockChannel				   //
//																   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Remote Code Managemenent						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu				   //
//  original author : Jim Fawcett								   //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "..\MockChannel\MockChannel.h"

using Message1 = std::string;

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IClient* client = objFact.createClient(pSendr, pRecvr);
	client->start("localhost", "9080", "input/");
	pSendr->postMessage("Hello World");
	pSendr->postMessage("CSE687 - Object Oriented Design");
	Message1 msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pSendr->postMessage("stopping");
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	client->stop();
	pSendr->postMessage("quit");
	std::cin.get();
}