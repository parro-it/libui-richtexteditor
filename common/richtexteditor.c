#include "../ui_richitexteditor.h"
#include <ui.h>

#define colorDodgerBlue 0x1E90FF

uiAreaHandler richTextArea_handler;
int richTextArea_initDone = 0;

static void setSolidBrush(uiDrawBrush *brush, uint32_t color, double alpha)
{
	uint8_t component;

	brush->Type = uiDrawBrushTypeSolid;
	component = (uint8_t) ((color >> 16) & 0xFF);
	brush->R = ((double) component) / 255;
	component = (uint8_t) ((color >> 8) & 0xFF);
	brush->G = ((double) component) / 255;
	component = (uint8_t) (color & 0xFF);
	brush->B = ((double) component) / 255;
	brush->A = alpha;
}

void richTextArea_draw(uiAreaHandler *self, uiArea *a, uiAreaDrawParams *p) {
	uiDrawPath *path;
	uiDrawBrush brush;

	// fill the area with white
	setSolidBrush(&brush, colorDodgerBlue, 1.0);
	path = uiDrawNewPath(uiDrawFillModeWinding);
	uiDrawPathAddRectangle(path, 0, 0, p->AreaWidth, p->AreaHeight);
	uiDrawPathEnd(path);
	uiDrawFill(p->Context, path, &brush);
	uiDrawFreePath(path);

}

void richTextArea_mouseEvent(uiAreaHandler *self, uiArea *a, uiAreaMouseEvent *m) {

}

void richTextArea_mouseCrossed(uiAreaHandler *self, uiArea *a, int left) {

}

void richTextArea_dragBroken(uiAreaHandler *self, uiArea *a) {

}

int richTextArea_keyEvent(uiAreaHandler *self, uiArea *a, uiAreaKeyEvent *k) {
	return 1;
}

void initHandler() {
	if (richTextArea_initDone) {
		return;
	}
	richTextArea_initDone = 1;

	richTextArea_handler.Draw = richTextArea_draw;
	richTextArea_handler.MouseEvent = richTextArea_mouseEvent;
	richTextArea_handler.MouseCrossed = richTextArea_mouseCrossed;
	richTextArea_handler.DragBroken = richTextArea_dragBroken;
	richTextArea_handler.KeyEvent = richTextArea_keyEvent;
}

_UI_EXTERN uiArea *uiNewRichTextArea() {
	initHandler();
	return uiNewArea(&richTextArea_handler);
}
