#include "BottomStateBar.h"

#include <stdio.h>

void bsbInit() {
    bsbState = BSB_STATUS_MODE;
    bsbHeight = 25;
    bsbWarning = 0;
    bsbOutputStream = (char*)malloc(256);  // Allocate memory for output stream
    if (bsbOutputStream) {
        bsbOutputStream[0] = '\0';  // Initialize as an empty string
    }
    end_in = 0;
}

static char* getInputStream() {
    if (end_in == 0) return "";

    char* tmp = (char*) malloc(end_in + 1);
    if (tmp) {
        tmp[end_in] = '\0';
        strncpy(tmp, bsbInputStream, end_in);
    }
    return tmp;
}

void bsbRender(int winWid, int winHei) {

    glFlush();

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2i(5, 7);

    if (bsbOutputStream != NULL) {
        switch (bsbState) {
        case BSB_STATUS_MODE:
            glutBitmapString(GLUT_BITMAP_HELVETICA_12, bsbOutputStream);
            break;

        case BSB_COMMAND_MODE:
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ':');
            if (bsbInputStream != NULL) {
                glutBitmapString(GLUT_BITMAP_HELVETICA_12, getInputStream());
            }
            break;

        case BSB_INPUT_MODE:
            if (bsbInputStream != NULL) {
                glutBitmapString(GLUT_BITMAP_HELVETICA_12, getInputStream());
            }
            glutBitmapString(GLUT_BITMAP_HELVETICA_12, "       ");
            glColor3f(1.0f, 0.0f, 0.0f);
            glRasterPos2i(5, 20);
            glutBitmapString(GLUT_BITMAP_HELVETICA_12, bsbOutputStream);
            break;
        }

    }
}

void bsbInputCharacter(char a) {
    if (bsbState == BSB_STATUS_MODE || end_in == size_in) return;

    bsbInputStream[end_in++] = a;
}

void bsbInputBackspace() {
    if (end_in == 0) return;
    end_in -= 1;
}

char* bsbInputFinish() {
    char* tmp = getInputStream();
    end_in = 0;
    return tmp;
}