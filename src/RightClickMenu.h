#pragma once

#include <stdio.h>

#include <GL/freeglut.h>

int fileMenu, editMenu, colorMenu, drawTypeMenu, brushSizeMenu;
int mainMenu;

void fileMenuCallback(int);
void editFunc(int value);
void colorFunc(int value);
void drawTypeFunc(int value);
void brushSizeFunc(int value);
void CreateRCMenu();