/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Blow_Box(int b_no)
{
 Add_Explode(box[b_no].x/32, box[b_no].y/32, box[b_no].z/8);
 map[box[b_no].x/32][box[b_no].y/32][box[b_no].z/8] = RESERVE;
 Del_Box(b_no);
}

void Bomb_Prime_Laser(void)
{
 int i, j;

 for (i = 0; i < box_count; i++)
 {
  if (box[i].type & BOMB)
  {
   if ((!box[i].dx) && (!box[i].dy) && (!box[i].dz) &&
       (map_laser[box[i].x/32][box[i].y/32][box[i].z/8]))
   {
    if (box[i].type == BOMB3)
    {
     for (j = 0; j < box_count; j++)
     if (box[j].type == BOMB3)
     {
      Blow_Box(j);
      j--;
     }
    }
    else
    {
     Blow_Box(i);
     i--;
    }
   }
  }
 }
}

void Del_Box(int b_no)
{
 int i;

 for (i = b_no; i < box_count; i++)
 {
  box[i].x = box[i + 1].x;
  box[i].y = box[i + 1].y;
  box[i].z = box[i + 1].z;
  box[i].dx = box[i + 1].dx;
  box[i].dy = box[i + 1].dy;
  box[i].dz = box[i + 1].dz;
  box[i].type = box[i + 1].type;
  box[i].dir = box[i + 1].dir;
  box[i].pic = box[i + 1].pic;
 }
 box_count--;
}

void Add_Explode(int x, int y, int z)
{
 explode[explode_count].x = x;
 explode[explode_count].y = y;
 explode[explode_count].z = z;
 explode[explode_count].frame = 0;
 explode[explode_count].type = map[x][y][z] & BOMB;

 explode_count++;

 Add_Sound(x, SFX_EXPLODE);
}

void Del_Explode(int e_no)
{
 int i;

 for (i = e_no; i < explode_count; i++)
 {
  explode[i].x = explode[i + 1].x;
  explode[i].y = explode[i + 1].y;
  explode[i].z = explode[i + 1].z;
  explode[i].frame = explode[i + 1].frame;
  explode[i].type = explode[i + 1].type;
 }

 explode_count--;
}

void Draw_Explode(void)
{
 int i;

 for (i = 0; i < explode_count; i++)
 {
  explode[i].frame++;

  if (explode[i].frame/10 == 0) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[0]);
  if (explode[i].frame/10 == 1) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[1]);
  if (explode[i].frame/10 == 2) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[2]);
  if (explode[i].frame/10 == 3) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[3]);
  if (explode[i].frame/10 == 4) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[4]);
  if (explode[i].frame/10 == 5) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[5]);
  if (explode[i].frame/10 == 6) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[6]);
  if (explode[i].frame/10 == 7) DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, explode_pic[7]);

  if (explode[i].frame/10 == 8)
  {
   DirtyList(explode[i].x * 32, explode[i].y * 32, explode[i].z * 8, 32, 40, blank);

   if (explode[i].type == BOMB) Explode_Prime_Chain(explode[i].x, explode[i].y);

   map[explode[i].x][explode[i].y][explode[i].z] = 0;
   Del_Explode(i);
   i--;
  }
 }
}

void Explode_Prime_Chain(int x, int y)
{
 if (map[x][y][0] & BLOWUP)     Explode_Prime(x, y, 0);
 if (map[x][y][1] & BLOWUP)     Explode_Prime(x, y, 1);

 if (map[x][y - 1][0] & BLOWUP)     Explode_Prime(x, y - 1, 0);
 if (map[x + 1][y - 1][0] & BLOWUP) Explode_Prime(x + 1, y - 1, 0);
 if (map[x + 1][y][0] & BLOWUP)     Explode_Prime(x + 1, y, 0);
 if (map[x + 1][y + 1][0] & BLOWUP) Explode_Prime(x + 1, y + 1, 0);
 if (map[x][y + 1][0] & BLOWUP)     Explode_Prime(x, y + 1, 0);
 if (map[x - 1][y + 1][0] & BLOWUP) Explode_Prime(x - 1, y + 1, 0);
 if (map[x - 1][y][0] & BLOWUP)     Explode_Prime(x - 1, y, 0);
 if (map[x - 1][y - 1][0] & BLOWUP) Explode_Prime(x - 1, y - 1, 0);

 if (map[x][y - 1][1] & BLOWUP)     Explode_Prime(x, y - 1, 1);
 if (map[x + 1][y - 1][1] & BLOWUP) Explode_Prime(x + 1, y - 1, 1);
 if (map[x + 1][y][1] & BLOWUP)     Explode_Prime(x + 1, y, 1);
 if (map[x + 1][y + 1][1] & BLOWUP) Explode_Prime(x + 1, y + 1, 1);
 if (map[x][y + 1][1] & BLOWUP)     Explode_Prime(x, y + 1, 1);
 if (map[x - 1][y + 1][1] & BLOWUP) Explode_Prime(x - 1, y + 1, 1);
 if (map[x - 1][y][1] & BLOWUP)     Explode_Prime(x - 1, y, 1);
 if (map[x - 1][y - 1][1] & BLOWUP) Explode_Prime(x - 1, y - 1, 1);

}

void Explode_Prime(int x, int y, int z)
{
 int i, j;

 i = 0;

 while ((box[i].x/32 != x) || (box[i].y/32 != y)  || (box[i].z/8 != z))
 i++;

 if (box[i].type == BOMB3)
 {
  for (j = 0; j < box_count; j++)
  if (box[j].type == BOMB3)
  {
   Blow_Box(j);
   j--;
  }
 }
 else Blow_Box(i);
}
