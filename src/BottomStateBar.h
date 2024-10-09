#pragma once

#include <GL/freeglut.h>
#include <string.h>
#include <stdlib.h>

#define		BSB_STATUS_MODE		1
#define		BSB_INPUT_MODE		2
#define		BSB_COMMAND_MODE	3

unsigned bsbState;

static int bsbHeight;
char* bsbOutputStream;
int bsbWarning;
static char bsbInputStream[128];
static unsigned end_in;
const static unsigned size_in = 128;

void bsbInit();
void bsbRender(int winWid, int winHei);
void bsbInputCharacter(char a);
void bsbInputBackspace();
char* bsbInputFinish();