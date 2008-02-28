/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include "blocks3.h"

void Box_Push(int x, int y, int z, int dx, int dy)
{
	int i;

	i = 0;

	while ((i < box_count) && ((box[i].x/32 != x) || (box[i].y/32 != y) || (box[i].z != z)))
		i++;

	if (i == box_count)
		exit(7);

	if ((!box[i].dx) && (!box[i].dy) && (!box[i].dz))
	{
		box[i].dx = dx;
		box[i].dy = dy;
	}

	map[box[i].x / 32 + box[i].dx][box[i].y / 32 + box[i].dy][box[i].z / 8] = (RESERVE | box[i].type);
}

void Box_Move(void)
{
	int i;

	for (i = 0; i < box_count; i++)
	{
		if ((box[i].dx) || (box[i].dy) || (box[i].dz))
		{
			box[i].x +=box[i].dx;
			box[i].y +=box[i].dy;

			if ((box[i].x % 32 == 0) && (box[i].y % 32 == 0) && (!box[i].dz))
			{
				box[i].dx = 0;
				box[i].dy = 0;
				map[box[i].x / 32][box[i].y / 32][box[i].z / 8] = box[i].type;
			}

			if (box[i].dz)
			{
				box[i].z +=box[i].dz;

				if (!box[i].z)
				{
					box[i].dz = 0;
					map[box[i].x/32][box[i].y/32][0] = box[i].type;
					map[box[i].x/32][box[i].y/32][1] = 0;
				}
			}

			DirtyList(box[i].x, box[i].y, box[i].z, 32, 40, box[i].pic);
		}
	}
}

void Box_Fall(void)
{
	int i;

	for (i = 0; i < box_count; i++)
	{
		if ((Check_Can_Fall(box[i].x, box[i].y, box[i].z)) &&
			((ply[0].x/32 != box[i].x/32) || (ply[0].y/32 != box[i].y/32)) &&
			(((ply[0].x + 31)/32 != box[i].x/32) || (ply[0].y/32 != box[i].y/32)) &&
			((ply[0].x/32 != box[i].x/32) || ((ply[0].y + 31)/32 != box[i].y/32)) &&
			(((ply[0].x + 31)/32 != box[i].x/32) || ((ply[0].y + 31)/32 != box[i].y/32)))
		{
			box[i].dz = -1;

			map[box[i].x/32][box[i].y/32][1] = RESERVE;
			map[box[i].x/32][box[i].y/32][0] = RESERVE;
		}
	}
}
