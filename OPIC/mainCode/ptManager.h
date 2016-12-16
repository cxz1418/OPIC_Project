#ifndef PTMANAGER
#define PTMANAGER

#include"68landmarkLoader.h"
#include"opencv2\opencv.hpp"
#include"BeizerLine3d.h"
#include"lineIterator_png.h"
#include"imageAffineTr.h"
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
	PointManager(int _maxSize);

	PointManager();
	
	
	~PointManager();

	void ClearAll();
	
	
	void ToggleOn();

	void ToggleOff();
	

	bool findTogglePoint(int dx, int dy);



	void drawPoints(Mat image);
	

    //Warp면적생성
	void FindWarpHeights(Mat image);

	Point GetRightNormalVec();

	Point GetLeftNormalVec();


	//매핑되는 사진 베이지어에서 매핑시 사용할 함수로써
	//각 매개변수로 개칭되는 warp영역을 만들어낸다.



	
	void genBeizerPointsFromRate(float rH,float lH,int sRate,int mRate,int eRate);


	void genBeizerPointsFromRate2(float rH,float lH,int sRate,int mRate,int eRate);



	float getCurveLength();

	void genBeizerPoints(int segmentNum);

	//비율에따라 거리를 일정하게 바꾼베지어 점들을생성
	void remapPoints(int segmentNum);
	void drawBeizerLine(Mat image);
	void generatePoints(int x, int y);
	void MovePointsTo(int rx, int ry);


	//포인트 3개이상의 점들을 받아
	//그점들에 근사하게 맞는 평균 베지어곡선을 찾는다.
	//여기서는 단순히 베지어 곡선 컨트롤의 평균값을 사용했다.
	//정확하지는 않지만 근사한 모양이 나오긴한다.
	Point* GetAverageControlPoint(Point pts[],int size);

	float PointDot(Point p1,Point p2);

	void FileOut(String _fileName,String _filePath);
	void FileIn(String _fileName);
	void FileIn_style(String _fileName,String stylePath);


	void ShowAllData();


	


void BeizerPoints_middle(vector<Point> &_dPoints);

void BeizerPoints_middle_resverse(vector<Point> &_dPoints);




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