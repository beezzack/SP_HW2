// test.cpp : �w�q�D���x���ε{�����i�J�I�C
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
#define			MaxBuf						64		//�C�������ζǰe���w�Ĥj�p

WSADATA			WsaData;			
char			BufSend[MaxBuf];					//��m�n�ǰe���T��
SOCKET			SelfSocket;							//�ۤv��socket
SOCKET			ServerSocket;						//Server��socket
BOOL			ExitServer;							//����O�_���}						
SOCKADDR_IN		AddrSelf;							//�ۤv��SOCKADDR_IN���c
SOCKADDR_IN		AddrServer;							//Server��SOCKADDR_IN���c


void			ExitClient(void);					//�����Ҧ���socket�Mthread
void			waitClient(void *p);				//������A�����s�u�M�����T��
void			connectServer(void);				//�ǰe�T����Server


////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])																	  //
{																									  //
//-----------------------------------------------��l��-----------------------------------------------//
	if ( 0 !=  WSAStartup(MAKEWORD(1,1), &WsaData) ){												  //																								
		ExitClient();																				  //
	}																								  //
//---------------------------------------------�إ�SCOKET---------------------------------------------//
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
//----------------------------------------------�}THREAD----------------------------------------------//						
	_beginthread( waitClient,0,(void*)"*");															  //
//-----------------------------------------------�e�T��-----------------------------------------------//	
	connectServer();																				  //
//------------------------------------------------����------------------------------------------------//
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
//--------------------------------------------��ܹ����--------------------------------------------// 
	char *pClientIP = inet_ntoa(addrClient.sin_addr);												  //
	u_short  clientPort = ntohs(addrClient.sin_port);												  //
	cout<<"Accept a client."<<endl;																	  //
	cout<<"IP: "<<pClientIP<<endl;																	  //						
	cout<<"Port: "<<clientPort<<endl;																  //
//----------------------------------------�}�sthread���L�H�s�u----------------------------------------// 	
	_beginthread( waitClient,0,(void*)"*");															  //
//-------------------------------------------������誺�T��-------------------------------------------// 	
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
//------------------------------------------------����------------------------------------------------//	
	_endthread();																					  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////
void	connectServer(void)																			  // 
{																									  //
//-----------------------------------------�إ�Server��SCOKET-----------------------------------------//
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
//------------------------------------------�ǰe��Server�T��------------------------------------------// 		 
	while(!ExitServer){																				  //																											 //
		memset(BufSend, 0, MaxBuf);																	  //
		cin.getline (BufSend,MaxBuf);																  //
		cout<<"me:"<<BufSend<<endl;																	  //
		if (send(ServerSocket, BufSend,strlen(BufSend),0)==SOCKET_ERROR){							  //																										 //
			ExitClient();																			  //
		}																							  //
	}																								  //
//------------------------------------------------����------------------------------------------------//	     
	ExitClient();																					  //
}																									  //
////////////////////////////////////////////////////////////////////////////////////////////////////////

