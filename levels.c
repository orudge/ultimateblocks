/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

void Change_Levels(void)
{
	int l, i, j;
	BITMAP *temp2 = create_bitmap(SCREEN_W, SCREEN_H);

	blit(temp, temp2, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	clear(temp);

	l = lev;

	Map_to_Maps(100);

	while(key[KEY_ESC])
	{
		Poll_Music();
	}

	while(key[KEY_ENTER])
	{
		Poll_Music();
	}

	while ((!key[KEY_ESC]) && (!key[KEY_ENTER]))
	{
		Maps_to_Map(l);
		Change_Motif(map_motif[l]);
		Draw_Map();
		text_mode(-1);

		textprintf(temp, fonts[0].dat, 10, 9, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat, 11, 9, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat, 11, 10, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat, 11, 11, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat, 10, 11, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat,  9, 11, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat,  9, 10, 0, "Level: %d", l + 1);
		textprintf(temp, fonts[0].dat,  9,  9, 0, "Level: %d", l + 1);

		textprintf(temp, fonts[0].dat, 10, 10, 7, "Level: %d", l + 1);

		if (map_done[l])
			masked_blit(tick_pic[1], temp, 0, 0, SCREEN_W - (_block_width + 15), SCREEN_H - (_block_height + 10), _block_width, _block_height);

		blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		while ((key[KEY_UP]) || (key[KEY_DOWN]) ||
				(key[KEY_LEFT]) || (key[KEY_RIGHT]))
		{
			Poll_Music();
		}

		while ((!key[KEY_UP]) && (!key[KEY_DOWN]) &&
			(!key[KEY_LEFT]) && (!key[KEY_RIGHT]) &&
			(!key[KEY_ESC]) && (!key[KEY_ENTER]))
		{
			Poll_Music();
		}

		if (key[KEY_ESC])
		{
			Maps_to_Map(100);
			Change_Motif(map_motif[lev]);

			for (j = 0; j < 15; j+=2)
			{
				for (i = 0; i < 20; i+=2)
				{
					blit(gfx, back, 0, 5*_block_height_depth, i*_block_width, j*_block_height, _block_width*2, _block_height*2);
				}
			}

			blit(temp2, temp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

			while(key[KEY_ESC])
			{
				Poll_Music();
			}

			return;
		}

		if (key[KEY_ENTER])
		{
			game_exit = 2;
			lev = l;
			return;
		}

		if (key[KEY_LEFT]) l--;
		if (key[KEY_RIGHT]) l++;
		if (key[KEY_UP]) l+=10;
		if (key[KEY_DOWN]) l-=10;

		if (l < 0) l+=100;
		if (l > 99) l-=100;
	}

	report_error("DEBUG ERROR: exit(66)");
}






