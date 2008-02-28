/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

int x, y, z, dx, dy;

void Laser(void)
{
 int i, j, k;
 int map_laser_back[20][15][2];

 for (i = 0; i < 20; i++)
 for (j = 0; j < 15; j++)
 {
  map_laser_back[i][j][0] = map_laser[i][j][0];
  map_laser_back[i][j][1] = map_laser[i][j][1];

  map_laser[i][j][0] = 0;
  map_laser[i][j][1] = 0;
 }

 for (i = 0; i < box_count; i++)
 {
  if (box[i].type & LASER)
  {
   if (box[i].type == LASER_4WAY)
   {
    Start_XYZ(box[i].x, box[i].y, box[i].z, box[i].dx, box[i].dy, 1);
    Track_Laser();
    Start_XYZ(box[i].x, box[i].y, box[i].z, box[i].dx, box[i].dy, 2);
    Track_Laser();
    Start_XYZ(box[i].x, box[i].y, box[i].z, box[i].dx, box[i].dy, 4);
    Track_Laser();
    Start_XYZ(box[i].x, box[i].y, box[i].z, box[i].dx, box[i].dy, 8);
    Track_Laser();
   }
   else
   {
    Start_XYZ(box[i].x, box[i].y, box[i].z, box[i].dx, box[i].dy, box[i].dir);
    Track_Laser();
   }
  }
 }


 for (i = 0; i < 20; i++)
 for (j = 0; j < 15; j++)
 for (k = 0; k < 2; k++)
 if (map_laser[i][j][k] != map_laser_back[i][j][k]) DirtyList(i*32, j*32, k*8, 32, 40, blank);

}

void Start_XYZ(int b_x, int b_y, int b_z, int b_dx, int b_dy, int b_dir)
{
 dx = 0;
 dy = 0;

 if (b_dir == 1) dy = -1;
 if (b_dir == 2) dx = 1;
 if (b_dir == 4) dy = 1;
 if (b_dir == 8) dx = -1;

 if ((!b_dx) && (!b_dy))
 {
  x = (b_x)/32 + dx;
  y = (b_y)/32 + dy;
 }

 if ((b_dx) && (dy))
 {
  x = (b_x + 16)/32;
  y = b_y/32 + dy;
 }

 if ((b_dy) && (dx))
 {
  x = b_x/32 + dx;
  y = (b_y + 16)/32;
 }

 if ((b_dx) && (dx))
 {
  x = (b_x)/32 + dx;
  y = b_y/32;
 }

 if ((b_dy) && (dy))
 {
  x = b_x/32;
  y = (b_y)/32 + dy;
 }


 if ((b_dx ==  1) && (dx)) x++;
 if ((b_dy ==  1) && (dy)) y++;

 z = b_z/8;
}

void Track_Laser(void)
{
 int j;

   while ((x > 0) && (x < 20) && (y > 0) && (y < 15) &&
         ((map[x][y][z] == 0) || (map[x][y][z] & MIRROR)
           || (map[x][y][z] & BOMB) || (map[x][y][z] & PICKUP)
           || (map[x][y][z] & RESERVE0)
           || (map[x][y][z] == FALL)
           || (map[x][y][z] & SWITCH)))
   {
    if ((map[x][y][z] == 0)
    || (map[x][y][z] & BOMB)
    || (map[x][y][z] & PICKUP)
    || (map[x][y][z] & RESERVE0)
    || (map[x][y][z] & SWITCH)
    || (map[x][y][z] == FALL))
    {
     if (dx) map_laser[x][y][z] = map_laser[x][y][z] | 2;
     else map_laser[x][y][z] = map_laser[x][y][z] | 1;
    }

    if ((map[x][y][z] & MIRROR_PLUS) == MIRROR_PLUS)
    {
     if ((dx == 1) || (dy == 1)) map_laser[x][y][z] = map_laser[x][y][z] | 32;
     else map_laser[x][y][z] = map_laser[x][y][z] | 8;

     j = dx;
     dx = -dy;
     dy = -j;
    }

    if ((map[x][y][z] & MIRROR_MINUS) == MIRROR_MINUS)
    {
     if ((dx == 1) || (dy == -1)) map_laser[x][y][z] = map_laser[x][y][z] | 16;
     else map_laser[x][y][z] = map_laser[x][y][z] | 4;

     j = dx;
     dx = dy;
     dy = j;
    }

    x += dx;
    y += dy;
   }
}
