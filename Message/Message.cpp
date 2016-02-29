#include "Message.h"
/////////////////////////////////////////////////////////////////////
//  Message.cpp - Constructs the header before sending data		   //
//																   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////

//----< set message object properties>-------------------------------------
void Message::setMessage(int contentLength,std::string command, std::string sendAddr,
	std::string recvAddr, std::string body) {

	this->contentLength = "CONTENT_LENGTH:" + std::to_string(contentLength);
	this->command = "COMMAND:" +command;
	this->sendAddr = "SENDER_ADDRESS:" + sendAddr;
	this->recAddr = "RECEIVER_ADDRESS:"+recvAddr;
	this->body = body;
}

//----< set header>-------------------------------------
void Message::setHeader(int contentLength, std::string command, std::string sendAddr,
	std::string recvAddr) {

	this->contentLength = "CONTENT_LENGTH:"+std::to_string(contentLength);
	this->command = "COMMAND:"+command;
	this->sendAddr = "SENDER_ADDRESS:"+sendAddr;
	this->recAddr = "RECEIVER_ADDRESS:"+recvAddr;
}

//----< set Message Body>-------------------------------------
void Message::setBody(std::string bodyNew) {
	this->body = bodyNew;
}

//----< set Message details>-------------------------------------
char* Message::getMessage() {
	result = command + delimiter + sendAddr + delimiter + recAddr
		+ delimiter + contentLength + delimiter + delimiter;
	int size = result.length();
	while (size < 120) {
		result = result + " ";
		size++;
	}
	result = result + body;
	return &(*result.begin());
}

//----< set size>-------------------------------------
int Message::getSize() {
	return result.size();
}

//----< get body>-------------------------------------
std::string Message::getBody() {
	return body;
}

//----< get send addr>-------------------------------------
std::string Message::getSendAdd() {
	std::vector<std::string> ve = this->split(sendAddr, ':');
	return ve[1] + ":"+ve[2];
}

//----< get recv addr>-------------------------------------
std::string Message::getRecvAdd() {
	std::vector<std::string> ve = this->split(recAddr, ':');
	return ve[1]+":"+ve[2];
}

//----< get command>-------------------------------------
std::string Message::getCommand() {
	std::vector<std::string> ve = this->split(command, ':');
	return ve[1];
}

//----< get content length>-------------------------------------
int Message::getContentLength()  {
	std::vector<std::string> ve = this->split(contentLength, ':');
	int res = std::stoi(ve[1]);
	return res;
}

//----< get header>-------------------------------------
char* Message::getHeader() {
	result = command + delimiter + sendAddr + delimiter + recAddr
		+ delimiter + contentLength;
	int size = result.length();
	while (size < 120) {
		result = result + " ";
		size++;
	}
	return &(*result.begin());
}

//----< Utility function to split the string >-------------------------------------
std::vector<std::string> Message::split(const std::string str, const char delim) {
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

//----< set command>-------------------------------------
void Message::setCommand(std::string comm) {
	this->command = "COMMAND:" + comm;
}

//----< set send addr>-------------------------------------
void Message::setSenderAddr(std::string addr) {
	this->sendAddr = "SENDER_ADDRESS:"+addr;
}

//----< set recv addr>-------------------------------------
void Message::setRecvAddr(std::string addr) {
	this->recAddr = "RECEIVER_ADDRESS:"+addr;
}

//----< set details>-------------------------------------
void Message::setDetails(char * details) {
	size_t current;
	size_t next = std::string::npos;
	std::string input_string = details;
	bool body_started = false;
	int counter = 0;
	do {
		current = next + 1;
		next = input_string.find_first_of("\n\n", current);
		if (body_started) {
			std::string body = input_string.substr(120);
			this->body = body;
			break;
		}
		std::string sub = input_string.substr(current, next - current);
		if (sub != ""){
			if (counter == 0) this->command = sub;
			else if (counter == 1) this->sendAddr = sub;
			else if (counter == 2) this->recAddr = sub;
			else if (counter == 3) this->contentLength = sub;
			counter++;
		} else body_started = true;
	} while (next != std::string::npos);
}

#ifdef TEST_MESSAGE
//----< TEST STUB>-------------------------------------
int main() {
	Message m;
	std::string body1 = "Thiasddd\nddds iasdasdasdasds\n a boasdasdadsas\nasdasddasddy";
	m.setMessage(20, "TEST_SENDFILE", "localhost:8090", "localhost:8080", body1);
	char* res = m.getMessage();
	std::cout << "My Message :" << res;
	m.setDetails(res);
	std::string body = m.getBody();
	std::string sendAddr = m.getSendAdd();
	std::string recvAddr = m.getRecvAdd();
	std::string command = m.getCommand();
	int contextLength = m.getContentLength();

	std::string header = m.getHeader();

	std::cout << "Send Addr is :" << sendAddr<<"\n";
	std::cout << "Command is :" << command << "\n";
	std::cout << "Recv Addr is :" << recvAddr<<"\n";
	std::cout << "Content length :" << contextLength<<"\n\n";
	std::cout << header << "\n";
}
#endif