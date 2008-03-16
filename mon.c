/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

void Monster(void)
{
 int i, j, k;

 Monster_Die_Check();

 for (i = 0; i < mon_count; i++)
 {
  mon[i].count++;

  if (mon[i].count > mon[i].speed)
  {
   mon[i].count = 1;

   if (mon[i].type == MON0) Monster_Mon0(i);
   if (mon[i].type == MON1) Monster_Mon1(i);
   if (mon[i].type == MON2) Monster_Mon1(i);
  }

  Monster_Draw(i);
 }

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 if (map[i][j][k] == RESERVE0) map[i][j][k] = 0;

 for (i = 0; i < mon_count; i++)
 {
  map[mon[i].x/_block_width][mon[i].y/_block_height][mon[i].z/_block_depth] = RESERVE0;
  map[(mon[i].x + (_block_width-1))/_block_width][mon[i].y/_block_height][mon[i].z/_block_depth] = RESERVE0;
  map[mon[i].x/_block_width][(mon[i].y + (_block_height-1))/_block_height][mon[i].z/_block_depth] = RESERVE0;
  map[(mon[i].x + (_block_width-1))/_block_width][(mon[i].y + (_block_height-1))/_block_height][mon[i].z/_block_depth] = RESERVE0;
 }
}

void Monster_Draw(int m_no)
{
 if ((mon[m_no].dx) || (mon[m_no].dy)) mon[m_no].frame++;
 if (Player_Frame(mon[m_no].frame) == -1) mon[m_no].frame = 0;

 if (mon[m_no].dir == 4) DirtyList(mon[m_no].x, mon[m_no].y, mon[m_no].z, _block_width, _block_height_depth, mon_pic[0][0 + Player_Frame(mon[m_no].frame)]);
 if (mon[m_no].dir == 1) DirtyList(mon[m_no].x, mon[m_no].y, mon[m_no].z, _block_width, _block_height_depth, mon_pic[0][3 + Player_Frame(mon[m_no].frame)]);
 if (mon[m_no].dir == 2) DirtyList(mon[m_no].x, mon[m_no].y, mon[m_no].z, _block_width, _block_height_depth, mon_pic[0][6 + Player_Frame(mon[m_no].frame)]);
 if (mon[m_no].dir == 8) DirtyList(mon[m_no].x, mon[m_no].y, mon[m_no].z, _block_width, _block_height_depth, mon_pic[0][9 + Player_Frame(mon[m_no].frame)]);
}

void Monster_Change_Dir(int m_no, int x, int y, int z)
{
 if (z == 0)
 {
  if (mon[m_no].dy == -1)
  {
   mon[m_no].dir = 0;
   if (map[x][y + 1][0] == 0) mon[m_no].dir = 4;
   if (map[x + 1][y][0] == 0) mon[m_no].dir = 2;
   if (map[x - 1][y][0] == 0) mon[m_no].dir = 8;
  }

  if (mon[m_no].dy == 1)
  {
   mon[m_no].dir = 0;
   if (map[x][y - 1][0] == 0) mon[m_no].dir = 1;
   if (map[x - 1][y][0] == 0) mon[m_no].dir = 8;
   if (map[x + 1][y][0] == 0) mon[m_no].dir = 2;
  }

  if (mon[m_no].dx == 1)
  {
   mon[m_no].dir = 0;
   if (map[x - 1][y][0] == 0) mon[m_no].dir = 8;
   if (map[x][y + 1][0] == 0) mon[m_no].dir = 4;
   if (map[x][y - 1][0] == 0) mon[m_no].dir = 1;
  }

  if (mon[m_no].dx == -1)
  {
   mon[m_no].dir = 0;
   if (map[x + 1][y][0] == 0) mon[m_no].dir = 2;
   if (map[x][y - 1][0] == 0) mon[m_no].dir = 1;
   if (map[x][y + 1][0] == 0) mon[m_no].dir = 4;
  }
 }
 else
 {
  if (mon[m_no].dy == -1)
  {
   mon[m_no].dir = 0;
   if ((map[x][y + 1][1] == 0) && (map[x][y + 1][0] & FULL)) mon[m_no].dir = 4;
   if ((map[x + 1][y][1] == 0) && (map[x + 1][y][0] & FULL)) mon[m_no].dir = 2;
   if ((map[x - 1][y][1] == 0) && (map[x - 1][y][0] & FULL)) mon[m_no].dir = 8;
  }

  if (mon[m_no].dy == 1)
  {
   mon[m_no].dir = 0;
   if ((map[x][y - 1][1] == 0) && (map[x][y - 1][0] & FULL))mon[m_no].dir = 1;
   if ((map[x - 1][y][1] == 0) && (map[x - 1][y][0] & FULL))mon[m_no].dir = 8;
   if ((map[x + 1][y][1] == 0) && (map[x + 1][y][0] & FULL))mon[m_no].dir = 2;
  }

  if (mon[m_no].dx == 1)
  {
   mon[m_no].dir = 0;
   if ((map[x - 1][y][1] == 0) && (map[x - 1][y][0] & FULL)) mon[m_no].dir = 8;
   if ((map[x][y + 1][1] == 0) && (map[x][y + 1][0] & FULL)) mon[m_no].dir = 4;
   if ((map[x][y - 1][1] == 0) && (map[x][y - 1][0] & FULL)) mon[m_no].dir = 1;
  }

  if (mon[m_no].dx == -1)
  {
   mon[m_no].dir = 0;
   if ((map[x + 1][y][1] == 0) && (map[x + 1][y][0] & FULL)) mon[m_no].dir = 2;
   if ((map[x][y - 1][1] == 0) && (map[x][y - 1][0] & FULL)) mon[m_no].dir = 1;
   if ((map[x][y + 1][1] == 0) && (map[x][y + 1][0] & FULL)) mon[m_no].dir = 4;
  }
 }


 mon[m_no].dx = 0;
 mon[m_no].dy = 0;

 if (mon[m_no].dir == 1) mon[m_no].dy = -1;
 if (mon[m_no].dir == 2) mon[m_no].dx = 1;
 if (mon[m_no].dir == 4) mon[m_no].dy = 1;
 if (mon[m_no].dir == 8) mon[m_no].dx = -1;

 if (mon[m_no].dir == 0) mon[m_no].dir = 4;

 mon[m_no].count = -4;
}

int Monster_Sight(int x, int y, int z, int dir)
{
 int dx, dy;

 dx = 0;
 dy = 0;

 if (dir == 1) dy = -1;
 if (dir == 2) dx = 1;
 if (dir == 4) dy = 1;
 if (dir == 8) dx = -1;

 while ((!(map[x][y][z] & FULL)) &&
       ((x != ply[0].x/_block_width) || (y != ply[0].y/_block_height) ||
        (x != (ply[0].x + (_block_width-1))/_block_width) || (y != (ply[0].y + (_block_height-1))/_block_height)))
 {
  x += dx;
  y += dy;
 }

 if (map[x][y][z] & FULL) return 0;

 return 1;

}

void Monster_Mon3(int m_no)
{
 if (mon[m_no].state == PATROL) Monster_Patrol(m_no);

 if (Monster_Sight(mon[m_no].x/_block_width, mon[m_no].y/_block_height, mon[m_no].z/_block_depth, mon[m_no].dir))
 {
  mon[m_no].speed = 0;
 }
}

void Monster_Move(int m_no)
{
 map[mon[m_no].x/_block_width][mon[m_no].y/_block_height][mon[m_no].z/_block_depth] = 0;
 map[(mon[m_no].x + (_block_width - 1))/_block_width][mon[m_no].y/_block_height][mon[m_no].z/_block_depth] = 0;
 map[mon[m_no].x/_block_width][(mon[m_no].y + (_block_height - 1))/_block_height][mon[m_no].z/_block_depth] = 0;
 map[(mon[m_no].x + (_block_width - 1))/_block_width][(mon[m_no].y + (_block_height - 1))/_block_height][mon[m_no].z/_block_depth] = 0;

 mon[m_no].x += mon[m_no].dx;
 mon[m_no].y += mon[m_no].dy;
 mon[m_no].z += mon[m_no].dz;

 if (mon[m_no].z == 0) mon[m_no].dz = 0;

 map[mon[m_no].x/_block_width][mon[m_no].y/_block_height][mon[m_no].z/_block_depth] = RESERVE0;
 map[(mon[m_no].x + (_block_width - 1))/_block_width][mon[m_no].y/_block_height][mon[m_no].z/_block_depth] = RESERVE0;
 map[mon[m_no].x/_block_width][(mon[m_no].y + (_block_height - 1))/_block_height][mon[m_no].z/_block_depth] = RESERVE0;
 map[(mon[m_no].x + (_block_width - 1))/_block_width][(mon[m_no].y + (_block_height - 1))/_block_height][mon[m_no].z/_block_depth] = RESERVE0;
}

void Monster_Mon0(int m_no)
{
 if (mon[m_no].state == PATROL) Monster_Patrol(m_no);
}

void Monster_Mon2(int m_no)
{
 if (mon[m_no].state == PATROL) Monster_Patrol(m_no);
}

void Monster_Mon1(int m_no)
{
 if (mon[m_no].state == PATROL) Monster_Patrol(m_no);
 if (mon[m_no].state == TRACK) Monster_Track(m_no);

 if ((mon[m_no].state != TRACK) && (Monster_Sight(mon[m_no].x/_block_width, mon[m_no].y/_block_height, mon[m_no].z/_block_depth, mon[m_no].dir)))
 {
  mon[m_no].speed = 2;
  mon[m_no].state = TRACK;
  Add_Sound(mon[m_no].x/_block_width, SFX_ROAR);
 }
}

void Monster_Patrol(int m_no)
{
 if ((mon[m_no].dz == 0) &&
     (mon[m_no].x%_block_width == 0) &&
     (mon[m_no].y%_block_height == 0) &&

     (((map[mon[m_no].x/_block_width + mon[m_no].dx][mon[m_no].y/_block_height + mon[m_no].dy][mon[m_no].z/_block_depth + mon[m_no].dz] != 0) &&
      ((map[mon[m_no].x/_block_width + mon[m_no].dx][mon[m_no].y/_block_height + mon[m_no].dy][mon[m_no].z/_block_depth + mon[m_no].dz] & SWITCH) == 0))
 ||
     (map_laser[mon[m_no].x/_block_width + mon[m_no].dx][mon[m_no].y/_block_height + mon[m_no].dy][mon[m_no].z/_block_depth + mon[m_no].dz] != 0)
 ||
     ((mon[m_no].z/_block_depth == 1) &&
     ((map[mon[m_no].x/_block_width + mon[m_no].dx][mon[m_no].y/_block_height + mon[m_no].dy][0] & FULL) == 0)
    )))
 {
  if ((mon[m_no].z /8 == 1) && (map[mon[m_no].x/_block_width][mon[m_no].y/_block_height][0] == 0))
  {
   mon[m_no].dx = 0;
   mon[m_no].dy = 0;
   mon[m_no].dz = -1;
   return;
  }
  else
  Monster_Change_Dir(m_no, mon[m_no].x/_block_width, mon[m_no].y/_block_height, mon[m_no].z/_block_depth);
  return;
 }

 Monster_Move(m_no);
}

void Monster_Track(int m_no)
{
 if ((mon[m_no].dz == 0) &&
     (mon[m_no].x%_block_width == 0) &&
     (mon[m_no].y%_block_height == 0))
 {
  if ((mon[m_no].z/_block_depth == 1) && (map[mon[m_no].x/_block_width][mon[m_no].y/_block_height][0] == 0))
  {
   mon[m_no].dx = 0;
   mon[m_no].dy = 0;
   mon[m_no].dz = -1;
   return;
  }
  else Monster_Change_Dir_Track(m_no, 0);
 }

 Monster_Move(m_no);
}

void Monster_Change_Dir_Track(int m_no, int p_no)
{
 mon[m_no].dx = 0;
 mon[m_no].dy = 0;

 if ((ply[p_no].x/_block_width > mon[m_no].x/_block_width) &&
     (!Check_Map(mon[m_no].x + 1, mon[m_no].y, mon[m_no].z, FULL, 0, 1)) &&
     (!Check_Map(mon[m_no].x + 1, mon[m_no].y, mon[m_no].z, PICKUP, 0, 1)))
  mon[m_no].dx = 1;

 if ((ply[p_no].x/_block_width < mon[m_no].x/_block_width) &&
     (!Check_Map(mon[m_no].x - 1, mon[m_no].y, mon[m_no].z, FULL, 0, 1)) &&
     (!Check_Map(mon[m_no].x - 1, mon[m_no].y, mon[m_no].z, PICKUP, 0, 1)))
  mon[m_no].dx = -1;

 if ((ply[p_no].y/_block_height > mon[m_no].y/_block_height) &&
     (!Check_Map(mon[m_no].x + mon[m_no].dx, mon[m_no].y + 1, mon[m_no].z, FULL, 0, 1)) &&
     (!Check_Map(mon[m_no].x + mon[m_no].dx, mon[m_no].y + 1, mon[m_no].z, PICKUP, 0, 1)))
  mon[m_no].dy = 1;

 if ((ply[p_no].y/_block_height < mon[m_no].y/_block_height) &&
     (!Check_Map(mon[m_no].x + mon[m_no].dx, mon[m_no].y - 1, mon[m_no].z, FULL, 0, 1)) &&
     (!Check_Map(mon[m_no].x + mon[m_no].dx, mon[m_no].y - 1, mon[m_no].z, PICKUP, 0, 1)))
  mon[m_no].dy = -1;

/* if ((mon[m_no].dx) && (mon[m_no].dy))
 {
  if (rand()%2) mon[m_no].dx = 0;
  else mon[m_no].dy = 0;
 }*/

 if (mon[m_no].dy == -1) mon[m_no].dir = 1;
 if (mon[m_no].dx == 1) mon[m_no].dir = 2;
 if (mon[m_no].dy == 1) mon[m_no].dir = 4;
 if (mon[m_no].dx == -1) mon[m_no].dir = 8;
}

void Monster_Die_Check(void)
{
 int i;

 for (i = 0; i < mon_count; i++)
 {
  if (map_laser[mon[i].x/_block_width][mon[i].y/_block_height][mon[i].z/_block_depth])
  {
   Add_Explode(mon[i].x/_block_width, mon[i].y/_block_height, mon[i].z/_block_depth);
   Del_Monster(i);
   i--;
  }

  if (map_laser[(mon[i].x + (_block_width - 1))/_block_width][mon[i].y/_block_height][mon[i].z/_block_depth])
  {
   Add_Explode((mon[i].x + (_block_width - 1))/_block_width, mon[i].y/_block_height, mon[i].z/_block_depth);
   Del_Monster(i);
   i--;
  }

  if (map_laser[mon[i].x/_block_width][(mon[i].y + (_block_height - 1))/_block_height][mon[i].z/_block_depth])
  {
   Add_Explode(mon[i].x/_block_width, (mon[i].y + (_block_height - 1))/_block_height, mon[i].z/_block_depth);
   Del_Monster(i);
   i--;
  }

  if (map_laser[(mon[i].x + (_block_width - 1))/_block_width][(mon[i].y + (_block_height - 1))/_block_height][mon[i].z/_block_depth])
  {

   Add_Explode((mon[i].x + (_block_width - 1))/_block_width, (mon[i].y + (_block_height - 1))/_block_height, mon[i].z/_block_depth);
   Del_Monster(i);
   i--;
  }
 }
}

void Del_Monster(int m_no)
{
 int i;

 DirtyList(mon[m_no].x, mon[m_no].y, mon[m_no].z, _block_width, _block_height_depth, blank);

 for (i = m_no; i < mon_count; i++)
 {
  mon[i].x = mon[i + 1].x;
  mon[i].y = mon[i + 1].y;
  mon[i].z = mon[i + 1].z;
  mon[i].dx = mon[i + 1].dx;
  mon[i].dy = mon[i + 1].dy;
  mon[i].dz = mon[i + 1].dz;

  mon[i].dir = mon[i + 1].dir;
  mon[i].state = mon[i + 1].state;
  mon[i].type = mon[i + 1].type;
  mon[i].frame = mon[i + 1].frame;
  mon[i].speed = mon[i + 1].speed;
  mon[i].count = mon[i + 1].count;
 }
 mon_count--;
}
