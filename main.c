#include "main.h"

#include "src/MouseEventHandler.h"
#include "src/RightClickMenu.h"
#include "src/ShapeContainer.h"

void displayFunction() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the shape in the shape container
    int isBegin = 0;
    int currentShape = 0;
    for (int i = 0; i < scSize(); i++) {
        float* c;
        int* a;
        switch (shapeContainer[i].infoType) {
        case 1: /* Shape */
            currentShape = shapeContainer[i].info.shape;
            break;
        case 2: /* Thickness */
            glPointSize(shapeContainer[i].info.thickness);
            glLineWidth(shapeContainer[i].info.thickness);
            break;
        case 3: /* Color */
            c = shapeContainer[i].info.color;
            glColor4f(c[0], c[1], c[2], c[3]);
            break;
        case 4: /* Axis */
            if (isBegin == 0) {
                isBegin = 1;
                glBegin(currentShape);
            }
            a = shapeContainer[i].info.axis;
            glVertex2i(a[0], WindowHeight - a[1]);
            break;
        case 0: /* End */
            glEnd();
            isBegin = 0;
            break;
        default:
            break;
        }
    }

    // Draw auxiliary shape
    if (isDragging) {
        glPointSize(Thickness);
        glLineWidth(Thickness);
        glColor4f(Color[0], Color[1], Color[2], Color[3]);
        switch (ShapeType) {
        case GP_LINE:
            glBegin(GL_LINES);
            glVertex2i(posX, WindowHeight - posY);
            glVertex2i(cursorX, WindowHeight - cursorY);
            glEnd();
            break;
        case GP_CURVE:
			// DON'T TOUCH THESE CODE
			// It will crash if I don't put these 2 lines of code
			glBegin(GL_LINE_STRIP);
			glEnd();
            break;
        default:
            break;
        }
    }

    glFinish();
}

void reshapeFunction(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WindowWidth, 0.0, WindowHeight);
	glViewport(0, 0, WindowWidth, WindowHeight);
	glMatrixMode(GL_MODELVIEW);

	glFlush();
}

void keyboardFunction(unsigned char key, int x, int y) {
	int mod = glutGetModifiers();
	switch (key) {
	case 26:
		if (mod == 2) {
			scUndo();
		}
		else if (mod == 3) {
			scRedo();
		}
		break;

	default:
		break;
	}
	displayFunction();
}

void motionFunction(int btn, int x, int y) {
	isDragging = true;
	if (btn == 0) {
		switch (ShapeType) {
		case GP_LINE:
			cursorX = x;
			cursorY = y;
			displayFunction();
			break;

		case GP_CURVE:
            glBegin(GL_LINES);
            glVertex2i(posX, WindowHeight - posY);
            glVertex2i(x, WindowHeight - y);
            glEnd();
            scPushAxis(posX, posY);
            posX = x;
            posY = y;
            scPushAxis(posX, posY);
			displayFunction();
            break;
		default:
			break;
		}
		
	}
	isDragging = false;
}

void mouseFunction(int button, int state, int x, int y) {
	if (button == 0) {
		if (state == 0) {
			switch (ShapeType) {
			case GP_POINT:
				glBegin(GL_POINTS);
				glVertex2i(x, WindowHeight - y);
				glEnd();

				scPushShape(GL_POINTS);
				scPushThick(Thickness);
				scPushColor(Color);
				scPushAxis(x, y);
				scPushEnd();
				break;

			case GP_CURVE:
                scPushShape(GL_LINE_STRIP);
                scPushThick(Thickness);
                scPushColor(Color);
			case GP_LINE:
				posX = x;
				posY = y;
				break;

			default:
				break;
			}
		}
		else {
			switch (ShapeType) {
			case GP_LINE:
				glBegin(GL_LINES);
				glVertex2i(posX, WindowHeight - posY);
				glVertex2i(x, WindowHeight - y);
				glEnd();

				scPushShape(GL_LINES);
				scPushThick(Thickness);
				scPushColor(Color);
				scPushAxis(posX, posY);
				scPushAxis(x, y);
				scPushEnd();
				break;

            case GP_CURVE:
                scPushEnd();

			default:
				break;
			}
		}
	}
	glFlush();
}

void InitPainter() {
	ShapeType = GP_LINE;
	Thickness = 2.5f;
	glPointSize(Thickness);
	glLineWidth(Thickness);
	Color[0] = .0f;
	Color[1] = .0f;
	Color[2] = .0f;
	Color[3] = 1.0f;
	glColor4f(Color[0], Color[1], Color[2], Color[3]);
	first = 0;
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);

	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WindowWidth, WindowHeight);

	glutCreateWindow("Good Painter");

	// Setup recall functions
	glutDisplayFunc(displayFunction);
	glutReshapeFunc(reshapeFunction);
	glutKeyboardFunc(keyboardFunction);
	SetupMouseFunc(mouseFunction, motionFunction);

	// Creating right click menu
	CreateRCMenu();

	// Initialize shape container
	scInit();
	InitPainter();

	glutMainLoop();
}
