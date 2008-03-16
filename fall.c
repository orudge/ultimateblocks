/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

void Check_Fall(int p_no)
{
	if (map[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] == FALL)
		map[ply[p_no].x/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] = FALL_PRIME;

	if (map[(ply[p_no].x + (_block_width-1))/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] == FALL)
		map[(ply[p_no].x + (_block_width-1))/_block_width][ply[p_no].y/_block_height][ply[p_no].z/_block_depth] = FALL_PRIME;

	if (map[ply[p_no].x/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth] == FALL)
		map[ply[p_no].x/_block_width][(ply[p_no].y + (_block_height-1))/_block_height][ply[p_no].z/_block_depth] = FALL_PRIME;

	if (map[(ply[p_no].x + (_block_width-1))/_block_width][(ply[p_no].y + _block_height)/_block_height][ply[p_no].z/_block_depth] == FALL)
		map[(ply[p_no].x + (_block_width-1))/_block_width][(ply[p_no].y + _block_height)/_block_height][ply[p_no].z/_block_depth] = FALL_PRIME;
}

void Check_Fall_Prime(void)
{
	int i, j, k;

	for (k = 0; k < 2; k++)
	{
		for (j = 0; j < 15; j++)
		{
			for (i = 0; i < 20; i++)
			{
				if ((map[i][j][k] == FALL_PRIME) && (!Player_On_Fall(i, j)))
				{
					map[i][j][k] = SOLID;
					DirtyList(i*_block_width - (_block_width/2), j*_block_height - (_block_height/2), k*_block_depth, _block_width, _block_height+_block_depth, blank);
					DirtyList(i*_block_width + (_block_width/2), j*_block_height - (_block_height/2), k*_block_depth, _block_width, _block_height+_block_depth, blank);
					DirtyList(i*_block_width - (_block_width/2), j*_block_height + (_block_height/2), k*_block_depth, _block_width, _block_height+_block_depth, blank);
					DirtyList(i*_block_width + (_block_width/2), j*_block_height + (_block_height/2), k*_block_depth, _block_width, _block_height+_block_depth, blank);

					Add_Sound(i, SFX_FALL);
				}
			}
		}
	}
}

int Player_On_Fall(int x, int y)
{
	if ((ply[0].x/_block_width == x) && (ply[0].y/_block_height == y)) return 1;
	if (((ply[0].x + (_block_width-1))/_block_width == x) && (ply[0].y/_block_height == y)) return 1;
	if ((ply[0].x/_block_width == x) && ((ply[0].y + (_block_height-1))/_block_height == y)) return 1;
	if (((ply[0].x + (_block_width-1))/_block_width == x) && ((ply[0].y + (_block_height-1))/_block_height == y)) return 1;

	if ((ply[1].x/_block_width == x) && (ply[1].y/_block_height == y)) return 1;
	if (((ply[1].x + (_block_width-1))/_block_width == x) && (ply[1].y/_block_height == y)) return 1;
	if ((ply[1].x/_block_width == x) && ((ply[1].y + (_block_height-1))/_block_height == y)) return 1;
	if (((ply[1].x + (_block_width-1))/_block_width == x) && ((ply[1].y + (_block_height-1))/_block_height == y)) return 1;

	return 0;
}
