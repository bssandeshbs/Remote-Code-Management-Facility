#include "Sender.h"

/////////////////////////////////////////////////////////////////////
//  Sender.cpp - Responsible for sending the header and data       //
//				 to Receiver									   //
//																   //
//  ver 2.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////

//----< performs the uploading of file from client to server >-------------------------------------
void Sender::clientFileHandling(Socket& si, std::vector<std::string> messageDetails,std::string path) 
{	const int BufLen = 1025; 
	char buffer[BufLen];
	std::string comPath = messageDetails[3];
	std::string fileName = comPath.substr(comPath.find_last_of("\\")+1);
	std::string pathName = comPath.substr(0, comPath.find_last_of("\\")+1);
	std::fstream fin(pathName + fileName, std::ios::in | std::ios::binary);
	buffer[BufLen - 1] = '\0';
	if (!fin) { // Show failure if not able to open file
		std::string fileError = "Unable to open file " + messageDetails[3] + " at the location " + messageDetails[4] + ":" + messageDetails[5];
		Display::showConfirmation(fileError);
		return;
	}
	else {
		Message message;
		std::string sendAddr = messageDetails[4] + ":" + messageDetails[5];
		std::string recvAddr = messageDetails[1] + ":" + messageDetails[2];
		message.setMessage(fileName.length(), "FILE_UPLOAD", sendAddr, recvAddr, fileName);
		si.send(message.getSize(), message.getMessage());
		char bufferdata[BufLen]; //let's say read by 1024 char block
		fin.seekg(0, fin.end);
		int totalSize = (unsigned int)fin.tellg(); //get the size of the file
		fin.seekg(0, fin.beg);
		bool flag = false;
		while (true) {
			int bSize = 1024;
			if (totalSize < 1024) {
				bSize = totalSize;
				char bufferdata2[1025];
				fin.read(bufferdata2, bSize);
				bufferdata2[BufLen - 1] = '\0';
				message.setHeader(bSize, "FILE_UPLOAD", sendAddr, recvAddr);
				si.send(120, message.getHeader());
				si.send(bSize, bufferdata2);
				flag = true;
			} else {
				totalSize = totalSize - 1024;
				fin.read(bufferdata, bSize);
				bufferdata[BufLen - 1] = '\0';
				message.setMessage(bSize, "FILE_UPLOAD", sendAddr, recvAddr, bufferdata);
				si.send(120, message.getHeader());
				si.send(bSize, bufferdata);
			}
			if (flag) {
				break;
			} } }
}

//----< Send a request to upload a File from server sender >-------------------------------------
void Sender::clientFileDownloadHandling(Socket& si, std::vector<std::string> messageDetails)
{
	Message message;
	std::string sendAddr = messageDetails[4] + ":" + messageDetails[5];
	std::string recvAddr = messageDetails[1] + ":" + messageDetails[2];
	message.setMessage(messageDetails[3].length(), "SERVER_FILE_UPLOAD_REQ", sendAddr, recvAddr, messageDetails[3]);
	si.send(message.getSize(), message.getMessage());
}

//----< Send a request to upload a File from server sender >-------------------------------------
void Sender::demoMessage(Socket& si, std::vector<std::string> messageDetails)
{
	Message message;
	std::string sendAddr = messageDetails[4] + ":" + messageDetails[5];
	std::string recvAddr = messageDetails[1] + ":" + messageDetails[2];
	message.setMessage(messageDetails[3].length(), "DEMO_HEADER_MESSAGE", sendAddr, recvAddr, messageDetails[3]);
	si.send(message.getSize(), message.getMessage());
}

//----< send text search query >-------------------------------------
void Sender::sendTextSearch(Socket& si, std::vector<std::string> messageDetails) {
	Message message;
	std::string sendAddr = messageDetails[4] + ":" + messageDetails[5];
	std::string recvAddr = messageDetails[1] + ":" + messageDetails[2];
	message.setMessage(messageDetails[3].length(), "TEXT_SEARCH", sendAddr, recvAddr, messageDetails[3]);
	si.send(message.getSize(), message.getMessage());
}

//----< send file search query >-------------------------------------
void Sender::sendFileSearch(Socket& si, std::vector<std::string> messageDetails) {
	Message message;
	std::string sendAddr = messageDetails[4] + ":" + messageDetails[5];
	std::string recvAddr = messageDetails[1] + ":" + messageDetails[2];
	message.setMessage(messageDetails[3].length(), "FILE_SEARCH", sendAddr, recvAddr, messageDetails[3]);
	si.send(message.getSize(), message.getMessage());
}

//----< provides a acknowledgmet after succesfully uploading >-------------------------------------
void Sender::clientAckowledgement(Socket& si, Message message)  {
	si.send(message.getSize(), message.getMessage());
}

//----< uploads the file from server sender to client(Downloading)>--------------------------------
void Sender::serverClientUpload(Socket& si, Message message,std::string path) {
	const int BufLen = 1025; //let's say read by 1024 char block
	char buffer[BufLen];
	std::fstream fin(path + message.getBody(), std::ios::in | std::ios::binary);
	buffer[BufLen - 1] = '\0';
	if (!fin) {
		si.send(message.getSize(), message.getMessage());
		message.setBody("File Not Found");
		si.sendString("FAILURE");
		return;
	} else {	
		std::string sendAddr = message.getSendAdd();
		std::string recvAddr = message.getRecvAdd();
		si.send(message.getSize(), message.getMessage());
		si.sendString("SUCCESS");

		char bufferdata[BufLen]; 
		fin.seekg(0, fin.end);
		int totalSize = (unsigned int)fin.tellg();
		fin.seekg(0, fin.beg);
		bool flag = false;
		while (true) {
			int bSize = 1024;
			if (totalSize < 1024) {   // Send last time if content length is less than 1024
				bSize = totalSize;
				char bufferdata2[1025];
				fin.read(bufferdata2, bSize);
				bufferdata2[BufLen - 1] = '\0';
				message.setHeader(bSize, "FILE_UPLOAD_SERVER", sendAddr, recvAddr);
				si.send(120, message.getHeader());
				si.send(bSize, bufferdata2);
				flag = true;
			} else {
				totalSize = totalSize - 1024; // Subtract from total length and send 1024 
				fin.read(bufferdata, bSize);
				bufferdata[BufLen - 1] = '\0'; // Sender header and then send buffer
 				message.setMessage(bSize, "FILE_UPLOAD_SERVER", sendAddr, recvAddr, bufferdata);
				si.send(120, message.getHeader());
				si.send(bSize, bufferdata);
			}
			if (flag) break;
		}
	}
}

//----< remove the delimiter and search string>-------------------------------------
void Sender::performSearch(Socket& si, Message message,bool searchType,std::string serverPath) {
		
	std::string inputString =	message.getBody();
	std::string searchStringInitial;
	std::string patternsInitial;

	std::string delimiter = "[$fL(^S*%S^$A$(R%FNP^$G]";
	size_t pos = 0;
	std::string token;
	
	int k = 0;

	while ((pos = inputString.find(delimiter)) != std::string::npos) {
		token = inputString.substr(0, pos);
		patternsInitial = token;
		k++;
		inputString.erase(0, pos + delimiter.length());
	}
	searchStringInitial = inputString;

	std::string removePattern = "**p&a*t)t(e%rn#s**:";
	std::string::size_type i = patternsInitial.find(removePattern);

	if (i != std::string::npos)
		patternsInitial.erase(i, removePattern.length());


	std::string removeSearchString = "**s&ea%rch#st^r)i(n(g**:";
	std::string::size_type j = searchStringInitial.find(removeSearchString);

	if (j != std::string::npos)
		searchStringInitial.erase(j, removeSearchString.length());

	std::vector<std::string> patterns = getDetails(patternsInitial, ',');

	if (searchType) performTextSearch(searchStringInitial, patterns, si, message, serverPath);
	else performFileSearch(searchStringInitial, patterns, si, message, serverPath);
}

//----< perform text search >-------------------------------------
void Sender::performTextSearch(std::string textString, std::vector<std::string> patterns, Socket& socket_,
	Message message, std::string serverPath) {

	std::vector<std::string> inputs;
	std::string patternString = "";
	int count = 0;

	inputs.push_back(serverPath);  //Set parameters
	inputs.push_back("/s");
	inputs.push_back("/d");
	std::string textStringNew = "/f" + textString;
	inputs.push_back(textStringNew);

	for (auto a : patterns) {
		inputs.push_back(a);
	}

	DataStore ds;
	FileManager fm(ds, inputs.size(), inputs);
	fm.processVectorInput();
	fm.search();
	Catalogue c(ds);

	if (std::find(fm.optionBegin(), fm.optionEnd(), "/f") != fm.optionEnd()) {
		if (fm.getSearchString() != "") { //If the option starts with /f and no search string
			c.searchCatalog(fm.getSearchString());
		}
	}
	std::set<std::string> inputSet = c.getFileSet();
	std::string finalString = "";
	for (auto s : inputSet) { //prepare result
		std::string sendAddr = message.getSendAdd() + "--" + s;
		finalString = finalString + sendAddr + "|";
	}
	sendSearchResults(finalString, socket_,message);
}

//----< perform file search >-------------------------------------
void Sender::performFileSearch(std::string path, std::vector<std::string> patterns, Socket& socket_,
	Message message, std::string serverPath) {

	std::vector<std::string> inputs;
	std::string patternString = "";
	int count = 0;

	inputs.push_back(serverPath+path);
	inputs.push_back("/s");

	for (auto a : patterns) {
		inputs.push_back(a);
	}

	DataStore ds;
	FileManager fm(ds, inputs.size(), inputs);
	fm.processVectorInput();
	fm.search();
	std::vector<std::string> inputSet = ds.getDataStore();
	std::string finalString = "";
	for (auto s : inputSet) {
		std::string sendAddr = message.getSendAdd() + "--" + s;

		finalString = finalString + sendAddr + "|";
	}

	sendFileSearchResults(finalString, socket_, message);
}

//----< send file search results to client >-------------------------------------
void Sender::sendFileSearchResults(std::string inputString, Socket& si, Message message) {
	int totalSize = inputString.size();
	const int BufLen = 1025;
	char bufferdata[BufLen];
	si.send(message.getSize(), message.getMessage());

	bool flag = false;
	while (true) {
		int bSize = 1024;
		if (totalSize < 1024) {   // Send last time if content length is less than 1024
			bSize = totalSize;
			char bufferdata2[1025];
			for (int i = 0; i < bSize; i++) {
				bufferdata2[i] = inputString[i];
			}
			bufferdata2[BufLen - 1] = '\0';
			message.setHeader(bSize, "FILE_SEARCH_RESULT", "", ""); //Send Add and Recv Address to be updated
			si.send(120, message.getHeader());
			si.send(bSize, bufferdata2);
			flag = true;
		}
		else {
			totalSize = totalSize - 1024; // Subtract from total length and send 1024 
			for (int i = 0; i < bSize; i++) {
				bufferdata[i] = inputString[i];
			}
			bufferdata[BufLen - 1] = '\0'; // Sender header and then send buffer
			message.setMessage(bSize, "FILE_SEARCH_RESULT", "", "", bufferdata);
			si.send(120, message.getHeader());
			si.send(bSize, bufferdata);
			inputString = inputString.substr(1024);
		}
		if (flag) break;
	}
}

//----< send text search results to client >-------------------------------------
void Sender::sendSearchResults(std::string inputString, Socket& si, Message message) {
	int totalSize = inputString.size();
	const int BufLen = 1025;
	char bufferdata[BufLen];
	si.send(message.getSize(), message.getMessage());

	bool flag = false;
	while (true) {
		int bSize = 1024;
		if (totalSize < 1024) {   // Send last time if content length is less than 1024
			bSize = totalSize;
			char bufferdata2[1025];
			for (int i = 0; i < bSize; i++) {
				bufferdata2[i] = inputString[i];
			}
			bufferdata2[BufLen - 1] = '\0';
			message.setHeader(bSize, "TEXT_SEARCH_RESULT", "", ""); //Send Add and Recv Address to be updated
			si.send(120, message.getHeader());
			si.send(bSize, bufferdata2);
			flag = true;
		}
		else {
			totalSize = totalSize - 1024; // Subtract from total length and send 1024 
			for (int i = 0; i < bSize; i++) {
				bufferdata[i] = inputString[i];
			}
			bufferdata[BufLen - 1] = '\0'; // Sender header and then send buffer
			message.setMessage(bSize, "TEXT_SEARCH_RESULT", "", "", bufferdata);
			si.send(120, message.getHeader());
			si.send(bSize, bufferdata);
			inputString = inputString.substr(1024);
		}
		if (flag) break;
	}
}

//----< Utility function to split the string >-------------------------------------
std::vector<std::string> Sender::getDetails(const std::string str, const char delim) {
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

#ifdef TEST_SENDER
//----< Test Stub to test the sender>--------------------------------
int main() {
	title("Message Passing Communication", '=');
	try
	{
		Sender s;
		Verbose v(true);
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 9080))
		{
			Verbose::show("client waiting to connect");
			::Sleep(100);
		}

		title("Starting string test on client");
		std::vector<std::string> messageDetails;
		messageDetails.push_back("upload");
		messageDetails.push_back("localhost");
		messageDetails.push_back("9080");
		messageDetails.push_back("Delay.pdf");
		messageDetails.push_back("localhost");
		messageDetails.push_back("9081");

		s.clientFileHandling(si, messageDetails, "input/");
		si.shutDownSend();
	}
	catch (std::exception& ex)
	{
		Verbose::show("Exception caught:", always);
		Verbose::show(std::string("\n  ") + ex.what() + "\n\n");
	}
}

#endif
