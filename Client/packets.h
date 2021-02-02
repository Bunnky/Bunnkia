#ifndef packets_h
#define packets_h

#include "main.h"

#define _VERSION					3309

#define Hostname					"127.0.0.1"
#define Port						1338

#define TITLETEXT					"Bunnkia v1.27ALPHA"


#define m_MiscGraphs				20
#define m_Tiles						1024 
#define m_NPC						64
#define m_Chunks					1024
#define m_Objects					1024

#define m_Client_Bodies				30

#define m_Object_Types				256


#define m_Item_Types				10
#define m_Items_per_type			200


#define anim_No						0
#define anim_Random					1
#define anim_Flash					2

#define _num_Frames					9

#define ATTACK						1
#define PEACE						2
#define IDENTIFY					3

	
#define ItemBackpack				1
#define ItemBank					2
#define ItemWeared					3
#define ItemGround					4
#define ItemIdentify				5
	

#define BACKPACK					1
#define MINIMAP						2
#define CHATBOX						3
#define BUTTON_N					4
#define BUTTON_W					5
#define BUTTON_G					6
#define BUTTON_GM					7
#define ITEM						8
#define CHARCHOOSESELECTED			9
#define BANK						10
#define TARGET						11 /* Target takes 4 rects, so 11, 12, 13 and 14 are used */
//									12
//									13
//									14
#define HIT							15
#define MISS						16
#define SPLIT						17
#define LOS							18
#define LOCK						19
#define ORB							20
//									21
//									22
//									23
#define EXIT						24
#define QUICKSLOTS					25
#define RESPAWN						26
#define YESNOBOX					27

#define BODY_DECAY1					28
#define BODY_DECAY2					29
#define BODY_DECAY3					30
#define BODY_DECAY4					31
#define BODY_DECAY5					32
#define login_mouseover_Login		33
#define login_mouseover_Exit		34
#define login_mouseover_Choose		35
#define login_mouseover_CreateAcc	36
#define login_mouseover_Create		37
#define BUTTON_WC					38
#define expo_1						39
#define alig_1						40
#define PIDGEON						41
#define BUTTON_SC					42
#define BUTTON_SCNO					43
#define MINIMAP_BIG					44
#define TRADEWINDOW					45
#define lock_up						46
#define lock_down					47
#define accept_trade				48
#define decline_trade				49
#define alig_pointer				50

#define secure_bg					51
#define secure_offerlock			52
#define secure_sup_inactive			53
#define secure_sup_active			54
#define secure_sdown_inactive		55
#define secure_sdown_active			56
#define secure_bexit_inactive		57
#define secure_bexit_active			58
#define secure_lock_inactive		59
#define secure_lock_active			60
#define button_all_inactive			61
#define button_all_active			62
#define selected_frame				63
#define button_plus					64
#define button_minus				65
#define button_change				66
#define tgprotection				67
#define bgframe						68

#define addgf_checkbox				69
#define addgf_textedit				70

#define MaxSkills					20

#define MaxTypes					21
#define MaxAmount					30
#define MaxLevel					205
#define MaxSkill					105

#define Max_Connections				20

#define m_Connections				200

#define CTUpdateTime				3000


#define PLAYER						0
#define MONSTER						1
#define T_NPC						2

#define FAMILY_WEAPON				1
#define fam_weapon					1
#define FAMILY_ARMOR				2
#define fam_armor					2
#define FAMILY_COLLECTABLE			3
#define fam_col						3
#define FAMILY_USEABLE				4
#define fam_use						4
#define FAMILY_CONTAINER			5

#define TYPE_SWORD					1
#define TYPE_AXE					2
#define TYPE_BLUNT					3
#define TYPE_POLEARM				4
#define TYPE_BOW					5

#define TYPE_ARMOR					11 // Plate
#define TYPE_HELM					12
#define TYPE_SHIELD					13
#define TYPE_LEGS					14
#define TYPE_GAUNTS					15
#define TYPE_RING1					16
#define TYPE_AMULET					17
#define TYPE_ROBE					18
#define TYPE_BELT					19

#define TYPE_COLLECTABLE			20 // Items: Money, jewels, food, ...

#define TYPE_USEABLEITEM			21

// Some class prototypes.
class GraphsClass;
class ClientClass;
class PlayerNode;
class LoginClass;
class ItemClass;
class MonsterClass;


// Event.cpp prototypes.
void MouseButtonUp();
void MouseButtonDown();
void MouseButtonUpShop();
void MouseButtonUpGMTool();
void MouseButtonUpDeed();
void MouseButtonUpBS();

// Main.cpp prototypes.
void PlayerList_Add( PlayerNode tmp);
void PlayerList_Remove( short tmp);
PlayerNode *PlayerList_GetID( short int ID);

// Item thing prototypes.
void Item_Remove(short This, int FromList);
//void Item_Move(ItemClass *This, int TargetList);

void Item_Add(ItemClass This, int TargetList);

void MonsterList_Remove( MonsterClass *tmp);
void MonsterList_Add( MonsterClass tmp);

void CreateTables();
void CheckQuickSlots( int key);
void DrawCursor();
void MouseOver();
MonsterClass *GetMonsterByID( short ID);
short its(short n1, short n2);
Uint32 GetPixel(SDL_Surface *surface, int x, int y);


//------------------------------------------------------------------
//
struct PACKET_LEVELUP
{
	short Level;
    short Statpoints;
	short HPMax;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_SKILLUP
{
	short SkillID;
    short NewSkill;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_ITEM
{
	bool Used;
	short KnowID;

	short ID, Type, Family;
	short Durability;
	short x, y;
	unsigned long Quantity;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct NewHPClass
{
	unsigned long KnowID;
	short Type; // 0 = Player, 1 = Monster, 2 = NPC

	short NewHP;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_STATS
{
	unsigned char Str, Con, Dex, Int;
	short DamMin, DamMax;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_SKILL
{
    unsigned char This;
	short SkillLevel;
	unsigned long SkillExp;	
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_WEIGHTANDAC
{
    unsigned short Weight, WeightMax;
	unsigned short AC;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_PLAYERINFO
{
	char Name[25], Tag[4];

	short int ID;

	unsigned char Avatar, Reputation;
	short Race;

	short int X, Y;

	bool IsInGuild;

	bool Wanted;

	short int HPMax, HPLeft;

	short int Type; // Player, Monster, NPC
};
//------------------------------------------------------------------

struct p_Guildinfo
{
	short guild;
	char tag[4];
};

struct SkillData
{
	long Exp;
	short Level;
	short Bonus;
	char Reserverd[10];
};


struct UserItem
{
	unsigned long ID;
};

#define mSkills	30
//------------------------------------------------------------------
//
struct OwnInfo
{
	bool Used;
	bool Marked;

	char Name[25];

	short X, Y, Maplevel;
	short HPLeft, HPMax;
	short ManaLeft, ManaMax;
	short CharUpdate;
	short Status[5], StatusMode[5];

	unsigned short DevMode;
	unsigned short Avatar, Reputation, Race;
	unsigned short Level;
	unsigned short Str, Con, Dex, Int, Cha, Lck;
	unsigned short StatPoints;

	long KilledMonsters, CrimCount;
	long overallMonCount, overallCrimCount, overallPlayerKills, overallDeathsMonster,
		 overallDeathsPlayer, monCountSinceDeath, expCountSinceDeath, pkSinceDeath;

	unsigned long ID;
	unsigned long Exp, ExpPool;

	SkillData Skills[30];

	long cMinute, cHour, cDay, cMonth, cYear;

	char reserved[100];


	unsigned short int Weight, WeightMax,
				   AC;

	bool Wanted;
	
	short DamMin, DamMax;
	short Guild;

	short fire, air, water, earth;
};

extern OwnInfo P;
//------------------------------------------------------------------



//------------------------------------------------------------------
//
struct PACKET_COORDINATES
{
	short int X, Y;

	short int ID;
};
//------------------------------------------------------------------

struct PACKET_CHANGEAVATAR
{
	unsigned short Avatar;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_CHANGERANK
{
	char Name[25];
	short Rank;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_CHATMESSAGE
{
	char Message[120];
//	short int MsgColor;
	short int MsgType;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_ITEMMOVE
{
	unsigned char Case;
    short ItemID;
	unsigned char ToList;
	unsigned char FromList;
	short x,y;
	unsigned long Amount;
};
extern PACKET_ITEMMOVE ItemMove;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWAMOUNT
{
	short InList;
	short KnowID;
    unsigned long Amount;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NPC
{
	short X,Y;
    short HP, ID, conv_id;
	unsigned char type;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_BUY
{
	short ID[8];
	unsigned long Amount[8];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_SHOPITEM
{
	unsigned char RealID;
	short ID, Type, Family;	
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_STATS_BASE
{
	unsigned char Str_base, Con_base, Dex_base, Int_base;
	unsigned short StatPoints;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_MONSTER
{
	short KnowID;

	short X,Y;

	short HPLeft;

	short Type;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_CHARCREATION
{
	char Slot;
	unsigned char ChosenAvatar;
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_ASKSTATS
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_MUTE
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_UNMUTE
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_CLEARMAP
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_REESETMAP
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_RESETSTATS
{
	char Name[25];
  short Slot;
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_JAIL
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_UNJAIL
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_KICKFROM
{
	char Name[25];
};
//------------------------------------------------------------------

//------------------------------------------------------------------
//
struct PACKET_KILL
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_HEAL
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_BANFROM
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_TELEPORTSELF
{
	short X,Y;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_TELEPORTTOOTHER
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_BACK_TELEPORTTOOTHER
{
	short X, Y;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_TELEPORTOTHER
{
	char Name[25];
	short X,Y;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_INFOSTATS
{
	char Name[25];
	char AccName[25];
	unsigned long Exp;
	unsigned char Str, Con, Dex, Lck, Int, Cha;
	short Level, HPMax, HPLeft;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWONLINEPLAYER
{
    char Name[25];
	long ID;
	unsigned char DevMode;    
	long X;
	long Y;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWGUILDADD
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//

struct PACKET_NEWGUILDMEMBER
{
	char Name[50][25];
    short ID[50];
	short rank[50];
	bool Status[50];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWGUILDNAME
{
	char Name[45];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWGUILDTAG
{
	char Tag[4];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_MINEAT
{
	short WithItem;
    short _x, _y;	
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_SMELTAT
{
	short x,y;
	short WithItem;
	short am;
	short id;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_FORGE
{
	short ID;
	short Type;
	short Family;

	short WithItem_ID;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_FOLLOW
{
	char Name[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_BODY
{
	short ID, RealID;
	short X,Y;
	unsigned char DecayState;

    char OwnerName[25];

	bool Looted;
    char LooterName[25];
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_NEWDECAY
{
	short ID;
	unsigned char NewDecay;
};
//------------------------------------------------------------------


struct PACKET_BODYINFOSTRUCT
{
	short BodyID;
    char Name[25];
	bool Looted;
};


struct PACKET_PLAYERNODE
{
	bool Used;
	bool Wanted;
	bool Marked;

	char Name[25];

	short X, Y, Maplevel;
	short HPLeft;
	short ManaLeft;
	short CharUpdate, ShrineX, ShrineY;
	short Status[5], StatusMode[5];

	unsigned short DevMode;
	unsigned short Avatar, Reputation, Race;
	unsigned short Level;
	unsigned short Str, Con, Dex, Int, Cha, Lck;
	unsigned short StatPoints;

	long KilledMonsters, CrimCount;
	long overallMonCount, overallCrimCount, overallPlayerKills, overallDeathsMonster,
		 overallDeathsPlayer, monCountSinceDeath, expCountSinceDeath, pkSinceDeath;

	UserItem Backpack[100];
	UserItem Bank[250];
	UserItem Worn[20];

	unsigned long ID;
	unsigned long Exp, ExpPool;

	SkillData Skills[mSkills];

	long cMinute, cHour, cDay, cMonth, cYear;

	char reserved[100];
};

struct PACKET_WHISPERTO
{
	char name[25];
	char msg[90];
};

struct PACKET_GUILDMSGTO
{
	char tag[5];
	char msg[90];
};

struct PACKET_IDENTIFYPACKET
{
	short ID[20], Type[20], Family[20];
	char Name[25];
	char Tag[4];
	char Guild[45];
	short reputation;
	long id;
};


struct PACKET_NEWRESISTANCE
{
	short fire, air, water, earth;
};


/*struct PACKET_NEWEMAIL
{
	char pass[25];
	char email[45];
};*/



struct PACKET_SPAWNMONSTERS
{
	short ID[3];
	short AM[3];
};


struct PACKET_NEWINFOMSG
{
	char Msg[120];
};

struct PACKET_NAMECHANGE
{
	char name1[25];
	char name2[25];
};


struct p_Loginpacket
{
	char Name[25];
	char Pass[25];
	long Version;
};

struct p_ACreationpacket
{
	char Name[25];
	char Pass[25];
	//char Email[65];	
	long Version;
};

struct p_CChoosepacket
{
	bool Used[4];
	char Name[4][25];
	short Level[4];
	short Avatar[4];
	short hp[4];
	short hpmax[4];
};

struct p_AccInfopacket
{
	bool Premium, Golden;
	char Accname[25], Accpass[25];//, EMail[65];
};

struct p_CCreatepacket
{
	char Name[25];
	short Race, Class, Avatar, Gender, Slot;
};


struct _SkillData
{
	long Exp;
	short Level;
	short Bonus;
	char Reserverd[10];
};

struct p_OwnCharInfo
{
	char Name[25], Tag[4];

	unsigned long ID;

	short devMode, Avatar, Reputation, X, Y, MapLevel, hpMax, hp, Race, Class, Level, 
		Guild, ShrineX, ShrineY, Status[5], Statusmode[5];

	long KilledMonsters;	

	short Str, Con, Dex, Int, Statpoints;

	long Exp, ExpPool;

	long crimCount;

	bool Wanted;

	_SkillData Skills[30];
};

struct p_PlayerInfo
{
	char Name[25], Tag[4];

	unsigned long ID;

	short devMode, Avatar, Reputation, X, Y, MapLevel, hpMax, hp, Race;

	bool Wanted;
};



























#endif