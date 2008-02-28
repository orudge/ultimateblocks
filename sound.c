/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Add_Sound(int x, int s)
{
	if (fx_count < MAX_SOUND_EFFECTS)
	{
		fx[fx_count].x = x*255/_block_width;
		fx[fx_count].s = s;

		fx_count++;
	}
}

void Play_Sound(void)
{
	int i;

	for (i = 0; i < fx_count; i++)
	{
		play_sample(sfx[fx[i].s].dat, 255, fx[i].x, 1000, 0);
	}

	fx_count = 0;
}


