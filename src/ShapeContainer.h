/* 這個容器用以儲存所有畫過的形狀 */
#pragma once

typedef union {
	int shape;
	float thickness;
	int axis[2];
	float color[4];
} shape_t;

typedef struct {
	// 1: shape, 2: thickness, 3: color, 4: axis, 0: end, -1: exception
	int infoType;
	shape_t info;
} shapeInfo;

#define SC_DEFAULT_SIZE 1024
#define UC_DEFAULT_SIZE 512

shapeInfo* shapeContainer; /* The array */
unsigned size_sc;		   /* The current size of the array*/
unsigned end_sc;	       /* Pointing to the back of the array */
int canRedo;

int scSaveToFile(char* path);
int scLoadFromFile(char* path);
int scBlendFromFile(char* path);
int scInit();
unsigned scSize();
int scPushBack(shapeInfo info);
void scPushShape(int shape);
void scPushThick(int thickness);
void scPushColor(float color[4]);
void scPushAxis(int x, int y);
void scPushEnd();
shapeInfo scPop();
shapeInfo scBack();
int scUndo();
int scRedo();