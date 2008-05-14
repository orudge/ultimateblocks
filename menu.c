/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>

#ifdef ENABLE_CDA
	#include <libcda.h>
#endif

#include <stdio.h>
#include "blocks3.h"

int Display_Menu(const Menu *menu, int *ret, int flags, int def_item)
{
	const Menu *mptr;
	int num_items = 0;
	int game_menu_exit;
	int menu_height = 0;
	int menu_font = FONT_HNEUE20;
	int font_height = 0;
	int actual_font_height = 0;
	int width = 0;
	int menu_width = 0;

	BITMAP *temp2;
	int item, i;

	mptr = menu;

	if (ret != NULL)
		*ret = NULL;

	if (flags & MENU_SMALL_FONT)
		menu_font = FONT_HELV12B;

	while (mptr++)
	{
		num_items++;

		if (mptr->title == NULL)
			break;

		width = MAX(width, text_length(fonts[menu_font].dat, mptr->title));
	}

	if (num_items == 0)
		return 0;

	actual_font_height = text_height(fonts[menu_font].dat);
	font_height = actual_font_height + 10;

	menu_height = (num_items*font_height) + 70; //35    50 * num_items;
	menu_width = (width + (10 - (width % 10)) + 100);

	#define MENU_WIDTH	menu_width //200
	#define MENU_HEIGHT menu_height
	#define MENU_X   ((SCREEN_W - MENU_WIDTH) / 2)
	#define MENU_Y	 ((SCREEN_H - MENU_HEIGHT) / 2)

	temp2 = create_bitmap(MENU_WIDTH, MENU_HEIGHT); // 200, 300

	if (def_item > num_items)
		def_item = num_items - 1;

	if (def_item < 0)
		def_item = 0;

	game_menu_exit = 0;
	item = def_item;

	rect(temp2, 0, 0, MENU_WIDTH - 1, MENU_HEIGHT - 1, makecol(0, 0, 0));

	if (!(flags & MENU_NO_REDRAW))
	{
		// save the screen
		blit(screen, temp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		rectfill(temp2, 1, 1, MENU_WIDTH - 2, MENU_HEIGHT - 2, makecol(255, 255, 255));
		rectfill(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(128, 128, 128));
		rectfill(temp2, 10, MENU_HEIGHT - 21, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(128, 128, 128));
		rect(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(0, 0, 0));
		rect(temp2, 10, MENU_HEIGHT - 11, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(0, 0, 0));

		rectfill(temp2, 10, item*font_height + (font_height), MENU_WIDTH - 10, item*font_height + ((font_height*2)), makecol(180, 180, 255));
		text_mode(-1);

		for (i = 0; i < num_items; i++)
		{
			textprintf_centre(temp2, fonts[menu_font].dat, MENU_WIDTH / 2, font_height + (font_height * i) + ((font_height - actual_font_height) / 2), makecol(0, 0, 0), menu[i].title);
		}

		for (i = -MENU_WIDTH; i < MENU_X+10; i+=10)
		{
			blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);
			blit(temp2, screen, 0, 0, i, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
		
			while (time_count < 1)
			{
				Poll_Music();
			}

			time_count = 0;
		}

		if (i != MENU_X)
			blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);

	}

	while (key[KEY_ESC])
	{
		Poll_Music();
	}

	while (!game_menu_exit)
	{
		rectfill(temp2, 1, 1, MENU_WIDTH - 2, MENU_HEIGHT - 2, makecol(255, 255, 255));
		rectfill(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(128, 128, 128));
		rectfill(temp2, 10, MENU_HEIGHT - 21, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(128, 128, 128));
		rect(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(0, 0, 0));
		rect(temp2, 10, MENU_HEIGHT - 11, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(0, 0, 0));

		rectfill(temp2, 10, item*font_height + (font_height), MENU_WIDTH - 10, item*font_height + ((font_height*2)), makecol(180, 180, 255));
		text_mode(-1);

		for (i = 0; i < num_items; i++)
		{
			if ((menu[i].options & MENUITEM_TITLE) || (menu[i].options & MENUITEM_DRAWLINES))
			{
				line(temp2, 10, font_height + (font_height * i), MENU_WIDTH - 11, font_height + (font_height * i), makecol(0, 0, 0));
				line(temp2, 10, (2*font_height) + (font_height * i), MENU_WIDTH - 11, (2*font_height) + (font_height * i), makecol(0, 0, 0));
			}

			textprintf_centre(temp2, fonts[menu_font].dat, MENU_WIDTH / 2, font_height + (font_height * i) + ((font_height - actual_font_height) / 2), makecol(0, 0, 0), menu[i].title);
		}

		blit(temp2, screen, 0, 0, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

		while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]) && (!key[KEY_LEFT]) && (!key[KEY_RIGHT]))
		{
			Poll_Music();
		}

		if (key[KEY_ESC])
		{
			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
			item = -1;
			game_menu_exit = 1;
		}

		if (key[KEY_ENTER])
		{
			game_menu_exit = 1;
			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
		}

		if ((key[KEY_LEFT]) && (menu[item].options & MENUITEM_KEY_LEFT))
		{
			if (ret != NULL)
				*ret = MENU_KEY_LEFT;

			game_menu_exit = 1;
		}

		if ((key[KEY_RIGHT]) && (menu[item].options & MENUITEM_KEY_RIGHT))
		{
			if (ret != NULL)
				*ret = MENU_KEY_RIGHT;

			game_menu_exit = 1;
		}

redo_key: ;

		if (key[KEY_UP])
			item--;

		if (key[KEY_DOWN])
			item++;

		play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

		if (item < 0)
			item = num_items - 1;

		if (item > (num_items - 1))
			item = 0;

		if (menu[item].options & MENUITEM_TITLE)
			goto redo_key; // I'm sorry.

		if ((flags & MENU_ALLOW_MANY_LR) && ((menu[item].options & MENUITEM_KEY_LEFT) || (menu[item].options & MENUITEM_KEY_RIGHT)))
		{
			while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]))
			{
				Poll_Music();
			}
		}
		else
		{
			while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER])|| (key[KEY_LEFT]) || (key[KEY_RIGHT]))
			{
				Poll_Music();
			}
		}
	}

	if (menu[item].options & MENUITEM_CLOSE)
	{
		for (i = MENU_X; i < SCREEN_W + 10; i+=10)
		{
			blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);
			blit(temp2, screen, 0, 0, i, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

			while (time_count < 1)
			{
				Poll_Music();
			}
			time_count = 0;
		}

		blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	}

	destroy_bitmap(temp2);

	if (item == -1)
		return(NULL);
	else
		return(menu[item].id);

	#undef MENU_WIDTH
	#undef MENU_HEIGHT
	#undef MENU_X
	#undef MENU_Y
}

void Display_Info_Window(const InfoWindow *menu)
{
	const InfoWindow *mptr;
	int num_items = 0;
	int menu_height = 0;
	int menu_width = 0;
	int line_height = 0;
	int game_menu_exit;

	BITMAP *temp2;
	int i;

	int width = 0;

	mptr = menu;

	while (mptr++)
	{
		num_items++;

		if (mptr->title == NULL)
			break;

		width = MAX(width, text_length(fonts[mptr->font].dat, mptr->title));
		line_height = MAX(line_height, text_height(fonts[mptr->font].dat));
	}

	if (num_items == 0)
		return;

	menu_height = (num_items*(line_height+10)) + 70; //35    50 * num_items;
	menu_width = (width + (10 - (width % 10)) + 100);

	#define MENU_WIDTH	menu_width //200
	#define MENU_HEIGHT menu_height
	#define MENU_X   ((SCREEN_W - MENU_WIDTH) / 2)
	#define MENU_Y	 ((SCREEN_H - MENU_HEIGHT) / 2)

	temp2 = create_bitmap(MENU_WIDTH, MENU_HEIGHT); // 200, 300

	game_menu_exit = 0;

	rect(temp2, 0, 0, MENU_WIDTH - 1, MENU_HEIGHT - 1, makecol(0, 0, 0));

	// save the screen
	blit(screen, temp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	rectfill(temp2, 1, 1, MENU_WIDTH - 2, MENU_HEIGHT - 2, makecol(255, 255, 255));
	rectfill(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(128, 128, 128));
	rectfill(temp2, 10, MENU_HEIGHT - 21, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(128, 128, 128));
	rect(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(0, 0, 0));
	rect(temp2, 10, MENU_HEIGHT - 11, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(0, 0, 0));

//	rectfill(temp2, 10, item*40 + 35, MENU_WIDTH - 10, item*40 + 65, makecol(180, 180, 255));
	text_mode(-1);

	for (i = 0; i < num_items; i++)
	{
		textprintf_centre(temp2, fonts[menu[i].font].dat, MENU_WIDTH / 2, 35 + ((line_height+10) * i), makecol(0, 0, 0), menu[i].title);
	}

	for (i = -MENU_WIDTH; i < MENU_X; i+=10)
	{
		blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);
		blit(temp2, screen, 0, 0, i, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	
		while (time_count < 1)
		{
			Poll_Music();
		}

		time_count = 0;
	}

	if (i != MENU_X)
		blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);

	while (key[KEY_ESC])
	{
		Poll_Music();
	}

	while (!game_menu_exit)
	{
		rectfill(temp2, 1, 1, MENU_WIDTH - 2, MENU_HEIGHT - 2, makecol(255, 255, 255));
		rectfill(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(128, 128, 128));
		rectfill(temp2, 10, MENU_HEIGHT - 21, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(128, 128, 128));
		rect(temp2, 10, 10, MENU_WIDTH - 11, 20, makecol(0, 0, 0));
		rect(temp2, 10, MENU_HEIGHT - 11, MENU_WIDTH - 11, MENU_HEIGHT - 11, makecol(0, 0, 0));

//		rectfill(temp2, 10, item*40 + 35, MENU_WIDTH - 10, item*40 + 65, makecol(180, 180, 255));
		text_mode(-1);

		for (i = 0; i < num_items; i++)
		{
			textprintf_centre(temp2, fonts[menu[i].font].dat, MENU_WIDTH / 2, 35 + ((line_height+10) * i),  makecol(0, 0, 0), menu[i].title);
		}

		blit(temp2, screen, 0, 0, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

		while ((!key[KEY_ESC]) && /*(!key[KEY_UP]) && (!key[KEY_DOWN]) &&*/ (!key[KEY_ENTER]))
		{
			Poll_Music();
		}

		if (key[KEY_ESC])
		{
			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
//			item = -1;
			game_menu_exit = 1;
		}

		if (key[KEY_ENTER])
		{
			game_menu_exit = 1;
			play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
		}

//		if (key[KEY_UP])
			//item--;

		//if (key[KEY_DOWN])
			//item++;

		play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

		while ((key[KEY_ESC]) || /*(key[KEY_UP]) || (key[KEY_DOWN]) ||*/ (key[KEY_ENTER]))
		{
			Poll_Music();
		}
	}

	for (i = MENU_X; i < SCREEN_W + 10; i+=10)
	{
		blit(temp, screen, i - 10, MENU_Y, i-10, MENU_Y, 10, MENU_HEIGHT);
		blit(temp2, screen, 0, 0, i, MENU_Y, MENU_WIDTH, MENU_HEIGHT);

		while (time_count < 1)
		{
			Poll_Music();
		}
		time_count = 0;
	}

	blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	destroy_bitmap(temp2);
}

static const Menu in_game_menu[] = {
	{"Return", 1, MENUITEM_CLOSE},
	{"Restart", 2, MENUITEM_CLOSE},
	{"Level", 3, MENUITEM_CLOSE},
	{"Options", 4, MENUITEM_CLOSE},
	{"Music", 5, MENUITEM_CLOSE},
	{"Exit", 6, MENUITEM_CLOSE},
	{END_OF_MENU}
};

void In_Game_Menu(void)
{
	int ret;

	ret = Display_Menu((const Menu *) &in_game_menu, NULL, 0, 0);

	switch (ret)
	{
		case 2:
			Maps_to_Map(lev);
			Draw_Map();
			Init_Level();
			Map_Setup();
			blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

			break;

		case 3:
			Change_Levels();

			if (game_exit == 2)
			{
				Maps_to_Map(lev);
				Draw_Map();
				Init_Level();
				Map_Setup();
				Get_Ready();

				game_exit = 0;
			}
			else
			{
				// redraw screen

				Draw_Screen();
				Draw_Map();

				Player_Draw(0);

				if (no_ply == 2)
					Player_Draw(1);

				blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
			}

			break;

		case 4:
			Options_Menu();
			break;

		case 5:
			Music_Menu();
			break;

		case 6:
			game_exit = 2;
			break;
	}

	_redraw_status = 1;
}

#define MAX_VOL_LABEL_LEN		30

#define OPTIONS_DELAY(x) {time_count = 0; while (time_count < x) { Poll_Music();	} }

void Options_Menu(void)
{
	int ret, retval = 0;
	char display_menu = 1;
	int flags = MENU_ALLOW_MANY_LR;
	int def_item = 0;

	char music_vol_str[MAX_VOL_LABEL_LEN];
	char cd_vol_str[MAX_VOL_LABEL_LEN];
	char sfx_vol_str[MAX_VOL_LABEL_LEN];
	char game_speed_str[MAX_VOL_LABEL_LEN];

	const Menu options_menu[] = {
		{"Keyboard", 1, MENUITEM_CLOSE},
		{(char *) &game_speed_str, 2, MENUITEM_KEY_LEFT | MENUITEM_KEY_RIGHT},
		{(char *) &music_vol_str, 3, MENUITEM_KEY_LEFT | MENUITEM_KEY_RIGHT},
		{(char *) &cd_vol_str, 4, MENUITEM_KEY_LEFT | MENUITEM_KEY_RIGHT},
		{(char *) &sfx_vol_str, 5, MENUITEM_KEY_LEFT | MENUITEM_KEY_RIGHT},
		{"Return", 6, MENUITEM_CLOSE},
		{END_OF_MENU}
	};

	int old_mus_vol, old_cd_vol;

	old_mus_vol = mus_vol;
	old_cd_vol = cd_vol;

	while (display_menu)
	{
		if (mus_vol < 0) mus_vol = 0;
		if (sfx_vol < 0) sfx_vol = 0;
		if (cd_vol < 0) cd_vol = 0;

		if (mus_vol > 255) mus_vol = 255;
		if (sfx_vol > 255) sfx_vol = 255;
		if (cd_vol > 255) cd_vol = 255;
	
		if (mus_vol != old_mus_vol)
		{
			al_duh_set_volume(mod_player, (mus_vol / 255.0));
			old_mus_vol = mus_vol;
		}

		if (cd_vol != old_cd_vol)
		{
#ifdef ENABLE_CDA
			cd_set_volume(cd_vol, cd_vol);
#endif
			old_cd_vol = cd_vol;
		}

		sprintf((char *) &game_speed_str, "Game Speed: %s", GAME_SPEEDS[_game_speed]);
		sprintf((char *) &music_vol_str, "Music: %d", mus_vol*100/255);
		sprintf((char *) &cd_vol_str, "CD: %d", cd_vol*100/255);
		sprintf((char *) &sfx_vol_str, "SFX: %d", sfx_vol*100/255);

		ret = Display_Menu((const Menu *) &options_menu, &retval, flags, def_item);

		switch (ret)
		{
			case 1:
				Keyboard_Menu();
				break;

			case 2:
				if (retval == MENU_KEY_LEFT)
					_game_speed--;
				else if (retval == MENU_KEY_RIGHT)
					_game_speed++;

				if (_game_speed < 0)
					_game_speed = 0;
				else if (_game_speed > 2)
					_game_speed = 2;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 1;

				UpdateGameSpeeds();

				OPTIONS_DELAY(50);
				break;

			case 3:
				if (retval == MENU_KEY_LEFT)
					mus_vol -= 3;
				else if (retval == MENU_KEY_RIGHT)
					mus_vol += 3;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 2;

				OPTIONS_DELAY(10);

				break;

			case 4:
				if (retval == MENU_KEY_LEFT)
					cd_vol -= 3;
				else if (retval == MENU_KEY_RIGHT)
					cd_vol += 3;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 3;

				OPTIONS_DELAY(10);
				break;
			
			case 5:
				if (retval == MENU_KEY_LEFT)
					sfx_vol -= 3;
				else if (retval == MENU_KEY_RIGHT)
					sfx_vol += 3;

				display_menu = 1;
				flags |= MENU_NO_REDRAW;
				def_item = 4;

				OPTIONS_DELAY(10);
				break;

			case 6:
				display_menu = 0;
				break;
		}
	}
}

void Keyboard_Menu(void)
{
	int ret, retval = 0;
	char display_menu = 1;
	int flags = MENU_SMALL_FONT;
	int def_item = 1;

	char p1_up_str[MAX_VOL_LABEL_LEN];
	char p1_down_str[MAX_VOL_LABEL_LEN];
	char p1_left_str[MAX_VOL_LABEL_LEN];
	char p1_right_str[MAX_VOL_LABEL_LEN];
	char p2_up_str[MAX_VOL_LABEL_LEN];
	char p2_down_str[MAX_VOL_LABEL_LEN];
	char p2_left_str[MAX_VOL_LABEL_LEN];
	char p2_right_str[MAX_VOL_LABEL_LEN];

	const Menu options_menu[] = {
		{"Player 1", 1, MENUITEM_TITLE},
		{(char *) &p1_up_str, 2, MENUITEM_CLOSE},
		{(char *) &p1_down_str, 3, MENUITEM_CLOSE},
		{(char *) &p1_left_str, 4, MENUITEM_CLOSE},
		{(char *) &p1_right_str, 5, MENUITEM_CLOSE},
		{"Player 2", 6, MENUITEM_TITLE},
		{(char *) &p2_up_str, 7, MENUITEM_CLOSE},
		{(char *) &p2_down_str, 8, MENUITEM_CLOSE},
		{(char *) &p2_left_str, 9, MENUITEM_CLOSE},
		{(char *) &p2_right_str, 10, MENUITEM_CLOSE},
		{"Return", 11, MENUITEM_CLOSE | MENUITEM_DRAWLINES},
		{END_OF_MENU}
	};

	while (display_menu)
	{
		sprintf((char *) &p1_up_str, "Up: UP");
		sprintf((char *) &p1_down_str, "Down: DOWN");
		sprintf((char *) &p1_left_str, "Left: LEFT");
		sprintf((char *) &p1_right_str, "Right: RIGHT");

		sprintf((char *) &p2_up_str, "Up: W");
		sprintf((char *) &p2_down_str, "Down: S");
		sprintf((char *) &p2_left_str, "Left: A");
		sprintf((char *) &p2_right_str, "Right: D");

		ret = Display_Menu((const Menu *) &options_menu, &retval, flags, def_item);

		switch (ret)
		{
			case 2:
			case 3:
			case 4:
			case 5:
				break;

			case 7:
			case 8:
			case 9:
			case 10:
				break;

			case 11:
				display_menu = 0;
				break;
		}
	}
}

static const Menu music_menu[] = {
	{(char *) "CD Player", 1, MENUITEM_CLOSE},
	{(char *) "MOD Music", 2, MENUITEM_CLOSE},
	{(char *) "Return", 3, MENUITEM_CLOSE},
	{END_OF_MENU}
};

void Music_Menu(void)
{
	switch (Display_Menu((const Menu *) &music_menu, NULL, 0, 0))
	{
		case 1:
			CD_Player();
			break;

		case 2:
			Mod_Music();
			break;
	}
}

static const InfoWindow about_box[] = {
	{"About Ultimate Blocks", FONT_HELV12B},
	{"Version 4.0", FONT_HELV12},
	{"", FONT_HELV12},
	{"Copyright \xC2\xA9 Owen Rudge 2001, 2008,", FONT_HELV12},
	{"An Ly 1999-2001", FONT_HELV12},
	{"", FONT_HELV12},
	{"www.ultimateblocks.com", FONT_HELV12},
	{END_OF_INFOWINDOW}
};

void About_Box(void)
{
	Display_Info_Window((const InfoWindow *) &about_box);
}
