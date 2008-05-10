/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <aldumb.h>
#ifdef ENABLE_CDA
	#include <libcda.h>
#endif
#include <string.h>
#include "blocks3.h"

int cd_track = -1;

#define CDPLAYER_PLAYSTOP_LEN		5

static char cdplayer_playstop[CDPLAYER_PLAYSTOP_LEN];

static const Menu cdplayer_menu[] = {
	{(char *) &cdplayer_playstop, 1, 0},
	{"Next", 2, 0},
	{"Previous", 3, 0},
	{"Eject", 4, 0},
	{"Return", 5, MENUITEM_CLOSE},
	{END_OF_MENU}
};

void CD_Player(void)
{
#ifdef ENABLE_CDA
	int ret, retval = 0;
	char display_menu = 1;
	int flags = 0;
	int def_item = 0;
	int first, last;

	while (display_menu)
	{
		if (cd_track == -1)
			safe_strcpy((char *) &cdplayer_playstop, CDPLAYER_PLAYSTOP_LEN, "Play");
		else
			safe_strcpy((char *) &cdplayer_playstop, CDPLAYER_PLAYSTOP_LEN, "Stop");

		ret = Display_Menu((const Menu *) &cdplayer_menu, NULL, flags, def_item);

		switch (ret)
		{
			case 1:
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

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 0;

				break;

			case 2:
				cd_get_tracks(&first, &last);
				cd_track = cd_current_track();

				cd_track++;

				if (cd_track > last)
					cd_track = first;

				if (cd_play_from(cd_track) == -1)
					cd_track = -1;

				textprintf_ex(screen, font, 10, 10, makecol(255, 255, 255), makecol(0, 0, 0), "%d ", cd_track);

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 1;

				break;
			
			case 3:
				cd_get_tracks(&first, &last);
				cd_track = cd_current_track();

				cd_track--;

				if (cd_play_from(cd_track) == -1)
					cd_track = -1;

				text_mode(makecol(0, 0, 0));
				textprintf_ex(screen, font, 10, 10, makecol(255, 255, 255), makecol(0, 0, 0), "%d ", cd_track);

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 2;

				break;

			case 4:
				cd_eject();
				cd_track = -1;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 3;

				break;

			case 5:
				display_menu = 0;
				break;
		}
	}
#endif
}

static const Menu mod_menu[] = {
	{"Next", 1, 0},
	{"Previous", 2, 0},
	{"Return", 3, MENUITEM_CLOSE},
	{END_OF_MENU}
};

static const InfoWindow no_music_box[] = {
	{"MOD Music", FONT_HELV12B},
	{"", FONT_HELV12},
	{"There are no music files present in the", FONT_HELV12},
	{"\"music\" directory. Please add some, or", FONT_HELV12},
	{"reinstall the game.", FONT_HELV12},
	{END_OF_INFOWINDOW}
};

void Mod_Music(void)
{
	int ret, retval = 0;
	char display_menu = 1;
	int flags = 0;
	int def_item = 0;
	int mod_track_last = mod_track;

	if (mod_last == 0)
	{
		Display_Info_Window((const InfoWindow *) &no_music_box);
		return;
	}

	while (display_menu)
	{
		if (mod_track >= mod_last)
			mod_track = 0;

		if (mod_track < 0)
			mod_track = mod_last;

		if (mod_track_last != mod_track)
		{
			mod_track_last = mod_track;
			Play_MOD_Track(mod[mod_track].name, mod[mod_track].type, B2Music);
		}

		ret = Display_Menu((const Menu *) &mod_menu, NULL, flags, def_item);

		switch (ret)
		{
			case 1:
				mod_track++;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 0;

				break;

			case 2:
				mod_track--;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 1;

				break;

			case 3:
				display_menu = 0;
				break;
		}
	}
}

char mod_current_loop;

void Play_MOD_Track(const char *filename, int type, char loop)
{
	DUH_SIGRENDERER *sr;
	DUMB_IT_SIGRENDERER *itsr;
	int sel = -1;
	int i;

	al_stop_duh(mod_player);
	mod_player = NULL;
	mod_current_loop = loop;

	if (music_unload == 1)
		unload_duh(music);

	// find a piece of music matching this one
	for (i = 0; i < mod_last; i++)
	{
		if (strcasecmp(mod[i].name, filename) == 0)
		{
			sel = i;
			break;
		}
	}

	if (sel == -1)
		return;

	music_unload = 1;

	switch (mod[sel].type)
	{
		case MODTYPE_NULL:
			break;

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

		case MODTYPE_DAT_MOD:
		case MODTYPE_DAT_S3M:
		case MODTYPE_DAT_XM:
		case MODTYPE_DAT_IT:
			music = (DUH *) music_dat[mod[sel].id].dat;
			music_unload = 0;
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

	sr = al_duh_get_sigrenderer(mod_player);
	itsr = duh_get_it_sigrenderer(sr);

	dumb_it_set_loop_callback(itsr, dumb_it_callback_terminate, NULL);
	dumb_it_set_xm_speed_zero_callback(itsr, dumb_it_callback_terminate, NULL);
}

void Poll_Music()
{
	if (al_poll_duh(mod_player) != 0)
	{
		if (mod_current_loop == 1)
		{
			//al_stop_duh(mod_player);
			mod_player = al_start_duh(music, 2, 0, (mus_vol / 255.0), 4096, 44100);
		}
		else
		{
			mod_player = NULL;
			//al_stop_duh(mod_player);

			// play the next track, if not using Blocks+-style music
			if (B2Music == 0)
			{
				mod_track++;

				if (mod_track >= mod_last)
					mod_track = 0;

				Play_MOD_Track(mod[mod_track].name, mod[mod_track].type, 0);
			}
		}
	}

	rest(0); // good opportunity to yield to other processes - todo: implement properly (http://www.allegro.cc/forums/thread/592422)
}
