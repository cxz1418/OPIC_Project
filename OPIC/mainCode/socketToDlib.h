#ifndef SOCKET_FOR_DLIB
#define SOCKET_FOR_DLIB

#include"opencv2\opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include"68landmarkLoader.h"
#include <winsock2.h>
using namespace cv;

void ErrorHandling(char* message);



class LandmarkGetterFromDlib
{
public :

	Point * GetLandmark68_dlib()
	{
		bool isSuccess=false;
		Point *getPoint;

		WSADATA wsaData;
		SOCKET hSocket;
		SOCKADDR_IN servAddr;

		char message[30];
		int strLen;

		char address[100]="127.0.0.1";
		char port[100]="2016";


		if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			isSuccess=false;
			ErrorHandling("WSAStartup() error!");  
		}
		hSocket=socket(PF_INET, SOCK_STREAM, 0);
		if(hSocket==INVALID_SOCKET)
		{
			isSuccess=false;
			ErrorHandling("socket() error");
		}
		memset(&servAddr, 0, sizeof(servAddr));
		servAddr.sin_family=AF_INET;

		servAddr.sin_addr.s_addr=inet_addr(address);
		servAddr.sin_port=htons(atoi(port));

		if(connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr))==SOCKET_ERROR)
		{
			ErrorHandling("connect() error!");
			isSuccess=false;
		}
		///////////////////////////////////////////////////////////////////


		char message_go[]="PLEASE";
		send(hSocket, message_go, sizeof(message_go), 0);
		/////////////////////////////////////////////////////////////////////

		strLen=recv(hSocket, message, sizeof(message)-1, 0);
		if(strLen==-1)
		{
			isSuccess=false;
			ErrorHandling("read() error!");
		}
		printf("Message from server: %s \n", message); 
		if(!strcmp(message,"Success"))
		{
			isSuccess=true;
			getPoint=Get68Landmarks_300("sis2.pts");
			if(getPoint==NULL)
				isSuccess=false;
			 
		}
		else
		{
			isSuccess=false;
		}

			///////////////////////////////////////////////////////////




		closesocket(hSocket);
		WSACleanup();

		if(isSuccess==false)
			return NULL;
		else
			return getPoint;

		

	}



};



void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


#endif