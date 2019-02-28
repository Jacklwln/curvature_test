#ifndef _OBJECT_
#define _OBJECT_

#include <vector>
#include "halfedge.h"
#include "vertex.h"
#include "face.h"
#include "testface.h"
using namespace std;

class object
{
public:
	vector<vertex > myvertexs;
	vector<face > myfaces;
	vector<halfedge > myhalfedges;
	vector<testface > mytestfaces;

	double Max_Min_InAll[4][2];

	int num_vertex;
	int num_face;
	int num_halfedge;

	double parameter_T; //user-defined
	double dt; //smoothing的步长

	object();
	~object();
	int calData(int index[3],double data[3][3],double &area);
	void getObjData();
	void calFaceInf();
	void calCurvatureAndNomal();
	void getMaxAndMin();
	void calWi();
	void smooth(int kind); //各向异性 or 各向同性

	void pipeline(int kind);

        void compare();

	void testdraw();
	void drawwithKH();
	void drawwithKG();
	void drawwithKMax();
	void drawwithKMin();
	void drawwithNormal();
	
};
#endif
