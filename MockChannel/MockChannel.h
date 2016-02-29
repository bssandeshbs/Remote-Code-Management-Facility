#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////
//  MockChannel.h - Builds as DLL so that C++\CLI client can user  //
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
/*
* Package Operations:
* ===================
* This package builds as DLL so that C++\CLI client can use native code. The Mockchannel provides the  
* ISender, IReceiver, IClient Interface as a DLL which is used in cppCli-WPF -App 
*
* Required Files:
* ===============
* AppHelpers.h AppHelpers.cpp Socket.h Socket.cpp Message.h Message.cpp Sender.h Sender.cpp
* Receiver.h Receiver.cpp Client.h Client.cpp FileManager.h, FileSystem.cpp,FileSystem.h,FileManager.cpp
* MockChannel.cpp MockChannel.h Cpp11-BlockingQueue.h Qserver.h QServer.cpp
*
* Build Process:
* ==============
* cl /DTEST_CLIENT Client.cpp ../ApplicationHelpers/AppHelpers.cpp ../Message/Message.cpp
* ../sockets/sockets.cpp ../Receiver/Receiver.cpp ../QServer/QServer/QServer.cpp ../QServer/Cpp11-
* BlockingQueue ../Sender/Sender.cpp /Cpp11-BlockingQueue.cpp ../Display/Display.cpp
* ../FileManager/FileManager.cpp ../FileSystem/FileSystem.cpp  ../Catalogue/Cataloge.cpp ../DataStore/DataStore.cpp
*
* Public Interface:
* =================
*  class ClientUI : public IClient
*  ClientUI(ISendr* pSendr, IRecvr* pRecvr); - initializes the clients sender and receiver threads
*  void start(std::string address, std::string port,std::string path)  - starts the client object
*  virtual void stop()  - stops the client object
*  class Sendr : public ISendr
*  void postMessage(const Message1& msg) - provides the functionality to enquee 
*
* class Recvr : public IRecvr
*  string getMessage(); - provides the functionailty to dequee from the queue
*
*	 DLL Factory
*
*    DLL_DECL ISendr* createSendr() - create sender
*    DLL_DECL IRecvr* createRecvr() - create receiver
* 	 DLL_DECL IClient* createClient(ISendr* pISendr, IRecvr* pIRecvr) - create client
*
* Usage :
* ==================
*  ObjectFactory* pObjFact = new ObjectFactory;
*  pSendr1_ = pObjFact->createSendr();
*  pRecvr2_ = pObjFact->createRecvr();
*  client_ = pObjFact->createClient(pSendr1_, pRecvr2_);
*  client_->start(toStdString(address), toStdString(port), toStdString(path));
*
*
* Maintenance History:
* ====================
* ver 2.0 : 27 Apr 15
*  added Iclient Interface
* ver 1.0 : 20 Apr 15
* - Fawcett Initial code
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message1 = std::string;

struct ISendr
{
  virtual void postMessage(const Message1& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IClient {
public:
	virtual void start(std::string address,std::string port,std::string path) = 0;
	virtual void stop() = 0;
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
	DLL_DECL IClient* createClient(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


