/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

/* Module for measuring FPS for Allegro games - implementation.
   Written by Miran Amon.
*/


#include "fps.h"

FPS *create_fps(int fps) {
	int i;
	FPS *ret = malloc(sizeof(FPS));
	
	ret->nSamples = fps;
	ret->samples = malloc(fps*sizeof(int));
	for (i=0; i<fps; i++) {
		ret->samples[i] = 1;
	}
	ret->curSample = 0;
	ret->frameCounter = 0;
	ret->framesPerSecond = fps;
	return ret;
}


void destroy_fps(FPS *fps) {
	if (fps == NULL)
		return;

	free(fps->samples);
	fps->samples = NULL;
	fps->nSamples = 0;
	fps->frameCounter = 0;
	free(fps);
	fps = NULL;
}


void fps_tick(FPS *fps) {
	fps->curSample++;
	fps->curSample %= fps->nSamples;
	fps->framesPerSecond -= fps->samples[fps->curSample];
	fps->framesPerSecond += fps->frameCounter;
	fps->samples[fps->curSample] = fps->frameCounter;
	fps->frameCounter = 0;
}


void fps_frame(FPS *fps) {
	++(fps->frameCounter);
}


int get_fps(FPS *fps) {
	return fps->framesPerSecond;
}


void draw_fps(FPS *fps, BITMAP *bmp, FONT *font, int x, int y, int fg, char *format) {
	textprintf(bmp, font, x, y, fg, format, get_fps(fps));
}
