#ifndef Items_h
#define Items_h


#include "main.h"


//----------------------------------
// Classes that are in other files.
//----------------------------------

struct PlayerNode;
class ClientClass;
struct ItemStruct;
struct ItemStruct;
class MonsterClass;


//----------------------------------------------
// Defines that has something to do with items.
//----------------------------------------------


// Different types of item lists.
#define ItemBackpack			1
#define ItemBank				2
#define ItemWeared				3
#define ItemGround				4
#define ItemInBody				5

// Item families.
#define FAMILY_WEAPON			1
#define FAMILY_ARMOR			2
#define FAMILY_COLLECTABLE		3
#define FAMILY_USEABLE			4
#define FAMILY_CONTAINER		5

// All item types.
#define TYPE_SWORD				1
#define TYPE_AXE				2
#define TYPE_BLUNT				3
#define TYPE_POLEARM			4
#define TYPE_BOW				5

#define TYPE_ARMOR				11
#define TYPE_HELM				12
#define TYPE_SHIELD				13
#define TYPE_LEGS				14
#define TYPE_GAUNTS				15
#define TYPE_RING1				16
#define TYPE_AMULET				17
#define TYPE_ROBE				18
#define TYPE_BELT				19

#define TYPE_COLLECTABLE		20

#define TYPE_USEABLEITEM		21

// Maximum amounts of different items. (There's no limit in backpack/bank/equip yet.)


/* Defining different use requirements. */
#define req_STR						1
#define req_DEX						2
#define req_CON						3
#define req_INT						4
#define req_LVL						5
#define req_SKL						6
#define req_REP						7
#define req_REP2					8


/* Defining different use types. */
#define use_MINE					1
#define use_SMELT					2
#define use_FORGE					3
#define use_BOOST					4
#define use_REPAIR					5
#define use_TELEPORT				6
#define use_TELEPANDSPAWN			7
#define use_SPAWNGATE				8


/* Defining different elemental types. */
#define elem_FIRE					1
#define elem_LIGHTNG				2
#define elem_ICE					3
#define elem_DARK					4
#define elem_LIGHT					5


void InformMassOfNewBodyDecay( short ThisBody);

//----------------------------------------------
// Classes that has something to do with Items.
//----------------------------------------------



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
	char data[72];

	char reserved[100];
};

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
	char data[72];

	char reserved[100];
};

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
	char data[72];

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
	char data[72];

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

extern UseableItemStruct	Useables[m_Items_per_type];
extern MiscItemStruct		Collectables[m_Items_per_type];
extern ArmorInfo			Armors[m_Item_Types][m_Items_per_type];
extern WeaponInfo			Weapons[m_Item_Types][m_Items_per_type];


struct ItemStruct
{
	bool Used;
	short KnowID;

	short ID, Type, Family;
	short Durability;
	short x, y;
	unsigned long Quantity;
};

extern ItemStruct Items[m_Items];


struct PACKET_BUY
{
	short ID[8];
	unsigned long Amount[8];
};

extern long LastTakenID;

//---------------------------------------------------
// List of in what order functions are in this file.
//---------------------------------------------------


// Create new item to ground
void CreateItemToGround ( ItemStruct This);


void RemoveItemFromMass( short This);

//
void RemoveItemFromPlayer( short This, int FromList, short KnowID);


//
void InformPlayerofItem( short This, long This2, int InList);

//
void CheckItemsOnScreen( short This );

// Get free item id for item on ground
short GetFreeGroundSlot();

// 
void FreeGroundSlot(short ThisSlot);

//
void InformMassOfNewAmount( short ID, unsigned long Amount);

//
void InformPlayerOfNewAmount( short This, short InList, short ID, unsigned long Amount);

//
void InformPlayersofGroundItemsNewCoordinates(  long This, short newx, short newy);

//
void CheckBodiesOnScreen( short This);

void mass_RemoveBodyItem( short ThisBody, short ThisItem);

void SplitItem(short Player);

long GetFreeItemID();
void PileItem(short Player);

void InformMassofItem( long ItemID);
void InformNewItemDurability(short Player, short ThisItem);
#endif