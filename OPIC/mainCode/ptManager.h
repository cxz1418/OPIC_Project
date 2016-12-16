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
	int keyPoints[2]; //������ �������� ��ȯ����ߵȴ� �� 1~50���� �����ϰ��ְ�
					//0�� keyPoint[0]/100  1�� 1.0-keyPoint[1]/100 �� �����̸� ���׸�Ʈ�� ���� ����� �ε����� �����ǰڴ�.
public:
	PointManager(int _maxSize);

	PointManager();
	
	
	~PointManager();

	void ClearAll();
	
	
	void ToggleOn();

	void ToggleOff();
	

	bool findTogglePoint(int dx, int dy);



	void drawPoints(Mat image);
	

    //Warp��������
	void FindWarpHeights(Mat image);

	Point GetRightNormalVec();

	Point GetLeftNormalVec();


	//���εǴ� ���� ��������� ���ν� ����� �Լ��ν�
	//�� �Ű������� ��Ī�Ǵ� warp������ ������.



	
	void genBeizerPointsFromRate(float rH,float lH,int sRate,int mRate,int eRate);


	void genBeizerPointsFromRate2(float rH,float lH,int sRate,int mRate,int eRate);



	float getCurveLength();

	void genBeizerPoints(int segmentNum);

	//���������� �Ÿ��� �����ϰ� �ٲۺ����� ����������
	void remapPoints(int segmentNum);
	void drawBeizerLine(Mat image);
	void generatePoints(int x, int y);
	void MovePointsTo(int rx, int ry);


	//����Ʈ 3���̻��� ������ �޾�
	//�����鿡 �ٻ��ϰ� �´� ��� �������� ã�´�.
	//���⼭�� �ܼ��� ������ � ��Ʈ���� ��հ��� ����ߴ�.
	//��Ȯ������ ������ �ٻ��� ����� �������Ѵ�.
	Point* GetAverageControlPoint(Point pts[],int size);

	float PointDot(Point p1,Point p2);

	void FileOut(String _fileName,String _filePath);
	void FileIn(String _fileName);
	void FileIn_style(String _fileName,String stylePath);


	void ShowAllData();


	


void BeizerPoints_middle(vector<Point> &_dPoints);

void BeizerPoints_middle_resverse(vector<Point> &_dPoints);




//������ ���̾�� �־���Ѵ�. ���̾�� ���� �� ������ ������ , �̵��� ��ȯ�� �־���Ѵ�
//�� �������� ����Ʈ���� ����� �߽����� ������
//Ȥ�� ��ü���� Ʈ�������̼� ���� �־���Ѵ�.
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
		
		//cout<<i << "��°���ҽ���" << endl;
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
		
		//cout<<i << "��°���ҽ���" << endl;
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
		
		//cout<<i << "��°���ҽ���" << endl;
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