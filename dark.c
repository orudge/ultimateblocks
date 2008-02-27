#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Map_Darkness(void)
{
 int i, j;

 for (i = 0; i < 25; i++)
 for (j = 0; j < 15; j++)
 map_dark[i][j] = 0;

 Add_Dark_Grad(ply[0].x/32, ply[0].y/32);

 for (i = 0; i < 25; i++)
 for (j = 0; j < 15; j++)
 if (map_dark[i][j] > 5) map_dark[i][j] = 5;
}

void Add_Dark_Grad(int x, int y)
{
 map_dark[x][y] +=3;

 map_dark[x][y - 1] +=2;
 map_dark[x + 1][y] +=2;
 map_dark[x][y + 1] +=2;
 map_dark[x - 1][y] +=2;

 map_dark[x][y - 2] +=1;
 map_dark[x + 1][y - 1] +=1;
 map_dark[x + 2][y] +=1;
 map_dark[x + 1][y + 1] +=1;
 map_dark[x][y + 2] +=1;
 map_dark[x + 1][y - 1] +=1;
 map_dark[x - 2][y] +=1;
 map_dark[x - 1][y - 1] +=1;

 DirtyList((x-2)*32, (y-2)*32, 0, 160, 160, blank);
}
