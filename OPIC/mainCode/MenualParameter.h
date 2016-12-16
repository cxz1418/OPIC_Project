#ifndef MENUALPARAM
#define MENUALPARAM

#include<iostream>
#include"ptManager.h"
using namespace std;

enum{RIGHT_DARK,LEFT_DARK,TOW    ,LEFT_EYEBLOW,LEFT_EYE,  RIGHT_EYEBLOW,RIGHT_EYE, MOUTH  ,NOSE,NOSE_UP};

class partInf
{
public:
	int PartName;

	String path;
	vector<int> allcNums;
	Point bias;
	float scale;
	PointManager ptManager;

	float volumeBias;

	partInf(String _path,int *_arr,int _arr_size) :path(_path)
	{
		volumeBias=1.0f;
		for(int i=0; i<_arr_size;i++)
		{
			allcNums.push_back(_arr[i]);
		}
		scale=1.0f;
		bias.x=0; bias.y=0;
	}

	void ClearAll()
	{
		allcNums.clear();
		ptManager.ClearAll();
	}
	~partInf()
	{
		allcNums.clear();
		ptManager.ClearAll();
	}
};

vector<partInf*> * GetMenualPartInfs()
{

	vector<partInf*> * partInfs = new vector<partInf*>;

	int arr1[]={1,2,3,4};
	partInfs->push_back(new partInf("left_tow_1",arr1,sizeof(arr1)/sizeof(int)));

	int arr2[]={4,5,6,7};
	partInfs->push_back(new partInf("left_tow_2",arr2,sizeof(arr2)/sizeof(int)));

	int arr3[]={7,8,9};
	partInfs->push_back(new partInf("left_tow_3",arr3,sizeof(arr3)/sizeof(int)));

	int arr4[]={9,10,11};
	partInfs->push_back(new partInf("right_tow_3",arr4,sizeof(arr4)/sizeof(int)));

	int arr5[]={11,12,13,14};
	partInfs->push_back(new partInf("right_tow_2",arr5,sizeof(arr5)/sizeof(int)));

	int arr6[]={14,15,16,17};
	partInfs->push_back(new partInf("right_tow_1",arr6,sizeof(arr6)/sizeof(int)));

	int arr7[]={18,19,20,21,22};
	partInfs->push_back(new partInf("left_eyeblow",arr7,sizeof(arr7)/sizeof(int)));

	int arr8[]={23,24,25,26,27};
	partInfs->push_back(new partInf("right_eyeblow",arr8,sizeof(arr8)/sizeof(int)));

	int arr9[]={28,29,30,31};
	partInfs->push_back(new partInf("nose_up",arr9,sizeof(arr9)/sizeof(int)));

	int arr10[]={32,33,34,35,36};
	partInfs->push_back(new partInf("nose_down",arr10,sizeof(arr10)/sizeof(int)));

	int arr11[]={37,38,39,40};
	partInfs->push_back(new partInf("left_eye_up",arr11,sizeof(arr11)/sizeof(int)));

	int arr12[]={43,44,45,46};
	partInfs->push_back(new partInf("right_eye_up",arr12,sizeof(arr12)/sizeof(int)));

	int arr13[]={49,61,62,63,64,65,55};
	partInfs->push_back(new partInf("mouth",arr13,sizeof(arr13)/sizeof(int)));

	int arr14[]={40,41,42,37};
	partInfs->push_back(new partInf("left_eye_down",arr14,sizeof(arr14)/sizeof(int)));

	int arr15[]={46,47,48,43};
	partInf * dummy = new partInf("right_eye_down",arr15,sizeof(arr15)/sizeof(int));
	partInfs->push_back(dummy);

	
	return partInfs;
}

vector<partInf*> * GetMenualPartInfs2()
{

	partInf * dummy;

	vector<partInf*> * partInfs = new vector<partInf*>;

	int arr1[]={1,2,3,4};
	dummy=new partInf("left_tow_1",arr1,sizeof(arr1)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr2[]={4,5,6,7};
	dummy=new partInf("left_tow_2",arr2,sizeof(arr2)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr3[]={7,8,9,10,11};
	dummy=new partInf("middle_tow",arr3,sizeof(arr3)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);


	int arr5[]={11,12,13,14};
	dummy=new partInf("right_tow_2",arr5,sizeof(arr5)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr6[]={14,15,16,17};
	dummy=new partInf("right_tow_1",arr6,sizeof(arr6)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr7[]={18,19,20,21,22};
	dummy = new partInf("left_eyeblow",arr7,sizeof(arr7)/sizeof(int));
	dummy->PartName=LEFT_EYEBLOW;
	partInfs->push_back(dummy);

	int arr8[]={23,24,25,26,27};
	dummy=new partInf("right_eyeblow",arr8,sizeof(arr8)/sizeof(int));
	dummy->PartName=RIGHT_EYEBLOW;
	partInfs->push_back(dummy);

	int arr9[]={28,29,30,31};
	dummy=new partInf("nose_up",arr9,sizeof(arr9)/sizeof(int));
	dummy->PartName=NOSE;
	partInfs->push_back(dummy);


	int arr10[]={32,33,34,35,36};
	dummy=new partInf("nose_down",arr10,sizeof(arr10)/sizeof(int));
	dummy->PartName=NOSE;
	partInfs->push_back(dummy);


	int arr13[]={49,61,62,63,64,65,55};
	dummy=new partInf("mouth",arr13,sizeof(arr13)/sizeof(int));
	dummy->PartName=MOUTH;
	partInfs->push_back(dummy);
	/////

	int arr11[]={37,38};  //11
	dummy=new partInf("left_eye_up_3",arr11,sizeof(arr11)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr12[]={38,39};  //12
	dummy =new partInf("left_eye_up_2",arr12,sizeof(arr12)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	//dummy->volumeBias=1.5f;
	partInfs->push_back(dummy);


	int arr14[]={39,40};   //13
	dummy=new partInf("left_eye_up_1",arr14,sizeof(arr14)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr15[]={40,41,42,37};  //14
	dummy = new partInf("left_eye_down",arr15,sizeof(arr15)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	//

	int arr16[]={43,44};  //15
	dummy = new partInf("right_eye_up_1",arr16,sizeof(arr16)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=0.7f;
	partInfs->push_back(dummy);

	int arr17[]={44,45};  //16
	dummy = new partInf("right_eye_up_2",arr17,sizeof(arr17)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=1.9f;
	partInfs->push_back(dummy);


	int arr4[]={45,46};//17
	dummy=new partInf("right_eye_up_3",arr4,sizeof(arr4)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);

	
	int arr18[]={46,47,48,43}; // 18
	dummy=new partInf("right_eye_down",arr18,sizeof(arr18)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);


	return partInfs;
}

vector<partInf*> * GetMenualPartInfs_70()
{

	partInf * dummy;

	vector<partInf*> * partInfs = new vector<partInf*>;

	/*
	int arr01[]={45,50,52};
	dummy=new partInf("left_dark",arr01,sizeof(arr01)/sizeof(int));
	dummy->PartName=LEFT_DARK;
	partInfs->push_back(dummy);	

	int arr02[]={46,51,53};
	dummy=new partInf("right_dark",arr02,sizeof(arr02)/sizeof(int));
	dummy->PartName=RIGHT_DARK;
	partInfs->push_back(dummy);	


	*/

	int arr1[]={66,68,5};
	dummy=new partInf("left_tow_1",arr1,sizeof(arr1)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr2[]={5,9};
	dummy=new partInf("left_tow_2",arr2,sizeof(arr2)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr3[]={9,11,10};
	dummy=new partInf("middle_tow",arr3,sizeof(arr3)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);


	int arr5[]={10,6};
	dummy=new partInf("right_tow_2",arr5,sizeof(arr5)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr6[]={6,69,67};
	dummy=new partInf("right_tow_1",arr6,sizeof(arr6)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr7[]={12,18,16,19,13};
	dummy = new partInf("left_eyeblow",arr7,sizeof(arr7)/sizeof(int));
	dummy->PartName=LEFT_EYEBLOW;
	dummy->volumeBias=0.8;
	partInfs->push_back(dummy);

	int arr8[]={14,20,17,21,15};
	dummy=new partInf("right_eyeblow",arr8,sizeof(arr8)/sizeof(int));
	dummy->PartName=RIGHT_EYEBLOW;
	dummy->volumeBias=0.8;
	partInfs->push_back(dummy);

	int arr9[]={22,2};
	dummy=new partInf("nose_up",arr9,sizeof(arr9)/sizeof(int));
	dummy->PartName=NOSE_UP;
	partInfs->push_back(dummy);


	int arr10[]={45,49,46};
	dummy=new partInf("nose_down",arr10,sizeof(arr10)/sizeof(int));
	dummy->PartName=NOSE;
	partInfs->push_back(dummy);


	int arr13[]={3,60,61,62,4};
	dummy=new partInf("mouth",arr13,sizeof(arr13)/sizeof(int));
	dummy->PartName=MOUTH;
	partInfs->push_back(dummy);
	/////

	int arr11[]={23,35};  //11
	dummy=new partInf("left_eye_up_3",arr11,sizeof(arr11)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr12[]={35,28,36};  //12
	dummy =new partInf("left_eye_up_2",arr12,sizeof(arr12)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	//dummy->volumeBias=1.5f;
	partInfs->push_back(dummy);


	int arr14[]={36,24};   //13
	dummy=new partInf("left_eye_up_1",arr14,sizeof(arr14)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr15[]={24,38,27,37,23};  //14
	dummy = new partInf("left_eye_down",arr15,sizeof(arr15)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	//

	int arr16[]={25,39};  //15
	dummy = new partInf("right_eye_up_1",arr16,sizeof(arr16)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=0.7f;
	partInfs->push_back(dummy);

	int arr17[]={39,32,40};  //16
	dummy = new partInf("right_eye_up_2",arr17,sizeof(arr17)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=1.9f;
	partInfs->push_back(dummy);


	int arr4[]={40,26};//17
	dummy=new partInf("right_eye_up_3",arr4,sizeof(arr4)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);

	
	int arr18[]={26,42,31,41,25}; // 18
	dummy=new partInf("right_eye_down",arr18,sizeof(arr18)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);


	return partInfs;
}


vector<partInf*> * GetMenualPartInfs_70_wanted()
{

	partInf * dummy;

	vector<partInf*> * partInfs = new vector<partInf*>;



	int arr1[]={66,68,5};
	dummy=new partInf("left_tow_1",arr1,sizeof(arr1)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr2[]={5,7,9};
	dummy=new partInf("left_tow_2",arr2,sizeof(arr2)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr3[]={9,11,10};
	dummy=new partInf("middle_tow",arr3,sizeof(arr3)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);


	int arr5[]={10,8,6};
	dummy=new partInf("right_tow_2",arr5,sizeof(arr5)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr6[]={6,69,67};
	dummy=new partInf("right_tow_1",arr6,sizeof(arr6)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr7[]={12,18,16,19,13};
	dummy = new partInf("left_eyeblow",arr7,sizeof(arr7)/sizeof(int));
	dummy->PartName=LEFT_EYEBLOW;
	dummy->volumeBias=0.8;
	partInfs->push_back(dummy);

	int arr8[]={14,20,17,21,15};
	dummy=new partInf("right_eyeblow",arr8,sizeof(arr8)/sizeof(int));
	dummy->PartName=RIGHT_EYEBLOW;
	dummy->volumeBias=0.8;
	partInfs->push_back(dummy);

	int arr9[]={22,2};
	dummy=new partInf("nose_up",arr9,sizeof(arr9)/sizeof(int));
	dummy->PartName=NOSE_UP;
	partInfs->push_back(dummy);


	int arr10[]={45,49,46};
	dummy=new partInf("nose_down",arr10,sizeof(arr10)/sizeof(int));
	dummy->PartName=NOSE;
	partInfs->push_back(dummy);


	int arr13[]={3,60,61,62,4};
	dummy=new partInf("mouth",arr13,sizeof(arr13)/sizeof(int));
	dummy->volumeBias=0.7;
	dummy->scale=0.8f;
	dummy->PartName=MOUTH;
	partInfs->push_back(dummy);
	/////

	
	int arr11[]={23,35};  //11
	dummy=new partInf("left_eye_up_3",arr11,sizeof(arr11)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr12[]={35,28,36};  //12
	dummy =new partInf("left_eye_up_2",arr12,sizeof(arr12)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	//dummy->volumeBias=1.5f;
	partInfs->push_back(dummy);


	int arr14[]={36,24};   //13
	dummy=new partInf("left_eye_up_1",arr14,sizeof(arr14)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr15[]={24,38,27,37,23};  //14
	dummy = new partInf("left_eye_down",arr15,sizeof(arr15)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	//

	int arr16[]={25,39};  //15
	dummy = new partInf("right_eye_up_1",arr16,sizeof(arr16)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=0.7f;
	partInfs->push_back(dummy);

	int arr17[]={39,32,40};  //16
	dummy = new partInf("right_eye_up_2",arr17,sizeof(arr17)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=1.9f;
	partInfs->push_back(dummy);


	int arr4[]={40,26};//17
	dummy=new partInf("right_eye_up_3",arr4,sizeof(arr4)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);

	
	int arr18[]={26,42,31,41,25}; // 18
	dummy=new partInf("right_eye_down",arr18,sizeof(arr18)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);


	return partInfs;
}


vector<partInf*> * GetMenualPartInfs_68()
{

	partInf * dummy;

	vector<partInf*> * partInfs = new vector<partInf*>;



	int arr1[]={0,1,2,3};
	dummy=new partInf("left_tow_1",arr1,sizeof(arr1)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr2[]={3,4,5,6};
	dummy=new partInf("left_tow_2",arr2,sizeof(arr2)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr3[]={6,7,8,9,10};
	dummy=new partInf("middle_tow",arr3,sizeof(arr3)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);


	int arr5[]={10,11,12,13};
	dummy=new partInf("right_tow_2",arr5,sizeof(arr5)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr6[]={13,14,15,16};
	dummy=new partInf("right_tow_1",arr6,sizeof(arr6)/sizeof(int));
	dummy->PartName=TOW;
	partInfs->push_back(dummy);

	int arr7[]={17,18,19,20,21};
	dummy = new partInf("left_eyeblow",arr7,sizeof(arr7)/sizeof(int));
	dummy->PartName=LEFT_EYEBLOW;
	dummy->volumeBias=0.4;
	partInfs->push_back(dummy);

	int arr8[]={22,23,24,25,26};
	dummy=new partInf("right_eyeblow",arr8,sizeof(arr8)/sizeof(int));
	dummy->PartName=RIGHT_EYEBLOW;
	dummy->volumeBias=0.4;
	partInfs->push_back(dummy);

	int arr9[]={27,28,29,30};
	dummy=new partInf("nose_up",arr9,sizeof(arr9)/sizeof(int));
	dummy->PartName=NOSE_UP;
	partInfs->push_back(dummy);


	int arr10[]={31,32,33,34,35};
	dummy=new partInf("nose_down",arr10,sizeof(arr10)/sizeof(int));
	dummy->PartName=NOSE;
	dummy->scale=1.7;
	dummy->bias=Point(0,-6);
	partInfs->push_back(dummy);


	int arr13[]={48,60,61,62,63,64,54};
	dummy=new partInf("mouth",arr13,sizeof(arr13)/sizeof(int));
	dummy->PartName=MOUTH;
	partInfs->push_back(dummy);
	/////

	int arr11[]={36,37};  //11
	dummy=new partInf("left_eye_up_3",arr11,sizeof(arr11)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr12[]={37,38};  //12
	dummy =new partInf("left_eye_up_2",arr12,sizeof(arr12)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	//dummy->volumeBias=1.5f;
	partInfs->push_back(dummy);


	int arr14[]={38,39};   //13
	dummy=new partInf("left_eye_up_1",arr14,sizeof(arr14)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	int arr15[]={39,40,41,36};  //14
	dummy = new partInf("left_eye_down",arr15,sizeof(arr15)/sizeof(int));
	dummy->PartName=LEFT_EYE;
	partInfs->push_back(dummy);

	//

	int arr16[]={42,43};  //15
	dummy = new partInf("right_eye_up_1",arr16,sizeof(arr16)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=0.7f;
	partInfs->push_back(dummy);

	int arr17[]={43,44};  //16
	dummy = new partInf("right_eye_up_2",arr17,sizeof(arr17)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	//dummy->volumeBias=1.9f;
	partInfs->push_back(dummy);


	int arr4[]={44,45};//17
	dummy=new partInf("right_eye_up_3",arr4,sizeof(arr4)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);

	
	int arr18[]={45,46,47,42}; // 18
	dummy=new partInf("right_eye_down",arr18,sizeof(arr18)/sizeof(int));
	dummy->PartName=RIGHT_EYE;
	partInfs->push_back(dummy);


	return partInfs;
}


Point GetAvgPoints(vector<Point> &points)
{
	Point center(0,0);
	for(int i=0; i<points.size(); i++)
	{
		center+=points[i];
	}
	center.x/=points.size();
	center.y/=points.size();
	return center;
}

//ptManager가 초기화된다음 사용해야된다.
vector<Point> * GetPointFromPartName(vector<partInf*> * partInfs ,int partName)
{
	vector<Point> *points=new vector<Point>;

	for(int i=0; i<partInfs->size(); i++)
	{
		if((*partInfs)[i]->PartName==partName)
			(*partInfs)[i]->ptManager.BeizerPoints_middle(*points);

	}


	return points;
}

vector<Point> * GetPointFromPartName_back(vector<partInf*> * partInfs ,int partName)
{
	vector<Point> *points=new vector<Point>;

	for(int i=0; i<partInfs->size(); i++)
	{
		if((*partInfs)[i]->PartName==partName)
			(*partInfs)[i]->ptManager.BeizerPoints_middle_resverse(*points);

	}


	return points;
}


void STFromPartName(vector<partInf*>*partInfs , int partName,float scale,Point trans)
{
	vector<Point> *points=GetPointFromPartName(partInfs,partName);
	
	Point averagePt=GetAvgPoints(*points);

	points->clear();



	for(int i=0; i<partInfs->size(); i++)
	{
		if((*partInfs)[i]->PartName==partName)
			{
				AffineScaleWithPivot((*partInfs)[i]->ptManager.BeizerPoints,trans,scale,averagePt);

			}
	}


}



#endif