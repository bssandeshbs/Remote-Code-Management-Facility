#include "Client.h"
/////////////////////////////////////////////////////////////////////
//  Client.cpp - Enquees the upload request and spaws client and   //
//             and server threads                                  //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication			       //
//				   Remote Code Managemenet						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////

//----< wrapper to enquee an Item >-------------------------------------
void Client::enQuee(std::string operation, std::string address, int port, std::string fileName) {
	std::string res = operation + "|" + address + "|" + std::to_string(port) + "|" + fileName + "|";
	q2.enQ(res);
}

//----< wrapper to Dequee an Item >-------------------------------------
std::string Client::dequee() {
	return q2.deQ();
}

//----< Utility function to split the string >-------------------------------------
std::vector<std::string> Client::getDetails(const std::string str, const char delim) {
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

//----< utility function to read files >-------------------------------------
void Client::readFile(std::string path) {
	std::ifstream file(path);
	std::string str;
	while (std::getline(file, str))
	{
		std::vector<std::string> vec = getDetails(str, '|');
		if (vec.size() > 3) enQuee(vec[0], vec[1], stoi(vec[2]), vec[3]);
	}
}

//----< utility function to read line >-------------------------------------
void Client::parseline(std::string line) {
	std::vector<std::string> vec = getDetails(line, '|');
	if (vec.size() > 3) enQuee(vec[0], vec[1], stoi(vec[2]), vec[3]);
}

//----< utility function to connect >-------------------------------------
bool Client::connectadd(SocketConnecter& si, std::string address, int port) {
	int counter = 0;
	while (!si.connect(address, port)){
		Display::showConfirmation("client waiting to connect");
		::Sleep(100);
		counter++;
		if (counter == 2) {
			BlockingQueue<std::string> &q1 = QServer<std::string, 5>().get();
			std::string res = "Failed to connect to Server at " + address + ":" + toString(port);
			q1.enQ(res);
			isConnected = false;
			return false;
		}
	}
	isConnected = true;
	if (!isShutdown) {
		std::string value = address + ":" + toString(port);
		if (!(std::find(addressSet.begin(), addressSet.end(), value) != addressSet.end())) {
			addressSet.push_back(value);
		}
	}
	return true;
}

//----< start client receiver >-------------------------------------
void Client::startReceiver() {
	try
	{
		Display::titleDisplay("Client Receiver Thread Starting");
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);

		Receiver cp(clientPath);
		sl.start(cp);

		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Display::showConfirmation("Exception caught:");
		Display::showConfirmation(std::string("\n  ") + ex.what() + "\n\n");
	}
}
//----< function to connect and destroy server senders >------------------------
void Client::connect(SocketConnecter& si) {
	Message msg;
	if (addressSet.size() > 0) {

		std::vector<std::string> vec = getDetails(addressSet[0], ':');
		msg.setHeader(0, "STOP_RECEIVER_THREAD", serverAddress + ":" + std::to_string(this->port),
			vec[0] + ":" + vec[1]);

		if (connectadd(si, vec[0], stoi(vec[1]))) {
			si.send(120, msg.getHeader()); 	::Sleep(600); si.shutDownSend();
		}
	}
	if (addressSet.size() > 1) {

		Message msg1;
		std::vector<std::string> vec1 = getDetails(addressSet[1], ':');
		msg1.setHeader(0, "STOP_RECEIVER_THREAD", serverAddress + ":" + std::to_string(this->port),
			vec1[0] + ":" + vec1[1]);

		if (connectadd(si, vec1[0], stoi(vec1[1]))) {
			si.send(120, msg1.getHeader()); 	::Sleep(600); si.shutDownSend();
		}
	}
}

//----< Start Client Sender >-------------------------------------
void Client::startSender() {
	Sender s; Verbose v(true);
	SocketSystem ss; SocketConnecter si;
	int prevPort = 0; bool first = false;
	BlockingQueue<std::string> &q1 = QServer<std::string, 5>().get();
	while (true) { 
		std::string item = dequee(); 
		std::vector<std::string> vec = getDetails(item, '|');
		int portNumb = std::stoi(vec[2]);
		try { if (vec[0] == "shutdown") { //showdown client sender and server threads
				isShutdown = true;	connect(si);
				break; }
			else { 	if (vec[0] == "connect") { 	if (portNumb != prevPort) {
						if (!first) first = true; else { Message msg;
							msg.setHeader(0, "STOP_THREAD", serverAddress + ":" + std::to_string(this->port),
								vec[1] + ":" + vec[2]);
							si.send(120, msg.getHeader());
							::Sleep(600); si.shutDownSend();
						} if (connectadd(si, vec[1], portNumb)) {
							q1.enQ(" "); prevPort = portNumb;
						} else { prevPort = 0; first = false; }
					} else {
						q1.enQ("Already Connected to adddress " + vec[1] + " and port " + vec[2]);
					} } if (vec[0] == "upload") {
					if (isConnected) { vec.push_back(serverAddress); vec.push_back(std::to_string(this->port));
						s.clientFileHandling(si, vec, clientPath);
					} else q1.enQ("Please check the port and address and make a connection");
				} else if (vec[0] == "download") {
					vec.push_back(serverAddress); vec.push_back(std::to_string(this->port));
					s.clientFileDownloadHandling(si, vec);
				} else if (vec[0] == "disconnect") {
					if (isConnected) { Message msg;
						msg.setHeader(0, "STOP_THREAD", serverAddress + ":" + std::to_string(this->port),
							vec[1] + ":" + vec[2]);
						q1.enQ(" "); 
						si.send(120, msg.getHeader()); ::Sleep(600); si.shutDownSend();
						first = false; 	prevPort = 0; isConnected = false;
					} else { isConnected = false; q1.enQ(" "); }
				} else if (vec[0] == "searchText") {
					vec.push_back(serverAddress); vec.push_back(std::to_string(this->port));
					s.sendTextSearch(si, vec);
				} else if (vec[0] == "fileSearch") {
					vec.push_back(serverAddress); vec.push_back(std::to_string(this->port));
					s.sendFileSearch(si, vec);
				} } }
		catch (std::exception& ex){
			Display::showConfirmation(std::string("\n  ") + ex.what() + "\n\n"); } }
}

//----< Start receiver and sender threads >-------------------------------------
void Client::start() {
	std::thread receiver(&Client::startReceiver, this);
	std::thread sender(&Client::startSender, this);

	sender.detach();
	receiver.detach();
}

#ifdef CLIENT_1
//----< Entry point to the client which starts client sender and receiver >----------
int main(int argc, char* argv[]) {

	if (argc < 5){
		Display::showConfirmation("\n  Please provide all the fields like address,port and path and input files \n\n");
		return 0;
	}
	else {
		std::string address = argv[1];
		int port = std::stoi(argv[2]);
		std::string clientPath = argv[3];
		std::string inputFiles = argv[4];

		Display::titleDisplay("CLIENT DETAILS");
		std::string details = "\n Address:" + address + "\tPort:" + argv[2] +
			"\tPath:" + clientPath + "\n\n";

		Display::showConfirmation(details);

		Client client(address, port, clientPath);
		client.readFile(clientPath + inputFiles);
		std::thread receiver(&Client::startReceiver, client);

		std::thread sender(&Client::startSender, client);

		sender.join();
		receiver.join();
	}
}
#endif