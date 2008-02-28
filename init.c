/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <jgmod.h>
#include <libcda.h>
#include "blocks3.h"
#include <string.h>

BITMAP *gfx, *back, *temp;
int B2Music;

//#define DO_GRAPHICS_LOG

#ifdef DO_GRAPHICS_LOG
   #include <stdio.h>
#endif

int f_no;

void increment_time_counter()
{
 time_count++;
}

END_OF_FUNCTION(increment_time_counter);

void Remember_Mod_File(char *fn, int a, int b)
{
 strcpy(mod[f_no].name, "./music/");
 strcat(mod[f_no].name, get_filename(fn));
 f_no++;
}

void LoadGraphicsPack(char *fn, int a, int b)
{
 char tmp_id[4];
 char *tmp_fn;
 char *tmp_ptr;
 int ver;
#ifdef DO_GRAPHICS_LOG
 FILE *tmp;
#endif

 if (num_motifs >= 9)
    return;

 motifs[num_motifs].filename = get_filename(fn);
 //strcpy(motifs[num_motifs].filename, get_filename(fn));
// sprintf(cfg_name, "%s#INFO", fn);

#ifdef DO_GRAPHICS_LOG
 tmp = fopen("tmpinit.log", "at");
 fprintf(tmp, "------------------------------------------\n");
 fprintf(tmp, "%s - %d\n", fn, num_motifs);
#endif

 set_config_file(fn);
 ver = get_config_int("Blocks", "Version", 4);

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, " Ver: %d\n", ver);
#endif

 if (ver > 4)
    return;

// motifs[num_motifs].dat = load_datafile(fn);

// if (motifs[num_motifs].dat == NULL)
//    return;

 sprintf(tmp_id, "%d", num_motifs);

// sprintf(motifs[num_motifs].gfx_fn, "%s#0", fn);
/* tmp_fn = get_config_string("Blocks", "FN", "");
 sprintf(motifs[num_motifs].gfx_fn, "./graphics/%s", tmp_fn);*/
// motifs[num_motifs].gfx_fn = (char *) get_config_string("Blocks", "FN", "");
// motifs[num_motifs].gfx = load_bitmap(cfg_name, motifs[num_motifs].pal); //(BITMAP *) motifs[num_motifs].dat[0].dat;
// motifs[num_motifs].pal = (RGB *) motifs[num_motifs].dat[1].dat;
 strcpy(motifs[num_motifs].uid, get_config_string("Blocks", "UID", tmp_id));
 motifs[num_motifs].uid[5] = 0;
 motifs[num_motifs].title = strdup(get_config_string("Blocks", "Title", get_filename(fn)));
 motifs[num_motifs].gfx_fn = strdup(get_config_string("Blocks", "Filename", ""));
 motifs[num_motifs].music_fn = strdup(get_config_string("Blocks", "Music", ""));
 motifs[num_motifs].special = get_config_int("Blocks", "Special", -1);
 num_motifs++;

#ifdef DO_GRAPHICS_LOG
 fprintf(tmp, " UID: %s  Title: %s\n Special: %d\n", motifs[num_motifs-1].uid, motifs[num_motifs-1].title, motifs[num_motifs-1].special);
 fprintf(tmp, " Filename: %s (Direct: %s)\n", motifs[num_motifs-1].gfx_fn, get_config_string("Blocks", "FN", ""));
 fclose(tmp);
#endif
}

// not an init routine, but put in here because it's next to load graphics
void UnloadGraphics()
{
/*	int i;

	for (i = 0; i < num_motifs+1; i++)
	{
		destroy_bitmap(motifs[num_motifs].gfx);
//		unload_datafile(motifs[num_motifs].dat);
	}*/
}

void Initialise(void)
{
	int i;

	// Initialise Allegro
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

	// Initialise CD player
	cd_init();

	set_config_file("blocks4.cfg");
	B2Music = get_config_int("Blocks4", "B2Music", 1);

	// Change screen resolution
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

	// Install timer

	LOCK_VARIABLE(time_count);
	LOCK_FUNCTION(increment_time_counter);

	install_int_ex(increment_time_counter, BPS_TO_TIMER(200));

	// Create translucency and light tables

	create_trans_table(&trans_table, palette, 50, 50, 50, NULL);
	create_light_table(&light_table, palette, 0, 0, 0, NULL);
	color_map = &light_table;

	// Create bitmaps

	for (i = 0; i < 12; i++)
	{
		ply_pic[0][i] = create_bitmap(32, 40);
		ply_pic[1][i] = create_bitmap(32, 40);
	}

	box_pic = create_bitmap(32, 40);
	clear(box_pic);

	blank = create_bitmap(32, 40);
	clear(blank);

	editor_icons = create_bitmap(1024, 32);
	clear(editor_icons);

	temp = create_bitmap(640, 480);
	back = create_bitmap(640, 480);

	// Iterate through motifs

	num_motifs = 0;
	for_each_file("./graphics/*.bgp", FA_RDONLY | FA_ARCH, LoadGraphicsPack, 0);

	// Load fonts and title graphics
	fonts = load_datafile("fonts.dat");
	title_gfx = load_bitmap("title256.bmp", title_palette);

	// Setup sound
	reserve_voices(20, -1);
	install_sound(DIGI_AUTODETECT, MIDI_NONE, "");
	install_mod(24);

	set_mod_volume(255);
	set_volume(255, 0);
	sfx = load_datafile("sfx.dat");

	mus_vol = 255;
	sfx_vol = 255;
	cd_vol = 255;

	// Iterate through music
	f_no = 0;
	for_each_file("./music/*.mod", FA_RDONLY | FA_ARCH, Remember_Mod_File, 0);
	for_each_file("./music/*.s3m", FA_RDONLY | FA_ARCH, Remember_Mod_File, 0);
	for_each_file("./music/*.xm", FA_RDONLY | FA_ARCH, Remember_Mod_File, 0);

	mod_last = f_no;
	mod_track = 0;

	// Use Blocks+-style music?
	if (B2Music == 1)
	{
/*		set_config_file("blocks4.cfg");
		music = load_mod(get_config_string("Blocks4", "InitialMusic", mod[mod_track].name));*/
	}
	else
		music = load_mod(mod[mod_track].name);
    
	// Start music and change motif to default (Sunny)
	play_mod(music, TRUE);
	Change_Motif("SUNY");
}
