#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

int map_backup[20][15][2];

void Door(void)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 map_backup[i][j][k] = map_door[i][j][k];

 for (i = 0; i < switches_count; i++)
 {
  if (map[switches[i].x][switches[i].y][switches[i].z] & SWITCH)
    map[switches[i].x][switches[i].y][switches[i].z] = 0;
 }

 i = 0;
 while (((switches[i].c != RED) ||
         (map[switches[i].x][switches[i].y][switches[i].z]) ||
         (Player_On(switches[i].x, switches[i].y, 0)) ||
         (Player_On(switches[i].x, switches[i].y, 1))) &&

         (i < switches_count))
 i++;

 if (i == switches_count) Open_Doors(RED);
 else Close_Doors(RED);

 i = 0;
 while (((switches[i].c != GREEN) ||
         (map[switches[i].x][switches[i].y][switches[i].z]) ||
         (Player_On(switches[i].x, switches[i].y, 0)) ||
         (Player_On(switches[i].x, switches[i].y, 1))) &&
         (i < switches_count))
 i++;

 if (i == switches_count) Open_Doors(GREEN);
 else Close_Doors(GREEN);

 i = 0;
 while (((switches[i].c != BLUE) ||
         (map[switches[i].x][switches[i].y][switches[i].z]) ||
         (Player_On(switches[i].x, switches[i].y, 0)) ||
         (Player_On(switches[i].x, switches[i].y, 1))) &&
         (i < switches_count))
 i++;

 if (i == switches_count) Open_Doors(BLUE);
 else Close_Doors(BLUE);

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 if (map_backup[i][j][k] != map_door[i][j][k])
 {
  if (map_door[i][j][k] == 0) DirtyList(i*32, j*32, k*8, 32, 40, blank);
  if (map_door[i][j][k] == RED) DirtyList(i*32, j*32, k*8, 32, 40, door_pic[0]);
  if (map_door[i][j][k] == GREEN) DirtyList(i*32, j*32, k*8, 32, 40, door_pic[1]);
  if (map_door[i][j][k] == BLUE) DirtyList(i*32, j*32, k*8, 32, 40, door_pic[2]);
 }

 for (i = 0; i < switches_count; i++)
 {
  if (map[switches[i].x][switches[i].y][switches[i].z] == 0)
  {
   map[switches[i].x][switches[i].y][switches[i].z] = switches[i].c + SWITCH;
   DirtyList(switches[i].x*32, switches[i].y*32, switches[i].z*8, 32, 40, blank);
  }
 }
}

void Open_Doors(int c)
{
 int i;

 for (i = 0; i < door_count; i++)
 {
  if ((door[i].c == c) && (map[door[i].x][door[i].y][door[i].z] & DOOR))
  {
   map[door[i].x][door[i].y][door[i].z] = 0;
   map_door[door[i].x][door[i].y][door[i].z] = 0;
  }
 }
}

void Close_Doors(int c)
{
 int i;

 for (i = 0; i < door_count; i++)
 {
  if ((door[i].c == c) && (map[door[i].x][door[i].y][door[i].z] == 0))
  {
   map[door[i].x][door[i].y][door[i].z] = DOOR+CLOSE+FULL+c;
   map_door[door[i].x][door[i].y][door[i].z] = c;
  }
 }
}

int Player_On(int x, int y, int p_no)
{
 int i;

 if ((ply[p_no].x/32 == x) && (ply[p_no].y/32 == y)) return 1;
 if (((ply[p_no].x + 31)/32 == x) && (ply[p_no].y/32 == y)) return 1;
 if ((ply[p_no].x/32 == x) && ((ply[p_no].y + 31)/32 == y)) return 1;
 if (((ply[p_no].x/32 + 31) == x) && ((ply[p_no].y + 31)/32 == y)) return 1;

 for (i = 0; i < mon_count; i++)
 {
  if ((mon[i].x/32 == x) && (mon[i].y/32 == y)) return 1;
  if (((mon[i].x + 31)/32 == x) && (mon[i].y/32 == y)) return 1;
  if ((mon[i].x/32 == x) && ((mon[i].y + 31)/32 == y)) return 1;
  if (((mon[i].x/32 + 31) == x) && ((mon[i].y + 31)/32 == y)) return 1;
 }

 return 0;
}
