#pragma once

#define FD_SETSIZE 512

#include "SDL.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <fstream>


using namespace std;

#pragma warning(disable:4996)



#define MaxSkills					20
#define MaxNPC						64
#define MaxItems					64 
#define MaxBackpackItems			64
#define MaxBankItems				196
#define MaxWearedItems				20
#define MaxClients					40
#define MaxMonsters					50

#define m_Clients					80
#define m_NPC						64
#define m_NPCTypes					64
#define m_MonsterTypes				96

#define m_Tiles						1024

//------------------------------------------------------------------
//
class ItemClass
{
public:

	ItemClass *next, *prev;

	class ItemDatabase *database;
	bool Used;
	short KnowID;

	short ID, Type, Family;
	short Durability;
	short x, y;
	unsigned long Quantity;

	void addto();
	void remove();
};


class ItemDatabase
{
public:

	class ItemClass *items;

	void clearlist();
};




extern ItemClass iBackpack[MaxBackpackItems];
extern ItemClass iBank[MaxBankItems];
extern ItemClass iWeared[MaxWearedItems];
extern ItemClass iGround[MaxItems];
//------------------------------------------------------------------

// Include basic gamefunctions headers
#include "BFont.h"
#include "BunnkiaSDL.h"
#include "packets.h"
#include "players.h"
#include "monsters.h"
#include "npc.h"
#include "body.h"
#include "draw.h"
#include "events.h"
#include "InitAll.h"
#include "FileHandling.h"
#include "sizes.h"

#include "textlink.h"
#include "button.h"
#include "textedit.h"
#include "race.h"

#include "GMTool.h"

// Include tradeskill headers
#include "Blacksmithing.h"

#include "class_stringtable.hpp"



// Include features headers
#include "Shop.h"
#include "Clan deed.h"
#include "Quickslots.h"
#include "skills.h"
#include "surfaces.h"
#include "Secure Trade.h"

#include "drag.h"
#include "identify.h"

#include "InfoMsg.h"

#include "Minimap.h"



#define CT_DAMAGETAKEN				1
#define CT_DAMAGEDEALT				2
#define CT_EXPRECEIVED				3
#define CT_SKILLRAISED				4
#define CT_HEALED					5
#define CT_REGEN					6
#define CT_EXPRECEIVED_MAX			7

#define TimeOutTime					10000

/* Defining different use types. */
#define use_MINE					1
#define use_SMELT					2
#define use_FORGE					3
#define use_BOOST					4
#define use_REPAIR					5
#define use_TELEPORT				6
#define use_TELEPANDSPAWN			7

const char * const use_Info[] = 
{ 
	"none", 
	"Where do you wish to mine at?", 
	"Where do you wish to smelt at?",
	"Choose ingots to forge.", 
	"Select target", 
	"What do you wish to repair?" 
};

/* Defining different use requirements. */
#define req_STR						1
#define req_DEX						2
#define req_CON						3
#define req_INT						4
#define req_LVL						5
#define req_SKL						6
#define req_REP						7
#define req_REP2					8

const char * const req_Info[] = 
{ 
	"none", 
	"strength", 
	"dexterity", 
	"constitution", 
	"intelligence", 
	"level", 
	"skill",
	"evil rep",
	"good rep"
};

const char * const req_Info_fail[] =
{
	"none",
	"You are not strong enough to use this.",
	"Your dexterity is not high enough to use this.",
	"Your constitution is not high enough to use this.",
	"Your intelligence is not high enough to use this.",
	"You are not high enough level to use this.",
	"You are too weak in this skill to use this.",
	"You are not evil enough to use this.",
	"Your heart is not pure enough to use this.",
};

/* Defining different elemental types. */
#define elem_FIRE					1
#define elem_LIGHTNG				2
#define elem_ICE					3
#define elem_DARK					4
#define elem_LIGHT					5


const char * const npc_conv[] =
{
	"Obey the law of the land!",
	"Welcome to my shop, adventurer!",
	"Welcome to the bank!",
	"Hello.",
	"Mrrr...",
	"Bunnky broke yer shit, son!",
	"Good day adventurer!",
	"My shop isn't open yet.. Maybe it'll be someday.",
	"Grr.. Go away.",
	"Yikes!",
};

#define m_conv						10


const char * const align_names[] =
{
	"scourge",
	"evil",
	"hated",
	"disliked",
	"neutral",
	"liked",
	"good",
	"hero",
	"divine",
	"nothing",
	"normal"
};


extern cStringTable *cStrings;



class ItemClass;

int				Game();
int				FilterEvents(const SDL_Event *event);

char			sgn (int a);

void			visible(int x1, int y1, int x2, int y2);
void			IdentifyItem(ItemClass *This);
void			Handle_Packet();
void			RequestMOTD();
void			HandleMessage();
void			GetMessage();
void			CleanUp();

bool			IsInRange2(short int plx, short int ply, short int tmpx, short int tmpy);
bool			itsi(short x1, short y1, short x2, short y2);

void			calc_LOS();


ItemClass		*GetItemByID( short InList, short ID);

struct MapObjects;
extern MapObjects			*obj_List;

extern unsigned long ExpTables[MaxLevel];
extern unsigned long SkillExpTables[MaxLevel];
extern unsigned long AlignmentTables[12];
extern bool ListenToServerChat;
extern bool Identify;

// Globals
extern int MX, MY;
//i, i1, i2, i3, i4, ii, j, k, ind;
extern SDL_Event event;
extern SOCKET Socket;
extern char *buffer;
extern bool Shift, Tab, Use;
extern bool IsPre;

extern short UseTarget;
extern short UseType;

extern unsigned long ObjectFrametime;
extern unsigned char Frames[3][10];

extern short SafeZones[512][512];
void OpenOwnBody();
void m_k();
void c_p(int size);
void c_p_b(int size);

void CheckAutoTarget();


const char * const elem_Info[] = 
{ "none", "fire", "air", "earth", "water", "light" };


void MakeOwnInfo(PlayerNode OwnNode);

void DrawMouseback();
void GetMouseback();
void DrawMouse();


extern InfoMsgClass *Info;

extern MiniMap *Minimap;

extern RaceInfo Races[6];
extern RaceClass *Race;

//------------------------------------------------------------------
//

class MapClass
{	
public:
	unsigned short int Chunks[2][4096][16][16];

	unsigned short int ChunkMap[512][512];
};


extern MapClass Map;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct MapAddClass
{
	bool				MapVisibility[512][512];
	char				Visible[21][21];
	unsigned char		WaterposFrames[4][4];
	char				VisBlocktable[21][21];
};

extern MapAddClass MapInfo; 
//------------------------------------------------------------------



//------------------------------------------------------------------
//
class TileClass
{
public:
	unsigned char Block, Visibility;


	char Name[25];

	unsigned char r,g,b;

	unsigned char ToLayer;

	unsigned char animated;
	short usetype, res_id, res_type;
	char reserved[50];
};

extern TileClass Tiles[m_Tiles];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct Container
{
	short int X, Y;
	bool Docked;

	bool On;    
};

extern Container Backpack; 
extern Container Bank;
extern Container v_Body;
extern int v_Body_Open;
//------------------------------------------------------------------




//------------------------------------------------------------------
//
class LoginClass
{
public:
	char AccName[20], AccPass[20];//, EMail[65];

	bool SaveAccountName;

	SDL_Surface *bGround;
	SDL_Surface *fGround;
	SDL_Surface *charchoose;
	SDL_Surface *charCreation;
	SDL_Surface *accCreation;

	SDL_Surface *mainmenu;

	p_CChoosepacket CharChooseInfo;

	int CurrentChoice;
	int ChosenAvatar;

	int curAv;

	char CharName[25];

	void Prepare();

	void DrawPics();
	void DrawTexts();
    
	void DrawCharChoosePics();
	void DrawCharChooseTexts();
	void PrepareAccCreation();
	void CleanupAccCreation();

	texteditdatabase *textedits;
};

extern LoginClass Login;
//------------------------------------------------------------------




//------------------------------------------------------------------
//
class ClientClass
{
public:

	short Type[20];
	short Family[20];
	short ID[20];
	char IDName[25];
	char IDGuild[40];
	char IDTag[5];
    bool MouseMoving;
	short AttackMode;
    bool TargetProtectionMode;

	bool SayOn;
	bool WhisOn;
	bool GlobOn;

	bool WASD;

	bool OwnName;
	bool PlNames;

	short CraftPer;

	bool IsInGuild;
	short Frame[m_Clients];
	unsigned long FrameTime[m_Clients];


	bool ShowHealth;
	bool CombatTextOn;

	long FPS;
	bool Update;
	short UpdateScreen;
	unsigned long LastMove;

	bool ShowMouseXY;
	bool mbut1, mbut2;

	void KeyboardMove();
	void Mousemove();

	bool mbut_down();
	bool mbut_up();
	bool key_down();
	bool key_up();

	bool NoLos;
};

extern ClientClass Client;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct MapObjects
{
	MapObjects *next;

	bool On;

	short X,Y;

	short _X, _Y;

	unsigned long Container[8];

	short Type;

	short Text;
};

extern MapObjects *Objects[256][256];
//------------------------------------------------------------------


struct MapObjectsToSave
{
	short X,Y;

	short _X, _Y;

	unsigned long Container[8];

	short Type;

	short Text;
};


//------------------------------------------------------------------
//
struct ObjectsStruct
{
	bool Used;

	bool Openable;

	bool Block;
	bool VisBlock;

	unsigned long UseableWithItemID;

	char Name[25];
    
	short UseType;

	bool Animated;
	unsigned char AnimationType;

	unsigned char num_Frames;

	SDL_Rect Frames[9];
};

extern ObjectsStruct ObjectTypes[ m_Object_Types ];
//------------------------------------------------------------------





//------------------------------------------------------------------
//
//------------------------------------------------------------------





//------------------------------------------------------------------
//
struct WeaponInfo
{
	char Name[25];

    short DamMin;
	short DamMax;

	unsigned long Value;

	short Weight;

	short Speed;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;
	short Level;

	short Speciality;
	short SpecialityAmount;
	
	short ElementalDamageType;
	short ElementalDamageMin;
	short ElementalDamageMax;

	short MaxDur;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	char reserved[100];
};

extern WeaponInfo Weapons[m_Item_Types][m_Items_per_type];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct ArmorInfo
{
	char Name[25];

	short AC;

	unsigned long Value;

	short Weight;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short Level;

	short Speciality;
	short SpecialityAmount;

	short ElementalProtection;
	short ElementalProtectionAmount;

	short MaxDur;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	char reserved[100];
};

extern ArmorInfo Armors[m_Item_Types][m_Items_per_type];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct UseableItemStruct
{
	char Name[25];

    unsigned long Value;

	short Weight;
	short DamMin;
	short DamMax;

	short MaxDurability;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_type; // use_MINE, use_SMELT, use_FORGE, use_REPAIR, use_BOOST,
	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	char reserved[100];
};

extern UseableItemStruct Useables[m_Items_per_type];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct MiscItemStruct
{
	char Name[25];

	unsigned long Value;

	short Weight;
    short DamMin;
	short DamMax;

	short CureType;

	short MaxDurability;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_type;
	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	char reserved[100];
};

extern MiscItemStruct Collectables[m_Items_per_type];
//------------------------------------------------------------------






void GetMapVisBlocktable();
bool IsInRange(short int plx, short int ply, short int tmpx, short int tmpy);

void recurfill(int x, int y);
void calclos(char type);
int GetDir();

extern bool Done2;
