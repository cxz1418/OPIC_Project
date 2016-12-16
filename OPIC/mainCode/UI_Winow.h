//MVC 의 View와 Control에 해당하는 클래스이다.
//UI_Manager가 이둘을 관리하며
//사용자의 입력값에 따라 상황에맞는
//서비스 클래스에게 명령을 한다.
//ex)EmailSender,Converter,Extracter

#ifndef UI_WINDOW
#define UI_WINDOW
#include"UI_Component.h"

void DrawToHDC_mat(Mat &mat,HDC _hdc,RECT *cameraRect);
void ConvertMatToCvImg(Mat &mat, CvvImage &cvImg);




void ControlSize(RECT &dummy_box )
{
	dummy_box.left*=SIZE_CONTROL;
	dummy_box.top*=SIZE_CONTROL;
	dummy_box.right*=SIZE_CONTROL;
	dummy_box.bottom*=SIZE_CONTROL;
}
void SetDummy(RECT &box,int left,int top,int right,int bottom)
{
	box.left=left;
	box.top=top;
	box.right=right;
	box.bottom=bottom;
	ControlSize(box);
}


//Stage별 Scene을 관리하는 클래스입니다.
//각 Scene은 이미지 + 버튼으로 구성됩니다
class UI_Window
{
	///
public:

	vector<UI_Image *> PartImages; //이미지
	vector<UI_Button *> Buttons;  //버튼

	//이미지를 삽입합니다 drawToggle=true
	void InputPartImage(RECT position,wchar_t * path)
	{
		PartImages.push_back(new UI_Image(position,path));
	}
	//이미지를 삽입합니다 drawToggle은 사용자 설정에따라
	void InputPartImage(RECT position,wchar_t * path,bool drawToggle)
	{
		PartImages.push_back(new UI_Image(position,path,drawToggle));
	}


	~UI_Window()
	{
		for(int i=0; i<PartImages.size();i++)
		{
			PartImages[i]->~UI_Image();
			PartImages.clear();
		}
		for(int k=0; k<Buttons.size(); k++)
		{

			Buttons.clear();
		}
	}

	//버튼을 삽입합니다.
	//마우스상태가 on,release,press 3가지일때 각각
	//다른 이미지를 뛰우며 버튼마다 고유id를 가지고있어 
	//버튼클릭시 구별합니다.
	void InputButton(RECT position,wchar_t *onPath, wchar_t* releasePath,wchar_t * pressPath,int id)
	{
		Buttons.push_back(new UI_Button(position,onPath,releasePath,pressPath,id));

	}

	//마우스가 화면을 클릭시 버튼을 감지하기위한 함수입니다.
	//충돌시 버튼의 상태를 리턴합니다 (on relase press)
	void SetMouseState(int x,int y , bool bNowPress,int &buttonState)
	{
		for(int i=0; i<Buttons.size(); i++)
		{
			
			Buttons[i]->StateCheck(x,y,bNowPress);
			if(Buttons[i]->state==MOUSE_PRESS)
			{
				buttonState=Buttons[i]->id;
				return ;
			}

		}
		buttonState=DEFAULT;

	}


	//이미지와 버튼을 그려줍니다.
	void Draw(HDC hdc)
	{

		for(int i=0; i<PartImages.size(); i++)
		{
			PartImages[i]->Draw(hdc);
		}

		for(int i=0; i<Buttons.size(); i++)
		{
			Buttons[i]->Draw(hdc);
		}


	}


};


//Stage를 나타냅니다
enum {SEX,STYLE,STYLE2,CAMERA,DETECTION,RESULT,EMAIL};
//SEX : 성별선택
//STYLE : 남자선택
//STYLE2 : 여자선택
//CAMERA : 촬영
//DETECTION : 추출결과
//RESULT : 변환결과
//EMAIL : Email전송


//Control+View를 담당하는 클래스입니다.
//Stage별 Scene을 컨트롤합니다.
class UI_WindowManager
{
public:



	int cameraPicType();

	//Service Class입니다
	SMTP_Helper EmailSender;  //EmailSender     
	MorphManager Converter;   //Converter
	LandmarkGetterFromDlib Extracter;  //Extracter

	//각 Scene에 해당하는 클래스입니다.
	UI_Window UC_Sex;
	UI_Window UC_Style;
	UI_Window UC_Style2;
	UI_Window UC_Camera;
	UI_Window UC_Detection;
	UI_Window UC_Result;
	UI_Window UC_Email;

	int styleChoice;
	int state;
	int libChoice;
	VideoCapture *cap;


	Mat mat;
	CvvImage mat_cv;
	Mat result_mat;
	CvvImage result_cv;

	RECT cameraRect;
	RECT cameraRect_dst;

	Mat detectionResult;
	Mat conversionResult;


	bool isFindFace;


	HWND hWnd_s;
	HFONT hFont; 
	HINSTANCE g_hInst_s;


	UI_Image *emailOK;
	UI_Image *emailFail;
	UI_Image *detectionOK;
	UI_Image *detectionFail;

	
	HWND hWndGetEmail;
	RECT emailBox;



	void Init(VideoCapture * _cap,HWND _hWnd, HINSTANCE _g_hInst)
	{



		libChoice=LUXAND;
		this->hWnd_s=_hWnd;
		this->g_hInst_s=_g_hInst;

		this->state=SEX;
		this->cap=_cap;


		RECT dummy_box;


		dummy_box.left=0; dummy_box.top=0;
		dummy_box.right=1920; dummy_box.bottom=1080;
		ControlSize(dummy_box);
		UC_Sex.InputPartImage(dummy_box,L"uiBack.png");


		dummy_box.left=637; dummy_box.top=436;
		dummy_box.right=884; dummy_box.bottom=613;
		ControlSize(dummy_box);
		UC_Sex.InputButton(dummy_box,L"UI_SEX/girlbutton_o.png",L"UI_SEX/girlbutton.png",L"UI_SEX/girlbutton.png",NEXT_SEX_W);
		dummy_box.left=998; dummy_box.top=436;
		dummy_box.right=1231; dummy_box.bottom=616;
		ControlSize(dummy_box);
		UC_Sex.InputButton(dummy_box,L"UI_SEX/boybutton_o.png",L"UI_SEX/boybutton.png",L"UI_SEX/boybutton.png",NEXT_SEX_M);



		dummy_box.left=1475; dummy_box.top=652;
		dummy_box.right=1896; dummy_box.bottom=1079;
		ControlSize(dummy_box);
		UC_Sex.InputPartImage(dummy_box,L"UI_SEX/helper.png");

		dummy_box.left=1372; dummy_box.top=125;
		dummy_box.right=1896; dummy_box.bottom=652;
		ControlSize(dummy_box);
		UC_Sex.InputPartImage(dummy_box,L"UI_SEX/talk_gender.png");

		dummy_box.left=528; dummy_box.top=178;
		dummy_box.right=1378; dummy_box.bottom=351;
		ControlSize(dummy_box);
		UC_Sex.InputPartImage(dummy_box,L"UI_SEX/title_gender.png");

		////////////////////////////////////////
		dummy_box.left=0; dummy_box.top=0;
		dummy_box.right=1920; dummy_box.bottom=1080;
		ControlSize(dummy_box);
		UC_Style.InputPartImage(dummy_box,L"uiBack.png");

		dummy_box.left=364; dummy_box.top=724;
		dummy_box.right=676; dummy_box.bottom=888;
		ControlSize(dummy_box);
		UC_Style.InputButton(dummy_box,L"UI_STYLE/selbutton1_o.png",L"UI_STYLE/selbutton1.png",L"UI_STYLE/selbutton1.png",NEXT_STYLE1_W);
		dummy_box.left=757; dummy_box.top=724;
		dummy_box.right=1069; dummy_box.bottom=888;
		ControlSize(dummy_box);
		UC_Style.InputButton(dummy_box,L"UI_STYLE/selbutton2_o.png",L"UI_STYLE/selbutton2.png",L"UI_STYLE/selbutton2.png",NEXT_STYLE2_W);
		dummy_box.left=1144; dummy_box.top=724;
		dummy_box.right=1454; dummy_box.bottom=888;
		ControlSize(dummy_box);	
		UC_Style.InputButton(dummy_box,L"UI_STYLE/selbutton3_o.png",L"UI_STYLE/selbutton3.png",L"UI_STYLE/selbutton3.png",NEXT_STYLE3_W);

		dummy_box.left=1475; dummy_box.top=652;
		dummy_box.right=1896; dummy_box.bottom=1079;
		ControlSize(dummy_box);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1491,474,1896,649);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/talk_sel.png");


		SetDummy(dummy_box,361,336,708,704);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");

		SetDummy(dummy_box,740,336,1089,704);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");

		SetDummy(dummy_box,1118,336,1461,705);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");
		//
		SetDummy(dummy_box,400,374,656,650);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/sel_girl1.png");

		SetDummy(dummy_box,767,374,1054,650);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/sel_girl2.png");

		SetDummy(dummy_box,1147,374,1425,650);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/sel_girl3.png");

		SetDummy(dummy_box,527,72,1376,249);
		UC_Style.InputPartImage(dummy_box,L"UI_STYLE/title_sel.png");


		/////////////////////////////
		dummy_box.left=0; dummy_box.top=0;
		dummy_box.right=1920; dummy_box.bottom=1080;
		ControlSize(dummy_box);
		UC_Style2.InputPartImage(dummy_box,L"uiBack.png");

		dummy_box.left=364; dummy_box.top=724;
		dummy_box.right=676; dummy_box.bottom=888;
		ControlSize(dummy_box);
		UC_Style2.InputButton(dummy_box,L"UI_STYLE/selbutton_boy1_o.png",L"UI_STYLE/selbutton_boy1.png",L"UI_STYLE/selbutton_boy1.png",NEXT_STYLE1_M);
		dummy_box.left=757; dummy_box.top=724;
		dummy_box.right=1069; dummy_box.bottom=888;
		ControlSize(dummy_box);
		UC_Style2.InputButton(dummy_box,L"UI_STYLE/selbutton_boy2_o.png",L"UI_STYLE/selbutton_boy2.png",L"UI_STYLE/selbutton_boy2.png",NEXT_STYLE2_M);
		dummy_box.left=1144; dummy_box.top=724;
		dummy_box.right=1454; dummy_box.bottom=888;
		ControlSize(dummy_box);	
		UC_Style2.InputButton(dummy_box,L"UI_STYLE/ready.png",L"UI_STYLE/ready.png",L"UI_STYLE/ready.png",NEXT_STYLE3_M);

		dummy_box.left=1475; dummy_box.top=652;
		dummy_box.right=1896; dummy_box.bottom=1079;
		ControlSize(dummy_box);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1478,357,1896,649);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/talk_sel_boy.png");


		SetDummy(dummy_box,361,336,708,704);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");

		SetDummy(dummy_box,740,336,1089,704);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");

		SetDummy(dummy_box,1118,336,1461,705);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/background_sel.png");
		//
		SetDummy(dummy_box,400,374,656,650);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/sel_boy1.png");

		SetDummy(dummy_box,767,374,1054,650);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/sel_boy2.jpg");

		SetDummy(dummy_box,1147,374,1425,650);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/ready.png");

		SetDummy(dummy_box,527,72,1376,249);
		UC_Style2.InputPartImage(dummy_box,L"UI_STYLE/title_sel.png");


		////////////////////////////////////////
		cameraRect.left=538;
		cameraRect.top=166;
		cameraRect.right=1364;
		cameraRect.bottom=782;
		ControlSize(cameraRect);



		SetDummy(dummy_box,0,0,1920,1080);
		UC_Camera.InputPartImage(dummy_box,L"uiBack.png");


		SetDummy(dummy_box,796,831,1108,995);	
		UC_Camera.InputButton(dummy_box,L"UI_CAMERA/shotbutton_o.png",L"UI_CAMERA/shotbutton.png",L"UI_CAMERA/shotbutton.png",NEXT_CAMERA);

		SetDummy(dummy_box,525,153,1377,795);
		UC_Camera.InputPartImage(dummy_box,L"UI_CAMERA/outline.png");


		SetDummy(dummy_box,1478,653,1896,1075);
		UC_Camera.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1402,263,1896,630);
		UC_Camera.InputPartImage(dummy_box,L"UI_CAMERA/talk_shot.png");

		/////////////////////////////////////////////////////////////////////

		SetDummy(dummy_box,0,0,1920,1080);
		UC_Detection.InputPartImage(dummy_box,L"uiBack.png");


		SetDummy(dummy_box,525,153,1377,795);
		UC_Detection.InputPartImage(dummy_box,L"UI_CAMERA/outline.png");


		SetDummy(dummy_box,1478,653,1896,1075);
		UC_Detection.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1423,335,1895,651);
		UC_Detection.InputPartImage(dummy_box,L"UI_DETECTION/talk_check.png");
		detectionOK=UC_Detection.PartImages[UC_Detection.PartImages.size()-1];


		SetDummy(dummy_box,1463,413,1887,677);
		UC_Detection.InputPartImage(dummy_box,L"UI_DETECTION/talk_check_fail.png",false);
		detectionFail=UC_Detection.PartImages[UC_Detection.PartImages.size()-1];

		SetDummy(dummy_box,980,798,1288,967);
		UC_Detection.InputButton(dummy_box,L"UI_DETECTION/convertbutton_o.png",L"UI_DETECTION/convertbutton.png",L"UI_DETECTION/convertbutton.png",NEXT_DETECTION);



		SetDummy(dummy_box,598,798,909,967);
		UC_Detection.InputButton(dummy_box,L"UI_DETECTION/reshotbutton_o.png",L"UI_DETECTION/reshotbutton.png",L"UI_DETECTION/reshotbutton.png",BACK_CAMERA);



		///////////////////////////////////////////////////////////////
		SetDummy(dummy_box,0,0,1920,1080);
		UC_Result.InputPartImage(dummy_box,L"uiBack.png");


		SetDummy(dummy_box,763,261-100,1215,813-100);
		UC_Result.InputPartImage(dummy_box,L"UI_CAMERA/outline.png");


		SetDummy(dummy_box,1478,653,1896,1075);
		UC_Result.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1446,128,1897,651);
		UC_Result.InputPartImage(dummy_box,L"UI_RESULT/talk_finish.png");
		//


		SetDummy(dummy_box,620,836,932,990);
		UC_Result.InputButton(dummy_box,L"UI_RESULT/gobackbutton_o.png",L"UI_RESULT/gobackbutton.png",L"UI_RESULT/gobackbutton.png",NEXT_RESULT);


		SetDummy(dummy_box,998,836,1308,990);
		UC_Result.InputButton(dummy_box,L"UI_RESULT/emailbutton_o.png",L"UI_RESULT/emailbutton.png",L"UI_RESULT/emailbutton.png",NEXT_RESULT2);

		SetDummy(dummy_box,0,0,1920,1080);
		UC_Email.InputPartImage(dummy_box,L"uiBack.png");	

		SetDummy(dummy_box,1478,653,1896,1075);
		UC_Email.InputPartImage(dummy_box,L"UI_STYLE/helper.png");

		SetDummy(dummy_box,1476,494,1878,650);
		UC_Email.InputPartImage(dummy_box,L"UI_EMAIL/talk_email.png");
		emailOK=UC_Email.PartImages[UC_Email.PartImages.size()-1];

		//


		SetDummy(dummy_box,1449,441,1897,669);
		UC_Email.InputPartImage(dummy_box,L"UI_EMAIL/talk_email_fail.png",false);
		emailFail=UC_Email.PartImages[UC_Email.PartImages.size()-1];

		//

		SetDummy(dummy_box,441,415,1408,576);
		UC_Email.InputPartImage(dummy_box,L"UI_EMAIL/background_email.png");
		//
		SetDummy(dummy_box,470,448,1369,539);
		UC_Email.InputPartImage(dummy_box,L"UI_EMAIL/background_email2.png");
		//

		emailBox.left=500;emailBox.top=465;
		emailBox.right=1336; emailBox.bottom=530;
		ControlSize(emailBox);
		hWndGetEmail=CreateWindow(TEXT("edit"),NULL,WS_CHILD   | LBS_NOTIFY,
			emailBox.left,emailBox.top,emailBox.right-emailBox.left
			,emailBox.bottom-emailBox.top,hWnd_s,(HMENU)ID_EDIT,g_hInst_s,NULL);


		hFont=CreateFont(40,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"Tahoma");
		SendMessage(hWndGetEmail,WM_SETFONT,(WPARAM)hFont,(LPARAM)FALSE);



		SetDummy(dummy_box,819,614,1020,735);
		UC_Email.InputButton(dummy_box,L"UI_EMAIL/sendbutton_o.png",L"UI_EMAIL/sendbutton.png",L"UI_EMAIL/sendbutton.png",NEXT_EMAIL);


		SetDummy(dummy_box,1213,595,1421,729);
		UC_Email.InputButton(dummy_box,L"UI_EMAIL/sendcancel.png",L"UI_EMAIL/sendcancel.png",L"UI_EMAIL/sendcancel.png",BACK_EMAIL);


	}

	void GetCameraScene()
	{
		if(this->state==CAMERA)
			*cap>>mat;
		cv::flip(mat,mat,1);
	}

	void GetDetectionResult(Mat &_mat)
	{
		HImage img1; 
		//여기서 사진파일 mat에다가 landmark를 찍어줘서 
		//표시해주고
		//랜드마크인풋도 여기서 받는다.

		detectionResult=imread("sis2.jpg",-1);

		char *fileName="sis2.jpg";

		printf("Loading file %s...\n","sis2.jpg");
		if (FSDK_LoadImageFromFile(&img1, fileName) != FSDKE_OK){
			fprintf(stderr, "Error loading file!\n");
			exit(2);
		}


		TFacePosition FaceArray[50];
		FSDK_Features Features; 
		int DetectedCount;

		FSDK_SetFaceDetectionThreshold(4);


		FSDK_DetectMultipleFaces(img1, &DetectedCount , FaceArray, sizeof(FaceArray));




		if(DetectedCount<1)
		{
			isFindFace=false;
		}
		else
		{
			isFindFace=true;
		}
		for(int i=0; i<DetectedCount; i++)
		{
			FSDK_DetectFacialFeaturesInRegion(img1, &FaceArray[i], &Features); 


			cv::Point *land_points = new cv::Point[70];
			for(int i=0; i<70; i++)
			{
				cv::Point pt;
				pt.x=Features[i].x;pt.y=Features[i].y;
				circle(_mat,pt,8,Scalar(0,0,255),2,1,0);

				land_points[i].x=Features[i].x;
				land_points[i].y=Features[i].y;


			}
			Converter.SetLandPoints(land_points);
		}






	}

	void GetDetectionResult_dlib(Mat &_mat)
	{

		cv::Point *points=Extracter.GetLandmark68_dlib();

		if(points!=NULL)
		{
			Converter.SetLandPoints(points);
			for(int i=0; i<68; i++)
			{
				circle(_mat,points[i],3,Scalar(255,0,0),2,1,0);
			}
			isFindFace=true;
		}
		else
		{
			cout<<"Fail Dlib"<<endl;
			isFindFace=false;
		}
	}

	void GetConversionResult(Mat _mat)
	{
		this->mat=_mat.clone();
		conversionResult=imread("result.png",-1);
	}

	void Draw(HDC _hdc,HDC normalHDC)
	{
		switch(state)
		{
		case SEX:
			{
				UC_Sex.Draw(_hdc);
				break;
			}

		case STYLE:
			{
				UC_Style.Draw(_hdc);
				break;
			}
		case STYLE2:
			{
				UC_Style2.Draw(_hdc);
				break;
			}
		case CAMERA:
			{
				UC_Camera.Draw(_hdc);

				DrawToHDC_mat(mat,_hdc,&cameraRect);

				break;
			}
		case DETECTION:

			UC_Detection.Draw(_hdc);			
			mat_cv.DrawToHDC(_hdc,&cameraRect);

			break;
		case RESULT:

			{
				UC_Result.Draw(_hdc);
				result_cv.DrawToHDC(_hdc,&cameraRect_dst);

				break;
			}
		case EMAIL:
			{
				ExcludeClipRect(normalHDC,emailBox.left,emailBox.top,emailBox.right,emailBox.bottom);///////////
				ExcludeClipRect(_hdc,emailBox.left,emailBox.top,emailBox.right,emailBox.bottom);///////////

				UC_Email.Draw(_hdc);
				break;
			}

		}
	}


	void SetMouseState(int _x,int _y,bool _bNowDraw)
	{
		int buttonPress=0;
		//cout<<"CurrentState :"<<this->state<<endl;
		switch(state)
		{
		case SEX:
			{
				UC_Sex.SetMouseState(_x,_y,_bNowDraw,buttonPress);
				if(buttonPress==NEXT_SEX_W)
				{
					this->state=STYLE;
				}
				else if(buttonPress==NEXT_SEX_M)
				{
					this->state=STYLE2;
				}
				break;
			}
		case STYLE:
			{
				UC_Style.SetMouseState(_x,_y,_bNowDraw,buttonPress);
				if(buttonPress==NEXT_STYLE1_W)
				{
					this->styleChoice=STYLE_1_W;
					this->state=CAMERA;
				}
				else if(buttonPress==NEXT_STYLE2_W)
				{
					this->styleChoice=STYLE_2_W;
					this->state=CAMERA;
				}
				else if(buttonPress==NEXT_STYLE3_W)
				{
					this->styleChoice=STYLE_3_W;
					this->state=CAMERA;
				}
				break;
			}
		case STYLE2:
			{
				UC_Style2.SetMouseState(_x,_y,_bNowDraw,buttonPress);
				if(buttonPress==NEXT_STYLE1_M)
				{
					this->styleChoice=STYLE_1_M;
					this->state=CAMERA;
				}
				else if(buttonPress==NEXT_STYLE2_M)
				{
					this->styleChoice=STYLE_2_M;
					this->state=CAMERA;
				}

				break;
			}

		case CAMERA:

			UC_Camera.SetMouseState(_x,_y,_bNowDraw,buttonPress);
			if(buttonPress==NEXT_CAMERA)
			{
				cout<<"Click NextCamera Button"<<endl;
				this->state=DETECTION;


				mat.convertTo(mat,CV_8UC3);
				//카메라작동시 적용
				//imwrite("sis.jpg", mat);

				mat=imread("sis.jpg",-1);
				cout<<mat.cols<<','<<mat.rows<<endl;

#define RESIZE_VALUE 2
				cv::resize(mat,mat,cv::Size(mat.cols*RESIZE_VALUE,mat.rows*RESIZE_VALUE),0,0,CV_INTER_NN);

				imwrite("sis2.jpg", mat);

				switch(libChoice)
				{
				case DLIB:
					GetDetectionResult_dlib(mat);
					if(isFindFace==false)  //Dlib실패시 LUXAND로 자동바꿔줌
					{
						GetDetectionResult(mat);
						this->libChoice=LUXAND;
					}
					break;
				case LUXAND:

					GetDetectionResult(mat);
					if(isFindFace==false) 
					{
						detectionFail->drawToggle=true;
						detectionOK->drawToggle=false;

			}
					break;

				}

				cv::resize(mat,mat,cv::Size(mat.cols/2,mat.rows/2),0,0,CV_INTER_NN);
				ConvertMatToCvImg(mat,mat_cv);

			}
			break;		
		case DETECTION:
			UC_Detection.SetMouseState(_x,_y,_bNowDraw,buttonPress);
			if(buttonPress==NEXT_DETECTION)
			{
				if(isFindFace==false)
				{
					cout<<"Face is not detected"<<endl;
					break;
				}
				this->state=RESULT;
				detectionFail->drawToggle=false;
				detectionOK->drawToggle=true;

				Converter.ConvertStart(&result_mat,this->styleChoice,libChoice);
				result_mat.convertTo(result_mat,CV_8UC3);

				cv::resize(result_mat,result_mat,cv::Size(result_mat.cols/2,result_mat.rows/2),0,0,CV_INTER_NN);
				imwrite("result_conversion.jpg", result_mat);

				result_mat=imread("result_conversion.jpg",-1);


				ConvertMatToCvImg(result_mat,result_cv);

				cameraRect_dst.left=773;
				cameraRect_dst.top=273-100;
				cameraRect_dst.right= 1203;
				cameraRect_dst.bottom= 799-100;
				ControlSize(cameraRect_dst);

				cout<<"ResultCamera:"<<result_mat.size().width<<','<<result_mat.size().height<<endl;


				RECT cameraRect2;
				cameraRect2.left=400;
				cameraRect2.top=100;
				cameraRect2.right=700;
				cameraRect2.bottom=500;
				//UC_Result.InputPartImage(cameraRect2,L"sis.jpg");

			}
			else if(buttonPress==BACK_CAMERA)
			{
				this->state=CAMERA;
				cout<<"gotoBack"<<endl;
				detectionFail->drawToggle=false;
				detectionOK->drawToggle=true;

			}

			break;
		case RESULT:


			UC_Result.SetMouseState(_x,_y,_bNowDraw,buttonPress);
			if(buttonPress==NEXT_RESULT)
			{
				this->state=SEX;
			}
			else if(buttonPress==NEXT_RESULT2)
			{
				this->state=EMAIL;

				ShowWindow(hWndGetEmail,SW_SHOW);

			}
			break;
		case EMAIL:
			{
				UC_Email.SetMouseState(_x,_y,_bNowDraw,buttonPress);
				if(buttonPress==NEXT_EMAIL)
				{
					TCHAR str[128];
					SendMessage(hWndGetEmail, WM_GETTEXT, (WPARAM)128, (LPARAM)str);
std:String emailAddress=str;
					if(EmailSender.SendEmail(emailAddress))
					{
						SendMessage(hWndGetEmail, WM_SETTEXT, 0, (LPARAM)(""));
						emailOK->drawToggle=true;
						emailFail->drawToggle=false;
						this->state=SEX;
						ShowWindow(hWndGetEmail,SW_HIDE);
					}
					else
					{
						emailOK->drawToggle=false;
						emailFail->drawToggle=true;
						cout<<"WrongMail Or Fail"<<endl;

						break;
					}



				}
				else if(buttonPress==BACK_EMAIL)
				{
					SendMessage(hWndGetEmail, WM_SETTEXT, 0, (LPARAM)(""));
					ShowWindow(hWndGetEmail,SW_HIDE);


					emailOK->drawToggle=true;
					emailFail->drawToggle=false;
					this->state=RESULT;
				}
			}
			break;

		}
	}

};


//OpenCv Mat형식을 HDC로 바꾸어 그려준다.
void DrawToHDC_mat(Mat &mat,HDC _hdc,RECT *cameraRect)
{
	IplImage *trans=&IplImage(mat);
	CvvImage cvImg;
	cvImg.CopyOf(trans,1);
	cvImg.DrawToHDC(_hdc,cameraRect);
}

//Mat을 cvImg형식으로 바꾸어준다.
void ConvertMatToCvImg(Mat &mat, CvvImage &cvImg)
{
	IplImage *trans=&IplImage(mat);
	cvImg.CopyOf(trans,1);
}



#endif