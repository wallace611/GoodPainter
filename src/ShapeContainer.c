#include <stdlib.h>
#include <stdio.h>

#include "ShapeContainer.h"

int scInit() {
	shapeContainer = 0;
	undoContainer = 0;
	size_sc = SC_DEFAULT_SIZE;
	size_uc = UC_DEFAULT_SIZE;
	shapeContainer = (shapeInfo*) malloc(size_sc * sizeof(shapeInfo));
	undoContainer = (shapeInfo*) malloc(size_uc * sizeof(shapeInfo));

	if (shapeContainer == NULL || undoContainer == NULL) {
		// Check exception
		return -1;
	}

	end_sc = 0;
	end_uc = 0;
	return 1;
}

unsigned scSize() {
	return end_sc;
}

int scPushBack(shapeInfo info) {
	ucClear();
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
	while (scBack().infoType != 1) {
		// When the space is not enough
		if (end_uc == size_uc) {
			size_uc <<= 1;
			shapeInfo* tmp = (shapeInfo*) realloc(undoContainer, size_uc * sizeof(shapeInfo));

			if (tmp == NULL) {
				// Exception
				size_uc >>= 1;
				return -1;
			}
			undoContainer = tmp;
		}

		undoContainer[end_uc++] = scPop();
	}
	undoContainer[end_uc++] = scPop();
	return 1;
}

int scRedo() {
	if (end_uc == 0) return -1;
	while (ucBack().infoType != 0) {
		scPushBackWithoutClear(ucPop());
	}
	scPushBackWithoutClear(ucPop());
	return 1;
}

void ucClear() {
	int sTmp = size_uc;
	size_uc = UC_DEFAULT_SIZE;
	shapeInfo* tmp = (shapeInfo*) realloc(undoContainer, size_uc * sizeof(shapeInfo));

	if (tmp != NULL) {
		undoContainer = tmp;
		size_uc = sTmp;
	}

	end_uc = 0;
}

shapeInfo ucPop() {
	if (end_uc == 0) {
		shapeInfo tmp = { .infoType = -1 };
		return tmp;
	}

	end_uc -= 1;
	if (end_uc < (size_uc >> 2) && size_uc > UC_DEFAULT_SIZE) {
		size_uc >>= 2;
		shapeInfo* tmp = (shapeInfo*) realloc(undoContainer, size_uc * sizeof(shapeInfo));

		if (tmp == NULL) {
			size_uc <<= 2;
			shapeInfo tmp = { .infoType = -1 };
			return tmp;
		}
		undoContainer = tmp;
	}
	return undoContainer[end_uc];
}

shapeInfo ucBack() {
	if (end_uc == 0) {
		shapeInfo tmp = { .infoType = -1 };
		return tmp;
	}
	return undoContainer[end_uc - 1];
}

int scPushBackWithoutClear(shapeInfo info) {
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
	return 1;
}


