#ifndef BEIZER2DLINE
#define BEIZER2DLINE

#include <stdio.h>
#include"opencv2\opencv.hpp"

using namespace cv;
using namespace std;
 
/***
* cp�� 4���� ��Ұ� �ִ� �迭��:
* cp[0]�� ������ (P0), cp[1]�� ù��° ������ (P1)
* cp[2]�� �ι�° ������ (P2), cp[3]�� ���� (P3)
* t�� �Ű�������, 0 �� t �� 1
***/
Point PointOnCubicBezier(Point* cp, float t)
{
    float   ax, bx, cx, ay, by, cy, tSquared, tCubed;
    Point result;
 
    /* ���׽� ����� ����Ѵ� */
    cx = 3.0 * (cp[1].x - cp[0].x);
    bx = 3.0 * (cp[2].x - cp[1].x) - cx;
    ax = cp[3].x - cp[0].x - cx - bx;
 
    cy = 3.0 * (cp[1].y - cp[0].y);
    by = 3.0 * (cp[2].y - cp[1].y) - cy;
    ay = cp[3].y - cp[0].y - cy - by;
 
    /* �Ű����� �� t���� � ���� ����Ѵ� */
    tSquared = t * t;
    tCubed = tSquared * t;
 
    result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x;
    result.y = (ay * tCubed) + (bx * tSquared) + (cy * t) + cp[0].y;
 
    return result;
}
 
/***
* ComputeBezier�� Point2D ����ü �迭�� ������ cp�κ��� ������� � �����
* ä���. ȣ��δ� ����� �޸𸮸� �Ҵ��ؾ� �ϸ�, �� ũ���
* "sizeof(Point2D) * numberOfPoints"
***/
void ComputeBezier(Point* cp, int numberOfPoints, Point* curve)
{
    float dt;
    int i;
 
    dt = 1.0 / (numberOfPoints - 1);
 
    for(i = 0; i < numberOfPoints; i++)
        curve[i] = PointOnCubicBezier(cp, i * dt);
}


int getPt22( int n1 , int n2 , float perc )
{
    int diff = n2 - n1;

	return cvRound( (double)( (float)n1+ (float)diff*perc ) );
    //return cvRound((double)((float)n1 + ( (float)diff * perc )));
}    

Point GetBeizerPoint2d(Point pt[],float per){

	Point greenDotA;
	Point greenDotB;
	// The Green Line
	greenDotA.x = getPt22( pt[0].x , pt[1].x , per );
	greenDotA.y = getPt22( pt[0].y , pt[1].y , per );
	greenDotB.x = getPt22( pt[1].x , pt[2].x , per );
	greenDotB.y = getPt22( pt[1].y , pt[2].y , per );


	Point blackDot;
	blackDot.x = getPt22( greenDotA.x , greenDotB.x , per );
	blackDot.y = getPt22( greenDotA.y , greenDotB.y , per );

	//cout<<endl<<"BlackDot::"<<blackDot.x <<blackDot.y<<endl;;

	return blackDot;

}

#endif