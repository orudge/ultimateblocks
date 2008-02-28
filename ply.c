/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Player2_Input(int p_no)
{
  if (key[KEY_W]) ply[p_no].dir = 1;
  if (key[KEY_S]) ply[p_no].dir = 4;
  if (key[KEY_A]) ply[p_no].dir = 8;
  if (key[KEY_D]) ply[p_no].dir = 2;

  if ((key[KEY_W]) && (ply[p_no].dy != -1) && (Player_Collision(p_no, 0, -1)))
  {
   ply[p_no].dy = -1;
   return;
  }
  if ((key[KEY_S]) && (ply[p_no].dy != 1)  && (Player_Collision(p_no, 0, 1)))
  {
   ply[p_no].dy = 1;
   return;
  }
  if ((key[KEY_A]) && (ply[p_no].dx != -1)  && (Player_Collision(p_no, -1, 0)))
  {
   ply[p_no].dx = -1;
   return;
  }
  if ((key[KEY_D]) && (ply[p_no].dx != 1)  && (Player_Collision(p_no, 1, 0)))
  {
   ply[p_no].dx = 1;
   return;
  }
}

void Player_Input(int p_no)
{
 if (ply[p_no].dz) return;

 if (key[KEY_UP]) ply[p_no].dir = 1;
 if (key[KEY_DOWN]) ply[p_no].dir = 4;
 if (key[KEY_LEFT]) ply[p_no].dir = 8;
 if (key[KEY_RIGHT]) ply[p_no].dir = 2;

 if ((key[KEY_UP]) && (ply[p_no].dy != -1) && (Player_Collision(p_no, 0, -1)))
 {
  ply[p_no].dy = -1;
  return;
 }
 if ((key[KEY_DOWN]) && (ply[p_no].dy != 1)  && (Player_Collision(p_no, 0, 1)))
 {
  ply[p_no].dy = 1;
  return;
 }
 if ((key[KEY_LEFT]) && (ply[p_no].dx != -1)  && (Player_Collision(p_no, -1, 0)))
 {
  ply[p_no].dx = -1;
  return;
 }
 if ((key[KEY_RIGHT]) && (ply[p_no].dx != 1)  && (Player_Collision(p_no, 1, 0)))
 {
  ply[p_no].dx = 1;
  return;
 }
}

void Player_Draw(int p_no)
{
 if ((ply[p_no].dx) || (ply[p_no].dy)) ply[p_no].frame++;
 if (Player_Frame(ply[p_no].frame) == -1) ply[p_no].frame = 0;

 if (ply[p_no].dir == 4) DirtyList(ply[p_no].x, ply[p_no].y, ply[p_no].z, _block_width, _block_height_depth, ply_pic[p_no][0 + Player_Frame(ply[p_no].frame)]);
 if (ply[p_no].dir == 1) DirtyList(ply[p_no].x, ply[p_no].y, ply[p_no].z, _block_width, _block_height_depth, ply_pic[p_no][3 + Player_Frame(ply[p_no].frame)]);
 if (ply[p_no].dir == 2) DirtyList(ply[p_no].x, ply[p_no].y, ply[p_no].z, _block_width, _block_height_depth, ply_pic[p_no][6 + Player_Frame(ply[p_no].frame)]);
 if (ply[p_no].dir == 8) DirtyList(ply[p_no].x, ply[p_no].y, ply[p_no].z, _block_width, _block_height_depth, ply_pic[p_no][9 + Player_Frame(ply[p_no].frame)]);

 if ((ply[p_no].frame%40 == 0) && ((ply[p_no].dx) || (ply[p_no].dy)))
   Add_Sound(ply[p_no].x/_block_width, SFX_CLICK);

}

int Player_Frame(int f)
{
 if (f/20 == 0) return 0;
 if (f/20 == 1) return 1;
 if (f/20 == 2) return 0;
 if (f/20 == 3) return 2;

 return -1;
}

void Player_Fall(int p_no)
{
 if (Check_Can_Fall(ply[p_no].x, ply[p_no].y, ply[p_no].z)) ply[p_no].dz = -1;
}

void Player_Move(int p_no)
{
 if (Check_Can_Fall(ply[p_no].x, ply[p_no].y, ply[p_no].z))
 ply[p_no].z +=ply[p_no].dz;

/* if (Check_Map_And(ply[p_no].x, ply[p_no].y + ply[p_no].dy, ply[p_no].z, 0))*/

 ply[p_no].x += ply[p_no].dx;
 ply[p_no].y += ply[p_no].dy;

 if (!Check_Can_Fall(ply[p_no].x, ply[p_no].y, ply[p_no].z)) ply[p_no].dz = 0;

 if ((ply[p_no].dx) || (ply[p_no].dy))
 {
  if (ply[p_no].x%_block_width == 0) ply[p_no].dx = 0;
  if (ply[p_no].y%_block_height == 0) ply[p_no].dy = 0;
  if ((!ply[p_no].dx) && (!ply[p_no].dy)) Player_Teleport(p_no);
 }
}

int Check_Can_Fall(int x, int y, int z)
{
 if (!z) return 0;

 if (z > 0)
 {
  if (map[x/_block_width][y/_block_height][0] & FULL) return 0;
  if (map[(x + (_block_width-1))/_block_width][y/_block_height][0] & FULL) return 0;
  if (map[x/_block_width][(y + (_block_height-1))/_block_height][0] & FULL) return 0;
  if (map[(x + (_block_width-1))/_block_width][(y + (_block_height-1))/_block_height][0] & FULL) return 0;

  return 1;
 }
}

void Player_Teleport(int p_no)
{
 int c, i, j;

 if (!(map[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] & TELEPORT)) return;

 c = map[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth];

 i = ply[p_no].x/_block_width;
 j = ply[p_no].y/_block_height;

 i++;
 if (i == 20)
 {
  i = 0;
  j++;
  if (j == 15)
  {
   i = 0;
   j = 0;
  }
 }

 while ((j < 15) && (map[i][j][0] != c) && (map[i][j][1] != c))
 {
  i++;
  if (i == 20)
  {
   i = 0;
   j++;
  }
 }

 if (j == 15)
 {
  i = 0;
  j = 0;

  while ((j < ply[p_no].y/_block_height + 1) && (map[i][j][0] != c) && (map[i][j][1] != c))
  {
   i++;
   if (i == 20)
   {
    i = 0;
    j++;
   }
  }

  if (j == ply[p_no].y/_block_height + 1) exit(12);
 }

 DirtyList(ply[p_no].x, ply[p_no].y, ply[p_no].z, _block_width, _block_height_depth, blank);

 ply[p_no].x = i*_block_width;
 ply[p_no].y = j*_block_height;

 if (map[i][j][0] == c) ply[p_no].z = 0;
 else ply[p_no].z = _block_depth;

}

int Check_Map(int x, int y, int z, int b, int or_and, int equals_and)
{
 if (or_and == 1)   /* AND */
 {
  if (equals_and == 1)  /* && */
  if ((map[x/_block_width][y/_block_height][z/_block_depth] & b) &&
      (map[(x + (_block_width-1))/_block_width][y/_block_height][z/_block_depth] & b) &&
      (map[x/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] & b) &&
      (map[(x + (_block_width-1))/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] & b)) return 1;
  else          /* EQUALS */
  if ((map[x/_block_width][y/_block_height][z/_block_depth] == b) &&
      (map[(x + (_block_width-1))/_block_width][y/_block_height][z/_block_depth] == b) &&
      (map[x/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] == b) &&
      (map[(x + (_block_width-1))/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] == b)) return 1;

  return 0;
 }
 else   /* OR */
 {
  if (equals_and == 1)  /* && */
  if ((map[x/_block_width][y/_block_height][z/_block_depth] & b) ||
      (map[(x + (_block_width-1))/_block_width][y/_block_height][z/_block_depth] & b) ||
      (map[x/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] & b) ||
      (map[(x + (_block_width-1))/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] & b)) return 1;
  else          /* EQUALS */
  if ((map[x/_block_width][y/_block_height][z/_block_depth] == b) ||
      (map[(x + (_block_width-1))/_block_width][y/_block_height][z/_block_depth] == b) ||
      (map[x/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] == b) ||
      (map[(x + (_block_width-1))/_block_width][(y + (_block_height-1))/_block_height][z/_block_depth] == b)) return 1;

  return 0;
 }
}

void Player_Pickup_Check(int p_no)
{
 if (map[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] & PICKUP) Player_Pickup(p_no, ply[p_no].x/_block_width, ply[p_no].y/_block_height);
 if (map[(ply[p_no].x + (_block_width-1))/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] & PICKUP) Player_Pickup(p_no, (ply[p_no].x + (_block_width-1))/_block_width, ply[p_no].y/_block_height);
 if (map[ply[p_no].x/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth] & PICKUP) Player_Pickup(p_no, ply[p_no].x/_block_width, (ply[p_no].y + (_block_height-1))/_block_height);
 if (map[(ply[p_no].x + (_block_width-1))/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth] & PICKUP) Player_Pickup(p_no, (ply[p_no].x + (_block_width-1))/_block_width, (ply[p_no].y + (_block_height-1))/_block_height);
}

void Player_Pickup(int p_no, int x, int y)
{
 map[x][y][ply[p_no].z/_block_depth] = 0;
 DirtyList(x*_block_width, y*_block_height, ply[p_no].z/_block_depth, _block_width, _block_height_depth, blank);

 Add_Sound(x, SFX_YEAH);
}

int Player_Collision(int p_no, int dx, int dy)
{
 if ((ply[p_no].z != 0) && (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, 0, MIRROR, 0, 1))) return 0;

 if (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, 0, 1, 1)) return 1;
 if (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, SOLID, 0, 0)) return 0;
 if (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, TELEPORT, 0, 1)) return 1;

 if ((ply[p_no].x%_block_width == 0) && (ply[p_no].y%_block_height == 0))
 if ((dx == 0) || (dy == 0))
 if (map[ply[p_no].x/_block_width + dx][ply[p_no].y/_block_height + dy][ply[p_no].z/_block_depth] & PUSH)
 if (!(map[ply[p_no].x/_block_width + dx*2][ply[p_no].y/_block_height + dy*2][0] & PICKUP))
 if (!Check_Can_Fall(ply[p_no].x, ply[p_no].y, ply[p_no].z))
 {
  if ((map[ply[p_no].x/_block_width + dx*2][ply[p_no].y/_block_height + dy*2][ply[p_no].z/_block_depth] == 0) ||
      (map[ply[p_no].x/_block_width + dx*2][ply[p_no].y/_block_height + dy*2][ply[p_no].z/_block_depth] & SWITCH))
  {
   Add_Undo();

   map[(ply[p_no].x/_block_width + dx)][(ply[p_no].y/_block_height + dy)][ply[p_no].z/_block_depth] = 0;
   Box_Push((ply[p_no].x/_block_width + dx), (ply[p_no].y/_block_height + dy), ply[p_no].z, dx, dy);
   return 1;
  }
  if (map[ply[p_no].x/_block_width + dx][ply[p_no].y/_block_height + dy][ply[p_no].z/_block_depth] == BOMB1)
  {
   Add_Undo();

   Explode_Prime(ply[p_no].x/_block_width + dx, ply[p_no].y/_block_height + dy, ply[p_no].z/_block_depth);
  }
 }

 if (((Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, PICKUP, 0, 1)) ||
     (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, SWITCH, 0, 1)) ||
     (Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, FALL, 0, 1))) &&
     (!(Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, PUSH, 0, 1))) &&
     (!(Check_Map(ply[p_no].x + dx, ply[p_no].y + dy, ply[p_no].z, SOLID, 0, 1))))
   return 1;

 return 0;
}

void Player_Monster_Collision(int p_no)
{
 int i;

 for (i = 0; i < mon_count; i++)
 {
  if ((abs(ply[p_no].x - mon[i].x) < 16) &&
      (abs(ply[p_no].y - mon[i].y) < 16))
  {
   game_exit = 1;
  }
 }
}

void Player_Laser_Collision(int p_no)
{
 if (map_laser[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth]) game_exit = 1;
 if (map_laser[(ply[p_no].x + (_block_width-1))/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth]) game_exit = 1;
 if (map_laser[ply[p_no].x/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth]) game_exit = 1;
 if (map_laser[(ply[p_no].x + (_block_width-1))/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth]) game_exit = 1;
}

void Player_Death(int p_no)
{

}
