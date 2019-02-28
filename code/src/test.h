#ifndef _TEST_
#define _TEST_

#include <math.h>
#include "object.h"
#include "camera.h"

object* myobject;
camera* mycamera;
int kind; //着色的种类: 平均 高斯 最大 最小 normal 
int count;//拍照计数

void saveAsVTK();
void drawObject();
void keyboard(unsigned char key,int x,int y);
void reshape(int w,int h) ;
void init();
void lightControl();
void makeSmooth();
void display(void);
void drawObject();
void saveSceneImage();
void saveBmp(const char* name,int width,int height,unsigned char* data);
#endif
