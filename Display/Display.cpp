#include "Display.h"
/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display the string messages in formatted fashion //
//																   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Message Passing Communication                   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu		           //
/////////////////////////////////////////////////////////////////////

//----< display message>-------------------------------------
void Display::showConfirmation(std::string msg) {
		Verbose::show(msg,always);
}

//----< display underlined message>-------------------------------------
void Display::titleDisplay(std::string res) {
	title(res, '=');
}

//----< display demo header>-------------------------------------
void Display::displayDemoHeader(std::string res){
	Verbose::show("\nDemonstrating Requirement 4", always);
	Verbose::show(res, always);
	Verbose::show("\n", always);
}

//----< display message>-------------------------------------
void Display::showAckowledgement(std::string msg) {
	Verbose::show("\n----Acknowledgement Received----"+msg+"\n", always);
}

//----< display Instructions>-------------------------------------
void Display::displayInstructions() {
	Display::showConfirmation("\t\t\tMESSAGE PASSING COMMUNICATION ");
	Display::titleDisplay("\n Requirements 3,4,5,6 and 7 are demonstrated using 2 clients and 2 servers");
	Display::showConfirmation("\n  First Client 1 and Client 2 sends one file each to Server 1 parallely,");
	Display::showConfirmation("Client 1 then breaks connection with Server 1 and sends multiple files \n  to Server 2");
	Display::showConfirmation("\nThe below cases are covered in the above scenario ");
	Display::showConfirmation("\n  -> 2 Clients sending files to same server parallely ");
	Display::showConfirmation("-> 1 client sending files to one server, breaking connection and then sending files to other sever");
	Display::showConfirmation("\n\n Please refer to README file for more information");

}

#ifdef TEST_DISPLAY
//----< test stub>-------------------------------------
int main() {
	Display::displayInstructions();
	Display::showConfirmation("TEST DISPLAY");
	Display::titleDisplay("TEST TITLE");
}
#endif