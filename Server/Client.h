#ifndef Client_h
#define Client_h


#include "main.h"


//----------------------------------
// Classes that are in other files.
//----------------------------------

struct PlayerNode;
class ClientClass;
struct ItemStruct;
class MonsterClass;


//------------------------------------------------
// Defines that has something to do with clients.
//------------------------------------------------


// Some limits.
#define Max_Connections			150
#define MaxLevel				205
#define MaxSkill				105
#define MaxSkills				20

#define m_Bodies				128
#define body_DecayTime			60000
#define m_Body_decays			4		// 0, 1, (2), [3], 4 Total: 5, () Lootable, [] Items drop to ground

#define repupoints_limit		21000
#define repu_killpenalty		1000
#define repu_lootpenalty		200
#define crim_killpenalty		4
#define crim_lootpenalty		1

//---------------------------------------------------
// Classes and structs.
//---------------------------------------------------

//---------------------------------------------------
// List of in what order functions are in this file.
//---------------------------------------------------

// Client_Add
//void Client_Add( CLIENT_TYP tmp);

// Client_Remove
//void Client_Remove( CLIENT_TYP *tmp);

// Client_GetID
short Client_GetID( short ID);

// Get free backpack slot
short GetFreeBackpackSlot( short This);

// Get free bank slot
short GetFreeBankSlot( short This);

// Get free weared slot
short GetFreeWearedSlot( short This);

// Informing player of all his items.
void InformPlayerofHisAllItems(short This);

//
void CheckStats(short This);
void DoItemWipe(short This);

//
void CheckStats_base(short This);

//
ItemStruct *GetWeapon( short This);

//
void PlayerAction_AttackToMonster( short This);

//
void PlayerAction_AttackToPlayer( short This);

//
void InformPlayerOfHisSkills(short This);

//
bool CheckLevelGain(short This);

//
short GetHPMax(short This);

//
bool CheckSkillLevelGain(short This, short Skill);

//
void InformPlayerOfLevelUp(short This);

//
void InformPlayerOfNewSkill(short This, short ThisSkill);

void CalcClientsConnected();
void ResetStats(int This, int ThisChar);

void InformGMofOnlineList( int This );
void InformGMofNewOnlinePlayer( int This, int ThisPlayer );

void CheckGuildPlayers( int This );

void Check_Guild( short Player, short Slot );

void InformGuildLeader(short This);

void BuyGuild(short This, short i);

void PlayerAction_Mine(short i);

void PlayerAction_Smelt(short i);

void PlayerAction_ForgeItem(short i);

void PlayerAction_AttackToNPC( short i);

void InformMassofNewCriminalStatus( short This );

void CreateBody(short This, short ToX, short ToY);

void RemoveBody( int ThisBody);

void mass_InformNewLooter(short BodyID);
void Player_Inform_GotLooted(char Name[25], short WhatBody);

void InformMassofNewAligment( int This );

void KillMonster(MonsterClass *This);

bool Check_Requirement(short This, short ReqType, short ReqAmount, short ThisSkill);

bool Check_Aligment(short This);

void InformGMofOnlineAmount( short ThisGM);

void InformSelfofCrimStatus(short Player);

struct authstruct
{
	char name[20];
	char pass[20];
};


class ClientClass 
{
public:
	bool Used;
	bool Login, preLogin;

	bool authed;
	short AccountID;
	Account *Acc;
	CharacterData *uChar;
	int Version;

	//I am guessing that state is which character you are using...Radiankh
	int State; // 0 = Passive, 1 = In game with first char, 2 = In game with second char, ...
	SOCKET ClientSocket;
	struct sockaddr_in clnt_addr;

	bool KnowPlayer[Max_Connections];
	bool KnowMonster[m_MonsterIDs];
	bool KnowItem[MaxItems];
	bool KnowNPC[m_NPCIDs];
	bool KnowBody[m_Bodies];


	unsigned long LstMsg;
	unsigned long LstMov;
	unsigned long LstAtk;
	unsigned long LstReg;
	unsigned long lst_tradeskill;
	unsigned long LstJailtime;
	unsigned long lstcrimtimer;
	unsigned long LstLogTime;

	bool is_logging;

	bool is_mute;

	bool tradeskill_inuse;
	bool t_tradesucceed;

	short t_useskill;
	short t_recvexp;

	unsigned long t_recvam, t_failam;
	long t_useitemid, t_useitem;
	bool t_useitembroke;
	short t_tarid, t_tarfam, t_tartype;	
	long t_cost;


	bool TradeinProgress;

	unsigned long LstDeath;
	
	short PLTarget;
	class MonsterClass *MonTarget;
	class NPCClass *NPCTarget;

	short int AttackTarget;

    short int Know_ID;

	bool GMMode;
	bool Inv;
	bool ListenWhisper;
	bool ListenNormal;
	bool ListenGuild;
	bool IsInJail;

	bool IsLeader;

	short BrowsingShop;
	short BrowsingBank;

	short DamMin, DamMax;

	long Weapon;

	bool TimeoutNo;

	short beingfollowed[2];
	short isfollowing;

	ofstream *logfile;


	void report_repupoints();
	void report_reputation();
	void use_skill();
	void mined();
	void smelted();
	void forged();
	void CrimDeath();

	void infoMsg(char Msg[120]);
};

extern ClientClass Clients[Max_Connections];




#endif