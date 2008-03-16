/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <libcda.h>
#include "blocks3.h"

void In_Game_Menu(void)
{
 BITMAP *temp2 = create_bitmap(200, 300);
 int item, game_menu_exit;
 int i;

 blit(screen, temp, 0, 0, 0, 0, 640, 480);

 rect(temp2, 0, 0, 199, 299, 0);

 game_menu_exit = 0;
 item = 0;

 rectfill(temp2, 1, 1, 198, 298, 7);
 rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
 rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
 rect(temp2, 10, 10, 189, 20, 0);
 rect(temp2, 10, 279, 189, 289, 0);

 rectfill(temp2, 10, item*40 + 35, 190, item*40 + 65, makecol(180, 180, 255));
 text_mode(-1);

 textprintf_centre(temp2, fonts[0].dat, 100, 35,  0, "Return");
 textprintf_centre(temp2, fonts[0].dat, 100, 75, 0, "Restart");
 textprintf_centre(temp2, fonts[0].dat, 100, 115, 0, "Level");
 textprintf_centre(temp2, fonts[0].dat, 100, 155, 0, "Options");
 textprintf_centre(temp2, fonts[0].dat, 100, 195, 0, "Music");
 textprintf_centre(temp2, fonts[0].dat, 100, 235, 0, "Exit");

 for (i = -200; i < 230; i+=10)
 {
  blit(temp, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 200, 300);
  while (time_count < 1)
  {
	  Poll_Music();
  }

  time_count = 0;
 }

 while (key[KEY_ESC])
 {
	 Poll_Music();
 }

 while(!game_menu_exit)
 {
  rectfill(temp2, 1, 1, 198, 298, 7);
  rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
  rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
  rect(temp2, 10, 10, 189, 20, 0);
  rect(temp2, 10, 279, 189, 289, 0);

  rectfill(temp2, 10, item*40 + 35, 190, item*40 + 65, makecol(180, 180, 255));
  text_mode(-1);

  textprintf_centre(temp2, fonts[0].dat, 100, 35,  0, "Return");
  textprintf_centre(temp2, fonts[0].dat, 100, 75, 0, "Restart");
  textprintf_centre(temp2, fonts[0].dat, 100, 115, 0, "Level");
  textprintf_centre(temp2, fonts[0].dat, 100, 155, 0, "Options");
  textprintf_centre(temp2, fonts[0].dat, 100, 195, 0, "Music");
  textprintf_centre(temp2, fonts[0].dat, 100, 235, 0, "Exit");

  blit(temp2, screen, 0, 0, 220, 100, 200, 300);

  while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]))
  {
	  Poll_Music();
  }

  if (key[KEY_ESC])
  {
   play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
   item = 0;
   game_menu_exit = 1;
  }

  if (key[KEY_ENTER])
  {
   game_menu_exit = 1;
   play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
  }

  if (key[KEY_UP]) item--;
  if (key[KEY_DOWN]) item++;
  play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

  if (item < 0) item = 5;
  if (item > 5) item = 0;

  while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]))
  {
	  Poll_Music();
  }
 }

 for (i = 220; i < 650; i+=10)
 {
  blit(temp, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 200, 300);
  while (time_count < 1)
  {
	  Poll_Music();
  }
  time_count = 0;
 }

 if (item == 0) blit(temp, screen, 0, 0, 0, 0, 640, 480);
 if (item == 2)
 {
  Change_Levels();
  if (game_exit == 2)
  {
   Maps_to_Map(lev);
   Draw_Map();
   Init_Level();
   Map_Setup();

   game_exit = 0;
  }
  blit(temp, screen, 0, 0, 0, 0, 640, 480);
 }
 if (item == 3) Options_Menu();

 if (item == 1)
 {
  Maps_to_Map(lev);
  Draw_Map();
  Init_Level();
  Map_Setup();
  blit(temp, screen, 0, 0, 0, 0, 640, 480);
 }

 if (item == 4) Music_Menu();

 if (item == 5) game_exit = 2;
}

void Options_Menu(void)
{
 BITMAP *temp2 = create_bitmap(200, 230);
 int item = 0, options_exit = 0, i = 0;
	int old_mus_vol, old_cd_vol;

 blit(screen, temp, 0, 0, 0, 0, 640, 480);

 rect(temp2, 0, 0, 199, 229, 0);

 rectfill(temp2, 1, 1, 198, 228, 7);
 rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
 rectfill(temp2, 10, 210, 189, 220, makecol(128, 128, 128));
 rect(temp2, 10, 10, 189, 20, 0);
 rect(temp2, 10, 210, 189, 220, 0);

 rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
 text_mode(-1);

 textprintf_centre(temp2, fonts[0].dat, 100, 40,  0, "Music Vol");
 textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "SFX Vol");
 textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "Return");

 for (i = -200; i < 230; i+=10)
 {
  blit(temp, screen, i - 10, 100, i-10, 100, 10, 230);
  blit(temp2, screen, 0, 0, i, 100, 200, 230);
  while (time_count < 1);
  time_count = 0;
 }

 options_exit = 0;
 item = 0;

	old_mus_vol = mus_vol;
	old_cd_vol = cd_vol;

 while (!options_exit)
 {
  rectfill(temp2, 1, 1, 198, 228, 7);
  rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
  rectfill(temp2, 10, 210, 189, 220, makecol(128, 128, 128));
  rect(temp2, 10, 10, 189, 20, 0);
  rect(temp2, 10, 210, 189, 220, 0);

  rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
  text_mode(-1);

  textprintf_centre(temp2, fonts[0].dat, 100, 40,  0, "Music: %d", mus_vol*100/255);
  textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "CD: %d", cd_vol*100/255);
  textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "SFX: %d", sfx_vol*100/255);
  textprintf_centre(temp2, fonts[0].dat, 100, 160, 0, "Return");

  blit(temp2, screen, 0, 0, 220, 100, 200, 230);

	while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) &&
			(!key[KEY_LEFT]) && (!key[KEY_RIGHT]) &&
			(!key[KEY_ENTER]))
	{
		Poll_Music();
	}


  if (key[KEY_ESC])
  {
   play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
   item = 0;
   options_exit = 1;
  }

  if ((key[KEY_ENTER]) || (key[KEY_RIGHT]))
  {
   if (item == 0) mus_vol+=3;
   if (item == 1) cd_vol+=3;
   if (item == 2) sfx_vol+=3;

   if (item == 3)
   {
    options_exit = 1;
    play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
   }

   time_count = 0;
	while (time_count < 10)
	{
		Poll_Music();
	}

  }

  if (key[KEY_LEFT])
  {
   if (item == 0) mus_vol-=3;
   if (item == 1) cd_vol-=3;
   if (item == 2) sfx_vol-=3;

   time_count = 0;
	while (time_count < 10)
	{
		Poll_Music();
	}
  }

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
		cd_set_volume(cd_vol, cd_vol);
		old_cd_vol = cd_vol;
	}

  if (key[KEY_UP]) item--;
  if (key[KEY_DOWN]) item++;
  play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

  if (item < 0) item = 3;
  if (item > 3) item = 0;

  while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) ||
         (key[KEY_ENTER]));
 }

 for (i = 220; i < 650; i+=10)
 {
	blit(temp, screen, i - 10, 100, i-10, 100, 10, 230);
	blit(temp2, screen, 0, 0, i, 100, 200, 230);
	while (time_count < 1)
	{
		Poll_Music();
	}

  time_count = 0;
 }
}

void Music_Menu(void)
{
 BITMAP *temp2 = create_bitmap(200, 190);
 int item = 0, music_exit = 0, i = 0;

 blit(screen, temp, 0, 0, 0, 0, 640, 480);

 rect(temp2, 0, 0, 199, 189, 0);

 rectfill(temp2, 1, 1, 198, 188, 7);
 rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
 rectfill(temp2, 10, 170, 189, 180, makecol(128, 128, 128));
 rect(temp2, 10, 10, 189, 20, 0);
 rect(temp2, 10, 170, 189, 180, 0);

 rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
 text_mode(-1);

 textprintf_centre(temp2, fonts[0].dat, 100, 40,  0, "CD Player", mus_vol*100/255);
 textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "MOD Music", sfx_vol*100/255);
 textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "Return");

 for (i = -200; i < 230; i+=10)
 {
  blit(temp, screen, i - 10, 100, i-10, 100, 10, 190);
  blit(temp2, screen, 0, 0, i, 100, 200, 190);
	while (time_count < 1)
	{
		Poll_Music();
	}

  time_count = 0;
 }

 music_exit = 0;
 item = 0;

 while (!music_exit)
 {
  rectfill(temp2, 1, 1, 198, 188, 7);
  rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
  rectfill(temp2, 10, 170, 189, 180, makecol(128, 128, 128));
  rect(temp2, 10, 10, 189, 20, 0);
  rect(temp2, 10, 170, 189, 180, 0);

  rectfill(temp2, 10, item*40 + 40, 190, item*40 + 70, makecol(180, 180, 255));
  text_mode(-1);

  textprintf_centre(temp2, fonts[0].dat, 100, 40,  0, "CD Player", mus_vol*100/255);
  textprintf_centre(temp2, fonts[0].dat, 100, 80, 0, "MOD Music", sfx_vol*100/255);
  textprintf_centre(temp2, fonts[0].dat, 100, 120, 0, "Return");

  blit(temp2, screen, 0, 0, 220, 100, 200, 300);

  while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]))
  {
		Poll_Music();
  }


  if (key[KEY_ESC])
  {
   play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);
   item = -1;
   music_exit = 1;
  }

  if (key[KEY_ENTER])
  {
   music_exit = 1;
  }

  if (key[KEY_UP]) item--;
  if (key[KEY_DOWN]) item++;
  play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);

  if (item < 0) item = 2;
  if (item > 2) item = 0;

  while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]))
  {
		Poll_Music();
  }

 }

 for (i = 220; i < 650; i+=10)
 {
  blit(temp, screen, i - 10, 100, i-10, 100, 10, 190);
  blit(temp2, screen, 0, 0, i, 100, 200, 190);
  while (time_count < 1)
  {
	  Poll_Music();
  }
  time_count = 0;
 }

 if (item == 0) CD_Player();
 if (item == 1) Mod_Music();
}


