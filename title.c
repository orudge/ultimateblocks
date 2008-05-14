/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"
#include <string.h>

#ifdef WIN32
	#include <direct.h>
#endif

int redraw_flag, title_menu_exit, f_no;

void Presents(void)
{
	clear(screen);
	clear(temp);

	textprintf_centre(temp, fonts[0].dat, 320, 150, 7, "An Ly and Owen Rudge");
	textprintf_centre(temp, fonts[0].dat, 320, 200, 7, "present");

	Transition_In(temp, palette);

	clear_keybuf();
	
	while (!keypressed())
	{
		Poll_Music();
	}

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
	while (!keypressed())
	{
		Poll_Music();
	}

	clear_keybuf();
	while (keypressed())
	{
		Poll_Music();
	}
}

static const Menu main_menu[] = {
	{"Start", 1, MENUITEM_CLOSE},
	{"Editor", 2, MENUITEM_CLOSE},
	{"Options", 3, MENUITEM_CLOSE},
	{"Music", 4, MENUITEM_CLOSE},
	{"Logout", 5, MENUITEM_CLOSE},
	{"About", 6, MENUITEM_CLOSE},
	{"Exit", 7, MENUITEM_CLOSE},
	{END_OF_MENU}
};

int Title_Menu(void)
{
	switch (Display_Menu((const Menu *) &main_menu, NULL, 0, 0))
	{
		case 1:
			return(2);

		case 2:
			return(1);

		case 3:
			Options_Menu();
			return -1;

		case 4:
			Music_Menu();
			return -1;

		case 5:
			Log_In();
			return -1;

		case 6:
			About_Box();
			return -1;

		default:
			return 0;
	}
}


void Log_In(void)
{
	BITMAP *temp2 = create_bitmap(200, 150);
	int i, j;

	rectfill(temp2, 0, 0, 199, 149, makecol(255, 255, 255));
	rect(temp2, 0, 0, 199, 149, makecol(0, 0, 0));

	rectfill(temp2, 10, 10, 189, 18, makecol(180, 180, 255));
	rect(temp2, 10, 10, 189, 18, makecol(0, 0, 0));

	rectfill(temp2, 10, 131, 189, 139, makecol(180, 180, 255));
	rect(temp2, 10, 131, 189, 139, makecol(0, 0, 0));

	text_mode(-1);

	play_sample(sfx[SFX_INQUIRY].dat, sfx_vol, 128, 1000, 0);

	textprintf_centre(temp2, fonts[0].dat, 100, 20, makecol(0, 0, 0), "Log In");
	textprintf_centre(temp2, fonts[0].dat, 100, 70, makecol(0, 0, 0), "_", login);

	for (i = 0; i < 13; i++)
		login[i] = 0;

	for (i = -200; i < 230; i+=10)
	{
		blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 150);
		blit(temp2, screen, 0, 0, i, 100, 200, 150);
		
		while (time_count < 1)
		{
			Poll_Music();
		}

		time_count = 0;
	}

	clear_keybuf();
	i = 0;

	while ((!key[KEY_ENTER]) || (i == 0))
	{
		while(!keypressed())
		{
			Poll_Music();
		}

		j = readkey() & 0xff;

		if (((j >= 'A') && (j <= 'Z')) || ((j >= 'a') && (j <= 'z')) && (i < 8))
		{
			login[i] = j;
			i++;
			rectfill(temp2, 10, 70, 189, 120, makecol(255, 255, 255));
			textprintf_centre(temp2, fonts[0].dat, 100, 70, makecol(0, 0, 0), "%s_", login);
			blit(temp2, screen, 0, 0, 220, 100, 200, 150);
			play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);
		}

		if ((key[KEY_BACKSPACE]) && (i > 0))
		{
			i--;
			login[i] = 0;
			rectfill(temp2, 10, 70, 189, 120, makecol(255, 255, 255));
			textprintf_centre(temp2, fonts[0].dat, 100, 70, makecol(0, 0, 0), "%s_", login);
			blit(temp2, screen, 0, 0, 220, 100, 200, 150);
			play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);
			while(key[KEY_BACKSPACE])
			{
				Poll_Music();
			}
		}
	}

	play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);

	login[i] = '.';
	login[i + 1] = 'l';
	login[i + 2] = 'o';
	login[i + 3] = 'g';

	sprintf(login_path, "%s/%s", find_resource_file(SETTINGS_DIR, NULL), login);

#ifdef WIN32
	_mkdir(login_path);
#else
	mkdir(login_path, 0755);
#endif

	for (i = 200; i < 650; i+=10)
	{
		blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 150);
		blit(temp2, screen, 0, 0, i, 100, 200, 150);
		
		while (time_count < 1)
		{
			Poll_Music();
		}

		time_count = 0;
	}

	blit(title_gfx, screen, 550, 10, 550, 10, 90, 10);
	textprintf(screen, font, 550, 11, makecol(255, 255, 255), "%s", login);

	while(key[KEY_ENTER])
	{
		Poll_Music();
	}
}

int Remember_File_Title(const char *fn, int a, void *b)
{
	if (f_no == MAX_DIRS)
		return(1);

	safe_strcpy(dir[f_no].name, DIR_MAXNAME, get_filename(fn));
	f_no++;

	return(0);
}

static const InfoWindow no_map_box[] = {
	{"Choose Map", FONT_HELV12B},
	{"", FONT_HELV12},
	{"There are no maps present in the \"maps\"", FONT_HELV12},
	{"directory. Please add some, or reinstall the", FONT_HELV12},
	{"game.", FONT_HELV12},
	{END_OF_INFOWINDOW}
};

int Open_Maps_Title(void)
{
 int file_sel = 0;
 char fn[MAX_PATH];
 int i, j, k, l;
 PACKFILE *file;
 BITMAP *temp2 = create_bitmap(400, 300);
 int b4file = 0;
 int font_height = 0;
 char map_path[MAX_PATH];

	f_no = 0;

	sprintf(map_path, "%s/*.map", _map_dir);
	for_each_file_ex(map_path, 0, FA_LABEL | FA_DIREC, Remember_File_Title, 0);

	if (f_no == 0)
	{
		Display_Info_Window((const InfoWindow *) &no_map_box);
		return 0;
	}

 font_height = text_height(fonts[FONT_HELV10].dat);

 /** draw File Box **/

 rectfill(temp2, 0, 0, 399, 299, makecol(255, 255, 255));
 rect(temp2, 0, 0, 399, 299, makecol(0, 0, 0));

 rectfill(temp2, 10, 10, 389, 20, makecol(180, 180, 255));
 rect(temp2, 10, 10, 389, 20, makecol(0, 0, 0));

 rectfill(temp2, 10, 279, 389, 289, makecol(180, 180, 255));
 rect(temp2, 10, 279, 389, 289, makecol(0, 0, 0));

 textprintf_centre(temp2, fonts[FONT_HELV12B].dat, 200, 20, makecol(0, 0, 0), "Choose Map");

 text_mode(makecol(255, 255, 255));
 for (i = 0; i < f_no; i++)
 textprintf(temp2, fonts[FONT_HELV10].dat, 35 + (i/20)*120, 70 + i%20*(font_height), makecol(0, 0, 0), "%s", dir[i].name); // was 10

 text_mode(makecol(180, 180, 255));
 textprintf(temp2, fonts[FONT_HELV10].dat, 35 + (file_sel/20)*120, 70 + file_sel%20*(font_height), makecol(0, 0, 0), "%s", dir[file_sel].name); // was 10

 for (i = -400; i < 130; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 400, 300);
  while (time_count < 1)
  {
	  Poll_Music();
  }
  time_count = 0;
 }

 while ((!key[KEY_ESC]) && (!key[KEY_ENTER]))
 {
	 Poll_Music();

  if ((key[KEY_DOWN]) || (key[KEY_UP]))
  {
   play_sample(sfx[SFX_CLICK].dat, sfx_vol, 128, 1000, 0);
   if (key[KEY_DOWN]) file_sel++;
   if (key[KEY_UP]) file_sel--;

   if (file_sel < 0) file_sel = f_no - 1;
   if (file_sel > f_no - 1) file_sel = 0;

   text_mode(makecol(255, 255, 255));
   for (i = 0; i < f_no; i++)
   textprintf(temp2, fonts[FONT_HELV10].dat, 35 + (i/20)*120, 70 + i%20*(font_height), makecol(0, 0, 0), "%s", dir[i].name); // was 10

   text_mode(makecol(180, 180, 255));
   textprintf(temp2, fonts[FONT_HELV10].dat, 35 + (file_sel/20)*120, 70 + file_sel%20*font_height, makecol(0, 0, 0), "%s", dir[file_sel].name); // was 10

   blit(temp2, screen, 0, 0, 120, 100, 400, 300);

   while ((key[KEY_UP]) || (key[KEY_DOWN]))
   {
	   Poll_Music();
   }
  }
 }

 play_sample(sfx[SFX_FALL].dat, sfx_vol, 128, 1000, 0);

 if (key[KEY_ESC]) l = 1;
 else l = 0;

 for (i = 120; i < 650; i+=10)
 {
  blit(title_gfx, screen, i - 10, 100, i-10, 100, 10, 300);
  blit(temp2, screen, 0, 0, i, 100, 400, 300);
  while (time_count < 1)
  {
	  Poll_Music();
  }
  time_count = 0;
 }

 if (l == 1)
 {
  while (key[KEY_ESC])
  {
	  Poll_Music();
  }
  return 0;
 }

 safe_strcpy(fn, MAX_PATH, _map_dir);
 //safe_strcpy(fn, MAX_PATH, "./maps/");
 safe_strcat(fn, MAX_PATH, dir[file_sel].name);
 safe_strcpy(map_save, MAX_PATH, dir[file_sel].name);

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
		pack_fread(&map_motif[l][0], 4, file);
    }
 }
 else
 {
    for (l = 0; l < 100; l++)
	{
		safe_strcpy(map_motif[l], MOTIF_ID_LEN, GetMotifFromInt(pack_igetw(file)));
	}
 }

 pack_fclose(file);

 while(key[KEY_ENTER])
 {
	 Poll_Music();
 }

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
