/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <string.h>
#include "blocks3.h"

void Change_Motif(const char *m)
{
	char graphics_str[MAX_PATH];

	int i;
	int old_block_width = 0, old_block_height = 0, old_block_depth = 0;
 
	if (strcmp(m, motif) == 0)
		return;

	Reset_Particles();

 	old_block_width = _block_width;
	old_block_height = _block_height;
	old_block_depth = _block_depth;

	for (i = 0; i < num_motifs; i++)
	{
		if (strstr(m, motifs[i].uid) != NULL)
		{
			sprintf(graphics_str, "%s/%s", _graphics_dir, motifs[i].gfx_fn);
			gfx = load_bitmap(graphics_str, palette);

			_block_width = motifs[i].block_width;
			_block_height = motifs[i].block_height;
			_block_depth = motifs[i].block_depth;
			_particles = motifs[i].particles;

			if (B2Music == 1)
				Play_MOD_Track(motifs[i].music_fn, 0, 1);

			break;
		}
	}

	if (gfx == NULL)
	{
		sprintf(graphics_str, "%s/sunny.bmp", _graphics_dir);
	    gfx = load_bitmap(graphics_str, palette);

		_block_width = motifs[i].block_width;
		_block_height = motifs[i].block_height;
		_block_depth = motifs[i].block_depth;
		_particles = 0;

		if (B2Music == 1)
		{
			set_config_file(_config_file);		
			Play_MOD_Track(get_config_string("Sound", "InitialMusic", "deadlock.xm"), 0, 1);
		}
	}

	if (gfx == NULL)
	{
		report_error("No graphics packs available. Please reinstall Ultimate Blocks.");
		return;
	}

	safe_strcpy(motif, MOTIF_ID_LEN, m);

 	// Recreate bitmaps if necessary

	if ((old_block_width != _block_width) || (old_block_height != _block_height) || (old_block_depth != _block_depth))
	{
		// first destroy the old ones
		for (i = 0; i < 12; i++)
		{
			destroy_bitmap(ply_pic[0][i]);
			destroy_bitmap(ply_pic[1][i]);
		}

		destroy_bitmap(box_pic);
		destroy_bitmap(blank);
		destroy_bitmap(editor_icons);

		// Now create the new ones
		for (i = 0; i < 12; i++)
		{
			ply_pic[0][i] = create_bitmap(_block_width, _block_height_depth);
			ply_pic[1][i] = create_bitmap(_block_width, _block_height_depth);
		}

		box_pic = create_bitmap(_block_width, _block_height_depth);
		clear(box_pic);

		blank = create_bitmap(_block_width, _block_height_depth);
		clear(blank);

		editor_icons = create_bitmap(_block_width * 32, _block_height_depth);
		clear(editor_icons);
	}
	
	blit(gfx, ply_pic[0][0], 0,  0, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][1], _block_width, 0, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][2], _block_width*2, 0, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[0][3], 0,  _block_height_depth, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][4], _block_width, _block_height_depth, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][5], _block_width*2, _block_height_depth, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[0][6], 0,  _block_height_depth*2, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][7], _block_width, _block_height_depth*2, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][8], _block_width*2, _block_height_depth*2, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[0][9] , 0,  _block_height_depth*3, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][10], _block_width, _block_height_depth*3, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[0][11], _block_width*2, _block_height_depth*3, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[1][0], _block_width*12,  0, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][1], _block_width*13, 0, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][2], _block_width*14, 0, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[1][3], _block_width*12, _block_height_depth, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][4], _block_width*13, _block_height_depth, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][5], _block_width*14, _block_height_depth, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[1][6], _block_width*12, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][7], _block_width*13, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][8], _block_width*14, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);

	blit(gfx, ply_pic[1][9] , _block_width*12, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][10], _block_width*13, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);
	blit(gfx, ply_pic[1][11], _block_width*14, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);

	blit(gfx, box_pic, 0, _block_width*5, 0, 0, _block_width, _block_height_depth);

	token_pic = create_sub_bitmap(gfx, _block_width, _block_height_depth * 4, _block_width, _block_height_depth);

	teleport_pic[0] = create_sub_bitmap(gfx, _block_width * 2, _block_height_depth * 4, _block_width, _block_height_depth);
	teleport_pic[1] = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth * 4, _block_width, _block_height_depth);
	teleport_pic[2] = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth * 4, _block_width, _block_height_depth);

	laser_pic[0] = create_sub_bitmap(gfx, _block_width * 2, _block_height_depth * 6, _block_width, _block_height_depth);
	laser_pic[1] = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth * 6, _block_width, _block_height_depth);
	laser_pic[2] = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth * 6, _block_width, _block_height_depth);
	laser_pic[3] = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth * 6, _block_width, _block_height_depth);
	laser_pic[4] = create_sub_bitmap(gfx, _block_width * 6, _block_height_depth * 6, _block_width, _block_height_depth);

	mirror_pic[0] = create_sub_bitmap(gfx, _block_width * 2, _block_height_depth * 5, _block_width, _block_height_depth);
	mirror_pic[1] = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth * 5, _block_width, _block_height_depth);

	beam_pic[0] = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[1] = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[3] = create_sub_bitmap(gfx, _block_width * 6, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[4] = create_sub_bitmap(gfx, _block_width * 7, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[5] = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[6] = create_sub_bitmap(gfx, _block_width * 9, _block_height_depth * 5, _block_width, _block_height_depth);
	beam_pic[2] = create_sub_bitmap(gfx, _block_width * 10, _block_height_depth * 5, _block_width, _block_height_depth);

	bomb_pic[0] = create_sub_bitmap(gfx, _block_width * 6, _block_height_depth * 4, _block_width, _block_height_depth);
	bomb_pic[1] = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth * 4, _block_width, _block_height_depth);
	bomb_pic[2] = create_sub_bitmap(gfx, _block_width * 11, _block_height_depth * 5, _block_width, _block_height_depth);

	explode_pic[0] = create_sub_bitmap(gfx, _block_width * 7, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[1] = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[2] = create_sub_bitmap(gfx, _block_width * 9, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[3] = create_sub_bitmap(gfx, _block_width * 10, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[4] = create_sub_bitmap(gfx, _block_width * 11, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[5] = create_sub_bitmap(gfx, _block_width * 12, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[6] = create_sub_bitmap(gfx, _block_width * 13, _block_height_depth * 4, _block_width, _block_height_depth);
	explode_pic[7] = create_sub_bitmap(gfx, _block_width * 14, _block_height_depth * 4, _block_width, _block_height_depth);

	mon_pic[0][0]  = create_sub_bitmap(gfx, _block_width * 3, 0, _block_width, _block_height_depth);
	mon_pic[0][1]  = create_sub_bitmap(gfx, _block_width * 4, 0, _block_width, _block_height_depth);
	mon_pic[0][2]  = create_sub_bitmap(gfx, _block_width * 5, 0, _block_width, _block_height_depth);
	mon_pic[0][3]  = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth, _block_width, _block_height_depth);
	mon_pic[0][4]  = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth, _block_width, _block_height_depth);
	mon_pic[0][5]  = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth, _block_width, _block_height_depth);
	mon_pic[0][6]  = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth * 2, _block_width, _block_height_depth);
	mon_pic[0][7]  = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth * 2, _block_width, _block_height_depth);
	mon_pic[0][8]  = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth * 2, _block_width, _block_height_depth);
	mon_pic[0][9]  = create_sub_bitmap(gfx, _block_width * 3, _block_height_depth * 3, _block_width, _block_height_depth);
	mon_pic[0][10] = create_sub_bitmap(gfx, _block_width * 4, _block_height_depth * 3, _block_width, _block_height_depth);
	mon_pic[0][11] = create_sub_bitmap(gfx, _block_width * 5, _block_height_depth * 3, _block_width, _block_height_depth);

	filter_pic = create_sub_bitmap(gfx, _block_width * 7, _block_height_depth * 6, _block_width, _block_height_depth);

	switches_pic[0] = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 6, _block_width, _block_height_depth);
	switches_pic[1] = create_sub_bitmap(gfx, _block_width * 9, _block_height_depth * 6, _block_width, _block_height_depth);
	switches_pic[2] = create_sub_bitmap(gfx, _block_width * 10, _block_height_depth * 6, _block_width, _block_height_depth);

	door_pic[0] = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 7, _block_width, _block_height_depth);
	door_pic[1] = create_sub_bitmap(gfx, _block_width * 9, _block_height_depth * 7, _block_width, _block_height_depth);
	door_pic[2] = create_sub_bitmap(gfx, _block_width * 10, _block_height_depth * 7, _block_width, _block_height_depth);

	fall_pic = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 8, _block_width, _block_height_depth);

	tick_pic[0] = create_sub_bitmap(gfx, _block_width * 8, _block_height_depth * 9, TICK_PIC_WIDTH, TICK_PIC_HEIGHT);
	tick_pic[1] = create_sub_bitmap(gfx, _block_width * 9, _block_height_depth * 9, _block_width, _block_height);

	blit(gfx, editor_icons, 0, _block_height_depth * 7, 0,   0, _block_width, _block_height);
	blit(box_pic, editor_icons, 0, 0,   _block_width,  0, _block_width, _block_height);
	blit(ply_pic[0][0], editor_icons, 0, 0,   _block_width*2,  0, _block_width, _block_height);
	blit(ply_pic[1][0], editor_icons, 0, 0,   _block_width*3,  0, _block_width, _block_height);
	blit(token_pic, editor_icons, 0, 0,   _block_width*4,  0, _block_width, _block_height);
	blit(teleport_pic[0], editor_icons, 0, 0,   _block_width*5,  0, _block_width, _block_height);
	blit(teleport_pic[1], editor_icons, 0, 0,   _block_width*6,  0, _block_width, _block_height);
	blit(teleport_pic[2], editor_icons, 0, 0,   _block_width*7,  0, _block_width, _block_height);
	blit(laser_pic[0], editor_icons, 0, 0, _block_width*8, 0, _block_width, _block_height);
	blit(laser_pic[1], editor_icons, 0, 0, _block_width*9, 0, _block_width, _block_height);
	blit(laser_pic[2], editor_icons, 0, 0, _block_width*10, 0, _block_width, _block_height);
	blit(laser_pic[3], editor_icons, 0, 0, _block_width*11, 0, _block_width, _block_height);
	blit(laser_pic[4], editor_icons, 0, 0, _block_width*12, 0, _block_width, _block_height);
	blit(mirror_pic[0], editor_icons, 0, 0, _block_width*13, 0, _block_width, _block_height);
	blit(mirror_pic[1], editor_icons, 0, 0, _block_width*14, 0, _block_width, _block_height);
	blit(bomb_pic[0], editor_icons, 0, 0, _block_width*15, 0, _block_width, _block_height);
	blit(bomb_pic[1], editor_icons, 0, 0, _block_width*16, 0, _block_width, _block_height);
	blit(bomb_pic[2], editor_icons, 0, 0, _block_width*17, 0, _block_width, _block_height);
	blit(mon_pic[0][0], editor_icons, 0, 0, _block_width*18, 0, _block_width, _block_height);
	blit(mon_pic[0][0], editor_icons, 0, 0, _block_width*19, 0, _block_width, _block_height);
	blit(filter_pic, editor_icons, 0, 0, _block_width*20,   0, _block_width, _block_height);

	blit(switches_pic[0], editor_icons, 0, 0, _block_width*21, 0, _block_width, _block_height);
	blit(switches_pic[1], editor_icons, 0, 0, _block_width*22, 0, _block_width, _block_height);
	blit(switches_pic[2], editor_icons, 0, 0, _block_width*23, 0, _block_width, _block_height);

	blit(door_pic[0], editor_icons, 0, 0, _block_width*24, 0, _block_width, _block_height);
	blit(door_pic[1], editor_icons, 0, 0, _block_width*25, 0, _block_width, _block_height);
	blit(door_pic[2], editor_icons, 0, 0, _block_width*26, 0, _block_width, _block_height);

	blit(fall_pic, editor_icons, 0, 0, _block_width*27, 0, _block_width, _block_height);

	clear(screen);
	set_palette(palette);
}


