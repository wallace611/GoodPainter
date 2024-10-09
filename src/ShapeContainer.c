#include <stdlib.h>
#include <stdio.h>

#include "ShapeContainer.h"

int scSaveToFile(char* path) {
	FILE* f = fopen(path, "wb");
	if (!f) {
		perror("Failed to open file");
		return -1;
	}
	fwrite(&end_sc, sizeof(unsigned), 1, f);
	fwrite(shapeContainer, sizeof(shapeInfo), end_sc, f);
	fclose(f);

	return 0;
}

int scLoadFromFile(char* path) {
	FILE* ifp = fopen(path, "rb");
	if (!ifp) {
		perror("Failed to open file");
		return -1;
	}

	unsigned newSize;
	fread(&newSize, sizeof(unsigned), 1, ifp);
	shapeInfo* newContainer = (shapeInfo*)malloc(newSize * sizeof(shapeInfo));
	if (!newContainer) {
		perror("Failed to allocate memory");
		fclose(ifp);
		return -1;
	}

	fread(newContainer, sizeof(shapeInfo), newSize, ifp);

	free(shapeContainer);
	shapeContainer = newContainer;
	end_sc = newSize;
	size_sc = newSize;
	fclose(ifp);

	return 0;
}

int scBlendFromFile(char* path) {
	FILE* ifp = fopen(path, "rb");
	if (!ifp) {
		perror("Failed to open file");
		return -1;
	}

	unsigned newSize;
	fread(&newSize, sizeof(unsigned), 1, ifp);
	shapeInfo* newContainer = (shapeInfo*)malloc(newSize * sizeof(shapeInfo));
	if (!newContainer) {
		perror("Failed to allocate memory");
		fclose(ifp);
		return -1;
	}

	fread(newContainer, sizeof(shapeInfo), newSize, ifp);

	for (int i = 0; i < newSize; i++) {
		scPushBack(newContainer[i]);
	}
	fclose(ifp);

	return 0;
}

int scInit() {
	free(shapeContainer);
	size_sc = SC_DEFAULT_SIZE;
	shapeContainer = (shapeInfo*) malloc(size_sc * sizeof(shapeInfo));

	if (shapeContainer == NULL) {
		// Check exception
		return -1;
	}

	end_sc = 0;
	canRedo = 0;
	return 1;
}

unsigned scSize() {
	return end_sc;
}

int scPushBack(shapeInfo info) {
	if (end_sc <= (size_sc >> 2) && size_sc > SC_DEFAULT_SIZE) {
		size_sc >>= 1;
		shapeInfo* tmp = (shapeInfo*) realloc(shapeContainer, size_sc * sizeof(shapeInfo));
		if (tmp == NULL) {
			size_sc <<= 1;
			return -1;
		}
		shapeContainer = tmp;
	}
	if (end_sc == size_sc) {
		size_sc <<= 1;
		shapeInfo* tmp = (shapeInfo*) realloc(shapeContainer, size_sc * sizeof(shapeInfo));
		if (tmp == NULL) {
			// Check exception
			size_sc >>= 1;
			return -1;
		}
		shapeContainer = tmp;
	}
	shapeContainer[end_sc++] = info;
	canRedo = 0;
	return 1;
}

void scPushShape(int shape) {
	shapeInfo tmp = { .infoType = 1 };
	tmp.info.shape = shape;
	scPushBack(tmp);
}

void scPushThick(int thickness) {
	shapeInfo tmp = { .infoType = 2 };
	tmp.info.thickness = thickness;
	scPushBack(tmp);
}

void scPushColor(float color[4]) {
	shapeInfo tmp = { .infoType = 3 };
	tmp.info.color[0] = color[0];
	tmp.info.color[1] = color[1];
	tmp.info.color[2] = color[2];
	tmp.info.color[3] = color[3];
	scPushBack(tmp);
}

void scPushAxis(int x, int y) {
	shapeInfo tmp = { .infoType = 4 };
	tmp.info.axis[0] = x;
	tmp.info.axis[1] = y;
	scPushBack(tmp);
}

void scPushEnd() {
	shapeInfo tmp = { .infoType = 0 };
	scPushBack(tmp);
}

shapeInfo scPop() {
	if (end_sc == 0) {
		shapeInfo tmp = { .infoType = -1 };
		return tmp;
	}
	end_sc -= 1;
	if (end_sc < (size_sc >> 2) && size_sc > SC_DEFAULT_SIZE) {
		size_sc >>= 2;
		shapeInfo* tmp = (shapeInfo*) realloc(shapeContainer, size_sc * sizeof(shapeInfo));
		if (tmp == NULL) {
			size_sc <<= 2;
			shapeInfo tmp = { .infoType = -1 };
			return tmp;
		}
		shapeContainer = tmp;
	}
	return shapeContainer[end_sc];
}

shapeInfo scBack() {
	if (end_sc == 0) {
		shapeInfo tmp = { .infoType = -1 };
		return tmp;
	}
	return shapeContainer[end_sc - 1];
}

int scUndo() {
	if (end_sc == 0) return -1;
	canRedo = 1;
	while (scBack().infoType != 1) {
		end_sc -= 1;
	}
	end_sc -= 1;
	return 1;
}

int scRedo() {
	if (end_sc == size_sc || !canRedo) {
		return -1;
	}
	end_sc += 1;
	while (scBack().infoType != 0) {
		end_sc += 1;
	}
	return 1;
}


