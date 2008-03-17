/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include <libcda.h>
#include <stdio.h>
#include "blocks3.h"
#include <string.h>

BITMAP *gfx, *back, *temp;
int B2Music;

int f_no;

void increment_time_counter(void)
{
	time_count++;
}

END_OF_FUNCTION(increment_time_counter);

void close_button_callback(void)
{
	close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

int Remember_Mod_File(char *fn, int a, void *b)
{
	if (f_no == MAX_MODS)
		return -1;

	strcpy(mod[f_no].name, "./music/");
	strcat(mod[f_no].name, get_filename(fn));
	mod[f_no].type = b;
	f_no++;

	return 0;
}

void LoadGraphicsPack(char *fn, int a, int b)
{
	char tmp_id[4];
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
#if 0
	motifs[num_motifs].particles = get_config_int("Blocks", "Particles", 0);
#else
	motifs[num_motifs].particles = 0;
#endif
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
	// Initialise Allegro
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

	// Initialise CD player
	cd_init();

	// Initialise DUMB
	dumb_register_stdfiles();

	set_config_file("blocks4.cfg");
	B2Music = get_config_int("Sound", "B2Music", 1);

	mus_vol = get_config_int("Sound", "Music", 255);
	sfx_vol = get_config_int("Sound", "SFX", 255);
	cd_vol = get_config_int("Sound", "CD", 255);

	// Change screen resolutions
	set_window_title("Ultimate Blocks");
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	set_display_switch_mode(SWITCH_BACKGROUND); // todo: may need to alter for fullscreen

	// Add close button handler

	close_button_pressed = FALSE;
	LOCK_FUNCTION(close_button_callback);
	set_close_button_callback(close_button_callback);

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

	temp_particle = create_bitmap(SCREEN_W, SCREEN_H);
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
	//install_mod(40);

	set_volume(255, 0);
	//set_mod_volume(mus_vol);
	cd_set_volume(cd_vol, cd_vol);

	sfx = load_datafile("sfx.dat");

	// Iterate through music
	f_no = 0;
	for_each_file_ex("./music/*.mod", 0, FA_DIREC | FA_LABEL, Remember_Mod_File, MODTYPE_MOD);
	for_each_file_ex("./music/*.s3m", 0, FA_DIREC | FA_LABEL, Remember_Mod_File, MODTYPE_S3M);
	for_each_file_ex("./music/*.xm", 0, FA_DIREC | FA_LABEL, Remember_Mod_File, MODTYPE_XM);
	for_each_file_ex("./music/*.it", 0, FA_DIREC | FA_LABEL, Remember_Mod_File, MODTYPE_IT);

	mod_last = f_no;
	mod_track = 0;

	// If we're not using the Blocks+-style music, we need to start playing the first track
	// If we are, it'll be done automatically when we change motif
	if (B2Music == 0)
		Play_MOD_Track(mod[mod_track].name, mod[mod_track].type, 1);
    
	// Change motif to default (Sunny)
	Change_Motif("SUNY");
}

void Save_Config()
{
	// Save config file
	set_config_file("blocks4.cfg");

	set_config_int("Sound", "Music", mus_vol);
	set_config_int("Sound", "SFX", sfx_vol);
	set_config_int("Sound", "CD", cd_vol);
}
