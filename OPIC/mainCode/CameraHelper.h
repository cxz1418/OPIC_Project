#ifndef CAMERA_HELPER
#define CAMERA_HELPER
#include<iostream>
#include"opencv2\opencv.hpp"
#include"CvvImage.h"

using namespace cv;
using namespace std;
CvSize init()
{
	//cvCaptureCam�� �̿��� ķ���� ���� �޾ƿ� ������ ����ü�� �����ϰ�
	//cvQueryFrame�� ������ �Ҵ�� ����ü�� �޸𸮸� �ҷ��ɴϴ�..
    IplImage *initCapture = cvQueryFrame(cvCaptureFromCAM(0));
	
    if(!initCapture)
    {
		//ķ���� ������ �޾ƿ��µ� ����������� 
        cout<<"Video capture failed, please check the camera."<<endl;
    }
    else
    {
		//������ �޾ƿ��µ� ������
        cout<<"Video camera capture status: OK"<<endl;
    }

    CvSize size = cvGetSize(initCapture);//ķ������ �̹���ũ�⸦ �޾ƿɴϴ�.
    cvReleaseImage(&initCapture);
    return size;
}



#endif