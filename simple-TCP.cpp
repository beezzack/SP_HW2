// test.cpp : 定義主控台應用程式的進入點。
//
// server.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <process.h>

using namespace std;
#include <WINSOCK2.H>
#pragma comment(lib, "wsock32.lib")



#define			ServerPort					4000	//port
#define			MaxBuf						64		//每次接受或傳送的緩衝大小

WSADATA			WsaData;			
char			BufSend[MaxBuf];					//放置要傳送的訊息
SOCKET			SelfSocket;							//自己的socket
SOCKET			ServerSocket;						//Server的socket
BOOL			ExitServer;							//控制是否離開						
SOCKADDR_IN		AddrSelf;							//自己的SOCKADDR_IN結構
SOCKADDR_IN		AddrServer;							//Server的SOCKADDR_IN結構


void			ExitClient(void);					//關閉所有的socket和thread
void			waitClient(void *p);				//執行緒，接收連線和接收訊息
void			connectServer(void);				//傳送訊息給Server


////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])																	  //
{																									  //
//-----------------------------------------------初始化-----------------------------------------------//
	if ( 0 !=  WSAStartup(MAKEWORD(1,1), &WsaData) ){												  //																								
		ExitClient();																				  //
	}																								  //
//---------------------------------------------建立SCOKET---------------------------------------------//
	SelfSocket= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);											  //
	if(INVALID_SOCKET == SelfSocket){																  //	
		ExitClient();																				  //
	}																								  //
//------------------------------------------------BIND------------------------------------------------//
	AddrSelf.sin_family = AF_INET;																	  //
	AddrSelf.sin_port = htons(ServerPort);															  //
	AddrSelf.sin_addr.s_addr = INADDR_ANY;															  //
	if(SOCKET_ERROR == bind(SelfSocket, (LPSOCKADDR)&AddrSelf, sizeof(AddrSelf))){					  //
		ExitClient();																				  //
	}																								  //
//-----------------------------------------------LISTEN-----------------------------------------------// 
	if(SOCKET_ERROR == listen(SelfSocket, 1)){														  //
		ExitClient();																				  //
	}																								  //
	cout << "Server succeeded!" << endl;															  //
	cout << "Waiting for new clients..." << endl;													  //
//----------------------------------------------開THREAD----------------------------------------------//						
	_beginthread( waitClient,0,(void*)"*");															  //
//-----------------------------------------------送訊息-----------------------------------------------//	
	connectServer();																				  //
//------------------------------------------------結束------------------------------------------------//
	ExitClient();																					  //
	system("pause");																				  //
	return 0;																						  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////
void	ExitClient()																				  //
{																									  //
	cout << "Server exiting..." << endl;															  //
	closesocket(ServerSocket);																		  //
	closesocket(SelfSocket);																		  //
	WSACleanup();																					  //
	_endthread();																					  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////
void	waitClient(void *p)																			  //
{																									  //
//-----------------------------------------------ACCEPT-----------------------------------------------//
	sockaddr_in addrClient;																			  //
	int addrClientlen = sizeof(addrClient);															  //
	SOCKET	ClientSocket= accept(SelfSocket,(sockaddr FAR*)&addrClient, &addrClientlen);			  //
	if(INVALID_SOCKET == ClientSocket){																  //
		ExitClient();																				  //
	}																								  //
//--------------------------------------------顯示對方資料--------------------------------------------// 
	char *pClientIP = inet_ntoa(addrClient.sin_addr);												  //
	u_short  clientPort = ntohs(addrClient.sin_port);												  //
	cout<<"Accept a client."<<endl;																	  //
	cout<<"IP: "<<pClientIP<<endl;																	  //						
	cout<<"Port: "<<clientPort<<endl;																  //
//----------------------------------------開新thread等他人連線----------------------------------------// 	
	_beginthread( waitClient,0,(void*)"*");															  //
//-------------------------------------------接收對方的訊息-------------------------------------------// 	
	char	BufRecv[MaxBuf];	
	int rVal;																	  //
	while(!ExitServer){																				  //
		memset(BufRecv, 0, MaxBuf);																	  //
		if ((rVal=recv(ClientSocket, BufRecv, MaxBuf, 0))==SOCKET_ERROR){									  //
			_endthread();																			  //
		}																							  //
		if (rVal==0){									  //
			_endthread();																			  //
		}																							  //
		cout <<pClientIP<< ":"<<BufRecv<<endl;														  //
	}																								  //
//------------------------------------------------結束------------------------------------------------//	
	_endthread();																					  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////
void	connectServer(void)																			  // 
{																									  //
//-----------------------------------------建立Server的SCOKET-----------------------------------------//
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                                         //
	if(INVALID_SOCKET == ServerSocket){																  //																											 
		ExitClient();																				  //
	}																								  //
//-----------------------------------------------CONNECT----------------------------------------------//		
	AddrServer.sin_family = AF_INET;																  //
	char ServerIP[20];																				  //
	cout<<"Enter the Server IP:";																	  //
	cin>>ServerIP;																					  //
	AddrServer.sin_addr.s_addr = inet_addr(ServerIP);												  //
	AddrServer.sin_port = htons(ServerPort);														  //
	if(SOCKET_ERROR == connect(ServerSocket,(LPSOCKADDR)&AddrServer, sizeof(AddrServer))){			  //																										
		ExitClient();																				  //
	}																								  //
	cout<<"Connect successfully!"<<endl;															  //
	cin.ignore();																					  //
//------------------------------------------傳送給Server訊息------------------------------------------// 		 
	while(!ExitServer){																				  //																											 //
		memset(BufSend, 0, MaxBuf);																	  //
		cin.getline (BufSend,MaxBuf);																  //
		cout<<"me:"<<BufSend<<endl;																	  //
		if (send(ServerSocket, BufSend,strlen(BufSend),0)==SOCKET_ERROR){							  //																										 //
			ExitClient();																			  //
		}																							  //
	}																								  //
//------------------------------------------------結束------------------------------------------------//	     
	ExitClient();																					  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////

