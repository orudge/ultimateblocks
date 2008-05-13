/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <allegro.h>
#include "blocks3.h"

DUH *music;
char music_unload;
AL_DUH_PLAYER *mod_player;

DATAFILE *fonts, *sfx, *music_dat;

char login[13];



BITMAP *ply_pic[2][12];
BITMAP *mon_pic[5][12];

BITMAP *temp_particle;

BITMAP *title_gfx;
PALETTE title_palette;

BITMAP *box_pic, *bomb_pic[3];
BITMAP *token_pic;
BITMAP *teleport_pic[3];
BITMAP *blank;

BITMAP *laser_pic[5];
BITMAP *beam_pic[7];
BITMAP *mirror_pic[2];

BITMAP *explode_pic[8];

BITMAP *filter_pic;

BITMAP *door_pic[3];
BITMAP *switches_pic[3];

BITMAP *editor_icons;
BITMAP *fall_pic;

BITMAP *dark[5];

PALETTE palette;

COLOR_MAP trans_table;
COLOR_MAP light_table;

int _block_width;
int _block_height;
int _block_depth;



int time_count;
int game_type;
int game_exit;
int lev;
int mus_vol, sfx_vol, cd_vol;



int map[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
int maps[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH][101];
char map_motif[100][MOTIF_ID_LEN];
int map2[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
int map_laser[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
int map_door[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
int map_done[100];
int no_ply;
int mod_track, mod_last;
char motif[MOTIF_ID_LEN];

char map_save[20];



PLY_DEF ply[2];

DIRTY_DEF dirty[MAX_DIRTY], dirty_sort[MAX_DIRTY];
int dirty_count;


BOX_DEF box[468];
uint16 box_count;


EXPLODE_DEF explode[468];
uint16 explode_count;


MENU_DEF menu[4], menu_title[4];

DIR_DEF dir[MAX_DIRS], mod[MAX_MODS];

MON_DEF mon[100];
int mon_count;

SWITCH_DEF switches[60];

DOOR_DEF door[60];

int switches_count;
int door_count;

SOUND_DEF fx[MAX_SOUND_EFFECTS];

int fx_count;

UNDO_DEF undo[20];
int undo_count;



char _particles;

MOTIF motifs[20];
int num_motifs;


PARTICLE_DEF part[MAX_PARTICLES];
int part_count;


int _redraw_status = 1;


volatile int close_button_pressed;

int _level_undos;
