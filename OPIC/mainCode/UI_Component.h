#ifndef UI_COMPONENT
#define UI_COMPONENT

#define SIZE_CONTROL 0.8
#define ID_EDIT 101

#include"CSMTP_custom.h"
#include<Windows.h>
#include"opencv2\opencv.hpp"
#include"opencv2\core\core.hpp"
#include"CameraHelper.h"
#include"CvvImage.h"
#include<iostream>
#include <GdiPlus.h>
#include"LuxandFaceSDK.h"
#include"MorphManager.h"
#include"socketToDlib.h"

using namespace std;
using namespace Gdiplus;


enum {MOUSE_ON=0,MOUSE_RELEASE,MOUSE_PRESS};
enum {DEFAULT=0,NEXT_SEX_M,NEXT_SEX_W,
	NEXT_STYLE1_W,NEXT_STYLE2_W,NEXT_STYLE3_W,
	NEXT_STYLE1_M,NEXT_STYLE2_M,NEXT_STYLE3_M,
	NEXT_CAMERA ,NEXT_DETECTION ,NEXT_RESULT, BACK_CAMERA,NEXT_RESULT2 ,NEXT_EMAIL,BACK_EMAIL};



class UI_Button{


public:

	int id;
	int state;

	RECT position;
	Image *onImg;
	Image *releaseImg;
	Image *pressImg;


	//PNG를 제외한 파일만 가능케해놨다.
	UI_Button(RECT _position,wchar_t * _onPath,wchar_t *_releasePath,wchar_t *_pressPath,int _id)
	{
		id=_id;
		onImg=new Image(_onPath);
		pressImg=new Image(_pressPath);
		releaseImg=new Image(_releasePath);



		state=MOUSE_RELEASE;
		this->position=_position;


	}


	void Draw(HDC hdc)
	{
		Graphics graphic(hdc);

		switch(this->state)
		{
		case MOUSE_ON:
			graphic.DrawImage(onImg,position.left,position.top,position.right-position.left,position.bottom-position.top);

			break;
		case MOUSE_PRESS:
			graphic.DrawImage(pressImg,position.left,position.top,position.right-position.left,position.bottom-position.top);
			break;
		case MOUSE_RELEASE:

			graphic.DrawImage(releaseImg,position.left,position.top,position.right-position.left,position.bottom-position.top);
			break;
		}

	}

	//마우스 정보를 받아서 지속적을 State관리를 한다.
	void StateCheck(int x,int y,bool bNowPress)
	{
		POINT pt;
		pt.x=x;pt.y=y;
		bool collision;
		if(PtInRect(&position,pt))
		{
			collision=true;
			//cout<<"collision Occur in"<<x<<','<<y<<"--"<<position.left<<','<<position.top<<','<<position.right<<','<<position.bottom<<endl;
		}
		else
			collision=false;

		if(bNowPress==false && collision==true)
		{
			if(this->state==MOUSE_PRESS)
			{
				this->state=MOUSE_RELEASE;

			}
			this->state=MOUSE_ON;
		}
		if(bNowPress==true && collision==true)
			this->state=MOUSE_PRESS;


		if(bNowPress==false && collision==false)
		{
			this->state=MOUSE_RELEASE;
		}

		if(bNowPress==true && collision==false)
		{
			this->state=MOUSE_RELEASE;
		}


	}
};
class UI_Image
{
public:
	bool drawToggle;
	RECT position;
	Image *image;

	UI_Image(RECT _position,wchar_t * path)
	{

		this->drawToggle=true;
		this->position=_position;

		image = new Image(path);



	}


	UI_Image(RECT _position,wchar_t * path,bool drawToggle)
	{

		this->drawToggle=drawToggle;
		this->position=_position;

		image = new Image(path);



	}

	~UI_Image()
	{
		image->~Image();
	}

	void Draw(HDC hdc)
	{
		if(drawToggle==true)
		{
			Graphics graphic(hdc);

			graphic.DrawImage(image,position.left,position.top,position.right-position.left,position.bottom-position.top);
		}
	}


};


#endif