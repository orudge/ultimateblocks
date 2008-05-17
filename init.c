/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>

#ifdef ENABLE_CDA
	#include <libcda.h>
#endif

#include <stdio.h>
#include "blocks3.h"
#include "musicdat.h"
#include "fps.h"
#include <string.h>

BITMAP *gfx, *back, *temp;
int B2Music;

int f_no;

void increment_time_counter(void)
{
	time_count++;
}

END_OF_FUNCTION(increment_time_counter);

void increment_fps_counter(void)
{
	fps_count++;
}

END_OF_FUNCTION(increment_fps_counter);

#ifndef DJGPP
void close_button_callback(void)
{
	close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)
#endif

int Remember_Mod_File(const char *fn, int a, void *b)
{
	if (f_no == MAX_MODS)
		return -1;

	safe_strcpy(mod[f_no].name, DIR_MAXNAME, _music_dir); //"./music/");
	safe_strcat(mod[f_no].name, DIR_MAXNAME, "/");
	safe_strcat(mod[f_no].name, DIR_MAXNAME, get_filename(fn));

	mod[f_no].type = (char) b;
	f_no++;

	return 0;
}

void Add_Datafile_Mod(const char *fn, const char id, const char type)
{
	if (f_no == MAX_MODS)
		return;

	safe_strcpy(mod[f_no].name, DIR_MAXNAME, fn);
	mod[f_no].type = type;
	mod[f_no].id = id;
	f_no++;

	return;
}


int LoadGraphicsPack(const char *fn, int a, void *b)
{
	char tmp_id_max[11];
	char tmp_id[MOTIF_ID_LEN];
	char graphics_str[MAX_PATH];
	int ver;

	if (num_motifs >= 9)
		return -1;

	// get filename of graphics pack
	motifs[num_motifs].filename = get_filename(fn);

	// check version == 4
	set_config_file(fn);
	ver = get_config_int("Blocks", "Version", 4);

	if (ver != 4)
		return 0;

	// allocate a temporary ID, but try to retrieve the real one from the config file
	sprintf(tmp_id_max, "%d", num_motifs);
	safe_strncpy(tmp_id, MOTIF_ID_LEN, tmp_id_max, MOTIF_ID_LEN);

	safe_strncpy(motifs[num_motifs].uid, MOTIF_ID_LEN, get_config_string("Blocks", "UID", tmp_id), 4);

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

	sprintf(graphics_str, "%s/%s", _graphics_dir, motifs[num_motifs].gfx_fn);

	if (exists(graphics_str))
		num_motifs++;

	return 0;
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

void UpdateGameSpeeds()
{
	int frames = 0;

	if (_game_speed == 0)
		frames = 100;
	else if (_game_speed == 1)
		frames = 150;
	else
		frames = 200;

	install_int_ex(increment_fps_counter, BPS_TO_TIMER(frames));

#ifdef MEASURE_FPS
	destroy_fps(fps);
	fps = create_fps(frames);
#endif
}
	

void Initialise(void)
{
	char graphics_str[MAX_PATH];
	char music_str[MAX_PATH];

	// Initialise Allegro
	allegro_init();
	install_keyboard();
	install_timer();
	install_mouse();

#ifdef ENABLE_CDA
	// Initialise CD player
	cd_init();
#endif

	// Initialise DUMB
	dumb_register_stdfiles();

	_config_file = strdup(find_resource_file(SETTINGS_DIR, "blocks4.cfg"));

	set_config_file(_config_file);
	B2Music = get_config_int("Sound", "B2Music", 1);

	mus_vol = get_config_int("Sound", "Music", 255);
	sfx_vol = get_config_int("Sound", "SFX", 255);
	cd_vol = get_config_int("Sound", "CD", 255);

	_game_speed = get_config_int("Game", "Speed", 1);

	if (_game_speed < 0)
		_game_speed = 0;
	else if (_game_speed > 2)
		_game_speed = 2;

	// Change screen resolutions
#ifndef MACOSX
	set_window_title("Ultimate Blocks");
#endif
#ifdef DJGPP
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
#else
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
#endif
#ifdef MACOSX
	set_window_title("Ultimate Blocks");
#endif
	set_display_switch_mode(SWITCH_BACKGROUND); // todo: may need to alter for fullscreen

	// Add close button handler

	close_button_pressed = FALSE;
#ifndef DJGPP
	LOCK_FUNCTION(close_button_callback);
	set_close_button_callback(close_button_callback);
#endif

	// Install timer

	LOCK_VARIABLE(time_count);
	LOCK_FUNCTION(increment_time_counter);

	install_int_ex(increment_time_counter, BPS_TO_TIMER(200));

	LOCK_VARIABLE(fps_count);
	LOCK_FUNCTION(increment_fps_counter);

	UpdateGameSpeeds();

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

	_graphics_dir = strdup(find_resource_file(GRAPHICS_DIR, NULL));
	sprintf(graphics_str, "%s/*.bgp", _graphics_dir);

	for_each_file_ex(graphics_str, 0, FA_DIREC | FA_LABEL, LoadGraphicsPack, 0);

	// Load fonts and title graphics
	fonts = load_encrypted_datafile(find_resource_file(APP_DIR, "fonts.dat"));

	if (fonts == NULL)
		report_error("\"fonts.dat\" not found or corrupt. Please reinstall Ultimate Blocks.");

	title_gfx = load_bitmap(find_resource_file(APP_DIR, "title256.bmp"), title_palette);

	if (title_gfx == NULL)
		report_error("\"title256.bmp\" not found or corrupt. Please reinstall Ultimate Blocks.");

	// Setup sound
	reserve_voices(64, -1);
	install_sound(DIGI_AUTODETECT, MIDI_NONE, "");
	//install_mod(40);

	set_volume(255, 0);
	//set_mod_volume(mus_vol);
#ifdef ENABLE_CDA
	cd_set_volume(cd_vol, cd_vol);
#endif

	sfx = load_encrypted_datafile(find_resource_file(APP_DIR, "sfx.dat"));

	if (sfx == NULL)
		report_error("\"sfx.dat\" not found or corrupt. Please reinstall Ultimate Blocks.");

	// add appropriate loaders for datafile
	dumb_register_dat_it_quick(DUMB_DAT_IT);
	dumb_register_dat_xm_quick(DUMB_DAT_XM);
	dumb_register_dat_s3m_quick(DUMB_DAT_S3M);
	dumb_register_dat_mod_quick(DUMB_DAT_MOD);

	music_dat = load_encrypted_datafile(find_resource_file(APP_DIR, "music.dat"));

	if (music_dat == NULL)
		report_error("\"music.dat\" not found or corrupt. Please reinstall Ultimate Blocks.");

	// Iterate through music
	f_no = 0;

	// Add music from datafiles
	Add_Datafile_Mod("deadlock.xm", MUSIC_DEADLOCK, MODTYPE_DAT_XM);
	Add_Datafile_Mod("fishtro.s3m", MUSIC_FISHTRO, MODTYPE_DAT_S3M);
	Add_Datafile_Mod("inside.s3m", MUSIC_INSIDE, MODTYPE_DAT_S3M);
	Add_Datafile_Mod("misthart.xm", MUSIC_MISTHART, MODTYPE_DAT_XM);

	_music_predefined = f_no;

	// Add any other files in music/
	_music_dir = strdup(find_resource_file(MUSIC_DIR, NULL));
	
	sprintf(music_str, "%s/*.mod", _music_dir);
	for_each_file_ex(music_str, 0, FA_DIREC | FA_LABEL, Remember_Mod_File, (void *) MODTYPE_MOD);

	sprintf(music_str, "%s/*.s3m", _music_dir);
	for_each_file_ex(music_str, 0, FA_DIREC | FA_LABEL, Remember_Mod_File, (void *) MODTYPE_S3M);

	sprintf(music_str, "%s/*.xm", _music_dir);
	for_each_file_ex(music_str, 0, FA_DIREC | FA_LABEL, Remember_Mod_File, (void *) MODTYPE_XM);

	sprintf(music_str, "%s/*.it", _music_dir);
	for_each_file_ex(music_str, 0, FA_DIREC | FA_LABEL, Remember_Mod_File, (void *) MODTYPE_IT);

	mod_last = f_no;
	mod_track = 0;

	Sort_Music_Files();

	// If we're not using the Blocks+-style music, we need to start playing the first track
	// If we are, it'll be done automatically when we change motif
	if (B2Music == 0)
		Play_MOD_Track(mod[mod_track].name, mod[mod_track].type, 0);
    
	// Change motif to default (Sunny)
	Change_Motif("SUNY");

	_map_dir = strdup(find_resource_file(MAP_DIR, NULL));
}

void Save_Config()
{
	// Save config file
	set_config_file(_config_file);

	set_config_int("Sound", "Music", mus_vol);
	set_config_int("Sound", "SFX", sfx_vol);
	set_config_int("Sound", "CD", cd_vol);

	set_config_int("Game", "Speed", _game_speed);

	free(_config_file);
	free(_music_dir);
	free(_graphics_dir);
	free(_map_dir);
}

inline DATAFILE * load_encrypted_datafile (const char *filename)
{
	DATAFILE *ret;

	packfile_password(DATAFILE_PASSWORD);
	ret = load_datafile(filename);
	packfile_password(NULL);

	return(ret);
}
