/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Add_Undo(void)
{
 int i, j, k;

 if (undo_count == 20)
 {
  Move_Undo_Down();
  undo_count = 19;
 }

 /* Player */

 for (i = 0; i < 2; i++)
 {
  undo[undo_count].ply[i].x = ply[i].x;
  undo[undo_count].ply[i].y = ply[i].y;
  undo[undo_count].ply[i].z = ply[i].z;
  undo[undo_count].ply[i].dx = ply[i].dx;
  undo[undo_count].ply[i].dy = ply[i].dy;
  undo[undo_count].ply[i].dz = ply[i].dz;
  undo[undo_count].ply[i].frame = ply[i].frame;
  undo[undo_count].ply[i].dir = ply[i].dir;
 }

 /* Boxes */

 for (i = 0; i < box_count; i++)
 {
  undo[undo_count].box[i].x = box[i].x;
  undo[undo_count].box[i].y = box[i].y;
  undo[undo_count].box[i].z = box[i].z;
  undo[undo_count].box[i].dx = box[i].dx;
  undo[undo_count].box[i].dy = box[i].dy;
  undo[undo_count].box[i].dz = box[i].dz;
  undo[undo_count].box[i].dir = box[i].dir;
  undo[undo_count].box[i].type = box[i].type;
  undo[undo_count].box[i].pic = box[i].pic;
 }

 /* Explode */

 for (i = 0; i < explode_count; i++)
 {
  undo[undo_count].explode[i].x = explode[i].x;
  undo[undo_count].explode[i].y = explode[i].y;
  undo[undo_count].explode[i].z = explode[i].z;
  undo[undo_count].explode[i].frame = explode[i].frame;
  undo[undo_count].explode[i].type = explode[i].type;
 }

 /* Monsters */

 for (i = 0; i < mon_count; i++)
 {
  undo[undo_count].mon[i].x = mon[i].x;
  undo[undo_count].mon[i].y = mon[i].y;
  undo[undo_count].mon[i].z = mon[i].z;
  undo[undo_count].mon[i].dx = mon[i].dx;
  undo[undo_count].mon[i].dy = mon[i].dy;
  undo[undo_count].mon[i].dz = mon[i].dz;

  undo[undo_count].mon[i].dir = mon[i].dir;
  undo[undo_count].mon[i].state = mon[i].state;
  undo[undo_count].mon[i].type = mon[i].type;

  undo[undo_count].mon[i].frame = mon[i].frame;
  undo[undo_count].mon[i].speed = mon[i].speed;
  undo[undo_count].mon[i].count = mon[i].count;
 }

 /* Doors */

 for (i = 0; i < door_count; i++)
 {
  undo[undo_count].door[i].x = door[i].x;
  undo[undo_count].door[i].y = door[i].y;
  undo[undo_count].door[i].z = door[i].z;
  undo[undo_count].door[i].c = door[i].c;
  undo[undo_count].door[i].state = door[i].state;
 }

 /* Switches */

 for (i = 0; i < switches_count; i++)
 {
  undo[undo_count].switches[i].x = switches[i].x;
  undo[undo_count].switches[i].y = switches[i].y;
  undo[undo_count].switches[i].z = switches[i].z;
  undo[undo_count].switches[i].c = switches[i].c;
 }

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 {
  undo[undo_count].map[i][j][k] = map[i][j][k];
  undo[undo_count].map_laser[i][j][k] = map_laser[i][j][k];
  undo[undo_count].map_door[i][j][k] = map_door[i][j][k];
 }

 undo[undo_count].box_count = box_count;
 undo[undo_count].explode_count = explode_count;
 undo[undo_count].mon_count = mon_count;
 undo[undo_count].door_count = door_count;
 undo[undo_count].switches_count = switches_count;

 undo_count++;
}

void Undo(void)
{
 int i, j, k;

 if (undo_count == 0) return;

 undo_count--;

 box_count = undo[undo_count].box_count;
 explode_count = undo[undo_count].explode_count;
 mon_count = undo[undo_count].mon_count;
 door_count = undo[undo_count].door_count;
 switches_count = undo[undo_count].switches_count;

 /* Player */

 for (i = 0; i < 2; i++)
 {
  ply[i].x = undo[undo_count].ply[i].x;
  ply[i].y = undo[undo_count].ply[i].y;
  ply[i].z = undo[undo_count].ply[i].z;
  ply[i].dx = undo[undo_count].ply[i].dx;
  ply[i].dy = undo[undo_count].ply[i].dy;
  ply[i].dz = undo[undo_count].ply[i].dz;
  ply[i].frame = undo[undo_count].ply[i].frame;
  ply[i].dir = undo[undo_count].ply[i].dir;
 }

 /* Boxes */

 for (i = 0; i < box_count; i++)
 {
  box[i].x = undo[undo_count].box[i].x;
  box[i].y = undo[undo_count].box[i].y;
  box[i].z = undo[undo_count].box[i].z;
  box[i].dx = undo[undo_count].box[i].dx;
  box[i].dy = undo[undo_count].box[i].dy;
  box[i].dz = undo[undo_count].box[i].dz;
  box[i].dir = undo[undo_count].box[i].dir;
  box[i].type = undo[undo_count].box[i].type;
  box[i].pic = undo[undo_count].box[i].pic;
 }

 /* Explode */

 for (i = 0; i < explode_count; i++)
 {
  explode[i].x = undo[undo_count].explode[i].x;
  explode[i].y = undo[undo_count].explode[i].y;
  explode[i].z = undo[undo_count].explode[i].z;
  explode[i].frame = undo[undo_count].explode[i].frame;
  explode[i].type = undo[undo_count].explode[i].type;
 }

 /* Monsters */

 for (i = 0; i < mon_count; i++)
 {
  mon[i].x = undo[undo_count].mon[i].x;
  mon[i].y = undo[undo_count].mon[i].y;
  mon[i].z = undo[undo_count].mon[i].z;
  mon[i].dx = undo[undo_count].mon[i].dx;
  mon[i].dy = undo[undo_count].mon[i].dy;
  mon[i].dz = undo[undo_count].mon[i].dz;

  mon[i].dir = undo[undo_count].mon[i].dir;
  mon[i].state = undo[undo_count].mon[i].state;
  mon[i].type = undo[undo_count].mon[i].type;

  mon[i].frame = undo[undo_count].mon[i].frame;
  mon[i].speed = undo[undo_count].mon[i].speed;
  mon[i].count = undo[undo_count].mon[i].count;
 }

 /* Doors */

 for (i = 0; i < door_count; i++)
 {
  door[i].x = undo[undo_count].door[i].x;
  door[i].y = undo[undo_count].door[i].y;
  door[i].z = undo[undo_count].door[i].z;
  door[i].c = undo[undo_count].door[i].c;
  door[i].state = undo[undo_count].door[i].state;
 }

 /* Switches */

 for (i = 0; i < switches_count; i++)
 {
  switches[i].x = undo[undo_count].switches[i].x;
  switches[i].y = undo[undo_count].switches[i].y;
  switches[i].z = undo[undo_count].switches[i].z;
  switches[i].c = undo[undo_count].switches[i].c;
 }

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 {
  map[i][j][k] = undo[undo_count].map[i][j][k];
  map_laser[i][j][k] = undo[undo_count].map_laser[i][j][k];
  map_door[i][j][k] = undo[undo_count].map_door[i][j][k];
 }

 Draw_Map();

 blit(temp, screen, 0, 0, 0, 0, 640, 480);
}

void Move_Undo_Down(void)
{
 int i, j, k;

 for (undo_count = 0; undo_count < 19; undo_count++)
 {

 /* Player */

 for (i = 0; i < 2; i++)
 {
  undo[undo_count].ply[i].x = undo[undo_count + 1].ply[i].x;
  undo[undo_count].ply[i].y = undo[undo_count + 1].ply[i].y;
  undo[undo_count].ply[i].z = undo[undo_count + 1].ply[i].z;
  undo[undo_count].ply[i].dx = undo[undo_count + 1].ply[i].dx;
  undo[undo_count].ply[i].dy = undo[undo_count + 1].ply[i].dy;
  undo[undo_count].ply[i].dz = undo[undo_count + 1].ply[i].dz;
  undo[undo_count].ply[i].frame = undo[undo_count + 1].ply[i].frame;
  undo[undo_count].ply[i].dir = undo[undo_count + 1].ply[i].dir;
 }

 /* Boxes */

 for (i = 0; i < box_count; i++)
 {
  undo[undo_count].box[i].x = undo[undo_count + 1].box[i].x;
  undo[undo_count].box[i].y = undo[undo_count + 1].box[i].y;
  undo[undo_count].box[i].z = undo[undo_count + 1].box[i].z;
  undo[undo_count].box[i].dx = undo[undo_count + 1].box[i].dx;
  undo[undo_count].box[i].dy = undo[undo_count + 1].box[i].dy;
  undo[undo_count].box[i].dz = undo[undo_count + 1].box[i].dz;
  undo[undo_count].box[i].dir = undo[undo_count + 1].box[i].dir;
  undo[undo_count].box[i].type = undo[undo_count + 1].box[i].type;
  undo[undo_count].box[i].pic = undo[undo_count + 1].box[i].pic;
 }

 /* Explode */

 for (i = 0; i < explode_count; i++)
 {
  undo[undo_count].explode[i].x = undo[undo_count + 1].explode[i].x;
  undo[undo_count].explode[i].y = undo[undo_count + 1].explode[i].y;
  undo[undo_count].explode[i].z = undo[undo_count + 1].explode[i].z;
  undo[undo_count].explode[i].frame = undo[undo_count + 1].explode[i].frame;
  undo[undo_count].explode[i].type = undo[undo_count + 1].explode[i].type;
 }

 /* Monsters */

 for (i = 0; i < mon_count; i++)
 {
  undo[undo_count].mon[i].x = undo[undo_count + 1].mon[i].x;
  undo[undo_count].mon[i].y = undo[undo_count + 1].mon[i].y;
  undo[undo_count].mon[i].z = undo[undo_count + 1].mon[i].z;
  undo[undo_count].mon[i].dx = undo[undo_count + 1].mon[i].dx;
  undo[undo_count].mon[i].dy = undo[undo_count + 1].mon[i].dy;
  undo[undo_count].mon[i].dz = undo[undo_count + 1].mon[i].dz;

  undo[undo_count].mon[i].dir = undo[undo_count + 1].mon[i].dir;
  undo[undo_count].mon[i].state = undo[undo_count + 1].mon[i].state;
  undo[undo_count].mon[i].type = undo[undo_count + 1].mon[i].type;

  undo[undo_count].mon[i].frame = undo[undo_count + 1].mon[i].frame;
  undo[undo_count].mon[i].speed = undo[undo_count + 1].mon[i].speed;
  undo[undo_count].mon[i].count = undo[undo_count + 1].mon[i].count;
 }

 /* Doors */

 for (i = 0; i < door_count; i++)
 {
  undo[undo_count].door[i].x = undo[undo_count + 1].door[i].x;
  undo[undo_count].door[i].y = undo[undo_count + 1].door[i].y;
  undo[undo_count].door[i].z = undo[undo_count + 1].door[i].z;
  undo[undo_count].door[i].c = undo[undo_count + 1].door[i].c;
  undo[undo_count].door[i].state = undo[undo_count + 1].door[i].state;
 }

 /* Switches */

 for (i = 0; i < switches_count; i++)
 {
  undo[undo_count].switches[i].x = undo[undo_count + 1].switches[i].x;
  undo[undo_count].switches[i].y = undo[undo_count + 1].switches[i].y;
  undo[undo_count].switches[i].z = undo[undo_count + 1].switches[i].z;
  undo[undo_count].switches[i].c = undo[undo_count + 1].switches[i].c;
 }

 for (k = 0; k < 2; k++)
 for (j = 0; j < 15; j++)
 for (i = 0; i < 20; i++)
 {
  undo[undo_count].map[i][j][k] = undo[undo_count + 1].map[i][j][k];
  undo[undo_count].map_laser[i][j][k] = undo[undo_count + 1].map_laser[i][j][k];
  undo[undo_count].map_door[i][j][k] = undo[undo_count + 1].map_door[i][j][k];
 }


 undo[undo_count].box_count = undo[undo_count + 1].box_count;
 undo[undo_count].explode_count = undo[undo_count + 1].explode_count;
 undo[undo_count].mon_count = undo[undo_count + 1].mon_count;
 undo[undo_count].door_count = undo[undo_count + 1].door_count;
 undo[undo_count].switches_count = undo[undo_count + 1].switches_count;

 }

 undo_count = 20;
}

void Clear_Undo(void)
{
	undo_count = 0;
}