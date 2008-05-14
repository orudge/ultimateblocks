/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

#define OWEN_HIDE_TRANSITIONS  0

void my_fade_out(int speed);
void my_fade_in(AL_CONST PALETTE p, int speed);
void my_fade_from_range(AL_CONST PALETTE source, AL_CONST PALETTE dest, int speed, int from, int to);

void Transition_Out(BITMAP *pic, PALETTE pal)
{
#if OWEN_HIDE_TRANSITIONS
 BITMAP *temp2 = create_bitmap(640, 480);
 PALETTE rgb;
 int i, j, k, l;
 int x, y;

 for (i = 2; i < 30; i+=2)
 {
  for (j = 0; j < 700; j += i)
  for (k = 0; k < 500; k += i*479/640)
  {
   x = j + i;
   y = k + i*480/640;
   if (x > 640) x = 640;
   if (y > 480) y = 480;

   rectfill(screen, j, k, x, y, getpixel(pic, (j + x)/2, (k + y)/2));
  }
 for (l = 0; l < 255; l++)
 {
  rgb[l].r = pal[l].r*(30-i)/30;
  rgb[l].g = pal[l].g*(30-i)/30;
  rgb[l].b = pal[l].b*(30-i)/30;
 }

   set_palette(rgb);
 while (time_count < 5);
 time_count = 0;
/*  blit(temp2, screen, 0, 0, 0, 0, 640, 480);*/
 }
#else
	blit(pic, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	my_fade_out(8);
#endif
}

void Transition_In(BITMAP *pic, PALETTE pal)
{
#if OWEN_HIDE_TRANSITIONS
BITMAP *temp2 = create_bitmap(640, 480);
 palette rgb;
 int i, j, k, l;
 int x, y;

 for (i = 0; i < 255; i++)
 {
  rgb[i].r = 0;
  rgb[i].g = 0;
  rgb[i].b = 0;
 }

 set_palette(rgb);

 for (i = 30; i > 2; i-=2)
 {
  for (j = 0; j < 700; j += i)
  for (k = 0; k < 500; k += i*479/640)
  {
   x = j + i;
   y = k + i*480/640;
   if (x > 640) x = 640;
   if (y > 480) y = 480;

   rectfill(screen, j, k, x, y, getpixel(pic, (j + x)/2, (k + y)/2));
  }

 for (l = 0; l < 255; l++)
 {
  rgb[l].r = pal[l].r*(30-i)/30;
  rgb[l].g = pal[l].g*(30-i)/30;
  rgb[l].b = pal[l].b*(30-i)/30;
 }

   set_palette(rgb);

/* while(!key[KEY_F1]);
 while(key[KEY_F1]);*/
 while (time_count < 5);
 time_count = 0;
/*  blit(temp2, screen, 0, 0, 0, 0, 640, 480);*/
 }

 set_palette(pal);
 blit(pic, screen, 0, 0, 0, 0, 640, 480);
#else
	set_palette(black_palette);
	blit(pic, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	my_fade_in(pal, 8);
#endif
}

void my_fade_from_range(AL_CONST PALETTE source, AL_CONST PALETTE dest, int speed, int from, int to)
{
   PALETTE temp;
   int c, start, last;

   ASSERT(speed > 0 && speed <= 64);
   ASSERT(from >= 0 && from < PAL_SIZE);
   ASSERT(to >= 0 && to < PAL_SIZE);

   for (c=0; c<PAL_SIZE; c++)
      temp[c] = source[c];

      start = retrace_count;
      last = -1;

      while ((c = (retrace_count-start)*speed/2) < 64) {
	 if (c != last) {
	    fade_interpolate(source, dest, temp, c, from, to);
	    set_palette_range(temp, from, to, TRUE);
	    last = c;

		Poll_Music();
	 }
      }

   set_palette_range(dest, from, to, TRUE);
}
/* fade_in:
 *  Fades from a solid black palette to p, at the specified speed (1 is
 *  the slowest, 64 is instantaneous).
 */
void my_fade_in(AL_CONST PALETTE p, int speed)
{
   ASSERT(speed > 0 && speed <= 64);
   my_fade_from_range(black_palette, p, speed, 0, PAL_SIZE-1);
}



/* fade_out:
 *  Fades from the current palette to a solid black palette, at the 
 *  specified speed (1 is the slowest, 64 is instantaneous).
 */
void my_fade_out(int speed)
{
   PALETTE temp;
   ASSERT(speed > 0 && speed <= 64);

   get_palette(temp);
   my_fade_from_range(temp, black_palette, speed, 0, PAL_SIZE-1);
}
