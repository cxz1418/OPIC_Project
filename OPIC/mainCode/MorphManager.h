#ifndef MORPHMANAGERVER1
#define MORPHMANAGERVER1



#include"opencv2\opencv.hpp"
#include<iostream>
using namespace std;
using namespace cv;
#include"imageAffineTr.h"
#include"ptManager.h"
#include"lineIterator_png.h"
#include"68landmarkLoader.h"
#include"MenualParameter.h"
#include"DrawBackPic.h"

#define DEFAULT_NUM 10
#define EYEBALL_SIZE 0.325f



PointManager beizerPoints(3);
PointManager beizerTest(3);

void onMouse(int event, int x, int y, int flags, void *param)
{
	Mat * _param=(Mat*)param;
	Mat dummy=Mat(*_param).clone();
	
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		if (flags & EVENT_FLAG_SHIFTKEY){
			beizerPoints.generatePoints(x, y);
			beizerPoints.remapPoints(DEFAULT_NUM);
			//beizerPoints.genBeizerPoints(DEFAULT_NUM);
			beizerPoints.FindWarpHeights(*_param);
		}
		else
		{
			if (beizerPoints.findTogglePoint(x, y))
				beizerPoints.ToggleOn();
		}
		break;

	case EVENT_MOUSEMOVE:
		if (beizerPoints.toggle == true)
		{
			beizerPoints.MovePointsTo(x, y);
			beizerPoints.remapPoints(DEFAULT_NUM);
			//beizerPoints.genBeizerPoints(DEFAULT_NUM);
			beizerPoints.FindWarpHeights(*_param);
		}
		break;

	case EVENT_LBUTTONUP:
		beizerPoints.ToggleOff();

		break;

	case EVENT_RBUTTONDOWN:
		
		break;


	}
	beizerPoints.drawPoints(dummy);
	beizerPoints.drawBeizerLine(dummy);
	imshow("partImage",dummy);

}


void onChange(int pos,void *param)
{

	Mat * _param=(Mat*)param;
	Mat dummy=Mat(*_param).clone();

//	cout<<beizerPoints.keyPoints[0]<<','<<beizerPoints.keyPoints[1]<<endl;
	beizerPoints.drawPoints(dummy);
	beizerPoints.drawBeizerLine(dummy);
	imshow("partImage",dummy);
}


////////////////////////////////////////


enum{STYLE_1_W,STYLE_2_W,STYLE_3_W,STYLE_1_M,STYLE_2_M};
enum{DLIB,LUXAND};
class MorphManager
{
private:

	Point *land_points;


public:
	//이부분의 최종 목적은 Name과 Part를 채우기만하면된다
	//이목적만 이루면 다른함수도 상관없다.

	void SetLandPoints(Point *points)
	{
		this->land_points=points;
	}

	void ConvertStart_comp(Mat *result,int detectionLib)
	{
		int landMarkNum;
		Point keyLeft_p,keyRight_p;
		Point keyBody_p;
		Point keyLeft,keyRight;

		Point bodyLeft,bodyRight;
		Point eyeballLeft,eyeballRight;

		Point faceBackCenter;
		Point leftEyeCenter;
		Point rightEyeCenter;



		switch(detectionLib)
		{
		case LUXAND:
			{

				landMarkNum=70;
				break;
			}
		case DLIB:
			{			
				landMarkNum=68;
				break;
			}
		}




		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back2.png",IMREAD_UNCHANGED);





		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);

		cout<<"Phase1"<<endl;

		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);


		cout<<"Phase2"<<endl;


		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}


		switch(detectionLib)
		{
		case LUXAND:
			{
				keyLeft_p=Point(83,278);
				keyRight_p=Point(229,248);
				keyBody_p=Point(186,40);

				keyLeft=land_points[66];
				keyRight=land_points[67];

				bodyLeft=land_points[7];
				bodyRight=land_points[8];

				eyeballLeft=land_points[23];
				eyeballRight=land_points[24];

				faceBackCenter=land_points[22];

				leftEyeCenter=land_points[0];
				rightEyeCenter=land_points[1];

				break;
			}
		case DLIB:
			{			
				keyLeft_p=Point(78,245);
				keyRight_p=Point(225,228);
				keyBody_p=Point(186,40);

				keyLeft=land_points[0];
				keyRight=land_points[16];

				bodyLeft=land_points[5];
				bodyRight=land_points[11];

				eyeballLeft=land_points[36];
				eyeballRight=land_points[39];

				faceBackCenter=land_points[30];

				leftEyeCenter=land_points[36]+land_points[39];
				leftEyeCenter.x/=2; leftEyeCenter.y/=2;

				rightEyeCenter=land_points[42]+land_points[45];
				rightEyeCenter.x/=2; rightEyeCenter.y/=2;

				break;
			}
		}





		//1.8
		float body_size=10.15+(float)(distance33(keyLeft,keyRight)-1108) * (6.0f/669.0f);
		float eyeball_size=0.68+(float)(distance33(eyeballLeft,eyeballRight)-213) * (0.456f/134.0f);
		//body_size=10.0;


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head2.png",keyLeft_p,keyRight_p);
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(keyLeft,keyRight);
		hdh.DrawH(k);


		Point bodyCenter =bodyLeft+bodyRight;
		bodyCenter.x/=2;
		bodyCenter.y/=2;

		BodyDrawHelper bdh("Body2.png",keyBody_p);
		bdh.MatchPointsToTarget(bodyCenter,body_size);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////

		cout<<"Phase3"<<endl;


		vector<partInf*> * partInfs;
		switch(detectionLib)
		{	
		case LUXAND:
			partInfs=GetMenualPartInfs_70();
			break;
		case DLIB:
			partInfs=GetMenualPartInfs_68();
			break;

		}
		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE2");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack2.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,faceBackCenter,EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,leftEyeCenter,eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,rightEyeCenter,eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE2");
			fromMat2=imread("Pointdata/STYLE2/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}



		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}



		*result=k.clone();
		delete land_points;
		//waitKey();
	}

	void ConvertStart3(Mat *result,int detectionLib)
	{

		int minX=9999,minY=9999;
		int maxX=0,maxY=0;

		int landMarkNum=70;

		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back2.png",IMREAD_UNCHANGED);





		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);


		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);



		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}








		float body_size=8.5+(float)(distance33(land_points[66],land_points[67])-1108) * (6.0f/669.0f);
		float eyeball_size=0.68+(float)(distance33(land_points[23],land_points[24])-213) * (0.456f/134.0f);
		//body_size=10.0;






		Point bodyCenter =land_points[7]+land_points[8];
		bodyCenter.x/=2;
		bodyCenter.y/=2;
		

		BodyDrawHelper bdh("Body3.png",Point(191,119));
		bdh.MatchPointsToTarget(bodyCenter,body_size);

		bdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////




		vector<partInf*> * partInfs=GetMenualPartInfs_70();

		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE3");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack2.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,land_points[22],EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,land_points[0],eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,land_points[1],eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE3");
			fromMat2=imread("Pointdata/STYLE3/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head3.png",Point(186,279),Point(324,279));
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(land_points[66],land_points[67]);
		hdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		hdh.DrawH(k);

		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}

		RECT profitRect=GetSquareWithMinMax(minX,minY,maxX,maxY);	
		k=k(cv::Rect(profitRect.left,profitRect.top,profitRect.right-profitRect.left,profitRect.bottom-profitRect.top));
		

#define BIAS_SIZE 10/10
		cv::resize(k,k,cv::Size(1024*BIAS_SIZE,1024),0,0,CV_INTER_NN);
			
		
		cout<<"K:"<<k.size().width<<','<<k.size().height<<endl;
		Mat logo =imread("logo.png",-1);
		Point lt1(599,367);
		overlayImage(&k,&logo,lt1);

		*result=k.clone();
		delete land_points;
		//waitKey();
	}
	void ConvertStart2(Mat *result,int detectionLib)
	{
		int minX=9999,minY=9999;
		int maxX=0,maxY=0;
		int landMarkNum=70;

		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back2.png",IMREAD_UNCHANGED);





		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);


		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);



		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}








		float body_size=8.15+(float)(distance33(land_points[66],land_points[67])-1108) * (6.0f/669.0f);
		float eyeball_size=0.68+(float)(distance33(land_points[23],land_points[24])-213) * (0.456f/134.0f);
		//body_size=10.0;


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head2.png",Point(83,278),Point(229,248));
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(land_points[66],land_points[67]);
		hdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		hdh.DrawH(k);

		cout<<"Current FaceWidth:"<<distance33(land_points[66],land_points[67])<<endl;
		cout<<"Current bodyScale :"<<body_size<<endl;

		cout<<"Current EyeWidth :"<<distance33(land_points[23],land_points[24])<<endl;
		cout<<"Current eyeScale :"<<  eyeball_size<<endl;


		Point bodyCenter =land_points[7]+land_points[8];
		bodyCenter.x/=2;
		bodyCenter.y/=2;

		BodyDrawHelper bdh("Body2.png",Point(186,40));
		bdh.MatchPointsToTarget(bodyCenter,body_size);
		bdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////




		vector<partInf*> * partInfs=GetMenualPartInfs_70();

		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE2");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack2.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,land_points[22],EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,land_points[0],eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,land_points[1],eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE2");
			fromMat2=imread("Pointdata/STYLE2/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}



		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}

			RECT profitRect=GetSquareWithMinMax(minX,minY,maxX,maxY);	
		k=k(cv::Rect(profitRect.left,profitRect.top,profitRect.right-profitRect.left,profitRect.bottom-profitRect.top));
		

#define BIAS_SIZE 10/10
		cv::resize(k,k,cv::Size(1024*BIAS_SIZE,1024),0,0,CV_INTER_NN);
			
		
		cout<<"K:"<<k.size().width<<','<<k.size().height<<endl;
		Mat logo =imread("logo.png",-1);
		Point lt1(599,367);
		overlayImage(&k,&logo,lt1);

		*result=k.clone();
		delete land_points;
		//waitKey();
	}
	void ConvertStart1(Mat *result,int detectionLib)
	{

		int minX=9999,minY=9999;
		int maxX=0,maxY=0;
		int landMarkNum=70;


		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back.png",IMREAD_UNCHANGED);





		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);


		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);



		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}


		float body_size=8.15+(float)(distance33(land_points[66],land_points[67])-1108) * (6.0f/669.0f);
		float eyeball_size=0.68+(float)(distance33(land_points[23],land_points[24])-213) * (0.456f/134.0f);
		//body_size=10.0;


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head.png",Point(67,169	),Point(174,145));
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(land_points[66],land_points[67]);
		hdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		hdh.DrawH(k);

		cout<<"Current FaceWidth:"<<distance33(land_points[66],land_points[67])<<endl;
		cout<<"Current bodyScale :"<<body_size<<endl;

		cout<<"Current EyeWidth :"<<distance33(land_points[23],land_points[24])<<endl;
		cout<<"Current eyeScale :"<<  eyeball_size<<endl;


		Point bodyCenter =land_points[7]+land_points[8];
		bodyCenter.x/=2;
		bodyCenter.y/=2;
		bodyCenter.y-=15;

		BodyDrawHelper bdh("Body.png",Point(157,17));
		bdh.MatchPointsToTarget(bodyCenter,body_size);
		bdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////




		vector<partInf*> * partInfs=GetMenualPartInfs_70();

		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE1");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,land_points[22],EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,land_points[0],eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,land_points[1],eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE1");
			fromMat2=imread("Pointdata/STYLE1/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}



		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}
		RECT profitRect=GetSquareWithMinMax(minX,minY,maxX,maxY);	
		k=k(cv::Rect(profitRect.left,profitRect.top,profitRect.right-profitRect.left,profitRect.bottom-profitRect.top));
		

#define BIAS_SIZE 10/10
		cv::resize(k,k,cv::Size(1024*BIAS_SIZE,1024),0,0,CV_INTER_NN);
			
		
		cout<<"K:"<<k.size().width<<','<<k.size().height<<endl;
		Mat logo =imread("logo.png",-1);
		Point lt1(599,367);
		overlayImage(&k,&logo,lt1);

		*result=k.clone();
		delete land_points;
		//waitKey();
	}


	//여자현상범
	void ConvertStart4(Mat *result,int detectionLib)
	{
		cout<<"Start Convert4"<<endl;

		int minX=9999,minY=9999;
		int maxX=0,maxY=0;
		int landMarkNum=70;


		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back4.png",IMREAD_UNCHANGED);

		cout<<"Phase1"<<endl;



		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);


		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);



		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}


		float body_size=1.0+(float)(distance33(land_points[66],land_points[67])-1108) * (6.0f/669.0f);
		if(body_size<0)
			body_size=1.1;
		float eyeball_size=0.68+(float)(distance33(land_points[23],land_points[24])-213) * (0.456f/134.0f);
		//body_size=10.0;

		cout<<"Phase2"<<endl;


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head4.png",Point(72,357	),Point(403,357));
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(land_points[66],land_points[67]);
		hdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		hdh.DrawH(k);

		cout<<"Current FaceWidth:"<<distance33(land_points[66],land_points[67])<<endl;
		cout<<"Current bodyScale :"<<body_size<<endl;

		cout<<"Current EyeWidth :"<<distance33(land_points[23],land_points[24])<<endl;
		cout<<"Current eyeScale :"<<  eyeball_size<<endl;


		Point bodyCenter =land_points[7]+land_points[8];
		bodyCenter.x/=2;
		bodyCenter.y/=2;
		bodyCenter.y-=15;

		BodyDrawHelper bdh("Body4.png",Point(201,55));
		bdh.MatchPointsToTarget(bodyCenter,body_size);
		bdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////


cout<<"BEFORE SUCCESS!:"<<endl;

		vector<partInf*> * partInfs=GetMenualPartInfs_70_wanted();

		cout<<"GETPARTINFO SUCCESS!:"<<endl;

		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE4");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack4.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,land_points[22],EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,land_points[0],eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,land_points[1],eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE4");
			fromMat2=imread("Pointdata/STYLE4/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}



		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}
		RECT profitRect=GetSquareWithMinMax(minX,minY,maxX,maxY);	
		k=k(cv::Rect(profitRect.left,profitRect.top,profitRect.right-profitRect.left,profitRect.bottom-profitRect.top));
		

#define BIAS_SIZE 10/10
		cv::resize(k,k,cv::Size(1024*BIAS_SIZE,1024),0,0,CV_INTER_NN);
			
		
		/*
		Mat logo2=imread("logo.png",-1);
		Point lt2(599,367);
		overlayImage(&k,&logo2,lt2);
		*/

		Mat logo =imread("wanted3.png",-1);
		cv::resize(logo,logo,cv::Size(860,1020),0,0,CV_INTER_NN);
		Point lt1(1,1);
		overlayImage(&k,&logo,lt1);


		*result=k.clone();
		delete land_points;
		//waitKey();
	}

	//남자현상범
	void ConvertStart5(Mat *result,int detectionLib)
	{
		cout<<"Start Convert4"<<endl;

		int minX=9999,minY=9999;
		int maxX=0,maxY=0;
		int landMarkNum=70;


		Mat r = imread("sis2.jpg",IMREAD_UNCHANGED);
		Mat k = imread("faceData/back4.png",IMREAD_UNCHANGED);

		cout<<"Phase1"<<endl;



		cv::resize(k,k,Size(r.cols*4,r.rows*4),0,0,CV_INTER_NN);


		Point avgPoint(0,0);
		for(int i=0;i<landMarkNum; i++)
		{
			avgPoint+=land_points[i];
		}
		avgPoint.x/=landMarkNum;
		avgPoint.y/=landMarkNum;
		avgPoint.y+=300;
		cv::Point transPt(k.cols/2-avgPoint.x,k.rows/2-avgPoint.y);



		for(int i=0; i<landMarkNum; i++)
		{
			land_points[i]+=transPt;

		}


		float body_size=2.0+(float)(distance33(land_points[66],land_points[67])-1108) * (6.0f/669.0f);
		if(body_size<0)
			body_size=1.2;
		float eyeball_size=0.68+(float)(distance33(land_points[23],land_points[24])-213) * (0.456f/134.0f);
		//body_size=10.0;

		cout<<"Phase2"<<endl;


		////////////Draw Head///////////////////////////
		HeadDrawHelper hdh("Head5.png",Point(55,382	),Point(323,372));
		//hdh.MatchPointsToTarget(cv::Point(100,100),cv::Point(101,100));
		hdh.MatchPointsToTarget(land_points[66],land_points[67]);
		hdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		hdh.DrawH(k);

		cout<<"Current FaceWidth:"<<distance33(land_points[66],land_points[67])<<endl;
		cout<<"Current bodyScale :"<<body_size<<endl;

		cout<<"Current EyeWidth :"<<distance33(land_points[23],land_points[24])<<endl;
		cout<<"Current eyeScale :"<<  eyeball_size<<endl;


		Point bodyCenter =land_points[7]+land_points[8];
		bodyCenter.x/=2;
		bodyCenter.y/=2;
		bodyCenter.y-=15;

		BodyDrawHelper bdh("Body4.png",Point(201,55));
		bdh.MatchPointsToTarget(bodyCenter,body_size);
		bdh.GetMinMaxXY_dst(minX,minY,maxX,maxY);
		bdh.DrawH(k);

		//////////////////////////////////////////////////////////


cout<<"BEFORE SUCCESS!:"<<endl;

		vector<partInf*> * partInfs=GetMenualPartInfs_70_wanted();

		cout<<"GETPARTINFO SUCCESS!:"<<endl;

		PointManager from;					
		Point *ptss;
		float hrRate ;
		float hlRate ;


		int path_num=0;

		Mat fromMat2;





		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 입력을 시작합니다"<<endl<<endl;

			Point dummyPtArr[100];
			for(int j=0; j<(*partInfs)[i]->allcNums.size(); j++)
			{
				dummyPtArr[j]=land_points[(*partInfs)[i]->allcNums[j]];
			}

			from.FileIn_style((*partInfs)[i]->path,"STYLE4");
			ptss=(*partInfs)[i]->ptManager.GetAverageControlPoint(dummyPtArr,(*partInfs)[i]->allcNums.size());

			(*partInfs)[i]->ptManager.generatePoints(ptss[0].x,ptss[0].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[1].x,ptss[1].y);
			(*partInfs)[i]->ptManager.generatePoints(ptss[2].x,ptss[2].y);

			hrRate = (*partInfs)[i]->volumeBias*(float)from.heightRight/from.getCurveLength();
			hlRate = (*partInfs)[i]->volumeBias*(float)from.heightLeft/from.getCurveLength();


			if((*partInfs)[i]->PartName==RIGHT_EYE ||(*partInfs)[i]->PartName==LEFT_EYE )
			{(*partInfs)[i]->ptManager.genBeizerPointsFromRate2(from.heightRight  *0.4,from.heightLeft  *0.4   ,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);

			}
			else
				(*partInfs)[i]->ptManager.genBeizerPointsFromRate(hrRate,hlRate,from.keyPoints[0],from.segmentNum-(from.keyPoints[0]+from.keyPoints[1]),from.keyPoints[1]);


			(*partInfs)[i]->ptManager.drawBeizerLine(r);

			from.ClearAll();

		}
		//////////////BiasSetting/////////////////
		STFromPartName(partInfs,RIGHT_EYE,1.05,Point(0,0));
		STFromPartName(partInfs,LEFT_EYE,1.01,Point(0,0));

		STFromPartName(partInfs,NOSE,0.8,Point(0,0));
		STFromPartName(partInfs,NOSE_UP,1.0,Point(-20,-10));


		STFromPartName(partInfs,RIGHT_EYEBLOW,1.0,Point(0,0));
		STFromPartName(partInfs,LEFT_EYEBLOW,1.0,Point(0,0));



		/////////////////////////////////
		/////////////////////
		//%/
		vector<Point> *convexEyePoints;

		convexEyePoints = new vector<Point>;

		vector<Point> *total=new vector<Point>;

		vector<Point> *leftEyeblow=GetPointFromPartName_back(partInfs,LEFT_EYEBLOW);
		vector<Point> *rightEyeblow=GetPointFromPartName_back(partInfs,RIGHT_EYEBLOW);
		vector<Point> *tow=GetPointFromPartName(partInfs,TOW);

		for(int a=0; a<rightEyeblow->size(); a++)
			total->push_back((*rightEyeblow)[a]);
		for(int a=0; a<leftEyeblow->size(); a++)
			total->push_back((*leftEyeblow)[a]);
		for(int a=0; a<tow->size(); a++)
			total->push_back((*tow)[a]);

		Mat faceBack=imread("faceBack4.png",-1);

		DrawInConvexPoly2(faceBack,k,*total,land_points[22],EYEBALL_SIZE*50);

		convexEyePoints->clear();



		//%/
		//////////////////////////


		Mat eyeLeft = imread("PointData/left_eyeball.PNG",IMREAD_UNCHANGED);
		Mat eyeRight = imread("PointData/right_eyeball.PNG",IMREAD_UNCHANGED);



		convexEyePoints=GetPointFromPartName(partInfs,LEFT_EYE);
		//DrawInConvexPoly(eyeLeft,k,*convexEyePoints,Point(0,-5),EYEBALL_SIZE);
		DrawInConvexPoly2(eyeLeft,k,*convexEyePoints,land_points[0],eyeball_size);

		delete convexEyePoints;


		///////////////////
		convexEyePoints=GetPointFromPartName(partInfs,RIGHT_EYE);
		DrawInConvexPoly2(eyeRight,k,*convexEyePoints,land_points[1],eyeball_size);
		delete convexEyePoints;






		for(int i=0; i<partInfs->size(); i++)
		{
			//cout<<endl<<endl<<(*partInfs)[i]->path<<"의 드로잉을 시작합니다"<<endl<<endl;

			from.FileIn_style((*partInfs)[i]->path,"STYLE4");
			fromMat2=imread("Pointdata/STYLE4/"+(*partInfs)[i]->path+".png",IMREAD_UNCHANGED);


			DrawWarpTriangleWithBias(fromMat2,from,k,(*partInfs)[i]->ptManager,(*partInfs)[i]->bias,(*partInfs)[i]->scale);


		}



		for(int i=0; i<partInfs->size(); i++)
		{
			(*partInfs)[i]->ptManager.ClearAll();
		}
		RECT profitRect=GetSquareWithMinMax(minX,minY,maxX,maxY);	
		k=k(cv::Rect(profitRect.left,profitRect.top,profitRect.right-profitRect.left,profitRect.bottom-profitRect.top));
		

#define BIAS_SIZE 10/10
		cv::resize(k,k,cv::Size(1024*BIAS_SIZE,1024),0,0,CV_INTER_NN);
			
	/*	
		Mat logo2=imread("logo.png",-1);
		Point lt2(599,367);
		overlayImage(&k,&logo2,lt2);
		*/

		Mat logo =imread("wanted3.png",-1);
		cv::resize(logo,logo,cv::Size(860,1020),0,0,CV_INTER_NN);
		Point lt1(1,1);
		overlayImage(&k,&logo,lt1);


		*result=k.clone();
		delete land_points;
		//waitKey();
	}


	void ConvertStart(Mat *mat,int style,int detectionLib)//1~68개의 랜드마크로 구성된 파트를 나누어 Morph파트를 생성한다.
	{
		//ConvertStart_comp(mat,detectionLib);
		
		switch(style)
		{
		case STYLE_1_W:
			ConvertStart4(mat,detectionLib);
			break;
		case STYLE_2_W:
			ConvertStart2(mat,detectionLib);
			break;
		case STYLE_3_W:
			ConvertStart1(mat,detectionLib);//
			break;
		case STYLE_1_M:
			ConvertStart5(mat,detectionLib);//
			break;
		case STYLE_2_M:
			ConvertStart3(mat,detectionLib);//
			break;
		default :
			cout<<"Critical Error Occured!"<<endl;
			
		}
		
	}



	
};




#endif 