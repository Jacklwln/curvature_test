#ifndef _VERTEX_
#define _VERTEX_

#include <math.h>
#include "myvector.h"
class vertex
{
public:
	myvector location;
	myvector normal;
	double KH;  //ƽ������
	double KG;  //��˹����
	double K_min;  //��С����
	double K_max;  //�������
	int index_HE_towards; //ָ���һ��halfedge������
	double area_mixed; //one ring ��Ļ�������������

	double wi; //��������smoothingʱ��ϵ��
	double wi_fixed; //����ͬ��smoothingʱ��ϵ��

        double u,v; //��������Ӧ�ı���
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
