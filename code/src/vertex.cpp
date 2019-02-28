#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "vertex.h"
using namespace std;

#define pi 3.1415926535898

vertex::vertex()
{
	;
}

vertex::vertex(myvector location)
{
	this->location=location;
	index_HE_towards=-1; //-1表示并没有找到一条边由其出发
	this->wi_fixed=0.5;
}

vertex::~vertex()
{
	;
}

void vertex::calPrincipalCurvature()
{
	double delta=KH*KH-KG;
	delta=(delta>=0?delta:0);
	K_max=KH+sqrtf(delta);
	K_min=KH-sqrtf(delta);
	return;
}


void vertex::getUVfromTorus()
{
	double C=1; //torus的较大半径
	double A=0.25; //torus的较小半径

	double v,u;  //最后确定的
	double v1,v2,u1,u2; //可能的

	double difOfv1,difOfv2,difOfu1,difOfu2;

	v1=asin(location.y/0.25);
	if(v1<0.00)
	{
		v2=-1*pi-v1;
	}
	else if(v1>=0.00)
	{
		v2=pi-v1;
	}
	
	difOfv1=fabs(pow(location.x/(1+0.25*cos(v1)),2)+pow(location.z/(1+0.25*cos(v1)),2)-1);
	difOfv2=fabs(pow(location.x/(1+0.25*cos(v2)),2)+pow(location.z/(1+0.25*cos(v2)),2)-1);

	if(difOfv1<difOfv2)
	{
		v=v1;
	}
	else
	{
		v=v2;
	}

	u1=asin(location.z/(1+0.25*cos(v)));

	if(u1<0.00)
	{
		u2=-1*pi-u1;
	}
	else if(u1>=0.00)
	{
		u2=pi-u1;
	}

	difOfu1=fabs(location.x-(1+0.25*cos(v))*cos(u1));
	difOfu2=fabs(location.x-(1+0.25*cos(v))*cos(u2));

	if(difOfu1<difOfu2)
	{
		u=u1;
	}
	else
	{
		u=u2;
	}

	this->u=u;
	this->v=v;
}

void vertex::calTruth()
{
	normal_truth=myvector(cos(u)*cos(v),sin(v),sin(u)*cos(v));
        

        printf("%lf %lf %lf ///// %lf %lf %lf\n",normal_truth.x,normal_truth.y,normal_truth.z,normal.x,normal.y,normal.z);
	KH_truth=-1*(1+0.5*cos(v))*2/(1+0.25*cos(v));
	KG_truth=cos(v)*4/(1+0.25*cos(v));

        double delta=KH_truth*KH_truth-KG_truth;
	delta=(delta>=0?delta:0);
	K_max_truth=KH_truth+sqrtf(delta);
	K_min_truth=KH_truth-sqrtf(delta);
}

