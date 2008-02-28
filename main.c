/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include <libcda.h>
#include "blocks3.h"
#include <string.h>

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

	cd_stop();
	cd_exit();

	UnloadGraphics();
	allegro_exit();
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
					ply[0].x = i*32;
					ply[0].y = j*32;
					ply[0].z = k*8;
					ply[0].dx = 0;
					ply[0].dy = 0;
					ply[0].dz = 0;
				}
				else if (map[i][j][k] == AN)
				{
					map[i][j][k] = 0;
					ply[1].x = i*32;
					ply[1].y = j*32;
					ply[1].z = k*8;
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

	while (!game_exit)
	{
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

		Draw_Explode();

		Draw_Screen();
		Play_Sound();

/*		textprintf(screen, font, 10, 10, 7, "%d ", undo_count);

		text_mode(0);

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

		while (time_count < 1);
		
		time_count = 0;

		if ((key[KEY_ESC]) && (game_type == EDITOR_GAME))
			game_exit = 2;

		if ((key[KEY_ESC]) && (game_type == PLAYER_GAME))
			In_Game_Menu();

		if (key[KEY_F2])
		{
			while (key[KEY_F2]);

			Maps_to_Map(lev);
			Draw_Map();
			Init_Level();
			Map_Setup();
			blit(temp, screen, 0, 0, 0, 0, 640, 480);
		}

		if (key[KEY_BACKSPACE])
		{
			while (key[KEY_BACKSPACE]);
			Undo();
		}
	}

	if (game_exit == 1)
	{
		Player_Death(0);
		blit(screen, temp, 0, 0, 0, 0, 640, 480);
		Transition_Out(temp, palette);
		return -1;
	}
	
	if (game_exit == 3)
	{
		Well_Done();
		blit(screen, temp, 0, 0, 0, 0, 640, 480);
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

void Get_Ready(void)
{
	BITMAP *temp2 = create_bitmap(640, 480);

	blit(temp, temp2, 0, 0, 0, 0, 640, 480);

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

	blit(temp2, screen, 0, 0, 0, 0, 640, 480);

	play_sample(sfx[SFX_GETRED].dat, 255, 128, 1000, 0);

	while(key[KEY_ENTER]);
	while(!key[KEY_ENTER]);
	blit(temp, screen, 0, 0, 0, 0, 640, 480);
	while(key[KEY_ENTER]);
}

void Well_Done(void)
{
	BITMAP *temp2 = create_bitmap(640, 480);

	blit(screen, temp2, 0, 0, 0, 0, 640, 480);

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

	blit(temp2, screen, 0, 0, 0, 0, 640, 480);

	play_sample(sfx[SFX_WELLDONE].dat, 255, 128, 1000, 0);

	while(key[KEY_ENTER]);
	while(!key[KEY_ENTER]);
	while(key[KEY_ENTER]);
}

void Init_Full_Game(void)
{
	int i;
	char fn[100];
	PACKFILE *file;

	clear(screen);
	set_palette(palette);

	for (i = 0; i < 100; i++)
	map_done[i] = 0;

	strcpy(fn, "./");
	strcat(fn, login);
	strcat(fn, "/");
	strcat(fn, map_save);

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
	char fn[100];
	PACKFILE *file;

	strcpy(fn, "./");
	strcat(fn, login);
	strcat(fn, "/");
	strcat(fn, map_save);

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
	textprintf_centre(screen, fonts[0].dat, 320, 220, 7, "level set of Blocks 4: Ultimate");
	textprintf_centre(screen, fonts[0].dat, 320, 280, 7, "Thank you for playing!");

	fade_in(palette, 10);

	while(key[KEY_ENTER]);
	while(!key[KEY_ENTER]);
}

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