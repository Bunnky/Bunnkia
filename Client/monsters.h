#ifndef monsters_h
#define monsters_h


#include "main.h"


//------------------------------------------------------------------
//
struct LootStruct
{
	long DropProbability;
	short Type, ID, Family;
	unsigned long AmountMin, AmountMax;
};
//------------------------------------------------------------------



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

	SDL_Rect Frames[9];

	bool Animated;

	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

extern MonsterTypesStruct MonsterTypes[m_MonsterTypes];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
class MonsterClass
{
public:

	MonsterClass *next, *prev;
	class MonsterDatabase *database;


	bool used;
	short knowid, x,y, hpleft, type;


	void addto();
	void removefrom();
	
	//============================
	short frame;
	unsigned long frametime;
	void anim_start();
	void anim_advance();
	//============================
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
class MonsterDatabase
{
public:

	MonsterClass *monsters;
	MonsterClass *mon;
	
	MonsterClass *getbyid(short id);
	MonsterClass *getlast();
	void clearlist();

	void drawmon();
};
extern MonsterDatabase *mondatabase;
//------------------------------------------------------------------






#endif