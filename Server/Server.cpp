#include "Server.h"
/////////////////////////////////////////////////////////////////////
//  Server.cpp -   Accepts the request from client, processes      //
//				   the header and data, provides ack to client	   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//				   Remote Code Managemenet Facility				   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////

//----< Utility function to split the string >-------------------------------------
std::vector<std::string> Server::getDetails(const std::string str, const char delim) {
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

//----< start server receiver >-------------------------------------
void Server::startReceiver() {
	try
	{
		Display::titleDisplay("Server Receiver Starting");
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);

		Receiver receiver(serverPath);
		sl.start(receiver);

		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Display::showConfirmation("Exception caught:");
		Display::showConfirmation(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< start server sender >-------------------------------------
void Server::startSender() {
	Display::titleDisplay("Server Sender Starting");
	Sender s; Verbose v(true); 
	SocketSystem ss;SocketConnecter si;
	while (true) {
		std::string item = q1.deQ();
		Message msg;
		msg.setDetails(&(*item.begin()));
		std::string sendAddr = msg.getSendAdd();
		std::string recvAddr = msg.getRecvAdd();
		std::vector<std::string> vec =  getDetails(msg.getSendAdd(), ':');
		if (msg.getCommand() == "STOP_SERVER_SENDER") {
			::Sleep(800); si.shutDownSend(); break;
		}
		else {
			int port = std::stoi(vec[1]);
			while (!si.connect(vec[0], port)) {
				Verbose::show("Server Client waiting to connect");
				::Sleep(100);
			}
			if (msg.getCommand() == "FILE_UPLOAD") {
				 msg.setCommand("FILE_UPLOAD_CONFIRMATION");
				s.clientAckowledgement(si, msg);
			}
			else if (msg.getCommand() == "TEXT_SEARCH") {
				msg.setRecvAddr(sendAddr);
				msg.setSenderAddr(recvAddr);
				msg.setCommand("TEXT_SEARCH_SERVER");
				s.performSearch(si, msg, true, serverPath);
			}
			else if (msg.getCommand() == "FILE_SEARCH") {
				msg.setRecvAddr(sendAddr);
				msg.setSenderAddr(recvAddr);
				msg.setCommand("FILE_SEARCH_SERVER");
				s.performSearch(si, msg, false, serverPath);
			}
			else if (msg.getCommand() == "SERVER_FILE_UPLOAD_REQ") {
				 msg.setRecvAddr(sendAddr); 
				 msg.setSenderAddr(recvAddr);
				msg.setCommand("FILE_UPLOAD_SERVER");
				s.serverClientUpload(si, msg, serverPath);
			}
		}
	} 
}

#ifdef SERVER_1
//----< Entry point to the server which starts server sender and receiver >----------
int main(int argc, char* argv[]) {

	if (argc < 4){
		Display::showConfirmation("\n  Please provide all the fields like address,port and path \n\n");
		return 0;
	}
	else {
		std::string address = argv[1];
		int port = std::stoi(argv[2]);
		std::string serverPath = argv[3];
		
		Display::titleDisplay("SERVER DETAILS");
		std::string details = "\n Address:" + address + "\tPort:" + argv[2] +
			"\tPath:" + serverPath + "\n\n";

		Display::showConfirmation(details);

		Server server(address, port, serverPath);

		std::thread receiver(&Server::startReceiver, server);
		std::thread sender(&Server::startSender, server);

		sender.join();
		receiver.join();
	}
}
#endif