/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <aldumb.h>
#include <libcda.h>
#include <string.h>
#include "blocks3.h"

int cd_track = -1;

void CD_Player(void)
{
	BITMAP *temp2 = create_bitmap(200, 300);
	BITMAP *temp3 = create_bitmap(640, 480);

	int cd_exit, item;
	int first, last, i;

	cd_exit = 0;
	item = 0;

	blit(screen, temp3, 0, 0, 0, 0, 640, 480);
	rect(temp2, 0, 0, 199, 299, 0);

	rectfill(temp2, 1, 1, 198, 298, 7);
	rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
	rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
	rect(temp2, 10, 10, 189, 20, 0);
	rect(temp2, 10, 279, 189, 289, 0);

	rectfill(temp2, 10, item*40 + 50, 190, item*40 + 80, makecol(180, 180, 255));
	text_mode(-1);

	if (cd_track == -1)
		textprintf_centre(temp2, fonts[0].dat, 100, 50,  0, "Play");
	else
		textprintf_centre(temp2, fonts[0].dat, 100, 50,  0, "Stop");

	textprintf_centre(temp2, fonts[0].dat, 100, 90, 0, "Next");
	textprintf_centre(temp2, fonts[0].dat, 100, 130, 0, "Previous");
	textprintf_centre(temp2, fonts[0].dat, 100, 170, 0, "Eject");
	textprintf_centre(temp2, fonts[0].dat, 100, 210, 0, "Exit");

	for (i = -200; i < 230; i+=10)
	{
		blit(temp3, screen, i - 10, 100, i-10, 100, 10, 300);
		blit(temp2, screen, 0, 0, i, 100, 200, 300);
		while (time_count < 1);
		time_count = 0;
	}

	while(!cd_exit)
	{
		rectfill(temp2, 1, 1, 198, 298, 7);
		rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
		rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
		rect(temp2, 10, 10, 189, 20, 0);
		rect(temp2, 10, 279, 189, 289, 0);

		rectfill(temp2, 10, item*40 + 50, 190, item*40 + 80, makecol(180, 180, 255));
		text_mode(-1);

		if (cd_track == -1)
			textprintf_centre(temp2, fonts[0].dat, 100, 50,  0, "Play");
		else
			textprintf_centre(temp2, fonts[0].dat, 100, 50,  0, "Stop");

		textprintf_centre(temp2, fonts[0].dat, 100, 90, 0, "Next");
		textprintf_centre(temp2, fonts[0].dat, 100, 130, 0, "Previous");
		textprintf_centre(temp2, fonts[0].dat, 100, 170, 0, "Eject");
		textprintf_centre(temp2, fonts[0].dat, 100, 210, 0, "Exit");

		blit(temp2, screen, 0, 0, 220, 100, 200, 300);

		while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]));

		if (key[KEY_ESC]) cd_exit = 1;

		if (key[KEY_ENTER])
		{
			if (item == 4)
				cd_exit = 1;

			if (item == 3)
			{
				cd_eject();
				cd_track = -1;
			}

			if (item == 0)
			{
				if (cd_track != -1)
				{
					cd_stop();
					cd_track = -1;
				}
				else
				{
					if (cd_play_from(1) != -1)
						cd_track = 1;
				}
			}

			if (item == 1)
			{
				cd_get_tracks(&first, &last);
				cd_track = cd_current_track();

				cd_track++;

				if (cd_track > last)
					cd_track = first;

				if (cd_play_from(cd_track) == -1)
					cd_track = -1;

				text_mode(0);
				textprintf(screen, font, 10, 10, 7, "%d ", cd_track);
			}

			if (item == 2)
			{
				cd_get_tracks(&first, &last);
				cd_track = cd_current_track();

				cd_track--;

				if (cd_play_from(cd_track) == -1)
					cd_track = -1;

				text_mode(0);
				textprintf(screen, font, 10, 10, 7, "%d ", cd_track);
			}

			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
		}
		else
			play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

		if (key[KEY_UP])
			item--;

		if (key[KEY_DOWN])
			item++;

		if (item < 0)
			item = 4;

		if (item > 4)
			item = 0;

		while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]));
	}

	for (i = 220; i < 650; i+=10)
	{
		blit(temp3, screen, i - 10, 100, i-10, 100, 10, 300);
		blit(temp2, screen, 0, 0, i, 100, 200, 300);
		while (time_count < 1);
		time_count = 0;
	}

	blit(temp3, screen, 0, 0, 0, 0, 640, 480);
}

void Mod_Music(void)
{
	BITMAP *temp2 = create_bitmap(200, 300);
	BITMAP *temp3 = create_bitmap(640, 480);

	int cd_exit, item;
	int i;

	cd_exit = 0;
	item = 0;

	blit(screen, temp3, 0, 0, 0, 0, 640, 480);
	rect(temp2, 0, 0, 199, 189, 0);

	rectfill(temp2, 1, 1, 198, 188, 7);
	rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
	rectfill(temp2, 10, 170, 189, 180, makecol(128, 128, 128));
	rect(temp2, 10, 10, 189, 20, 0);
	rect(temp2, 10, 170, 189, 180, 0);

	rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
	text_mode(-1);

	textprintf_centre(temp2, fonts[0].dat, 100, 40, 0, "Next");
	textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "Previous");
	textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "Return");

	for (i = -200; i < 230; i+=10)
	{
		blit(temp, screen, i - 10, 100, i-10, 100, 10, 190);
		blit(temp2, screen, 0, 0, i, 100, 200, 190);
		while (time_count < 1);
		time_count = 0;
	}

	cd_exit = 0;
	item = 0;

	while (!cd_exit)
	{
		rectfill(temp2, 1, 1, 198, 188, 7);
		rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
		rectfill(temp2, 10, 170, 189, 180, makecol(128, 128, 128));
		rect(temp2, 10, 10, 189, 20, 0);
		rect(temp2, 10, 170, 189, 180, 0);

		rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
		text_mode(-1);

		textprintf_centre(temp2, fonts[0].dat, 100, 40,  0, "Next");
		textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "Previous");
		textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "Return");

		blit(temp2, screen, 0, 0, 220, 100, 200, 190);

		while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]))
		{
			Poll_Music();
		}

		if (key[KEY_ESC]) cd_exit = 1;

		if (key[KEY_ENTER])
		{
			if (item != 2)
			{
				if (item == 0)
					mod_track++;

				if (item == 1)
					mod_track--;

				if (mod_track < 0)
					mod_track = mod_last;

				if (mod_track > mod_last)
					mod_track = 0;

				Play_MOD_Track(mod[mod_track].name, mod[mod_track].type, 1); // todo: specify whether should loop?
			}
			else
				cd_exit = 1;

			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
		}
		else
			play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

		if (key[KEY_UP])
			item--;

		if (key[KEY_DOWN])
			item++;

		if (item < 0)
			item = 2;

		if (item > 2)
			item = 0;

		while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]))
		{
			Poll_Music();
		}
	}

	for (i = 220; i < 650; i+=10)
	{
		blit(temp3, screen, i - 10, 100, i-10, 100, 10, 190);
		blit(temp2, screen, 0, 0, i, 100, 200, 190);
		while (time_count < 1);
		time_count = 0;
	}

	blit(temp3, screen, 0, 0, 0, 0, 640, 480);
}

char *mod_current_fn;
int mod_current_type;
char mod_current_loop;

void Play_MOD_Track(char *filename, int type, char loop)
{
	al_stop_duh(mod_player);
	mod_player = NULL;

	unload_duh(music);

	switch (type)
	{
		case MODTYPE_MOD:
			music = dumb_load_mod_quick(filename);
			break;

		case MODTYPE_S3M:
			music = dumb_load_s3m_quick(filename);
			break;

		case MODTYPE_XM:
			music = dumb_load_xm_quick(filename);
			break;

		case MODTYPE_IT:
			music = dumb_load_it_quick(filename);
			break;

		default:
			{
			char *lwr = strlwr(get_extension(filename));

			if (strcmp(lwr, "mod") == 0)
				music = dumb_load_mod_quick(filename);
			else if (strcmp(lwr, "s3m") == 0)
				music = dumb_load_s3m_quick(filename);
			else if (strcmp(lwr, "xm") == 0)
				music = dumb_load_xm_quick(filename);
			else if (strcmp(lwr, "it") == 0)
				music = dumb_load_it_quick(filename);
			else
				music = NULL;

			break;
			}

	}

	mod_player = al_start_duh(music, 2, 0, (mus_vol / 255.0), 4096, 44100);
}

void Poll_Music()
{
	if (al_poll_duh(mod_player) != 0)
	{
		if (mod_current_loop == 1)
		{
			al_stop_duh(music);
			mod_player = al_start_duh(music, 2, 0, (mus_vol / 255.0), 4096, 44100);
		}
		else
			al_stop_duh(music);
	}

	rest(0); // good opportunity to yield to other processes
}
