#define _WINSOCKAPI_ 


#include <windows.h>
#include"LuxandFaceSDK.h"
#include "resource.h"
#include <GdiPlus.h>
#include "CvvImage.h"
#include"opencv\cv.h"
#include"opencv2\opencv.hpp"
#include"UI_Winow.h"
#include"opencv2\core\core.hpp"
#include"CameraHelper.h"


using namespace cv;
using namespace Gdiplus;



#define WND_WIDTH 1920*SIZE_CONTROL
#define WND_HEIGHT 1080*SIZE_CONTROL
#pragma comment(lib, "gdiplus")


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;

HWND hWndMain;
LPCTSTR lpszClass=TEXT("OPIC");




int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	AllocConsole();
	freopen("conout$","wt",stdout);





	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,WND_WIDTH,WND_HEIGHT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while(1)
	{
		if(PeekMessage(&Message,NULL,0,0,PM_REMOVE))
		{
			if(Message.message==WM_QUIT)break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	/*
	while (GetMessage(&Message,NULL,0,0)) {
	TranslateMessage(&Message);
	DispatchMessage(&Message);
	}
	*/


	return (int)Message.wParam;
}


std::vector<cv::Mat> channels;
UI_WindowManager uiManager;



LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HDC hMemDC;
	HBITMAP hBackBit,hOldBitmap;

	static int x;
	static int y;
	static BOOL bNowDraw=FALSE;

	bNowDraw=FALSE;


	static VideoCapture cap(2); // open the default camera


	switch (iMessage) {
	case WM_CREATE:
		{


			//FSDK초기화
			if (FSDKE_OK != FSDK_ActivateLibrary("vn448BvFsIsF23cl8ZA/2eO6DzsPsohfaJvTW7OrPCtA9k/aigd0de6LwoEbHfd9rTHSfZ3p7X/WSnh/htMzMH0VqCecALZksj7USKsjH3/+HMl7AhpSnqYL72zIq+kAt8fwuMsOUTzeLiNJOddDNA1fuSEQvg6wt7gs7K7CsWo=")) {
				fprintf(stderr, "Error activating Luxand FaceSDK\n");
				fprintf(stderr, "Please run the License Key Wizard (Start - Luxand - FaceSDK - License Key Wizard)\n");

				char * buf = new char[1024];
				memset((void *)buf, 0, 1024);
				FSDK_GetLicenseInfo(buf);
				fprintf(stderr, "Licensing info: %s\n", buf);
				exit(1);
			}

			if (FSDK_Initialize("") != FSDKE_OK){
				fprintf(stderr, "Error initializing Luxand FaceSDK!\n");
				exit(1);
			}
			//


			RECT wndSize;
			GetClientRect(hWndMain,&wndSize);


			hWndMain=hWnd;
			SetTimer(hWnd,1,2,NULL);

			//GDI초기화
			GdiplusStartupInput         m_GdiplusStartupInput;
			ULONG_PTR                   m_GdiplusToken;
			GdiplusStartup(&m_GdiplusToken, &m_GdiplusStartupInput, NULL);




			uiManager.Init(&cap,hWnd,g_hInst);


			return 0;
		}
	case WM_TIMER:

		uiManager.GetCameraScene();//cap >> frame; // get a new frame from camera	
		InvalidateRect(hWnd,NULL,FALSE);
		return 0;

	case WM_PAINT:
		{
			//더블 버퍼링
			hdc=BeginPaint(hWnd, &ps);
			hMemDC=CreateCompatibleDC(hdc);
			hBackBit=CreateCompatibleBitmap(hdc,WND_WIDTH,WND_HEIGHT);
			hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackBit);



			uiManager.Draw(hMemDC,hdc);


			// 완성된 그림 전송
			BitBlt(hdc,0,0,WND_WIDTH,WND_HEIGHT,hMemDC,0,0,SRCCOPY);


			SelectObject(hMemDC,hOldBitmap);
			DeleteObject(hBackBit);
			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);
			return 0;
		}

	case WM_MOUSEMOVE:

		x=LOWORD(lParam);
		y=HIWORD(lParam);
		uiManager.SetMouseState(x,y,bNowDraw);			

		return 0;
	case WM_LBUTTONUP:

		bNowDraw=FALSE;
		uiManager.SetMouseState(x,y,bNowDraw);
		return 0;
	case WM_LBUTTONDOWN:

		x=LOWORD(lParam);
		y=HIWORD(lParam);
		bNowDraw=TRUE;
		uiManager.SetMouseState(x,y,bNowDraw);
		return 0;



	case WM_DESTROY:


		PostQuitMessage(0);

		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


