/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"
#include <string.h>

int redraw_flag, title_menu_exit, f_no;

void Presents(void)
{
	clear(screen);
	clear(temp);

	textprintf_centre(temp, fonts[0].dat, 320, 150, 7, "An Ly and Owen Rudge");
	textprintf_centre(temp, fonts[0].dat, 320, 200, 7, "Present");

	Transition_In(temp, palette);

	clear_keybuf();
	while (!keypressed());

	Transition_Out(temp, palette);
}

void Title(void)
{
	scare_mouse();

	clear(screen);
/*	set_palette(black_palette);
	blit(title_gfx, screen, 0, 0, 0, 0, 640, 480);

	text_mode(-1);
	textprintf(screen, font, 600, 468, 7, "v0.5");

	fade_in(title_palette, 5);*/

	set_palette(title_palette);
	Transition_In(title_gfx, title_palette);
	set_palette(title_palette); // just to be sure :-)

	clear_keybuf();
	while (!keypressed());
	clear_keybuf();
	while (keypressed());
}

int Title_Menu(void)
{
 BITMAP *temp2 = create_bitmap(200, 300);
 int item, game_menu_exit;
 int i, j;

/* for (i = 0; i < 640; i+=2)
 for (j = 0; j < 480; j+=2)
 {
  putpixel(screen, i, j, 0);
  putpixel(screen, i + 1, j + 1, 0);
 }*/

 rect(temp2, 0, 0, 199, 299, 0);

 title_menu_exit = 0;
 item = 0;

 /** Come on Down!! (menu) **/

 rectfill(temp2, 1, 1, 198, 298, 7);
 rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
 rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
 rect(temp2, 10, 10, 189, 20, 0);
 rect(temp2, 10, 279, 189, 289, 0);

 rectfill(temp2, 10, item*40 + 35, 190, item*40 + 65, makecol(180, 180, 255));
 text_mode(-1);

 textprintf_centre(temp2, fonts[0].dat, 100, 35,  0, "Start");
 textprintf_centre(temp2, fonts[0].dat, 100, 75, 0, "Editor");
 textprintf_centre(temp2, fonts[0].dat, 100, 115, 0, "Options");
 textprintf_centre(temp2, fonts[0].dat, 100, 155, 0, "Music");
 textprintf_centre(temp2, fonts[0].dat, 100, 195, 0, "Logout");
 textprintf_centre(temp2, fonts[0].dat, 100, 235, 0, "Exit");

 blit(title_gfx, screen, 520, 10, 520, 10, 120, 10);
 textprintf(screen, font, 520, 11, 7, "%s", login);

 for (i = -200; i < 230; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 200, 300);
  while (time_count < 1);
  time_count = 0;
 }

 while(!title_menu_exit)
 {
  rectfill(temp2, 1, 1, 198, 298, 7);
  rectfill(temp2, 10, 10, 189, 20, makecol(128, 128, 128));
  rectfill(temp2, 10, 279, 189, 289, makecol(128, 128, 128));
  rect(temp2, 10, 10, 189, 20, 0);
  rect(temp2, 10, 279, 189, 289, 0);

  rectfill(temp2, 10, item*40 + 35, 190, item*40 + 65, makecol(180, 180, 255));
  text_mode(-1);

  textprintf_centre(temp2, fonts[0].dat, 100, 35,  0, "Start");
  textprintf_centre(temp2, fonts[0].dat, 100, 75, 0, "Editor");
  textprintf_centre(temp2, fonts[0].dat, 100, 115, 0, "Options");
  textprintf_centre(temp2, fonts[0].dat, 100, 155, 0, "Music");
  textprintf_centre(temp2, fonts[0].dat, 100, 195, 0, "Logout");
  textprintf_centre(temp2, fonts[0].dat, 100, 235, 0, "Exit");

  blit(temp2, screen, 0, 0, 220, 100, 200, 300);

  while ((!key[KEY_ESC]) && (!key[KEY_UP]) && (!key[KEY_DOWN]) && (!key[KEY_ENTER]));

  if (key[KEY_ESC])
  {
   play_sample(sfx[SFX_FALL].dat, 255, 128, 1000, 0);
   item = 4;
  }

  if (key[KEY_ENTER])
  {
   title_menu_exit = 1;
   play_sample(sfx[SFX_FALL].dat, 255, 128, 1000, 0);
  }

  if (key[KEY_UP]) item--;
  if (key[KEY_DOWN]) item++;

  if (item < 0) item = 5;
  if (item > 5) item = 0;
  play_sample(sfx[SFX_CLICK].dat, 255, 128, 1000, 0);

  while ((key[KEY_ESC]) || (key[KEY_UP]) || (key[KEY_DOWN]) || (key[KEY_ENTER]));
 }

 for (i = 220; i < 650; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 200, 300);
  while (time_count < 1);
  time_count = 0;
 }

 if (item == 0) return 2;
 if (item == 1) return 1;
 if (item == 2)
 {
  Options_Menu();
  return -1;
 }
 if (item == 3)
 {
  Music_Menu();
  return -1;
 }

 if (item == 4)
 {
  Log_In();
  return -1;
 }
 if (item == 5) return 0;
}

void Log_In(void)
{
 BITMAP *temp2 = create_bitmap(200, 150);
 int i, j;

 rectfill(temp2, 0, 0, 199, 149, 7);
 rect(temp2, 0, 0, 199, 149, 0);

 rectfill(temp2, 10, 10, 189, 18, makecol(180, 180, 255));
 rect(temp2, 10, 10, 189, 18, 0);

 rectfill(temp2, 10, 131, 189, 139, makecol(180, 180, 255));
 rect(temp2, 10, 131, 189, 139, 0);

 text_mode(-1);

 play_sample(sfx[SFX_INQUIRY].dat, 255, 128, 1000, 0);

 textprintf_centre(temp2, fonts[0].dat, 100, 20, 0, "Log In");
 textprintf_centre(temp2, fonts[0].dat, 100, 70, 0, "_", login);

 for (i = 0; i < 13; i++)
 login[i] = 0;

 for (i = -200; i < 230; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 150);
  blit(temp2, screen, 0, 0, i, 100, 200, 150);
  while (time_count < 1);
  time_count = 0;
 }

 clear_keybuf();
 i = 0;

 while ((!key[KEY_ENTER]) || (i == 0))
 {
  while(!keypressed());

  j = readkey() & 0xff;

  if (((j > 64) && (j < 91)) || ((j > 96) && (j < 123)) && (i < 8))
  {
   login[i] = j;
   i++;
   rectfill(temp2, 10, 70, 189, 120, 7);
   textprintf_centre(temp2, fonts[0].dat, 100, 70, 0, "%s_", login);
   blit(temp2, screen, 0, 0, 220, 100, 200, 150);
   play_sample(sfx[SFX_CLICK].dat, 255, 128, 1000, 0);
  }
  if ((key[KEY_BACKSPACE]) && (i > 0))
  {
   i--;
   login[i] = 0;
   rectfill(temp2, 10, 70, 189, 120, 7);
   textprintf_centre(temp2, fonts[0].dat, 100, 70, 0, "%s_", login);
   blit(temp2, screen, 0, 0, 220, 100, 200, 150);
   play_sample(sfx[SFX_CLICK].dat, 255, 128, 1000, 0);
   while(key[KEY_BACKSPACE]);
  }
 }

 play_sample(sfx[SFX_FALL].dat, 255, 128, 1000, 0);

 login[i] = 46;
 login[i + 1] = 76;
 login[i + 2] = 79;
 login[i + 3] = 71;

 mkdir(login, 0);

 for (i = 200; i < 650; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 150);
  blit(temp2, screen, 0, 0, i, 100, 200, 150);
  while (time_count < 1);
  time_count = 0;
 }

 blit(title_gfx, screen, 550, 10, 550, 10, 90, 10);
 textprintf(screen, font, 550, 11, 7, "%s", login);

 while(key[KEY_ENTER]);
}

void Remember_File_Title(char *fn, int a, int b)
{
 strcpy(dir[f_no].name, get_filename(fn));
 f_no++;
}

int Open_Maps_Title(void)
{
 int mx, my;
 int file_sel = 0;
 char fn[20];
 int i, j, k, l;
 PACKFILE *file;
 BITMAP *temp2 = create_bitmap(400, 300);
 int b4file = 0;
 char tmpbuf[4];

 f_no = 0;
 for_each_file("./maps/*.map", FA_RDONLY | FA_ARCH, Remember_File_Title, 0);

 /** draw File Box **/

 rectfill(temp2, 0, 0, 399, 299, 7);
 rect(temp2, 0, 0, 399, 299, 0);

 rectfill(temp2, 10, 10, 389, 20, makecol(180, 180, 255));
 rect(temp2, 10, 10, 389, 20, 0);

 rectfill(temp2, 10, 279, 389, 289, makecol(180, 180, 255));
 rect(temp2, 10, 279, 389, 289, 0);

 textprintf_centre(temp2, fonts[0].dat, 200, 20, 0, "Choose Map");

 text_mode(7);
 for (i = 0; i < f_no; i++)
 textprintf(temp2, font, 35 + (i/20)*120, 70 + i%20*10, 0, "%s", dir[i].name);

 text_mode(makecol(180, 180, 255));
 textprintf(temp2, font, 35 + (file_sel/20)*120, 70 + file_sel%20*10, 0, "%s", dir[file_sel].name);

 for (i = -400; i < 130; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 400, 300);
  while (time_count < 1);
  time_count = 0;
 }

 while ((!key[KEY_ESC]) && (!key[KEY_ENTER]))
 {
  if ((key[KEY_DOWN]) || (key[KEY_UP]))
  {
   play_sample(sfx[SFX_CLICK].dat, 255, 128, 1000, 0);
   if (key[KEY_DOWN]) file_sel++;
   if (key[KEY_UP]) file_sel--;

   if (file_sel < 0) file_sel = f_no - 1;
   if (file_sel > f_no - 1) file_sel = 0;

   text_mode(7);
   for (i = 0; i < f_no; i++)
   textprintf(temp2, font, 35 + (i/20)*120, 70 + i%20*10, 0, "%s", dir[i].name);

   text_mode(makecol(180, 180, 255));
   textprintf(temp2, font, 35 + (file_sel/20)*120, 70 + file_sel%20*10, 0, "%s", dir[file_sel].name);

   blit(temp2, screen, 0, 0, 120, 100, 400, 300);

   while ((key[KEY_UP]) || (key[KEY_DOWN]));
  }
 }

 play_sample(sfx[SFX_FALL].dat, 255, 128, 1000, 0);

 if (key[KEY_ESC]) l = 1;
 else l = 0;

 for (i = 120; i < 650; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 400, 300);
  while (time_count < 1);
  time_count = 0;
 }

 if (l == 1)
 {
  while (key[KEY_ESC]);
  return 0;
 }

 strcpy(fn, "./maps/");
 strcat(fn, dir[file_sel].name);
 strcpy(map_save, dir[file_sel].name);

 file = pack_fopen(fn, "rp");
 
 if (pack_igetw(file) == 'B' &&
     pack_igetw(file) == 'L' &&
     pack_igetw(file) == 'n')
 {
    b4file = 1;
 }
 else
    pack_fseek(file, 0);

 for (l = 0; l < 100; l++)
 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 maps[i][j][k][l] = pack_igetw(file);

 if (b4file == 1)
 {
    for (l = 0; l < 100; l++)
    {
       pack_fread(&map_motif[l], 4, file);
//       strcpy(map_motif[l], tmpbuf);
    }
 }
 else
 {
    for (l = 0; l < 100; l++)
       strcpy(map_motif[l], GetMotifFromInt(pack_igetw(file)));
 }

 pack_fclose(file);

 while(key[KEY_ENTER]);

 return 1;
}

char *GetMotifFromInt(int motif)
{
   int i;
   
   for (i = 0; i < num_motifs; i++)
   {
      if (motifs[i].special == motif)
         return(motifs[i].uid);
   }

   // Not found
   return("    ");
}