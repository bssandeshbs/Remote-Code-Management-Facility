/////////////////////////////////////////////////////////////////////
//  MockChannel.cpp - Builds as DLL so that C++\CLI client can user//
//				  native code									   //
//																   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Remote Code Managemenent						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu				   //
//  original author : Jim Fawcett								   //
/////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>
#include "../Client/client.h"

using BQueue = BlockingQueue < Message1 > ;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
	void postMessage(const Message1& msg);
	BQueue& queue();
private:
	BQueue sendQ_;
};

void Sendr::postMessage(const Message1& msg)
{
	sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	Message1 getMessage();
	BQueue& queue();
private:
	BQueue recvQ_;
};

Message1 Recvr::getMessage()
{
	return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
	return recvQ_;
}

/////////////////////////////////////////////////////////////////////////////
// ClientUI class
// - reads messages from Sendr and enquees to client, recieves messages from the client reciever
//   and enquees to Recvr Queue
class ClientUI : public IClient
{
public:
	ClientUI(ISendr* pSendr, IRecvr* pRecvr);
	void start(std::string address, std::string port,std::string path);
	void stop() {

	}
	std::vector<std::string> getDetails(const std::string str, const char delim);
private:
	std::string address;
	std::string port;
	std::string path;
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	Client* cl;
	bool stop_ = false;
};

//----< constructor >------------
ClientUI::ClientUI(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates a client instance which invokes sender and receiver threads >------------
void ClientUI::start(std::string address, std::string port,std::string path)
{
	cl = new Client(address,stoi(port),path); // Default Path
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		cl->start();
		if (pSendr == nullptr || pRecvr == nullptr) {
			return;
		}
		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		BlockingQueue<std::string> &q1 = QServer<std::string, 5>().get();
		while (!stop_) {
			Message1 msg = sendQ.deQ();  // will block here so send quit message when stopping
			std::vector<std::string> details = getDetails(msg, '?');
			std::string finalString = "";
			bool passed = true; bool first = true;
			for (auto detail : details) {
				if (passed) cl->parseline(detail);
				else {
					passed = true; 	continue;
				}
				std::string res = q1.deQ();
				if (res.substr(0, 6) == "Failed") {
					if (first) { 
						finalString = res; first = false;
					}
					else finalString = finalString + "|" + res;
					passed = false;
				} else {
					passed = true;
					if (first) {
						finalString = res; first = false;
					} else {
						finalString = finalString + "|" + res;
					}
				}
			}
			recvQ.enQ(finalString);
		}
	});
}

//----< factory functions >--------------------------------------------------

//----< create sender >-------------------------------------
ISendr* ObjectFactory::createSendr() { return new Sendr; }

//----< create receiver >-------------------------------------
IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

//----< create a client ui instance >-------------------------------------
IClient* ObjectFactory::createClient(ISendr* pISendr, IRecvr* pIRecvr) {
	return new ClientUI(pISendr, pIRecvr);
}

//----< Utility function to split the string >-------------------------------------
std::vector<std::string> ClientUI::getDetails(const std::string str, const char delim) {
	typedef std::string::const_iterator iter;
	iter beg = str.begin();
	std::vector<std::string> tokens;
	while (beg != str.end()) {
		iter temp = std::find(beg, str.end(), delim);
		if (beg != str.end())
			tokens.push_back(std::string(beg, temp));
		beg = temp;
		while ((beg != str.end()) && (*beg == delim))
			beg++;
	}
	return tokens;
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IClient* client = objFact.createClient(pSendr, pRecvr);
	client->start("localhost","9080","input/");
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
#endif
