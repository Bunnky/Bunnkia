#pragma once


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

	short devMode, Avatar, Reputation, X, Y, MapLevel, hpMax, hp, Race, Guild;

	bool Wanted;
};

struct p_Guildinfo
{
	short guild;
	char tag[4];
};
