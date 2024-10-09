#include "main.h"

#include "src/BottomStateBar.h"
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

	if (bsbState == BSB_STATUS_MODE) {
		sprintf(bsbOutputStream,
			"ShapeType: %d | Thickness: %.2f | Color: (%.2f, %.2f, %.2f, %.2f)",
			ShapeType,
			Thickness,
			Color[0], Color[1], Color[2], Color[3]);
	}
	bsbRender(WindowWidth, WindowHeight);

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

static void parseColor(const char* commandStr, float* color) {
	int r, g, b, a = 255;  // Default alpha to 255 (fully opaque)
	float rf, gf, bf, af = 1.0f;
	unsigned int hexColor;

	// Try parsing as integers (e.g., "255 255 255" or "255 255 255 255")
	int numParsed = sscanf(commandStr, "%d %d %d %d", &r, &g, &b, &a);
	if (numParsed >= 3) {
		color[0] = r / 255.0f;
		color[1] = g / 255.0f;
		color[2] = b / 255.0f;
		color[3] = a / 255.0f;
		return;
	}

	// Try parsing as floats (e.g., "1.0 0.5 0.0" or "1.0 0.5 0.0 0.8")
	numParsed = sscanf(commandStr, "%f %f %f %f", &rf, &gf, &bf, &af);
	if (numParsed >= 3) {
		color[0] = rf;
		color[1] = gf;
		color[2] = bf;
		color[3] = af;
		return;
	}

	// Try parsing as a hexadecimal string (e.g., "#FF00FF" or "FF00FF80")
	if (commandStr[0] == '#') {
		commandStr++;  // Skip the '#' character
	}
	numParsed = sscanf(commandStr, "%x", &hexColor);
	if (numParsed == 1) {
		if (strlen(commandStr) == 6) {  // RGB format
			color[0] = ((hexColor >> 16) & 0xFF) / 255.0f;
			color[1] = ((hexColor >> 8) & 0xFF) / 255.0f;
			color[2] = (hexColor & 0xFF) / 255.0f;
			color[3] = 1.0f;  // Default alpha
		} else if (strlen(commandStr) == 8) {  // RGBA format
			color[0] = ((hexColor >> 24) & 0xFF) / 255.0f;
			color[1] = ((hexColor >> 16) & 0xFF) / 255.0f;
			color[2] = ((hexColor >> 8) & 0xFF) / 255.0f;
			color[3] = (hexColor & 0xFF) / 255.0f;
		}
		return;
	}

	// If parsing fails, set a default color (e.g., black)
	color[0] = color[1] = color[2] = 0.0f;
	color[3] = 1.0f;
}

void keyboardFunction(unsigned char key, int x, int y) {
	int mod = glutGetModifiers();
	if (bsbState == BSB_STATUS_MODE) {
		switch (key) {
		case 26: // ctrl + z
			if (mod == 2) {
				scUndo();
			}
			else if (mod == 3) {
				scRedo();
			}
			break;

		case 27: // esc
			bsbState = BSB_STATUS_MODE;
			if (ShapeType == GP_POLYGON) {
				glEnd();
				scPushEnd();
				glFlush();
			}
			break;

		case ':':
			bsbState = BSB_COMMAND_MODE;
			break;

		default:
			break;
		}
	}
	else { // Input mode or command mode
		switch (key) {
		case 8: // backspace
			bsbInputBackspace();
			break;
		case 13: // enter
			commandStr = bsbInputFinish();
			if (commandStr != NULL) {
				if (bsbState == BSB_INPUT_MODE) {
					switch (inputCmdState) {
					case IN_COLOR:
						parseColor(commandStr, Color);
						break;

					case IN_THICK:
						sscanf(commandStr, "%f", &Thickness);
						break;

					case IN_FILE:
						switch (fileState) {
						case 0:
							scLoadFromFile(commandStr);
							break;

						case 1:
							scSaveToFile(commandStr);
							break;

						case 2:
							scBlendFromFile(commandStr);
							break;
						}
						displayFunction();
					}
				}
				else if (bsbState == BSB_COMMAND_MODE) {
					if (commandStr[0] == 'c') {
						InitPainter();
					} 
					else if (commandStr[0] == 'q') {
						exit(0);
					}
				}
			}
			bsbState = BSB_STATUS_MODE;
			break;

		default:
			bsbInputCharacter(key);
			break;
		}
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

			case GP_POLYGON:
				if (first == 0) {
					glBegin(GL_POLYGON);
					scPushShape(GL_POLYGON);
					scPushThick(Thickness);
					scPushColor(Color);
					glVertex2i(x, WindowHeight - y);
					scPushAxis(x, y);
					side = 1;
				}
				else {
					scPushColor(Color);
					glVertex2i(x, WindowHeight - y);
					scPushAxis(x, y);
					side += 1;
				}

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	first = 0;

	scInit();
	bsbInit();

	displayFunction();
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
