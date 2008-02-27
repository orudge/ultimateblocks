#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Animate(void)
{
 int i;

 for (i = 0; i < ani_count; i++)
 {
  if (ani[i].type == SPLASH) Ani_Splash(i);
 }
}

void Ani_Splash(int a)
{
 ani[a].frame++;

 if (ani[a].frame/10 == 0) DirtyList(ani[a].x, ani[a].y, ani[a].z, 32, 32, splash_pic[0]);
 if (ani[a].frame/10 == 1) DirtyList(ani[a].x, ani[a].y, ani[a].z, 32, 32, splash_pic[1]);
 if (ani[a].frame/10 == 2) DirtyList(ani[a].x, ani[a].y, ani[a].z, 32, 32, splash_pic[2]);
 if (ani[a].frame/10 == 3) DirtyList(ani[a].x, ani[a].y, ani[a].z, 32, 32, splash_pic[3]);

 if (ani[a].frame/10 == 4)
 {
  DirtyList(ani[a].x, ani[a].y, ani[a].z, 32, 32, blank);
  Del_Ani(a);
 }
}

void Del_Ani(int a)
{
 int i;

 for (i = a; i < ani_count; i++)
 {
  ani[i].x = ani[i + 1].x;
  ani[i].y = ani[i + 1].y;
  ani[i].z = ani[i + 1].z;
  ani[i].type = ani[i + 1].type;
  ani[i].frame = ani[i + 1].frame;
 }

 ani_count--;
}

void Add_Ani(int x, int y, int z, int type)
{
 ani[ani_count].x = x;
 ani[ani_count].y = y;
 ani[ani_count].z = z;
 ani[ani_count].type = type;

 ani_count++;
}
