/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"
#include <string.h>

int redraw_flag, block_type, mb_flag, level_flag;
int menu_hl, item_hl;
int elev;
int icon_start;
int exit_flag;

int f_no;

char f[20];

void Editor(void)
{
 int mx, my, mb;
 int i, j;

 /** Editor Menus **/

 menu[0].x = 12;
 menu[0].y = 10;
 menu[0].w = 128;
 menu[0].h = 50;
 menu[0].n = 3;

 strcpy(menu[0].item[0].name, "Open        F3");
 strcpy(menu[0].item[1].name, "Save        F2");
 strcpy(menu[0].item[2].name, "Exit     Alt-X");
 menu[1].x = 72;
 menu[1].y = 10;
 menu[1].w = 184;
 menu[1].h = 70;
 menu[1].n = 5;

 strcpy(menu[1].item[0].name, "Clear          Ctrl-N");
 strcpy(menu[1].item[1].name, "Clear All            ");
 strcpy(menu[1].item[2].name, "Copy         Ctrl-Ins");
 strcpy(menu[1].item[3].name, "Paste       Shift-Ins");
 strcpy(menu[1].item[4].name, "Arrange              ");


 menu[2].x = 132;
 menu[2].y = 10;
 menu[2].w = 64;
 menu[2].h = 70;
 menu[2].n = 5;

/* strcpy(menu[2].item[0].name, "Sunny ");
 strcpy(menu[2].item[1].name, "Castle");
 strcpy(menu[2].item[2].name, "Rocky ");
 strcpy(menu[2].item[3].name, "Snowy ");
 strcpy(menu[2].item[4].name, "Jelly ");*/

 for (j = 0; j < num_motifs; j++)
    strcpy(menu[2].item[j].name, motifs[j].title);


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

 set_pallete(pallete);

 Clear_All();

 Maps_to_Map(0);

 block_type = BOX;
 level_flag = 0;
 exit_flag = 0;

 show_mouse(screen);

 scare_mouse();
 Editor_Draw_Screen();
 unscare_mouse();

 while (!exit_flag)
 {
  mx = mouse_x;
  my = mouse_y;
  mb = mouse_b;

  if ((mb & 1) && (mx > 32) && (my > 32) && (mx < 608) && (my < 448)) Editor_Put_Block(mx/32, my/32, block_type);
  if ((mb & 2) && (mx > 32) && (my > 32) && (mx < 608) && (my < 448)) Editor_Put_Block(mx/32, my/32, 0);

  if ((mb & 1) && (my > 448)) Editor_Change_Block_Type(mx/32);

  if (my < 10) Highlight_Menu(mx);
  else
  if (menu_hl != -1)
  {
   Editor_Draw_Menu();
   blit(temp, screen, 0, 0, 0, 0, 232, 10);
   menu_hl = -1;
  }

  if (key[KEY_TAB])
  {
   level_flag = 1 - level_flag;
   while(key[KEY_TAB]);

   scare_mouse();
   if (level_flag) textprintf(screen, font, 480, 2, 0, "Top   ");
   else textprintf(screen, font, 480, 2, 0, "Bottom");
   unscare_mouse();
  }

  if ((key[KEY_F9]) || ((mb & 1) && (menu_hl == 3)))
  {
   Map_to_Maps(elev);

   game_type = EDITOR_GAME;
   lev = elev;

   scare_mouse();
   Run_Level();
   fade_in(pallete, 10);
   unscare_mouse();

   Maps_to_Map(elev);
   redraw_flag = 1;
  }

  if ((menu_hl != -1) && (mb & 1)) Drop_Down();

  if (key[KEY_MINUS_PAD])
  {
   Map_to_Maps(elev);

   elev--;
   if (elev == -1) elev = 99;

   Maps_to_Map(elev);

   Editor_Draw_Menu();
   blit(temp, screen, 0, 0, 0, 0, 639, 10);

   while (key[KEY_MINUS_PAD]);
   redraw_flag = 1;
  }

  if (key[KEY_PLUS_PAD])
  {
   Map_to_Maps(elev);

   elev++;
   if (elev == 100) elev = 0;

   Maps_to_Map(elev);

   Editor_Draw_Menu();
   blit(temp, screen, 0, 0, 0, 0, 639, 10);

   while (key[KEY_PLUS_PAD]);
   redraw_flag = 1;
  }

  if (key[KEY_PGUP])
  {
   Map_to_Maps(elev);

   elev+=10;
   if (elev > 99) elev -= 100;

   Maps_to_Map(elev);

   Editor_Draw_Menu();
   blit(temp, screen, 0, 0, 0, 0, 639, 10);

   while (key[KEY_PGUP]);
   redraw_flag = 1;
  }

  if (key[KEY_PGDN])
  {
   Map_to_Maps(elev);

   elev-=10;
   if (elev < 0) elev += 100;

   Maps_to_Map(elev);

   Editor_Draw_Menu();
   blit(temp, screen, 0, 0, 0, 0, 639, 10);

   while (key[KEY_PGDN]);
   redraw_flag = 1;
  }

  if (((key[KEY_LCONTROL]) || (key[KEY_RCONTROL])) && (key[KEY_INSERT]))
  {
   Map_to_Maps(elev);
   Copy_To_Buffer(elev);
   redraw_flag = 1;
   while(key[KEY_INSERT]);
  }

  if (((key[KEY_LSHIFT]) || (key[KEY_RSHIFT])) && (key[KEY_INSERT]))
  {
   Copy_From_Buffer(elev);
   Maps_to_Map(elev);
   redraw_flag = 1;
   while(key[KEY_INSERT]);
  }

  if ((key[KEY_RIGHT]) && (icon_start < 20))
  {
   icon_start++;
   Editor_Draw_Tiles();
   scare_mouse();
   blit(temp, screen, 0, 448, 0, 448, 640, 32);
   unscare_mouse();
   while (key[KEY_RIGHT]);
  }

  if ((key[KEY_LEFT]) && (icon_start > 0))
  {
   icon_start--;
   Editor_Draw_Tiles();
   scare_mouse();
   blit(temp, screen, 0, 448, 0, 448, 640, 32);
   unscare_mouse();
   while (key[KEY_LEFT]);
  }

  if (redraw_flag)
  {
   Editor_Draw_Screen();
   redraw_flag = 0;
  }

  if (key[KEY_F2])
  {
   while (key[KEY_F2]);
   Save_Maps();

   redraw_flag = 1;
  }

  if (key[KEY_F3])
  {
   while (key[KEY_F3]);
   Open_Maps();

   redraw_flag = 1;
  }

  if ((mx > 32) && (my > 32) && (mx < 608) && (my < 448)) Editor_Key_Put_Tile(mx/32, my/32);

  if (mb & 1) mb_flag = 1;
  else mb_flag = 0;
 }

 blit(screen, temp, 0, 0, 0, 0, 640, 480);

 Transition_Out(temp, pallete);
}

void Editor_Draw_Screen(void)
{
 Change_Motif(map_motif[elev]);

 blit(back, temp, 0, 0, 0, 0, 640, 480);
 Draw_Map();

 Editor_Draw_Menu();
 Editor_Draw_Tiles();

 scare_mouse();
 blit(temp, screen, 0, 0, 0, 0, 640, 480);
 unscare_mouse();
}

void Editor_Draw_Menu(void)
{
 rectfill(temp, 0, 0, 639, 12, 7);
 rect(temp, 0, 0, 639, 12, 0);

 text_mode(7);

 textprintf(temp, font, 12, 2, 0, " File ");
 textprintf(temp, font, 72, 2, 0, " Edit ");
 textprintf(temp, font, 132, 2, 0, " Motif ");
 textprintf(temp, font, 192, 2, 0, " Run ");

 if (level_flag) textprintf(temp, font, 480, 2, 0, "Top   ");
 else textprintf(temp, font, 480, 2, 0, "Bottom");

 textprintf(temp, font, 560, 2, 0, "Level: %d ", elev);

}

void Editor_Put_Block(int x, int y, int b)
{
 if (map[x][y][level_flag] != b)
 {
  map[x][y][level_flag] = b;
  redraw_flag = 1;
 }
}

void Highlight_Menu(int x)
{
 int old_menu_hl;

 old_menu_hl = menu_hl;

 text_mode(7);

 textprintf(temp, font, 12, 2, 0, " File ");
 textprintf(temp, font, 72, 2, 0, " Edit ");
 textprintf(temp, font, 132, 2, 0, " Motif ");
 textprintf(temp, font, 192, 2, 0, " Run ");

 text_mode(makecol(180, 180, 255));

 if ((x > 20) && ( x < 52) && (menu_hl != 0))
 {
  textprintf(temp, font, 12, 2, 0, " File ");
  menu_hl = 0;
 }
 if ((x > 80) && ( x < 112) && (menu_hl != 1))
 {
  textprintf(temp, font, 72, 2, 0, " Edit ");
  menu_hl = 1;
 }
 if ((x > 140) && ( x < 180) && (menu_hl != 2))
 {
  textprintf(temp, font, 132, 2, 0, " Motif ");
  menu_hl = 2;
 }
 if ((x > 200) && ( x < 224) && (menu_hl != 3))
 {
  textprintf(temp, font, 192, 2, 0, " Run ");
  menu_hl = 3;
 }

 if (old_menu_hl == menu_hl) return;

 scare_mouse();
 blit(temp, screen, 0, 0, 0, 0, 232, 10);
 unscare_mouse();
}

void Draw_Drop_Down(int item)
{
 int i;

 rectfill(temp, menu[menu_hl].x, menu[menu_hl].y,
          menu[menu_hl].x + menu[menu_hl].w,
          menu[menu_hl].y + menu[menu_hl].h, 7);

 rect(temp, menu[menu_hl].x, menu[menu_hl].y,
          menu[menu_hl].x + menu[menu_hl].w - 1,
          menu[menu_hl].y + menu[menu_hl].h - 1, 0);

 text_mode(7);

 for (i = 0; i < menu[menu_hl].n; i++)
 textprintf(temp, font, menu[menu_hl].x + 8, menu[menu_hl].y + 10 + i*10, 0, "%s", menu[menu_hl].item[i].name);

 if ((item > -1) && (item < menu[menu_hl].n))
 {
  text_mode(makecol(180, 180, 255));
  textprintf(temp, font, menu[menu_hl].x + 8, menu[menu_hl].y + 10 + item*10, 0, "%s", menu[menu_hl].item[item].name);
 }

 scare_mouse();

 blit(temp, screen, menu[menu_hl].x, menu[menu_hl].y,
          menu[menu_hl].x, menu[menu_hl].y,
          menu[menu_hl].w, menu[menu_hl].h);

 unscare_mouse();
}

void Drop_Down(void)
{
 int old_item_hl;

 old_item_hl = -1;

 while (mouse_b);

 while ((!(mouse_b & 1)) && (mouse_x > menu[menu_hl].x)
    && (mouse_x < menu[menu_hl].x + menu[menu_hl].w)
    && (mouse_y < menu[menu_hl].y + menu[menu_hl].h))
 {
  old_item_hl = item_hl;

  item_hl = (mouse_y - menu[menu_hl].y - 10)/10;

  if (item_hl != old_item_hl) Draw_Drop_Down(item_hl);
 }

 redraw_flag = 1;

 if (!(mouse_b & 1)) return;

 while (mouse_b);

 if (menu_hl == 0)
 {
  if (item_hl == 2) exit_flag = 1;
  if (item_hl == 0) Open_Maps(13);
  if (item_hl == 1) Save_Maps(13);
 }
 if (menu_hl == 1)
 {
  if (item_hl == 0) Clear(elev);
  if (item_hl == 1) Clear_All();

  if (item_hl == 2)
  {
   Map_to_Maps(elev);
   Copy_To_Buffer(elev);
  }

  if (item_hl == 3)
  {
   Copy_From_Buffer(elev);
   Maps_to_Map(elev);
  }
 }
 if (menu_hl == 2)
 {
/*  if (item_hl == 0) Change_Motif(SUNNY);
  if (item_hl == 1) Change_Motif(CASTLE);
  if (item_hl == 2) Change_Motif(ROCKY);
  if (item_hl == 3) Change_Motif(SNOWY);
  map_motif[elev] = motif;*/
  Change_Motif(motifs[item_hl].uid);
  strcpy(map_motif[elev], motifs[item_hl].uid);
 }
}

void Remember_File(char *fn, int a, int b)
{
 strcpy(dir[f_no].name, get_filename(fn));
 f_no++;
}

void Directory(void)
{
 int j;

 rectfill(temp, 0, 12, 640, 480, 7);
 rectfill(temp, 0, 0, 640, 12, makecol(180, 180, 255));
 line(temp, 0, 12, 640, 12, 0);
 line(temp, 128, 12, 128, 468, 0);
 line(temp, 256, 12, 256, 468, 0);
 line(temp, 384, 12, 384, 468, 0);
 line(temp, 512, 12, 512, 468, 0);
 line(temp, 0, 468, 640, 468, 0);

 f_no = 0;
 for_each_file("./maps/*.map", FA_RDONLY | FA_ARCH, Remember_File, 0);

 text_mode(7);
 for (j = 0; j < f_no; j++)
 textprintf(temp, font, j/37*128 + 10, (j % 37) * 12 + 20, 0, "%s", dir[j].name);
}

int Yes_Or_No(void)
{
 int m, mx, my;

 while (mouse_b & 1);
 m = -1;

 while ((!(mouse_b & 1)) && (!key[KEY_ESC])
       && (!key[KEY_Y]) && (!key[KEY_N]))
 {
  mx = mouse_x;
  my = mouse_y;
  if ((my > 130) && (my < 140))
  {
   if ((mx > 276) && (mx < 300) && (m != 1))
   {
    m = 1;
    scare_mouse();
    text_mode(7);
    textprintf(screen, font, 268, 130, 0, " YES      NO ");
    text_mode(makecol(180, 180, 255));
    textprintf(screen, font, 268, 130, 0, " YES ");
    unscare_mouse();
   }
   if ((mx > 348) && (mx < 364) && (m != 2))
   {
    m = 2;
    scare_mouse();
    text_mode(7);
    textprintf(screen, font, 268, 130, 0, " YES      NO ");
    text_mode(makecol(180, 180, 255));
    textprintf(screen, font, 340, 130, 0, " NO ");
    unscare_mouse();
   }
  }
 }

 if (((m == 1) && (mouse_b & 1)) || (key[KEY_Y])) return 1;

 return 0;
}

void Open_Maps(void)
{
 int mx, my;
 int file_sel = 0;
 char fn[20];
 int i, j, k, l;
 PACKFILE *file;
 int b4file = 0;
 char tmpbuf[4];

 Directory();

 text_mode(-1);
 textprintf_centre(temp, font, 320, 2, 0, "Open File...");

 scare_mouse();
 blit(temp, screen, 0, 0, 0, 0, 640, 480);
 unscare_mouse();

 while ((!mouse_b & 1) && (!key[KEY_ESC]))
 {
  mx = mouse_x;
  my = mouse_y;

  if ((file_sel != mx/128 * 37 + (my-20)/12) && (my > 12) && (my < 460)
  && (mx/128 * 37 + (my-20)/12 < f_no))
  {
   text_mode(7);
   scare_mouse();
   textprintf(screen, font, file_sel/37*128 + 10, (file_sel % 37) * 12 + 20, 0, "%s", dir[file_sel].name);
   file_sel = mx/128 * 37 + (my-20)/12;
   text_mode(makecol(180, 180, 255));
   textprintf(screen, font, file_sel/37*128 + 10, (file_sel % 37) * 12 + 20, 0, "%s", dir[file_sel].name);
   unscare_mouse();
  }
 }

 if (key[KEY_ESC]) return;

 /** Load File **/

 rectfill(screen, 192, 90, 448, 140, 7);
 rect(screen, 192, 90, 448, 140, 0);

 text_mode(7);

 scare_mouse();
 textprintf_centre(screen, font, 320, 100, 0, "WARNING: Loading file will");
 textprintf_centre(screen, font, 320, 110, 0, "delete unsaved maps. Continue?");
 textprintf(screen, font, 276, 130, 0, "YES      NO");
 unscare_mouse();

 if (Yes_Or_No() == 0) return;

 strcpy(fn, "./maps/");
 strcat(fn, dir[file_sel].name);

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
       pack_fread(tmpbuf, 4, file);
       strcpy(map_motif[l], tmpbuf);
    }
 }
 else
 {
    for (l = 0; l < 100; l++)
       strcpy(map_motif[l], GetMotifFromInt(pack_igetw(file)));
 }

 pack_fclose(file);

 elev = 0;
 Maps_to_Map(0);

 while(mouse_b & 1);
}

void Save_Maps(void)
{
 int file_sel = 0;
 char fn[20];
 int i, j, k, l, mx, my;
 PACKFILE *file;

 Directory();

 text_mode(makecol(180, 180, 255));
 textprintf_centre(temp, font, 320, 2, 0, "Save Maps...");

 text_mode(7);
 textprintf(temp, font, 10, 470, 0, "Filename:");

 k = 0;

 clear_keybuf();

 scare_mouse();
 blit(temp, screen, 0, 0, 0, 0, 640, 480);
 unscare_mouse();

 while (!(mouse_b & 1) && (!key[KEY_ESC]) && (!key[KEY_ENTER]))
 {
  mx = mouse_x;
  my = mouse_y;

  if ((file_sel != mx/128 * 37 + (my-20)/12) && (my > 12) && (my < 460)
  && (mx/128 * 37 + (my-20)/12 < f_no))
  {
   text_mode(7);
   scare_mouse();
   textprintf(screen, font, file_sel/37*128 + 10, (file_sel % 37) * 12 + 20, 0, "%s", dir[file_sel].name);
   file_sel = mx/128 * 37 + (my-20)/12;
 text_mode(makecol(180, 180, 255));
   textprintf(screen, font, file_sel/37*128 + 10, (file_sel % 37) * 12 + 20, 0, "%s", dir[file_sel].name);
   unscare_mouse();
  }
  if (keypressed())
  {
   j = readkey();
   if (j >> 8 == KEY_BACKSPACE)
   {
    if (k > 0)
    {
     k--;
     text_mode(7);
     textprintf(screen, font, 85 + k*8, 470, 0, "  ");
    }
   }
   else
   {
    if (k == 8) k = 7;
    text_mode(7);
    textprintf(screen, font, 85 + k*8, 470, 0, " ");
    textprintf(screen, font, 85 + k*8, 470, 0, "%c", j & 0xff);
    f[k] = j & 0xff;
    k++;
   }
  }
 }

 if ((key[KEY_ENTER]) && (k > 0))
 {
  f[k - 1] = 46;
  f[k] = 109;
  f[k + 1] = 97;
  f[k + 2] = 112;
  f[k + 3] = NULL;
 }

 if (key[KEY_ESC])
 {
  scare_mouse();
  blit(temp, screen, 0, 0, 0, 0, 640, 480);
  unscare_mouse();
  return;
 }

 /** Save File **/

 if (mouse_b & 1) strcpy(f, dir[file_sel].name);

 strcpy(fn, "./maps/");
 strcat(fn, f);

 if (exists(fn))
 {
  rectfill(screen, 220, 90, 420, 140, 7);
  rect(screen, 220, 90, 420, 140, 0);

  text_mode(7);

  scare_mouse();
  textprintf_centre(screen, font, 320, 100, 0, "Are you sure you want");
  textprintf_centre(screen, font, 320, 110, 0, "to overwrite this file?");
  textprintf(screen, font, 276, 130, 0, "YES      NO");
  unscare_mouse();

  if (Yes_Or_No() == 0) return;

  delete_file(fn);
 }

 Map_to_Maps(elev);

 file = pack_fopen(fn, "wp");

 pack_iputw('B', file);
 pack_iputw('L', file);
 pack_iputw('n', file);

 for (l = 0; l < 100; l++)
 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 pack_iputw(maps[i][j][k][l], file);

 for (l = 0; l < 100; l++)
 {
// pack_iputw(map_motif[l], file);
 pack_fwrite(map_motif[l], 4, file);
 }

 pack_fclose(file);

 while(mouse_b & 1);
 while(key[KEY_ENTER]);
}

void Map_to_Maps(int lev)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 maps[i][j][k][lev] = map[i][j][k];
}

void Maps_to_Map(int lev)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 map[i][j][k] = maps[i][j][k][lev];
}

void Copy_To_Buffer(int lev)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 maps[i][j][k][100] = maps[i][j][k][lev];
}

void Copy_From_Buffer(int lev)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 maps[i][j][k][lev] = maps[i][j][k][100];
}

void Editor_Draw_Tiles(void)
{
 blit(editor_icons, temp, icon_start*32, 0, 0, 448, 640, 32);
}

void Editor_Change_Block_Type(int b)
{
 b = b + icon_start;

 if (b == 0) block_type = SOLID;
 if (b == 1) block_type = BOX;
 if (b == 2) block_type = AUDREY;
 if (b == 3) block_type = AN;
 if (b == 4) block_type = TOKEN;

 if (b == 5) block_type = RED_TELEPORT;
 if (b == 6) block_type = BLUE_TELEPORT;
 if (b == 7) block_type = GREEN_TELEPORT;

 if (b == 8) block_type = LASER_UP;
 if (b == 9) block_type = LASER_RIGHT;
 if (b == 10) block_type = LASER_DOWN;
 if (b == 11) block_type = LASER_LEFT;
 if (b == 12) block_type = LASER_4WAY;

 if (b == 13) block_type = MIRROR_MINUS;
 if (b == 14) block_type = MIRROR_PLUS;

 if (b == 15) block_type = BOMB1;
 if (b == 16) block_type = BOMB2;
 if (b == 17) block_type = BOMB3;

 if (b == 18) block_type = MON0;
 if (b == 19) block_type = MON1;
 if (b == 20) block_type = LASER_FILTER;

 if (b == 21) block_type = RED_SWITCH;
 if (b == 22) block_type = GREEN_SWITCH;
 if (b == 23) block_type = BLUE_SWITCH;

 if (b == 24) block_type = RED_DOOR_CLOSE;
 if (b == 25) block_type = GREEN_DOOR_CLOSE;
 if (b == 26) block_type = BLUE_DOOR_CLOSE;

 if (b == 27) block_type = FALL;
}

void Clear(int l)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (i = 1; i < 19; i++)
 for (j = 1; j < 14; j++)
 maps[i][j][k][l] = 0;

 Maps_to_Map(l);
}

void Clear_All(void)
{
 int i;

 for (i = 0; i < 100; i++)
 Clear(i);

 Reset_Motifs();

 Maps_to_Map(elev);
}

void Editor_Key_Put_Tile(int mx, int my)
{
 if (key[KEY_1]) Editor_Put_Block(mx, my, SOLID);
 if (key[KEY_2]) Editor_Put_Block(mx, my, BOX);
 if (key[KEY_3]) Editor_Put_Block(mx, my, TOKEN);
 if (key[KEY_4]) Editor_Put_Block(mx, my, FALL);
 if (key[KEY_5]) Editor_Put_Block(mx, my, BOMB1);
 if (key[KEY_6]) Editor_Put_Block(mx, my, BOMB2);
 if (key[KEY_7]) Editor_Put_Block(mx, my, BOMB3);
 if (key[KEY_8]) Editor_Put_Block(mx, my, AN);
 if (key[KEY_9]) Editor_Put_Block(mx, my, AUDREY);

 if (key[KEY_Q]) Editor_Put_Block(mx, my, RED_SWITCH);
 if (key[KEY_W]) Editor_Put_Block(mx, my, GREEN_SWITCH);
 if (key[KEY_E]) Editor_Put_Block(mx, my, BLUE_SWITCH);
 if (key[KEY_A]) Editor_Put_Block(mx, my, RED_DOOR_CLOSE);
 if (key[KEY_S]) Editor_Put_Block(mx, my, GREEN_DOOR_CLOSE);
 if (key[KEY_D]) Editor_Put_Block(mx, my, BLUE_DOOR_CLOSE);

 if (key[KEY_I]) Editor_Put_Block(mx, my, LASER_UP);
 if (key[KEY_J]) Editor_Put_Block(mx, my, LASER_LEFT);
 if (key[KEY_M]) Editor_Put_Block(mx, my, LASER_DOWN);
 if (key[KEY_L]) Editor_Put_Block(mx, my, LASER_RIGHT);
 if (key[KEY_K]) Editor_Put_Block(mx, my, LASER_4WAY);

 if (key[KEY_F]) Editor_Put_Block(mx, my, LASER_FILTER);

 if (key[KEY_Z]) Editor_Put_Block(mx, my, RED_TELEPORT);
 if (key[KEY_C]) Editor_Put_Block(mx, my, GREEN_TELEPORT);
 if (key[KEY_X]) Editor_Put_Block(mx, my, BLUE_TELEPORT);

 if (key[KEY_SLASH]) Editor_Put_Block(mx, my, MIRROR_PLUS);
 if (key[KEY_BACKSLASH]) Editor_Put_Block(mx, my, MIRROR_MINUS);

 if (key[KEY_BACKSPACE]) Editor_Put_Block(mx, my, 0);
 if (key[KEY_DEL]) Editor_Put_Block(mx, my, 0);
}

void Reset_Motifs(void)
{
 int i;

 for (i = 0; i < 100; i++)
    strcpy(map_motif[i], "SUNY");
 //map_motif[i] = SUNNY;
}
