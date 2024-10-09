#include "RightClickMenu.h"

#include "BottomStateBar.h"
#include "ShapeContainer.h"
#include "../main.h"

void fileMenuCallback(int opt) {
	printf("Selected option %d\n", WindowWidth);
}

void fileFunc(int value) {
	switch (value) {
	case 1:
		InitPainter();
		break;

	case 2:
		inputCmdState = IN_FILE;
		fileState = 0;
		bsbState = BSB_INPUT_MODE;
		bsbWarning = 1;
		bsbOutputStream = "Please input file path";
		break;

	case 3:
		inputCmdState = IN_FILE;
		fileState = 1;
		bsbState = BSB_INPUT_MODE;
		bsbWarning = 1;
		bsbOutputStream = "Please input file path";
		break;

	case 4:
		inputCmdState = IN_FILE;
		fileState = 2;
		bsbState = BSB_INPUT_MODE;
		bsbWarning = 1;
		bsbOutputStream = "Please input file path";
		break;

	case -1:
		exit(0);
		break;

	default:
		break;
	}
	displayFunction();
}

void editFunc(int value) {
	switch (value) {
	case 1:
		scUndo();
		break;
	case 2:
		scRedo();
		break;
	default:
		break;
	}
	displayFunction();
}

void colorFunc(int value) {
	switch (value) {
	case 2:
		Color[0] = .0f;
		Color[1] = .0f;
		Color[2] = .0f;
		Color[3] = 1.0f;
		break;

	case 1:
		Color[0] = 1.0f;
		Color[1] = 1.0f;
		Color[2] = 1.0f;
		Color[3] = 1.0f;
		break;

	case -1:
		bsbState = BSB_INPUT_MODE;
		inputCmdState = IN_COLOR;
		bsbOutputStream = "please input color, byte:\"124 233 100\" or float:\"0.2 0.4 0.23\" or hex:\"#42e3f5\"";
		break;
	}
	displayFunction();
}

void drawTypeFunc(int value) {
	ShapeType = value;
	first = 0;
}

void brushSizeFunc(int value) {
	switch (value) {
	case 1:
		Thickness -= 1.0f;
		break;
	case 2:
		Thickness += 1.0f;
	case -1:
		bsbState = BSB_INPUT_MODE;
		inputCmdState = IN_THICK;
		bsbOutputStream = "please input thickness";
		break;
	}
	if (Thickness < 1.0f) {
		Thickness = 1.0f;
	}
	displayFunction();
}

void CreateRCMenu() {
	fileMenu = glutCreateMenu(fileFunc);
	glutAddMenuEntry("New File", 1);
	glutAddMenuEntry("Load File", 2);
	glutAddMenuEntry("Save File", 3);
	glutAddMenuEntry("Blend File", 4);
	glutAddMenuEntry("Exit", -1);

	editMenu = glutCreateMenu(editFunc);
	glutAddMenuEntry("Undo", 1);
	glutAddMenuEntry("Redo", 2);

	colorMenu = glutCreateMenu(colorFunc);
	glutAddMenuEntry("White", 1);
	glutAddMenuEntry("Black", 2);
	glutAddMenuEntry("Input Color", -1);

	drawTypeMenu = glutCreateMenu(drawTypeFunc);
	glutAddMenuEntry("Point", GP_POINT);
	glutAddMenuEntry("Line", GP_LINE);
	glutAddMenuEntry("Brush", GP_CURVE);

	brushSizeMenu = glutCreateMenu(brushSizeFunc);
	glutAddMenuEntry("Smaller", 1);
	glutAddMenuEntry("Bigger", 2);
	glutAddMenuEntry("Input Thickness", -1);

	mainMenu = glutCreateMenu(fileMenuCallback);
	glutAddSubMenu("Files", fileMenu);
	glutAddSubMenu("Edit", editMenu);
	glutAddSubMenu("Colors", colorMenu);
	glutAddSubMenu("Brush Type", drawTypeMenu);
	glutAddSubMenu("Brush Size", brushSizeMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
