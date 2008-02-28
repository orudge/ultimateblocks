/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <string.h>
#include <jgmod.h>
#include "blocks3.h"

//#define DO_GRAPHICS_LOG

#ifdef DO_GRAPHICS_LOG
   #include <stdio.h>
#endif

void Change_Motif(char *m)
{
	int i;
	int palid;
	int old_block_width = 0, old_block_height = 0, old_block_depth = 0;
 
#ifdef DO_GRAPHICS_LOG
 FILE *tmp;
#endif

 if (strcmp(m, motif) == 0) return;

#ifdef DO_GRAPHICS_LOG
 tmp = fopen("tmp.log", "at");
 fprintf(tmp, "\n\nMOTIF SELECT: %s\n", m);
#endif

/* if ((m < 1) || (m > 4)) m = SUNNY;

 if (m == SUNNY) gfx = load_bitmap("sunny256.bmp", palette);
 if (m == CASTLE) gfx = load_bitmap("castl256.bmp", palette);
 if (m == ROCKY) gfx = load_bitmap("rocky256.bmp", palette);
 if (m == SNOWY) gfx = load_bitmap("snowy256.bmp", palette);*/

 for (i = 0; i < num_motifs; i++)
 {
#ifdef DO_GRAPHICS_LOG
    fprintf(tmp, " Motif %d (%s) - Special: %d\n", i, motifs[i].uid, motifs[i].special);
    fprintf(tmp, "   Filename: %s\n", motifs[i].gfx_fn);
#endif

/*    if (motifs[i].special == m)
    {
#ifdef DO_GRAPHICS_LOG
       fprintf(tmp, " Motif %d is the same as %d\n", m, motifs[i].special);
#endif
       gfx = load_bitmap(motifs[i].gfx_fn, palette); //motifs[i].gfx; //dat[0].dat;
//       palid = i;
//       palette = (PALETTE *) motifs[i].pal;

       break;
    }*/

    if (strstr(m, motifs[i].uid) != NULL)// strcmp(m, motifs[i].uid) == 0)
    {
#ifdef DO_GRAPHICS_LOG
       fprintf(tmp, "IT'S THE ONE! :-)\n");
#endif
       gfx = load_bitmap(motifs[i].gfx_fn, palette); //motifs[i].gfx; //dat[0].dat;

	   old_block_width = _block_width;
	   old_block_height = _block_height;
	   old_block_depth = _block_depth;

	   _block_width = motifs[i].block_width;
	   _block_height = motifs[i].block_height;
	   _block_depth = motifs[i].block_depth;

       if (B2Music == 1)
       {
          destroy_mod(music);
          
          music = load_mod(motifs[i].music_fn);
          play_mod(music, TRUE);
       }

       break;
    }
 }

 if (gfx == NULL)
 {
    gfx = load_bitmap("graphics/sunny.bmp", palette);

    if (B2Music == 1)
    {
       destroy_mod(music);
       
       music = load_mod("music/deadlock.xm");
       play_mod(music, TRUE);
    }
 }

 strcpy(motif, m);

 	// Recreate bitmaps if necessary

	if ((old_block_width != _block_width) || (old_block_height != _block_height) || (old_block_depth != _block_depth))
	{
		// first destroy the old ones
		for (i = 0; i < 12; i++)
		{
			destroy_bitmap(ply_pic[0][i]);
			destroy_bitmap(ply_pic[0][i]);
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

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past first blit block...\n");
#endif

 blit(gfx, ply_pic[0][3], 0,  _block_height_depth, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][4], _block_width, _block_height_depth, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][5], _block_width*2, _block_height_depth, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past second blit block...\n");
#endif

 blit(gfx, ply_pic[0][6], 0,  _block_height_depth*2, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][7], _block_width, _block_height_depth*2, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][8], _block_width*2, _block_height_depth*2, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past third blit block...\n");
#endif

 blit(gfx, ply_pic[0][9] , 0,  _block_height_depth*3, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][10], _block_width, _block_height_depth*3, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[0][11], _block_width*2, _block_height_depth*3, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fourth blit block...\n");
#endif

 blit(gfx, ply_pic[1][0], _block_width*12,  0, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][1], _block_width*13, 0, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][2], _block_width*14, 0, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fifth blit block...\n");
#endif

 blit(gfx, ply_pic[1][3], _block_width*12, _block_height_depth, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][4], _block_width*13, _block_height_depth, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][5], _block_width*14, _block_height_depth, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past sixth blit block...\n");
#endif

 blit(gfx, ply_pic[1][6], _block_width*12, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][7], _block_width*13, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][8], _block_width*14, _block_height_depth * 2, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past seventh blit block...\n");
#endif

 blit(gfx, ply_pic[1][9] , _block_width*12, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][10], _block_width*13, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);
 blit(gfx, ply_pic[1][11], _block_width*14, _block_height_depth * 3, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past eighth blit block...\n");
#endif

 blit(gfx, box_pic, 0, _block_width*5, 0, 0, _block_width, _block_height_depth);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past last blit call after blocks...\n");
#endif

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

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past create_sub_bitmap() calls...\n");
#endif

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

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past first BIG blit block...\n");
#endif

 blit(switches_pic[0], editor_icons, 0, 0, _block_width*21, 0, _block_width, _block_height);
 blit(switches_pic[1], editor_icons, 0, 0, _block_width*22, 0, _block_width, _block_height);
 blit(switches_pic[2], editor_icons, 0, 0, _block_width*23, 0, _block_width, _block_height);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past switches blit block...\n");
#endif

 blit(door_pic[0], editor_icons, 0, 0, _block_width*24, 0, _block_width, _block_height);
 blit(door_pic[1], editor_icons, 0, 0, _block_width*25, 0, _block_width, _block_height);
 blit(door_pic[2], editor_icons, 0, 0, _block_width*26, 0, _block_width, _block_height);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past doors blit block...\n");
#endif

 blit(fall_pic, editor_icons, 0, 0, _block_width*27, 0, _block_width, _block_height);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fall blit block...\n");
#endif

#ifdef DO_GRAPHICS_LOG
 fclose(tmp);
#endif

 clear(screen);
 set_palette(palette);
}


