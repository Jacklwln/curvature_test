#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <map>
#include <GL/glut.h>
#include "object.h"
#include "physic.h"
using namespace std;

#define pi 3.1415926535898
#define min(a,b) (((a) < (b)) ? (a) : (b))

object::object()
{
	dt=0.00001;
	parameter_T=30;
	getObjData();
	calCurvatureAndNomal();
	getMaxAndMin();

	compare();
}

object::~object()
{
	
}

void object::compare()
{
	int i;
	for(i=0;i<num_vertex;i++)
	{
		myvertexs[i].getUVfromTorus();
		myvertexs[i].calTruth();
	}
}

void object::calWi()
{
	int siz=myvertexs.size();
	int i;
	for(i=0;i<siz;i++)
	{
		//计算wi
		double k1=fabs(myvertexs[i].K_max);
		double k2=fabs(myvertexs[i].K_min);
		double k3=fabs(myvertexs[i].KH);
		if(k1<=parameter_T&&k2<=parameter_T)
		{
			myvertexs[i].wi=1;
		}
		else if(k1>parameter_T&&k2>parameter_T&&myvertexs[i].K_max*myvertexs[i].K_min>0)
		{
			myvertexs[i].wi=0;
		}
		else if(k1==min(k1,min(k2,k3)))
		{
			myvertexs[i].wi=myvertexs[i].K_max/myvertexs[i].KH;
		}
		else if(k2==min(k1,min(k2,k3)))
		{
			myvertexs[i].wi=myvertexs[i].K_min/myvertexs[i].KH;
		}
		else if(k3==min(k1,min(k2,k3)))
		{
			myvertexs[i].wi=1;
		}

		myvertexs[i].wi=myvertexs[i].wi<-0.1?-0.1:myvertexs[i].wi; //设置一个threshold进行纠正
	}
}


void object::smooth(int kind)
{
	int i=0; 
	int siz=myvertexs.size();
	for(i=0;i<siz;i++)
	{
		if(kind==1) //各向同性
		{
			myvertexs[i].location=myvertexs[i].location+myvertexs[i].wi_fixed*dt*myvertexs[i].KH*myvertexs[i].normal;
		}
		else if(kind==0) //各向异性
		{
			myvertexs[i].location=myvertexs[i].location+myvertexs[i].wi*dt*myvertexs[i].KH*myvertexs[i].normal;
		}
		
	}
}

void object::pipeline(int kind)
{
	int num=0;
	while(num<10)
	{
		calFaceInf();
		calCurvatureAndNomal();
		getMaxAndMin();
		drawwithKH();
		calWi();
	    smooth(kind);
		num++;
	}
	if(kind==1)
	{
		printf("isotropic\n");
	}
	else
	{
		printf("anisotropic\n");
	}
	
}

void object::getMaxAndMin()
{
	int i;
	Max_Min_InAll[0][0]=Max_Min_InAll[0][1]=myvertexs[0].KH;
	Max_Min_InAll[1][0]=Max_Min_InAll[1][1]=myvertexs[0].KG;
	Max_Min_InAll[2][0]=Max_Min_InAll[2][1]=myvertexs[0].K_max;
	Max_Min_InAll[3][0]=Max_Min_InAll[3][1]=myvertexs[0].K_min;
	for(i=1;i<num_vertex;i++)
	{
		if(myvertexs[i].KH>Max_Min_InAll[0][0])
		{
			Max_Min_InAll[0][0]=myvertexs[i].KH;
		}
		else if(myvertexs[i].KH<Max_Min_InAll[0][1]) 
		{
			Max_Min_InAll[0][1]=myvertexs[i].KH;
		}
		
		if(myvertexs[i].KG>Max_Min_InAll[1][0])
		{
			Max_Min_InAll[1][0]=myvertexs[i].KG;
		}
		else if(myvertexs[i].KG<Max_Min_InAll[1][1]) 
		{
			Max_Min_InAll[1][1]=myvertexs[i].KG;
		}
		
		if(myvertexs[i].K_max>Max_Min_InAll[2][0])
		{
			Max_Min_InAll[2][0]=myvertexs[i].K_max;
		}
		else if(myvertexs[i].K_max<Max_Min_InAll[2][1]) 
		{
			Max_Min_InAll[2][1]=myvertexs[i].K_max;
		}
		
		if(myvertexs[i].K_min>Max_Min_InAll[3][0])
		{
			Max_Min_InAll[3][0]=myvertexs[i].K_min;
		}
		else if(myvertexs[i].K_min<Max_Min_InAll[3][1]) 
		{
			Max_Min_InAll[3][1]=myvertexs[i].K_min;
		}
	}
	/*
	printf("%lf %lf\n",Max_Min_InAll[0][0],Max_Min_InAll[0][1]);
	printf("%lf %lf\n",Max_Min_InAll[1][0],Max_Min_InAll[1][1]);
	printf("%lf %lf\n",Max_Min_InAll[2][0],Max_Min_InAll[2][1]);
	printf("%lf %lf\n",Max_Min_InAll[3][0],Max_Min_InAll[3][1]);*/
	
}

void  object::testdraw()
{
	int i;
	int siz=mytestfaces.size();
	testface now;
	glColor3d(0,1,1);
	for(i=0;i<siz;i++)
	{
		now=mytestfaces[i];
		glBegin(GL_POLYGON);
		{
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);
		}
		glEnd();
	}

/*
	face nowface;
	glColor3d(0,1,1);
	for(i=0;i<num_face;i++)
	{
		nowface=myfaces[i];
		glBegin(GL_POLYGON);
		{
			int index_beginHE=nowface.index_HE;
			int index_nowHE=index_beginHE;
			do 
			{
				int index_vertex=myhalfedges[index_nowHE].index_vertex_towards;
				glVertex3d(myvertexs[index_vertex].location.x,myvertexs[index_vertex].location.y,myvertexs[index_vertex].location.z);
				index_nowHE=myhalfedges[index_nowHE].index_nextHE;
			} while (index_nowHE!=index_beginHE);
		}
		glEnd();
	}*/

}

void object::drawwithKH()
{
	int i,j;
	testface now;
	int key[3]; //当前face的三个顶点的颜色索引
	for(i=0;i<num_face;i++)
	{
		now=mytestfaces[i];
		for(j=0;j<3;j++)
		{
			key[j]=floor((myvertexs[now.index_vertex[j]].KH-Max_Min_InAll[0][1])/(Max_Min_InAll[0][0]-Max_Min_InAll[0][1])*7.0);
			//key[j]=key[j]>6?6:key[j];
			//key[j]=key[j]<0?0:key[j];
		}
		
		glBegin(GL_POLYGON);
		{
			glColor3dv(physic::color[key[0]]);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glColor3dv(physic::color[key[1]]);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glColor3dv(physic::color[key[2]]);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);
		}
		glEnd();	
	}
}

void object::drawwithKG()
{
	int i,j;
	testface now;
	int key[3]; //当前face的三个顶点的颜色索引
	for(i=0;i<num_face;i++)
	{
		now=mytestfaces[i];
		for(j=0;j<3;j++)
		{
			/*double test=(myvertexs[now.index_vertex[j]].KG-Max_Min_InAll[1][1])/(0.1-Max_Min_InAll[1][1]);
			test=pow(test,100); //pow一下 使得高斯曲率分布的散一点
			key[j]=floor(test*7.0);
			key[j]=key[j]>6?6:key[j];
			key[j]=key[j]<0?0:key[j];*/
                        key[j]=floor((myvertexs[now.index_vertex[j]].KG-Max_Min_InAll[1][1])/(Max_Min_InAll[1][0]-Max_Min_InAll[1][1])*7.0);
		}
		
		glBegin(GL_POLYGON);
		{
			glColor3dv(physic::color[key[0]]);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glColor3dv(physic::color[key[1]]);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glColor3dv(physic::color[key[2]]);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);
		}
		glEnd();
		
	}
}

void object::drawwithKMax()
{
	int i,j;
	testface now;
	int key[3]; //当前face的三个顶点的颜色索引
	for(i=0;i<num_face;i++)
	{
		now=mytestfaces[i];
		for(j=0;j<3;j++)
		{
			/*key[j]=floor((myvertexs[now.index_vertex[j]].K_max-Max_Min_InAll[2][1])/(4-Max_Min_InAll[2][1])*7.0);
			key[j]=key[j]>6?6:key[j];
			key[j]=key[j]<0?0:key[j];*/
                        key[j]=floor((myvertexs[now.index_vertex[j]].K_max-Max_Min_InAll[2][1])/(Max_Min_InAll[2][0]-Max_Min_InAll[2][1])*7.0);
		}
		
		glBegin(GL_POLYGON);
		{
			glColor3dv(physic::color[key[0]]);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glColor3dv(physic::color[key[1]]);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glColor3dv(physic::color[key[2]]);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);
		}
		glEnd();
		
	}
}

void object::drawwithKMin()
{
	int i,j;
	testface now;
	int key[3]; //当前face的三个顶点的颜色索引
	for(i=0;i<num_face;i++)
	{
		now=mytestfaces[i];
		for(j=0;j<3;j++)
		{
			/*double test=(myvertexs[now.index_vertex[j]].K_min-Max_Min_InAll[3][1])/(0.06-Max_Min_InAll[3][1]);
			test=test>1?1:test;
			test=pow(test,10);
			key[j]=floor(test*7.0);
			key[j]=key[j]>6?6:key[j];
			key[j]=key[j]<0?0:key[j];*/
                        key[j]=floor((myvertexs[now.index_vertex[j]].K_min-Max_Min_InAll[3][1])/(Max_Min_InAll[3][0]-Max_Min_InAll[3][1])*7.0);
		}
		
		glBegin(GL_POLYGON);
		{
			glColor3dv(physic::color[key[0]]);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glColor3dv(physic::color[key[1]]);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glColor3dv(physic::color[key[2]]);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);
		}
		glEnd();
		
	}
}

void object::drawwithNormal()
{
	int i;
	int siz=mytestfaces.size();
	testface now;
	glColor3d(0.8,0.8,0.8);
	for(i=0;i<siz;i++)
	{
		now=mytestfaces[i];
		glBegin(GL_POLYGON);
		{
			glNormal3d(myvertexs[now.index_vertex[0]].normal.x,myvertexs[now.index_vertex[0]].normal.y,myvertexs[now.index_vertex[0]].normal.z);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glNormal3d(myvertexs[now.index_vertex[1]].normal.x,myvertexs[now.index_vertex[1]].normal.y,myvertexs[now.index_vertex[1]].normal.z);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glNormal3d(myvertexs[now.index_vertex[2]].normal.x,myvertexs[now.index_vertex[2]].normal.y,myvertexs[now.index_vertex[2]].normal.z);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);

                        /*glNormal3d(myvertexs[now.index_vertex[0]].normal_truth.x,myvertexs[now.index_vertex[0]].normal_truth.y,myvertexs[now.index_vertex[0]].normal_truth.z);
			glVertex3d(myvertexs[now.index_vertex[0]].location.x,myvertexs[now.index_vertex[0]].location.y,myvertexs[now.index_vertex[0]].location.z);
			glNormal3d(myvertexs[now.index_vertex[1]].normal_truth.x,myvertexs[now.index_vertex[1]].normal_truth.y,myvertexs[now.index_vertex[1]].normal_truth.z);
			glVertex3d(myvertexs[now.index_vertex[1]].location.x,myvertexs[now.index_vertex[1]].location.y,myvertexs[now.index_vertex[1]].location.z);
			glNormal3d(myvertexs[now.index_vertex[2]].normal_truth.x,myvertexs[now.index_vertex[2]].normal_truth.y,myvertexs[now.index_vertex[2]].normal_truth.z);
			glVertex3d(myvertexs[now.index_vertex[2]].location.x,myvertexs[now.index_vertex[2]].location.y,myvertexs[now.index_vertex[2]].location.z);*/
		}
		glEnd();
	}
}

int object::calData(int index[3],double data[3][3],double &area) //角度 0，需要的面积 1，边长 2
{
	int i;
	myvector here[3];
	int one,two;
	for(i=0;i<3;i++)
	{
		one=index[i%3]; two=index[(i+1)%3];
		here[i]=myvector(myvertexs[two].location-myvertexs[one].location);
		data[i][2]=here[i].len();
	}
	double cosData[3];
	int returnValue;
	for(i=0;i<3;i++)
	{
		cosData[i]=here[i].dot(here[(i+2)%3]*-1)/here[i].len()/here[(i+2)%3].len();
		data[i][0]=acos(cosData[i]); //弧度制
	}
	if(cosData[0]>=0&&cosData[1]>=0&&cosData[2]>=0)
	{
		returnValue=1;
	}
	else
	{
		returnValue=0;
	}
	
	if(returnValue==1)
	{
		for(i=0;i<3;i++)
		{
			data[i][1]=(here[i].len_sq()/tan(data[(i+2)%3][0])+here[(i+2)%3].len_sq()/tan(data[(i+4)%3][0]))/8.0;
		}
	}
	else
	{
		;
	}
	//海伦秦九韶公式
	double p=(data[0][2]+data[1][2]+data[2][2])/2.0;
	area=sqrtf(p*(p-data[0][2])*(p-data[1][2])*(p-data[2][2]));
	
	return returnValue;
}

//在object初始化时从txt中获得数据时即getObjData()中除了如何连接的数据以外的数据都应当在smooth过程中每次迭代时被修正
void object::calFaceInf()
{
	int i,j;
	int index[3];
	double data[3][3]; //每一个行向量保存计算一条有向边在一个逆时针围绕的三角形中左手边的角度，需要的面积 ，边长 
	double area; //当前处理的三角形的面积

	int one,two,help;

	int is;
	for(i=0;i<num_face;i++)
	{
		for(j=0;j<3;j++)
		{
			index[j]=mytestfaces[i].index_vertex[j];
		}
		is=calData(index,data,area);
		
		myfaces[i].area=area;
		myfaces[i].is_non_obtuse=is;
		
		myvector itself; //保存当前halfedge的向量形式
		
		int index_now_halfedge=myfaces[i].index_HE; //当前这个face的第一条halfedge的索引值
		for(j=0;j<3;j++)
		{
			one=index[j%3]; two=index[(j+1)%3]; //当前处理的halfedge的起点和终点
			itself=myvertexs[two].location-myvertexs[one].location;
			
			myhalfedges[index_now_halfedge].angle_accompany=data[j][0];
			myhalfedges[index_now_halfedge].angle_towards=data[(j+2)%3][0];
			myhalfedges[index_now_halfedge].area_accompany=data[j][1];
			myhalfedges[index_now_halfedge].itself=itself;
			myhalfedges[index_now_halfedge].length=data[j][2];
			index_now_halfedge=myhalfedges[index_now_halfedge].index_nextHE;
		}	
	}
}
void object::getObjData()
{

	string name="/home/wtyatzoo/project/model/cow.txt";
	FILE* file=fopen(name.c_str(),"r");
	fscanf(file,"%d%d",&num_vertex,&num_face);
	printf("%d %d \n",num_vertex,num_face);
	
	int i,j;
	double x,y,z;
	vertex here;
	char  filter[5];
	for(i=0;i<num_vertex;i++)
	{
		fscanf(file,"%s%lf%lf%lf",filter,&x,&y,&z);
		myvertexs.push_back(vertex(myvector(x,y,z)));
	}
	
	printf("%lf %lf %lf \n",x,y,z);
	
	map<pair<int,int >,int > mp1,mp2,mp3;
	map<pair<int,int >,int > ::iterator it;
	int index[3];
	int one,two,help;
	int mark;
	int is;
	
	double data[3][3]; //每一个行向量保存计算一条有向边在一个逆时针围绕的三角形中左手边的角度，需要的面积 ，边长 
	double area; //当前处理的三角形的面积
	for(i=0;i<num_face;i++)
	{
		fscanf(file,"%s%d%d%d",filter,&index[0],&index[1],&index[2]);
		for(j=0;j<3;j++)
		{
			index[j]--;
		}
		
		is=calData(index,data,area);

		int siz_begin=myhalfedges.size(); //这轮循环开始的size

		myfaces.push_back(face(siz_begin,area,is));
		mytestfaces.push_back(testface(index[0],index[1],index[2]));
		
		myvector itself; //保存当前halfedge的向量形式

		
		for(j=0;j<3;j++)
		{
			one=index[j%3]; two=index[(j+1)%3]; //当前处理的halfedge的起点和终点
			itself=myvertexs[two].location-myvertexs[one].location;
			if(myvertexs[one].index_HE_towards==-1)
			{
				myvertexs[one].index_HE_towards=siz_begin+j;
			}
			else
			{
				;
			}
			
			if(one<two)
			{
				mark=1;
			}
			else
			{
				help=one; one=two; two=help;
				mark=-1;
			}
			int siz=myhalfedges.size(); //当前halfedge数目，就是当前处理的halfedge的索引
			if(mp1.find(make_pair(one,two))==mp1.end())
			{	
				if(mark==1)
				{
					mp1[make_pair(one,two)]=1;
					mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
					myhalfedges.push_back(halfedge(two,one,i,siz_begin+(j+1)%3,siz_begin+((j-1)%3+3)%3,data[(j+2)%3][0],data[j][0],data[j][1],data[j][2],itself));
                    
				}
				else if(mark==-1)
				{
					mp1[make_pair(one,two)]=-1;  
					mp2[make_pair(one,two)]=siz; //保存这条halfedge的索引号，待其反向的halfedge出现时建立联系
					myhalfedges.push_back(halfedge(one,two,i,siz_begin+(j+1)%3,siz_begin+((j-1)%3+3)%3,data[(j+2)%3][0],data[j][0],data[j][1],data[j][2],itself));
				}
				
			}
			else if(mp1[make_pair(one,two)]==1||mp1[make_pair(one,two)]==-1)
			{
				int key=mp1[make_pair(one,two)];
				if(key==1||key==-1)
				{
					int oppHE=mp2[make_pair(one,two)];
					myhalfedges[oppHE].index_oppHE=siz;
					
					if(mark==1)
					{
						myhalfedges.push_back(halfedge(two,one,i,siz_begin+(j+1)%3,siz_begin+((j-1)%3+3)%3,oppHE,data[(j+2)%3][0],data[j][0],data[j][1],data[j][2],itself));
						mp1[make_pair(one,two)]=2;
					}
					else if(mark==-1)
					{
						myhalfedges.push_back(halfedge(one,two,i,siz_begin+(j+1)%3,siz_begin+((j-1)%3+3)%3,oppHE,data[(j+2)%3][0],data[j][0],data[j][1],data[j][2],itself));
						mp1[make_pair(one,two)]=2;  
					}
				}
			}
		}
		
	}
	
}

void object::calCurvatureAndNomal()
{
	int i,j;
	int siz=myvertexs.size();
	vertex vertex_here;
	int index_HE_begin; // one ring开始时的halfedge索引
	int index_now,index_now_opp; //当前halfedge的索引以及其反向halfedge的索引
	
	int index_face_now;
	myvector vector_up; //分子上的向量
	double area_mixed; //mixed面积
	double angle_sum; //one ring 角度和
	for(i=0;i<siz;i++)
	{
		vector_up=myvector(); //清零 
		area_mixed=0; //清零
		angle_sum=0;
		
		vertex_here=myvertexs[i];
		index_HE_begin=vertex_here.index_HE_towards;
		index_now=index_HE_begin; //当前的处理的halfedge索引
		index_now_opp=myhalfedges[index_now].index_oppHE;
		index_face_now=myhalfedges[index_now].index_face;
	
	//	printf("now:%d begin:%d begin_vertex:%d\n",index_now,index_HE_begin,myhalfedges[index_now].index_vertex_begin);
		do 
		{
			
			vector_up+=myhalfedges[index_now].itself*(1.0/tan(myhalfedges[index_now].angle_towards)+1.0/tan(myhalfedges[index_now_opp].angle_towards));
			
			angle_sum+=myhalfedges[index_now].angle_accompany;
			
			face face_here=myfaces[index_face_now];
			if(face_here.is_non_obtuse==1)
			{
				area_mixed+=myhalfedges[index_now].area_accompany;
			}
			else
			{
				if(cos(myhalfedges[index_now].angle_accompany)>=0) //钝角三角形  但是 该角不是钝角
				{
					area_mixed+=face_here.area/4.0;
				}
				else
				{
					area_mixed+=face_here.area/2.0;
				}
			}
			
			index_now=myhalfedges[index_now_opp].index_nextHE;
			index_now_opp=myhalfedges[index_now].index_oppHE;
			index_face_now=myhalfedges[index_now].index_face;

		//	printf("now:%d begin:%d begin_vertex:%d\n",index_now,index_HE_begin,myhalfedges[index_now].index_vertex_begin);
			
		} while (index_now!=index_HE_begin);

		//平均曲率向量
		myvector K=vector_up/area_mixed/2.0;
		
		myvertexs[i].area_mixed=area_mixed;
		myvertexs[i].KH=-1*K.len()/2;
		K.normalize();
		myvertexs[i].normal=K*-1;
		myvertexs[i].KG=(2*pi-angle_sum)/area_mixed;
		myvertexs[i].calPrincipalCurvature();
		
	}
}
