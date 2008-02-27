#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Add_Sound(int x, int s)
{
 fx[fx_count].x = x*255/32;
 fx[fx_count].s = s;

 fx_count++;
}

void Play_Sound(void)
{
 int i;

 for (i = 0; i < fx_count; i++)
 {
  play_sample(sfx[fx[i].s].dat, 255, fx[i].x, 1000, 0);
 }

 fx_count = 0;
}


