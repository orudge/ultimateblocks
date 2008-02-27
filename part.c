#include <allegro.h>
#include <jgmod.h>

#include "blocks3.h"

int counter;

void Add_Part(int x, int y, int dx, int dy, int col)
{
 part[part_count].x = x;
 part[part_count].y = y;
 part[part_count].dx = dx;
 part[part_count].dy = dy;
 part[part_count].col = col;

 part_count++;
}

void Del_Part(int p_no)
{
 int i;

 for (i = p_no; i < part_count; i++)
 {
  part[i].x = part[i + 1].x;
  part[i].y = part[i + 1].y;
  part[i].dx = part[i + 1].dx;
  part[i].dy = part[i + 1].dy;
  part[i].col = part[i + 1].col;
 }

 part_count--;
}

void Particle(void)
{
 if (part_count < 999)
 {
  if (rand()%2 == 1) Add_Part(320 + rand()%16, 200 + rand()%32, (rand()%50 + 20), -(rand()%10 + 5), rand()%255);
  else  Add_Part(320 + rand()%16, 200 + rand()%32, -(rand()%50 + 20), -(rand()%10 + 5), rand()%255);
 }

 Particle_Move();
}

void Particle_Move(void)
{
 int i;

 counter++;

 if (counter > 50) counter = 0;

 for (i = 0; i < part_count; i++)
 {
  putpixel(screen, part[i].x, part[i].y, getpixel(temp, part[i].x, part[i].y));

  if (!(counter%part[i].dx)) part[i].x += part[i].dx/abs(part[i].dx);
  if (!(counter%part[i].dy)) part[i].y += part[i].dy/abs(part[i].dy);

  part[i].col++;

  if (part[i].col == 256)
  {
   Del_Part(i);
   i--;
  }
  else
  {
   putpixel(screen, part[i].x, part[i].y, makecol8(255, 255 - part[i].col, 255 - part[i].col));

   if ((part[i].col > 100) && (part_count < 900))
   {
    Add_Part(part[i].x, part[i].y + 1, part[i].dx, part[i].dy, part[i].col);
   }
  }
 }
}


