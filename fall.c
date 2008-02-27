#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Check_Fall(int p_no)
{
 if (map[ply[p_no].x/32][ply[p_no].y/32][ply[p_no].z/8] == FALL)
 map[ply[p_no].x/32][ply[p_no].y/32][ply[p_no].z/8] = FALL_PRIME;

 if (map[(ply[p_no].x + 31)/32][ply[p_no].y/32][ply[p_no].z/8] == FALL)
 map[(ply[p_no].x + 31)/32][ply[p_no].y/32][ply[p_no].z/8] = FALL_PRIME;

 if (map[ply[p_no].x/32][(ply[p_no].y + 31)/32][ply[p_no].z/8] == FALL)
 map[ply[p_no].x/32][(ply[p_no].y + 31)/32][ply[p_no].z/8] = FALL_PRIME;

 if (map[(ply[p_no].x + 31)/32][(ply[p_no].y + 31)/32][ply[p_no].z/8] == FALL)
 map[(ply[p_no].x + 31)/32][(ply[p_no].y + 31)/32][ply[p_no].z/8] = FALL_PRIME;
}

void Check_Fall_Prime(void)
{
 int i, j, k;

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 if ((map[i][j][k] == FALL_PRIME) && (!Player_On_Fall(i, j)))
 {
  map[i][j][k] = SOLID;
  DirtyList(i*32 - 16, j*32 - 16, k*8, 32, 40, blank);
  DirtyList(i*32 + 16, j*32 - 16, k*8, 32, 40, blank);
  DirtyList(i*32 - 16, j*32 + 16, k*8, 32, 40, blank);
  DirtyList(i*32 + 16, j*32 + 16, k*8, 32, 40, blank);

  Add_Sound(i, SFX_FALL);
 }
}

int Player_On_Fall(int x, int y)
{
 if ((ply[0].x/32 == x) && (ply[0].y/32 == y)) return 1;
 if (((ply[0].x + 31)/32 == x) && (ply[0].y/32 == y)) return 1;
 if ((ply[0].x/32 == x) && ((ply[0].y + 31)/32 == y)) return 1;
 if (((ply[0].x + 31)/32 == x) && ((ply[0].y + 31)/32 == y)) return 1;

 if ((ply[1].x/32 == x) && (ply[1].y/32 == y)) return 1;
 if (((ply[1].x + 31)/32 == x) && (ply[1].y/32 == y)) return 1;
 if ((ply[1].x/32 == x) && ((ply[1].y + 31)/32 == y)) return 1;
 if (((ply[1].x + 31)/32 == x) && ((ply[1].y + 31)/32 == y)) return 1;

 return 0;
}
