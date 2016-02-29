/////////////////////////////////////////////////////////////////////
// Windows.cpp - C++/CLI Implementation of WPF Application   	   //
//																   //
//  ver 3.0                                                        //
//  Language:      Visual C++ 2013, Update 2                       //
//  Platform:      ASUS TP300L, Windows 8.1						   //
//  Application:   Remote Code Managemenent						   //
//  Author:        Sandesh Bellur Shashidhara, Syracuse University //
//                 (315) 751-4826, sbellurs@syr.edu				   //
//  original author : Jim Fawcett								   //
/////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;
#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

//----< constructor >-------------------------------------
WPFCppCliDemo::WPFCppCliDemo(String^ address,String^ port,String^ path)
{
  // set up client
  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr1_ = pObjFact->createSendr();
  pRecvr2_ = pObjFact->createRecvr();
  client_ = pObjFact->createClient(pSendr1_, pRecvr2_);
  client_->start(toStdString(address), toStdString(port), toStdString(path));
  delete pObjFact;
  // set event handlers
  this->Loaded +=  gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing +=  gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadFile);
  hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadFile);
  hSearchButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchFile);
  hSearchButtonNew->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::searchText);
  hClearFileButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clearDownText);
  this->Title = "Remote Code Management Facility";
  this->Width = 800;
  this->Height = 600;
  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);
  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  //File List Stack
  setUploadstack();
  setUpFileListView();
  //Download stack
  setDownloadstack();
  setUpDownloadView();
  //Text Search
  setTextSearchstack();
  setUpTextSearchView();
  //File Search
  setFileSearchstack();
  setUpFileSearchView();
}

//----< destructor >-------------------------------------
WPFCppCliDemo::~WPFCppCliDemo()
{
  delete client_;
  delete pSendr1_;
  delete pRecvr2_;
}

//----< check if string is number >-------------------------------------
bool WPFCppCliDemo::is_number(const std::string& s)
{
	for (size_t i = 0; i < s.length(); i++) {
		if (!std::isdigit(s[i]))
			return false;
	}

	return true;
}

//----< setupTab status Bar >-------------------------------------
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "Hello User! Welcome to Remote Code Management Application";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//----< setupTab Control >-------------------------------------
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hFileListTab->Header = "Upload File";
  hDownloadTab->Header = "Download File";
  hFileSearchTab->Header = "File Search";
  hTextSearchTab->Header = "Text Search";

  hTabControl->Items->Add(hFileListTab);
  hTabControl->Items->Add(hDownloadTab);
  hTabControl->Items->Add(hFileSearchTab);
  hTabControl->Items->Add(hTextSearchTab);
}

//----< cpnvert to standard string >-------------------------------------
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

//----< function to upload file on button click >-------------------------------------
void WPFCppCliDemo::uploadFile(Object^ obj, RoutedEventArgs^ args)
{
	String^ str = (String^)hListBox->SelectedValue;
	bool checkBox1 = (bool)checkBox_U1->IsChecked; bool checkBox2 = (bool)checkBox_U2->IsChecked;
	bool first = false; bool second = false;
	String ^status = "";
	if (checkBox1 || checkBox2) {
		if (checkBox1) {
			if (!(htextboxAddress_U1->Text->IsNullOrEmpty(htextboxAddress_U1->Text) || 
				htextboxPort_U1->Text->IsNullOrEmpty(htextboxPort_U1->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_U1->Text))))  {
				first = true;
			} else {
				first = false;
				status = status + " Please enter valid server 1 details / Select a file to upload ";
			} }
		if (checkBox2) {
			if (!(htextboxAddress_U2->Text->IsNullOrEmpty(htextboxAddress_U2->Text) ||
				htextboxPort_U2->Text->IsNullOrEmpty(htextboxPort_U2->Text) || str->IsNullOrEmpty(str) || 
				!is_number(toStdString(htextboxPort_U2->Text))))   {
				second = true;
			} else {
				status = status+ " Please enter valid server 2 details / Select a file to upload";
				second = false;
			} } } 
	else {
		status = status +" Please select either checkbox to perform upload";
		second = false; first = false;
	} 

	downloadFileProcessing("upload", first, second, htextboxAddress_U1->Text, htextboxAddress_U2->Text,
		htextboxPort_U1->Text, htextboxPort_U2->Text, str,status);
}

//----< functions to prepare Input String >-------------------------------------
std::string WPFCppCliDemo::prepareInputString(std::string operation, String^ address, String^ port, String^ text) {
	std::string connectDetails = "connect|" + toStdString(address) + "|" + toStdString(port) + "|NOFILE";
	std::string addressDetails = operation+ "|" + toStdString(address) + "|" + toStdString(port) + "|" + toStdString(text);
	std::string disconnect = "disconnect|" + toStdString(address) + "|" + toStdString(port) + "|NOFILE";
	std::string result = connectDetails + "?" + addressDetails + "?" + disconnect;
	return result;
}

//----< display text processing  >-------------------------------------
String^ WPFCppCliDemo::displayTextFileProcessing(std::vector<std::string> files, String ^ status, ListBox^ listbox) {
	for (auto a : files) {
		std::string file = a;
		if (file.substr(0, 6) == "Failed") {
			status = status + "\n" + toSystemString(file);
			continue;
		}
		if (a != " ") listbox->Items->Add(toSystemString(a));
	}
	return status;
}

//----< display text and file processing >-------------------------------------
void WPFCppCliDemo::PerformTextFileProcessing(std::string operation, bool first, bool second, String^ add1, String^ add2, String^ port1, String^ port2, String^ str,String ^pattern, ListBox^ listbox,String^ status) {
	if (first&&second){
		std::string finalString = preparePatternTextEnc(pattern, str);
		std::string result0 = prepareInputString(operation, add1, port1, toSystemString(finalString));
		std::string result1 = prepareInputString(operation, add2, port2, toSystemString(finalString));
		hStatus->Text = "Connecting !!!";
		pSendr1_->postMessage(result0 + "?" + result1);
		std::string messages = pRecvr2_->getMessage();
		std::vector<std::string> files = getDetails(messages, '|');
		status = displayTextFileProcessing(files, status, listbox);
		hStatus->Text = status + "\n" + " If connection is Succesfull, Results are displayed on the screen ";
	}
	else if (first) {
		std::string finalString = preparePatternTextEnc(pattern, str);
		std::string result0 = prepareInputString(operation, add1, port1, toSystemString(finalString));
		hStatus->Text = "Connecting !!!";
		pSendr1_->postMessage(result0);
		std::string messages = pRecvr2_->getMessage();
		std::vector<std::string> files = getDetails(messages, '|');
		status = displayTextFileProcessing(files, status, listbox);
		hStatus->Text = status + "\n" + " If connection is Succesfull, Results are displayed on the screen ";
	}
	else if (second){
		std::string finalString = preparePatternTextEnc(pattern, str);
		std::string result1 = prepareInputString(operation, add2, port2, toSystemString(finalString));
		hStatus->Text = "Connecting !!!";
		pSendr1_->postMessage(result1);
		std::string messages = pRecvr2_->getMessage();
		std::vector<std::string> files = getDetails(messages, '|');
		status = displayTextFileProcessing(files, status, listbox);
		hStatus->Text = status + "\n" + " If connection is Succesfull, Results are displayed on the screen ";

	}
	else {
		hStatus->Text = status;
	}
}
//----< clear download text screen >-------------------------------------
void WPFCppCliDemo::clearDownText(Object^ obj, RoutedEventArgs^ args) {
	fileDownBox->Clear();
}


//----< download file screen >-------------------------------------
void WPFCppCliDemo::downloadFile(Object^ obj, RoutedEventArgs^ args) {

	String^ str = fileDownBox->Text;
	String^ status;
	bool checkBox1 = (bool)checkBox_UD1->IsChecked; bool checkBox2 = (bool)checkBox_UD2->IsChecked;
	bool first = false; bool second = false;
	if (checkBox1 || checkBox2) {
		if (checkBox1) {
			htextboxPort_UD1->Text; //validate input
			if (!(htextboxAddress_UD1->Text->IsNullOrEmpty(htextboxAddress_UD1->Text) ||
				htextboxPort_UD1->Text->IsNullOrEmpty(htextboxPort_UD1->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UD1->Text))))  {
				first = true;
			}
			else {
				first = false;
				status = status + " Please enter valid server 1 details or provide the file name to download";
			}
		}
		if (checkBox2) {
			if (!(htextboxAddress_UD2->Text->IsNullOrEmpty(htextboxAddress_UD2->Text) ||
				htextboxPort_UD2->Text->IsNullOrEmpty(htextboxPort_UD2->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UD2->Text))))   {
				second = true;
			}
			else {
				status = status + " Please enter valid server 2 details or provide the file name to download";
				second = false;
			}
		}
	}
	else {
		status = "Please select either checkbox to perform upload";
		second = false; first = false;
	} 
	downloadFileProcessing("download",first, second, htextboxAddress_UD1->Text, htextboxAddress_UD2->Text,
		htextboxPort_UD1->Text, htextboxPort_UD2->Text, str, status);
}

//----< invoke download function upon validation >-------------------------------------
void WPFCppCliDemo::downloadFileProcessing(std::string operation,bool first,bool second,String^ add1,String^ add2,String^ port1,String^ port2,String^ str,String^ status) {
	if (first&&second){
		std::string result = prepareInputString(operation, add1, port1, str);
		std::string result1 = prepareInputString(operation, add2, port2, str);
		hStatus->Text = "Connecting to Servers !!! ";
		pSendr1_->postMessage(result + "?" + result1);
		std::string delStr = pRecvr2_->getMessage();
		std::string resultFinal;
	//	std::remove_copy(delStr.begin(), delStr.end(), std::back_inserter(resultFinal), '|');
		hStatus->Text = status + "\n" + toSystemString(resultFinal);
	}
	else if (first) {
		std::string result = prepareInputString(operation, add1, port1, str);
		hStatus->Text = "Connecting to Servers !!!";
		pSendr1_->postMessage(result);
		std::string delStr = pRecvr2_->getMessage();
		std::string resultFinal;
	//	std::remove_copy(delStr.begin(), delStr.end(), std::back_inserter(resultFinal), '|');
		hStatus->Text = status + "\n" + toSystemString(resultFinal);
	}
	else if (second){
		std::string result1 = prepareInputString(operation, add2, port2, str);
		hStatus->Text = "Connecting to Servers !!!";
		pSendr1_->postMessage(result1);
		std::string delStr = pRecvr2_->getMessage();
		std::string resultFinal;
	//	std::remove_copy(delStr.begin(), delStr.end(), std::back_inserter(resultFinal), '|');
		hStatus->Text = status + "\n" + toSystemString(resultFinal);
	}
	else {
		hStatus->Text = status;
	}
}

//----< convert to System string >-------------------------------------
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}


//----< Utility function to split the string >-------------------------------------
std::vector<std::string> WPFCppCliDemo::getDetails(const std::string str, const char delim) {
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

//----< search text UI validation screen >-------------------------------------
void WPFCppCliDemo::searchText(Object^ sender, RoutedEventArgs^ args) {
	String^ str = hAddTextTextBoxNew->Text;
	hTextListBoxNew->Items->Clear();
	bool checkBox1 = (bool)checkBox_UT1->IsChecked; bool checkBox2 = (bool)checkBox_UT2->IsChecked;
	bool first = false; bool second = false;
	String^ status;
	if (checkBox1 || checkBox2) {
		if (checkBox1) {
			htextboxPort_UT1->Text; // validate text search screen
			if (!(htextboxAddress_UT1->Text->IsNullOrEmpty(htextboxAddress_UT1->Text) ||
				htextboxPort_UT1->Text->IsNullOrEmpty(htextboxPort_UT1->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UT1->Text))))  {
				first = true;
			}
			else {
				first = false;
				status = status +" Please enter valid server 1 details or specify the search text to search in server ";
			}
		}
		if (checkBox2) {
			if (!(htextboxAddress_UT2->Text->IsNullOrEmpty(htextboxAddress_UT2->Text) ||
				htextboxPort_UT2->Text->IsNullOrEmpty(htextboxPort_UT2->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UT2->Text))))   {
				second = true;
			}
			else {
				status = status + " Please enter valid server 2 details or specify the search text to search in server";
				second = false;
			}
		}
	}
	else {
		status = "Please select either checkbox to perform upload";
		second = false; first = false;
	} 
	
	PerformTextFileProcessing("searchText", first, second, htextboxAddress_UT1->Text, htextboxAddress_UT2->Text, htextboxPort_UT1->Text, htextboxPort_UT2->Text, str, hAddPatternTextBoxNew->Text, hTextListBoxNew, status);
}

//----< search file validation screen>-------------------------------------
void WPFCppCliDemo::searchFile(Object^ sender, RoutedEventArgs^ args) {
	String^ str =	(String^)comboBoxServer1->SelectedItem;
	hTextListBox->Items->Clear();
	bool checkBox1 = (bool)checkBox_UF1->IsChecked; bool checkBox2 = (bool)checkBox_UF2->IsChecked;
	bool first = false; bool second = false;
	String^ status;
	if (checkBox1 || checkBox2) {
		if (checkBox1) {
			htextboxPort_UF1->Text; //validate UI components 
			if (!(htextboxAddress_UF1->Text->IsNullOrEmpty(htextboxAddress_UF1->Text) ||
				htextboxPort_UF1->Text->IsNullOrEmpty(htextboxPort_UF1->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UF1->Text))))  {
				first = true;
			}
			else {
				first = false;
				status = status + " Please enter valid server 1 details ";
			}
		}
		if (checkBox2) {
			if (!(htextboxAddress_UF2->Text->IsNullOrEmpty(htextboxAddress_UF2->Text) ||
				htextboxPort_UF2->Text->IsNullOrEmpty(htextboxPort_UF2->Text) || str->IsNullOrEmpty(str)
				|| !is_number(toStdString(htextboxPort_UF2->Text))))   {
				second = true;
			}
			else {
				status = status + " Please enter valid server 2 details ";
				second = false;
			}
		}
	}
	else {
		status = " Please select either checkbox to perform upload";
		second = false; first = false;
	}

	PerformTextFileProcessing("fileSearch", first, second, htextboxAddress_UF1->Text, htextboxAddress_UF2->Text, htextboxPort_UF1->Text, htextboxPort_UF2->Text, str, hAddPatternTextBox->Text, hTextListBox, status);
}

//----< setup file list view >-------------------------------------
void WPFCppCliDemo::setUpFileListView()
{
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(75);
  hFileListGrid->RowDefinitions->Add(hRow2Def);
  hFolderBrowseButton->Content = "Select Directory";
 
  Border^ hBorder2 = gcnew Border();
  hBorder2->Width = 120; hBorder2->Height = 30;
  hBorder2->BorderThickness = Thickness(1);
  hBorder2->BorderBrush = Brushes::Black;

  hBorder2->Child = hFolderBrowseButton;
  hBrowseUploadStackPanel->Children->Add(hBorder2);
  hUploadButton->Content = "Upload File";
  Border^ hBorder3 = gcnew Border();
  hBorder3->Width = 120;
  hBorder3->Height = 30;
  hBorder3->BorderThickness = Thickness(1);
  hBorder3->BorderBrush = Brushes::Black;
  hBorder3->Child = hUploadButton;
  TextBlock^ hSpacer = gcnew TextBlock();
  hSpacer->Width = 10;
  hBrowseUploadStackPanel->Children->Add(hSpacer);
  hBrowseUploadStackPanel->Children->Add(hBorder3);

  hBrowseUploadStackPanel->Orientation = Orientation::Horizontal;
  hBrowseUploadStackPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
  hFileListGrid->SetRow(hBrowseUploadStackPanel, 2);
  hFileListGrid->Children->Add(hBrowseUploadStackPanel);
  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();

  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hRow1Def->Height = GridLength(200);
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);
  hBorder1->BorderBrush = Brushes::Black;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 3);
  hFileListGrid->Children->Add(hBorder1);
}

//----< set up file search view >-------------------------------------
void WPFCppCliDemo::setUpFileSearchView() {
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	hTextSearchGrid->RowDefinitions->Add(hRow2Def);
	Label^ addLabel = gcnew Label();
	addLabel->Content = L"Specify Path/Directory\nrelative to server path";
	addLabel->Height = 50; addLabel->Width = 150;
	comboBoxServer1->Height = 20; comboBoxServer1->Width = 90;
	comboBoxServer1->Items->Add("C#"); comboBoxServer1->Items->Add("C++");
	comboBoxServer1->Items->Add("Java"); comboBoxServer1->SelectedIndex = 1;
	comboBoxServer1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hFileUploadStackPanel->Children->Add(addLabel);
	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 10; hFileUploadStackPanel->Children->Add(comboBoxServer1);
	hFileUploadStackPanel->Children->Add(hSpacer1);
	hTextSearchGrid->SetRow(hFileUploadStackPanel, 2);
	hTextSearchGrid->Children->Add(hFileUploadStackPanel);
	hFileUploadStackPanel->Orientation = Orientation::Horizontal;
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50); hTextSearchGrid->RowDefinitions->Add(hRow3Def);
	Label^ addPaternsLabel = gcnew Label();
	addPaternsLabel->Content = L"Specify file patterns \nseperated by comma ,";
	addPaternsLabel->Height = 50;addPaternsLabel->Width = 150;
	hAddPatternTextBox->Height = 25; hAddPatternTextBox->Width = 200;
	hButtonStackPanel->Children->Add(addPaternsLabel);
	hButtonStackPanel->Children->Add(hAddPatternTextBox);
	hButtonStackPanel->Orientation = Orientation::Horizontal;
	hTextSearchGrid->SetRow(hButtonStackPanel, 3);
	hTextSearchGrid->Children->Add(hButtonStackPanel);
	hSearchButton->Content = "Search Files";
	Border^ hBorder7 = gcnew Border(); hBorder7->Width = 125; hBorder7->Height = 30;
	hBorder7->BorderThickness = Thickness(1); hBorder7->BorderBrush = Brushes::Black;
	hBorder7->Child = hSearchButton; hTextSearchGrid->Margin = Thickness(20);
	RowDefinition^ hRow5Def = gcnew RowDefinition();
	hRow5Def->Height = GridLength(50); hTextSearchGrid->RowDefinitions->Add(hRow5Def);
	hTextSearchGrid->SetRow(hBorder7, 4); hTextSearchGrid->Children->Add(hBorder7);
	hFileSearchTab->Content = hTextSearchGrid; Label^ addLabel4 = gcnew Label();
	addLabel4->Content = L"Results are displayed in the below grid after search";
	addLabel4->Height = 35; addLabel->Width = 170;
	RowDefinition^ hRow8Def = gcnew RowDefinition();
	hRow8Def->Height = GridLength(30); hTextSearchGrid->RowDefinitions->Add(hRow8Def);
	hTextSearchGrid->SetRow(addLabel4, 5); hTextSearchGrid->Children->Add(addLabel4);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(200);
	hTextSearchGrid->RowDefinitions->Add(hRow1Def);
	hTextSearchGrid->SetRow(hTextListBox, 6);
	hTextSearchGrid->Children->Add(hTextListBox);
}

//----< setup upload stack screen >-------------------------------------
void WPFCppCliDemo::setUploadstack()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(30);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	checkBox_U1->Width = 20; checkBox_U1->Height = 20;
	hStackPanel_U1->Children->Add(checkBox_U1);
	TextBlock^ Srver_no = gcnew TextBlock();
	Srver_no->Width = 100; Srver_no->Height = 20;
	Srver_no->Text = "Server Address 1:";
	hStackPanel_U1->Children->Add(Srver_no);
	checkBox_U2->Width = 20; checkBox_U2->Height = 20;
	hStackPanel_U2->Children->Add(checkBox_U2);
	TextBlock^ Srver_no2 = gcnew TextBlock();
	Srver_no2->Width = 100; Srver_no2->Height = 20;
	Srver_no2->Text = "Server Address 2 :";
	hStackPanel_U2->Children->Add(Srver_no2); htextboxAddress_U1->FontSize = 12;
	htextboxAddress_U1->Height = 20; htextboxAddress_U1->Width = 200;
	hStackPanel_U1->Children->Add(htextboxAddress_U1);
	TextBlock^ hSpacer = gcnew TextBlock(); hSpacer->Width = 10; 
	hStackPanel_U1->Children->Add(hSpacer);
	TextBlock^ portNo = gcnew TextBlock();
	portNo->Width = 100; portNo->Height = 20; portNo->Text = "Port 1:";
	hStackPanel_U1->Children->Add(portNo);
	htextboxPort_U1->FontSize = 12;
	htextboxPort_U1->Height = 20; htextboxPort_U1->Width = 200;
	hStackPanel_U1->Children->Add(htextboxPort_U1);
	htextboxAddress_U2->FontSize = 12;
	htextboxAddress_U2->Height = 20; htextboxAddress_U2->Width = 200;
	hStackPanel_U2->Children->Add(htextboxAddress_U2);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10; hStackPanel_U2->Children->Add(hSpacer2);
	TextBlock^ port_No1 = gcnew TextBlock();
	port_No1->Width = 100; port_No1->Height = 20;
	port_No1->Text = "Port 2:";
	hStackPanel_U2->Children->Add(port_No1);
	htextboxPort_U2->FontSize = 12; htextboxPort_U2->Height = 20;
	htextboxPort_U2->Width = 200; hStackPanel_U2->Children->Add(htextboxPort_U2);
	hStackPanel_U1->Orientation = Orientation::Horizontal;
	hStackPanel_U1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel_U2->Orientation = Orientation::Horizontal;
	hStackPanel_U2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileListGrid->SetRow(hStackPanel_U1, 0); hFileListGrid->Children->Add(hStackPanel_U1);
	hFileListGrid->SetRow(hStackPanel_U2, 1); hFileListGrid->Children->Add(hStackPanel_U2);
}

//----< setup text search screen with servers >-------------------------------------
void WPFCppCliDemo::setTextSearchstack() {
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hTextSearchGridNew->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(30);
	hTextSearchGridNew->RowDefinitions->Add(hRow2Def);
	checkBox_UT1->Width = 20;checkBox_UT1->Height = 20;
	hStackPanel_UT1->Children->Add(checkBox_UT1);
	TextBlock^ Srver_no = gcnew TextBlock();
	Srver_no->Width = 100;Srver_no->Height = 20;
	Srver_no->Text = "Server Address 1:";
	hStackPanel_UT1->Children->Add(Srver_no);
	checkBox_UT2->Width = 20; checkBox_UT2->Height = 20;
	hStackPanel_UT2->Children->Add(checkBox_UT2);
	TextBlock^ Srver_no2 = gcnew TextBlock();
	Srver_no2->Width = 100; Srver_no2->Height = 20;
	Srver_no2->Text = "Server Address 2 :";
	hStackPanel_UT2->Children->Add(Srver_no2);
	htextboxAddress_UT1->FontSize = 12;
	htextboxAddress_UT1->Height = 20; htextboxAddress_UT1->Width = 200;
	hStackPanel_UT1->Children->Add(htextboxAddress_UT1);
	TextBlock^ hSpacer = gcnew TextBlock(); hSpacer->Width = 10;
	hStackPanel_UT1->Children->Add(hSpacer);
	TextBlock^ portNo = gcnew TextBlock();
	portNo->Width = 100; portNo->Height = 20;
	portNo->Text = "Port 1:";
	hStackPanel_UT1->Children->Add(portNo);
	htextboxPort_UT1->FontSize = 12;
	htextboxPort_UT1->Height = 20; htextboxPort_UT1->Width = 200;
	hStackPanel_UT1->Children->Add(htextboxPort_UT1);
	htextboxAddress_UT2->FontSize = 12;
	htextboxAddress_UT2->Height = 20; htextboxAddress_UT2->Width = 200;
	hStackPanel_UT2->Children->Add(htextboxAddress_UT2);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;hStackPanel_UT2->Children->Add(hSpacer2);
	TextBlock^ port_No1 = gcnew TextBlock();
	port_No1->Width = 100; port_No1->Height = 20; port_No1->Text = "Port 2:";
	hStackPanel_UT2->Children->Add(port_No1);
	htextboxPort_UT2->FontSize = 12;
	htextboxPort_UT2->Height = 20; htextboxPort_UT2->Width = 200;
	hStackPanel_UT2->Children->Add(htextboxPort_UT2);
	hStackPanel_UT1->Orientation = Orientation::Horizontal;
	hStackPanel_UT1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel_UT2->Orientation = Orientation::Horizontal;
	hStackPanel_UT2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSearchGridNew->SetRow(hStackPanel_UT1, 0); hTextSearchGridNew->Children->Add(hStackPanel_UT1);
	hTextSearchGridNew->SetRow(hStackPanel_UT2, 1); hTextSearchGridNew->Children->Add(hStackPanel_UT2);
}

//----< setup file search screen >-------------------------------------
void WPFCppCliDemo::setFileSearchstack() {	
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hTextSearchGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(30);
	hTextSearchGrid->RowDefinitions->Add(hRow2Def);
	checkBox_UF1->Width = 20; checkBox_UF1->Height = 20;
	hStackPanel_UF1->Children->Add(checkBox_UF1);
	TextBlock^ Srver_no = gcnew TextBlock();
	Srver_no->Width = 100; Srver_no->Height = 20;
	Srver_no->Text = "Server Address 1:";
	hStackPanel_UF1->Children->Add(Srver_no);
	checkBox_UF2->Width = 20; checkBox_UF2->Height = 20;
	hStackPanel_UF2->Children->Add(checkBox_UF2);
	TextBlock^ Srver_no2 = gcnew TextBlock();
	Srver_no2->Width = 100; Srver_no2->Height = 20;
	Srver_no2->Text = "Server Address 2 :";
	hStackPanel_UF2->Children->Add(Srver_no2);
	htextboxAddress_UF1->FontSize = 12;
	htextboxAddress_UF1->Height = 20; htextboxAddress_UF1->Width = 200;
	hStackPanel_UF1->Children->Add(htextboxAddress_UF1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10; hStackPanel_UF1->Children->Add(hSpacer);
	TextBlock^ portNo = gcnew TextBlock();
	portNo->Width = 100; portNo->Height = 20;
	portNo->Text = "Port :"; hStackPanel_UF1->Children->Add(portNo);
	htextboxPort_UF1->FontSize = 12;
	htextboxPort_UF1->Height = 20;htextboxPort_UF1->Width = 200;
	hStackPanel_UF1->Children->Add(htextboxPort_UF1);
	htextboxAddress_UF2->FontSize = 12; htextboxAddress_UF2->Height = 20;
	htextboxAddress_UF2->Width = 200; hStackPanel_UF2->Children->Add(htextboxAddress_UF2);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10;  hStackPanel_UF2->Children->Add(hSpacer2);
	TextBlock^ port_No1 = gcnew TextBlock();
	port_No1->Width = 100; 	port_No1->Height = 20;
	port_No1->Text = "Port 2:";
	hStackPanel_UF2->Children->Add(port_No1);
	htextboxPort_UF2->FontSize = 12;
	htextboxPort_UF2->Height = 20; htextboxPort_UF2->Width = 200;
	hStackPanel_UF2->Children->Add(htextboxPort_UF2);
	hStackPanel_UF1->Orientation = Orientation::Horizontal;
	hStackPanel_UF1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel_UF2->Orientation = Orientation::Horizontal;
	hStackPanel_UF2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSearchGrid->SetRow(hStackPanel_UF1, 0); hTextSearchGrid->Children->Add(hStackPanel_UF1);
	hTextSearchGrid->SetRow(hStackPanel_UF2, 1); hTextSearchGrid->Children->Add(hStackPanel_UF2);
}

//----< setup download stack setup screen >-------------------------------------
void WPFCppCliDemo::setDownloadstack() {
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(30);
	hDownloadFileGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(30);
	hDownloadFileGrid->RowDefinitions->Add(hRow2Def);
	checkBox_UD1->Width = 20; checkBox_UD1->Height = 20;
	hStackPanel_UD1->Children->Add(checkBox_UD1);
	TextBlock^ Srver_no = gcnew TextBlock();
	Srver_no->Width = 100; Srver_no->Height = 20;
	Srver_no->Text = "Server Address 1:";
	hStackPanel_UD1->Children->Add(Srver_no);
	checkBox_UD2->Width = 20; checkBox_UD2->Height = 20;
	hStackPanel_UD2->Children->Add(checkBox_UD2);
	TextBlock^ Srver_no2 = gcnew TextBlock();
	Srver_no2->Width = 100; Srver_no2->Height = 20;
	Srver_no2->Text = "Server Address 2 :";
	hStackPanel_UD2->Children->Add(Srver_no2);
	htextboxAddress_UD1->FontSize = 12; htextboxAddress_UD1->Height = 20;
	htextboxAddress_UD1->Width = 200; hStackPanel_UD1->Children->Add(htextboxAddress_UD1);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10; hStackPanel_UD1->Children->Add(hSpacer);
	TextBlock^ portNo = gcnew TextBlock();
	portNo->Width = 100; portNo->Height = 20; portNo->Text = "Port 1:";
	hStackPanel_UD1->Children->Add(portNo);
	htextboxPort_UD1->FontSize = 12; htextboxPort_UD1->Height = 20;
	htextboxPort_UD1->Width = 200; hStackPanel_UD1->Children->Add(htextboxPort_UD1);
	htextboxAddress_UD2->FontSize = 12;
	htextboxAddress_UD2->Height = 20; htextboxAddress_UD2->Width = 200;
	hStackPanel_UD2->Children->Add(htextboxAddress_UD2);
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 10; hStackPanel_UD2->Children->Add(hSpacer2);
	TextBlock^ port_No1 = gcnew TextBlock();
	port_No1->Width = 100; port_No1->Height = 20;
	port_No1->Text = "Port 2:";
	hStackPanel_UD2->Children->Add(port_No1);
	htextboxPort_UD2->Height = 20; htextboxPort_UD2->Width = 200;
	hStackPanel_UD2->Children->Add(htextboxPort_UD2);
	hStackPanel_UD1->Orientation = Orientation::Horizontal;
	hStackPanel_UD1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hStackPanel_UD2->Orientation = Orientation::Horizontal;
	hStackPanel_UD2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadFileGrid->SetRow(hStackPanel_UD1, 0);
	hDownloadFileGrid->Children->Add(hStackPanel_UD1);
	hDownloadFileGrid->SetRow(hStackPanel_UD2, 1);
	hDownloadFileGrid->Children->Add(hStackPanel_UD2);
}

//----< setup text search screen view >-------------------------------------
void WPFCppCliDemo::setUpTextSearchView() {
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	hTextSearchGridNew->RowDefinitions->Add(hRow2Def);
	Label^ addLabel = gcnew Label();
	addLabel->Content = L"Specify Text to search in\nserver";
	addLabel->Height = 50; addLabel->Width = 170;
	hAddTextTextBoxNew->Height = 25; hAddTextTextBoxNew->Width = 200;
	hFileUploadStackPanelNew->Children->Add(addLabel);
	TextBlock^ hSpacer = gcnew TextBlock();	hSpacer->Width = 10; 
	hFileUploadStackPanelNew->Children->Add(hAddTextTextBoxNew);
	hTextSearchGridNew->SetRow(hFileUploadStackPanelNew, 2);
	hTextSearchGridNew->Children->Add(hFileUploadStackPanelNew);
	hFileUploadStackPanelNew->Orientation = Orientation::Horizontal;
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50);
	hTextSearchGridNew->RowDefinitions->Add(hRow3Def);
	Label^ addPaternsLabel = gcnew Label();
	addPaternsLabel->Content = L"Specify multiple file patterns \nseperated by comma ,";
	addPaternsLabel->Height = 50; addPaternsLabel->Width = 170;
	hAddPatternTextBoxNew->Height = 25; hAddPatternTextBoxNew->Width = 200;
	hButtonStackPanelNew->Children->Add(addPaternsLabel);
	hButtonStackPanelNew->Children->Add(hAddPatternTextBoxNew);
	hButtonStackPanelNew->Orientation = Orientation::Horizontal;
	hTextSearchGridNew->SetRow(hButtonStackPanelNew, 3);
	hTextSearchGridNew->Children->Add(hButtonStackPanelNew);
	hSearchButtonNew->Content = "Search Text"; Border^ hBorder7 = gcnew Border();
	hBorder7->Width = 125; hBorder7->Height = 30;
	hBorder7->BorderThickness = Thickness(1);
	hBorder7->BorderBrush = Brushes::Black;	hBorder7->Child = hSearchButtonNew;
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(50);
	hTextSearchGridNew->RowDefinitions->Add(hRow4Def);
	hFinalButtonStackPanelNew->Children->Add(hBorder7);
	hTextSearchGridNew->SetRow(hFinalButtonStackPanelNew, 4);
	hTextSearchGridNew->Children->Add(hFinalButtonStackPanelNew);
	hFinalButtonStackPanelNew->Orientation = Orientation::Horizontal;
	hFinalButtonStackPanelNew->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hTextSearchGridNew->Margin = Thickness(20);	hTextSearchTab->Content = hTextSearchGridNew;
	Label^ addLabel4 = gcnew Label();
	addLabel4->Content = L"Results are displayed in the below grid after search";
	addLabel4->Height = 35; addLabel->Width = 170;
	RowDefinition^ hRow8Def = gcnew RowDefinition();
	hRow8Def->Height = GridLength(30); hTextSearchGridNew->RowDefinitions->Add(hRow8Def);
	hTextSearchGridNew->SetRow(addLabel4, 5); hTextSearchGridNew->Children->Add(addLabel4);
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(200);	hTextSearchGridNew->RowDefinitions->Add(hRow1Def);
	hTextSearchGridNew->SetRow(hTextListBoxNew, 6);	hTextSearchGridNew->Children->Add(hTextListBoxNew);
}

//----< browse for folder upon button click >-------------------------------------
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
  }
}

//----< setup download screen >-------------------------------------
void WPFCppCliDemo::setDownProperties() {
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadFileGrid->RowDefinitions->Add(hRow1Def);
	hRow1Def->Height = GridLength(50);
	Label^ fileNameLb = gcnew Label();
	fileNameLb->Content = L"File Name";
	fileNameLb->Height = 25; fileNameLb->Width = 325;
	fileDownBox->Height = 25; fileDownBox->Width = 200;
	hDownloadFileGrid->SetRow(fileDownBox, 2);
	hDownloadFileGrid->SetRow(fileNameLb, 2);
	hDownloadFileGrid->Children->Add(fileNameLb);
	hDownloadFileGrid->Children->Add(fileDownBox);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDownloadFileGrid->RowDefinitions->Add(hRow2Def);
	hDownloadButton->Content = "Download";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120; hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadButton;
	hStackPanel3->Children->Add(hBorder2);
	hClearFileButton->Content = "Clear";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120; hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearFileButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel3->Children->Add(hSpacer);
	hStackPanel3->Children->Add(hBorder3);
	hStackPanel3->Orientation = Orientation::Horizontal;
	hStackPanel3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadFileGrid->SetRow(hStackPanel3, 3);
	hDownloadFileGrid->Children->Add(hStackPanel3);
}

//----< setup download view screen >-------------------------------------
void WPFCppCliDemo::setUpDownloadView() {
	hDownloadFileGrid ->Margin = Thickness(20);
	hDownloadTab->Content = hDownloadFileGrid;
	setDownProperties();
}

//----< display screen on load >-------------------------------------
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{

}

//----< close servers on close of window >-------------------------------------
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  std::string disconnect = "shutdown|localhost|9080|NOFILE";
  pSendr1_->postMessage(disconnect);
  Thread::Sleep(1000);
}

//----< utility function to encode for string and text search >--------------------------
std::string WPFCppCliDemo::preparePatternTextEnc(String^ patterns, String^ searchText) {
	std::string pattern = "**p&a*t)t(e%rn#s**:";
	if (patterns->IsNullOrEmpty(patterns)) {
	}
	else {
		std::vector<std::string> res = getDetails(toStdString(patterns), ',');
		bool first = true;
		for (auto vec : res) {
			if (first) {
				pattern = pattern + vec;
				first = false;
			}
			else {
				pattern = pattern + "," + vec;
			}
		}
	}

	std::string searchStr = "**s&ea%rch#st^r)i(n(g**:" + toStdString(searchText);
	std::string resStr = pattern + "[$fL(^S*%S^$A$(R%FNP^$G]" + searchStr;
	return resStr;
}

[STAThread]
//----< starting pointing of the UI application >--------------------------
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");

  String^ address = args[0];
  String^ port = args[1];
  String^ path = args[2];

  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo(address, port, path));
  Console::WriteLine(L"\n\n");
}