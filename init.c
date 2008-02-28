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

	if (num_motifs >= 9)
		return;

	// get filename of graphics pack
	motifs[num_motifs].filename = get_filename(fn);

	// check version == 4
	set_config_file(fn);
	ver = get_config_int("Blocks", "Version", 4);

	if (ver != 4)
		return;

	// allocate a temporary ID, but try to retrieve the real one from the config file
	sprintf(tmp_id, "%d", num_motifs);
	strncpy(motifs[num_motifs].uid, get_config_string("Blocks", "UID", tmp_id), 4);

	// update motifs table with appropriate details
	motifs[num_motifs].uid[5] = 0;
	motifs[num_motifs].title = strdup(get_config_string("Blocks", "Title", get_filename(fn)));
	motifs[num_motifs].gfx_fn = strdup(get_config_string("Blocks", "Filename", ""));
	motifs[num_motifs].music_fn = strdup(get_config_string("Blocks", "Music", ""));
	motifs[num_motifs].special = get_config_int("Blocks", "Special", -1);
	motifs[num_motifs].block_width = get_config_int("Blocks", "BlockWidth", 32);
	motifs[num_motifs].block_height = get_config_int("Blocks", "BlockHeight", 32);
	motifs[num_motifs].block_depth = get_config_int("Blocks", "BlockDepth", 8);
	num_motifs++;
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

	memset(ply_pic, 0, sizeof(ply_pic));
	box_pic = blank = editor_icons = NULL;

	temp = create_bitmap(SCREEN_W, SCREEN_H);
	back = create_bitmap(SCREEN_W, SCREEN_H);

	// Iterate through motifs

	num_motifs = 0;
	for_each_file("./graphics/*.bgp", FA_RDONLY | FA_ARCH, LoadGraphicsPack, 0);

	// Load fonts and title graphics
	fonts = load_datafile("fonts.dat");
	title_gfx = load_bitmap("title256.bmp", title_palette);

	// Setup sound
	reserve_voices(64, -1);
	install_sound(DIGI_AUTODETECT, MIDI_NONE, "");
	install_mod(40);

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

	// If we're not using the Blocks+-style music, we need to start playing the first track
	// If we are, it'll be done automatically when we change motif
	if (B2Music == 0)
	{
		music = load_mod(mod[mod_track].name);
		play_mod(music, TRUE);
	}
    
	// Change motif to default (Sunny)
	Change_Motif("SUNY");
}
