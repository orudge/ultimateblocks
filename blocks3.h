/*****************************************************/
/* Ultimate Blocks                                   */
/* Copyright (c) An Ly 2000, Owen Rudge 2001, 2008   */
/*****************************************************/

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

JGMOD *music;
DATAFILE *fonts, *sfx;

char login[13];

extern BITMAP *gfx, *temp, *back;
BITMAP *ply_pic[2][12];
BITMAP *mon_pic[5][12];

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
#define _block_height_depth (_block_height + _block_depth)

int time_count;
int game_type;
int game_exit;
int lev;
int mus_vol, sfx_vol, cd_vol;

int map[20][15][2];
int maps[20][15][2][101];
char map_motif[4][100];
int map2[20][15];
int map_laser[20][15][2];
int map_door[20][15][2];
int map_done[100];
int no_ply;
int mod_track, mod_last;
char motif[4];

char map_save[20];

typedef struct PLY_DEF
{
 int x, y, z, dx, dy, dz;
 int frame, dir;
} PLY_DEF;

PLY_DEF ply[2];

typedef struct DIRTY_DEF
{
 int x, y, z, w, h;
 BITMAP *pic;
} DIRTY_DEF;

DIRTY_DEF dirty[300], dirty_sort[300];
int dirty_count;

typedef struct BOX_DEF
{
 int x, y, z, dx, dy, dz, dir, type;
 BITMAP *pic;
} BOX_DEF;

BOX_DEF box[468];
int box_count;

typedef struct EXPLODE_DEF
{
 int x, y, z, frame, type;
} EXPLODE_DEF;

EXPLODE_DEF explode[468];

int explode_count;

typedef struct MENU_ITEM
{
 char name[50];
} MENU_ITEM;

typedef struct MENU_DEF
{
 int x, y, w, h, n;
 MENU_ITEM item[10];
} MENU_DEF;

MENU_DEF menu[4], menu_title[4];

typedef struct DIR_DEF
{
 char name[50];
} DIR_DEF;

DIR_DEF dir[500], mod[50];

typedef struct MON_DEF
{
 int x, y, z, dx, dy, dz;
 int dir, state, type, frame;
 int speed, count;
} MON_DEF;

MON_DEF mon[100];

int mon_count;

typedef struct SWITCH_DEF
{
 int x, y, z, c;
} SWITCH_DEF;

SWITCH_DEF switches[60];

typedef struct DOOR_DEF
{
 int x, y, z, c, state;
} DOOR_DEF;

DOOR_DEF door[60];

int switches_count;
int door_count;

typedef struct SOUND_DEF
{
 int x, s;
} SOUND_DEF;

#define MAX_SOUND_EFFECTS		40

SOUND_DEF fx[MAX_SOUND_EFFECTS];

int fx_count;

typedef struct UNDO_DEF
{
 PLY_DEF ply[2];
 BOX_DEF box[468];
 EXPLODE_DEF explode[468];
 MON_DEF mon[100];
 DOOR_DEF door[60];
 SWITCH_DEF switches[60];

 int map[20][15][2];
 int map_laser[20][15][2];
 int map_door[20][15][2];

 int box_count;
 int explode_count;
 int mon_count;
 int door_count;
 int switches_count;
} UNDO_DEF;

UNDO_DEF undo[20];
int undo_count;

typedef struct MOTIF
{
	char *filename;
	char *title;
	int special;
	char uid[5];
	char *gfx_fn;
	char *music_fn;
	int block_width;
	int block_height;
	int block_depth;
} MOTIF;

MOTIF motifs[20];
int num_motifs;

extern int B2Music;

// functions

// bomb.c
void Del_Box(int b_no);
void Add_Explode(int x, int y, int z);
void Explode_Prime_Chain(int x, int y);
void Explode_Prime(int x, int y, int z);
void Bomb_Prime_Laser();
void Draw_Explode(void);

// box.c
void Box_Push(int x, int y, int z, int dx, int dy);
void Box_Move(void);
void Box_Fall(void);

// cdplay.c
void CD_Player(void);

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
void Remember_Mod_File(char *fn, int a, int b);
void LoadGraphicsPack(char *fn, int a, int b);
void UnloadGraphics();
void Initialise(void);

// laser.c
void Start_XYZ(int b_x, int b_y, int b_z, int b_dx, int b_dy, int b_dir);
void Track_Laser(void);
void Laser(void);

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

// menu.c
void Music_Menu(void);
void Options_Menu(void);
void In_Game_Menu(void);
void Music_Menu(void);

// mon.c
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

// motif.c
void Change_Motif(char *m);

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
int Player_Frame(int f);
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
void Remember_File_Title(char *fn, int a, int b);
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