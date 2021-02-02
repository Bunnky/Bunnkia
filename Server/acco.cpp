#include <iostream>
#include <fstream>

#include "main.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)

extern GuildClass Guilds[MaxGuilds];

AccountManager::AccountManager()
{
	Accs = NULL;
	LastSave = GetTickCount();
}


AccountManager::~AccountManager()
{
	Clearlist();
}


Account::Account()
{
	memset(&Data, 0, sizeof(Data));
	memset(&Char, 0, sizeof(Char));
	memset(&tChar, 0, sizeof(tChar));
}

Account::~Account()
{

}


VOID AccountManager::Clearlist()
{
	Account *tmp;
	
	while (Accs)
	{
		tmp = Accs->Next;
		delete Accs;
		Accs = tmp;
	}
	Accs = NULL;
}

char *Account::GetCharData()
{
	return (char *)&Char;
}

char *Account::GetAccData()
{
	return (char *)&Data;
}


BOOL AccountManager::LoadAccounts()
{
	ifstream iFile;
	ofstream oFile("temppijee.txt");
	
	iFile.open("Accounts.vs2", ios::binary);

	if (!iFile.is_open())
	{
		cout << "Account file doesn't exist! Created new file." << endl;

		char const *saveFile = "-File Created-";
		ofstream zFile("Accounts.vs2", ios::binary);
			zFile.write((char *)saveFile, sizeof(saveFile));
			zFile.close();

		LastusedID = 1;

		return TRUE;
	}

	unsigned long LastID;
	iFile.read((char *)&LastID, sizeof(LastID));

	LastusedID = LastID; 
	printf("LASTUSEDID == %d\n", LastusedID);

	int dlen = sizeof(AccountData);
	int clen = sizeof(CharacterData) * 2;
	char *data = new char[dlen];
	char *cdata = new char[clen];
	
	Account *tmp;

	short count = -1;

	cout << "dlen = " << dlen << " ja clen = " << clen << endl;

	while (!iFile.eof())
	{
		tmp = new Account();

		iFile.read((char *)data, dlen);

		for (int i = 0; i < dlen; i++)
			if (data[i] != 0)
				data[i] ^= 123;

		tmp->CopyAccData(data, dlen);

		iFile.read((char *)cdata, clen);
		
		for (int i = 0; i < clen; i++)
			if (cdata[i] != 0)
				cdata[i] ^= 15;

		tmp->CopyCharData(cdata, clen);

		tmp->Addto(this);
		count++;
		tmp = NULL;
	}

	cout << count << " accounts loaded." << endl;

	delete[] data, cdata;	

	iFile.close();



	tmp = Accs;

	CharacterData *Char1 = NULL, *Char2 = NULL;

	bool rimyyv;

	while (tmp)
	{
		if (tmp->CharacterExists(0))
			Char1 = tmp->GetCharPointer(0);
		if (tmp->CharacterExists(1))
			Char2 = tmp->GetCharPointer(1);

		if (Char1 != NULL)
		{
			Char1->ID = tmp->getAccID();
			if (Char1->DevMode > 0) oFile << "Account " << tmp->getAccName() << " has gm char " << Char1->Name << " of status " << Char1->DevMode << endl;
		}
		if (Char2 != NULL)
		{
			Char2->ID = tmp->getAccID();
			if (Char2->DevMode > 0) oFile << "Account " << tmp->getAccName() << " has gm char " << Char2->Name << " of status " << Char2->DevMode << endl;
		}

		for (int i = 0; i < MaxBankItems; i++)
		{
			if (i < MaxBackpackItems)
			{
				if (Char1 != NULL)
				{
					rimyyv = false;
					if (Char1->Backpack[i].ID < 0 || Char1->Backpack[i].ID >= m_Items)
					{
						Char1->Backpack[i].ID = 0;
						printf("-er-Char1-MaxBackpackItems-2-");
					}					
					if (!Items[Char1->Backpack[i].ID].Used && Char1->Backpack[i].ID != 0)
					{
						Char1->Backpack[i].ID = 0;
						printf("-er-Char2-MaxBackpackItems-3-");
					}
					if (Items[Char1->Backpack[i].ID].Used)
					{
						if (Items[Char1->Backpack[i].ID].Durability <= 0) Items[Char1->Backpack[i].ID].Durability = 1;
						if (Items[Char1->Backpack[i].ID].Family <= 0) rimyyv = true;
						if (Items[Char1->Backpack[i].ID].Type <= 0) rimyyv = true;
						if (Items[Char1->Backpack[i].ID].KnowID < 0) Items[Char1->Backpack[i].ID].KnowID = i;
						if (Items[Char1->Backpack[i].ID].Quantity <= 0) Items[Char1->Backpack[i].ID].Quantity = 1;						
					}
					if (rimyyv) Char1->Backpack[i].ID = 0;
				}
				if (Char2 != NULL)
				{
					rimyyv = false;
					if (Char2->Backpack[i].ID < 0 || Char2->Backpack[i].ID >= m_Items)
					{
						Char2->Backpack[i].ID = 0;
						printf("-er-Char2-MaxBackpackItems-2-");
					}					
					if (!Items[Char2->Backpack[i].ID].Used && Char2->Backpack[i].ID != 0)
					{
						Char2->Backpack[i].ID = 0;
						printf("-er-Char2-MaxBackpackItems-3-");
					}
					if (Items[Char2->Backpack[i].ID].Used)
					{
						if (Items[Char2->Backpack[i].ID].Durability <= 0) Items[Char2->Backpack[i].ID].Durability = 1;
						if (Items[Char2->Backpack[i].ID].Family <= 0) rimyyv = true;
						if (Items[Char2->Backpack[i].ID].Type <= 0) rimyyv = true;
						if (Items[Char2->Backpack[i].ID].KnowID < 0) Items[Char2->Backpack[i].ID].KnowID = i;
						if (Items[Char2->Backpack[i].ID].Quantity <= 0) Items[Char2->Backpack[i].ID].Quantity = 1;						
					}
					if (rimyyv) Char2->Backpack[i].ID = 0;
				}
			}
				
			
			if (i < MaxWearedItems)
			{
				if (Char1 != NULL)
				{
					rimyyv = false;
					if (Char1->Worn[i].ID < 0 || Char1->Worn[i].ID >= m_Items)
					{
						Char1->Worn[i].ID = 0;
						printf("-er-Char1-MaxWearedItems-2-");
					}					
					if (!Items[Char1->Worn[i].ID].Used && Char1->Worn[i].ID != 0)
					{
						Char1->Worn[i].ID = 0;
						printf("-er-Char1-MaxWearedItems-3-");
					}
					if (Items[Char1->Worn[i].ID].Used)
					{
						if (Items[Char1->Worn[i].ID].Durability <= 0) Items[Char1->Worn[i].ID].Durability = 1;
						if (Items[Char1->Worn[i].ID].Family <= 0) rimyyv = true;
						if (Items[Char1->Worn[i].ID].Type <= 0) rimyyv = true;
						if (Items[Char1->Worn[i].ID].KnowID < 0) Items[Char1->Worn[i].ID].KnowID = i;
						if (Items[Char1->Worn[i].ID].Quantity <= 0) Items[Char1->Worn[i].ID].Quantity = 1;						
					}
					if (rimyyv) Char1->Worn[i].ID = 0;
				}
				if (Char2 != NULL)
				{
					rimyyv = false;
					if (Char2->Worn[i].ID < 0 || Char2->Worn[i].ID >= m_Items)
					{
						Char2->Worn[i].ID = 0;
						printf("-er-Char2-MaxWearedItems-2-");
					}					
					if (!Items[Char2->Worn[i].ID].Used && Char2->Worn[i].ID != 0)
					{
						Char2->Worn[i].ID = 0;
						printf("-er-Char2-MaxWearedItems-3-");
					}
					if (Items[Char2->Worn[i].ID].Used)
					{
						if (Items[Char2->Worn[i].ID].Durability <= 0) Items[Char2->Worn[i].ID].Durability = 1;
						if (Items[Char2->Worn[i].ID].Family <= 0) rimyyv = true;
						if (Items[Char2->Worn[i].ID].Type <= 0) rimyyv = true;
						if (Items[Char2->Worn[i].ID].KnowID < 0) Items[Char2->Worn[i].ID].KnowID = i;
						if (Items[Char2->Worn[i].ID].Quantity <= 0) Items[Char2->Worn[i].ID].Quantity = 1;						
					}
					if (rimyyv) Char2->Worn[i].ID = 0;
				}
			}


			if (Char1 != NULL)
			{
				rimyyv = false;
				if (Char1->Bank[i].ID < 0 || Char1->Bank[i].ID >= m_Items)
				{
					Char1->Bank[i].ID = 0;
					printf("-er2-");
				}					
				if (!Items[Char1->Bank[i].ID].Used && Char1->Bank[i].ID != 0)
				{
					Char1->Bank[i].ID = 0;
					printf("-er3-");
				}
				if (Items[Char1->Bank[i].ID].Used)
				{
					if (Items[Char1->Bank[i].ID].Durability <= 0) Items[Char1->Bank[i].ID].Durability = 1;
					if (Items[Char1->Bank[i].ID].Family <= 0) rimyyv = true;
					if (Items[Char1->Bank[i].ID].Type <= 0) rimyyv = true;
					if (Items[Char1->Bank[i].ID].KnowID < 0) Items[Char1->Bank[i].ID].KnowID = i;
					if (Items[Char1->Bank[i].ID].Quantity <= 0) Items[Char1->Bank[i].ID].Quantity = 1;						
				}
				if (rimyyv) Char1->Bank[i].ID = 0;
			}

			if (Char2 != NULL)
			{
				rimyyv = false;
				if (Char2->Bank[i].ID < 0 || Char2->Bank[i].ID >= m_Items)
				{
					Char2->Bank[i].ID = 0;
					printf("-er2-");
				}					
				if (!Items[Char2->Bank[i].ID].Used && Char2->Bank[i].ID != 0)
				{
					Char2->Bank[i].ID = 0;
					printf("-er3-");
				}
				if (Items[Char2->Bank[i].ID].Used)
				{
					if (Items[Char2->Bank[i].ID].Durability <= 0) Items[Char2->Bank[i].ID].Durability = 1;
					if (Items[Char2->Bank[i].ID].Family <= 0) rimyyv = true;
					if (Items[Char2->Bank[i].ID].Type <= 0) rimyyv = true;
					if (Items[Char2->Bank[i].ID].KnowID < 0) Items[Char2->Bank[i].ID].KnowID = i;
					if (Items[Char2->Bank[i].ID].Quantity <= 0) Items[Char2->Bank[i].ID].Quantity = 1;						
				}
				if (rimyyv) Char2->Bank[i].ID = 0;
			}

		}
	
		tmp = tmp->Next;
	}

	//EDIT STATS HERE


	Account *acc;

	acc = this->GetByName("Admin");
	if(acc != NULL)
	{
		Char1 = acc->GetCharPointer(0);
		
		if (Char1->Name != "¥[*DEV] ÅAdmin")
		{
			strcpy(Char1->Name, "¥[*DEV] ÅAdmin");
			Char1->Level = 205;
			Char1->DevMode = 6;
			Char1->Con = 100;
			Char1->Dex = 100;
			Char1->Str = 100;
			Char1->Int = 100;
			Char1->Skills[1].Level = 100;
			Char1->Skills[2].Level = 100;
			Char1->Skills[3].Level = 100;
			Char1->Skills[4].Level = 100;
			Char1->Skills[15].Level = 100;
			Char1->Skills[16].Level = 100;

			printf("DEV Admin");
		}
	}

	printf(" - ");

	SaveAccounts();


	return TRUE;
}


VOID AccountManager::ResetStats(char *Name, short Slot)
{
  Account *acc;
  CharacterData *Char1 = NULL;

  acc = this->GetByName(Name);
  Char1 = acc->GetCharPointer(Slot);
  Char1->StatPoints = Char1->Level * 2 + 8;
  Char1->Reputation = 4;
  Char1->KilledMonsters = 3500;
  Char1->Int = 5;
  Char1->Str = 5;
  Char1->Dex = 5;
  Char1->Con = 5;
}

BOOL AccountManager::SaveAccounts()
{	
	Save_Creations();

	cout << "Do Accounts save." << endl;

	BOOL DoArchiveSave = FALSE;

//	if (GetTickCount() - ASLastSave > 3600000)
//		DoArchiveSave = TRUE;

	ofstream oFile, asFile;

	oFile.open("Accounts.vs2", ios::binary);

	if (!oFile.is_open())
	{
		cout << "Failed to open accounts.vs2!!! Accounts NOT SAVED." << endl;
		return FALSE;
	}
	
/*	if (DoArchiveSave)
	{
		char Folder[25];
		char Filename[25];

		sprintf(Folder, "Bunnkia Save %d-%d-%d-%d", SYS->Timer->CurrentTime.wHour, SYS->Timer->CurrentTime.wDay, SYS->Timer->CurrentTime.wMonth, SYS->Timer->CurrentTime.wYear);
		sprintf(Filename, "Accounts.vsf");

		CreateDirectory(Filename, NULL);		

//		asFile.open(
	}*/


	oFile.write((char *)&LastusedID, sizeof(LastusedID));


	int dlen = sizeof(AccountData);
	int clen = sizeof(CharacterData) * 2;
	char *data = new char[dlen];
	char *cdata = new char[clen];

	
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		memcpy(data, tmp->GetAccData(), dlen);
		memcpy(cdata, tmp->GetCharData(), clen);

		for (int i = 0; i < dlen; i++)
			if (data[i] != 0)
				data[i] ^= 123;

		for (int i = 0; i < clen; i++)
			if (cdata[i] != 0)
				cdata[i] ^= 15;

		oFile.write((char *)data, dlen);
		oFile.write((char *)cdata, clen);
		
		tmp = tmp->Next;
	}


	delete[] data, cdata;	

	oFile.close();

	return TRUE;
}


VOID Account::CopyAccData(char *data, long dlen)
{
	memcpy(&Data, data, dlen);
	Data.InUse = false;
	Data.IsPremium	= false;
	Data.IsGolden	= false;
	Data.LoggedInID = 0;
}


VOID Account::CopyCharData(char *data, long dlen)
{
	memcpy(&Char, data, dlen);
}


void Account::Addto(AccountManager *This)
{
	Base = This;

	if (Base->Accs == NULL)
	{
		Next				= NULL;
		Prev				= NULL;
		Base->Accs			= this;
	}
	else
	{		
		Next				= Base->Accs;
		Prev				= NULL;
		Base->Accs->Prev	= this;
		Base->Accs			= this;			
	}
}

Account *AccountManager::GetByName(char const *Name)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->CompareName(Name))
			return tmp;

		tmp = tmp->Next;
	}
	return NULL;
}

Account *AccountManager::GetByPass(char const *Pass)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->ComparePass(Pass))
			return tmp;
	}
	return NULL;
}

/*Account *AccountManager::GetByEmail(char *Email)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->CompareEmail(Email))
			return tmp;

		tmp = tmp->Next;
	}
	return NULL;
}*/


Account *AccountManager::GetByID(unsigned long ID)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->CompareID(ID))
			return tmp;

		if (tmp) tmp = tmp->Next;
	}
	return NULL;
}

bool Account::CompareID(unsigned long ID)
{
	if (Data.ID == ID) return true;
	else return false;
}


Account *AccountManager::GetByCharName(char const *Name)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->CompareCharName(Name) != NULL)
		{
			return tmp;
		}
		tmp = tmp->Next;
	}
	return NULL;
}

CharacterData *Account::CompareCharName(char const *Name)
{
	if (!strcmp(Name, Char[0].Name))
		return GetCharPointer(0);
	if (!strcmp(Name, Char[1].Name))
		return GetCharPointer(1);
	return NULL;
}

/*CharacterData *AccountManager::GetCharByID(unsigned long ID)
{
	Account *tmp;
	tmp = Accs;
	while (tmp)
	{
		if (tmp->CompareCharID(ID))
		{
			return tmp;
		}
	}
	return NULL;
}*/

p_CChoosepacket *Account::CopyCChoose()
{
	p_CChoosepacket *tmp;
	tmp = new p_CChoosepacket;
	memset(tmp, 0, sizeof(p_CChoosepacket));

	for (int i = 0; i < 2; i++)
	{
		tmp->Used[i] = Char[i].Used;
		
		if (tmp->Used[i])
		{
			strcpy(tmp->Name[i],  Char[i].Name);
			tmp->Avatar[i]		= Char[i].Avatar;
			tmp->hp[i]			= Char[i].HPLeft;
			//tmp->hpmax[i]		= Char[i].HPMax;
			tmp->hpmax[i]		= 55 + Char[i].Level * 15 + Char[i].Con * 10;
			tmp->Level[i]		= Char[i].Level;
		}
	}

	return tmp;
}

/*
p_AccInfopacket *Account::CopyAccInfo()
{
	p_AccInfopacket *tmp;
	tmp = new p_AccInfopacket;
	memset(tmp, 0, sizeof(p_AccInfopacket));

	strcpy(tmp->Accname, Data.Name);
	strcpy(tmp->Accpass, Data.Pass);
	strcpy(tmp->EMail,	 Data.Mail);
	tmp->Golden		   = Data.Golden;
	tmp->Premium	   = Data.Premium;

	return tmp;
}*/


BOOL Account::CompareName(const char *Name)
{
	if (!strcmp(Data.AccName, Name))
		return TRUE;
	else return FALSE;
}

BOOL Account::ComparePass(const char *Pass)
{
	if (strcmp(Data.AccPass, Pass))
		return TRUE;
	else return FALSE;
}

/*BOOL Account::CompareEmail(char *Email)
{
	if (!strcmp(Data.Email, Email))
		return TRUE;
	else return FALSE;
}*/


//BOOL Account::CompareID(unsigned long ID)
//{
	
//}


Account *AccountManager::NewAccount(p_ACreationpacket *pACreation)
{
	Account *New;
	New = GetByName(pACreation->Name);
	if (New != NULL)
	{
		// Account name taken.
/*		char *buf;
		buf = new char[2];
		buf[0] = MSG_MESSAGE;
		buf[1] = MESSAGE_ANAMETAKEN;
		Peer->Send(Current->cSock, buf, 2);
		delete buf;*/
		return NULL;
	}

	/*New = GetByEmail(pACreation->Email);
	if (New != NULL)
	{
		// EMail has an account already.
/*		char *buf;
		buf = new char[2];
		buf[0] = MSG_MESSAGE;
		buf[1] = MESSAGE_EMAILREGISTERED;
		Peer->Send(Current->cSock, buf, 2);
		delete buf;
		return FALSE;
	}*/

	// All requirements met, create account.

	New = NULL;

	New = new Account;
	New->Addto(this);

	New->Create(pACreation->Name, pACreation->Pass);

/*	char *buf;
	buf = new char[2];
	buf[0] = MSG_MESSAGE;
	buf[1] = MESSAGE_REGISTERED;
	Peer->Send(Current->cSock, buf, 2);
	delete buf;*/

	SaveAccounts();
	return New;
}

BOOL AccountManager::NewChar(Account *Acc, p_CCreatepacket *pCCreate)
{
/*	if (Acc->CharacterExists(pCCreate->Slot))
	{
		char *buf;
		buf = new char[2];

		buf[0] = MSG_MESSAGE;
		buf[1] = MESSAGE_FAILUNKOWN;
		Peer->Send(Acc->uClient->cSock, buf, 2);

		delete buf;

		return FALSE;
	}*/
	
	CharacterData Data;
	memset(&Data, 0, sizeof(Data));

	strcpy(Data.Name, pCCreate->Name);
	Data.Avatar		= pCCreate->Avatar;
//	Data.Gender		= pCCreate->Gender != 0 ? true : false;
	Data.Race		= pCCreate->Race;
	if (Data.Race != 0 && Data.Race != 2) Data.Race = 0;
//	Data.Class		= pCCreate->Class;

	Data.Str		= 5;
	Data.Dex		= 5;
	Data.Int		= 5;
	Data.Con		= 5;
	Data.Level		= 1;
	Data.Exp		= 2;
	Data.ID			= GetNewID();
	Data.Used		= true;
	Data.StatPoints = 10;
	Data.KilledMonsters	= 3500;
	Data.Reputation = 4;
	if (_CHAR_UPDATE == 16)
		Data.CharUpdate = _CHAR_UPDATE-1;	
	else
		Data.CharUpdate = _CHAR_UPDATE;

	Data.X			= 411;
	Data.Y			= 529;
	Data.ShrineX	= 411;
	Data.ShrineY	= 529;
	Data.HPLeft		= 55 + Data.Level * 15 + Data.Con * 10;
	Data.HPMax		= 55 + Data.Level * 15 + Data.Con * 10;
		
	Acc->CopyNewChar(Data, pCCreate->Slot);

	return TRUE;
}

BOOL AccountManager::CharNameTaken(char Name[25])
{
	Account *tmp;
	tmp = Accs;

	while (tmp)
	{
		if (tmp->CharNamed(Name))
			return TRUE;
		
		tmp = tmp->Next;
	}
	return FALSE;
}

BOOL Account::CharNamed(char Name[25])
{
	if (!strcmp(Name, Char[0].Name))
		return TRUE;
	if (!strcmp(Name, Char[1].Name))
		return TRUE;
	return FALSE;
}

VOID Account::CopyNewChar(CharacterData cData, short Slot)
{
	memcpy(&Char[Slot], &cData, sizeof(cData));
}


CharacterData *Account::GetCharPointer(short Slot)
{
	return &Char[Slot];
}


AccountData *Account::GetAccPointer()
{
	return &Data;
}


short Account::Calc_hpMax()
{
	return 55 + uChar->Level * 15 + tChar.Con * 10;
}

BOOL Account::CharacterExists(short Slot)
{
	if (Char[Slot].Used) return TRUE;
	else return FALSE;
}

/*
p_OwnCharInfo *Account::CreateCharInfo()
{
	p_OwnCharInfo *tmp;
	tmp = new p_OwnCharInfo;

	tmp->Avatar			= uChar->Avatar;
	tmp->Con			= uChar->Con;
	tmp->crimCount		= uChar->CrimCount;
	tmp->devMode		= uChar->DevMode;
	tmp->Dex			= uChar->Dex;
	tmp->Exp			= uChar->Exp;
	tmp->ExpPool		= uChar->ExpPool;
	tmp->hp				= uChar->HPLeft;
	tmp->ID				= uChar->ID;
	tmp->Int			= uChar->Int;
	tmp->Level			= uChar->Level;
	tmp->MapLevel		= uChar->Maplevel;
	tmp->Race			= uChar->Race;
	tmp->Reputation		= uChar->Reputation;
	tmp->Statpoints		= uChar->StatPoints;
	tmp->Str			= uChar->Str;
	tmp->X				= uChar->X;
	tmp->Y				= uChar->Y;
	strcpy(tmp->Name,			uChar->Name);
	memcpy(&tmp->Status,		&uChar->Status, sizeof(tmp->Status));
	memcpy(&tmp->Statusmode,	&uChar->StatusMode, sizeof(tmp->Statusmode));
	memcpy(&tmp->Skills,		&uChar->Skills, sizeof(tmp->Skills));
	
	return tmp;
}*/

p_PlayerInfo *Account::CreatePlayerInfo()
{
	p_PlayerInfo *tmp;
	tmp = new p_PlayerInfo;

	tmp->Avatar			= uChar->Avatar;
	tmp->hp				= uChar->HPLeft;
	tmp->hpMax			= uChar->HPMax;
	tmp->ID				= uChar->ID;
	tmp->MapLevel		= uChar->Maplevel;
	tmp->Race			= uChar->Race;
	tmp->Reputation		= uChar->Reputation;
	tmp->X				= uChar->X;
	tmp->Y				= uChar->Y;
	tmp->Guild			= uChar->Guild;
	strcpy(tmp->Name,	uChar->Name);
	strcpy(tmp->Tag,	uChar->Tag);

	return tmp;
}

VOID Account::Create(char Name[25], char Pass[25])
{
	memset(&Data, 0, sizeof(Data));
	memset(&Char, 0, sizeof(Char));
	
	strcpy(Data.AccName, Name);
	//strcpy(Data.Email,	 Email);
	strcpy(Data.AccPass, Pass);

//	Data.cMinute	= SYS->Timer->CurrentTime.wMinute;
//	Data.cHour		= SYS->Timer->CurrentTime.wHour;
//	Data.cDay		= SYS->Timer->CurrentTime.wDay;
//	Data.cMonth		= SYS->Timer->CurrentTime.wMonth;
//	Data.cYear		= SYS->Timer->CurrentTime.wYear;

	Data.ID			= Base->GetNewID();
}

ULONG AccountManager::GetNewID()
{
	ULONG newID;
	newID = LastusedID;

	LastusedID++;

	return newID;
}

VOID AccountManager::CheckSave()
{
	if (GetTickCount() - LastSave > 600000)
	{
		SaveAccounts();
		SaveItems();
		saveguilds();
		LastSave = GetTickCount();
	}
}


BOOL Account::IsBanned()
{
	if (Data.IsBanned) return TRUE;
	else return FALSE;
}

unsigned long Account::getAccID()
{
	return Data.ID;
}


char *Account::getAccName()
{
	return Data.AccName;
}

void Account::NotUsed()
{
	Data.InUse = false;
	Data.IsPremium	= false;
	Data.IsGolden	= false;
	Data.LoggedInID = -1;
}

bool Account::isUsed()
{
	return Data.InUse;
}

bool Account::isPremium()
{
	if (Data.IsPremium) return TRUE;
	else return FALSE;
}

bool Account::isGolden()
{
	if (Data.IsGolden) return TRUE;
	else return FALSE;
}

short Account::whoUsing()
{
	return Data.LoggedInID;
}

void Account::StartUsing(short x)
{
	Data.InUse		= true;
	Data.IsPremium	= false;
	Data.IsGolden	= false;
	Data.LoggedInID = x;
	UsedClient		= x;
}


void Account::Ban()
{
	Data.IsBanned = true;
}


void Account::getStats()
{
	unsigned short TmpAC = 0, TmpWeight = 0, TmpDamMin = 0, TmpDamMax = 0;

	tChar.Str			= uChar->Str;
	tChar.Con			= uChar->Con;
	tChar.Dex			= uChar->Dex;
	tChar.Int			= uChar->Int;

	tChar.prot_Air		= 0;
	tChar.prot_Fire		= 0;
	tChar.prot_Earth	= 0;
	tChar.prot_Water	= 0;

	long item;

	int i, am;
	for (i = 0; i < MaxWearedItems; i++)
	{
		if (uChar->Worn[i].ID > 0)
		{
			item = uChar->Worn[i].ID;
			if (Items[item].Family == 2)
			{
				TmpAC += Armors[Items[item].Type-11][Items[item].ID].AC;
				TmpWeight += Armors[Items[item].Type-11][Items[item].ID].Weight;

				if (Armors[Items[item].Type-11][Items[item].ID].Speciality != 0)
				{
					am =    Armors[Items[item].Type-11][Items[item].ID].SpecialityAmount;
			        switch (Armors[Items[item].Type-11][Items[item].ID].Speciality)
					{
					case boost_str:
						tChar.Str += am;						
						break;
					case boost_dex:
						tChar.Dex += am;						
						break;
					case boost_con:
						tChar.Con += am;						
						break;
					case boost_int:
						tChar.Int += am;						
						break;
					default: break;
					}
				}
				if (Armors[Items[item].Type-11][Items[item].ID].ElementalProtection != 0)
				{
					am =    Armors[Items[item].Type-11][Items[item].ID].ElementalProtectionAmount;
			        switch (Armors[Items[item].Type-11][Items[item].ID].ElementalProtection)
					{
					case elem_FIRE:
						tChar.prot_Fire += am;
						break;
					case elem_AIR:
						tChar.prot_Air += am;
						break;
					case elem_EARTH:
						tChar.prot_Earth += am;
						break;
					case elem_WATER:
						tChar.prot_Water += am;
						break;
					default: break;
					}
				}
			}
			if (Items[item].Family == FAMILY_WEAPON)
			{
				TmpWeight += Weapons[Items[item].Type][Items[item].ID].Weight;
				if (Weapons[Items[item].Type][Items[item].ID].Speciality != 0)
				{
					am =    Weapons[Items[item].Type][Items[item].ID].SpecialityAmount;
			        switch (Weapons[Items[item].Type][Items[item].ID].Speciality)
					{
					case boost_str:
						tChar.Str += am;						
						break;
					case boost_dex:
						tChar.Dex += am;						
						break;
					case boost_con:
						tChar.Con += am;						
						break;
					case boost_int:
						tChar.Int += am;						
						break;
					default: break;
					}
				}
			}
		}
	}


	// Backpack

	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (uChar->Backpack[i].ID > 0)
		{
			if (Items[uChar->Backpack[i].ID].Family == FAMILY_WEAPON)
			{
				TmpWeight += Weapons[Items[uChar->Backpack[i].ID].Type][Items[uChar->Backpack[i].ID].ID].Weight;
			}
			if (Items[uChar->Backpack[i].ID].Family == FAMILY_ARMOR)
			{
				TmpWeight += Armors[Items[uChar->Backpack[i].ID].Type-11][Items[uChar->Backpack[i].ID].ID].Weight;
			}
			if (Items[uChar->Backpack[i].ID].Family == FAMILY_USEABLE)
			{
				TmpWeight += Useables[Items[uChar->Backpack[i].ID].ID].Weight;
			}
			if (Items[uChar->Backpack[i].ID].Family == FAMILY_COLLECTABLE)
			{
				TmpWeight += Collectables[Items[uChar->Backpack[i].ID].ID].Weight * Items[uChar->Backpack[i].ID].Quantity;
			}
		}
	}


	Clients[UsedClient].Weapon = uChar->Worn[0].ID;

	if (Clients[UsedClient].Weapon > 0)
	{
		TmpDamMin = Weapons[Items[Clients[UsedClient].Weapon].Type][Items[Clients[UsedClient].Weapon].ID].DamMin;
		TmpDamMax = Weapons[Items[Clients[UsedClient].Weapon].Type][Items[Clients[UsedClient].Weapon].ID].DamMax;

		// Add formula bonuses to here also

		TmpDamMin *= (1 + (float)uChar->Skills[Items[Clients[UsedClient].Weapon].Type].Level / 100 + (float)tChar.Str / 400);
		TmpDamMax *= (1 + (float)uChar->Skills[Items[Clients[UsedClient].Weapon].Type].Level / 100 + (float)tChar.Str / 400);

        ////
	}
	else
	{
		TmpDamMin = 1;
		TmpDamMax = 2;
	}


	TmpAC		   += tChar.Dex >> 1;

	tChar.AC		= TmpAC;
	tChar.Weight	= TmpWeight;	
	tChar.WeightMax = 10 + 5 * tChar.Str;
	tChar.HPMax		= Calc_hpMax();
	
	buffer[0] = 30;
	buffer[1] = 6;
	memcpy(&buffer[2], &tChar.HPMax, sizeof(tChar.HPMax));
	send(Clients[UsedClient].ClientSocket, buffer, 2 + sizeof(tChar.HPMax), 0);

    //Inform everything to client.

	PACKET_WEIGHTANDAC	NewWeight;

	memset(&NewWeight, 0, sizeof(NewWeight));

	NewWeight.AC		= tChar.AC;
	NewWeight.Weight	= tChar.Weight;
	NewWeight.WeightMax = tChar.WeightMax;

	buffer[0] = 30;
	buffer[1] = 1;

	memcpy(&buffer[2], &NewWeight, sizeof(NewWeight));

	send(Clients[UsedClient].ClientSocket, buffer, 2 + sizeof(NewWeight), 0);


	PACKET_STATS		NewStats;
	memset(&NewStats, 0, sizeof(NewStats));

	NewStats.Con	= tChar.Con;
	NewStats.Dex	= tChar.Dex;
	NewStats.Int	= tChar.Int;
	NewStats.Str	= tChar.Str;
	NewStats.DamMax = TmpDamMax;
	NewStats.DamMin = TmpDamMin;	
	
	//Radian
	//printf("\nPACKET_STATS: %d %d %d %d %d-%d", NewStats.Str, NewStats.Dex, NewStats.Con, NewStats.Int, NewStats.DamMin, NewStats.DamMax);
	
	buffer[0] = 30;
	buffer[1] = 2;

	memcpy(&buffer[2], &NewStats, sizeof(NewStats));

	send(Clients[UsedClient].ClientSocket, buffer, 2 + sizeof(NewStats), 0);

	PACKET_NEWRESISTANCE NewRes;
	memset(&NewRes, 0, sizeof(NewRes));

	NewRes.fire  = tChar.prot_Fire;
	NewRes.air	 = tChar.prot_Air;
	NewRes.earth = tChar.prot_Earth;
	NewRes.water = tChar.prot_Water;
	
	//Radian
	//printf("\nPACKET_NEWRESISTANCE:  %d  %d  %d  %d\n", NewRes.fire, NewRes.air, NewRes.earth, NewRes.water);

	buffer[0] = 30;
	buffer[1] = 8;
	memcpy(&buffer[2], &NewRes, sizeof(NewRes));

	send(Clients[UsedClient].ClientSocket, buffer, 2 + sizeof(NewRes), 0);
}

void Account::informGuild()
{
	//if (uChar->Guild == 0) return;

	p_Guildinfo nGuildInfo;

	if (uChar->Guild == 0)
	{
		nGuildInfo.guild = 0;
		strcpy(nGuildInfo.tag, uChar->Tag);
	} else {
		nGuildInfo.guild = uChar->Guild;
		strcpy(nGuildInfo.tag, uChar->Tag);
	}

	buffer[0] = 8;
	buffer[1] = 4;
	memcpy(&buffer[2], &nGuildInfo, sizeof(nGuildInfo));
	send(Clients[UsedClient].ClientSocket, buffer, 2 + sizeof(nGuildInfo), 0);
}



p_OwnCharInfo *Account::CreateCharInfo()
{
	p_OwnCharInfo *tmp;
	tmp = new p_OwnCharInfo;

	tmp->Avatar					= uChar->Avatar;
//	tmp->Class					= uChar->Class;
	tmp->Con					= uChar->Con;
	tmp->crimCount				= uChar->CrimCount;
	tmp->devMode				= uChar->DevMode;
	tmp->Dex					= uChar->Dex;
	tmp->Exp					= uChar->Exp;
	tmp->ExpPool				= uChar->ExpPool;
	tmp->Guild					= uChar->Guild;
	tmp->hp						= uChar->HPLeft;
	tmp->hpMax					= uChar->HPMax;
	tmp->ID						= uChar->ID;
	tmp->Int					= uChar->Int;
	tmp->Level					= uChar->Level;
	tmp->MapLevel				= uChar->Maplevel;
	tmp->Race					= uChar->Race;
	tmp->Reputation				= uChar->Reputation;
	tmp->Statpoints				= uChar->StatPoints;
	tmp->KilledMonsters			= uChar->KilledMonsters;
	tmp->Str					= uChar->Str;
	tmp->X						= uChar->X;
	tmp->Y						= uChar->Y;
	strcpy(tmp->Name,			uChar->Name);
	strcpy(tmp->Tag,			uChar->Tag);
	memcpy(&tmp->Status,		&uChar->Status, sizeof(tmp->Status));
//	memcpy(&tmp->Statusmode,	&uChar->Statusmode, sizeof(tmp->Statusmode));
	memcpy(&tmp->Skills,		&uChar->Skills, sizeof(tmp->Skills));
	
	return tmp;
}

bool IPBanned(char *name)
{
	char compare[25];
	memset (compare, 0, 25);
	char curchar;

	int i = 0;

	ifstream nameFile("ipbans.txt", ios::in);
	if (!nameFile.is_open()) {
		cout << "ipbans file can't be opened!\n";
		return 1;
	}
	nameFile.seekg(0, ios::beg);
	do 
	{
		do 
		{
			nameFile.get(curchar);
			if (nameFile.good() && curchar != '\n')
				compare[i] = curchar;
			i++;
		} while (nameFile.good() && curchar != '\n');

		if (!strcmpi(compare, name)) 
		{
			nameFile.close();
			return 1;
		}
	i = 0;

	} while (nameFile.good());
	memset (compare, 0, 25);

	nameFile.close();

	return 0;
}


int loadguilds() 
{
	ifstream filet("guilds.vsf", ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Guilds, sizeof(Guilds));
		cout << "Guilds loaded." << endl;
	
		filet.close();
	}
	else
	{
		cout << "Couldn't open guilds.vsf" << endl;
		cout << "Account file doesn't exist! Created new file." << endl;

		char const* saveFile = "-File Created-";
		ofstream zFile("guilds.vsf", ios::binary);
		zFile.write((char*)saveFile, sizeof(saveFile));
		zFile.close();
	}

	return 1;
}


int saveguilds() 
{
	ofstream filet("guilds.vsf", ios::binary);

	if (filet.is_open())
	{
		filet.write((char *)&Guilds, sizeof(Guilds));
		cout << "Guilds saved." << endl;
	
		filet.close();
	}
	else
	{
		cout << "Couldn't open guilds.vsf" << endl;		
	}
	return 1;
}


