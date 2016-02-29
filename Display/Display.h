#ifndef DISPLAY_H
#define DISPLAY_H
/////////////////////////////////////////////////////////////////////
//  Display.h - Display the string messages in formatted fashion   //
//																   //
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
* This class provides serveral wrapper methods to display the header information
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Display.h Display.cpp
*
* Build Process:
* ==============
* cl /DTEST_DISPLAY Display.cpp ../ApplicationHelpers/AppHelpers.cpp
*
* Public Interface:
* =================
* displayDemoHeader(std::string res) - Display the header information 
* titleDisplay(std::string res) - Display the title
* showConfirmation(std::string result) - Display the contents using verbose
* displayInstructions(); - Display static information
* showAckowledgement(std::string msg) - shows the ackowledgment message
* Usage :
* ==================
* Display::showConfirmation("TEST DISPLAY");
* Display::titleDisplay("TEST TITLE");
*
* Maintenance History:
* ====================
* ver 1.0 : 05 Apr 14
* - first release
*/
#include <string>
#include <iostream>
#include "../ApplicationHelpers/AppHelpers.h"
using namespace ApplicationHelpers;

class Display {
public :
	static void showConfirmation(std::string result);
	static void titleDisplay(std::string res);
	static void displayDemoHeader(std::string result);
	static void showAckowledgement(std::string msg);
	static void displayInstructions();
};

#endif