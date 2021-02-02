#ifndef character_h
#define character_h

#include "main.h"


#define mSkills	30


class CharacterTool
{
public:

	void check_command();

};
extern CharacterTool CharTool;



struct SkillData
{
	long Exp;
	short Level;
	short Bonus;
	char Reserverd[10];
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

struct SkillsStruct
{	
	short In;
    char Name[25];
};

extern CharacterToEdit Char[100];





#endif