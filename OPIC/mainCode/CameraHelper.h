#ifndef CAMERA_HELPER
#define CAMERA_HELPER
#include<iostream>
#include"opencv2\opencv.hpp"
#include"CvvImage.h"

using namespace cv;
using namespace std;
CvSize init()
{
	//cvCaptureCam을 이용해 캠으로 부터 받아온 영상을 구조체에 저장하고
	//cvQueryFrame은 위에서 할당된 구조체의 메모리를 불러옵니다..
    IplImage *initCapture = cvQueryFrame(cvCaptureFromCAM(0));
	
    if(!initCapture)
    {
		//캠에서 영상을 받아오는데 실패했을경우 
        cout<<"Video capture failed, please check the camera."<<endl;
    }
    else
    {
		//영상을 받아오는데 성공시
        cout<<"Video camera capture status: OK"<<endl;
    }

    CvSize size = cvGetSize(initCapture);//캠영상의 이미지크기를 받아옵니다.
    cvReleaseImage(&initCapture);
    return size;
}



#endif