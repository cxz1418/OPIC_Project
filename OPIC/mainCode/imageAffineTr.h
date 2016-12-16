#ifndef IMAGEAFFINE
#define IMAGEAFFINE

#include <opencv2\opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;
#define PAI_1 3.141592


void overlayImage(Mat* src, Mat* overlay, const Point& location)
{
	for (int y = max(location.y, 0); y < src->rows; ++y)
	{
		int fY = y - location.y;

		if (fY >= overlay->rows)
			break;

		for (int x = max(location.x, 0); x < src->cols; ++x)
		{
			int fX = x - location.x;

			if (fX >= overlay->cols)
				break;

			double opacity = ((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255;

			for (int c = 0; opacity > 0 && c < src->channels(); ++c)
			{
				unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
				unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
				src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
			}
		}
	}
}

// Warps and alpha blends triangular regions from img1 and img2 to img
void warpTriangle(Mat &img1, Mat &img2, vector<Point2f> tri1, vector<Point2f> tri2)
{
	// Find bounding rectangle for each triangle
	Rect r1 = boundingRect(tri1);
	Rect r2 = boundingRect(tri2);

	// Offset points by left top corner of the respective rectangles
	vector<Point2f> tri1Cropped, tri2Cropped;
	vector<Point> tri2CroppedInt;
	for (int i = 0; i < 3; i++)
	{
		tri1Cropped.push_back(Point2f(tri1[i].x - r1.x, tri1[i].y - r1.y));
		tri2Cropped.push_back(Point2f(tri2[i].x - r2.x, tri2[i].y - r2.y));

		// fillConvexPoly needs a vector of Point and not Point2f
		tri2CroppedInt.push_back(Point((int)(tri2[i].x - r2.x), (int)(tri2[i].y - r2.y)));

	}

	// Apply warpImage to small rectangular patches
	Mat img1Cropped;
	img1(r1).copyTo(img1Cropped);

	// Given a pair of triangles, find the affine transform.
	Mat warpMat = getAffineTransform(tri1Cropped, tri2Cropped);

	// Apply the Affine Transform just found to the src image
	Mat img2Cropped = Mat::zeros(r2.height, r2.width, img1Cropped.type());


	warpAffine(img1Cropped, img2Cropped, warpMat, img2Cropped.size(), INTER_LINEAR, BORDER_REFLECT_101);





	// Get mask by filling triangle
	Mat mask = Mat::zeros(r2.height, r2.width, img2.type());

	fillConvexPoly(mask, tri2CroppedInt, Scalar(1.0, 1.0, 1.0, 1.0), 16, 0);



	// Copy triangular region of the rectangular patch to the output image
	multiply(img2Cropped, mask, img2Cropped);




	//cout << "문제발생구간";
	//
	Mat imgCropped_png;
	img2(r2).copyTo(imgCropped_png);


	Mat mask2= Mat::zeros(r2.height, r2.width, img1.type());
	fillConvexPoly(mask2, tri2CroppedInt, Scalar(1.0, 1.0, 1.0, 1.0), 16, 0);


	multiply(imgCropped_png,mask2,imgCropped_png);

	overlayImage(&imgCropped_png,&img2Cropped,Point(0,0));

	//

	//waitKey();
	//overlayImage(&img2(r2),&img2Cropped,Point(0,0));

	multiply(img2(r2), Scalar(1.0, 1.0, 1.0, 1.0) - mask, img2(r2));

	img2(r2) = img2(r2) + imgCropped_png;
	//img2(r2) = img2(r2) + img2Cropped;
}




void AffineTR2D(vector<Point> &vec,Point trans,float scale,float rotation)

{
	Point average(0,0);
	for(int i=0; i<vec.size(); i++)
	{
		average+=vec[i];
	}
	average.x=average.x/vec.size();
	average.y=average.y/vec.size();



	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]-=average;
	}

	

	//S
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]=vec[i]*scale;
	}

	


	//R
	for(int i=0; i<vec.size(); i++)
	{
		float r_angle=PAI_1/180.0*rotation;
		float xx=cos(r_angle)*vec[i].x-sin(r_angle)*vec[i].y;
		float yy=sin(r_angle)*vec[i].x+cos(r_angle)*vec[i].y;
		vec[i].x=xx; vec[i].y=yy;
	}
	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]+=(average+trans);
	}
	

}


void AffineTR2DwithPivot(vector<Point> &vec,Point trans,float scale,float rotation,Point pivot)

{


	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]-=pivot;
	}

	

	//S
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]=vec[i]*scale;
	}

	


	//R
	for(int i=0; i<vec.size(); i++)
	{
		float r_angle=PAI_1/180.0*rotation;
		float xx=cos(r_angle)*vec[i].x-sin(r_angle)*vec[i].y;
		float yy=sin(r_angle)*vec[i].x+cos(r_angle)*vec[i].y;
		vec[i].x=xx; vec[i].y=yy;
	}
	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]+=(pivot+trans);
	}
	

}


void AffineScaleWithPivot(vector<Point> &vec,Point trans,float scale,Point pivot)

{
	

	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]-=pivot;
	}

	

	//S
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]=vec[i]*scale;
	}

	



	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]+=(pivot+trans);
	}
	

}


void AffineTR2D_float(vector<Point2f> &vec,Point2f trans,float scale,float rotation)

{
	Point2f average(0,0);
	for(int i=0; i<vec.size(); i++)
	{
		average+=vec[i];
	}
	average.x=average.x/vec.size();
	average.y=average.y/vec.size();



	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]-=average;
	}

	

	//S
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]=vec[i]*scale;
	}

	


	//R
	for(int i=0; i<vec.size(); i++)
	{
		float r_angle=PAI_1/180.0*rotation;
		float xx=cos(r_angle)*vec[i].x-sin(r_angle)*vec[i].y;
		float yy=sin(r_angle)*vec[i].x+cos(r_angle)*vec[i].y;
		vec[i].x=xx; vec[i].y=yy;
	}
	//T
	for(int i=0; i<vec.size(); i++)
	{
		vec[i]+=(average+trans);
	}
	

}



//polys로 둘러싸인 영역을 마스크로하여 그림을 잘라내여 입력한다.
void DrawInConvexPoly(Mat src,Mat dst,vector<Point> &polys,Point center_weight,double _scale)
{
	
	if(src.empty() || dst.empty()|| polys.size()<3)
	{
		cout<<"No Condition"<<endl;
		return ;
	}

	
	Mat dummy = Mat::zeros(dst.cols,dst.rows,dst.type());
	Mat dummy2 =dummy.clone();

	Point center(0,0);
	for(int i=0; i<polys.size(); i++)
	{
		center+=polys[i];
	}
	center.x=center.x/polys.size();
	center.y=center.y/polys.size();
	center+=center_weight;
	
	overlayImage(&dummy2,&src,center-Point(src.cols/2,src.rows/2));
	//circle(dummy,center,3,Scalar(255,0,0),2,1,0);
	//circle(dummy2,center,3,Scalar(0,0,255),2,1,0);


	Mat rot=getRotationMatrix2D(center,0,_scale);
	int flags=INTER_LINEAR;
	warpAffine(dummy2,dummy2,rot,Size(),flags);
	

	
	fillConvexPoly(dummy,polys , Scalar(1.0, 1.0, 1.0, 1.0), 16, 0);
	
	
	multiply(dummy,dummy2,dummy2);
	
	
	overlayImage(&dst,&dummy2,Point(0,0));

	
}
//pivot대응점으로 입력
void DrawInConvexPoly2(Mat src,Mat dst,vector<Point> &polys,Point _center,double _scale)
{
	
	if(src.empty() || dst.empty()|| polys.size()<3)
	{
		cout<<"No Condition"<<endl;
		return ;
	}

	
	Mat dummy = Mat::zeros(dst.cols,dst.rows,dst.type());
	Mat dummy2 =dummy.clone();

	Point center(0,0);
	for(int i=0; i<polys.size(); i++)
	{
		center+=polys[i];
	}
	center.x=center.x/polys.size();
	center.y=center.y/polys.size();
	
	cout<<"CENTER :"<<center<<endl;

	
	overlayImage(&dummy2,&src,_center-Point(src.cols/2,src.rows/2));
	//circle(dummy,center,3,Scalar(255,0,0),2,1,0);
	//circle(dummy2,center,3,Scalar(0,0,255),2,1,0);


	Mat rot=getRotationMatrix2D(center,0,_scale);
	int flags=INTER_LINEAR;
	warpAffine(dummy2,dummy2,rot,Size(),flags);
	

	
	fillConvexPoly(dummy,polys , Scalar(1.0, 1.0, 1.0, 1.0), 16, 0);
	
	
	multiply(dummy,dummy2,dummy2);
	
	
	overlayImage(&dst,&dummy2,Point(0,0));

	
}


#endif