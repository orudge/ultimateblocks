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
 
#ifdef DO_GRAPHICS_LOG
 FILE *tmp;
#endif

 if (strcmp(m, motif) == 0) return;

#ifdef DO_GRAPHICS_LOG
 tmp = fopen("tmp.log", "at");
 fprintf(tmp, "\n\nMOTIF SELECT: %s\n", m);
#endif

/* if ((m < 1) || (m > 4)) m = SUNNY;

 if (m == SUNNY) gfx = load_bitmap("sunny256.bmp", pallete);
 if (m == CASTLE) gfx = load_bitmap("castl256.bmp", pallete);
 if (m == ROCKY) gfx = load_bitmap("rocky256.bmp", pallete);
 if (m == SNOWY) gfx = load_bitmap("snowy256.bmp", pallete);*/

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
       gfx = load_bitmap(motifs[i].gfx_fn, pallete); //motifs[i].gfx; //dat[0].dat;
//       palid = i;
//       pallete = (PALETTE *) motifs[i].pal;

       break;
    }*/

    if (strstr(m, motifs[i].uid) != NULL)// strcmp(m, motifs[i].uid) == 0)
    {
#ifdef DO_GRAPHICS_LOG
       fprintf(tmp, "IT'S THE ONE! :-)\n");
#endif
       gfx = load_bitmap(motifs[i].gfx_fn, pallete); //motifs[i].gfx; //dat[0].dat;

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
    gfx = load_bitmap("graphics/sunny.bmp", pallete);

    if (B2Music == 1)
    {
       destroy_mod(music);
       
       music = load_mod("music/deadlock.xm");
       play_mod(music, TRUE);
    }
 }

 strcpy(motif, m);

 blit(gfx, ply_pic[0][0], 0,  0, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][1], 32, 0, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][2], 64, 0, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past first blit block...\n");
#endif

 blit(gfx, ply_pic[0][3], 0,  40, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][4], 32, 40, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][5], 64, 40, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past second blit block...\n");
#endif

 blit(gfx, ply_pic[0][6], 0,  80, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][7], 32, 80, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][8], 64, 80, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past third blit block...\n");
#endif

 blit(gfx, ply_pic[0][9] , 0,  120, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][10], 32, 120, 0, 0, 32, 40);
 blit(gfx, ply_pic[0][11], 64, 120, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fourth blit block...\n");
#endif

 blit(gfx, ply_pic[1][0], 384,  0, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][1], 416, 0, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][2], 448, 0, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fifth blit block...\n");
#endif

 blit(gfx, ply_pic[1][3], 384,  40, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][4], 416, 40, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][5], 448, 40, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past sixth blit block...\n");
#endif

 blit(gfx, ply_pic[1][6], 384,  80, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][7], 416, 80, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][8], 448, 80, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past seventh blit block...\n");
#endif

 blit(gfx, ply_pic[1][9] , 384,  120, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][10], 416, 120, 0, 0, 32, 40);
 blit(gfx, ply_pic[1][11], 448, 120, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past eighth blit block...\n");
#endif

 blit(gfx, box_pic, 0, 160, 0, 0, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past last blit call after blocks...\n");
#endif

 token_pic = create_sub_bitmap(gfx, 32, 160, 32, 40);

 teleport_pic[0] = create_sub_bitmap(gfx, 64, 160, 32, 40);
 teleport_pic[1] = create_sub_bitmap(gfx, 96, 160, 32, 40);
 teleport_pic[2] = create_sub_bitmap(gfx, 128, 160, 32, 40);

 laser_pic[0] = create_sub_bitmap(gfx, 64, 240, 32, 40);
 laser_pic[1] = create_sub_bitmap(gfx, 96, 240, 32, 40);
 laser_pic[2] = create_sub_bitmap(gfx, 128, 240, 32, 40);
 laser_pic[3] = create_sub_bitmap(gfx, 160, 240, 32, 40);
 laser_pic[4] = create_sub_bitmap(gfx, 192, 240, 32, 40);

 mirror_pic[0] = create_sub_bitmap(gfx, 64, 200, 32, 40);
 mirror_pic[1] = create_sub_bitmap(gfx, 96, 200, 32, 40);

 beam_pic[0] = create_sub_bitmap(gfx, 128, 200, 32, 40);
 beam_pic[1] = create_sub_bitmap(gfx, 160, 200, 32, 40);
 beam_pic[3] = create_sub_bitmap(gfx, 192, 200, 32, 40);
 beam_pic[4] = create_sub_bitmap(gfx, 224, 200, 32, 40);
 beam_pic[5] = create_sub_bitmap(gfx, 256, 200, 32, 40);
 beam_pic[6] = create_sub_bitmap(gfx, 288, 200, 32, 40);
 beam_pic[2] = create_sub_bitmap(gfx, 320, 200, 32, 40);

 bomb_pic[0] = create_sub_bitmap(gfx, 192, 160, 32, 40);
 bomb_pic[1] = create_sub_bitmap(gfx, 160, 160, 32, 40);
 bomb_pic[2] = create_sub_bitmap(gfx, 352, 200, 32, 40);

 explode_pic[0] = create_sub_bitmap(gfx, 224, 160, 32, 40);
 explode_pic[1] = create_sub_bitmap(gfx, 256, 160, 32, 40);
 explode_pic[2] = create_sub_bitmap(gfx, 288, 160, 32, 40);
 explode_pic[3] = create_sub_bitmap(gfx, 320, 160, 32, 40);
 explode_pic[4] = create_sub_bitmap(gfx, 352, 160, 32, 40);
 explode_pic[5] = create_sub_bitmap(gfx, 384, 160, 32, 40);
 explode_pic[6] = create_sub_bitmap(gfx, 416, 160, 32, 40);
 explode_pic[7] = create_sub_bitmap(gfx, 448, 160, 32, 40);

 mon_pic[0][0]  = create_sub_bitmap(gfx,  96, 0, 32, 40);
 mon_pic[0][1]  = create_sub_bitmap(gfx, 128, 0, 32, 40);
 mon_pic[0][2]  = create_sub_bitmap(gfx, 160, 0, 32, 40);
 mon_pic[0][3]  = create_sub_bitmap(gfx,  96, 40, 32, 40);
 mon_pic[0][4]  = create_sub_bitmap(gfx, 128, 40, 32, 40);
 mon_pic[0][5]  = create_sub_bitmap(gfx, 160, 40, 32, 40);
 mon_pic[0][6]  = create_sub_bitmap(gfx,  96, 80, 32, 40);
 mon_pic[0][7]  = create_sub_bitmap(gfx, 128, 80, 32, 40);
 mon_pic[0][8]  = create_sub_bitmap(gfx, 160, 80, 32, 40);
 mon_pic[0][9]  = create_sub_bitmap(gfx,  96, 120, 32, 40);
 mon_pic[0][10] = create_sub_bitmap(gfx, 128, 120, 32, 40);
 mon_pic[0][11] = create_sub_bitmap(gfx, 160, 120, 32, 40);

 filter_pic = create_sub_bitmap(gfx, 224, 240, 32, 40);

 switches_pic[0] = create_sub_bitmap(gfx, 256, 240, 32, 40);
 switches_pic[1] = create_sub_bitmap(gfx, 288, 240, 32, 40);
 switches_pic[2] = create_sub_bitmap(gfx, 320, 240, 32, 40);

 door_pic[0] = create_sub_bitmap(gfx, 256, 280, 32, 40);
 door_pic[1] = create_sub_bitmap(gfx, 288, 280, 32, 40);
 door_pic[2] = create_sub_bitmap(gfx, 320, 280, 32, 40);

 fall_pic = create_sub_bitmap(gfx, 256, 320, 32, 40);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past create_sub_bitmap() calls...\n");
#endif

 blit(gfx, editor_icons, 0, 280, 0,   0, 32, 32);
 blit(box_pic, editor_icons, 0, 0,   32,  0, 32, 32);
 blit(ply_pic[0][0], editor_icons, 0, 0,   64,  0, 32, 32);
 blit(ply_pic[1][0], editor_icons, 0, 0,   96,  0, 32, 32);
 blit(token_pic, editor_icons, 0, 0,   128,  0, 32, 32);
 blit(teleport_pic[0], editor_icons, 0, 0,   160,  0, 32, 32);
 blit(teleport_pic[1], editor_icons, 0, 0,   192,  0, 32, 32);
 blit(teleport_pic[2], editor_icons, 0, 0,   224,  0, 32, 32);
 blit(laser_pic[0], editor_icons, 0, 0, 256, 0, 32, 32);
 blit(laser_pic[1], editor_icons, 0, 0, 288, 0, 32, 32);
 blit(laser_pic[2], editor_icons, 0, 0, 320, 0, 32, 32);
 blit(laser_pic[3], editor_icons, 0, 0, 352, 0, 32, 32);
 blit(laser_pic[4], editor_icons, 0, 0, 384, 0, 32, 32);
 blit(mirror_pic[0], editor_icons, 0, 0, 416, 0, 32, 32);
 blit(mirror_pic[1], editor_icons, 0, 0, 448, 0, 32, 32);
 blit(bomb_pic[0], editor_icons, 0, 0, 480, 0, 32, 32);
 blit(bomb_pic[1], editor_icons, 0, 0, 512, 0, 32, 32);
 blit(bomb_pic[2], editor_icons, 0, 0, 544, 0, 32, 32);
 blit(mon_pic[0][0], editor_icons, 0, 0, 576, 0, 32, 32);
 blit(mon_pic[0][0], editor_icons, 0, 0, 608, 0, 32, 32);
 blit(filter_pic, editor_icons, 0, 0, 640,   0, 32, 32);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past first BIG blit block...\n");
#endif

 blit(switches_pic[0], editor_icons, 0, 0, 672, 0, 32, 32);
 blit(switches_pic[1], editor_icons, 0, 0, 704, 0, 32, 32);
 blit(switches_pic[2], editor_icons, 0, 0, 736, 0, 32, 32);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past switches blit block...\n");
#endif

 blit(door_pic[0], editor_icons, 0, 0, 768, 0, 32, 32);
 blit(door_pic[1], editor_icons, 0, 0, 800, 0, 32, 32);
 blit(door_pic[2], editor_icons, 0, 0, 832, 0, 32, 32);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past doors blit block...\n");
#endif

 blit(fall_pic, editor_icons, 0, 0, 864, 0, 32, 32);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, "Got past fall blit block...\n");
#endif

#ifdef DO_GRAPHICS_LOG
 fclose(tmp);
#endif

 clear(screen);
 set_pallete(pallete);
}


