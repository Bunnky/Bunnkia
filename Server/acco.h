#ifndef ACCO_H
#define ACCO_H

#include "main.h"

int loadguilds();
int saveguilds(); 
bool IPBanned(char *name);


#define mSkills	30


class Account;
class AccountManager
{
public:

	AccountManager();
	~AccountManager();

	Account *Accs; // DO NOT TOUCH!!! If this list goes broken, Accounts will be lost!!!!!!

	Account *GetByName(char const *Name);
	Account *GetByPass(char const *Pass);
	//Account *GetByEmail(char *Email);
	Account *GetByID(ULONG ID);
	Account *GetByCharName(char const *Name);
//	CharacterData *AccountManager::GetCharByID(ULONG ID);

	ULONG LastusedID;
	ULONG GetNewID();

	ULONG LastSave, ASLastSave;
	VOID CheckSave();
	VOID DoSave();

	
	VOID Clearlist();
	Account *NewAccount(p_ACreationpacket *pACreation);
	BOOL NewChar(Account *Acc, p_CCreatepacket *pCCreate);

	BOOL LoadAccounts();
	BOOL SaveAccounts();
  VOID ResetStats(char *Name, short Slot);

	BOOL CharNameTaken(char Name[25]);
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

struct CharacterData
{
	bool Used;
	bool Wanted;
	bool Marked;

	char Name[25];

	short X, Y, Maplevel;
	short HPLeft;
	short HPMax;
	short ManaLeft;
	short CharUpdate, ShrineX, ShrineY;
	short Status[5], StatusMode[5];

	char Tag[4];

	unsigned short DevMode;
	unsigned short Avatar, Reputation, Race, Guild;
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

class AccountData
{
public:

	bool InUse;
	bool IsBanned;
	bool IsPremium;
	bool IsGolden;

	char AccName[20];
	char AccPass[20];
	//char Email[65];

	short LoggedInID;

	long cMinute, cHour, cDay, cMonth, cYear;

	unsigned long ID;
};


struct TempData
{
	short Avatar, Reputation, Race, Str, Con, Dex, Int, Status[5], Statusmode[5], HPMax, Guild;
	char Tag[4];
	short prot_Air, prot_Fire, prot_Earth, prot_Water;
	long AC, Weight, WeightMax;
};


class Account
{
private:

	AccountManager *Base;
	AccountData		Data;
	CharacterData	Char[2];

public:

	Account();
	~Account();

	Account *Next, *Prev;

	void Addto(AccountManager *This);

	TempData		tChar;

	int				UsedClient;
	int				Status;
	CharacterData	*uChar;

	BOOL CompareName(char const *Name);
	BOOL ComparePass(char const *Pass);
	//BOOL CompareEmail(char *Email);
	bool CompareID(unsigned long ID);
	CharacterData *CompareCharName(char const *Name);
	BOOL CharNamed(char Name[25]);
	BOOL IsBanned();

	p_CChoosepacket *CopyCChoose();
	CharacterData *GetCharPointer(short uChar);
	AccountData *GetAccPointer();

	VOID CopyAccData(char *data, long dlen);
	VOID CopyCharData(char *data, long dlen);
	VOID CopyNewChar(CharacterData cData, short Slot);

	BOOL CharacterExists(short Slot);

	char *GetAccData();
	char *GetCharData();

	VOID Create(char Name[25], char Pass[25]);

	p_OwnCharInfo *CreateCharInfo();
	p_PlayerInfo  *CreatePlayerInfo();

	short Calc_hpMax();
	unsigned long getAccID();
	char *getAccName();

	void NotUsed();
	bool isUsed();
	bool isPremium();
	bool isGolden();

	short whoUsing();
	void StartUsing(short x);
	void Ban();

	void getStats();
	void reportStats();
	void informGuild();
};



#endif