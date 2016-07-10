// 13 october 2015
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../../ui_richitexteditor.h"
#include "../../node_modules/libui-npm/ui.h"

uiWindow *mainwin;
uiArea *histogram;


static int onClosing(uiWindow *w, void *data)
{
	uiControlDestroy(uiControl(mainwin));
	uiQuit();
	return 0;
}

static int shouldQuit(void *data)
{
	uiControlDestroy(uiControl(mainwin));
	return 1;
}

int main(void)
{
	uiInitOptions o;
	const char *err;
	uiBox *hbox;
	memset(&o, 0, sizeof (uiInitOptions));
	err = uiInit(&o);
	if (err != NULL) {
		fprintf(stderr, "error initializing ui: %s\n", err);
		uiFreeInitError(err);
		return 1;
	}

	uiOnShouldQuit(shouldQuit, NULL);

	mainwin = uiNewWindow("libui Histogram Example", 640, 480, 1);
	uiWindowSetMargined(mainwin, 1);
	uiWindowOnClosing(mainwin, onClosing, NULL);

	hbox = uiNewHorizontalBox();
	uiBoxSetPadded(hbox, 1);
	uiWindowSetChild(mainwin, uiControl(hbox));

	histogram = uiNewRichTextArea();
	uiBoxAppend(hbox, uiControl(histogram), 1);

	uiControlShow(uiControl(mainwin));
	uiMain();
	uiUninit();
	return 0;
}
