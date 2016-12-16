#ifndef PTMANAGER
#define PTMANAGER

#include"68landmarkLoader.h"
#include"opencv2\opencv.hpp"
#include"BeizerLine3d.h"
#include"lineIterator_png.h"
#include<math.h>

using namespace cv;
using namespace std;

#define GROW_DIST 3.0
#define PAI 3.141592
#define MEASURE_SEGMENT 300

double distance33(Point a, Point b)
{
	return sqrt(pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0));

}
//void onMouse(int event, int x, int y, int flags, void *param);
class PointManager
{

public:
	bool toggle;
	int toggleNum;
	
	vector<Point> Points;
	vector<Point> BeizerPoints;
	


	float pointDetectRadius;
	int maxSize;
	float heightRight;
	float heightLeft;
	String filepath;

	int segmentNum;
	int keyPoints[2]; //나누기 연산으로 변환해줘야된다 각 1~50값을 저장하고있고
					//0은 keyPoint[0]/100  1은 1.0-keyPoint[1]/100 의 비율이며 세그먼트에 가장 가까운 인덱스로 지정되겠다.
public:
	PointManager(int _maxSize)
	{
		keyPoints[0]=0;keyPoints[1]=0;

		segmentNum=0;
		heightRight=GROW_DIST;
		heightLeft=GROW_DIST;

		maxSize = _maxSize;
		toggle = false;
		pointDetectRadius = 2.0;
	}
	PointManager()
	{
		keyPoints[0]=0;keyPoints[1]=0;


		segmentNum=0;
		heightRight=GROW_DIST;
		heightLeft=GROW_DIST;

		maxSize = 100;
		toggle = false;
		pointDetectRadius = 5.0;
	}
	
	~PointManager()
	{
		Points.clear();
		BeizerPoints.clear();
		
	}

	void ClearAll()
	{
		Points.clear();
		BeizerPoints.clear();
		heightRight=GROW_DIST;
		heightLeft=GROW_DIST;

		keyPoints[0]=0; keyPoints[1]=0;
		segmentNum=0;
		toggle=false;
	}
	
	void ToggleOn()
	{
		toggle = true;
	}
	void ToggleOff()
	{
		toggle = false;
	}

	bool findTogglePoint(int dx, int dy)
	{
		for (int i = 0; i<Points.size(); i++)
		{
			if (distance33(Point(dx, dy), Points[i]) <= pointDetectRadius)
			{
				toggleNum = i; return true;
			}
		}
		return false;
	}


	void drawPoints(Mat image)
	{
		if (Points.size()<2)
			return;
		
		circle(image, Points[0], pointDetectRadius, Scalar(0, 250, 250),5, 2, 0);

		for (int i = 0; i< Points.size() - 1; i++)
		{
			circle(image, Points[i + 1], pointDetectRadius, Scalar(0, 250, 250), 5, 2,0);
			line(image, Points[i], Points[i + 1], Scalar(100, 100, 200), 1, 1, 0);
		}
	}

    //Warp면적생성
	void FindWarpHeights(Mat image)
	{
		
		

		if(BeizerPoints.size()==0)
			return;

		bool pass=false;
		heightRight=GROW_DIST;
		heightLeft=GROW_DIST;


		while(1)
		{
			Point right=GetRightNormalVec();
			for(int i=0; i<BeizerPoints.size()-1; i++)
			{		
		
				if(!CheckIntersectedPix_PNG(image,BeizerPoints[i]+right,BeizerPoints[i+1]+right,Scalar(255,255,255,0)))
				{			

					break;
				}

				if(i==BeizerPoints.size()-2)
				{
					pass=true;
				}		
			}

			if(pass==true)
			{			
				break;
			}
			else
			{	
				heightRight+=GROW_DIST;
			}
			//this->drawBeizerLine(image);
			//waitKey();

		
		}

		

		pass=false;
		while(1)
		{

			Point left =GetLeftNormalVec();
			for(int i=0; i<BeizerPoints.size()-1; i++)
			{

				if(!CheckIntersectedPix_PNG(image,BeizerPoints[i]+left,BeizerPoints[i+1]+left,Scalar(255,255,255,0)))
				{
					break;
				}
				
				if(i==BeizerPoints.size()-2)
				{
					pass=true;
				}
			}
			if(pass==true)
				break;
			else
				this->heightLeft+=GROW_DIST;

		}
	}

	Point GetRightNormalVec()
	{
		Point dir=Points[2]-Points[0];
		float dirx = heightRight*(float)dir.x/sqrt(pow(dir.x,2.0) + pow(dir.y,2.0));
		float diry = heightRight*(float)dir.y/sqrt(pow(dir.x,2.0) + pow(dir.y,2.0));

		Point2f rightDir; 
		rightDir.x =cos(PAI/2.0)*dirx - sin(PAI/2.0)*diry;
		rightDir.y =sin(PAI/2.0)*dirx + cos(PAI/2.0)*diry;

		return Point((int)rightDir.x,(int)rightDir.y);
	}
	Point GetLeftNormalVec()
	{
		Point dir=Points[2]-Points[0];

		float dirx = heightLeft*(float)dir.x/(float)sqrt(pow((double)dir.x,2.0) + pow((double)dir.y,2.0));
		float diry = heightLeft*(float)dir.y/(float)sqrt(pow((double)dir.x,2.0) + pow((double)dir.y,2.0));

		Point2f leftDir;
		leftDir.x =cos(-PAI/2.0)*dirx - sin(-PAI/2.0)*diry;
		leftDir.y =sin(-PAI/2.0)*dirx + cos(-PAI/2.0)*diry;

		return Point((int)leftDir.x,(int)leftDir.y);
	}


	//매핑되는 사진 베이지어에서 매핑시 사용할 함수로써
	//각 매개변수로 개칭되는 warp영역을 만들어낸다.



	
	void genBeizerPointsFromRate(float rH,float lH,int sRate,int mRate,int eRate)
	{
		

		if(Points.size()<3)
			return;
		if(BeizerPoints.size()!=0)
			BeizerPoints.clear();

		this->segmentNum=sRate+mRate+eRate;

		//비교대상 beizer로 부터 getRightNormal 을 거리값으로 변환하여 보내면
		//여기서 그값을 height 으로 삼고 그뒤 getRightNormal을하면 자동으로 
		//Right 포인트를 얻을수있다
		
		

		//cout<<endl<<"Rate RAte :"<<sRate<<','<<mRate<<','<<eRate<<','<<endl;

		
		//m의길이를 구한다.
		Point pt[3];
		pt[0]=Points[0];pt[1]=Points[1];pt[2]=Points[2];



		float k=0;
		float rate= (1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy;
		
		while(1)
		{
			disDummy.push_back(GetBeizerPoint2d(pt,k));
		
			k+=rate;
			if(k>1.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis(disDummy.size()-1);
		
		dis[0]=Getdistance(disDummy[0],disDummy[1]);
		for(int j=1; j<dis.size(); j++)
		{
			dis[j]=dis[j-1]+Getdistance(disDummy[j],disDummy[j+1]);
			
		}
	
		////////////////////////////베이지어를 잘게 쪼개서 곡선의 길이를 구한다.
		float k2=0;
		float totalDist = dis[dis.size()-1];
		float sDist = totalDist*(float)sRate/(float)(mRate);
		float eDist = totalDist*(float)eRate/(float)(mRate);
	//	cout<<"totalDist:"<<totalDist<<','<<"sDIst:"<<sDist<<','<<"eDist"<<eDist<<endl;
		float rate2=dis[dis.size()-1]/(float)mRate;
		float min=10000;
		int index=0;
		Vector<Point> mBeizerPoints;

		mBeizerPoints.push_back(GetBeizerPoint2d(pt,0));
		k2+=rate2;
		for(int i=0; i<mRate; i++)
		{
			min=10000;
			for(int j=0; j<dis.size(); j++)
			{
				if(abs(dis[j]-k2)<min)
				{
					index=j;
					min=abs(dis[j]-k2);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			mBeizerPoints.push_back(Point(disDummy[index+1]));
			k2+=rate2;
		}
		////sssssss///////////////////////////////s
		float k_s=0;
		float rate_s= -(1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy_s;
		
		while(1)
		{
			disDummy_s.push_back(GetBeizerPoint2d(pt,k_s));
		
			k_s+=rate_s;
			if(k_s<-5.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis_s(disDummy_s.size()-1);
		
		float min_s=9999;
		int index_s_end=0;

		dis_s[0]=Getdistance(disDummy_s[0],disDummy_s[1]);
		for(int j=1; j<dis_s.size(); j++)
		{
			dis_s[j]=dis_s[j-1]+Getdistance(disDummy_s[j],disDummy_s[j+1]);
			if( abs(sDist-dis_s[j]) < min_s)
			{
				min_s=abs(sDist-dis_s[j]);
				index_s_end=j;  //sDist의 거리까지 닿는 점의 인덱스지점
			}
		}
		//cout<<"최종 j = "<<index_s_end<<endl;
		//cout<<disDummy_s[index_s_end]<<endl;;

		float k2_s=0;
		float rate2_s=sDist/(float)sRate;
		min_s=10000;
		int index_s=0;

		Vector<Point> sBeizerPoints;
		sBeizerPoints.push_back(GetBeizerPoint2d(pt,0));
		k2_s+=rate2_s;
		for(int i=0; i<sRate; i++)
		{
			min_s=10000;
			for(int j=0; j<index_s_end; j++)
			{
				if(abs(dis_s[j]-k2_s)<min_s)
				{
					index_s=j;
					min_s=abs(dis_s[j]-k2_s);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			sBeizerPoints.push_back(Point(disDummy_s[index_s+1]));
			k2_s+=rate2_s;
		}				




		////////eee////////////////////////////
		float k_e=1;
		float rate_e= (1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy_e;
		
		while(1)
		{
			disDummy_e.push_back(GetBeizerPoint2d(pt,k_e));
		
			k_e+=rate_e;
			if(k_e>5.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis_e(disDummy_e.size()-1);
		
		float min_e=9999;
		float index_e_end=0;

		dis_e[0]=Getdistance(disDummy_e[0],disDummy_e[1]);
		for(int j=1; j<dis_e.size(); j++)
		{
			dis_e[j]=dis_e[j-1]+Getdistance(disDummy_e[j],disDummy_e[j+1]);
			if( abs(eDist-dis_e[j]) < min_e)
			{
				min_e=abs(eDist-dis_e[j]);
				index_e_end=j;  //sDist의 거리까지 닿는 점의 인덱스지점
			}
		}
	
		float k2_e=0;
		float rate2_e=eDist/(float)eRate;
		min_e=10000;
		int index_e=0;

		Vector<Point> eBeizerPoints;
		eBeizerPoints.push_back(GetBeizerPoint2d(pt,1));
		k2_e+=rate2_e;
		for(int i=0; i<eRate; i++)
		{
			min_e=10000;
			for(int j=0; j<index_e_end; j++)
			{
				if(abs(dis_e[j]-k2_e)<min_e)
				{
					index_e=j;
					min_e=abs(dis_e[j]-k2_e);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			eBeizerPoints.push_back(Point(disDummy_e[index_e+1]));
			k2_e+=rate2_e;
		}				

		///////////////////////////////////////
		int nums=sBeizerPoints.size()-1;
		for(int i=0; i<nums; i++)
		{
			BeizerPoints.push_back(sBeizerPoints.back());
			sBeizerPoints.pop_back();
		}
		for(int i=0; i<mBeizerPoints.size()-1; i++)
		{
			BeizerPoints.push_back(mBeizerPoints[i]);
		}

		for(int i=0; i<eBeizerPoints.size(); i++)
		{
			BeizerPoints.push_back(eBeizerPoints[i]);
		}

	//	cout<<"Total BeizerPoints:"<<BeizerPoints.size()<<endl;

		this->keyPoints[0]=sRate;
		this->keyPoints[1]=eRate;
		//////////////////////////////////////

		this->heightRight=this->getCurveLength()*rH*(this->segmentNum)/mRate;
		this->heightLeft=this->getCurveLength()*lH*(this->segmentNum)/mRate;
		
		//cout<<"------------------------------------------"<<endl;
		//	cout<<"hrRateAfter"<<endl;
		//cout<<getCurveLength()*rH<<','<<getCurveLength()*lH<<endl;
		//cout<<"------------------------------------------"<<endl;
		//sRate mRate eRate 는 각각(remap) 정규화된 값으로 변환된뒤 비율로 보낸것이미로
		//비율값이 거리에 비례한다고 생각하면된다

		//현재 베이지어의 거리를 구하여 m등분한다
	

}


	void genBeizerPointsFromRate2(float rH,float lH,int sRate,int mRate,int eRate)
	{
		

		if(Points.size()<3)
			return;
		if(BeizerPoints.size()!=0)
			BeizerPoints.clear();

		this->segmentNum=sRate+mRate+eRate;

		//비교대상 beizer로 부터 getRightNormal 을 거리값으로 변환하여 보내면
		//여기서 그값을 height 으로 삼고 그뒤 getRightNormal을하면 자동으로 
		//Right 포인트를 얻을수있다
		
		

		//cout<<endl<<"Rate RAte :"<<sRate<<','<<mRate<<','<<eRate<<','<<endl;
//
		
		//m의길이를 구한다.
		Point pt[3];
		pt[0]=Points[0];pt[1]=Points[1];pt[2]=Points[2];



		float k=0;
		float rate= (1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy;
		
		while(1)
		{
			disDummy.push_back(GetBeizerPoint2d(pt,k));
		
			k+=rate;
			if(k>1.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis(disDummy.size()-1);
		
		dis[0]=Getdistance(disDummy[0],disDummy[1]);
		for(int j=1; j<dis.size(); j++)
		{
			dis[j]=dis[j-1]+Getdistance(disDummy[j],disDummy[j+1]);
			
		}
	
		////////////////////////////베이지어를 잘게 쪼개서 곡선의 길이를 구한다.
		float k2=0;
		float totalDist = dis[dis.size()-1];
		float sDist = totalDist*(float)sRate/(float)(mRate);
		float eDist = totalDist*(float)eRate/(float)(mRate);
	//	cout<<"totalDist:"<<totalDist<<','<<"sDIst:"<<sDist<<','<<"eDist"<<eDist<<endl;
		float rate2=dis[dis.size()-1]/(float)mRate;
		float min=10000;
		int index=0;
		Vector<Point> mBeizerPoints;

		mBeizerPoints.push_back(GetBeizerPoint2d(pt,0));
		k2+=rate2;
		for(int i=0; i<mRate; i++)
		{
			min=10000;
			for(int j=0; j<dis.size(); j++)
			{
				if(abs(dis[j]-k2)<min)
				{
					index=j;
					min=abs(dis[j]-k2);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			mBeizerPoints.push_back(Point(disDummy[index+1]));
			k2+=rate2;
		}
		////sssssss///////////////////////////////s
		float k_s=0;
		float rate_s= -(1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy_s;
		
		while(1)
		{
			disDummy_s.push_back(GetBeizerPoint2d(pt,k_s));
		
			k_s+=rate_s;
			if(k_s<-5.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis_s(disDummy_s.size()-1);
		
		float min_s=9999;
		int index_s_end=0;

		dis_s[0]=Getdistance(disDummy_s[0],disDummy_s[1]);
		for(int j=1; j<dis_s.size(); j++)
		{
			dis_s[j]=dis_s[j-1]+Getdistance(disDummy_s[j],disDummy_s[j+1]);
			if( abs(sDist-dis_s[j]) < min_s)
			{
				min_s=abs(sDist-dis_s[j]);
				index_s_end=j;  //sDist의 거리까지 닿는 점의 인덱스지점
			}
		}
	//	cout<<"최종 j = "<<index_s_end<<endl;
	//	cout<<disDummy_s[index_s_end]<<endl;;

		float k2_s=0;
		float rate2_s=sDist/(float)sRate;
		min_s=10000;
		int index_s=0;

		Vector<Point> sBeizerPoints;
		sBeizerPoints.push_back(GetBeizerPoint2d(pt,0));
		k2_s+=rate2_s;
		for(int i=0; i<sRate; i++)
		{
			min_s=10000;
			for(int j=0; j<index_s_end; j++)
			{
				if(abs(dis_s[j]-k2_s)<min_s)
				{
					index_s=j;
					min_s=abs(dis_s[j]-k2_s);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			sBeizerPoints.push_back(Point(disDummy_s[index_s+1]));
			k2_s+=rate2_s;
		}				




		////////eee////////////////////////////
		float k_e=1;
		float rate_e= (1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy_e;
		
		while(1)
		{
			disDummy_e.push_back(GetBeizerPoint2d(pt,k_e));
		
			k_e+=rate_e;
			if(k_e>5.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis_e(disDummy_e.size()-1);
		
		float min_e=9999;
		float index_e_end=0;

		dis_e[0]=Getdistance(disDummy_e[0],disDummy_e[1]);
		for(int j=1; j<dis_e.size(); j++)
		{
			dis_e[j]=dis_e[j-1]+Getdistance(disDummy_e[j],disDummy_e[j+1]);
			if( abs(eDist-dis_e[j]) < min_e)
			{
				min_e=abs(eDist-dis_e[j]);
				index_e_end=j;  //sDist의 거리까지 닿는 점의 인덱스지점
			}
		}
	
		float k2_e=0;
		float rate2_e=eDist/(float)eRate;
		min_e=10000;
		int index_e=0;

		Vector<Point> eBeizerPoints;
		eBeizerPoints.push_back(GetBeizerPoint2d(pt,1));
		k2_e+=rate2_e;
		for(int i=0; i<eRate; i++)
		{
			min_e=10000;
			for(int j=0; j<index_e_end; j++)
			{
				if(abs(dis_e[j]-k2_e)<min_e)
				{
					index_e=j;
					min_e=abs(dis_e[j]-k2_e);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			eBeizerPoints.push_back(Point(disDummy_e[index_e+1]));
			k2_e+=rate2_e;
		}				

		///////////////////////////////////////
		int nums=sBeizerPoints.size()-1;
		for(int i=0; i<nums; i++)
		{
			BeizerPoints.push_back(sBeizerPoints.back());
			sBeizerPoints.pop_back();
		}
		for(int i=0; i<mBeizerPoints.size()-1; i++)
		{
			BeizerPoints.push_back(mBeizerPoints[i]);
		}

		for(int i=0; i<eBeizerPoints.size(); i++)
		{
			BeizerPoints.push_back(eBeizerPoints[i]);
		}

	//	cout<<"Total BeizerPoints:"<<BeizerPoints.size()<<endl;

		this->keyPoints[0]=sRate;
		this->keyPoints[1]=eRate;
		//////////////////////////////////////

		this->heightRight=rH;//this->getCurveLength()*rH*(this->segmentNum)/mRate;
		this->heightLeft=lH;//this->getCurveLength()*lH*(this->segmentNum)/mRate;
		
		

		//cout<<"------------------------------------------"<<endl;
		//	cout<<"hrRateAfter"<<endl;
	//	cout<<getCurveLength()*rH<<','<<getCurveLength()*lH<<endl;
	//	cout<<"------------------------------------------"<<endl;
		//sRate mRate eRate 는 각각(remap) 정규화된 값으로 변환된뒤 비율로 보낸것이미로
		//비율값이 거리에 비례한다고 생각하면된다

		//현재 베이지어의 거리를 구하여 m등분한다
	

}



	float getCurveLength()
	{
		Point pt[3];
		pt[0]=Points[0];pt[1]=Points[1];pt[2]=Points[2];

		float k=0;
		float rate= (1.0f/float(MEASURE_SEGMENT));
		
		Vector<Point> disDummy;
		
		while(1)
		{
			disDummy.push_back(GetBeizerPoint2d(pt,k));
		
			k+=rate;
			if(k>1.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis(disDummy.size()-1);
		
		dis[0]=Getdistance(disDummy[0],disDummy[1]);
		for(int j=1; j<dis.size(); j++)
		{
			dis[j]=dis[j-1]+Getdistance(disDummy[j],disDummy[j+1]);
			
		}
	
		return dis[dis.size()-1];
	}

	void genBeizerPoints(int segmentNum)
	{
		this->segmentNum=segmentNum;
		if(Points.size()<3)
			return;
		if(BeizerPoints.size()!=0)
			BeizerPoints.clear();

		float k=0;
		float rate= (1.0f/float(segmentNum));
		Point pt[3];
		pt[0]=Points[0];pt[1]=Points[1];pt[2]=Points[2];

		
		
		while(1)
		{
			BeizerPoints.push_back(GetBeizerPoint2d(pt,k));
		
			k+=rate;
			if(k>1.000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}

	}

	//비율에따라 거리를 일정하게 바꾼베지어 점들을생성
	void remapPoints(int segmentNum)
	{
		this->segmentNum=segmentNum;
		if(Points.size()<3)
			return;
		if(BeizerPoints.size()!=0)
			BeizerPoints.clear();

		float k=0;
		float rate= (1.0f/float(MEASURE_SEGMENT));
		Point pt[3];
		pt[0]=Points[0];pt[1]=Points[1];pt[2]=Points[2];

		Vector<Point> disDummy;
		
		while(1)
		{
			disDummy.push_back(GetBeizerPoint2d(pt,k));
		
			k+=rate;
			if(k>1.0000001f) //float 은 == 이 통하지않으므로 입실론이나 이런방식
				break;
		}
		Vector<float> dis(disDummy.size()-1);
		
		dis[0]=Getdistance(disDummy[0],disDummy[1]);
		for(int j=1; j<dis.size(); j++)
		{
			dis[j]=dis[j-1]+Getdistance(disDummy[j],disDummy[j+1]);
			
		}
	
		float k2=0;
		float rate2=dis[dis.size()-1]/(float)segmentNum;
		float min=10000;
		int index=0;

		BeizerPoints.push_back(GetBeizerPoint2d(pt,0));
		k2+=rate2;
		for(int i=0; i<segmentNum; i++)
		{
			min=10000;
			for(int j=0; j<dis.size(); j++)
			{
				if(abs(dis[j]-k2)<min)
				{
					index=j;
					min=abs(dis[j]-k2);
				}
			}
			//cout<<Point(disDummy[index+1])<<endl;
			BeizerPoints.push_back(Point(disDummy[index+1]));
			k2+=rate2;
		}

	}
	void drawBeizerLine(Mat image)
	{
		if(BeizerPoints.size()==0)
			return ;
		if(Points.size()<3)
			return;


		Point right=GetRightNormalVec();
		Point left =GetLeftNormalVec();
		

		for(int i=0; i<BeizerPoints.size()-1; i++)
		{
			
			line(image,BeizerPoints[i],BeizerPoints[i+1],Scalar(255,50,0),2,1,0);
			line(image,BeizerPoints[i]+right,BeizerPoints[i+1]+right,Scalar(0,255,0),1,1,0);
			line(image,BeizerPoints[i]+left,BeizerPoints[i+1]+left,Scalar(0,255,0),1,1,0);
			
		}

		circle(image,BeizerPoints[0+(int)keyPoints[0]],3,Scalar(255,0,255),2,1,0);
		circle(image,BeizerPoints[BeizerPoints.size()-1-(int)keyPoints[1]],3,Scalar(255,0,255),2,1,0);
	}
	void generatePoints(int x, int y)
	{
		if (Points.size() >= maxSize)
			return ;
		Points.push_back(Point(x, y)); 
	}//Point생성
	void MovePointsTo(int rx, int ry)
	{
		if (toggle)
		{
			Points[toggleNum].x = rx;
			Points[toggleNum].y = ry;
		}
	}


	//포인트 3개이상의 점들을 받아
	//그점들에 근사하게 맞는 평균 베지어곡선을 찾는다.
	//여기서는 단순히 베지어 곡선 컨트롤의 평균값을 사용했다.
	//정확하지는 않지만 근사한 모양이 나오긴한다.
	Point* GetAverageControlPoint(Point pts[],int size)
	{
		if(size<2)
		{
			return NULL;
		}
		if(size==2)
		{
			
			Point *ptsDummy=new Point[3];
			ptsDummy[0]=pts[0];
			ptsDummy[2]=pts[1];
			ptsDummy[1].x=(pts[0].x+pts[1].x)/2;
			ptsDummy[1].y=(pts[0].y+pts[1].y)/2;
			
			return ptsDummy;
		}

		Point dir=pts[size-1] - pts[0];
		
		Point resultPt(0,0);
		for(int i=0; i<size-2; i++)
		{
			Point u=pts[i+1] - pts[0];
			
			Point proj  = ( PointDot(dir,u)/PointDot(dir,dir) ) * dir ;
			Point orth = u-proj;
			
			resultPt+= pts[i+1]+orth; 
		}
		resultPt.x=resultPt.x/(size-2);
		resultPt.y=resultPt.y/(size-2);

		Point * resultpts=new Point[3];
		resultpts[0]=pts[0];
		resultpts[1]=resultPt;
		resultpts[2]=pts[size-1];
		return resultpts;
	}

	float PointDot(Point p1,Point p2)
	{
		int result = p1.x*p2.x + p1.y*p2.y;
		return result;
	}

	//heightRight
	//heightLeft
	//Points
	//BeizerPoints
	//segmentNum  //segmentNum+1의 BeizerPoint들이 생성
	//keyPoints
	void FileOut(String _fileName,String _filePath)  //fileName의 PointManager값을 저장한다.
	{
		if(Points.size()<3 || BeizerPoints.size()<3)
		{
			cout<<"Need Initialization "<<endl;
			return ;
		}

	//	cout<<"Start FileOutput"<<endl;
		ofstream fout;
		String path("PointData/");
		path=path+_fileName;
		//path=path+"ppts";
		fout.open(path);

		fout<<_filePath<<"\t";

		fout<<heightRight<<"\t"; fout<<heightLeft<<"\t";
		fout<<Points.size()<<"\t";
		for(int i=0; i<Points.size(); i++)
		{
			fout<<Points[i].x<<"\t";  fout<<Points[i].y<<"\t";
		}
		fout<<BeizerPoints.size()<<"\t";
		for(int i=0; i<BeizerPoints.size(); i++)
		{
			fout<<BeizerPoints[i].x<<"\t"; fout<<BeizerPoints[i].y<<"\t";
		}

		fout<<segmentNum<<"\t";
		fout<<keyPoints[0]<<"\t"; fout<<keyPoints[1]<<"\t";

		fout.close();

	}
	void FileIn(String _fileName)  //fileName으로 부터 PointManager값을 받아온다
	{
		

	//	cout<<"파일 입력시작"<<endl;
		ifstream fin;
		String path("PointData/");
		path=path+_fileName;
		fin.open(path);
		if(fin.is_open()==false)
		{
			cout<<"failed to open "+_fileName<<endl;
		}
		this->ClearAll();

		fin>>this->filepath;

		/////
		fin>>heightRight; fin>>heightLeft;
		int PointSize;
		fin>>PointSize;
		for(int i=0; i<PointSize; i++)
		{
			Point dummy;
			fin>>dummy.x;  fin>>dummy.y;
			Points.push_back(dummy);
		}
		int BeizerPointSize;
		fin>>BeizerPointSize;
		for(int i=0; i<BeizerPointSize; i++)
		{
			Point dummy;
			fin>>dummy.x; fin>>dummy.y;
			BeizerPoints.push_back(dummy);
		}

		fin>>segmentNum;
		fin>>keyPoints[0]; fin>>keyPoints[1];

		fin.close();
		//cout<<"파일In종료"<<endl;
	}
	void FileIn_style(String _fileName,String stylePath)  //fileName으로 부터 PointManager값을 받아온다
	{
		

	//	cout<<"파일 입력시작"<<endl;
		ifstream fin;
		String path("PointData/");
		path=path+stylePath+"/"+_fileName;
		fin.open(path);
		if(fin.is_open()==false)
		{
			cout<<"failed to open "+_fileName<<endl;
		}
		this->ClearAll();

		fin>>this->filepath;

		/////
		fin>>heightRight; fin>>heightLeft;
		int PointSize;
		fin>>PointSize;
		for(int i=0; i<PointSize; i++)
		{
			Point dummy;
			fin>>dummy.x;  fin>>dummy.y;
			Points.push_back(dummy);
		}
		int BeizerPointSize;
		fin>>BeizerPointSize;
		for(int i=0; i<BeizerPointSize; i++)
		{
			Point dummy;
			fin>>dummy.x; fin>>dummy.y;
			BeizerPoints.push_back(dummy);
		}

		fin>>segmentNum;
		fin>>keyPoints[0]; fin>>keyPoints[1];

		fin.close();
		//cout<<"파일In종료"<<endl;
	}


	void ShowAllData()
	{
		cout<<endl<<endl;
		cout<<"height :"<<heightRight<<','<<heightLeft<<endl;
		cout<<"PointSize:"<<Points.size()<<endl;
		for(int i=0; i<Points.size(); i++)
			cout<<Points[i]<<endl;

		cout<<"BeizerSize:"<<BeizerPoints.size()<<endl;
		for(int i=0; i<BeizerPoints.size(); i++)
			cout<<BeizerPoints[i]<<endl;
	}


	


void BeizerPoints_middle(vector<Point> &_dPoints)
{
	for(int i=0; i<BeizerPoints.size()-(this->keyPoints[0]+this->keyPoints[1]);i++)
	{
		_dPoints.push_back(BeizerPoints[this->keyPoints[0]+i]);
	}
}

void BeizerPoints_middle_resverse(vector<Point> &_dPoints)
{

	for(int i=0; i<BeizerPoints.size()-(this->keyPoints[0]+this->keyPoints[1]);i++)
	{
		_dPoints.push_back(BeizerPoints[BeizerPoints.size()-this->keyPoints[1]-i-1]);
	}	

}




//임의의 바이어스를 주어야한다. 바이어스에 따라 각 부위는 스케일 , 이동의 변환을 주어야한다
//각 베이지어 포인트들의 평균을 중심으로 스케일
//혹은 전체값에 트랜슬레이션 값을 주어야한다.
};



void DrawWarpTriangle(Mat &fromMat,PointManager fromPM,Mat &toMat,PointManager toPM)
{

	if(fromPM.BeizerPoints.size()!=toPM.BeizerPoints.size())
	{
		cout<<"not match BEizerPoint size"<<endl;
		return;
	}
	
	for (int i = 0; i < fromPM.BeizerPoints.size() - 1; i++)
	{
		
		//cout<<i << "번째스왑시작" << endl;
		vector <Point2f> triIn;
		vector <Point2f> triOut;

		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());
		
		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		


		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetRightNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetRightNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());

		// Warp all pixels inside input triangle to output triangle

		
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		


		//////////////////////////////////////////////////////


		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());
		
		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		

		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetLeftNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetLeftNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());

		// Warp all pixels inside input triangle to output triangle

		
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		







	}
	/////

}

void DrawWarpTriangleWithBias(Mat &fromMat,PointManager &fromPM,Mat &toMat,PointManager &toPM,Point trans,float scale,Point pivot)
{
	

	
	//AffineTR2D(toPM.BeizerPoints,trans,scale,0);
	AffineScaleWithPivot(toPM.BeizerPoints,trans,scale,pivot);



	if(fromPM.BeizerPoints.size()!=toPM.BeizerPoints.size())
	{
		cout<<"not match BEizerPoint size"<<endl;
		return;
	}
	
	for (int i = 0; i < fromPM.BeizerPoints.size() - 1; i++)
	{
		
		//cout<<i << "번째스왑시작" << endl;
		vector <Point2f> triIn;
		vector <Point2f> triOut;

		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());
		
		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		


		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetRightNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetRightNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());

		// Warp all pixels inside input triangle to output triangle

		
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();
		


		//////////////////////////////////////////////////////


		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());

		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();


		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetLeftNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetLeftNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());

		// Warp all pixels inside input triangle to output triangle


		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();








	}
	/////
	AffineTR2D(toPM.BeizerPoints,-trans,1/scale,0);

}

void DrawWarpTriangleWithBias(Mat &fromMat,PointManager &fromPM,Mat &toMat,PointManager &toPM,Point trans,float scale)
{



	AffineTR2D(toPM.BeizerPoints,trans,scale,0);




	if(fromPM.BeizerPoints.size()!=toPM.BeizerPoints.size())
	{
		cout<<"not match BEizerPoint size"<<endl;
		return;
	}

	for (int i = 0; i < fromPM.BeizerPoints.size() - 1; i++)
	{

		//cout<<i << "번째스왑시작" << endl;
		vector <Point2f> triIn;
		vector <Point2f> triOut;

		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());

		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();



		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetRightNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetRightNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetRightNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetRightNormalVec());

		// Warp all pixels inside input triangle to output triangle


		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();



		//////////////////////////////////////////////////////


		triIn.push_back(fromPM.BeizerPoints[i]);
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());

		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i]);
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());

		// Warp all pixels inside input triangle to output triangle
		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();


		triIn.push_back(fromPM.BeizerPoints[i+1]+fromPM.GetLeftNormalVec());
		triIn.push_back(fromPM.BeizerPoints[i+1]);
		triIn.push_back(fromPM.BeizerPoints[i]+fromPM.GetLeftNormalVec());


		// Output triangle
		triOut.push_back(toPM.BeizerPoints[i+1]+toPM.GetLeftNormalVec());
		triOut.push_back(toPM.BeizerPoints[i+1]);
		triOut.push_back(toPM.BeizerPoints[i]+toPM.GetLeftNormalVec());

		// Warp all pixels inside input triangle to output triangle


		warpTriangle(fromMat, toMat, triIn, triOut);
		triIn.clear(); triOut.clear();








	}
	/////
	AffineTR2D(toPM.BeizerPoints,-trans,1/scale,0);

}

#endif