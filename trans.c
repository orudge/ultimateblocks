/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

#define OWEN_HIDE_TRANSITIONS  0

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
#endif
}
