/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

void Box_Push(int x, int y, int z, int dx, int dy)
{
	int i;

	i = 0;

	while ((i < box_count) && ((box[i].x / _block_width != x) || (box[i].y / _block_height != y) || (box[i].z != z)))
		i++;

	if (i == box_count)
		exit(7);

	if ((!box[i].dx) && (!box[i].dy) && (!box[i].dz))
	{
		box[i].dx = dx;
		box[i].dy = dy;
	}

	map[box[i].x / _block_width + box[i].dx][box[i].y / _block_height + box[i].dy][box[i].z / _block_depth] = (RESERVE | box[i].type);
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

			if ((box[i].x % _block_width == 0) && (box[i].y % _block_height == 0) && (!box[i].dz))
			{
				box[i].dx = 0;
				box[i].dy = 0;
				map[box[i].x / _block_width][box[i].y / _block_height][box[i].z / _block_depth] = box[i].type;
			}

			if (box[i].dz)
			{
				box[i].z +=box[i].dz;

				if (!box[i].z)
				{
					box[i].dz = 0;
					map[box[i].x/_block_width][box[i].y/_block_height][0] = box[i].type;
					map[box[i].x/_block_width][box[i].y/_block_height][1] = 0;
				}
			}

			DirtyList(box[i].x, box[i].y, box[i].z, _block_width, _block_height + _block_depth, box[i].pic);
		}
	}
}

void Box_Fall(void)
{
	int i;

	for (i = 0; i < box_count; i++)
	{
		if ((Check_Can_Fall(box[i].x, box[i].y, box[i].z)) &&
			((ply[0].x / _block_width != box[i].x / _block_width) || (ply[0].y / _block_height != box[i].y / _block_height)) &&
			(((ply[0].x + (_block_width-1)) / _block_width != box[i].x / _block_width) || (ply[0].y/_block_height != box[i].y/_block_height)) &&
			((ply[0].x / _block_width != box[i].x / _block_width) || ((ply[0].y + (_block_height-1))/_block_height != box[i].y/_block_height)) &&
			(((ply[0].x + (_block_width-1))/_block_width != box[i].x/_block_width) || ((ply[0].y + (_block_height-1))/_block_height != box[i].y/_block_height)))
		{
			box[i].dz = -1;

			map[box[i].x/_block_width][box[i].y/_block_height][1] = RESERVE;
			map[box[i].x/_block_width][box[i].y/_block_height][0] = RESERVE;
		}
	}
}
