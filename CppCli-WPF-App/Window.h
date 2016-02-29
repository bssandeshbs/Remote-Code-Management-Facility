#ifndef WINDOW_H
#define WINDOW_H
/////////////////////////////////////////////////////////////////////
// Windows.h - C++/CLI Implementation of WPF Application   	       //
//			 provides the functionality to display the screens     //
//  ver 3.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Remote Code Managemenent						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu				   //
//  original author : Jim Fawcett								   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package build a C++\CLI WPF application.  It 
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with four views, 
*  upload tab - provides the functionality to select a file using the browse dialogue and then 
*				upload the file and display the status information of upload
*  download tab - provides the functionality to download a file by connecting to the servers
*  search text - provides the functionality to search the text in server location containing
*                the search text
*  search file - provides the functioanlity to get list of files based on the category user selected
*
*  The Client can post a message to the MockChannel's send queue. The mockchannel then 
*  dequees the send queue and enquees to client queue. The client receiver finally
*  enquees the confirmation to the mockchannel receive queue 
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h 
*  Message.cpp Sender.h Sender.cpp Receiver.h Receiver.cpp Client.h Client.cpp Server.h Server.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
* *
* Public Interface:
* =================
*   	WPFCppCliDemo()  - Constructor
*					- Creates a object factory instance and invokes the createClient, passing the sender 
*					  and receiver instances and then creates the UI screen.
*
*		~WPFCppCliDemo() - Destructor 
*					- Destroys the client, sender and receiver objects
*        setUpTabControl() - Creates all the tabs required for the application 
*		 setUpStatusBar() - sets up the status bar
*		 setUploadstack() - creates the select server in upload screen
*        setUpFileListView() - creates the upload screen 
*		 setDownloadstack() - creates the select server in download screen 
*	     setUpDownloadView() -  creates the download screen 
*	     setTextSearchstack() - creates the select server in text search screen 
*	     setUpTextSearchView() - - creates the text search screen 
*		 setFileSearchstack() - creates the select server in file search screen 
*		 setUpFileSearchView() - - creates the file search screen 
* 
*		  Event listeners invoked on specific action in UI screen
*		  this->Loaded +=  gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
*		  this->Closing +=  gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
*		  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
*		  hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadFile);
*		  hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadFile);
*		  hSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchFile);
*		  hSearchButtonNew->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchText);
*
*  Maintenance History:
*  --------------------
*  ver 3.0 : 27 Apr 2015
*   - Added functionailities to display the upload, download, text search and file search screens
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/
using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>
#include <vector>
#include<set>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
	ISendr* pSendr1_;
	IRecvr* pRecvr2_;
	IClient* client_;
    // Controls for Window
    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hSendMessageTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
    TabItem^ hConnectTab = gcnew TabItem();
	TabItem^ hDownloadTab = gcnew TabItem();
	TabItem^ hFileSearchTab = gcnew TabItem();
	TabItem^ hTextSearchTab = gcnew TabItem();

    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

    // Controls for SendMessage View

    Grid^ hSendMessageGrid = gcnew Grid();
    Button^ hSendButton = gcnew Button();
    Button^ hClearButton = gcnew Button();
    TextBlock^ hTextBlock1 = gcnew TextBlock();
    ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
    StackPanel^ hStackPanel1 = gcnew StackPanel();

	//Controls for Connection View
	Grid^ connectionGrid = gcnew Grid();
	ScrollViewer^ connScrollViewer1 = gcnew ScrollViewer();
	Button^ hConnectButton = gcnew Button();
	Button^ hDisconnectButton = gcnew Button();
	StackPanel^ hConnectStackPanel1 = gcnew StackPanel();
	TextBox^ address = gcnew TextBox();
	TextBox^ port = gcnew TextBox();

	String^ addressString;
	String^ portString;

    // Controls for FileListView View

    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
	StackPanel^ hBrowseUploadStackPanel = gcnew StackPanel();
	ListBox^ hListBox = gcnew ListBox();
    Button^ hFolderBrowseButton = gcnew Button();
	Button^ hUploadButton = gcnew Button();
    Grid^ hGrid2 = gcnew Grid();

	// Controls for Download View

	Grid^ hDownloadFileGrid = gcnew Grid();
	Button^ hDownloadButton = gcnew Button();
	Button^ hClearFileButton = gcnew Button();
	ScrollViewer^ hScrollViewer3 = gcnew ScrollViewer();
	StackPanel^ hStackPanel3 = gcnew StackPanel();
	TextBox^ fileDownBox = gcnew TextBox();

	// Controls for FileSearch View

	Grid^ hTextSearchGrid = gcnew Grid();
	StackPanel^ hFileUploadStackPanel = gcnew StackPanel();
	StackPanel^ hButtonStackPanel = gcnew StackPanel();
	StackPanel^ hFinalButtonStackPanel = gcnew StackPanel();
	ListBox^ hTextListBox = gcnew ListBox();
	Button^ hSearchButton = gcnew Button();
	Grid^ hGrid3 = gcnew Grid();

	ComboBox^ comboBoxServer1 = gcnew ComboBox();
	TextBox^ hAddPatternTextBox = gcnew TextBox();

	// Controls for TextSearch View

	Grid^ hTextSearchGridNew = gcnew Grid();
	StackPanel^ hFileUploadStackPanelNew = gcnew StackPanel();
	StackPanel^ hButtonStackPanelNew = gcnew StackPanel();
	StackPanel^ hFinalButtonStackPanelNew = gcnew StackPanel();
	ListBox^ hTextListBoxNew = gcnew ListBox();
	Button^ hSearchButtonNew = gcnew Button();
	Grid^ hGrid3New = gcnew Grid();

	TextBox^ hAddTextTextBoxNew = gcnew TextBox();
	TextBox^ hAddPatternTextBoxNew = gcnew TextBox();

	// Controls for Server Stack for upload Screen

	StackPanel^ hStackPanel_U1 = gcnew StackPanel();
	StackPanel^ hStackPanel_U2 = gcnew StackPanel();
	CheckBox^ checkBox_U1 = gcnew CheckBox();
	CheckBox^ checkBox_U2 = gcnew CheckBox();
	TextBox^ htextboxAddress_U1 = gcnew TextBox();
	TextBox^ htextboxAddress_U2 = gcnew TextBox();
	TextBox^ htextboxPort_U1 = gcnew TextBox();
	TextBox^ htextboxPort_U2 = gcnew TextBox();

	// Controls for Server Stack for download Screen
	StackPanel^ hStackPanel_UD1 = gcnew StackPanel();
	StackPanel^ hStackPanel_UD2 = gcnew StackPanel();
	CheckBox^ checkBox_UD1 = gcnew CheckBox();
	CheckBox^ checkBox_UD2 = gcnew CheckBox();
	TextBox^ htextboxAddress_UD1 = gcnew TextBox();
	TextBox^ htextboxAddress_UD2 = gcnew TextBox();
	TextBox^ htextboxPort_UD1 = gcnew TextBox();
	TextBox^ htextboxPort_UD2 = gcnew TextBox();

	// Controls for Server Stack for text search Screen
	StackPanel^ hStackPanel_UT1 = gcnew StackPanel();
	StackPanel^ hStackPanel_UT2 = gcnew StackPanel();
	CheckBox^ checkBox_UT1 = gcnew CheckBox();
	CheckBox^ checkBox_UT2 = gcnew CheckBox();
	TextBox^ htextboxAddress_UT1 = gcnew TextBox();
	TextBox^ htextboxAddress_UT2 = gcnew TextBox();
	TextBox^ htextboxPort_UT1 = gcnew TextBox();
	TextBox^ htextboxPort_UT2 = gcnew TextBox();

	// Controls for Server Stack for file search Screen
	StackPanel^ hStackPanel_UF1 = gcnew StackPanel();
	StackPanel^ hStackPanel_UF2 = gcnew StackPanel();
	CheckBox^ checkBox_UF1 = gcnew CheckBox();
	CheckBox^ checkBox_UF2 = gcnew CheckBox();
	TextBox^ htextboxAddress_UF1 = gcnew TextBox();
	TextBox^ htextboxAddress_UF2 = gcnew TextBox();
	TextBox^ htextboxPort_UF1 = gcnew TextBox();
	TextBox^ htextboxPort_UF2 = gcnew TextBox();
    Thread^ recvThread;

  public:
	WPFCppCliDemo(String^ address,String ^ port,String^ path);
	~WPFCppCliDemo();

  private:
    std::string toStdString(String^ pStr);
	std::string preparePatternTextEnc(String^ patterns, String^ searchText);
	void downloadFileProcessing(std::string operation, bool first, bool second, String^ add1, String^ add2, String^ port1, String^ port2, String^ str, String ^status);
	void PerformTextFileProcessing(std::string operation, bool first, bool second, String^ add1, String^ add2, String^ port1, String^ port2, String^ str, String ^pattern, ListBox^ listbox, String^ status);
	String^ toSystemString(std::string& str);
	String^ displayTextFileProcessing(std::vector<std::string> files, String ^ status, ListBox^ listbox);
	void browseForFolder(Object^ sender, RoutedEventArgs^ args);
	void OnLoaded(Object^ sender, RoutedEventArgs^ args);
	void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	void uploadFile(Object^ sender, RoutedEventArgs^ args);;
	bool is_number(const std::string& s);
	void setUpDownloadView();
	void setDownProperties();
	void downloadFile(Object^ obj, RoutedEventArgs^ args);
	void searchText(Object^ sender, RoutedEventArgs^ args);
	void searchFile(Object^ sender, RoutedEventArgs^ args);
	std::string prepareInputString(std::string operation, String^ address, String^ port, String^ text);
	void clearDownText(Object^ obj, RoutedEventArgs^ args);
	void setUploadstack();
	void setDownloadstack();
	void setTextSearchstack();
	void setFileSearchstack();
	void setUpStatusBar();
	void setUpTabControl();
	void setUpFileListView();
	void setUpFileSearchView();
	void setUpTextSearchView();
	std::vector<std::string> getDetails(const std::string str, const char delim);
  };
}


#endif
