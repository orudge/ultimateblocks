#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Draw_Block(int x, int y, int z, BITMAP *pic);
void Draw_Status(void);

//#define DO_GRAPHICS_LOG

#ifdef DO_GRAPHICS_LOG
   #include <stdio.h>
#endif

void DirtyList(int x,int y, int z, int w, int h, BITMAP *pic)
{
 dirty[dirty_count].x = x;
 dirty[dirty_count].y = y;
 dirty[dirty_count].z = z;
 dirty[dirty_count].w = w;
 dirty[dirty_count].h = h;
 dirty[dirty_count].pic = pic;

 dirty_count++;
}

/*void Draw_Screen(void)
{
 int i, j, k;

 for (j = 0; j < 15; j++)
 {
  for (k = 0; k < dirty_count; k++)
  {
   if ((dirty[k].y > j*32 - 1) && (dirty[k].y < j*32+32))
   {
    Draw_Block(dirty[k].x/32, j, 0, temp);
    Draw_Block(dirty[k].x/32, j - 1, 0, temp);
    Draw_Block(dirty[k].x/32, j - 1, 1, temp);

    if (dirty[k].x%32)
    {
     Draw_Block(dirty[k].x/32 + 1, j, 0, temp);
     Draw_Block(dirty[k].x/32 + 1, j - 1, 0, temp);
     Draw_Block(dirty[k].x/32 + 1, j - 1, 1, temp);
    }
   }
  }

  for (k = 0; k < dirty_count; k++)
  {
   if ((dirty[k].y > j*32 - 1) && (dirty[k].y < j*32+32))
   {
    masked_blit(dirty[k].pic, temp, 0, 0, dirty[k].x, dirty[k].y - dirty[k].z, dirty[k].w, dirty[k].h);
   }
  }

  for (k = 0; k < dirty_count; k++)
  {
   if ((dirty[k].y > j*32 - 1) && (dirty[k].y < j*32+32))
   {
    Draw_Block(dirty[k].x/32, j, 1, temp);
    Draw_Block(dirty[k].x/32, j + 1, 0, temp);
    Draw_Block(dirty[k].x/32, j + 1, 1, temp);
    Draw_Block(dirty[k].x/32, j + 2, 0, temp);
    Draw_Block(dirty[k].x/32, j + 2, 1, temp);

    if (dirty[k].x%32)
    {
     Draw_Block(dirty[k].x/32 + 1, j, 1, temp);
     Draw_Block(dirty[k].x/32 + 1, j + 1, 0, temp);
     Draw_Block(dirty[k].x/32 + 1, j + 1, 1, temp);
     Draw_Block(dirty[k].x/32 + 1, j + 2, 0, temp);
     Draw_Block(dirty[k].x/32 + 1, j + 2, 1, temp);
    }
   }
  }
 }

 for (i = 0; i < dirty_count; i++)
 blit(temp, screen, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].w + 2, dirty[i].h + 2);

 for (i = 0; i < dirty_count; i++)
 blit(back, temp, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].w + 2, dirty[i].h + 2);

 dirty_count = 0;
} */

void Draw_Map(void)
{
 int i, j, k, l;
#ifdef DO_GRAPHICS_LOG
 FILE *tmp;
#endif

#ifdef DO_GRAPHICS_LOG
 tmp = fopen("tmp.log", "at");
 fprintf(tmp, "\n\nDraw_Map\n-------\n");
 fclose(tmp);
#endif

 for (j = 0; j < 15; j+=2)
 {
    for (i = 0; i < 20; i+=2)
    {
       blit(gfx, back, 0, 200, i*32, j*32, 64, 64);
#ifdef DO_GRAPHICS_LOG
       tmp = fopen("tmp.log", "at");
       fprintf(tmp, "Drawn background - [i=%d;j=%d] blit(gfx, back, 0, 200, %d, %d, 64, 64);\n", i,j,i*32, j*32);
       fclose(tmp);
#endif
    }
 }

 blit(back, temp, 0, 0, 0, 0, 640, 480);

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 {
  if ((k == 0) || ((i != 0) && (i != 19) && (j != 0) && (j != 14))) Draw_Block(i, j, k, temp);
 }

 rect(temp, 16, 16, 623, 463, 7);
}

void Draw_Area(void)
{
 int i, j, k;

 for (i = 0; i < 20; i++)
 for (j = 0; j < 15; j++)
 map2[i][j] = 0;

 for (i = 0; i < dirty_count; i++)
 {
  map2[dirty[i].x/32][dirty[i].y/32] = 1;
  map2[(dirty[i].x + 31)/32][dirty[i].y/32] = 1;

  map2[dirty[i].x/32][(dirty[i].y + 31)/32] = 1;
  map2[(dirty[i].x + 31)/32][(dirty[i].y + 31)/32] = 1;

  map2[dirty[i].x/32 - 1][dirty[i].y/32] = 1;
  map2[dirty[i].x/32][dirty[i].y/32 - 1] = 1;
  map2[dirty[i].x/32 - 1][dirty[i].y/32 - 1] = 1;

  map2[(dirty[i].x + 31)/32 + 1][dirty[i].y/32] = 1;
  map2[(dirty[i].x + 31)/32][dirty[i].y/32 - 1] = 1;
  map2[(dirty[i].x + 31)/32 + 1][dirty[i].y/32 - 1] = 1;

  map2[dirty[i].x/32 - 1][(dirty[i].y + 31)/32] = 1;
  map2[dirty[i].x/32][(dirty[i].y + 31)/32 + 1] = 1;
  map2[dirty[i].x/32 - 1][(dirty[i].y + 31)/32 + 1] = 1;

  map2[(dirty[i].x + 31)/32 + 1][(dirty[i].y + 31)/32] = 1;
  map2[(dirty[i].x + 31)/32][(dirty[i].y + 31)/32 + 1] = 1;
  map2[(dirty[i].x + 31)/32 + 1][(dirty[i].y + 31)/32 + 1] = 1;
 }
}

void Sort_Dirty(void)
{
 int i, j, k;

 j = 0;

 for (k = 0; k < dirty_count; k++)
 {
  i = 0;

  while (i < dirty_count)
  {
   if ((i != j) && (dirty[i].y < dirty[j].y))
   {
    j = i;
   }
   i++;
  }

  dirty_sort[k].x = dirty[j].x;
  dirty_sort[k].y = dirty[j].y;
  dirty_sort[k].z = dirty[j].z;
  dirty_sort[k].w = dirty[j].w;
  dirty_sort[k].h = dirty[j].h;
  dirty_sort[k].pic = dirty[j].pic;

  dirty[j].y = 500;

  j = 0;

  while (dirty[j].y != 500)
  j++;
 }

 for (i = 0; i < dirty_count; i++)
 {
  dirty[i].x = dirty_sort[i].x;
  dirty[i].y = dirty_sort[i].y;
  dirty[i].z = dirty_sort[i].z;
  dirty[i].w = dirty_sort[i].w;
  dirty[i].h = dirty_sort[i].h;
  dirty[i].pic = dirty_sort[i].pic;
 }
}

void Draw_Block(int x, int y, int z, BITMAP *pic)
{
 int k;

 if ((x < 0) || (y < 0) || (x > 19) || (y > 14)) return;

 if ((map[x][y][z] == 0) && (!z))
 {
  if (map[x][y - 1][z] == 0) blit(back, pic, x*32, y*32 - z*8, x*32, y*32 - z*8, 32, 40);
  else blit(back, pic, x*32, y*32 - z*8 + 8, x*32, y*32 - z*8 + 8, 32, 32);
 }
 if (map[x][y][z] == SOLID)
 {
  k = 0;

  if (map[x][y - 1][z] != SOLID) k = 1;
  if (map[x + 1][y][z] != SOLID) k += 2;
  if (map[x][y + 1][z] != SOLID) k += 4;
  if (map[x - 1][y][z] != SOLID) k += 8;

  if (x == 0) k = k & 7;
  if (x == 19) k = k & 13;
  if (y == 0) k = k & 14;
  if (y == 14) k = k & 11;

  if (z == 1)
  {
   if (x == 0) k = k | 8;
   if (x == 19) k = k | 2;
   if (y == 0) k = k | 1;
   if (y == 14) k = k | 4;
  }

  masked_blit(gfx, pic, (k%4)*32 + z*128, 280 + (k/4)*40, x*32, y*32 - z*8, 32, 40);
 }

 if (map[x][y][z] == BOX) masked_blit(box_pic, pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == AUDREY) masked_blit(ply_pic[0][0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == AN) masked_blit(ply_pic[1][0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == TOKEN) masked_blit(token_pic, pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == RED_TELEPORT) masked_blit(teleport_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == GREEN_TELEPORT) masked_blit(teleport_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == BLUE_TELEPORT) masked_blit(teleport_pic[2], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == LASER_UP) masked_blit(laser_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == LASER_RIGHT) masked_blit(laser_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == LASER_DOWN) masked_blit(laser_pic[2], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == LASER_LEFT) masked_blit(laser_pic[3], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == LASER_4WAY) masked_blit(laser_pic[4], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == MIRROR_MINUS) masked_blit(mirror_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == MIRROR_PLUS) masked_blit(mirror_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == BOMB1) masked_blit(bomb_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == BOMB2) masked_blit(bomb_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == BOMB3) masked_blit(bomb_pic[2], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == LASER_FILTER) masked_blit(filter_pic, pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == MON0) masked_blit(mon_pic[0][0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == MON1) masked_blit(mon_pic[0][0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == MON2) masked_blit(mon_pic[0][0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == RED_SWITCH) masked_blit(switches_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == GREEN_SWITCH) masked_blit(switches_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == BLUE_SWITCH) masked_blit(switches_pic[2], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == RED_DOOR_CLOSE) masked_blit(door_pic[0], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == GREEN_DOOR_CLOSE) masked_blit(door_pic[1], pic, 0, 0, x*32, y*32 - z*8, 32, 40);
 if (map[x][y][z] == BLUE_DOOR_CLOSE) masked_blit(door_pic[2], pic, 0, 0, x*32, y*32 - z*8, 32, 40);

 if (map[x][y][z] == FALL) masked_blit(fall_pic, pic, 0, 0, x*32, y*32 - z*8, 32, 40);
}

void Draw_Screen(void)
{
 int i, j, k;

 Sort_Dirty();
 Draw_Area();

 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 if (map2[i][j]) blit(back, temp, i * 32, j * 32, i * 32, j * 32, 32, 40);

 for (j = 0; j < 15; j++)
 {
  for (i = 0; i < 20; i++)
  if ((map2[i][j]) && (map[i][j][0])) Draw_Block(i, j, 0, temp);

  for (k = 0; k < dirty_count; k++)
  {
   if ((dirty[k].y/32 == j) && (dirty[k].z == 0))
   masked_blit(dirty[k].pic, temp, 0, 0, dirty[k].x, dirty[k].y - dirty[k].z, dirty[k].w, dirty[k].h);

   if ((dirty[k].y/32 == j - 1) && (dirty[k].z != 0))
   masked_blit(dirty[k].pic, temp, 0, 0, dirty[k].x, dirty[k].y - dirty[k].z, dirty[k].w, dirty[k].h);
  }

  for (i = 0; i < 20; i++)
  if (map2[i][j])
  {
   if (map_laser[i][j][0] == 1) masked_blit(beam_pic[0], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] == 2) masked_blit(beam_pic[1], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] == 3) masked_blit(beam_pic[2], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] & 4) masked_blit(beam_pic[3], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] & 8) masked_blit(beam_pic[4], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] & 16) masked_blit(beam_pic[5], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
   if (map_laser[i][j][0] & 32) masked_blit(beam_pic[6], temp, 0, 0, i*32, j*32 - 0*8, 32, 35);
  }

  for (i = 0; i < 20; i++)
  if ((map2[i][j]) && (map[i][j][1])) Draw_Block(i, j, 1, temp);

  for (i = 0; i < 20; i++)
  if (map2[i][j])
  {
   if (map_laser[i][j][1] == 1) masked_blit(beam_pic[0], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] == 2) masked_blit(beam_pic[1], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] == 3) masked_blit(beam_pic[2], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] & 4) masked_blit(beam_pic[3], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] & 8) masked_blit(beam_pic[4], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] & 16) masked_blit(beam_pic[5], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
   if (map_laser[i][j][1] & 32) masked_blit(beam_pic[6], temp, 0, 0, i*32, j*32 - 1*8, 32, 35);
  }
 }

 for (i = 0; i < dirty_count; i++)
 blit(temp, screen, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].x - 1, dirty[i].y - dirty[i].z - 1, dirty[i].w + 2, dirty[i].h + 2);

 dirty_count = 0;

 Draw_Status();
}
void Map_Setup(void)
{
 int i, j, k, l;

 box_count = 0;
 mon_count = 0;
 explode_count = 0;
 door_count = 0;
 switches_count = 0;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 {
  map_door[i][j][k] = 0;
  map_laser[i][j][k] = 0;

  if (map[i][j][k] & PUSH)
  {
   box[box_count].x = i*32;
   box[box_count].y = j*32;
   box[box_count].z = k*8;
   box[box_count].dx = 0;
   box[box_count].dy = 0;
   box[box_count].dz = 0;
   box[box_count].type = map[i][j][k];

   if (map[i][j][k] == BOX) box[box_count].pic = box_pic;
   if (map[i][j][k] == MIRROR_MINUS) box[box_count].pic = mirror_pic[0];
   if (map[i][j][k] == MIRROR_PLUS) box[box_count].pic = mirror_pic[1];
   if (map[i][j][k] == BOMB1) box[box_count].pic = bomb_pic[0];
   if (map[i][j][k] == BOMB2) box[box_count].pic = bomb_pic[1];
   if (map[i][j][k] == BOMB3) box[box_count].pic = bomb_pic[2];
   if (map[i][j][k] == LASER_UP) box[box_count].pic = laser_pic[0];
   if (map[i][j][k] == LASER_RIGHT) box[box_count].pic = laser_pic[1];
   if (map[i][j][k] == LASER_DOWN) box[box_count].pic = laser_pic[2];
   if (map[i][j][k] == LASER_LEFT) box[box_count].pic = laser_pic[3];
   if (map[i][j][k] == LASER_4WAY) box[box_count].pic = laser_pic[4];

   if (map[i][j][k] & LASER) box[box_count].dir = map[i][j][k] & 15;

   box_count++;
  }

  if (map[i][j][k] & MONSTER)
  {
   mon[mon_count].x = i*32;
   mon[mon_count].y = j*32;
   mon[mon_count].z = k*8;
   mon[mon_count].dx = -1;
   mon[mon_count].dy = 0;
   mon[mon_count].dz = 0;

   mon[mon_count].type = map[i][j][k];

   mon[mon_count].dir = 8;
   mon[mon_count].state = 1;
   mon[mon_count].speed = 3;
   mon[mon_count].count = 0;

   if (map[i][j][k] == MON2) mon[mon_count].speed = 1;

   Monster_Change_Dir(mon_count, i, j, k);

   mon_count++;

   map[i][j][k] = 0;
  }

  if (map[i][j][k] & DOOR)
  {
   door[door_count].x = i;
   door[door_count].y = j;
   door[door_count].z = k;
   door[door_count].c = map[i][j][k] & 7;
   door[door_count].state = CLOSE;
   door_count++;
   map_door[i][j][k] = map[i][j][k] & 7;
  }

  if (map[i][j][k] & SWITCH)
  {
   switches[switches_count].x = i;
   switches[switches_count].y = j;
   switches[switches_count].z = k;
   switches[switches_count].c = map[i][j][k] & 7;
   switches_count++;
  }
 }
}

void Draw_Status(void)
{
}
