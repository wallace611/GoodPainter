#include "RightClickMenu.h"

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
		scLoadFromFile("test.dat");
		break;

	case 3:
		scSaveToFile("test.dat");
		break;

	case 4:
		scBlendFromFile("test.dat");
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
	default:
		break;
	}
	if (Thickness < 1.0f) {
		Thickness = 1.0f;
	}
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

	drawTypeMenu = glutCreateMenu(drawTypeFunc);
	glutAddMenuEntry("Point", GP_POINT);
	glutAddMenuEntry("Line", GP_LINE);
	glutAddMenuEntry("Brush", GP_CURVE);

	brushSizeMenu = glutCreateMenu(brushSizeFunc);
	glutAddMenuEntry("Smaller", 1);
	glutAddMenuEntry("Bigger", 2);

	mainMenu = glutCreateMenu(fileMenuCallback);
	glutAddSubMenu("Files", fileMenu);
	glutAddSubMenu("Edit", editMenu);
	glutAddSubMenu("Colors", colorMenu);
	glutAddSubMenu("Brush Type", drawTypeMenu);
	glutAddSubMenu("Brush Size", brushSizeMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
