/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>

#if defined(WIN32) || defined(WIN64)
	#include <winalleg.h>
#endif

#ifdef ENABLE_CDA
	#include <libcda.h>
#endif

#include "blocks3.h"
#include <string.h>

#include "fps.h"

int i, j, full_exit;

int main(void)
{
	Initialise();

/* play_mod (music, TRUE);*/

	Presents();

	Title();
	Log_In();

	full_exit = 1;

	while (full_exit)
	{
		full_exit = Title_Menu();

		if (full_exit == 1)
		{
			Transition_Out(title_gfx, title_palette);
			//fade_out(10);
			Editor();
			Title();
		}

		if ((full_exit == 2) && (Open_Maps_Title() == 1))
		{
			Transition_Out(title_gfx, title_palette);
			Full_Game();
			Title();
		}
	}

	Transition_Out(title_gfx, title_palette);

#ifdef ENABLE_CDA
	cd_stop();
	cd_exit();
#endif

#ifdef MEASURE_FPS
	destroy_fps(fps);
#endif

	UnloadGraphics();

	Save_Config();

	dumb_exit();
	allegro_exit();

	return(0);
} END_OF_MAIN()

/* Function: Init_Level
 *
 * Initialises player data for current level
 */
void Init_Level(void)
{
	int i, j, k;

	no_ply = 1;

	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 15; j++)
		{
			for (k = 0; k < 2; k++)
			{
				if (map[i][j][k] == AUDREY)
				{
					map[i][j][k] = 0;
					ply[0].x = i*_block_width;
					ply[0].y = j*_block_height;
					ply[0].z = k*_block_depth;
					ply[0].dx = 0;
					ply[0].dy = 0;
					ply[0].dz = 0;
				}
				else if (map[i][j][k] == AN)
				{
					map[i][j][k] = 0;
					ply[1].x = i*_block_width;
					ply[1].y = j*_block_height;
					ply[1].z = k*_block_depth;
					ply[1].dx = 0;
					ply[1].dy = 0;
					ply[1].dz = 0;
					no_ply = 2;
				}

				map_laser[i][j][k] = 0;
			}
		}
	}
}

/* Function: Run_Level
 *
 * Main routine for playing a level
 */
int Run_Level(void)
{
	Change_Motif(map_motif[lev]);

	Clear_Undo();
	Draw_Map();
	Init_Level();
	Map_Setup();

/*	set_palette(black_palette);
	blit(temp, screen, 0, 0, 0, 0, 640, 480);
	fade_in(palette, 10);*/

	set_palette(palette);

	Transition_In(temp, palette);

	Get_Ready();

	game_exit = 0;
	_level_undos = 0;
	fps_count = 0;

	while (!game_exit)
	{
		while (fps_count)
		{
			--fps_count;

			Poll_Music();

			Box_Move();
			Box_Fall();

			Bomb_Prime_Laser();

			Player_Fall(0);
			Player_Fall(1);

			if (((key[KEY_LCONTROL]) || key[KEY_RCONTROL]) && (no_ply == 2))
				Player_Input(1);
			else
				Player_Input(0);

			if (no_ply == 2)
				Player2_Input(1);

			Player_Move(0);

			if (no_ply == 2)
				Player_Move(1);

			Player_Pickup_Check(0);

			if (no_ply == 2)
				Player_Pickup_Check(1);
  
			Player_Draw(0);

			if (no_ply == 2)
				Player_Draw(1);

			Check_Fall(0);

			if (no_ply == 2)
				Check_Fall(1);

			Check_Fall_Prime();

			Monster();

			Laser();
			Door();

			Player_Monster_Collision(0);
			Player_Laser_Collision(0);

			if (no_ply == 2)
			{
				Player_Monster_Collision(1);
				Player_Laser_Collision(1);
			}

			rest(1);
#ifdef MEASURE_FPS
			fps_tick(fps);
#endif
		}

		Draw_Explode();

		Draw_Screen();

#ifdef MEASURE_FPS
		text_mode(-1);
		rectfill(screen, 0, 0, 200, 10, 0);
		draw_fps(fps, screen, font, 0, 0, makecol(255,255,255), "Current FPS is: %d");
		fps_frame(fps);
#endif

		Play_Sound();

/*		textprintf(screen, font, 10, 10, 7, "%d ", undo_count);

		text_mode(makecol(0, 0, 0));

		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 15; j++)
			{
				textprintf(screen, font, i*32, j*32, 7, "%d ", (map[i][j][0]));
			}
		}
*/

		if (Token_Count())
			game_exit = 3;

		while (time_count < 1)
		{
			Poll_Music();
		}
		
		time_count = 0;

		if ((key[KEY_ESC]) && (game_type == EDITOR_GAME))
			game_exit = 2;

		if ((key[KEY_ESC]) && (game_type == PLAYER_GAME))
			In_Game_Menu();

		// TODO: better implementation - should display a proper message
		if (close_button_pressed)
		{
			close_button_pressed = FALSE;
			In_Game_Menu();

			fps_count = 0;
		}

		if (key[KEY_F2])
		{
			while (key[KEY_F2]);

			Maps_to_Map(lev);
			Draw_Map();
			Init_Level();
			Map_Setup();
			blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

			fps_count = 0;
		}

		if (key[KEY_BACKSPACE])
		{
			while (key[KEY_BACKSPACE])
			{
				Poll_Music();
			}

			Undo();
		}
	}

	if (game_exit == 1)
	{
		Player_Death(0);
		blit(screen, temp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		Transition_Out(temp, palette);
		return -1;
	}
	
	if (game_exit == 3)
	{
		Well_Done();
		blit(screen, temp, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
		Transition_Out(temp, palette);
		return 1;
	}

	return 0;
}

void Full_Game(void)
{
	int exit_full_game;

	lev = 0;
	exit_full_game = 1;
	game_type = PLAYER_GAME;

	Init_Full_Game();

	while (exit_full_game)
	{
		Poll_Music();

		Maps_to_Map(lev);
		exit_full_game = Run_Level();

		if (exit_full_game == 1)
		{
			map_done[lev] = 1;
			lev = Next_Level();

			if (lev == 999)
			{
				for (lev = 0; lev < 100; lev++)
					map_done[lev] = 0;

				exit_full_game = 0;
			}
		}
	}

	Save_Map_Done();
}

int Token_Count(void)
{
	int i, j, k;

	for (k = 0; k < 2; k++)
	for (j = 0; j < 15; j++)
	for (i = 0; i < 20; i++)

	if (map[i][j][k] == TOKEN)
		return 0;

	return 1;
}

int Num_Tokens(void)
{
	int i, j, k;
	int count = 0;

	for (k = 0; k < 2; k++)
	{
		for (j = 0; j < 15; j++)
		{
			for (i = 0; i < 20; i++)
			{
				if (map[i][j][k] == TOKEN)
					count++;
			}
		}
	}

	return count;
}

void Get_Ready(void)
{
	BITMAP *temp2 = create_bitmap(SCREEN_W, SCREEN_H);

	blit(temp, temp2, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	text_mode(-1);

	textprintf_centre(temp2, fonts[1].dat, 320, 149, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 321, 149, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 321, 150, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 321, 151, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 320, 151, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 319, 151, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 319, 150, 0, "GET.READY!");
	textprintf_centre(temp2, fonts[1].dat, 319, 149, 0, "GET.READY!");

	textprintf_centre(temp2, fonts[1].dat, 320, 150, 7, "GET.READY!");

	blit(temp2, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	play_sample(sfx[SFX_GETRED].dat, sfx_vol, 128, 1000, 0);

	while(key[KEY_ENTER])
	{
		Poll_Music();
	}

	while(!key[KEY_ENTER])
	{
		Poll_Music();
	}

	blit(temp, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	while(key[KEY_ENTER])
	{
		Poll_Music();
	}
}

void Well_Done(void)
{
	BITMAP *temp2 = create_bitmap(SCREEN_W, SCREEN_H);

	blit(screen, temp2, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	text_mode(-1);

	textprintf_centre(temp2, fonts[1].dat, 320, 149, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 321, 149, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 321, 150, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 321, 151, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 320, 151, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 319, 151, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 319, 150, 0, "WELL.DONE!");
	textprintf_centre(temp2, fonts[1].dat, 319, 149, 0, "WELL.DONE!");
	
	textprintf_centre(temp2, fonts[1].dat, 320, 150, 7, "WELL.DONE!");

	blit(temp2, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	play_sample(sfx[SFX_WELLDONE].dat, sfx_vol, 128, 1000, 0);

	while(key[KEY_ENTER])
	{
		Poll_Music();
	}
	while(!key[KEY_ENTER])
	{
		Poll_Music();
	}
	while(key[KEY_ENTER])
	{
		Poll_Music();
	}
}

void Init_Full_Game(void)
{
	int i;
	char fn[MAX_PATH];
	PACKFILE *file;

	clear(screen);
	set_palette(palette);

	for (i = 0; i < 100; i++)
	map_done[i] = 0;

	safe_strcpy(fn, MAX_PATH, "./");
	safe_strcat(fn, MAX_PATH, login);
	safe_strcat(fn, MAX_PATH, "/");
	safe_strcat(fn, MAX_PATH, map_save);

	if (!exists(fn))
		return;

	file = pack_fopen(fn, "rp");

	for (i = 0; i < 100; i++)
		map_done[i] = pack_igetw(file);

	pack_fclose(file);

	lev = 0;
	lev = Next_Level();
}


void Save_Map_Done(void)
{
	int i;
	char fn[MAX_PATH];
	PACKFILE *file;

	safe_strcpy(fn, MAX_PATH, "./");
	safe_strcat(fn, MAX_PATH, login);
	safe_strcat(fn, MAX_PATH, "/");
	safe_strcat(fn, MAX_PATH, map_save);

	file = pack_fopen(fn, "wp");

	for (i = 0; i < 100; i++)
		pack_iputw(map_done[i], file);

	pack_fclose(file);
}

int Next_Level(void)
{
	int l, i, j, k, tk;

	for (l = lev; l < 100; l++)
	{
		if (map_done[l] == 0)
		{
			tk = 0;

			for (i = 0; i < 20; i++)
			{
				for (j = 0; j < 15; j++)
				{
					for (k = 0; k < 2; k++)
					{			
						if (maps[i][j][k][l] == TOKEN)
							tk++;
					}
				}
			}

			if (tk != 0)
				return l;
		}
	}

	for (l = 0; l < 100; l++)
	{
		if (map_done[l] == 0)
		{
			tk = 0;

			for (i = 0; i < 20; i++)
			{
				for (j = 0; j < 15; j++)
				{
					for (k = 0; k < 2; k++)
					{
						if (maps[i][j][k][l] == TOKEN)
							tk++;
					}
				}
			}

			if (tk != 0)
				return l;
		}
	}

	Finish_Game();
	return 999;
}

void Finish_Game(void)
{
	clear(screen);

	set_palette(black_palette);

	textprintf_centre(screen, fonts[0].dat, 320, 20, 7, "CONGRAGULATIONS!");
	textprintf_centre(screen, fonts[0].dat, 320, 100, 7, "You have completed the");
	textprintf_centre(screen, fonts[0].dat, 320, 160, makecol(255, 255, 0), "%s", map_save);
	textprintf_centre(screen, fonts[0].dat, 320, 220, 7, "level set of Ultimate Blocks.");
	textprintf_centre(screen, fonts[0].dat, 320, 280, 7, "Thank you for playing!");

	fade_in(palette, 10);

	while(key[KEY_ENTER])
	{
		Poll_Music();
	}

	while(!key[KEY_ENTER])
	{
		Poll_Music();
	}
}

#if 0
void Convert(void)
{
	int i, j, k, l;
	PACKFILE *file;

	file = pack_fopen("./maps/blocks2.map", "rp");

	for (k = 0; k < 100; k++)
	{
		for (j = 0; j < 15; j++)
		{
			for (i = 0; i < 20; i++)
			{
				maps[i][j][1][k] = pack_getc(file);
			}
		}
	}

	pack_fclose(file);

	for (k = 0; k < 100; k++)
	{
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 15; j++)
			{
				if (maps[i][j][1][k] == OLD_SOLID) maps[i][j][0][k] = SOLID;
				if (maps[i][j][1][k] == OLD_BOX)   maps[i][j][0][k] = BOX;
				if (maps[i][j][1][k] == OLD_TOKEN) maps[i][j][0][k] = TOKEN;
  				if (maps[i][j][1][k] == OLD_FALL)  maps[i][j][0][k] = FALL;
  				if (maps[i][j][1][k] == OLD_AUDREY) maps[i][j][0][k] = AUDREY;
  				if (maps[i][j][1][k] == OLD_AN)     maps[i][j][0][k] = AN;
  				if (maps[i][j][1][k] == OLD_LASER_UP)    maps[i][j][0][k] = LASER_UP;
  				if (maps[i][j][1][k] == OLD_LASER_DOWN)  maps[i][j][0][k] = LASER_DOWN;
  				if (maps[i][j][1][k] == OLD_LASER_LEFT)  maps[i][j][0][k] = LASER_LEFT;
  				if (maps[i][j][1][k] == OLD_LASER_RIGHT) maps[i][j][0][k] = LASER_RIGHT;
  				if (maps[i][j][1][k] == OLD_LASER_4WAY)  maps[i][j][0][k] = LASER_4WAY;
  				if (maps[i][j][1][k] == OLD_MIRROR_PLUS) maps[i][j][0][k] = MIRROR_PLUS;
  				if (maps[i][j][1][k] == OLD_MIRROR_MINUS) maps[i][j][0][k] = MIRROR_MINUS;
  				if (maps[i][j][1][k] == OLD_BOMB)         maps[i][j][0][k] = BOMB1;
  				if (maps[i][j][1][k] == OLD_BOMB2)        maps[i][j][0][k] = BOMB2;
  				if (maps[i][j][1][k] == OLD_DOOR1)        maps[i][j][0][k] = RED_DOOR_CLOSE;
  				if (maps[i][j][1][k] == OLD_DOOR2)        maps[i][j][0][k] = GREEN_DOOR_CLOSE;
  				if (maps[i][j][1][k] == OLD_DOOR3)        maps[i][j][0][k] = BLUE_DOOR_CLOSE;
  				if (maps[i][j][1][k] == OLD_SWITCH1)      maps[i][j][0][k] = RED_SWITCH;
  				if (maps[i][j][1][k] == OLD_SWITCH2)      maps[i][j][0][k] = GREEN_SWITCH;
  				if (maps[i][j][1][k] == OLD_SWITCH3)      maps[i][j][0][k] = BLUE_SWITCH;
  				if (maps[i][j][1][k] == OLD_MON1)         maps[i][j][0][k] = MON0;
  				if (maps[i][j][1][k] == OLD_MON2)         maps[i][j][0][k] = MON2;
 			}
		}
	}

	for (k = 0; k < 100; k++)
	{
		for (i = 1; i < 19; i++)
		{
			for (j = 1; j < 14; j++)
			{
				maps[i][j][1][k] = 0;
			}
		}
	}

	for (j = 0; j < 100; j++)
	{
		for (i = 0; i < 20; i++)
		{
			maps[i][0][0][j] = SOLID;
			maps[i][14][0][j] = SOLID;
			maps[i][0][1][j] = RESERVE;
			maps[i][14][1][j] = RESERVE;
		}

		for (i = 0; i < 15; i++)
		{
			maps[0][i][0][j] = SOLID;
			maps[19][i][0][j] = SOLID;
			maps[0][i][1][j] = RESERVE;
			maps[19][i][1][j] = RESERVE;
		}
	}

	file = pack_fopen("./maps/b2.map", "wp");

	for (l = 0; l < 100; l++)
	{
		for (k = 0; k < 2; k++)
		{
			for (j = 0; j < 15; j++)
			{
				for (i = 0; i < 20; i++)
				{
					pack_iputw(maps[i][j][k][l], file);
				}
			}
		}
	}

	pack_fclose(file);
}
#endif

char *safe_strcpy(char *dest, const size_t dest_len, const char *src)
{
	if ((!dest) || (!src))
		return NULL;

	if (strlen(src) > dest_len)
		strncpy(dest, src, dest_len);
	else
		strcpy(dest, src);

	return(dest);
}

char *safe_strncpy(char *dest, const size_t dest_len, const char *src, const size_t source_len)
{
	if ((!dest) || (!src))
		return NULL;

	if (source_len > dest_len)
		strncpy(dest, src, dest_len);
	else
		strncpy(dest, src, source_len);

	return(dest);
}

char *safe_strcat(char *dest, const size_t dest_len, const char *src)
{
	size_t real_dest_len = 0;

	if ((!dest) || (!src))
		return NULL;

	real_dest_len = strlen(dest);

	if (strlen(src) + real_dest_len > dest_len)
		strncat(dest, src, dest_len - real_dest_len);
	else
		strcat(dest, src);

	return(dest);
}

void report_error(char *str)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	
#if defined(WIN32) || defined(WIN64)
	MessageBox(NULL, str, "Ultimate Blocks", MB_ICONERROR);
#else
	allegro_message(str);
#endif

	exit(1);
}
