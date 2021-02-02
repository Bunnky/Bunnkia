#ifndef main_h
#define main_h

#define FD_SETSIZE	512	

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <fstream>
using namespace std;
//#include <SDL.h>

#define m_Items						1000000
#define MaxSkills					20

#define _VERSION					3309
#define _CHAR_UPDATE				17

#define _NEWBIE_PROTECTION			10

#define PORT						1338

#define YES							1 
#define NO							2  

#define TimeOutTime					10000

#define CT_DAMAGETAKEN				1
#define CT_DAMAGEDEALT				2
#define CT_EXPRECEIVED				3
#define CT_SKILLRAISED				4
#define CT_HEALED					5
#define CT_REGEN					6
#define CT_EXPRECEIVED_MAX			7

#define	GUILDCOST					500000
#define GUILDREQ					1

#define MonsterActionCheckTime		50
#define MonsterSpawnTimeCheck		180000
#define PlayerActionTimeCheck		25

#define MaxMonsterSpawns			256
#define MaxMonsters					256

#define MaxGuilds					1000

#define MaxAmount					30
#define MaxObjects					1024

#define m_MiscGraphs				20
#define m_Tiles						1024
#define m_Monsters					256
#define m_NPC						64
#define m_Chunks					1024
#define m_Objects					1024

#define m_Object_Types				256

#define m_Item_Types				10
#define m_Items_per_type			200
#define m_Items_per_type_old		30

#define MaxItems				512 
#define MaxBackpackItems		64
#define MaxBankItems			196
#define MaxWearedItems			20


#define _num_Frames					9

#define GUARD						10

#define m_MonsterIDs			10000
#define m_NPCIDs				2048

#define STR_MAX					100
#define DEX_MAX					100
#define CON_MAX					100
#define Int_MAX					100
#define LVL_MAX					200


/* Defining different elemental types. */
#define elem_FIRE					1
#define elem_AIR					2
#define elem_EARTH					3
#define elem_WATER					4
#define elem_LIGHT					5

#define m_elem						5

#define boost_str					1
#define boost_dex					2
#define boost_con					3
#define boost_lck					4
#define boost_int					5
#define boost_cha					6

#define m_boost						6

#include "packets.h"


#include "acco.h"
#include "Client.h"
#include "Items.h"
#include "Monsters.h"
#include "Secure trade.h"
#include "objects.h"

#include "race.h"

struct PlayerNode;
class ClientClass;
struct ItemStruct;
class MonsterClass;

extern char *buffer;
extern char *buffer2;
extern char *buffer3;
extern char *buffer4;


//extern int i, id, i1, i2, i3, j;

bool	Error(char *error);
bool	IsValid(short int tx2, short int ty2);
bool	IsInRange(short int plx, short int ply, short int tmpx, short int tmpy);
bool	IsInRange2(short int plx, short int ply, short int tmpx, short int tmpy);
bool	IsNearBanker(short x, short y);


short	Player_GetFreeID();
short	GetFreeClient();


int		Disconnect( short Player);
int		Check_Connections();


void	Setup();
void	Shutdown_Server();
void	Save_Creations();
void	Check_Message();
void	Get_Message( short Player);
void	Player_FreeID(short int This);
void	Load_Objects_obj_Lists();
void	Load_Monsters_Lists();
void	Load_NPCs_Lists();
void	SaveItems();
void	InformMassofNewCriminalStatus2(ClientClass *This);


extern	MonsterClass			*Monsters[128][128];
extern	MonsterClass			*mon_List;
extern	NPCClass				*npc_List;
extern	bool UsedMonsterID[m_MonsterIDs];
extern	bool UsedNPCID[m_NPCIDs];

extern	long Ground[MaxItems];

void	CreateTables();

extern	sockaddr_in Server_Address;

extern	SOCKET ListeningSocket;

extern	timeval timeout; 
extern	int ClientsConnectedtoShow;

extern	long LastStaticID;

extern	unsigned long ExpTables[MaxLevel];
extern	unsigned long SkillExpTables[MaxLevel];
extern	unsigned long AligmentTables[12];


extern	RaceInfo	Races[6];


extern	ItemStruct Items[m_Items];


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

extern TileClass		Tiles[m_Tiles];

class MapClass
{	
public:
	unsigned short int Chunks[2][4096][16][16];

	unsigned short int ChunkMap[512][512];
};


extern MapClass			Map;


struct PACKET_ASKSTATS
{
	char Name[25];
};

struct PACKET_ASK_BANK
{
	char Name[25];
	short ID;
	short Family;
	short Type;
};

struct PACKET_ASK_BACKPACK
{
	char Name[25];
	short ID;
	short Family;
	short Type;
};

struct PACKET_MUTE
{
	char Name[25];
};

struct PACKET_UNMUTE
{
	char Name[25];
};

struct PACKET_CLEARMAP
{
	char Name[25];
};

struct PACKET_REESETMAP
{
	char Name[25];
};

struct PACKET_RESETSTATS
{
	char Name[25];
  short Slot;
};

struct PACKET_KICKBAN
{
	char Name[25];
	unsigned char KickBan;
};

struct PACKET_INFOSTATS
{
	char Name[25];
	char AccName[25];
	unsigned long Exp;
	unsigned char Str, Con, Dex, Lck, Int, Cha;
	short Level, HPMax, HPLeft;
};



struct PACKET_LOGIN
{
	char LoginName[20];
	char LoginPass[20];
	short Version;
};

struct PACKET_STATS
{
	unsigned char Str, Con, Dex, Int;
	short DamMin, DamMax;
};

struct PACKET_STATS_BASE
{
	unsigned char Str, Con, Dex, Int;
	unsigned short StatPoints;
};
extern PACKET_STATS_BASE NewStats_base;

struct PACKET_SKILL
{
    unsigned char This;
	short SkillLevel;
	unsigned long SkillExp;	
};

struct PACKET_WEIGHTANDAC
{
    unsigned short Weight, WeightMax;
	unsigned short AC;
};

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

	short int Type; // Player, Monster, NPC. Player-0, NPC-1, Monster-2to5
};

struct PACKET_CHARCHOOSE
{	
	unsigned char Used[2];

	char Name[2][25], Tag[2][4];

	unsigned short Level[2];
    
	unsigned char Avatar[2], Reputation[2];

	bool Wanted[2];

	short int HPMax[2], HPLeft[2];

	bool IsInGuild[2];	
};

struct PACKET_CHANGEAVATAR
{
	unsigned short Avatar;
};


struct PACKET_CHANGERANK
{
	char Name[25];
	short Rank;
};

struct PACKET_MOTDLINE
{
	char Type;
	char Msg[60];
};

struct PACKET_COORDINATES
{	
	short int X, Y;

	short int ID;
};

struct PACKET_CHATMESSAGE
{
	char Message[120];
	short int MsgType;
};

struct PACKET_ITEMMOVE
{
	unsigned char Case;
    short ItemID;
	unsigned char ToList;
	unsigned char FromList;
	short x,y;
	unsigned long Amount;
};

struct PACKET_NEWAMOUNT
{
	short InList;
	short KnowID;
    unsigned long Amount;
};

struct PACKET_NPC
{
	short X,Y;
    short HP, ID, conv_id;
	unsigned char type;
};

struct MiscPicClass
{
	short Pic;

	short X,Y;
};

struct NewCombatTextClass
{
	short Amount;
	short Type;
	short SkillType;
	short FromTo;	
};

struct NewHPClass
{
	unsigned long KnowID;
	short Type; // 0 = Player, 1 = Monster, 2 = NPC

	short NewHP;
};

struct PACKET_MONSTER
{
	short KnowID;

	short X,Y;

	short HPLeft;

	short Type;
};

struct PACKET_SHOPITEM
{
	unsigned char RealID;
	short ID, Type, Family;	
};

struct PACKET_LEVELUP
{
	short Level;
    short Statpoints;
	short HPMax;
};

struct PACKET_SKILLUP
{
	short SkillID;
    short NewSkill;
};

struct PACKET_CHARCREATION
{
	char Slot;
	unsigned char ChosenAvatar;
	char Name[25];
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

struct PACKET_JAIL
{
	char Name[25];
};

struct PACKET_UNJAIL
{
	char Name[25];
};

struct PACKET_KICKFROM
{
	char Name[25];
};

struct PACKET_KILL
{
	char Name[25];
};

struct PACKET_HEAL
{
	char Name[25];
};

struct PACKET_BANFROM
{
	char Name[25];
};

struct PACKET_TELEPORTSELF
{
	short X,Y;
};

struct PACKET_TELEPORTTOOTHER
{
	char Name[25];
};

struct PACKET_BACK_TELEPORTTOOTHER
{
	short X, Y;
};

struct PACKET_TELEPORTOTHER
{
	char Name[25];
	short X,Y;
};

struct PACKET_NEWONLINEPLAYER
{
    char Name[25];
	long ID;
	unsigned char DevMode;    
	long X;
	long Y;
};

struct GuildMember 
{
	char Name[25];
	int AccID;
	int Slot;
	int Active;

	int rank;

	char reserved[20];
};

class GuildClass
{
public:
	char Name[45];
	char Tag[4];

	int Active;

	int Leader_AccID;
	int Leader_Slot;

	GuildMember Members[50];

	char reserver[100];

	int amount_ranks(int rank);
	int promote(int member);
	int demote(int member);	
};


struct PACKET_NEWGUILDMEMBER
{
	char Name[50][25];
    short ID[50];
	short rank[50];
	bool Status[50];
};

struct PACKET_GUILDINFOFORLEADER
{
	char Name[45];
	char Tag[4];    
};

struct PACKET_NEWGUILDNAME
{
	char Name[45];
};
struct PACKET_NEWGUILDTAG
{
	char Tag[4];
};
struct PACKET_MINEAT
{
	short WithItem;
    short _x, _y;	
};
struct PACKET_SMELTAT
{
	short x,y;
	short WithItem;
	short am;
	short id;
};

struct PACKET_FORGE
{
	short ID;
	short Type;
	short Family;

	short WithItem_ID;
};

struct PACKET_FOLLOW
{
	char Name[25];
};

struct BodyClass
{
	bool Used;

	short X,Y;

	bool Looted;
	char LooterName[25];
	bool LooterCrim;

	short Owner_ID;
	short Owner_Slot;
	short Guild;
	char OwnerName[25];

	unsigned char DecayState;
	unsigned long DecayStateTime;
        
	long Items[84];

	bool DropItems(short ThisBody);
	bool BodyLooted(short WhoLooted, short ThisBody);
	bool RemoveClientsKnowledge(short ThisBody);	
};

struct PACKET_BODY
{
	short ID, RealID;
	short X,Y;
	unsigned char DecayState;

    char OwnerName[25];

	bool Looted;
    char LooterName[25];
};

struct PACKET_NEWDECAY
{
	short ID;
	unsigned char NewDecay;
};

struct PACKET_BODYINFOSTRUCT
{
	short BodyID;
    char Name[25];
	bool Looted;
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

struct PACKET_NEWRESISTANCE
{
	short fire, air, water, earth;
};


struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};


struct ServerControlInfo
{
	bool ServerWideChatEnabled;
	bool LoginEnabled;
	bool NoChatLogging;    
};

void InformPlayerofNewCrimCounts(short This);
void CheckPlayersOnScreen(short Player, PACKET_COORDINATES NewCoordinates);

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



struct CharacterToEdit
{
	bool Used;
	bool Wanted;
	bool Marked;

	char Name[25];

	unsigned long accid;
	short charslot;

	short X, Y, Maplevel;
	short HPLeft;
	short ManaLeft;
	short ShrineX, ShrineY;

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

	SkillData Skills[mSkills];

	long cMinute, cHour, cDay, cMonth, cYear;
};




class AccountToEdit
{
public:

	bool InUse;
	bool IsBanned;
	bool IsPremium;
	bool IsGolden;

	char AccName[20];
	char AccPass[20];
	char Email[65];

	long cMinute, cHour, cDay, cMonth, cYear;

	unsigned long ID;

	char CharNames[2][25];
};









#endif