#ifndef _VERTEX_
#define _VERTEX_

#include <math.h>
#include "myvector.h"
class vertex
{
public:
	myvector location;
	myvector normal;
	double KH;  //平均曲率
	double KG;  //高斯曲率
	double K_min;  //最小曲率
	double K_max;  //最大曲率
	int index_HE_towards; //指向的一条halfedge的索引
	double area_mixed; //one ring 后的混合型有限面积域

	double wi; //各向异性smoothing时的系数
	double wi_fixed; //各项同性smoothing时的系数

        double u,v; //参数化对应的变量
	double KH_truth,KG_truth;
        double K_min_truth,K_max_truth;
	myvector normal_truth;

	vertex();
	vertex(myvector location);
	~vertex();

	void calPrincipalCurvature();
        
        void getUVfromTorus();
        void calTruth();
};

#endif
