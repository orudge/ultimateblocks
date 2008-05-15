/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

#include <aldumb.h>

#include "b3types.h"
#include "fps.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
	#ifndef WIN32	
		#define WIN32
	#endif

	#define strdup	_strdup
	#define strlwr	_strlwr
#endif

#ifndef MAX_PATH
	#define MAX_PATH		200
#endif

#define OLD_SOLID           1
#define OLD_BOX             2
#define OLD_TOKEN           3
#define OLD_FALL            4
#define OLD_AUDREY          9
#define OLD_AN              8
#define OLD_LASER_4WAY      56
#define OLD_LASER_UP        57
#define OLD_LASER_LEFT      58
#define OLD_LASER_DOWN      59
#define OLD_LASER_RIGHT     60
#define OLD_MIRROR_MINUS    61
#define OLD_MIRROR_PLUS     62
#define OLD_MON1            6
#define OLD_MON2            20
#define OLD_BOMB            7
#define OLD_BOMB2           10
#define OLD_EXPLODE         11
#define OLD_DOOR1           12
#define OLD_DOOR2           13
#define OLD_DOOR3           14
#define OLD_SWITCH1         15
#define OLD_SWITCH2         16
#define OLD_SWITCH3         17
#define OLD_DOOR_CLOSE      18
#define OLD_BOX_SLIDE       19
#define OLD_BOX_FALL        21
#define OLD_REVERSE         22

#define SFX_CLICK                        0        /* SAMP */
#define SFX_DOOR                         1        /* SAMP */
#define SFX_EXPLODE                      2        /* SAMP */
#define SFX_FALL                         3        /* SAMP */
#define SFX_GETRED                       4        /* SAMP */
#define SFX_INQUIRY                      5        /* SAMP */
#define SFX_POPON                        6        /* SAMP */
#define SFX_ROAR                         7        /* SAMP */
#define SFX_WELLDONE                     8        /* SAMP */
#define SFX_YEAH                         9        /* SAMP */

#define EDITOR_GAME     1
#define PLAYER_GAME     2

#define RESERVE0        8
#define FULL            32
#define PUSH            64
#define MONSTER         128
#define PICKUP          256
#define BLOWUP          512
#define TELEPORT        1024
#define LASER           2048
#define MIRROR          4096
#define BOMB            8192
#define DOOR            16384
#define SWITCH          32768

#define FALL            4
#define FALL_PRIME      5

#define SOLID           48      /* 16, FULL */

#define RESERVE         52      /* 4, 16, FULL */

#define BOX             609     /* 1, PUSH, BLOWUP, FULL */
#define TOKEN           257     /* 1, PICKUP */
#define AUDREY          1000
#define AN              1001

#define RED             1
#define BLUE            2
#define GREEN           4

#define RED_TELEPORT    1025    /* RED, TELEPORT */
#define BLUE_TELEPORT   1026    /* BLUE, TELEPORT */
#define GREEN_TELEPORT  1028    /* GREEN, TELEPORT */

#define OPEN            8
#define CLOSE           16

#define RED_DOOR_OPEN   16393   /* RED, DOOR, OPEN */
#define BLUE_DOOR_OPEN  16394
#define GREEN_DOOR_OPEN 16396

#define RED_DOOR_CLOSE    16433 /* RED, DOOR, CLOSE, FULL */
#define BLUE_DOOR_CLOSE   16434
#define GREEN_DOOR_CLOSE  16436

#define RED_SWITCH      32769
#define BLUE_SWITCH     32770
#define GREEN_SWITCH    32772

#define UP              1
#define RIGHT           2
#define DOWN            4
#define LEFT            8

#define LASER_UP        2145    /* LASER, UP, PUSH, FULL */
#define LASER_DOWN      2148    /* LASER, DOWN, PUSH, FULL */
#define LASER_LEFT      2152    /* LASER, LEFT, PUSH, FULL */
#define LASER_RIGHT     2146    /* LASER, RIGHT, PUSH, FULL */
#define LASER_4WAY      2159

#define MIRROR_PLUS     4161    /* 1, MIRROR, PUSH */
#define MIRROR_MINUS    4162    /* 2, MIRROR, PUSH */

#define BOMB1           8801    /* 1, BOMB, PUSH, BLOWUP, FULL */
#define BOMB2           8802    /* 2, BOMB, PUSH, BLOWUP, FULL */
#define BOMB3           8804    /* 4, BOMB, PUSH, BLOWUP, FULL */

#define LASER_FILTER    40      /* 32, 8 */

#define STOP            0
#define PATROL          1
#define ALERT           2
#define TRACK           4

#define MON0            128     /* 0, MONSTER */
#define MON1            129     /* 1, MONSTER */
#define MON2            130     /* 2, MONSTER */
#define MON3            132     /* 4, MONSTER */

#define SUNNY           1
#define CASTLE          2
#define ROCKY           3
#define SNOWY           4

#define MODTYPE_NULL	0

#define MODTYPE_MOD		1
#define MODTYPE_S3M		2
#define MODTYPE_XM		3
#define MODTYPE_IT		4

#define MODTYPE_DAT_MOD	5
#define MODTYPE_DAT_S3M	6
#define MODTYPE_DAT_XM	7
#define MODTYPE_DAT_IT	8

#define MOTIF_ID_LEN	5

#define FONT_OCR20		0
#define FONT_OCR72		1
#define FONT_HELV12		2
#define FONT_HELV12B	3
#define FONT_HNEUE20	4
#define FONT_HELV10		5

#define DATAFILE_PASSWORD	"ub-dat-4.0"

extern DUH *music;
extern char music_unload;
extern AL_DUH_PLAYER *mod_player;

extern DATAFILE *fonts, *sfx, *music_dat;

extern char login[13];
extern char login_path[MAX_PATH];

extern BITMAP *gfx, *temp, *back;
extern BITMAP *ply_pic[2][12];
extern BITMAP *mon_pic[5][12];

extern BITMAP *temp_particle;

extern BITMAP *title_gfx;
extern PALETTE title_palette;

extern BITMAP *box_pic, *bomb_pic[3];
extern BITMAP *token_pic;
extern BITMAP *teleport_pic[3];
extern BITMAP *blank;

extern BITMAP *laser_pic[5];
extern BITMAP *beam_pic[7];
extern BITMAP *mirror_pic[2];

extern BITMAP *explode_pic[8];

extern BITMAP *filter_pic;

extern BITMAP *door_pic[3];
extern BITMAP *switches_pic[3];

extern BITMAP *editor_icons;
extern BITMAP *fall_pic;

extern BITMAP *dark[5];

extern BITMAP *tick_pic[2];

#define TICK_PIC_WIDTH	16
#define TICK_PIC_HEIGHT	16

extern PALETTE palette;

extern COLOR_MAP trans_table;
extern COLOR_MAP light_table;

extern int _block_width;
extern int _block_height;
extern int _block_depth;
#define _block_height_depth (_block_height + _block_depth)

extern int fps_count;
extern int time_count;
extern int game_type;
extern int game_exit;
extern int lev;
extern int mus_vol, sfx_vol, cd_vol;

extern int _game_speed;
extern char *GAME_SPEEDS[3];

extern FPS *fps;

#define MAP_MAX_WIDTH		20
#define MAP_MAX_HEIGHT		15
#define MAP_MAX_DEPTH		2

#define MAP_WIDTH		20
#define MAP_HEIGHT		15
#define MAP_DEPTH		2

extern int map[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
extern int maps[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH][101];
extern char map_motif[100][MOTIF_ID_LEN];
extern int map2[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
extern int map_laser[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
extern int map_door[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
extern int map_done[100];
extern int no_ply;
extern int mod_track, mod_last;
extern char motif[MOTIF_ID_LEN];

extern char map_save[20];

typedef struct PLY_DEF
{
 int x, y, z, dx, dy, dz;
 int frame, dir;
} PLY_DEF;

extern PLY_DEF ply[2];

typedef struct DIRTY_DEF
{
 int x, y, z, w, h;
 BITMAP *pic;
} DIRTY_DEF;

#define MAX_DIRTY	300

extern DIRTY_DEF dirty[MAX_DIRTY], dirty_sort[MAX_DIRTY];
extern int dirty_count;

typedef struct BOX_DEF
{
 int16 x, y, z, dx, dy, dz, dir, type;
 BITMAP *pic;
} BOX_DEF;

extern BOX_DEF box[468];
extern uint16 box_count;

typedef struct EXPLODE_DEF
{
 int16 x, y, z, frame, type;
} EXPLODE_DEF;

extern EXPLODE_DEF explode[468];

extern uint16 explode_count;

#define MENU_ITEM_NAME_LEN		50

typedef struct MENU_ITEM
{
 char name[MENU_ITEM_NAME_LEN];
} MENU_ITEM;

typedef struct MENU_DEF
{
 int x, y, w, h, n;
 MENU_ITEM item[10];
} MENU_DEF;

extern MENU_DEF menu[4], menu_title[4];

#define DIR_MAXNAME		MAX_PATH

typedef struct DIR_DEF
{
 char name[DIR_MAXNAME];
 char type;
 char id;
} DIR_DEF;

#define MAX_MODS		50
#define MAX_DIRS		500

extern DIR_DEF dir[MAX_DIRS], mod[MAX_MODS];

typedef struct MON_DEF
{
 int x, y, z, dx, dy, dz;
 int dir, state, type, frame;
 int speed, count;
} MON_DEF;

extern MON_DEF mon[100];

extern int mon_count;

typedef struct SWITCH_DEF
{
 int x, y, z, c;
} SWITCH_DEF;

extern SWITCH_DEF switches[60];

typedef struct DOOR_DEF
{
 int x, y, z, c, state;
} DOOR_DEF;

extern DOOR_DEF door[60];

extern int switches_count;
extern int door_count;

typedef struct SOUND_DEF
{
 int x, s;
} SOUND_DEF;

#define MAX_SOUND_EFFECTS		40

extern SOUND_DEF fx[MAX_SOUND_EFFECTS];

extern int fx_count;

typedef struct UNDO_DEF
{
 PLY_DEF ply[2];
 BOX_DEF box[468];
 EXPLODE_DEF explode[468];
 MON_DEF mon[100];
 DOOR_DEF door[60];
 SWITCH_DEF switches[60];

 int map[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
 int map_laser[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];
 int map_door[MAP_MAX_WIDTH][MAP_MAX_HEIGHT][MAP_MAX_DEPTH];

 int box_count;
 int explode_count;
 int mon_count;
 int door_count;
 int switches_count;
} UNDO_DEF;

extern UNDO_DEF undo[20];
extern int undo_count;

typedef struct MOTIF
{
	char *filename;
	char *title;
	int special;
	char uid[MOTIF_ID_LEN];
	char *gfx_fn;
	char *music_fn;
	int block_width;
	int block_height;
	int block_depth;
	char particles;
} MOTIF;

extern char _particles;

extern MOTIF motifs[20];
extern int num_motifs;

extern int B2Music;

#define MAX_PARTICLES 1000

typedef struct PARTICLE_DEF
{
	int x;
	int y;
	int dx;
	int dy;
	int col;
} PARTICLE_DEF;

extern PARTICLE_DEF part[MAX_PARTICLES];
extern int part_count;

// todo: new particles
typedef struct particle
{
	double x, y;           // location of the particle on the screen
	double dx, dy;         // speed and direction of movement
	double rx,ry;          // the amount of randomness in the movement
	double tx,ty;          // the location this particle was last at.
	int colour;            // the particle's colour
	int oldcol[5];         // colour of the old pixel
	int type;              // the drawing type of the particle
	int life;              // When the counter hits zero we remove the particle.
	struct particle *next; // a link to the next particle.
	char first_update;     // first time round?
} particle;

#define RAIN_PARTICLE  0  // here are just some types for the drawing types :)
#define SNOW_PARTICLE1 1
#define SNOW_PARTICLE2 2

extern int _redraw_status;

// functions

// bomb.c
void Del_Box(uint16 b_no);
void Add_Explode(uint16 x, uint16 y, uint16 z);
void Explode_Prime_Chain(uint16 x, uint16 y);
void Explode_Prime(uint16 x, uint16 y, uint16 z);
void Bomb_Prime_Laser(void);
void Draw_Explode(void);

// box.c
void Box_Push(int x, int y, int z, int dx, int dy);
void Box_Move(void);
void Box_Fall(void);

// music.c
void CD_Player(void);
void Mod_Music(void);
void Play_MOD_Track(const char *filename, int type, char loop);
void Poll_Music();

// door.c
void Door(void);
void Open_Doors(int c);
void Close_Doors(int c);
int Player_On(int x, int y, int p_no);

// editor.c
void Editor_Draw_Screen(void);
void Editor_Draw_Menu(void);
void Editor_Put_Block(int x, int y, int b);
void Highlight_Menu(int x);
void Drop_Down(void);
void Open_Maps(void);
void Save_Maps(void);
void Map_to_Maps(int lev);
void Maps_to_Map(int lev);
void Copy_To_Buffer(int lev);
void Copy_From_Buffer(int lev);
void Editor_Draw_Tiles(void);
void Editor_Change_Block_Type(int b);
void Clear(int l);
void Clear_All(void);
void Editor_Key_Put_Tile(int mx, int my);
void Reset_Motifs(void);
void Editor(void);

// fall.c
void Check_Fall(int p_no);
void Check_Fall_Prime(void);
int Player_On_Fall(int x, int y);

// gfx.c
void Draw_Block(int x, int y, int z, BITMAP *pic);
void Draw_Status(void);
void DirtyList(int x,int y, int z, int w, int h, BITMAP *pic);
void Draw_Map(void);
void Draw_Area(void);
void Sort_Dirty(void);
void Draw_Screen(void);
void Map_Setup(void);

// init.c
int Remember_Mod_File(const char *fn, int a, void *b);
void Add_Datafile_Mod(const char *fn, const char id, const char type);
int LoadGraphicsPack(const char *fn, int a, void *b);
void UnloadGraphics();
void Initialise(void);
void Save_Config();
void close_button_callback(void);
inline DATAFILE * load_encrypted_datafile(const char *filename);
void UpdateGameSpeeds();

extern volatile int close_button_pressed;

// laser.c
void Start_XYZ(int b_x, int b_y, int b_z, int b_dx, int b_dy, int b_dir);
void Track_Laser(void);
void Laser(void);
void Draw_Lasers();

// levels.c
void Change_Levels(void);

// main.c
void Full_Game(void);
void Get_Ready(void);
void Well_Done(void);
void Init_Full_Game(void);
void Save_Map_Done(void);
void Finish_Game(void);
void Init_Level(void);
int Run_Level(void);
int Token_Count(void);
int Next_Level(void);
int Num_Tokens(void);
char *safe_strcpy(char *dest, const size_t dest_len, const char *src);
char *safe_strcat(char *dest, const size_t dest_len, const char *src);
char *safe_strncpy(char *dest, const size_t dest_len, const char *src, const size_t source_len);
void report_error(char *str);

// menu.c
typedef struct {
	char *title;
	int id;
	char options;
} Menu;

typedef struct {
	char *title;
	int font;
} InfoWindow;

void Music_Menu(void);
void Options_Menu(void);
void In_Game_Menu(void);
void Music_Menu(void);
int Display_Menu(const Menu *menu, int *ret, int flags, int def_item);
void Display_Info_Window(const InfoWindow *menu);
void About_Box(void);
void Keyboard_Menu(void);

#define END_OF_MENU  (char *) NULL, (int) NULL, (char) NULL
#define END_OF_INFOWINDOW   (char *) NULL, (int) NULL

enum {
	MENUITEM_KEY_LEFT  = 1,
	MENUITEM_KEY_RIGHT = 2,
	MENUITEM_CLOSE = 4,
	MENUITEM_TITLE = 8,
	MENUITEM_DRAWLINES = 16
};

enum {
	MENU_KEY_ENTER = 1,
	MENU_KEY_ESC   = 2,
	MENU_KEY_SPACE = 4,
	MENU_KEY_LEFT  = 8,
	MENU_KEY_RIGHT = 16
};

enum {
	MENU_NO_REDRAW = 1,
	MENU_ALLOW_MANY_LR = 2,
	MENU_SMALL_FONT = 4
};

// mon.c
void Draw_Monsters();
void Monster(void);
void Monster_Draw(int m_no);
void Monster_Mon0(int m_no);
void Monster_Mon1(int m_no);
void Monster_Mon2(int m_no);
void Monster_Mon3(int m_no);
void Monster_Patrol(int m_no);
void Monster_Track(int m_no);
void Monster_Change_Dir_Track(int m_no, int p_no);
void Monster_Die_Check(void);
void Monster_Move(int m_no);
int Monster_Sight(int x, int y, int z, int dir);
void Del_Monster(int m_no);
void Monster_Change_Dir(int m_no, int x, int y, int z);

// motif.c
void Change_Motif(const char *m);

// part.c
void Add_Part(int x, int y, int dx, int dy, int col);
void Del_Part(int p_no);
void Particle(void);
void Particle_Move(void);
void Reset_Particles();

// ply.c
void Player_Fall(int p_no);
void Player_Move(int p_no);
void Player_Teleport(int p_no);
void Player_Pickup(int p_no, int x, int y);
void Player_Pickup_Check(int p_no);
void Player_Input(int p_no);
void Player_Draw(int p_no);
void Player_Monster_Collision(int p_no);
void Player_Laser_Collision(int p_no);
void Player_Death(int p_no);
void Player2_Input(int p_no);
inline int Player_Frame(int f);
int Player_Collision(int p_no, int dx, int dy);
int Check_Can_Fall(int x, int y, int z);
int Check_Map(int x, int y, int z, int b, int or_and, int equals_and);

// sound.c
void Add_Sound(int x, int s);
void Play_Sound(void);

// title.c
char *GetMotifFromInt(int motif);
void Log_In(void);
void Presents(void);
void Title(void);
int Title_Menu(void);
int Remember_File_Title(const char *fn, int a, void *b);
int Open_Maps_Title(void);

// trans.c
void Transition_Out(BITMAP *pic, PALETTE pal);
void Transition_In(BITMAP *pic, PALETTE pal);

// undo.c
void Move_Undo_Down(void);
void Add_Undo(void);
void Undo(void);
void Move_Undo_Down(void);
void Clear_Undo(void);

extern int _level_undos;

// platform-specific .c

#include "b4dirs.h"

const char *get_resource_file_path (const char * str1, const char * str2, const char * file, char userdir);
const char *find_resource_file (int dir, const char *file);

const char *_config_file;
const char *_graphics_dir;
const char *_music_dir;
const char *_map_dir;
