#pragma once

#include <stdio.h>
#include <stdlib.h>    
#include <stdbool.h>

#include <GL/freeglut.h>

#define    GP_POINT   1
#define    GP_LINE	  2
#define    GP_CURVE	  3

const static int WINDOW_POS_X = 1000;
const static int WINDOW_POS_Y = 500;

static int WindowWidth = 800;
static int WindowHeight = 600;

int ShapeType;
int Thickness;
float Color[4];

int first; /* Flag of initial point for lines and curve */

static bool isDragging = false;
int posX, posY;  /* Point positioin */
int cursorX, cursorY; /* Cursor position, for auxiliary shape */

void displayFunction();
void reshapeFunction(int w, int h);
void keyboardFunction(unsigned char key, int x, int y);
void motionFunction(int btn, int x, int y);
void mouseFunction(int button, int state, int x, int y);
void InitPainter();