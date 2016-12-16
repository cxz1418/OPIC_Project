#ifndef LANDMARKLOADER
#define LANDMARKLOADER


#include"opencv2\opencv.hpp"
#include<fstream>
#include<iostream>
#include <sstream>

using namespace cv;
using namespace std;

void SetTextCustom(int num, int _x, int _y, Mat dstImage,string text)
{
	
	int fontFace = FONT_HERSHEY_SIMPLEX;
	double fontScale = 0.5;
	int thickness = 1;
	int baseLine = 0;
	Point org(_x - 3, _y - 3);

	putText(dstImage, text, org, fontFace, fontScale, Scalar(0, 255, 0));

}



//#include<ssstream>필요 
string int2str(int n)
{
    stringstream ss; // create a stringstream
    ss << n; // add n to the stream
    return ss.str();
}



Point * Get68Landmarks_300(string _fileName)
{
	double data2;
	char data1[30];

	double landmarks[136];
	
	ifstream fin;
	fin.open(_fileName);

	if(fin.is_open()==false)
	{
		cout<<"현재정보없음"<<endl;
		return NULL;
	}
	else
	{
		fin>>data1;
		fin>>data2;
		fin>>data1;
		fin>>data2;
		fin>>data1;
		for(int i=0; i<136; i++)
		{
			fin>>landmarks[i];
		}
		Point * points=new Point[68];
		for(int i=0; i<68; i++)
		{
			points[i].x=landmarks[i*2];
			points[i].y=landmarks[i*2+1];
		}
		return points;
	}
}



#endif