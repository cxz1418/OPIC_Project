#ifndef DRAW_BACK_PIC
#define DRAW_BACK_PIC
#include<iostream>
#include "opencv2\opencv.hpp"
#include "imageAffineTr.h"
#include <math.h>


using namespace cv;
using namespace std;



//반드시 PNG파일이어야한다.
class HeadDrawHelper
{
public:
	Mat src;
	
	vector<cv::Point> points_src;
	vector<cv::Point> points_dst;

	
public :
	HeadDrawHelper(String path,cv::Point keyLeft,cv::Point keyRight)
	{
		SetMat(path);
		SetPoints(keyLeft,keyRight);
	}

	void MatchPointsToTarget(cv::Point keyLeft,cv::Point keyRight)
	{
		SetKeyLeftToCenter();
		SetPointsByTarget(keyLeft,keyRight);
	}


	void DrawH(Mat &dst)
	{
		cout<<"DrawStart"<<endl;

		// Create an instance of Subdiv2D
		/*
		2 0 4
		2 0 3
		0 1 3
		1 3 5
		0 1 5
		0 4 5

		*/
		int arrData[6][3]={{2,0,4},{2,0,3},{0,1,3},{1,3,5},{0,1,5},{0,4,5}};
		//vector<Point2f> t1, t2, t;
        
		vector<cv::Point2f> pt_src,pt_dst;
		cv::Size size = src.size();
		cv::Rect rect(0,0, size.width, size.height);

		for( int i = 0; i < 6; i++ )
		{
			for(int j=0; j<3; j++)
			{
				pt_src.push_back(points_src[arrData[i][j]]);
				pt_dst.push_back(points_dst[arrData[i][j]]);
				
			}
			warpTriangle(src,dst,pt_src,pt_dst);
			pt_src.clear();
			pt_dst.clear();
		}

	}


		void GetMinMaxXY_dst(int &minX,int &minY,int &maxX,int &maxY)
	{
		
		for(int i=0; i<points_dst.size(); i++)
		{
			if(points_dst[i].x<minX)
				minX=points_dst[i].x;

			if(points_dst[i].x>maxX)
				maxX=points_dst[i].x;

			if(points_dst[i].y<minY)
				minY=points_dst[i].y;

			if(points_dst[i].y>maxY)
				maxY=points_dst[i].y;


		}
	}


private :
	void SetMat(String path)
	{
		src = imread(path,-1);
		if(src.empty())
			cout<<"MatReadErrorOccured!!"<<endl;
	}
	void SetPoints(cv::Point keyLeft,cv::Point keyRight)
	{	

#define OFFSET 1
		cv::Size size = src.size();
		//65 160
		cv::Point customKeyLeft(keyLeft);
		points_src.push_back(customKeyLeft);
		points_dst.push_back(customKeyLeft);
		//172 142
		cv::Point customKeyRight(keyRight);
		points_src.push_back(customKeyRight);
		points_dst.push_back(customKeyRight);
		
		cv::Point leftTop(0+OFFSET,0+OFFSET);
		points_src.push_back(leftTop);
		points_dst.push_back(leftTop);
		
		cv::Point rightTop(size.width-OFFSET,0+OFFSET);
		points_src.push_back(rightTop);
		points_dst.push_back(rightTop);
		
		cv::Point leftBottom(0+OFFSET,size.height-OFFSET);
		points_src.push_back(leftBottom);
		points_dst.push_back(leftBottom);
		

		cv::Point rightBottom(size.width-OFFSET,size.height-OFFSET);
		points_src.push_back(rightBottom);
		points_dst.push_back(rightBottom);
		
		
		


	}


	void SetKeyLeftToCenter()
	{
		cv::Point center= points_src[0];

		for(int i=0; i<points_dst.size(); i++)
		{
			points_dst[i]-=center;
		}


	}

	void SetPointsByTarget(cv::Point keyLeft,cv::Point keyRight)
	{
		cv::Point currenVector = points_dst[1]-points_dst[0];
		cv::Point targetVector = keyRight-keyLeft;

		double headDist=distance33(points_dst[1],points_dst[0]);
		cv::Point headVec=points_dst[1]-points_dst[0];
		double headAngle=atan((double)headVec.y/(double)headVec.x);
		

		double keyDist=distance33(keyRight,keyLeft);
		cv::Point keyVec = keyRight-keyLeft;
		double keyAngle=atan((double)keyVec.y/(double)keyVec.x);
		
		AffineTR2DwithPivot(points_dst,cv::Point(0,0),1.0,-headAngle/PAI_1*180.0,points_dst[0]); //수평맞추고 
		AffineTR2DwithPivot(points_dst,cv::Point(0,0),  keyDist/headDist  ,0,points_dst[0]); //스케일 맞추고
		AffineTR2DwithPivot(points_dst,keyLeft,1.0,keyAngle/PAI_1*180.0,points_dst[0]); //돌리고 이동까지
		


		cout<<"POINTS:"<<points_dst[0]<<','<<points_dst[1]<<endl;
		cout<<"KEYPTS:"<<keyLeft<<','<<keyRight<<endl;

		points_dst[0]=keyLeft;
		points_dst[1]=keyRight;
		
	}

	

};



class BodyDrawHelper
{
public:
	Mat src;
	
	vector<cv::Point> points_src;
	vector<cv::Point> points_dst;

	
public :
	BodyDrawHelper(String path,cv::Point keyCenter)
	{
		SetMat(path);
		SetPoints(keyCenter);
	}

	void MatchPointsToTarget(cv::Point keyCenter,float scale)
	{
		SetKeyToCenter();
		SetPointsByTarget(keyCenter,scale);
	}

	void GetMinMaxXY_dst(int &minX,int &minY,int &maxX,int &maxY)
	{
		
		for(int i=0; i<points_dst.size(); i++)
		{
			if(points_dst[i].x<minX)
				minX=points_dst[i].x;

			if(points_dst[i].x>maxX)
				maxX=points_dst[i].x;

			if(points_dst[i].y<minY)
				minY=points_dst[i].y;

			if(points_dst[i].y>maxY)
				maxY=points_dst[i].y;


		}
	}

	void DrawH(Mat &dst)
	{
		cout<<"DrawStart"<<endl;

		// Create an instance of Subdiv2D
		/*
		3 0 1
		1 0 2
		0 2 4
		3 0 4

		*/
		int arrData[4][3]={{3,0,1},{1,0,2},{0,2,4},{3,0,4}};
		//vector<Point2f> t1, t2, t;
        
		vector<cv::Point2f> pt_src,pt_dst;
		cv::Size size = src.size();
		cv::Rect rect(0,0, size.width, size.height);

		for( int i = 0; i < 4; i++ )
		{
			for(int j=0; j<3; j++)
			{
				pt_src.push_back(points_src[arrData[i][j]]);
				pt_dst.push_back(points_dst[arrData[i][j]]);
				
			}
			warpTriangle(src,dst,pt_src,pt_dst);
			pt_src.clear();
			pt_dst.clear();
		}



	}


private :
	void SetMat(String path)
	{
		src = imread(path,-1);
		if(src.empty())
			cout<<"MatReadErrorOccured!!"<<endl;
	}
	void SetPoints(cv::Point keyCenter)
	{	

#define OFFSET 1
		cv::Size size = src.size();


		points_src.push_back(keyCenter);
		points_dst.push_back(keyCenter);
		
		cv::Point leftTop(0+OFFSET,0+OFFSET);
		points_src.push_back(leftTop);
		points_dst.push_back(leftTop);
		
		cv::Point rightTop(size.width-OFFSET,0+OFFSET);
		points_src.push_back(rightTop);
		points_dst.push_back(rightTop);
		
		cv::Point leftBottom(0+OFFSET,size.height-OFFSET);
		points_src.push_back(leftBottom);
		points_dst.push_back(leftBottom);
		
		cv::Point rightBottom(size.width-OFFSET,size.height-OFFSET);
		points_src.push_back(rightBottom);
		points_dst.push_back(rightBottom);
		
		
		


	}

	void SetKeyToCenter()
	{
		cv::Point center= points_src[0];

		for(int i=0; i<points_dst.size(); i++)
		{
			points_dst[i]-=center;
		}


	}

	void SetPointsByTarget(cv::Point keyCenter,float _scale)
	{

		cv::Point trans = keyCenter-points_dst[0];


		AffineTR2DwithPivot(points_dst,trans,_scale,0.0,points_dst[0]); //이동,스케일
		
		points_dst[0]=keyCenter;
		
	}

	

};

RECT GetSquareWithMinMax(int minX,int minY,int maxX,int maxY)
{
	
	cv::Point center;
	center.x=(minX+maxX)/2;
	center.y=(minY+maxY)/2;

	int width;
	if((maxX-minX)>(maxY-minY))
	{
		width=maxX-minX;
	}
	else
		width=maxY-minY;

	RECT resultRect;
	resultRect.left=center.x-width/2;
	resultRect.top=center.y-width/2;
	resultRect.right=resultRect.left+width;
	resultRect.bottom=resultRect.top+width;

	return resultRect;
}

#endif