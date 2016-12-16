#ifndef LINEINTER
#define LINEINTER

#include<opencv2\opencv.hpp>
using namespace  cv; using namespace std;


//�ش� �̹����ȿ��� 
//������Ʈ ������ ���л��̿� �ش� ������ 
//�ٸ����� �����ϴ°�
//image�����̶� color������ �¾ƾߵȴ�
//���λ��̿� �ƹ��͵� ���ٸ� true�� �����մϴ�

bool CheckIntersectedPix_PNG(Mat image, Point pt1, Point pt2, Scalar color)
{
	uchar *img_Cut_RGB = (uchar*)image.data;

	/*
	for (int x = 0; x < image.rows; x++)
	for (int y = 0; y < image.cols; y++)
	{
	img_Cut_RGB[(y* image.cols + x) * 4] = 255;
	img_Cut_RGB[(y* image.cols + x) * 4 + 1] = 255;
	img_Cut_RGB[(y* image.cols + x) * 4 + 2] = 255;

	}
	*/

	double maxX, minX;
	if (pt1.x > pt2.x)
	{
		maxX = pt1.x; minX = pt2.x;
	}
	else
	{

		maxX = pt2.x; minX = pt1.x;
	}

	//������ �˻縦 �����Ѵ�.
	for (int i = minX; i <= maxX; i++)
	{


		int y = ((double)pt2.y - (double)pt1.y) /
			((double)pt2.x - (double)pt1.x)*((double)i - (double)pt1.x) + (double)pt1.y;


		if (Scalar(img_Cut_RGB[(y*image.cols + i) * 4], img_Cut_RGB[(y*image.cols + i) * 4 + 1]
			, img_Cut_RGB[(y*image.cols + i) * 4 + 2], img_Cut_RGB[(y*image.cols + i) * 4 + 3]) != color)
			return false;
	}

	return true;


}


double Getdistance(Point pt1, Point pt2)
{
	return sqrt(pow(((double)pt2.y - (double)pt1.y), 2) + pow(((double)pt2.x - (double)pt1.x), 2));
}


#endif