#ifndef Monsters_h
#define Monsters_h


#include "main.h"

#define Spawn_Rate					50

//----------------------------------
// Classes that are in other files.
//----------------------------------

struct PlayerNode;
class ClientClass;
struct ItemStruct;
struct ItemStruct;
class MonsterClass;
struct MonsterSpawnStruct;


void CheckNPCSpawn();
short GetFreeNPCID();



//---------------------------------------------------------
// Defines that has something to do with monsters or NPCs.
//---------------------------------------------------------


// Some limits.
#define MaxMonsters					256
#define MaxNPC						64
#define MonsterSpawnRate			100

#define DEAD						0
#define ALIVE						1

#define m_Store_Items				100
#define m_Storages					20
#define m_NPC						64


#define m_MonsterTypes					250
#define m_NPCTypes						150


#define walk_NO						0
#define walk_RANDOM					1
#define walk_CHECKPNT				2
#define walk_LIMITS					3

//--------------------------------------------------------
// Classes that has something to do with monsters / NPCs.
//--------------------------------------------------------

class NPCClass;

class MonsterClass
{
public:
	MonsterClass *next;
	MonsterSpawnStruct *Used_Spawn;
	short KnowID;

	short X,Y;

	short HPLeft;

	short Type;

	ClientClass *Target;
	NPCClass *NPCTarget;
	unsigned long LstMove;
	unsigned long LstAttack;	
	unsigned long LstReg;
};

class ShopStorageClass
{
public:

	bool ShopUsed;

	char StoreName[25];

	short StoreBonus;
    
	short StoreItems;

	short Type		[m_Store_Items];
	short ID		[m_Store_Items];
	short Family	[m_Store_Items];	
};

extern ShopStorageClass	ShopStorages[m_Storages];

class NPCClass
{
public:
	NPCClass *next;

	short Type;
	unsigned char ShopID;
	short conv_id;

	struct NPCSpawnStruct *UsedSpawn;

	short HPLeft;

	short ID;

	unsigned long LstAttack;
	unsigned long LstMove;
	unsigned long LstReg;

	short NPCPLTarget;
	MonsterClass *NPCMonsterTarget;

	short X, oX;
	short Y, oY;

	int MaxDist_x;
	int MaxDist_y;
};


extern NPCClass			*NPC[128][128];
extern NPCClass			*npc_List;
extern NPCClass			*npc2_List;

struct NPCSpawnStruct
{
	NPCSpawnStruct *next;

	bool Used;

	unsigned long LstSpawned;

	short X,Y;

	unsigned char Type;
	unsigned char ShopID;
	short conv_id;

	int MaxDist_x;
	int MaxDist_y;
};

struct NPCSpawnStruct2
{
	NPCSpawnStruct *next;

	short X,Y;

	unsigned char Type;
	unsigned char ShopID;
	short conv_id;

	int MaxDist_x;
	int MaxDist_y;
};
extern NPCSpawnStruct	*NPCSpawns;

struct NPCTypesStruct
{
	char Name[25];

	bool Used, print_name;


	bool Walking, fly, ghost;
	short WalkingType;
	short WalkingRange;

	bool is_shop;
	bool is_guard;
	short alignment;


    short dam_min, dam_max;
	short max_hp;

	short atk_type;

	short dex, con, AC;


	short speed;
	short atk_speed;

	bool Animated;

	char conv[80];
	bool adv_conv;
	short adv_conv_id;


	char data[72];
    
	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

extern NPCTypesStruct NPCTypes[m_NPCTypes];


struct MonsterSpawnStruct
{
	MonsterSpawnStruct *next;
	
	short X,Y;

	bool Used;

	unsigned char Type;
    
	int MaxDist_x;
	int MaxDist_y;
};

struct MonsterSpawnStruct2
{
	MonsterSpawnStruct *next;
	
	short X,Y;

	unsigned char Type;
    
	int MaxDist_x;
	int MaxDist_y;
};

extern MonsterSpawnStruct		*MonsterSpawns;

extern MonsterClass				*Monsters[128][128];
extern MonsterClass				*mon2_List;

void Monster_MoveList( MonsterClass *monster, short newx, short newy);

struct LootStruct
{
	long DropProbability;
	short Type, ID, Family;
	unsigned long AmountMin, AmountMax;
};

//------------------------------------------------------------------
//
struct MonsterTypesStruct
{
	char Name[25];

	bool Used, ghost, fly, rnd_walk_off;
	short rnd_walk_range;

	short DamMin;
	short DamMax;
	short HPMax;
	short AC;
	short Dex, con;

	short MovingSpeed;
	short AttackSpeed;
    
	unsigned char MonsterLevel;

	short ElementalDefence;
	short DefenceAmount;

	short ElementalAttack;
	short AttackAmount;

	LootStruct Loot[10];

	char data[72];

	bool Animated;

	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

extern MonsterTypesStruct MonsterTypes[m_MonsterTypes];



//---------------------------------------------------
// List of in what order functions are in this file.
//---------------------------------------------------


//
//void AddMonster( MonsterClass This);

//
//void RemoveMonster( MonsterClass *This);

//
short GetFreeMonsterID();

//
void FreeMonsterID(short ThisSlot);

//
void MonsterAction_HasTarget( MonsterClass *This);

//
void MonsterAction_Attack( MonsterClass *This);

//
void MonsterAction_Move( MonsterClass *This);

//
void CheckMonsterSpawn();

// Check direction in what monster will move.
int Check_dir(short x, short y);

//
bool MoveEnemy(MonsterClass *This);

//
void CheckTarget( MonsterClass *This);

//
void InformNewMonsterHP( MonsterClass *This);

//
void CheckMonstersOnScreen( short This);

//
void CheckNPCsOnScreen( short This);

//
MonsterClass *GetMonsterByID( short ID);

//
void DropLoot(MonsterClass *This, int Luck);


void NPCAction_Respawn(NPCClass *npc);
void NPCAction_Died(NPCClass *npc);
void NPCAction_WalkRandom(NPCClass *npc);
void NPCAction_WalkLimits(NPCClass *npc);
void NPCAction_HasTarget( NPCClass *npc);
void NPCAction_Move( NPCClass *npc, short tx, short ty);
bool MoveNPC(NPCClass *npc, short ox, short oy);
void CheckNPCTarget( NPCClass *npc);
void NPCAction_AttackToPlayer(NPCClass *npc, ClientClass *Target);
void NPCAction_AttackToMonster(NPCClass *npc, MonsterClass *Target);

void NPC_MoveList( NPCClass *npc, short newx, short newy);

bool Check_PosMove(short ToX, short ToY);

NPCClass *GetNPCByID(short ID);
void DeathPenalty(ClientClass *Target);
void JailPlayer(short This);
void ReleaseFromJail( short This);

void MonsterAction_Regenerate( MonsterClass *This );
void MonsterAction_Berserk( MonsterClass *This);

void MonsterAction_AttackToNPC(MonsterClass *This);

void SpawnMonster(short x, short y, short id);
void InformPlayersofNewMonster(MonsterClass *NewMon);


#endif