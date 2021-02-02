
#pragma comment(lib, "wsock32.lib")

#include <iostream>
#include <time.h>
#include <string.h>

#define STDIN	0

#include "main.h"


#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:4018)

struct authstruct auth[4] = 
{
	{ "radiankh", "omgwtf" },{ "admin", "login" },
};


bool				Done						= false;
bool				Debug						= true;
bool				UsedItemID[MaxItems];
bool				UsedMonsterID[m_MonsterIDs];
bool				UsedNPCID[m_NPCIDs];
bool				UsedMonsterSpawns[MaxMonsterSpawns];
bool				UsedPlayerID[Max_Connections];
bool				Failsafe;
bool				Objects_Use[25];
bool				CanAttack = false;


char				*buffer						= new char[512];
char				*buffer2					= new char[512];
char				*buffer3					= new char[512];
char				*buffer4					= new char[2720];
char				LastAccountCreates[50][25];
char				MoTD[8][60];
char				LineType[8];
char				Time[20], Date[20];
char				Msg[120];


unsigned char		Ores[512][512];


short				LastJail					= 0;
short				LastCreated;
short				SafeZones[512][512];


int					ClientsConnected			= 0;
int					ClientsConnectedtoShow		= 0;
int					mon_loopi, mon_loopj;


long				LastStaticID;
long				Ground[MaxItems];


unsigned long		TimeoutandSaveTime			= 0;
unsigned long		AttackCheck					= 0;
unsigned long		Autosavetime				= 0;
unsigned long		MonsterActionTime;
unsigned long		LastSpawnTime;
unsigned long		PlayerActionTime;
unsigned long		GroundTime[MaxItems];
unsigned long		CheckObjectsTime;
unsigned long		Objects_UseTime[25];
unsigned long		TimeCheckTime;
unsigned long		ReplaceOreTime;



ClientClass			Clients[Max_Connections];
AccountToEdit		accToEdit;
CharacterToEdit		charToEdit;


MapClass			Map;
TileClass			Tiles[m_Tiles];
GuildClass			Guilds[MaxGuilds];
BodyClass			Body[m_Bodies];


MiscPicClass		NewPic;
NewCombatTextClass	NewCT;
NewHPClass			NewHP;


PACKET_NEWAMOUNT	NewAmount;
PACKET_MONSTER		NewMonster;
PACKET_NPC			NewNPC;
PACKET_ITEMMOVE		ItemMove;
PACKET_SHOPITEM		NewShopItem;
PACKET_STATS_BASE	NewStats_base;
PACKET_CHARCREATION	CharCreation;
PACKET_BODYINFOSTRUCT NewBodyInfo;


struct sockaddr_in	Server_Address;
SOCKET				ListeningSocket;
timeval				timeout; 



UseableItemStruct	Useables[m_Items_per_type];
MiscItemStruct		Collectables[m_Items_per_type];
ArmorInfo			Armors[m_Item_Types][m_Items_per_type];
WeaponInfo			Weapons[m_Item_Types][m_Items_per_type];


ShopStorageClass	ShopStorages[m_Storages];
NPCClass			*NPC[128][128];
NPCClass			*npc_List;
NPCClass			*npc2_List;
NPCClass			*npc_Failsafe = NULL;
NPCClass			*npclooplist = NULL;
NPCTypesStruct		NPCTypes[m_NPCTypes];
NPCSpawnStruct		*NPCSpawns;


MonsterClass		*Monsters[128][128];
MonsterClass		*mon_List = NULL;
MonsterClass		*mon2_List = NULL;
MonsterClass		*mon_Failsafe = NULL;


MonsterTypesStruct	MonsterTypes[m_MonsterTypes];
MonsterSpawnStruct	*MonsterSpawns = NULL;


MapObjects			*Objects[128][128];
MapObjects			*obj_List = NULL;
ObjectsStruct		ObjectTypes[ m_Object_Types ];
MapObjects			*UseTargets[25];

ItemStruct			Items[m_Items];

SecureTradeClass	SecureTrade[10];


RaceInfo			Races[6];
AccountManager		*AccManager = NULL;



CrimSpawnList		*crimSpawns = NULL;


ofstream			login("logs/logins.txt", ios::app);
ofstream			flogin("logs/flogins.txt", ios::app);
ofstream			acccreation("logs/acccre.txt", ios::app);
ofstream			shop("logs/shop.txt", ios::app);
ofstream			normalchat("logs/nchat.txt", ios::app);
ofstream			globalchat("logs/gchat.txt", ios::app);
ofstream			whisperchat("logs/wchat.txt", ios::app);
ofstream			gmchat("logs/gmchat.txt", ios::app);
ofstream			guildchat("logs/guildchat.txt", ios::app);
ofstream			GMLOG("logs/telepother.txt", ios::app);
ofstream			GMLOG2("logs/GMKills.txt", ios::app);
ofstream			GMACTION("logs/GMlogs.txt", ios::app);
ofstream			drops("logs/drops.txt", ios::app);
ofstream			dropz("logs/dropz.txt", ios::app);
ofstream			debuggi("debug.txt", ios::app);
ofstream			abusers("abusers.txt", ios::app);
ofstream			ipbans("ipbans.txt", ios::app);




int main()
{
	srand(time(NULL));

	SetConsoleTitleA("Bunnkia Server");
	printf("CharacterData = %d\n", sizeof(CharacterData));
	printf("p_owncharinfo = %d\n", sizeof(p_OwnCharInfo));
	printf("AccountToEdit = %d\n", sizeof(AccountToEdit));


	Setup();
	CreateTables();
	atexit(Shutdown_Server);


	CreateDirectoryA("GM personal logs", NULL);
	printf("Set Up() completed.\n");


	memset(&Ores, 25, sizeof(Ores));
	memset(&Monsters, 0, sizeof(Monsters));
	memset(&LastAccountCreates, 0, sizeof(LastAccountCreates));

	TimeoutandSaveTime	= GetTickCount64();
	AttackCheck			= GetTickCount64();
	Autosavetime		= GetTickCount64();
	ReplaceOreTime		= GetTickCount64();
	LastSpawnTime		= GetTickCount64();


	CheckMonsterSpawn();
	CheckNPCSpawn();
	_tzset();
	loadguilds();
	LoadRaces();

	login << "----------------------------------------------------------------------" << endl;
	login << " New server session. " << Date << " " << Time							  << endl;
	login << "----------------------------------------------------------------------" << endl;

	flogin << "----------------------------------------------------------------------" << endl;
	flogin << " New server session. " << Date << " " << Time							  << endl;
	flogin << "----------------------------------------------------------------------" << endl;

	acccreation << "----------------------------------------------------------------------" << endl;
	acccreation << " New server session. " << Date << " " << Time							  << endl;
	acccreation << "----------------------------------------------------------------------" << endl;

	shop << "----------------------------------------------------------------------" << endl;
	shop << " New server session. " << Date << " " << Time							  << endl;
	shop << "----------------------------------------------------------------------" << endl;

	normalchat << "----------------------------------------------------------------------" << endl;
	normalchat << " New server session. " << Date << " " << Time							  << endl;
	normalchat << "----------------------------------------------------------------------" << endl;

	globalchat << "----------------------------------------------------------------------" << endl;
	globalchat << " New server session. " << Date << " " << Time							  << endl;
	globalchat << "----------------------------------------------------------------------" << endl;

	whisperchat << "----------------------------------------------------------------------" << endl;
	whisperchat << " New server session. " << Date << " " << Time							  << endl;
	whisperchat << "----------------------------------------------------------------------" << endl;

	gmchat << "----------------------------------------------------------------------" << endl;
	gmchat << " New server session. " << Date << " " << Time							  << endl;
	gmchat << "----------------------------------------------------------------------" << endl;

	guildchat << "----------------------------------------------------------------------" << endl;
	guildchat << " New server session. " << Date << " " << Time							  << endl;
	guildchat << "----------------------------------------------------------------------" << endl;

	GMLOG << "----------------------------------------------------------------------" << endl;
	GMLOG << " New server session. " << Date << " " << Time							  << endl;
	GMLOG << "----------------------------------------------------------------------" << endl;

	GMLOG2 << "----------------------------------------------------------------------" << endl;
	GMLOG2 << " New server session. " << Date << " " << Time							  << endl;
	GMLOG2 << "----------------------------------------------------------------------" << endl;

	GMACTION << "----------------------------------------------------------------------" << endl;
	GMACTION << " New server session. " << Date << " " << Time							  << endl;
	GMACTION << "----------------------------------------------------------------------" << endl;

	dropz << "----------------------------------------------------------------------" << endl;
	dropz << " New server session. " << Date << " " << Time							  << endl;
	dropz << "----------------------------------------------------------------------" << endl;

	drops << "----------------------------------------------------------------------" << endl;
	drops << " New server session. " << Date << " " << Time							  << endl;
	drops << "----------------------------------------------------------------------" << endl;
	
	debuggi << "----------------------------------------------------------------------" << endl;
	debuggi << " New server session. " << Date << " " << Time							  << endl;
	debuggi << "----------------------------------------------------------------------" << endl;

	abusers << "----------------------------------------------------------------------" << endl;
	abusers << " New server session. " << Date << " " << Time							  << endl;
	abusers << "----------------------------------------------------------------------" << endl;



	int oindex, ojindex;
	for (oindex = 0; oindex < 128; oindex++)
	{
		for (ojindex = 0; ojindex < 128; ojindex++)
		{
			obj_List = Objects[ojindex][ojindex];
			while (obj_List != NULL)
			{
				Objects[ojindex][ojindex]->On = true;
                
				obj_List = obj_List->next;			
			}		
		}	
	}


	//=== LOAD file containing last 50 IPs from what accounts have been created
	//	  This will prevent spam-account creation, since we don't have e-mail system yet.
	printf("Loading Account creations history");

	fstream filet;
	filet.open ("accrehistory.vdt", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&LastAccountCreates, sizeof(LastAccountCreates));			
		filet.close();
		printf("...done.\n");
	}
	else
	{
		memset(&LastAccountCreates, 0, sizeof(LastAccountCreates));
		printf("..failed.\n");
	}
	//=== END LOAD


	printf("Loading safe zones");

	filet.open ("data/safezones.dat", ios::in | ios::binary);
	if (filet.is_open())
	{
		filet.read((char *)&SafeZones, sizeof(SafeZones));
		filet.close();
		printf("...done.\n");
	}
	else
	{
		memset(&SafeZones, 0, sizeof(SafeZones));
		printf("..failed.\n");
	}


	// Newest new, first totally C++ thingie in the server code.
	AccManager = new AccountManager();
	AccManager->LoadAccounts();


	unsigned long LastDotTime = 0;

	while (!Done)
	{

		// Update timers. Since there's no timer class, it's global.
		if (GetTickCount64() - TimeCheckTime > 50)
		{
		    _strtime( Time );
			_strdate( Date );
			TimeCheckTime = GetTickCount64();
		}


		// Check incoming connections, if someone is connecting.
		// Since there won't be connections coming nonstop, we can have little delay
		// To save CPU time for other things.
		if (GetTickCount64() - LastDotTime > 10)
		{
			Check_Connections();
			LastDotTime = GetTickCount64();
		}


		// Check for incoming messages from clients.
		Check_Message();


		// Big loop which could be in whole different function to save some space.
		// Checks for clients that timeout, and account/guild/item saves.
		if (GetTickCount64() - TimeoutandSaveTime > 3000)
    {
			int i, j;

			for (i = 0; i < Max_Connections; i++)
			{
				if (!Clients[i].Used && Clients[i].Acc != NULL)
				{
					Clients[i].Acc->NotUsed();
					memset(&Clients[i], 0, sizeof(Clients[i]));
				}
				if (Clients[i].Used)
				{
					if (GetTickCount64() - Clients[i].LstMsg > 15000)
					{
						if (Clients[i].Acc == NULL || Clients[i].State == 5)
						{
							if (GetTickCount64() - Clients[i].LstMsg > 15000)
								memset(&Clients[i], 0, sizeof(Clients[i]));
						}
						else if (Clients[i].Acc->getAccID() != 0)
						{
							if (!Clients[i].TimeoutNo && Clients[i].State != 5)
							{
								//-----------------------------------------
								// Inform other players about this timeout
								//-----------------------------------------
		
								printf("%s: [Timeout %d]: %s\n", Time, i, Clients[i].Acc->getAccName());
			
								unsigned long accID;
								accID = Clients[i].Acc->getAccID();
						        buffer[0] = 11;
								buffer[1] = 1;
								memcpy(&buffer[2], &accID, sizeof(accID));
		
								for (j = 0; j < Max_Connections; j++)
								{
									if (Clients[j].Used && Clients[j].State != 5)
									{
									    if (j != i)
										{
											if (Clients[i].uChar->X - Clients[j].uChar->X >= -10 &&
												Clients[i].uChar->X - Clients[j].uChar->X <= 10 &&
												Clients[i].uChar->Y - Clients[j].uChar->Y >= -10 &&
												Clients[i].uChar->Y - Clients[j].uChar->Y <= 10)
												send(Clients[j].ClientSocket, buffer, 2 + sizeof(accID), 0);							
										}
										Clients[j].KnowPlayer[Clients[i].Know_ID] = false;
									}
								}

								Disconnect(i);
							}
							else  Disconnect(i);
						}
					}
				}
			}

			AccManager->CheckSave();
		
			TimeoutandSaveTime = GetTickCount64();
		}


		// Check for what monsters and NPCs are doing around.. This is basically the main function for
		// NPC and Monster actions!!! Takes MOST OF THE CPU TIME! There must be number of ways to fasten
		// this function that you can have a lot more monsters / npcs ingame. Could also be in own function
		// to make it clearer to read.
		if (GetTickCount64() - MonsterActionTime > MonsterActionCheckTime)
		{
			for (mon_loopi = 0; mon_loopi < 128; mon_loopi++)
			{
				for (mon_loopj = 0; mon_loopj < 128; mon_loopj++)
        {
					mon_List = Monsters[mon_loopi][mon_loopj];

					while (mon_List != NULL)
					{
						Failsafe = false;
						mon_Failsafe = mon_List->next;
						if (mon_List->Target != NULL || mon_List->NPCTarget != NULL)
						{
							if (mon_List->Target != NULL)
							{
								if (!IsInRange( mon_List->X, mon_List->Y, mon_List->Target->uChar->X, mon_List->Target->uChar->Y))
								{
									mon_List->Target = NULL;							
								}
							}
							else if (mon_List->NPCTarget != NULL)
							{
								if (!IsInRange( mon_List->X, mon_List->Y, mon_List->NPCTarget->X, mon_List->NPCTarget->Y))
								{
									mon_List->NPCTarget = NULL;							
								}
							}

							if (mon_List->Target != NULL || mon_List->NPCTarget != NULL)
                                MonsterAction_HasTarget( mon_List );
						}
						else 
							CheckTarget(mon_List);

						if (GetTickCount64() - mon_List->LstReg >= 4000)
							MonsterAction_Regenerate( mon_List );
											
						if (Failsafe)
							mon_List = mon_Failsafe;
						else if (mon_List != NULL)
							mon_List = mon_List->next;
					}

					npclooplist = NPC[mon_loopi][mon_loopj];

					while (npclooplist != NULL)
					{
						npc_Failsafe = npclooplist->next;

						if (NPCTypes[ npclooplist->Type ].is_guard)
						{
							if ( npclooplist->NPCPLTarget != -1 || npclooplist->NPCMonsterTarget != NULL)
								NPCAction_HasTarget( npclooplist );
							else 
								CheckNPCTarget( npclooplist );

							if (npclooplist->X != npclooplist->oX || npclooplist->Y != npclooplist->oY)
							{
								if (NPCTypes[ npclooplist->Type ].is_guard && GetTickCount64() - npclooplist->LstAttack > 15000)
									NPCAction_Respawn( npclooplist );
							}
						}
						else
						if (NPCTypes[ npclooplist->Type ].Walking)
						{
							if (GetTickCount64() - npclooplist->LstMove > 1800 - rand() % 1600)
							{
								if (NPCTypes[ npclooplist->Type ].WalkingType == walk_LIMITS)
									NPCAction_WalkLimits( npclooplist );
								if (NPCTypes[ npclooplist->Type ].WalkingType == walk_RANDOM)
									NPCAction_WalkRandom( npclooplist );
							}
						}

						npclooplist = npc_Failsafe;
					}
				}
			}

			MonsterActionTime = GetTickCount64();
		}


		// Check for monster and NPC spawn, and also for items disappearing that are on ground.
		if (GetTickCount64() - LastSpawnTime > MonsterSpawnTimeCheck)
		{
			printf("%s: [Monster Spawn]\n", Time);

			CheckMonsterSpawn();
			CheckNPCSpawn();

			if (Debug) printf("Monsterspawn debug,");

			int i;
			for (i = 0; i < MaxItems; i++)
			{
				if (Ground[i] > 0)
				{
					if (GetTickCount64() - GroundTime[i] > 180000)
					{
						if (Debug) printf("remove item %d,", i);
						RemoveItemFromMass( i );
						
						memset(&Items[Ground[i]], 0, sizeof(Items[Ground[i]]));
						Ground[i] = 0;
					}
				}
			}

			if (Debug) printf("Monster spawn debug end.\n");
			LastSpawnTime = GetTickCount64();
		}


		// Check for some player functionality, like attack, jail and things like that.
		if (GetTickCount64() - PlayerActionTime > PlayerActionTimeCheck)
		{
			int i;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (Clients[i].IsInJail)
					{
						if (GetTickCount64() - Clients[i].LstJailtime >= 60000)
						{
							Clients[i].uChar->CrimCount++;
							InformPlayerofNewCrimCounts(i);
							if (Clients[i].uChar->CrimCount >= 0)
							{
								ReleaseFromJail(i);
							}
							Clients[i].LstJailtime = GetTickCount64();
						}
					}
					if (Clients[i].AttackTarget == 0 && Clients[i].PLTarget != -1)
					{
						Clients[i].MonTarget = NULL;
						CanAttack = false;
	
						if (Clients[i].Weapon > 0)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].Speed) //- Skill)
								CanAttack = true;
						}
						if (Clients[i].Weapon == 0)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= 1400) //- Skill)
								CanAttack = true;
						}
	
						if (CanAttack)
						{
							PlayerAction_AttackToPlayer( i );
						}
					}	
					if (Clients[i].AttackTarget == 1 && Clients[i].MonTarget != NULL)
					{
						Clients[i].PLTarget = NULL;
						CanAttack = false;
	
						if (Clients[i].Weapon != NULL)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].Speed) //- Skill)
								CanAttack = true;
						}
						if (Clients[i].Weapon == NULL)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= 1400) //- Skill)
								CanAttack = true;
						}
	
						if (CanAttack)
						{
							PlayerAction_AttackToMonster( i);
						}
					}
					if (Clients[i].AttackTarget == 2 && Clients[i].NPCTarget != NULL)
					{
						Clients[i].MonTarget = NULL;
						Clients[i].PLTarget = NULL;
						CanAttack = false;
	
						if (Clients[i].Weapon != NULL)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].Speed) //- Skill)
								CanAttack = true;
						}
						if (Clients[i].Weapon == NULL)
						{
							if (GetTickCount64() - Clients[i].LstAtk >= 1400) //- Skill)
								CanAttack = true;
						}
	
						if (CanAttack)
						{
							PlayerAction_AttackToNPC( i );
						}
					}

					if (GetTickCount64() - Clients[i].LstReg >= 4000 && Clients[i].uChar->HPLeft < Clients[i].Acc->Calc_hpMax())
					{
						// Regenerate HP
						short Damage;
						Damage = Clients[i].Acc->tChar.Con / 4;
	
						Clients[i].uChar->HPLeft += Damage;
	
						if (Clients[i].uChar->HPLeft > Clients[i].Acc->Calc_hpMax())
							Clients[i].uChar->HPLeft = Clients[i].Acc->Calc_hpMax();
		
						memset(&NewHP, 0, sizeof(NewHP));
	
						NewHP.KnowID = Clients[i].Acc->getAccID();
						NewHP.NewHP = Clients[i].uChar->HPLeft;
						NewHP.Type = 0;
		
						// REGEN HP
						buffer[0] = 97;
						buffer[1] = 1;

						NewCT.Amount = NewHP.NewHP;
						NewCT.Type = CT_REGEN;
						NewCT.FromTo = 0;
						NewCT.SkillType = 1;

						memcpy(&buffer[2], &NewCT, sizeof(NewCT));

						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

						buffer[0] = 20;
						buffer[1] = 3;
				
						memcpy(&buffer[2], &NewHP, sizeof(NewHP));
	
						short ox,oy,tx,ty;
						ox = Clients[i].uChar->X;
						oy = Clients[i].uChar->Y;

						int j;
						for (j = 0; j < Max_Connections; j++)
						{
							if (Clients[j].Used && Clients[j].State != 5)
							{
								tx = Clients[i].uChar->X;
								ty = Clients[i].uChar->Y;
								if (IsInRange(ox, oy, tx, ty))
								{
									if (Clients[j].KnowPlayer[i] || j == i)
										send(Clients[j].ClientSocket, buffer, 2 + sizeof( NewHP), 0);
								}
							}
						}
	
						Clients[i].LstReg = GetTickCount64();	
					}
					if (GetTickCount64() - Clients[i].lst_tradeskill > 3000 && Clients[i].tradeskill_inuse)
					{
						Clients[i].use_skill();
					}
				}
			}		
		
			PlayerActionTime = GetTickCount64();
		}



		// Check object things, like closing doors. Also check for body decay and safetrade things.
		if (GetTickCount64() - CheckObjectsTime > 500)
		{
			int i;
			for (i = 0; i < 25; i++)
			{
				if (Objects_Use[i])
				{
					if (GetTickCount64() - Objects_UseTime[i] > 2000 && UseTargets[i]->Type == 1)
					{
						if (!UseTargets[i]->deuse(-1))
						{
							Objects_UseTime[i] = GetTickCount64();
						}
						else
						{
							Objects_Use[i] = false;

							UseTargets[i] = NULL;
						}
					}
				}
			}
			for (i = 0; i < m_Bodies; i++)
			{
				if (Body[i].Used)
				{
					if (GetTickCount64() - Body[i].DecayStateTime > body_DecayTime)
					{
						Body[i].DecayState++;
						Body[i].DecayStateTime = GetTickCount64();

						InformMassOfNewBodyDecay( i );
					}
                    
					if (Body[i].DecayState >= 5)
					{
						// Remove body
                        InformMassOfNewBodyDecay( i );
						RemoveBody( i );
					}
				}			
			}

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].TradeInPending && !SecureTrade[i].TradeInProgress)
					if (GetTickCount64() - SecureTrade[i].LastAction > 4000)
						SecureTrade[i].Abort();

				if (!SecureTrade[i].TradeInPending && SecureTrade[i].TradeInProgress)
					if (GetTickCount64() - SecureTrade[i].LastAction > 20000)
						SecureTrade[i].Abort();
			}				

			CheckObjectsTime = GetTickCount64(); 
		}

		// Replace mountains with ores.
		if (GetTickCount64() - ReplaceOreTime > 600000)
		{
			if (Debug) printf("Check ores,");
			memset(&Ores, 25, sizeof(Ores));
	
			ReplaceOreTime = GetTickCount64();
			if (Debug) printf("check done.\n");
			
		}

		// Free some CPU time for system idle process. If comp is dedicated, removing this line will
		// speed everything up VERY VERY VERY much (tested.)
		//Sleep(1);
	}	


	// Bye bye.
	return (1);
}



// Wtf is this, even I don't know...
bool Error(char *error)
{
	printf("%s\n", error);
	return (true);
}


// Disconnect player, and remove all links to the client... Must be easier ways to do this too.
// Maybe MonsterManager and NPCManager would help. ;)
int Disconnect(short Player)
{
	AccManager->SaveAccounts();
	if (Clients[Player].Acc != NULL)
		printf("%s: [Disconnect]: %s\n", Time, Clients[Player].Acc->getAccName());

	char *ClientIp = inet_ntoa(Clients[Player].clnt_addr.sin_addr);

	// Remove all targets to this Player.

	if (Clients[Player].uChar != NULL)
	{
		if (Clients[Player].uChar->DevMode > 0)
		{
			char tmp[80];
			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [LOGOUT]\n", Date, Time);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

			Clients[Player].logfile->close();
			delete Clients[Player].logfile;
			Clients[Player].logfile = NULL;

			if (Clients[Player].isfollowing >= 0) Clients[Clients[Player].isfollowing].beingfollowed[0] = -1;
		}

		for (int i = 0; i < 128; i++)
		{
			for (int j = 0; j < 128; j++)
			{			
				mon_List = Monsters[i][j];
		
				while (mon_List != NULL)
				{
					if (mon_List->Target == &Clients[Player])
						mon_List->Target = NULL;
				
					mon_List = mon_List->next;
				}

				npc_List = NPC[i][j];

				while (npc_List != NULL)
				{
					if (npc_List->NPCPLTarget == Clients[Player].Acc->getAccID())
						npc_List->NPCPLTarget = -1;
				
					npc_List = npc_List->next;
				}
			}	
		}
	}

	Clients[Player].State = 5;

	closesocket(Clients[Player].ClientSocket);

	if (Clients[Player].Acc != NULL)
	{
		unsigned long accID = Clients[Player].Acc->getAccID();

		buffer[0] = 11;
		buffer[1] = 4;
		memcpy(&buffer[2], &accID, sizeof(accID));
		
		if (Clients[Player].preLogin)
			Clients[Player].Acc->NotUsed();
		
		memset(&Clients[Player], 0, sizeof(Clients[Player]));
		CalcClientsConnected();

		for (int i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				if (Clients[i].PLTarget == Player && i != Player)
					Clients[i].PLTarget = -1;

				if (i != Player && Clients[i].uChar->DevMode > 0)
				{
					InformGMofOnlineAmount(i);
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(long), 0);
				}
			}
		}
	}
	else
	{
		memset(&Clients[Player], 0, sizeof(Clients[Player]));
		CalcClientsConnected();
	}

	return(1);
}



// This has never happened. NEVER. Even if server is closed from [X], this doesn't run. :D
void Shutdown_Server()
{
	// Close The Socket that Listens for Connections
	closesocket(ListeningSocket);
	
	printf("saving Account creations history");
	// Load Acc Creations
	fstream filet;
	filet.open ("accrehistory.vdt", ios::out | ios::binary);

	if (filet.is_open())
	{
		filet.write((char *)&LastAccountCreates, sizeof(LastAccountCreates));			
		filet.close();
		printf("...done.\n");
	}

	AccManager->SaveAccounts();

	// Remove Winsock
	WSACleanup();
}

void Save_Creations()
{
	printf("saving Account creations history");
	// Load Acc Creations
	fstream filet;
	filet.open ("accrehistory.vdt", ios::out | ios::binary);

	if (filet.is_open())
	{
		filet.write((char *)&LastAccountCreates, sizeof(LastAccountCreates));			
		filet.close();
		printf("...done.\n");
	}
}



// Checking for incoming connections. Check Cyldan server code to get better version. PeerClass is good
// in there too.
int Check_Connections()
{	
	int s;
	fd_set conn;
	int Client_Length = sizeof(sockaddr_in);

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	
	if (ClientsConnected < Max_Connections)
	{
		FD_ZERO(&conn);
		FD_SET(ListeningSocket, &conn);
		s = select(STDIN + 1, &conn, NULL, NULL, &timeout);

		if (s > 0)
		{
			short i;
			i = GetFreeClient();
			if (i == -1) return 0;

			memset(&Clients[i], 0, sizeof(Clients[i]));
			Clients[i].PLTarget = -1;
			Clients[i].Used = true;

			Clients[i].ClientSocket = accept(ListeningSocket, (struct sockaddr*)&Clients[i].clnt_addr, &Client_Length);

			char *ClientIp = inet_ntoa(Clients[i].clnt_addr.sin_addr);

			buffer[0] = 1;
			buffer[1] = 8;
			send(Clients[i].ClientSocket, buffer, 2, 0);


			// IPBanned function kinda sucks.. But it has possibilities. When you trace IP to some ISP, 
			// you get IP range  that the ISP is holding. Then you can just enter all the IPs to 
			// IPBanned.txt, and that ISP is banned!
			if (IPBanned(ClientIp))
			{
				printf("Blocked connection attempt from banned IP %s.\n", ClientIp);

				buffer[0] = 12;
				send(Clients[i].ClientSocket, buffer, 1, 0);

				memset(&Clients[i], 0, sizeof(Clients[i]));


				return 0;
			}


			// Following feature is bad code... But works in most cases.
			if (Clients[i].isfollowing != -1) Clients[i].isfollowing = -1;
			if (Clients[i].beingfollowed[0] != -1) Clients[i].beingfollowed[0] = -1;
			if (Clients[i].beingfollowed[1] != -1) Clients[i].beingfollowed[1] = -1;


			ClientsConnected++;
			Clients[i].Know_ID = i;
	
			// State 5 is IDLE, State 0 is logged in with character 1, state 1 is logged in with char 2.
			Clients[i].State = 5;

			// Preventing packet hack which Wipe came up with. Won't accept login packets after logged in.
			Clients[i].Login = false;
		}
	}

	return 1;
}

short GetFreeClient()
{
	short i;
	for (i = 1; i < Max_Connections; i++)
	{
		if (!Clients[i].Used)
		{
			Clients[i].Used = true;
			return i;
		}
	}
	printf("%s %s: [Error] at GetFreeClient(); -> No free IDs left.\n", Time, Date);
    return -1;
}


// Handle the message from client. Check_Message() function will call this.
void Get_Message( short Player)
{
	//memset(buffer, 0, sizeof(char) * 512);
	buffer[0] = 0;
	buffer[1] = 0;

	recv(Clients[Player].ClientSocket, buffer, 1, 0);

	char *ClientIp = inet_ntoa(Clients[Player].clnt_addr.sin_addr);

	int i;
	if (buffer[0] != 120 && buffer[0] != 0)
	{
		Clients[Player].LstMsg = GetTickCount64();
	}



	// The following packet handling is bad.. I should have made different functions for EACH cases.
	// But I decided to go on like this, because Dransik had the same.


//==============================================================================
//
//	buffer[0] definitions
//	[1]				=		Account creation
//	[2]				=		Login packet received
//	[3]				=		Char chosen
//	[4]				=		*Char creation
//	[5]				=		Send infos to player
//	[6]				=		
//	[7]				=		
//	[8]				=		
//	[9]				=		
//	[10]			=		
//	[11]			=		
//	[12]			=		
//	[13]			=		
//	[14]			=		
//	[15]			=		
//	[16]			=		
//	[17]			=		
//	[18]			=		
//	[19]			=		
//	[20]			=		
//	[21]			=		
//
//==============================================================================


	switch (buffer[0])
	{
	case 1: // Account creation

		p_ACreationpacket pACreate;

		recv(Clients[Player].ClientSocket, buffer, sizeof(pACreate), 0);
		memcpy(&pACreate, buffer, sizeof(pACreate));

		cout << Date << " " << Time << " - Account creation: IP " << ClientIp << " Account name " << pACreate.Name << " Account pass " << pACreate.Pass << endl;

		if (pACreate.Version < _VERSION)
		{
			cout << Date << " " << Time << " -      -> Error, Client has wrong version." << endl;
			buffer[0] = 1;
			buffer[1] = 2;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			Disconnect(Player);
			break;
		}

		Account *tmp;
		tmp = AccManager->GetByName(pACreate.Name);
		
		if (tmp != NULL)
		{
			cout << Date << " " << Time << " -      -> Error, Account name taken." << endl;
			buffer[0] = 1;
			buffer[1] = 10;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			Disconnect(Player);
			break;
		}

		/*tmp = AccManager->GetByEmail(pACreate.Email);
		
		if (tmp != NULL)
		{
			cout << Date << " " << Time << " -      -> Error, EMail registered." << endl;
			buffer[0] = 1;
			buffer[1] = 11;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			Disconnect(Player);
			break;
		}*/

		if (tmp == NULL)
		{

			bool iError;
			iError = false;
		/*
			for (i = 0; i < 50; i++)
			{
				if (!strcmp(ClientIp, LastAccountCreates[i]))
				{
					iError = true;
					i = 50;
				}			
			}
		*/

			if (iError)
			{
				cout << Date << " " << Time << " -      -> Error, IP address recently used." << endl;
				Disconnect( Player);
				break;
			}

			acccreation << Date << " " << Time << "[Account creation][SUCCEED]::[IP]: " << ClientIp << " [Account name]: " << pACreate.Name << " [Account pass]: " << pACreate.Pass << endl;

			Clients[Player].Acc = AccManager->NewAccount(&pACreate);
			Clients[Player].Acc->StartUsing(Player);

			Clients[Player].preLogin = true;


			buffer[0] = 1;
			buffer[1] = 9;				
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			for(i = 1; i < 49; i++)
			{
				strcpy(LastAccountCreates[i+1], LastAccountCreates[i]);
			}
			strcpy(LastAccountCreates[0], ClientIp);
		}

		break;

	// Login packet received
	case 2:

		if (Clients[Player].State != 5) break;
		
		p_Loginpacket pLogin;

		recv(Clients[Player].ClientSocket, buffer, sizeof(pLogin), 0);
		memcpy(&pLogin, buffer, sizeof(pLogin));

		cout << Date << " " << Time << " - Login: IP " << ClientIp << " Account name " << pLogin.Name << " Account pass " << pLogin.Pass << endl;
		login << Date << " " << Time << " [IP]: " << ClientIp << " [Account name]: " << pLogin.Name << " [Account pass]: " << pLogin.Pass << endl;

		
		//if (pLogin.Version < _VERSION)
		//{
		//	cout << Date << " " << Time << " -      -> Error, Client has wrong version." << endl;
		//	buffer[0] = 1;
		//	buffer[1] = 2;
		//	send(Clients[Player].ClientSocket, buffer, 2, 0);
		//	Disconnect(Player);
		//	break;
		//}


		Clients[Player].Version = pLogin.Version;
		
		Clients[Player].Acc = AccManager->GetByName(pLogin.Name);

		if (Clients[Player].Acc == NULL)
		{
			// Failed to locate this account, informing client.
			flogin << Date << " " <<  Time << " [IP]: " << ClientIp << " [Reason] -> Account \"" << pLogin.Name << "\" doesn't exist." << endl;

			cout << Date << " " << Time << " -      -> Error, Account doesn't exist." << endl;

			buffer[0] = 1;
			buffer[1] = 4;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
            
			Disconnect (Player);
			break;
		}

		if (Clients[Player].Acc->isUsed())
		{
			if (Clients[Clients[Player].Acc->whoUsing()].Acc != Clients[Player].Acc)
				Clients[Player].Acc->NotUsed();
		}

		Clients[Player].AccountID = Clients[Player].Acc->getAccID();

		if (Clients[Player].Acc->isUsed())
		{
			// Account already in use.
            
			flogin << Date << " " << Time << " [IP]: " << ClientIp << " [Reason] -> Account \"" << pLogin.Name << ".acc\" is already logged in." << endl;

			cout << Date << " " << Time << " -      -> Error, Account already logged in." << endl;

			buffer[0] = 1;
			buffer[1] = 7;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			Disconnect (Player);
			break;
		}
		else
		{
			if (Clients[Player].Acc->ComparePass(pLogin.Pass))
			{
				flogin << Date << " " << Time << " [IP]: " << ClientIp << " Loading account \"" << pLogin.Name << "\" failed, user gave wrong password." << endl;

				cout << Date << " " << Time << " -      -> Error, wrong password." << endl;

				buffer[0] = 1;
				buffer[1] = 5;								
				send(Clients[Player].ClientSocket, buffer, 2, 0);

				Disconnect (Player);	
				break;
			}

			if (Clients[Player].Acc->IsBanned())
			{
				cout << Date << " " << Time << " -      -> Error, account banned." << endl;
				buffer[0] = 1;
				buffer[1] = 6;
				send(Clients[Player].ClientSocket, buffer, 2, 0);

				Disconnect(Player);
				break;
			}
			cout << Date << " " << Time << " -      -> Successfully logged in." << endl;

			Clients[Player].Acc->StartUsing(Player);
			Clients[Player].preLogin = true;

			p_CChoosepacket *pCChoosepacket;
			pCChoosepacket = Clients[Player].Acc->CopyCChoose();
	
			buffer[0] = 1;
			buffer[1] = 3;
			memcpy(&buffer[2], pCChoosepacket, sizeof(p_CChoosepacket));	
			send(Clients[Player].ClientSocket, buffer, 2 + sizeof(p_CChoosepacket), 0);

			Clients[Player].State = 5;

			delete pCChoosepacket;

			break;
		}

		break;

	case 3: //  Char chosen

		recv(Clients[Player].ClientSocket, buffer, 1, 0);
        
		short tryChar;
		tryChar = buffer[0];

		if (!Clients[Player].Acc->CharacterExists(tryChar) || tryChar < 0 || tryChar > 1)
		{
			Disconnect(Player);
			break;
		}

		Clients[Player].State = tryChar;

		Clients[Player].uChar = Clients[Player].Acc->GetCharPointer(tryChar);
		Clients[Player].Acc->uChar = Clients[Player].Acc->GetCharPointer(tryChar);

		if (Clients[Player].uChar->CharUpdate != _CHAR_UPDATE)
		{
			ResetStats(Player, tryChar);
			Clients[Player].uChar->CharUpdate = _CHAR_UPDATE;
		}

		Clients[Player].uChar->ID = Clients[Player].Acc->getAccID();

		printf("Race = %d\n", Clients[Player].uChar->Race);

		if (Clients[Player].Acc->uChar->Guild > 0)
		{
			Check_Guild(Player, tryChar);
			CheckGuildPlayers(Player);
		}

		if (Clients[Player].uChar->DevMode > 0)
		{
			Clients[Player].logfile = new ofstream;
			char tmp[80];
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "GM personal logs\\%s.txt", Clients[Player].uChar->Name);

			Clients[Player].logfile->open(tmp, ios::app);

			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "--------------------------------------------------------------------------\n");
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [LOGIN]\n", Date, Time);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
		}


		if (Clients[Player].uChar->CrimCount < 0)
		{
			Clients[Player].LstJailtime = GetTickCount64();
			Clients[Player].Acc->uChar->Wanted = true;

			InformMassofNewCriminalStatus(Player);
			
			if (SafeZones[Clients[Player].uChar->X >> 4][Clients[Player].uChar->Y >> 4] != 2)
			{
				Clients[Player].uChar->X = 296;
				Clients[Player].uChar->Y = 795;
			}
			Clients[Player].IsInJail = true;
		}

		Check_Aligment(Player);		

		p_OwnCharInfo *pOwnInfo;
		pOwnInfo = Clients[Player].Acc->CreateCharInfo();

		buffer4[0] = 1;
		buffer4[1] = 1;
		memcpy(&buffer4[2], pOwnInfo, sizeof(p_OwnCharInfo));
		send(Clients[Player].ClientSocket, buffer4, 2 + sizeof(p_OwnCharInfo), 0);

		delete pOwnInfo;

		break;

	case 5: // Send infos to player

		if (Clients[Player].Login) Disconnect(Player);

		Clients[Player].Login = true;

		CheckStats(Player);
		Clients[Player].Acc->UsedClient = Player;
//		Clients[Player].Acc->reportStats();
		InformPlayerofHisAllItems(Player);
		Clients[Player].Acc->informGuild();
//		Clients[Player].ReportStats();
		InformPlayerOfHisSkills(Player);
		InformSelfofCrimStatus(Player);
		Clients[Player].Acc->getStats();

		if (Clients[Player].Acc->uChar->Guild > 0)
		{
			Check_Guild(Player, Clients[Player].Acc->getAccID());
			CheckGuildPlayers(Player);
		}

		if (Clients[Player].IsLeader)
		{
			InformGuildLeader(Player);
		}

		sprintf(Msg, "Hail adventurer! Take heed, there is danger in this world!");
		Clients[Player].infoMsg(Msg); // infoMsg is new way to make server messages. Also MotD can be made with this.

		// Make GM's/Devs premium
		if (Clients[Player].uChar->DevMode > 1)
		{		
			if (!Clients[Player].Acc->isPremium())
			{				
				Account *thisAcc;

				thisAcc = AccManager->GetByCharName(Clients[Player].uChar->Name);
		
				AccountData *tmpacc;

				tmpacc = thisAcc->GetAccPointer();

				tmpacc->IsPremium = true;
				tmpacc->IsGolden = false;
			}
		}

		// find/display players account type
		if (Clients[Player].Acc->isPremium())
		{
			Account *thisAcc;

				thisAcc = AccManager->GetByCharName(Clients[Player].uChar->Name);
		
			AccountData *tmpacc;

				tmpacc = thisAcc->GetAccPointer();

				tmpacc->IsGolden = false;

			buffer[0] = 62;
			buffer[1] = 1;
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			buffer[0] = 62;
			buffer[1] = 2; // remove black from map
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			sprintf(Msg, "Account Type: Premium.");
			Clients[Player].infoMsg(Msg);
			
			printf("%s: [Account Type]: Premium\n", Time);
		} else if (Clients[Player].Acc->isGolden()){

			buffer[0] = 63;
			buffer[1] = 1;
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			buffer[0] = 63;
			buffer[1] = 2; // remove black from map
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			sprintf(Msg, "Account Type: Golden.");
			Clients[Player].infoMsg(Msg);

			printf("%s: [Account Type]: Golden\n", Time);

		} else {

			sprintf(Msg, "Account Type: Regular.");
			Clients[Player].infoMsg(Msg);
			//Clients[Player].uChar->DevMode = 6;

			printf("%s: [Account Type]: Regular\n", Time);	

		}

		// Nice new login message, implemented this in 0.32. Informing player of how much he has jail left when he logs in.
		if (Clients[Player].IsInJail) 
		{
			sprintf(Msg, "You have %d criminal counts left (%d minutes jailtime).", Clients[Player].uChar->CrimCount * -1, Clients[Player].uChar->CrimCount * -1);
			Clients[Player].infoMsg(Msg);
		}

		if (Clients[Player].uChar->DevMode > 0)
		{
			if (Clients[Player].uChar->DevMode == 1)
			{
				Clients[Player].GMMode = true;
				buffer[0] = 60;
				buffer[1] = 1;
				buffer[2] = Clients[Player].uChar->DevMode;
				send(Clients[Player].ClientSocket, buffer, 3, 0);

				Clients[Player].Inv = false;
				buffer[0] = 60;
				buffer[1] = 2;
				buffer[2] = 0;
				send(Clients[Player].ClientSocket, buffer, 3, 0);
			} else {		
				Clients[Player].GMMode = true;
				buffer[0] = 60;
				buffer[1] = 1;
				buffer[2] = Clients[Player].uChar->DevMode;
				send(Clients[Player].ClientSocket, buffer, 3, 0);

				Clients[Player].Inv = true;
				buffer[0] = 60;
				buffer[1] = 2;
				buffer[2] = 1;
				send(Clients[Player].ClientSocket, buffer, 3, 0);	
			}
		}
		
			InformGMofOnlineList(Player);
			InformGMofOnlineAmount(Player);

		for (i = 0; i < Max_Connections; i++)
		{			
			if (Clients[i].Used && Clients[i].State != 5 &&	i != Player && Clients[i].GMMode)
			{
				if (Clients[i].uChar->DevMode >= Clients[Player].uChar->DevMode)
				{
					InformGMofNewOnlinePlayer( i, Player );				
					InformGMofOnlineAmount( i );
				}
			}
		}

		PACKET_COORDINATES NewCoordinates;
		NewCoordinates.X = Clients[Player].uChar->X;
		NewCoordinates.Y = Clients[Player].uChar->X;
		NewCoordinates.ID = Clients[Player].Acc->getAccID();

		CheckPlayersOnScreen(Player, NewCoordinates);
		CheckNPCsOnScreen(Player);
		CheckItemsOnScreen(Player);
		CheckBodiesOnScreen(Player);

		CalcClientsConnected();

		break;

	case 4: // Char creation

		p_CCreatepacket pCCreate; //Create an instance of the Create Character Packet

		recv(Clients[Player].ClientSocket, buffer, sizeof(pCCreate), 0); //Receive the Create Character Packet

		memcpy(&pCCreate, buffer, sizeof(pCCreate)); //Copy the information from the buffer to the Create Character Packet

		if (strlen( pCCreate.Name) > 23) break; //If the character name is is bigger than 23 characters then break
		if (strlen( pCCreate.Name) < 3) break; //If the character name is smaller than 3 characters then break

		printf("Char create\n"); //Print a message to the Server Console
	
		// Should do more checking for bad names here...

		//If the character's name has a blank space ' ' in any of the last 2 indexes then break
		if (pCCreate.Name[strlen(pCCreate.Name)] == ' ' ||
			pCCreate.Name[strlen(pCCreate.Name)-1] == ' ') 
		{
			printf("Blocked char creation with name [%s]\n", pCCreate.Name); //Print a message to the Server Console
			break;
		}

		int ThisChar;
		ThisChar = -1;

		if (Clients[Player].Acc->CharacterExists(0) == 0)
		{
            ThisChar = 0;
		}
		else if (Clients[Player].Acc->CharacterExists(1) == 0)
		{
			ThisChar = 1;
		}

		if (ThisChar == -1) break;

		pCCreate.Slot = ThisChar;

		if (AccManager->CharNameTaken(pCCreate.Name))
		{
			printf("char name taken\n");
			// Char name taken


			Disconnect(Player);	
		
			break;
		}

		Clients[Player].State = ThisChar;

		AccManager->NewChar(Clients[Player].Acc, &pCCreate);
		Clients[Player].uChar = Clients[Player].Acc->GetCharPointer(pCCreate.Slot);

		for (i = 0; i < MaxBankItems; i++)
		{
			if (i < MaxWearedItems)
				Clients[Player].uChar->Worn[i].ID = 0;

			if (i < MaxBackpackItems)
				Clients[Player].uChar->Backpack[i].ID = 0;

			Clients[Player].uChar->Bank[i].ID = 0;
		}

		// This code should belong to Item manager, which doesn't exist.. Need to manually create few items at start.

		long Temp;
		Temp = GetFreeItemID();
		if (Temp != -1)
		{
			Clients[Player].uChar->Backpack[0].ID = Temp;

			Items[Temp].Type = TYPE_SWORD;
			Items[Temp].ID = 1;
			Items[Temp].Quantity = 1;
			Items[Temp].Family = 1;
			Items[Temp].x = 50;
			Items[Temp].y = 50;
			Items[Temp].Durability = 10;
			Items[Temp].Used = true;
			Items[Temp].KnowID = 0;
		}
		Temp = GetFreeItemID();
		if (Temp != -1)
		{
			Clients[Player].uChar->Backpack[1].ID = Temp;

			Items[Temp].Type = TYPE_COLLECTABLE;
			Items[Temp].ID = 0;
			Items[Temp].Quantity = 50;
			Items[Temp].Family = 3;
			Items[Temp].x = 75;
			Items[Temp].y = 75;
			Items[Temp].Durability = 10;
			Items[Temp].Used = true;
			Items[Temp].KnowID = 1;
		}

		Disconnect(Player); //edited by Radiankh : Why is this here?

		buffer4[0] = 1;
		buffer4[1] = 1;
		send(Clients[Player].ClientSocket, buffer4, 2, 0);

		break; //edited by Radiankh : Why is this here?

		buffer4[0] = 1;
		buffer4[1] = 1;
		memcpy(&buffer4[2], &Clients[Player].uChar, sizeof(CharacterData));

		send(Clients[Player].ClientSocket, buffer4, 2 + sizeof(CharacterData), 0);

		NewCoordinates.X = Clients[Player].uChar->X;
		NewCoordinates.Y = Clients[Player].uChar->Y;
		NewCoordinates.ID = Clients[Player].Acc->getAccID();

		CheckStats(Player); // I never figured out why first packets sent to client at this state gets lost... That's why Check stats is there two times.
		InformPlayerofHisAllItems(Player);
		CheckStats(Player);
		InformPlayerOfHisSkills(Player);
		CheckPlayersOnScreen(Player, NewCoordinates);
		CheckNPCsOnScreen(Player);
		CheckMonstersOnScreen(Player);
		CheckItemsOnScreen(Player);

		CalcClientsConnected();

		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].GMMode && Clients[i].State != 5)
			{
				InformGMofNewOnlinePlayer( i, Player );
			}			
		}

		break;

	// Chatting Messages.

	case 9: // This thing needs recoding.. :D

		PACKET_CHATMESSAGE NewPacket;

		memset(&NewPacket, 0, sizeof(NewPacket));

		recv(Clients[Player].ClientSocket, buffer, sizeof(NewPacket), 0);

		memcpy(&NewPacket, buffer, sizeof(NewPacket));

		// First check if the player is muted
		if (Clients[Player].is_mute) 
		{	
			// Tell player they are muted		
			buffer[0] = 99;
			buffer[1] = 94;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			NewPacket.MsgType = 11;
		}

		char NewMsg_2[100];
		memset(&NewMsg_2, 0, sizeof(NewMsg_2));

		int j;
		for (j = 0; j < 90; j++)
		{
			NewMsg_2[j] = NewPacket.Message[j];		
		}
		
		char NewMsg[120];
/*
		if (NewPacket.MsgType != 10)
			sprintf(NewMsg, "%s: %s", Clients[Player].uChar->Name, NewMsg_2);
		else
			sprintf(NewMsg, "%s",NewMsg_2);
*/

		if (NewPacket.MsgType == 1){
			sprintf(NewMsg, "[Say] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 2){
			sprintf(NewMsg, "[Whisper] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 3){
			sprintf(NewMsg, "[Global] %s: Å%s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 4){
			sprintf(NewMsg, "[Guild] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 5){
			sprintf(NewMsg, "[GM] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 6){
			sprintf(NewMsg, "[EWC] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 8){
			sprintf(NewMsg, "[PM] %s: %s", Clients[Player].uChar->Name, NewMsg_2);
		} else if (NewPacket.MsgType == 10){
			sprintf(NewMsg, "%s",NewMsg_2);
		} else {
			sprintf(NewMsg, "%s: %s", Clients[Player].uChar->Name, NewMsg_2);
		}

		strcpy(NewPacket.Message, NewMsg);

		char tmptxt[80];

		buffer[0] = 9;
		buffer[1] = 0;
		buffer2[0] = 9;
		buffer2[1] = 0;
	
		memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));

        short int tx, ty, ox, oy;

		switch (NewPacket.MsgType)
		{
		case 1: // Say.

			ox = Clients[Player].uChar->X;
			oy = Clients[Player].uChar->Y;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					tx = Clients[i].uChar->X;
					ty = Clients[i].uChar->Y;
	
					if (ox - tx >= -10 && ox - tx <= 10 &&
						oy - ty >= -10 && oy - ty <= 10 || Clients[i].ListenNormal)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
					}
				}
			}

			if (Clients[Player].uChar->DevMode >= 1 &&
				Clients[Player].uChar->DevMode <= 4)
			{
				if (Clients[Player].logfile->is_open())
				{
					memset(&tmptxt, 0, sizeof(tmp));
					sprintf(tmptxt, "%s %s: [Say Chat]: %s\n", Date, Time, NewMsg);
					Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();
				}
			}

			printf("%s: %s\n", Time, NewPacket.Message);

			normalchat << Date << " " << Time << NewMsg << endl;

			break;
		case 2: // Whisper.

			ox = Clients[Player].uChar->X;
			oy = Clients[Player].uChar->Y;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					tx = Clients[i].uChar->X;
					ty = Clients[i].uChar->Y;
	
					if ((ox - tx >= -1 && ox - tx <= 1 &&
						oy - ty >= -1 && oy - ty <= 1) || Clients[i].ListenWhisper)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
					}
				}				
			}

			printf("%s: %s\n", Time, NewPacket.Message);

			whisperchat << Date << " " << Time << NewMsg << endl;
	
			if (Clients[Player].uChar->DevMode >= 1 &&
				Clients[Player].uChar->DevMode <= 4)
			{
				if (Clients[Player].logfile->is_open())
				{
					memset(&tmptxt, 0, sizeof(tmp));
					sprintf(tmptxt, "%s %s: [WHISPER Chat]: %s\n", Date, Time, NewMsg);
					Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();
				}
			}

			break;
		case 3: // Global
			
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
				}				
			}

			printf("%s: %s\n", Time, NewPacket.Message);

			globalchat << Date << " " << Time << NewMsg << endl;
	
			if (Clients[Player].uChar->DevMode >= 1 &&
				Clients[Player].uChar->DevMode <= 4)
			{
				if (Clients[Player].logfile->is_open())
				{
					memset(&tmptxt, 0, sizeof(tmp));
					sprintf(tmptxt, "%s %s: [Global Chat]: %s\n", Date, Time, NewMsg);
					Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();
				}
			}

			break;
		case 4: // Guild.

			PACKET_CHATMESSAGE NewGuildMessage;
			memset(&NewGuildMessage, 0, sizeof(NewGuildMessage));
			NewGuildMessage.MsgType = 3;
			sprintf(NewGuildMessage.Message, "æ[%s] ï%s", Guilds[Clients[Player].Acc->uChar->Guild].Tag, NewPacket.Message);
			memcpy(&buffer2[2], &NewGuildMessage, sizeof(NewGuildMessage));
			if (Clients[Player].Acc->uChar->Guild > 0)
			{
				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{	
						if (Clients[i].Acc->uChar->Guild == Clients[Player].Acc->uChar->Guild)
						{
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
						}
						if (Clients[i].ListenGuild && Clients[i].GMMode)
						{
							send(Clients[i].ClientSocket, buffer2, 2 + sizeof(NewGuildMessage), 0);
						}
					}				
				}
			}

			guildchat << Date << " " << Time << "[" << Guilds[Clients[Player].Acc->uChar->Guild].Name << "]: " << NewMsg << endl;

			break;
		case 5: // GM

			if (Clients[Player].uChar->DevMode == 0) break;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (Clients[i].uChar->DevMode >= 2)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
					}
				}
			}

			//Clients[Player].logfile << Date << " " << Time << " [GM Chat]: " << NewMsg << endl;
			
			gmchat << Date << " " << Time << NewMsg << endl;

			printf("%s: %s\n", Time, NewPacket.Message);

			break;
		case 6: // EWC

			if (Clients[Player].uChar->DevMode == 0) break;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (Clients[i].uChar->DevMode >= 1)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
					}
				}
			}


			memset(&tmptxt, 0, sizeof(tmp));
			sprintf(tmptxt, "%s %s: [EWC Chat]: %s\n", Date, Time, NewMsg);
			Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();

			gmchat << Date << " " << Time << NewMsg << endl;

			printf("%s: %s\n", Time, NewPacket.Message);

			break;
		case 7: // Serverchat

			break;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
					Clients[i].infoMsg(NewPacket.Message);
			}

			memset(&tmptxt, 0, sizeof(tmp));
			sprintf(tmptxt, "%s %s: [SRVR MSG]: %s\n", Date, Time, NewMsg);
			Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();

			//Clients[Player].logfile << Date << " " << Time << " [SRVR MSG]: " << NewMsg << endl;

			normalchat << Date << " " << Time << "[Server chat]: " << NewMsg << endl;

			printf("%s: [Server chat]: %s\n", Time, NewPacket.Message);

			break;
		case 8: // PM
			
			PACKET_WHISPERTO whisperto;
			memset(&whisperto, 0, sizeof(whisperto));
			recv(Clients[Player].ClientSocket, buffer, sizeof(whisperto), 0);
			memcpy(&whisperto, buffer, sizeof(whisperto));

			short pl;
			pl = -1;

            for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, whisperto.name))
					{
						pl = i;
						i = Max_Connections;
					}
				}
			}

			if (pl == -1)
			{
				buffer[0] = 99;
				buffer[1] = 60;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

			NewPacket.MsgType = 11;

			buffer[0] = 9;
			buffer[1] = 0;
			memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));
			send(Clients[pl].ClientSocket, buffer, sizeof(NewPacket), 0);
			send(Clients[Player].ClientSocket, buffer, sizeof(NewPacket), 0);
				
			if (Clients[Player].uChar->DevMode >= 1)
			{
				if (Clients[Player].logfile->is_open())
				{
					memset(&tmptxt, 0, sizeof(tmp));
					sprintf(tmptxt, "%s %s: [PM Chat]: %s\n", Date, Time, NewMsg);
					Clients[Player].logfile->write(tmptxt, strlen(tmptxt)); Clients[Player].logfile->flush();
				}
			}

			printf("%s: [PM chat]: %s\n", Time, NewPacket.Message);

			normalchat << Date << " " << Time << NewMsg << endl;

			break;
		case 10: // Server message

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					Clients[i].infoMsg(NewPacket.Message);
//					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
				}
			}

			normalchat << Date << " " << Time << "[Srvr msg]: " << NewMsg << endl;

			printf("%s: [Server message]: %s\n", Time, NewPacket.Message);

			break;
		case 11: // Blank for muted

			break;
		default:
			printf("wtf? invalid chatting msg.\n");
			break;
		}
		
		break;

	// Someone is moving.
	case 10: 

		char Direction;

		char Error;

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		if (Clients[Player].TradeinProgress) break;

		if (Clients[Player].BrowsingShop)
		{
			Clients[Player].BrowsingShop = 0;
		}

		if (Clients[Player].BrowsingBank)
		{
			Clients[Player].BrowsingBank = 0;
		}		

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		if (Clients[Player].uChar->DevMode >= 2)
		{
			if (GetTickCount64() - Clients[Player].LstMov < 50)
				break;
		}

		if (!Clients[Player].GMMode && Clients[Player].uChar->DevMode < 2)
		{
			if (GetTickCount64() - Clients[Player].LstMov < 150)
				break;
		}

		if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax && !Clients[Player].GMMode && Clients[Player].uChar->DevMode < 2)
		{
			if (GetTickCount64() - Clients[Player].LstMov < 550)
				break;
		}

		Clients[Player].LstMov = GetTickCount64();

		Direction = buffer[0];

		short int tx2, ty2;

		tx = Clients[Player].uChar->X;
		ty = Clients[Player].uChar->Y;

		bool Dev;
		Dev = false;

		if (Clients[Player].uChar->DevMode > 1) Dev = true;

		if (tx <= 1 || tx >= 8000 || ty <= 1 || ty >= 8000) break;

		bool Valid;
			 Valid = false;

		short int ValidDir;
				  ValidDir = 0;


		switch (Direction)
		{
		case 4: // Left

			tx2 = tx-1; ty2 = ty;

			Valid = IsValid(tx2, ty2);

			break;		
		case 6: // Right

			tx2 = tx+1; ty2 = ty;

			Valid = IsValid(tx2, ty2);

			break;		
		case 8: // Up

			tx2 = tx; ty2 = ty-1;

			Valid = IsValid(tx2, ty2);

			break;		
		case 2: // Down

			tx2 = tx; ty2 = ty+1;

			Valid = IsValid(tx2, ty2);

			break;		

		//############################################################
		// These directions are a bit more complex, they need to 
		// test if player still can go up/down or left/right even if
		// south/north and east/west tile is blocked.
		//############################################################

		case 7: // Up-Left

			tx2 = tx-1; ty2 = ty-1;


			Valid = IsValid(tx2, ty2);

			if (Dev) Valid = true;

			if (!Valid)
			{
				// Check if player can move up when left is blocked.
				Valid = IsValid(tx, ty2); 
				if (!Valid)
				{
					//Check if player can move left when up is blocked.
					Valid = IsValid(tx2, ty);

					if (Valid) ValidDir = 4;
				}
				else
				{
					ValidDir = 8;
				}
			}

			break;		
		case 9: // Up-Right

			tx2 = tx+1; ty2 = ty-1;

			Valid = IsValid(tx2, ty2);

			if (Dev) Valid = true;

			if (!Valid)
			{
				// Check if player can move up when right is blocked.
				Valid = IsValid(tx, ty2); 
				if (!Valid)
				{
					//Check if player can move right when up is blocked.
					Valid = IsValid(tx2, ty);

					if (Valid) ValidDir = 6;
				}
				else
				{
					ValidDir = 8;
				}
			}

			
			break;		
		case 3: // Down-Right

			tx2 = tx+1; ty2 = ty+1;

			Valid = IsValid(tx2, ty2);

			if (Dev) Valid = true;

			if (!Valid)
			{
				// Check if player can move down when left is blocked.
				Valid = IsValid(tx, ty2); 
				if (!Valid)
				{
					//Check if player can move left when down is blocked.
					Valid = IsValid(tx2, ty);

					if (Valid) ValidDir = 6;
				}
				else
				{
					ValidDir = 2;
				}
			}

			break;		

		case 1: // Down-Left

			tx2 = tx-1; ty2 = ty+1;

			Valid = IsValid(tx2, ty2);

			if (Dev) Valid = true;

			if (!Valid)
			{
				// Check if player can move down when right is blocked.
				Valid = IsValid(tx, ty2); 
				if (!Valid)
				{
					//Check if player can move right when down is blocked.
					Valid = IsValid(tx2, ty);

					if (Valid) ValidDir = 4;
				}
				else
				{
					ValidDir = 2;
				}
			}
			
			break;
		}
        
		if (Dev) Valid = true;

		if (!Valid) break;

		short int tx3, ty3;

		if (Valid && ValidDir == 0)
		{
			tx3 = tx2;
			ty3 = ty2;		
		}


		if (Valid && ValidDir != 0)
		{
		
			switch (ValidDir)
			{
			case 2:
			case 8:
				tx3 = tx;
				ty3 = ty2;
				break;
			case 4:
			case 6:
				tx3 = tx2;
				ty3 = ty;
				break;
			}
		}

		// Check if Player is trying to move onto monster
		//REMOVED FOR DEBUG PURPOSE
		

		short hashx, hashy;
		hashx = tx3 / 64;
		hashy = ty3 / 64;

		mon_List = Monsters[hashx][hashy];

		while (mon_List != NULL)
		{
            if (mon_List->X == tx3 && mon_List->Y == ty3)
			{
				Error = 1;
                mon_List = NULL;                
			}

            if (mon_List != NULL)
				mon_List = mon_List->next;
		}

		obj_List = Objects[hashx][hashy];

		while (obj_List != NULL)
		{
			if (obj_List->X == tx3 && obj_List->Y == ty3 && ObjectTypes[ obj_List->Type ].Block && obj_List->On)
			{
				Error = 1;
                obj_List = NULL;                
			}

            if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		npc_List = NPC[hashx][hashy];

		while (npc_List != NULL)
		{
            if (npc_List->X == tx3 && npc_List->Y == ty3)
			{
				Error = 1;
                npc_List = NULL;                
			}

            if (npc_List != NULL)
				npc_List = npc_List->next;
		}

		//DEBUG!
		//if (Dev) 
			Error = 0;

		if (Error == 1) break;

		// Is he trying to ram on player?
		//---------------------------------------------------------------------------------
		// In this case, check for BLOCK that depends on rammed player's CON, STR and DEX.
		//---------------------------------------------------------------------------------

		short blocks;
		blocks = 0;
		
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				if (Clients[i].uChar->X == tx3 && Clients[i].uChar->Y == ty3 && i != Player && !Clients[i].Inv)
				{
					blocks = 50 + Clients[i].uChar->Str / 6
								+ Clients[i].uChar->Dex / 6
								+ Clients[i].uChar->Con / 6;
					if (rand() % 100 <= blocks)
					{
						Error = 1;
						i = Max_Connections;
					}
				}
			}
		}


		//DEBUG!
		//if (Dev)
			Error = 0;

		if (Error == 1) break;
		
		memset(&NewCoordinates, 0, sizeof(NewCoordinates));

		NewCoordinates.X = tx3;
		NewCoordinates.Y = ty3;
		NewCoordinates.ID = Clients[Player].Acc->getAccID();

		Clients[Player].uChar->X = tx3;
		Clients[Player].uChar->Y = ty3;

		// Inform player himself for moving around.

		buffer[0] = 10;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

		send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);


		CheckPlayersOnScreen(Player, NewCoordinates);
		CheckItemsOnScreen(Player);
		CheckMonstersOnScreen(Player);
		CheckNPCsOnScreen(Player);
		CheckBodiesOnScreen(Player);

		if (Clients[Player].beingfollowed[0] != -1)
		{
			memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
			NewCoordinates.X = tx3;
			NewCoordinates.Y = ty3;
			NewCoordinates.ID = Clients[Clients[Player].beingfollowed[0]].Acc->getAccID();
	
			Clients[Clients[Player].beingfollowed[0]].uChar->X = tx3;
			Clients[Clients[Player].beingfollowed[0]].uChar->Y = ty3;
	
			// Inform player himself for moving around.
	
			buffer[0] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
	
			send(Clients[Clients[Player].beingfollowed[0]].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);
	

			CheckPlayersOnScreen(Clients[Player].beingfollowed[0], NewCoordinates);
			CheckItemsOnScreen(Clients[Player].beingfollowed[0]);
			CheckMonstersOnScreen(Clients[Player].beingfollowed[0]);
			CheckNPCsOnScreen(Clients[Player].beingfollowed[0]);
			CheckBodiesOnScreen(Clients[Player].beingfollowed[0]);
		}

		if (Clients[Player].beingfollowed[1] != -1)
		{
			memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
			NewCoordinates.X = tx3;
			NewCoordinates.Y = ty3;
			NewCoordinates.ID = Clients[Clients[Player].beingfollowed[1]].Acc->getAccID();
	
			Clients[Clients[Player].beingfollowed[1]].uChar->X = tx3;
			Clients[Clients[Player].beingfollowed[1]].uChar->Y = ty3;
	
			// Inform player himself for moving around.
	
			buffer[1] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
	
			send(Clients[Clients[Player].beingfollowed[1]].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);
	

			CheckPlayersOnScreen(Clients[Player].beingfollowed[1], NewCoordinates);
			CheckItemsOnScreen(Clients[Player].beingfollowed[1]);
			CheckMonstersOnScreen(Clients[Player].beingfollowed[1]);
			CheckNPCsOnScreen(Clients[Player].beingfollowed[1]);
			CheckBodiesOnScreen(Clients[Player].beingfollowed[1]);
		}

		break;

	case 20: // Item handling.

		recv(Clients[Player].ClientSocket, buffer, sizeof(ItemMove), 0);

		if (Clients[Player].TradeinProgress) break;

		if (Clients[Player].uChar->DevMode == 1) 
			break;

		memcpy(&ItemMove, buffer, sizeof(ItemMove));

		long This;
		
		PACKET_COORDINATES NewCoords;

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		short TmpID;
		long TempID;
		long ItemB;
		bool CanWear;
		CanWear = false;

		// Item thing is a mess, if you don't know who it works. Takes time to learn it or recode it. :)
		// One important thing is, that in Items[] database .Used MUST be true if item is used.
		// Another important thing is, that Items[].KnowID must be same as item's position in the 
		// player's backpack, bank or worn list!!!!!! IMPORTANT!!!!!
		// Example:
		//
		// Clients[Player].uChar->Bank[i].ID gets set to Items[TmpID].
		// Items[TmpID].KnowID = i; !!!^!!


		switch (ItemMove.Case)
		{
		case 1:
			//---------------------------------------
			// Item is moved from Worn to Backpack
			//---------------------------------------
            
			This = Clients[Player].uChar->Worn[ItemMove.ItemID].ID;
			if ( This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (ItemMove.Amount == 0)
			{
				TmpID = GetFreeBackpackSlot(Player);
				if (TmpID == -1) break;

				RemoveItemFromPlayer(Player, ItemWeared, ItemMove.ItemID);

				Clients[Player].uChar->Backpack[TmpID].ID = This;
				Clients[Player].uChar->Worn[ItemMove.ItemID].ID = 0;

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;
				Items[This].KnowID = TmpID; // <--- THIS IS IMPORTANT.

				InformPlayerofItem(Player, This, ItemBackpack);

				CheckStats(Player);
			}

			break;
		case 2:
			//---------------------------------------
			// Item is moved from Backpack to Worn 
			//---------------------------------------

			ItemB = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
			if (ItemB <= 0) break;
			if (Items[ItemB].KnowID != ItemMove.ItemID) break;

			if (ItemMove.Amount == 0)
			{
				This = -1;
				if (Items[ItemB].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].uChar->Worn[0].ID == 0)
					{                        
						CanWear = Check_Requirement(Player, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_type, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_amnt, Items[ItemB].Type);
						if (!CanWear) break;
						This = 0;
					}
					else break;
				}
				if (Items[ItemB].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].uChar->Worn[Items[ItemB].Type].ID == 0 && ItemMove.ToList == Items[ItemB].Type)
					{
						CanWear = Check_Requirement(Player, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_type, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_amnt, 0);
						if (!CanWear) break;
						This = Items[ItemB].Type;
					}
					else break;
				}
				if (This == -1) break;
	
				RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);

				Clients[Player].uChar->Worn[This].ID = ItemB;
				Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;

				Items[ItemB].KnowID = This;

				InformPlayerofItem(Player, ItemB, ItemWeared);

				CheckStats(Player);
			}

			break;
		case 3:
			//-----------------------------------
			// Item is moved from Worn to Bank
			//-----------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

			This = Clients[Player].uChar->Worn[ItemMove.ItemID].ID;
			if ( This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (ItemMove.Amount == 0)
			{
				TmpID = GetFreeBankSlot(Player);
				if (TmpID == -1) break;

				RemoveItemFromPlayer(Player, ItemWeared, ItemMove.ItemID);

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;

				Clients[Player].uChar->Bank[TmpID].ID = This;
				Clients[Player].uChar->Worn[ItemMove.ItemID].ID = 0;

				Items[This].KnowID = TmpID;

				InformPlayerofItem(Player, This, ItemBank);

				CheckStats(Player);
			}

			break;
		case 4:
			//-----------------------------------
			// Item is moved from Bank to Worn
			//-----------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

			if (ItemMove.Amount == 0)
			{
				This = -1;
				ItemB = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
				if (Items[ItemB].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].uChar->Worn[0].ID == 0)
					{
						//if (Clients[Player].Acc->tChar.Weight + Weapons[Items[ItemB].Type][Items[ItemB].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						//	break;

						CanWear = Check_Requirement(Player, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_type, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_amnt, Items[ItemB].Type);
						if (!CanWear) break;

						This = 0;
					}
					else break;
				}
				if (Items[ItemB].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].uChar->Worn[Items[ItemB].Type].ID == 0 && ItemMove.ToList == Items[ItemB].Type)
					{
						//if (Clients[Player].Acc->tChar.Weight + Armors[Items[ItemB].Type-11][Items[ItemB].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						//	break;

						CanWear = Check_Requirement(Player, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_type, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_amnt, 0);
						if (!CanWear) break;
						
						This = Items[ItemB].Type;					
					}
					else break;
				}
				if (This == -1) break;
	
				RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);

				Clients[Player].uChar->Worn[This].ID = ItemB;
				Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;
		
				Items[ItemB].KnowID = This; // See? I'm ALWAYS setting the KnowID, it's very important.

				InformPlayerofItem(Player, ItemB, ItemWeared);

				CheckStats(Player);
			}

			break;

		case 5:
			//-------------------------------------
			// Item is moved from Worn to Ground
			//-------------------------------------

			This = Clients[Player].uChar->Worn[ItemMove.ItemID].ID;
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (ItemMove.Amount == 0)
			{
				if (IsValid(ItemMove.x, ItemMove.y))
				{
					TmpID = GetFreeGroundSlot();					
					if (TmpID == -1) break;

					RemoveItemFromPlayer(Player, ItemWeared, ItemMove.ItemID);

					Items[This].x = ItemMove.x;
					Items[This].y = ItemMove.y;

					Ground[TmpID] = This;
					GroundTime[TmpID] = GetTickCount64();
					Clients[Player].uChar->Worn[ItemMove.ItemID].ID = 0;

					Items[This].KnowID = TmpID;

					InformMassofItem( TmpID );

					CheckStats(Player);
				}
			}
	
			break;
		case 6:
			//-------------------------------------
			// Item is moved from Ground to Worn
			//-------------------------------------

			if (ItemMove.Amount == 0)
			{
				This = -1;
				ItemB = Ground[ItemMove.ItemID];
				if (ItemB == 0 || Items[ItemB].KnowID != ItemMove.ItemID)break;

				if (Items[This].x - Clients[Player].uChar->X < -1 ||
					Items[This].x - Clients[Player].uChar->X > 1 ||
					Items[This].y - Clients[Player].uChar->Y < -1 ||
					Items[This].y - Clients[Player].uChar->Y > 1)
				break;

				if (Items[ItemB].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].uChar->Worn[0].ID == 0)
					{
						//if (Clients[Player].Acc->tChar.Weight + Weapons[Items[ItemB].Type][Items[ItemB].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						//	break;

						CanWear = Check_Requirement(Player, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_type, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_amnt, Items[ItemB].Type);
						if (!CanWear) break;

						This = 0;
					}
					else break;
				}
				if (Items[ItemB].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].uChar->Worn[Items[ItemB].Type].ID == 0 && ItemMove.ToList == Items[ItemB].Type)
					{
						//if (Clients[Player].Acc->tChar.Weight + Armors[Items[ItemB].Type-11][Items[ItemB].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						//	break;

						CanWear = Check_Requirement(Player, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_type, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_amnt, 0);
						if (!CanWear) break;

						This = Items[ItemB].Type;					
					}
					else break;
				}
				if (This == -1) break;
	
				RemoveItemFromMass(ItemMove.ItemID);

				Clients[Player].uChar->Worn[This].ID = ItemB;
				Ground[ItemMove.ItemID] = 0;

				Items[ItemB].KnowID = This;

				InformPlayerofItem(Player, ItemB, ItemWeared);

				CheckStats(Player);

				if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax)
				{
					buffer[0] = 99;
					buffer[1] = 2;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			}

			break;

		case 7:
			//-------------------------------------
			// Item is moved from Backpack to Bank
			//-------------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

			This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (ItemMove.Amount == 0)
			{
				TmpID = GetFreeBankSlot(Player);
				if (TmpID == -1) break;

				RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);	

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;
                
				Clients[Player].uChar->Bank[TmpID].ID = This;
				Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;

				Items[This].KnowID = TmpID;

				InformPlayerofItem(Player, This, ItemBank);

				CheckStats(Player);
			}

			break;

		case 8:
			//-------------------------------------
			// Item is moved from Bank to Backpack
			//-------------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

            
			This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;
            
			if (ItemMove.Amount == 0)
			{
				/*
				if (Items[This].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_COLLECTABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight*Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_USEABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				*/

				TmpID = GetFreeBackpackSlot(Player);
				if (TmpID == -1) break;

				RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;

				Clients[Player].uChar->Backpack[TmpID].ID = This;
				Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;

				Items[This].KnowID = TmpID;

				InformPlayerofItem(Player, This, ItemBackpack);

				CheckStats(Player);

				if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax)
				{
					buffer[0] = 99;
					buffer[1] = 2;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}
			}

			break;
		case 9:
			//---------------------------------------
			// Item is moved from Backpack to Ground
			//---------------------------------------

			if (ItemMove.x - Clients[Player].uChar->X < -1 ||
				ItemMove.x - Clients[Player].uChar->X > 1 ||
				ItemMove.y - Clients[Player].uChar->Y < -1 ||
				ItemMove.y - Clients[Player].uChar->Y > 1)
			break;

			if (ItemMove.Amount == 0)
			{
				TmpID = GetFreeGroundSlot();
				if (TmpID == -1) break;

                if (IsValid(ItemMove.x, ItemMove.y))
				{
					This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
					printf("Item = %d\n", This);
					if (This <= 0) break;
					if (Items[This].KnowID != ItemMove.ItemID) break;

					RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);

					Items[This].x = ItemMove.x;
					Items[This].y = ItemMove.y;

					Ground[TmpID] = This;
					GroundTime[TmpID] = GetTickCount64();
					Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;

					Items[This].KnowID = TmpID;

					InformMassofItem( TmpID );

					CheckStats(Player);
				}
			}

			break;
		case 10:
			//---------------------------------------
			// Item is moved from Ground to Backpack
			//---------------------------------------

			This = Ground[ItemMove.ItemID];
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (Items[This].x - Clients[Player].uChar->X < -1 ||
				Items[This].x - Clients[Player].uChar->X > 1 ||
				Items[This].y - Clients[Player].uChar->Y < -1 ||
				Items[This].y - Clients[Player].uChar->Y > 1)
			break;


 			if (ItemMove.Amount == 0)
			{
				/*
				if (Items[This].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_COLLECTABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight* Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_USEABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				*/

				TmpID = GetFreeBackpackSlot(Player);
				if (TmpID == -1) break;

				RemoveItemFromMass( ItemMove.ItemID );

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;

				Clients[Player].uChar->Backpack[TmpID].ID = This;
				Ground[ItemMove.ItemID] = 0;

				Items[This].KnowID = TmpID;

				InformPlayerofItem(Player, This, ItemBackpack);

				CheckStats(Player);

				if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax)
				{
					buffer[0] = 99;
					buffer[1] = 2;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}
			}

			break;

		case 11: 
			//-----------------------------------
			// Item is moved from Bank to Ground
			//-----------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

			This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (ItemMove.x - Clients[Player].uChar->X < -1 ||
				ItemMove.x - Clients[Player].uChar->X > 1 ||
				ItemMove.y - Clients[Player].uChar->Y < -1 ||
				ItemMove.y - Clients[Player].uChar->Y > 1)
			break;

			if (ItemMove.Amount == 0)
			{
				if (IsValid(ItemMove.x, ItemMove.y))
				{
					TmpID = GetFreeGroundSlot();
					if (TmpID == -1) break;

					RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);

					Items[This].x = ItemMove.x;
					Items[This].y = ItemMove.y;

					Ground[TmpID] = This;
					GroundTime[TmpID] = GetTickCount64();
                    Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;

					Items[This].KnowID = TmpID;

					InformMassofItem( TmpID );
				}
			}

			break;
		case 12: 
			//-----------------------------------
			// Item is moved from Ground to Bank
			//-----------------------------------

			if (!IsNearBanker(Clients[Player].uChar->X, Clients[Player].uChar->Y))
				break;

			This = Ground[ItemMove.ItemID];
			
			if (This <= 0) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			if (Items[This].x - Clients[Player].uChar->X < -1 ||
				Items[This].x - Clients[Player].uChar->X > 1 ||
				Items[This].y - Clients[Player].uChar->Y < -1 ||
				Items[This].y - Clients[Player].uChar->Y > 1)
			break;

			if (ItemMove.Amount == 0)
			{
				TmpID = GetFreeBankSlot( Player);
				if (TmpID == -1) break;

				RemoveItemFromMass(ItemMove.ItemID);

				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;

				Clients[Player].uChar->Bank[TmpID].ID = This;
				Ground[ItemMove.ItemID] = 0;
				
				Items[This].KnowID = TmpID;

				InformPlayerofItem(Player, This, ItemBank);
			}

			break;
		case 13:
			//-------------------------------------------
			// Item only gets new coords.
			//-------------------------------------------

			if (ItemMove.ToList == ItemGround)
			{
				This = Ground[ItemMove.ItemID];
				if (This <= 0) break;
				if (Items[This].KnowID != ItemMove.ItemID) break;

				if (Items[This].x - Clients[Player].uChar->X < -1 ||
					Items[This].x - Clients[Player].uChar->X > 1 ||
					Items[This].y - Clients[Player].uChar->Y < -1 ||
					Items[This].y - Clients[Player].uChar->Y > 1)
				break;

				if (ItemMove.Amount == 0)
				{
					if (IsValid(ItemMove.x, ItemMove.y))
					{
						InformPlayersofGroundItemsNewCoordinates(ItemMove.ItemID, ItemMove.x, ItemMove.y);
						Items[This].x = ItemMove.x;
						Items[This].y = ItemMove.y;
						GroundTime[ItemMove.ItemID] = GetTickCount64();
					}
					break;
				}
			}
			
			memset(&NewCoords, 0, sizeof(NewCoords));
		
			NewCoords.X = ItemMove.x;
			NewCoords.Y = ItemMove.y;
			NewCoords.ID = ItemMove.ItemID;

			buffer[0] = 12;
			switch (ItemMove.ToList)
			{
			case ItemBank:
				This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
				if (This <= 0) break;
				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;
				buffer[1] = 16;
				break;
			case ItemBackpack:
				This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
				if (This <= 0) break;
				Items[This].x = ItemMove.x;
				Items[This].y = ItemMove.y;
				buffer[1] = 17;
				break;
			}

			memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));

			send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);
			break;

		case 14: // Splitting.

			// Look, this is the way it should be... Everything handled in their own functions. :)

			SplitItem(Player);

			CheckStats(Player);

			break;
		case 15:  // Piling

			PileItem(Player);

            CheckStats(Player);

			break;
		}       

		break;
	case 21: // Potions

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		if (Clients[Player].TradeinProgress) break;

		if (Clients[Player].uChar->DevMode < 3 && Clients[Player].uChar->DevMode >= 1) break;

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		memcpy(&TmpID, buffer, sizeof(TmpID));

		This = Clients[Player].uChar->Backpack[TmpID].ID;

		if (!Items[This].Used) break;

		if (Items[This].Type == TYPE_COLLECTABLE && Collectables[Items[This].ID].CureType > 0)
		{
			if (GetTickCount64() - Clients[Player].LstDeath < 2000) // I'm using LstDeath as timer in using potions, since deathtimer isn't operational.
			{
				buffer[0] = 99;
				buffer[1] = 14;
	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}
			
			if (Clients[Player].uChar->HPLeft < Clients[Player].Acc->Calc_hpMax())
			{
				if (Items[This].Quantity > 1)
				{
					Clients[Player].LstDeath = GetTickCount64();
					short Damage;
					Damage = rand() % (Collectables[Items[This].ID].DamMax - Collectables[Items[This].ID].DamMin);
					Damage += Collectables[Items[This].ID].DamMin;
	
					Items[This].Quantity--;
	
					InformPlayerOfNewAmount(Player, ItemBackpack, TmpID, Items[This].Quantity);

					Clients[Player].uChar->HPLeft += Damage;

					if (Clients[Player].uChar->HPLeft > Clients[Player].Acc->Calc_hpMax())
						Clients[Player].uChar->HPLeft = Clients[Player].Acc->Calc_hpMax();

					memset(&NewHP, 0, sizeof(NewHP));

					NewHP.KnowID = Clients[Player].Acc->getAccID();
					NewHP.NewHP = Clients[Player].uChar->HPLeft;
					NewHP.Type = 0;
	
					printf("Potion Used Heals for: %d\n", Damage);

					buffer[0] = 20;
					buffer[1] = 3;
			
					memcpy(&buffer[2], &NewHP, sizeof(NewHP));
			
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewHP), 0);

					ox = Clients[Player].uChar->X;
					oy = Clients[Player].uChar->Y;

					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							tx = Clients[i].uChar->X;
							ty = Clients[i].uChar->Y;
							if (IsInRange(ox, oy, tx, ty))
							{
								if (Clients[i].KnowPlayer[Clients[Player].Know_ID] || i == Player)
									send(Clients[i].ClientSocket, buffer, 2 + sizeof( NewHP), 0);
							}
						}
					}
	
					buffer[0] = 99;
					buffer[1] = 10;
					memcpy(&buffer[2], &Damage, sizeof(Damage));
	
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Damage), 0);

					CheckStats(Player);
				}
				else if (Items[This].Quantity == 1)
				{
					Clients[Player].LstDeath = GetTickCount64();
					short Damage;
					Damage = rand() % (Collectables[Items[This].ID].DamMax - Collectables[Items[This].ID].DamMin);
					Damage += Collectables[Items[This].ID].DamMin;

					Clients[Player].uChar->Backpack[TmpID].ID = 0;
					memset(&Items[This], 0, sizeof(Items[This]));

					memset(&Items[Clients[Player].uChar->Backpack[TmpID].ID], 0, sizeof(Items[Clients[Player].uChar->Backpack[TmpID].ID]));

					RemoveItemFromPlayer(Player, ItemBackpack, TmpID);


					Clients[Player].uChar->HPLeft += Damage;

					if (Clients[Player].uChar->HPLeft > Clients[Player].Acc->Calc_hpMax())
						Clients[Player].uChar->HPLeft = Clients[Player].Acc->Calc_hpMax();

					memset(&NewHP, 0, sizeof(NewHP));
	
					NewHP.KnowID = Clients[Player].Acc->getAccID();
					NewHP.NewHP = Clients[Player].uChar->HPLeft;
					NewHP.Type = 0;

					buffer[0] = 20;
					buffer[1] = 3;
			
					memcpy(&buffer[2], &NewHP, sizeof(NewHP));
			
					ox = Clients[Player].uChar->X;
					oy = Clients[Player].uChar->Y;
			
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							tx = Clients[i].uChar->X;
							ty = Clients[i].uChar->Y;
							if (IsInRange(ox, oy, tx, ty))
							{
								if (Clients[i].KnowPlayer[Clients[Player].Know_ID] || i == Player)
									send(Clients[i].ClientSocket, buffer, 2 + sizeof( NewHP), 0);
							}
						}
					}
	
					buffer[0] = 99;
					buffer[1] = 10;
					memcpy(&buffer[2], &Damage, sizeof(Damage));
	
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Damage), 0);

					CheckStats(Player);
				}
			}
			else
			{
				buffer[0] = 99;
				buffer[1] = 11;

				send(Clients[Player].ClientSocket, buffer, 2, 0);
			}		
		}

		break;
	case 22: // Stat points

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		switch (buffer[0])
		{
		case 1: // Str

			if (Clients[Player].uChar->StatPoints <= 0)
				break;

			printf("Player's race: %d, limit: %d\n", Clients[Player].uChar->Race, Races[Clients[Player].uChar->Race].StatLimits[0]);
			// Implemented in 0.32 :)
			if (Clients[Player].uChar->Str >= Races[Clients[Player].uChar->Race].StatLimits[0]) 
				break;

			Clients[Player].uChar->StatPoints--;
			Clients[Player].uChar->Str++;

			CheckStats_base(Player);

			CheckStats(Player);

			break;
		case 2: // Dex

			if (Clients[Player].uChar->StatPoints <= 0)
				break;

			if (Clients[Player].uChar->Dex >= Races[Clients[Player].uChar->Race].StatLimits[1]) 
				break;

			Clients[Player].uChar->StatPoints--;
			Clients[Player].uChar->Dex++;

			CheckStats_base(Player);
			CheckStats(Player);

			break;
		case 3: // Con

			if (Clients[Player].uChar->StatPoints <= 0)
				break;

			if (Clients[Player].uChar->Con >= Races[Clients[Player].uChar->Race].StatLimits[2]) 
				break;

			Clients[Player].uChar->StatPoints--;
			Clients[Player].uChar->Con++;

			Clients[Player].uChar->HPMax  = Clients[Player].Acc->Calc_hpMax();

			buffer[0] = 30;
			buffer[1] = 6;

			memcpy(&buffer[2], &Clients[Player].uChar->HPMax, sizeof(Clients[Player].uChar->HPMax));

			send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Clients[Player].uChar->HPMax), 0);

			CheckStats_base(Player);
			CheckStats(Player);

			break;
		case 4: // Int

			if (Clients[Player].uChar->StatPoints <= 0)
				break;

			if (Clients[Player].uChar->Int >= Races[Clients[Player].uChar->Race].StatLimits[3]) 
				break;

			Clients[Player].uChar->StatPoints--;
			Clients[Player].uChar->Int++;

			CheckStats_base(Player);
			CheckStats(Player);

			break;
		}

		break;

	case 23: // Item usage in backpack by right click. Should belong to item handling case with other item handlings. o.O

		recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);

		if (Clients[Player].uChar->DevMode <= 3 && Clients[Player].uChar->DevMode >= 1) break;

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		if (Clients[Player].TradeinProgress) break;
		
		memcpy(&TmpID, buffer, sizeof(TmpID));

		This = -1;

		ItemB = Clients[Player].uChar->Backpack[TmpID].ID;
		if (ItemB <= 0) break;
		if (Items[ItemB].KnowID != TmpID) break;

		if (Items[ItemB].Family == FAMILY_WEAPON)
		{
			CanWear = Check_Requirement(Player, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_type, Weapons[Items[ItemB].Type][Items[ItemB].ID].use_req_amnt, Items[ItemB].Type);
			if (!CanWear) break;
			This = 0;
		}
		else
		if (Items[ItemB].Family == FAMILY_ARMOR)
		{
			CanWear = Check_Requirement(Player, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_type, Armors[Items[ItemB].Type-11][Items[ItemB].ID].use_req_amnt, 0);
			if (!CanWear) break;
			This = Items[ItemB].Type;
		}
		if (This == -1) break;

		if (Clients[Player].uChar->Worn[This].ID == 0)
		{
			RemoveItemFromPlayer(Player, ItemBackpack, TmpID);
			Clients[Player].uChar->Worn[This].ID = ItemB;

			Clients[Player].uChar->Backpack[TmpID].ID = 0;

			ItemB = Clients[Player].uChar->Worn[This].ID;

			Items[ItemB].KnowID = This;

            InformPlayerofItem(Player, ItemB, ItemWeared);
			
		}
		else
		{
			RemoveItemFromPlayer(Player, ItemWeared, This);
			RemoveItemFromPlayer(Player, ItemBackpack, TmpID);

			long Temppi;
			Temppi = Clients[Player].uChar->Worn[This].ID;
			TempID = Clients[Player].uChar->Backpack[TmpID].ID;
	
			Clients[Player].uChar->Backpack[TmpID].ID = Temppi;
			Clients[Player].uChar->Worn[This].ID = TempID;

			Items[TempID].KnowID = This;
			Items[Temppi].KnowID = TmpID;

			InformPlayerofItem(Player, Temppi, ItemBackpack);
			InformPlayerofItem(Player, TempID, ItemWeared);
		}

		CheckStats(Player);

		break;
	case 24: // Someone "used" item on ground, it goes to backpack

		recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);

		if (Clients[Player].TradeinProgress) break;

		if (Clients[Player].uChar->DevMode < 3 && Clients[Player].uChar->DevMode >= 1) break;

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		TempID = GetFreeBackpackSlot(Player);
		if (TempID == -1) break;

		memcpy(&TmpID, buffer, sizeof(TmpID));

        This = Ground[TmpID];
		if (This <= 0) break;

		if (Items[This].x - Clients[Player].uChar->X < -1 ||
			Items[This].x - Clients[Player].uChar->X > 1 ||
			Items[This].y - Clients[Player].uChar->Y < -1 ||
			Items[This].y - Clients[Player].uChar->Y > 1 ||
			Items[This].KnowID != TmpID)
		break;
	/*
		if (Items[This].Family == FAMILY_WEAPON)
		{
			if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
				break;
		}
		if (Items[This].Family == FAMILY_ARMOR)
		{
			if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
				break;
		}
		if (Items[This].Family == FAMILY_COLLECTABLE)
		{
			if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight*Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
				break;
		}
		if (Items[This].Family == FAMILY_USEABLE)
		{
			if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
				break;
		}
	*/

		ox = Clients[Player].uChar->X;
		oy = Clients[Player].uChar->Y;

		tx = Items[This].x;
		ty = Items[This].y;

		if (ox - tx > -2 && ox - tx < 2 && oy - ty > -2 && oy - ty < 2)
		{
			if (Items[This].Type == TYPE_COLLECTABLE)
			{
				bool Error = false;
				for (i = 0; i < MaxBackpackItems; i++)
				{
					ItemB = Clients[Player].uChar->Backpack[i].ID;
					if (ItemB > 0)
					{
						if (Items[This].ID == Items[ItemB].ID && Items[ItemB].Type == TYPE_COLLECTABLE && Items[ItemB].KnowID == i)
						{
							Items[ItemB].Quantity += Items[This].Quantity;

							RemoveItemFromMass( TmpID );

							memset(&Items[This], 0, sizeof(Items[This]));
							Ground[TmpID] = 0;

							InformPlayerOfNewAmount( Player, ItemBackpack, i, Items[ItemB].Quantity);
					
							Error = true;
							i = MaxBackpackItems;
						}
					}					
				}
				if (!Error)
				{
					/*
					if (Items[This].Family == FAMILY_WEAPON)
					{
						if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
							break;
					}
					if (Items[This].Family == FAMILY_ARMOR)
					{
						if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
							break;
					}
					if (Items[This].Family == FAMILY_COLLECTABLE)
					{
						if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight* Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
							break;
					}
					if (Items[This].Family == FAMILY_USEABLE)
					{
						if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
							break;
					}
					*/

					RemoveItemFromMass( TmpID );

					Items[This].x = rand() % 100;
					Items[This].y = rand() % 100;

					Clients[Player].uChar->Backpack[TempID].ID = This;
					Ground[TmpID] = 0;

					Items[This].KnowID = TempID;

					InformPlayerofItem(Player, This, ItemBackpack);		

					if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax)
					{
						buffer[0] = 99;
						buffer[1] = 2;
						send(Clients[Player].ClientSocket, buffer, 2, 0);
					}
				}
			}
			else
			{
				/*
				if (Items[This].Family == FAMILY_WEAPON)
				{
					if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_ARMOR)
				{
					if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_COLLECTABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight* Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				if (Items[This].Family == FAMILY_USEABLE)
				{
					if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
						break;
				}
				*/

				RemoveItemFromMass( TmpID);
	
				Items[This].x = rand() % 100;
				Items[This].y = rand() % 100;

				Clients[Player].uChar->Backpack[TempID].ID = This;
				Ground[TmpID] = 0;

				Items[This].KnowID = TempID;

				InformPlayerofItem(Player, This, ItemBackpack);

				if (Clients[Player].Acc->tChar.Weight > Clients[Player].Acc->tChar.WeightMax)
				{
					buffer[0] = 99;
					buffer[1] = 2;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}
			}
			CheckStats(Player);
		}

		break;

	case 25: // Identify

		short Who;
		Who = 0; 

		recv(Clients[Player].ClientSocket, buffer, sizeof(Who), 0);

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		memcpy(&Who, buffer, sizeof(Who));

		Who = Client_GetID(Who);

		if (Who >= 0 && Who != Player)
		{
			if (IsInRange(Clients[Player].uChar->X, Clients[Player].uChar->Y, Clients[Who].uChar->X, Clients[Who].uChar->Y))
			{
				PACKET_IDENTIFYPACKET IdentifyPacket;

				memset(&IdentifyPacket, 0, sizeof(IdentifyPacket));
				for (i = 0; i < 20; i++)
				{
					if (Clients[Who].uChar->Worn[i].ID != 0)
					{
						IdentifyPacket.ID[i] = Items[Clients[Who].uChar->Worn[i].ID].ID;
						IdentifyPacket.Type[i] = Items[Clients[Who].uChar->Worn[i].ID].Type;
						IdentifyPacket.Family[i] = Items[Clients[Who].uChar->Worn[i].ID].Family;
					}			
				}
				strcpy(IdentifyPacket.Name, Clients[Who].uChar->Name);
				if (Clients[Who].Acc->uChar->Guild > 0)
				{
					sprintf(IdentifyPacket.Guild, "%s", Guilds[ Clients[Who].Acc->uChar->Guild ].Name);
					sprintf(IdentifyPacket.Tag, "%s", Guilds[ Clients[Who].Acc->uChar->Guild ].Tag);
				}

				IdentifyPacket.reputation = Clients[Who].uChar->Reputation;
				IdentifyPacket.id = Clients[Who].Acc->getAccID();
	
				buffer[0] = 25;
				buffer[1] = 0;
				memcpy(&buffer[2], &IdentifyPacket, sizeof(IdentifyPacket));

				send(Clients[Player].ClientSocket, buffer, 2 + sizeof(IdentifyPacket), 0);
			}
		}

		break;

	case 26: // Identify self

		if (Clients[Player].Acc->uChar->Guild > 0)
		{
			buffer[0] = 26;
			buffer[1] = 1;

			PACKET_CHATMESSAGE NewMsg2;
			memset(&NewMsg2, 0, sizeof(NewMsg2));

			sprintf(NewMsg2.Message, "You are in the %s clan.", Guilds[ Clients[Player].Acc->uChar->Guild ].Name);
			NewMsg2.MsgType = 4;

			memcpy(&buffer[2], &NewMsg2, sizeof(NewMsg2));
			send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewMsg2), 0);
		}
		else
		{
            buffer[0] = 26;
			buffer[1] = 2;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
		}

		break;

	case 27: // Body things.........

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		switch (buffer[0])
		{
		case 1: // Body info requested

			recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);

			memcpy(&TmpID, buffer, sizeof(TmpID));

			if (!Body[TmpID].Used) break;
		
			ox = Clients[Player].uChar->X;
			oy = Clients[Player].uChar->Y;
			tx = Body[TmpID].X;
			ty = Body[TmpID].Y;

			if (ox - tx >= -1 && ox - tx <= 1 && oy - ty >= -1 && oy - ty <= 1)
			{
				Clients[Player].BrowsingShop = TmpID;				
				for (i = 0; i < 84; i++)
				{
					if (Body[TmpID].Items[i] != 0)
					{
						InformPlayerofItem(Player, Body[TmpID].Items[i], ItemInBody);
					}
				}			
			}

			break;
		case 2:  // Loot item

			short BodyID, ItemID;
			long ThisItem;
			BodyID = -1;
			ItemID = -1;

			recv(Clients[Player].ClientSocket, buffer, sizeof(BodyID), 0);
			memcpy(&BodyID, buffer, sizeof(BodyID));

			recv(Clients[Player].ClientSocket, buffer, sizeof(ItemID), 0);
			memcpy(&ItemID, buffer, sizeof(ItemID));

			if (!Body[BodyID].Used) break;

			ThisItem = Body[BodyID].Items[ItemID];
			if (ThisItem == 0) break;
			if (Items[ThisItem].KnowID != ItemID) break;

			if (Body[BodyID].X - Clients[Player].uChar->X < -1 ||
				Body[BodyID].X - Clients[Player].uChar->X > 1 ||
				Body[BodyID].Y - Clients[Player].uChar->Y < -1 ||
				Body[BodyID].Y - Clients[Player].uChar->Y > 1)				
			break;			

            //--------------------------------
			// 2 cases:
			// #1 - This body is his own body
			// #2 - This body is not his own body

			if ((Body[BodyID].Owner_ID == Clients[Player].Acc->getAccID() &&
				Body[BodyID].Owner_Slot == Clients[Player].State) ||
				(Clients[Player].Acc->uChar->Guild == Body[BodyID].Guild &&
				Body[BodyID].DecayState >= 3))
			{
				TempID = GetFreeBackpackSlot(Player);
				if (TempID == -1) break;

				Clients[Player].uChar->Backpack[TempID].ID = ThisItem;

				Body[BodyID].Items[ItemID] = 0;

				Items[ThisItem].KnowID = TempID;

				mass_RemoveBodyItem(BodyID, ItemID);
				InformPlayerofItem(Player, ThisItem, ItemBackpack);

				CheckStats(Player);
			}
			else
			{
				if (Body[BodyID].DecayState < 3) 
				{
					// Not lootable, babai. Or an idea: make GMs able to loot the body anyway. :)

					break;
				}
		
				if (Clients[Player].uChar->Level < _NEWBIE_PROTECTION) // Can't loot newbs.
					break;


				// Alright, item looted..

				TempID = GetFreeBackpackSlot(Player);
				if (TempID == -1) break;

				Clients[Player].uChar->Backpack[TempID].ID = ThisItem;

				Body[BodyID].Items[ItemID] = 0;

				Items[ThisItem].KnowID = TempID;

				mass_RemoveBodyItem(BodyID, ItemID);
				InformPlayerofItem(Player, ThisItem, ItemBackpack);

				if (!Body[BodyID].LooterCrim)
				{
					Clients[Player].Acc->uChar->Wanted = true;

					Clients[Player].uChar->CrimCount += 1;

					InformMassofNewCriminalStatus(Player);
				}

				Body[BodyID].Looted = true;
				strcpy(Body[BodyID].LooterName, Clients[Player].uChar->Name);

				mass_InformNewLooter(BodyID);
				Player_Inform_GotLooted(Body[BodyID].LooterName, BodyID);

				CheckStats(Player);
			}

			break;	
		
		}

		break;
	case 28: // Requesting someone's backpack for viewing (only GMs atm, I had the idea of Dransik's Snoop command.. :)

		Who = 0; 
		long it;

		recv(Clients[Player].ClientSocket, buffer, sizeof(Who), 0);

		if (Clients[Player].uChar->DevMode < 2) break;

		memcpy(&Who, buffer, sizeof(Who));

		Who = Client_GetID(Who);

		printf("Backpack request...\n");

		if (Who >= 0 && Who != Player)
		{			
			printf("Player found!\n");
		//	if (IsInRange(Clients[Player].uChar->X, Clients[Player].uChar->Y, Clients[Who].uChar->X, Clients[Who].uChar->Y))
		//	{
					buffer[0] = 12;
					buffer[1] = 22;
					memcpy(&buffer[2], &Clients[Who].uChar->Name, sizeof(Clients[Who].uChar->Name));
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Clients[Who].uChar->Name), 0);

				for (i = 0; i < MaxBackpackItems; i++)
				{
					if (Clients[Who].uChar->Backpack[i].ID != 0)
					{
						it = Clients[Who].uChar->Backpack[i].ID;

						if (Items[it].Type == 0 || Items[it].Family == 0)
						{
							printf("Istant!!!!\n");
							memset(&Items[it], 0, sizeof(Items[it]));
							Clients[Who].uChar->Backpack[i].ID = 0;
						}
						else
						{
							Items[it].KnowID = i;
						
							buffer[0] = 12;
							buffer[1] = 20;
				
							memcpy(&buffer[2], &Items[it], sizeof(Items[it]));
				
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Items[it]), 0);
						}
					}
				}
			//}
		}

		break;

	case 29: // Requesting someone's bank for viewing (only Lead GMs and higher, DevMode >= 4.)

		Who = 0; 
		it = 0;

		recv(Clients[Player].ClientSocket, buffer, sizeof(Who), 0);

		if (Clients[Player].uChar->DevMode < 4) break;

		memcpy(&Who, buffer, sizeof(Who));

		Who = Client_GetID(Who);

		printf("Bank request...\n");

		if (Who >= 0 && Who != Player)
		{
			printf("Player found!\n");
			//if (IsInRange(Clients[Player].uChar->X, Clients[Player].uChar->Y, Clients[Who].uChar->X, Clients[Who].uChar->Y))
			//{
					buffer[0] = 12;
					buffer[1] = 23;
					memcpy(&buffer[2], &Clients[Who].uChar->Name, sizeof(Clients[Who].uChar->Name));
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Clients[Who].uChar->Name), 0);

				for (i = 0; i < MaxBankItems; i++)
				{
					if (Clients[Who].uChar->Bank[i].ID != 0)
					{
						it = Clients[Who].uChar->Bank[i].ID;
						Items[it].KnowID = i;

						buffer[0] = 12;
						buffer[1] = 21;
				
						memcpy(&buffer[2], &Items[it], sizeof(Items[it]));
				
						send(Clients[Player].ClientSocket, buffer, 2 + sizeof(Items[it]), 0);
					}
				}
			//}
		}

		break;

	case 30: // Player targetted something.

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		switch (buffer[0])
		{
		case 1: // Target is monster

			recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));

			if (Clients[Player].uChar->DevMode == 1)
				break;

			Clients[Player].MonTarget = GetMonsterByID(TmpID);
			if (Clients[Player].MonTarget != NULL)
			{
				Clients[Player].PLTarget = -1;
				Clients[Player].NPCTarget = NULL;

				Clients[Player].AttackTarget = 1;

				Clients[Player].LstAtk = GetTickCount64();			
			}

			break;
		case 2: // Target is another player

			recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));
            
			if (Clients[Player].uChar->DevMode == 1)
				break;

			Clients[Player].PLTarget = Client_GetID(TmpID);
			if (Clients[Player].PLTarget != -1)
			{
				if (Clients[Clients[Player].PLTarget].uChar->DevMode > 0 && Clients[Player].uChar->DevMode < 5)
					Clients[Player].PLTarget = -1;

				Clients[Player].MonTarget = NULL;
				Clients[Player].NPCTarget = NULL;

				Clients[Player].AttackTarget = 0;

				Clients[Player].LstAtk = GetTickCount64();			
			}

			if (Clients[Player].PLTarget == Player)
				Clients[Player].PLTarget = -1;

			break;

		case 3: // Target is NPC

			recv(Clients[Player].ClientSocket, buffer, sizeof(TmpID), 0);
			memcpy(&TmpID, buffer, sizeof(TmpID));
            
			if (TmpID < 1) break;

			Clients[Player].NPCTarget = GetNPCByID( TmpID );

			if (Clients[Player].NPCTarget != NULL)
			{
				Clients[Player].MonTarget = NULL;
				Clients[Player].PLTarget = -1;

				Clients[Player].AttackTarget = 2;

				Clients[Player].LstAtk = GetTickCount64();			
			}

			break;
		case 4: // TArget removed.

			Clients[Player].MonTarget = NULL;
			Clients[Player].PLTarget = -1;
			Clients[Player].NPCTarget = NULL;

			break;		
		}


		break;

	// Someone is asking info about some NPC.
	case 31: 

		//---------------------------------------------------------------------
		// If the NPC is shopkeeper, send all his items to client.
		// Banker needs just to send buffer[0] = X, and client will open bank.
		//---------------------------------------------------------------------
		
		recv(Clients[Player].ClientSocket, buffer, sizeof(short), 0);

		short Temppi2;

		memcpy(&Temppi2, buffer, sizeof(short));

		Temp = Temppi2;

		if (Temp < 1) break;

		ox = Clients[Player].uChar->X;
		oy = Clients[Player].uChar->Y;


		short hshx[9], hshy[9];
		hshx[0] = ox / 64;
		hshy[0] = oy / 64;
		hshx[1] = hshx[0] -1;
		hshy[1] = hshy[0] -1; 
		hshx[2] = hshx[0] -1;
		hshy[2] = hshy[0]; 
		hshx[3] = hshx[0] -1;
		hshy[3] = hshy[0] +1; 
		hshx[4] = hshx[0];
		hshy[4] = hshy[0] -1; 
		hshx[5] = hshx[0];
		hshy[5] = hshy[0] +1; 
		hshx[6] = hshx[0] +1;
		hshy[6] = hshy[0] -1; 
		hshx[7] = hshx[0] +1;
		hshy[7] = hshy[0]; 
		hshx[8] = hshx[0] +1;
		hshy[8] = hshy[0] +1; 

		NPCClass *npc;
		npc = NULL;

		for (i = 0; i < 9; i++)
		{
			if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
			{
				npc_List = NPC[hshx[i]][hshy[i]];

				while (npc_List != NULL)
				{
					ox = npc_List->X;
					oy = npc_List->Y;
					if (npc_List->ID == Temp)
					{
						npc = npc_List;
					    npc_List = NULL;
						i = 9;
					}	
	
					if (npc_List != NULL)
						npc_List = npc_List->next;	
				}			
			}
		}


		if (npc == NULL) break;

		tx = npc->X;
		ty = npc->Y;

		if (ox - tx < -1 ||
			ox - tx > 1 ||
			oy - ty < -1 ||
			oy - ty > 1)				
		break;

		if (ox - tx > -3 && ox - tx < 3 && oy - ty > -3 && oy - ty < 3)
		{
			if (NPCTypes[ npc->Type ].is_shop) // Shopkeeper
			{
				buffer[0] = 31;
				buffer[1] = 5;
				send(Clients[Player].ClientSocket, buffer, 2, 0);

				short i2;
				for (i2 = 0; i2 < m_Store_Items; i2++)
				{
					memset(&NewShopItem, 0, sizeof(NewShopItem));
	
					if (ShopStorages[ npc->ShopID ].Type[i2] > 0)
					{
						buffer[0] = 31; 
						buffer[1] = 1;
	
						NewShopItem.ID = ShopStorages[npc->ShopID].ID[i2];
						NewShopItem.Type = ShopStorages[npc->ShopID].Type[i2];
						NewShopItem.Family = ShopStorages[npc->ShopID].Family[i2];
						NewShopItem.RealID = i2;

                        memcpy(&buffer[2], &NewShopItem, sizeof(NewShopItem));

						send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewShopItem), 0);
					}
				}
				Clients[Player].BrowsingShop = Temp;

				buffer[0] = 31;
				buffer[1] = 6;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
			}
			if (npc->Type == 16) // Banker
			{
				buffer[0] = 32;
				buffer[1] = 1;

				send(Clients[Player].ClientSocket, buffer, 2, 0);	
				Clients[Player].BrowsingBank = 1;
			}
			if (npc->Type == 4) // guild dude
			{
				if (Clients[Player].uChar->Level < GUILDREQ)
				{
					buffer[0] = 32;
					buffer[1] = 3;

					send(Clients[Player].ClientSocket, buffer, 2, 0);
					break;
				}
				if (Clients[Player].Acc->uChar->Guild > 0)
				{
					buffer[0] = 32;
					buffer[1] = 5;

					send(Clients[Player].ClientSocket, buffer, 2, 0);
					break;
				}

				buffer[0] = 32;
				buffer[1] = 2;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
			}
		}

		break;

	case 32: // buy guild

		short NewGuild;
		NewGuild = -1;
		
		/*
		// Guilds turned off atm
		buffer[0] = 32;
		buffer[1] = 9;

		send(Clients[Player].ClientSocket, buffer, 2, 0);
		*/
//*
		for (i = 1; i < MaxGuilds; i++)
		{
			if (Guilds[i].Active == 0)
			{
				NewGuild = i;
				i = MaxGuilds;
			}
		}
		if (NewGuild == -1)
		{
			buffer[0] = 32;
			buffer[1] = 6;

			send(Clients[Player].ClientSocket, buffer, 2, 0);
			break;
		}

		if (Clients[Player].uChar->Level < GUILDREQ) 
		{
			buffer[0] = 32;
			buffer[1] = 3;

			send(Clients[Player].ClientSocket, buffer, 2, 0);
			break;
		}

		bool _gError;
			_gError = false;
		for (i = 0; i < MaxBackpackItems; i++)
		{
			if (Items[Clients[Player].uChar->Backpack[i].ID].Type == TYPE_COLLECTABLE &&
				Items[Clients[Player].uChar->Backpack[i].ID].ID == 0 &&
				Items[Clients[Player].uChar->Backpack[i].ID].Quantity >= GUILDCOST)
			{
				
				_gError = true;
                       
				RemoveItemFromPlayer(Player, ItemBackpack, i);
				memset(&Items[Clients[Player].uChar->Backpack[i].ID], 0, sizeof(Items[Clients[Player].uChar->Backpack[i].ID]));
				Clients[Player].uChar->Backpack[i].ID = 0;

				BuyGuild(Player, NewGuild);
				i = MaxBackpackItems;
			}				
		}

		if (!_gError)
		{
			buffer[0] = 32;
			buffer[1] = 4;

			send(Clients[Player].ClientSocket, buffer, 2, 0);
			break;
		}
//*/

		break;

	case 40: // Shop, BUY

		PACKET_BUY NewBuy;
		memset(&NewBuy, 0, sizeof(NewBuy));
	
		recv(Clients[Player].ClientSocket, buffer, sizeof(NewBuy), 0);
		memcpy(&NewBuy, buffer, sizeof(NewBuy));

		// Get the NPC we are dealing with

		npc = NULL;

		ox = Clients[Player].uChar->X;
		oy = Clients[Player].uChar->Y;

		hshx[0] = ox / 64;
		hshy[0] = oy / 64;
		hshx[1] = hshx[0] -1;
		hshy[1] = hshy[0] -1; 
		hshx[2] = hshx[0] -1;
		hshy[2] = hshy[0]; 
		hshx[3] = hshx[0] -1;
		hshy[3] = hshy[0] +1; 
		hshx[4] = hshx[0];
		hshy[4] = hshy[0] -1; 
		hshx[5] = hshx[0];
		hshy[5] = hshy[0] +1; 
		hshx[6] = hshx[0] +1;
		hshy[6] = hshy[0] -1; 
		hshx[7] = hshx[0] +1;
		hshy[7] = hshy[0]; 
		hshx[8] = hshx[0] +1;
		hshy[8] = hshy[0] +1; 

		for (i = 0; i < 9; i++)
		{
			if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
			{
				npc_List = NPC[hshx[i]][hshy[i]];

				while (npc_List != NULL)
				{
					ox = npc_List->X;
					oy = npc_List->Y;
					if (npc_List->ID == Clients[Player].BrowsingShop)
					{
						npc = npc_List;
					    npc_List = NULL;
						i = 9;
					}	
	
					if (npc_List != NULL)
						npc_List = npc_List->next;	
				}			
			}
		}

		if (npc == NULL) 
		{
			// Abort Trade

			buffer[0] = 31; 
			buffer[1] = 7;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			break;
		}

		bool failed;
		failed = false;
		long cost; cost = 0; 
		unsigned long omoney, omoneys[MaxBackpackItems]; omoney = 0;
		bool omoneyis[MaxBackpackItems];
		memset(&omoneyis, 0, sizeof(omoneyis));
		short s;
		s = npc->ShopID;
		long id;

		for (i = 0; i < 8; i++)
		{
			printf("Newbuy[%d].amount = %lu - ", i, NewBuy.Amount[i]);
			if (NewBuy.ID[i] != -1)
			{
				// Check itemlist for incorrect amounts.
				id = NewBuy.ID[i];

				if (NewBuy.Amount[i] > 100)
					failed = true;
				if (NewBuy.Amount[i] < 0)
					failed = true;

				if (ShopStorages[s].Type[id] > 0)
				{
					switch (ShopStorages[s].Family[id])
					{
					case FAMILY_WEAPON:
						cost += Weapons[ShopStorages[s].Type[id]][ShopStorages[s].ID[id]].Value;
						break;
					case FAMILY_ARMOR:
						cost += Armors[ShopStorages[s].Type[id] - 11][ShopStorages[s].ID[id]].Value;
						break;
					case FAMILY_COLLECTABLE:
						cost += Collectables[ShopStorages[s].ID[id]].Value * (NewBuy.Amount[i] > 0 ? NewBuy.Amount[i] : 1);
						break;
					case FAMILY_USEABLE:
						cost += Useables[ShopStorages[s].ID[id]].Value;
						break;					
					}
				}
			}
		}
		printf("\n", i, NewBuy.Amount[i]);

		if (failed) break;

		for (i = 0; i < MaxBackpackItems; i++)
		{
			if (Clients[Player].uChar->Backpack[i].ID > 0)
			{
				id = Clients[Player].uChar->Backpack[i].ID;
				if (Items[id].Family == FAMILY_COLLECTABLE && Items[id].Type == TYPE_COLLECTABLE && Items[id].ID == 0)
				{
					omoney += Items[id].Quantity;
					omoneys[i] = Items[id].Quantity;
					omoneyis[i] = true;
				}
			}
		}

		if (cost > omoney)
		{
			// Trade failed.

			buffer[0] = 31; 
			buffer[1] = 3;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			break;
		}

		i = 0;
		id = 0;

		while (cost > 0 && i < MaxBackpackItems)
		{
			id = Clients[Player].uChar->Backpack[i].ID;
			if (omoneyis[i] && id > 0)
			{
				if (omoneys[i] <= cost)
				{
					// Remove this moneypile.
				
					cost -= omoneys[i];
					RemoveItemFromPlayer(Player, ItemBackpack, i);
					memset(&Items[id], 0, sizeof(Items[id]));
					Clients[Player].uChar->Backpack[i].ID = 0;
				}
				else
				{
					Items[id].Quantity -= cost;
					cost -= omoneys[i];

					InformPlayerOfNewAmount(Player, ItemBackpack, i, Items[id].Quantity);
				}
			}
			i++;
		}

		for (i = 0; i < 8; i++)
		{
			if (NewBuy.ID[i] != -1 && ShopStorages[npc->ShopID].Type[NewBuy.ID[i]] != NULL)
			{
				ItemStruct			ThisItem;

				if (NewBuy.Amount[i] > 0)
					ThisItem.Quantity = NewBuy.Amount[i];
				else
					ThisItem.Quantity = 1;

				ThisItem.Family		= ShopStorages[npc->ShopID].Family[NewBuy.ID[i]];
				ThisItem.Type		= ShopStorages[npc->ShopID].Type[NewBuy.ID[i]];
				ThisItem.ID			= ShopStorages[npc->ShopID].ID[NewBuy.ID[i]];
				ThisItem.x			= rand() % 100;
				ThisItem.y			= rand() % 100;
				ThisItem.Durability = 200;
				ThisItem.Used		= true;

				TempID = GetFreeItemID();
				if (TempID == -1) break;

				This = GetFreeBackpackSlot(Player);
				if (This == -1) break;

				memcpy(&Items[TempID], &ThisItem, sizeof(ThisItem));
                    
				Clients[Player].uChar->Backpack[This].ID = TempID;

				Items[Clients[Player].uChar->Backpack[This].ID].KnowID = This;

				InformPlayerofItem(Player, Clients[Player].uChar->Backpack[This].ID, ItemBackpack);
			}
		}

		buffer[0] = 31; 
		buffer[1] = 4;
		send(Clients[Player].ClientSocket, buffer, 2, 0);

		CheckStats(Player);

		break;

	case 41: // Sell

		memset(&NewBuy, 0, sizeof(NewBuy));
	
		recv(Clients[Player].ClientSocket, buffer, sizeof(NewBuy), 0);
		memcpy(&NewBuy, buffer, sizeof(NewBuy));

		cost = 0;

		for (i = 0; i < 8; i++)
		{
			if (NewBuy.ID[i] != -1)
			{
				id = Clients[Player].uChar->Backpack[NewBuy.ID[i]].ID;
				
				if (id > 0)
				{
					if (Items[id].Quantity >= NewBuy.Amount[i])
					{
						if (Items[id].Family == FAMILY_WEAPON)
						{
							if (Weapons[Items[id].Type][Items[id].ID].Level == 100)
								NewBuy.ID[i] = -1;
							else
								cost += Weapons[ Items[id].Type ][ Items[id].ID ].Value/2;
						}
			
						if (Items[id].Family == FAMILY_ARMOR)
						{
							if (Armors[Items[id].Type-11][Items[id].ID].Level == 100)
								NewBuy.ID[i] = -1;
							else
								cost += Armors[ Items[id].Type -11 ][ Items[id].ID ].Value/2;
						}

						if (Items[id].Family == FAMILY_COLLECTABLE)
						{
//							if (Collectables[Items[id].ID].Level == 100)
//								NewBuy.ID[i] = -1;
//							else
							cost += (Collectables[ Items[id].ID ].Value / 2) * NewBuy.Amount[i];
						}
	
						if (Items[id].Family == FAMILY_USEABLE)
						{
//							if (Useables[Items[id].ID].lev.Level == 100)
//								NewBuy.ID[i] = -1;
//							else
							cost += Useables[ Items[id].ID ].Value/2;
						}

						if (NewBuy.ID[i] != -1)
						{
							if (Items[id].Quantity <= NewBuy.Amount[i] || Items[id].Family != FAMILY_COLLECTABLE)
							{
								// Remove whole pile.
								RemoveItemFromPlayer(Player, ItemBackpack, NewBuy.ID[i]);				
								memset(&Items[id], 0, sizeof(Items[id]));
								Clients[Player].uChar->Backpack[NewBuy.ID[i]].ID = 0;						
							}
							else
							{
								Items[id].Quantity -= NewBuy.Amount[i];

								InformPlayerOfNewAmount(Player, ItemBackpack, NewBuy.ID[i], Items[id].Quantity);													
							}
						}
					}
				}
			}
		}

		if (cost <= 0) break;

		ItemStruct NewItemC;
		NewItemC.Durability = 1;
		NewItemC.Family		= FAMILY_COLLECTABLE;
		NewItemC.ID			= 0;
		NewItemC.Quantity	= cost;
		NewItemC.Type		= TYPE_COLLECTABLE;
		NewItemC.x			= rand() % 100;
		NewItemC.y			= rand() % 100;
		NewItemC.Used		= true;


		This = GetFreeBackpackSlot(Player);
		if (This == -1) break;


		TempID = GetFreeItemID();
		if (TempID == -1) break;


		memcpy(&Items[TempID], &NewItemC, sizeof(NewItemC));
                    
		Clients[Player].uChar->Backpack[This].ID = TempID;
		Items[Clients[Player].uChar->Backpack[This].ID].KnowID = This;

		InformPlayerofItem(Player, Clients[Player].uChar->Backpack[This].ID, ItemBackpack);

		buffer[0] = 31; 
		buffer[1] = 4;
		send(Clients[Player].ClientSocket, buffer, 2, 0);

		CheckStats(Player);

		break;

	case 50: // Guild things

		recv(Clients[Player].ClientSocket, buffer, 1, 0);
		
		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		switch (buffer[0])
		{
		case 1: // Guild Name changed.

			unsigned long accID;

			printf("Guild Name change.\n");

			PACKET_NEWGUILDNAME NewName;
			memset(&NewName, 0, sizeof(NewName));

			recv(Clients[Player].ClientSocket, buffer, sizeof(NewName), 0);

			memcpy(&NewName, buffer, sizeof(NewName));

			if (strlen( NewName.Name ) < 1 || strlen( NewName.Name ) > 43)
			{
				buffer[0] = 40; 
				buffer[1] = 21;
				memcpy(&buffer[2], &NewName, sizeof(NewName));
				send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewName), 0);

				break;
			}
			short State; State = Clients[Player].State;
			short Guild; Guild = Clients[Player].Acc->uChar->Guild;

			if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
				Guilds[Guild].Leader_Slot == State)
			{
				bool Error2;
				Error2 = false;
				for (i = 1; i < MaxGuilds; i++)
				{
					if (Guilds[i].Active == 1)
					{
						if (!strcmp(NewName.Name, Guilds[i].Name))
						{
							// Name already taken.
							Error2 = true;
							
							buffer[0] = 40; 
							buffer[1] = 18;
							memcpy(&buffer[2], &NewName, sizeof(NewName));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewName), 0);
						}
					}
				}

				if (Error2) break;

				// Inform all players of this name change.

				strcpy(Guilds[Guild].Name, NewName.Name);	
				accID = Clients[Player].Acc->getAccID();
				
				buffer[0] = 40; 
				buffer[1] = 5;
				memcpy(&buffer[2], &NewName, sizeof(NewName));

				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (Clients[i].Acc->uChar->Guild == Guild)
						{		
							buffer[0] = 40;
							buffer[1] = 5;
							memcpy(&buffer[2], &NewName, sizeof(NewName));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewName), 0);						
						}
					}

				}
			}
			break;
		case 2: // Tag changed.

			printf("Tag change.\n");

			PACKET_NEWGUILDTAG NewTag;
			memset(&NewTag, 0, sizeof(NewTag));

			recv(Clients[Player].ClientSocket, buffer, sizeof(NewTag), 0);

			memcpy(&NewTag, buffer, sizeof(NewTag));

			State = Clients[Player].State;
			Guild = Clients[Player].Acc->uChar->Guild;

			if (strlen( NewTag.Tag ) < 1 || strlen( NewTag.Tag ) > 4)
			{
				buffer[0] = 40; 
				buffer[1] = 22;
				memcpy(&buffer[2], &NewTag, sizeof(NewTag));
				send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewTag), 0);

				break;
			}

			if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
				Guilds[Guild].Leader_Slot == State)
			{
				bool Error2;
				Error2 = false;
				for (i = 1; i < MaxGuilds; i++)
				{
					if (Guilds[i].Active == 1)
					{
						if (!strcmp(NewTag.Tag, Guilds[i].Tag))
						{
							// Tag already taken.
							Error2 = true;
							
							buffer[0] = 40; 
							buffer[1] = 17;
							memcpy(&buffer[2], &NewTag, sizeof(NewTag));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewTag), 0);
						}
					}
				}

				if (Error2) break;

				strcpy(Guilds[Guild].Tag, NewTag.Tag);

				// Inform all players of this tag change.

				accID = Clients[Player].Acc->getAccID();
				
				buffer[0] = 40; 
				buffer[1] = 4;
				memcpy(&buffer[2], &NewTag, sizeof(NewTag));

				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (Clients[i].Acc->uChar->Guild == Guild)
						{
							strcpy(Clients[i].Acc->uChar->Tag, NewTag.Tag);
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewTag), 0);							
						}
					}

				}
			}

			break;
		case 3: // Added member

			short _ID;
			_ID = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(_ID), 0);

			printf("Guild Member Add...\n");

            memcpy(&_ID, buffer, sizeof(_ID));

			_ID = Client_GetID( _ID );

			if (_ID >= 0)
			{
				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

				if (Clients[_ID].Acc->uChar->Guild > 0)
					break;

				short freeslot;
					  freeslot = -1;
				bool approve;
					 approve = false;

				printf("Looking for free slot...\n");

				for (int i = 0; i < 50; i++)
				{
					if (Guilds[Guild].Members[i].Active == 0 && freeslot == -1)
						freeslot = i;
					if (Guilds[Guild].Members[i].rank >= 3 &&
						Guilds[Guild].Members[i].AccID == Clients[Player].Acc->getAccID() &&
						Guilds[Guild].Members[i].Slot == Clients[Player].State)
						approve = true;

					if (freeslot != -1 && approve)
						i = 50;
				}


				if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[Guild].Leader_Slot == Clients[Player].State)
					approve = true;

				printf("freeslot = %d, approve = %d\n", freeslot, approve);


				if (approve && freeslot != -1)
				{
					printf("Adding Member...\n");
					Guilds[Guild].Members[freeslot].AccID = Clients[_ID].Acc->getAccID();
					Guilds[Guild].Members[freeslot].Slot = Clients[_ID].State;
					strcpy(Guilds[Guild].Members[freeslot].Name, Clients[_ID].uChar->Name);
					Guilds[Guild].Members[freeslot].Active = 1;
                            
					strcpy(Clients[_ID].Acc->uChar->Tag, Guilds[Guild].Tag);

					Clients[_ID].Acc->uChar->Guild = Guild;

					// Send info to player
					Clients[_ID].Acc->informGuild();
							
					buffer[0] = 40;
					buffer[1] = 6;
					send(Clients[_ID].ClientSocket, buffer, 2, 0);

					buffer[0] = 40;
					buffer[1] = 4;
					strcpy(NewTag.Tag, Clients[_ID].Acc->uChar->Tag);
					memcpy(&buffer[2], &NewTag, sizeof(NewTag));
					send(Clients[_ID].ClientSocket, buffer, 2 + sizeof(NewTag), 0);
									
					buffer[0] = 40;
					buffer[1] = 5;
					strcpy(Guilds[Guild].Name, NewName.Name);	
					memcpy(&buffer[2], &NewName, sizeof(NewName));
					send(Clients[_ID].ClientSocket, buffer, 2 + sizeof(NewName), 0);

					PACKET_NEWGUILDMEMBER NewMember;
					memset(&NewMember, 0, sizeof(NewMember));
					strcpy(NewMember.Name[freeslot], Clients[_ID].uChar->Name);
					NewMember.ID[freeslot] = _ID;
					NewMember.rank[freeslot] = 0;
					NewMember.Status[freeslot] = true;
					
					buffer[0] = 40;
					buffer[1] = 1;
					memcpy(&buffer[2], &NewMember, sizeof(NewMember));
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewMember), 0);

					memset(&NewCoordinates, 0, sizeof(NewCoordinates));
					
					NewCoordinates.X = Clients[_ID].uChar->X;
					NewCoordinates.Y = Clients[_ID].uChar->Y;
					NewCoordinates.ID = Clients[_ID].Acc->getAccID();
					
					CheckPlayersOnScreen(_ID, NewCoordinates);
					CheckMonstersOnScreen(_ID);
					CheckNPCsOnScreen(_ID);
					CheckItemsOnScreen(_ID);

					Disconnect( _ID );
					
				}
			}

			break;
		case 4: // Removed member
			
			_ID = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(_ID), 0);

            memcpy(&_ID, buffer, sizeof(_ID));

			if (_ID >= 0)
			{
				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

				short slot, orank;
					  slot = -1;
					  orank = -1;

				bool approve;
					 approve = false;

				for (i = 0; i < 50; i++)
				{
					if (Guilds[Guild].Members[i].AccID == _ID)
						slot = i;

					if (Guilds[Guild].Members[i].rank >= 3 &&
						Guilds[Guild].Members[i].AccID == Clients[Player].Acc->getAccID() &&
						Guilds[Guild].Members[i].Slot == Clients[Player].State)
					{
						approve = true;
						orank = i;
					}

					if (slot != -1 && approve)
						i = 50;
				}

				if (orank > Guilds[Guild].Members[slot].rank)
					approve = true;
				else approve = false;

				if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[Guild].Leader_Slot == Clients[Player].State)
					approve = true;

				if (approve)
				{
                    for (i = 0; i < 50; i++)
					{
						if (Guilds[Guild].Members[i].AccID == _ID)
						{
							Guilds[Guild].Members[i].AccID = 0;
							Guilds[Guild].Members[i].Slot = 0;
							memset(&Guilds[Guild].Members[i].Name, 0, sizeof (Guilds[Guild].Members[i].Name));
                            Guilds[Guild].Members[i].Active = 0;
                            
							buffer[0] = 40;
							buffer[1] = 2;
							memcpy(&buffer[2], &_ID, sizeof(_ID));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(_ID), 0);

							_ID = Client_GetID(_ID);
							if (_ID >= 0)
							{
								memset(&Clients[_ID].Acc->uChar->Tag, 0, sizeof(Clients[_ID].Acc->uChar->Tag));
	
								Clients[_ID].Acc->uChar->Guild = 0;
								
								buffer[0] = 40;
								buffer[1] = 7;
								send(Clients[_ID].ClientSocket, buffer, 2, 0);
							}
						
							i = 50;
						}
					}
				}
			}


			break;
		case 5: // Member is leaving.

			printf("Guild Member Leave...\n");

			short InGuild;
			InGuild = Clients[Player].Acc->uChar->Guild;

			if (Clients[Player].Acc->getAccID() == Guilds[InGuild].Leader_AccID &&
				Clients[Player].State == Guilds[InGuild].Leader_Slot) // Member is leader so guild is disbanded
			{
				printf("Guild Disband...\n");

				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (Clients[i].Acc->uChar->Guild == Guild)
							{
								for (int z = 0; z < 50; z++)
								{
									Guilds[Guild].Members[z].AccID = 0;
									Guilds[Guild].Members[z].Slot = 0;
									memset(&Guilds[Guild].Members[z].Name, 0, sizeof (Guilds[Guild].Members[z].Name));
									Guilds[Guild].Members[z].Active = 0;
								}

								memset(&Clients[i].Acc->uChar->Tag, 0, sizeof(Clients[i].Acc->uChar->Tag));
				
								Clients[i].Acc->uChar->Guild = 0;
								memset(&Clients[i].Acc->uChar->Guild, 0, sizeof(Clients[i].Acc->uChar->Guild));

								buffer[0] = 40;
								buffer[1] = 8;
								send(Clients[i].ClientSocket, buffer, 2, 0);
							}
						}
					}
				memset(&Guilds[Guild], 0, sizeof(Guilds[Guild]));
			}

			for (i = 0; i < 50; i++)
			{
				if (Guilds[InGuild].Members[i].AccID == Clients[Player].Acc->getAccID() &&
					Guilds[InGuild].Members[i].Slot == Clients[Player].State)
				{
					Guilds[InGuild].Members[i].AccID = 0;
					Guilds[InGuild].Members[i].Slot = 0;
					memset(&Guilds[InGuild].Members[i].Name, 0, sizeof (Guilds[InGuild].Members[i].Name));
					Guilds[InGuild].Members[i].Active = 0;

					_ID = Guilds[InGuild].Leader_AccID;

					memset(&Clients[Player].Acc->uChar->Tag, 0, sizeof(Clients[Player].Acc->uChar->Tag));	
					Clients[Player].Acc->uChar->Guild = 0;
					memset(&Clients[Player].Acc->uChar->Guild, 0, sizeof(Clients[Player].Acc->uChar->Guild));
								
					buffer[0] = 40;
					buffer[1] = 13;
					send(Clients[Player].ClientSocket, buffer, 2, 0);

					_ID = Client_GetID(_ID);
					if (_ID >= 0 && Clients[_ID].State == Guilds[InGuild].Leader_Slot)
					{
						buffer[0] = 40;
						buffer[1] = 2;
						memcpy(&buffer[2], &_ID, sizeof(_ID));
						send(Clients[_ID].ClientSocket, buffer, 2 + sizeof(_ID), 0);
					}

					i = 50;
				}
			}
			break;
		case 6: // disband
			printf("Guild Disband...\n");

			State = Clients[Player].State;
			Guild = Clients[Player].Acc->uChar->Guild;

			if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
				Guilds[Guild].Leader_Slot == State)
			{
				buffer[0] = 50;
				buffer[1] = 8;
				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (Clients[i].Acc->uChar->Guild == Guild)
						{
							send(Clients[i].ClientSocket, buffer, 2, 0);
							
							buffer[0] = 40;
							buffer[1] = 8;
							send(Clients[i].ClientSocket, buffer, 2, 0);
						}
					}
				}
                memset(&Guilds[Guild], 0, sizeof(Guilds[Guild]));                
			}
			break;
		case 7: // Deed Transfer

			_ID = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(_ID), 0);

            memcpy(&_ID, buffer, sizeof(_ID));
			_ID = Client_GetID( _ID );

			if (_ID >= 0)
			{
				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

				if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[Guild].Leader_Slot == State)
				{
                    for (i = 0; i < 50; i++)
					{
						if (Guilds[Guild].Members[i].AccID == _ID)
						{
							GuildMember SwapMember;
							SwapMember.AccID = Guilds[Guild].Leader_AccID;
							SwapMember.Slot = Guilds[Guild].Leader_Slot;
							strcpy(SwapMember.Name, Clients[Player].uChar->Name);
                            
							Guilds[Guild].Leader_AccID = Guilds[Guild].Members[i].AccID;
							Guilds[Guild].Leader_Slot = Guilds[Guild].Members[i].Slot;

							Guilds[Guild].Members[i].AccID = SwapMember.AccID;
							Guilds[Guild].Members[i].Slot = SwapMember.Slot;
							strcpy(Guilds[Guild].Members[i].Name, SwapMember.Name);
                            
							buffer[0] = 40;
							buffer[1] = 14;
							memcpy(&buffer[2], &_ID, sizeof(_ID));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(_ID), 0);

							i = 50;
						}
					}
				}
			}

			break;

		case 8: // Promote

			printf("Guild Promote...\n");

			_ID = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(_ID), 0);

            memcpy(&_ID, buffer, sizeof(_ID));
			_ID = Client_GetID( _ID );

			if (_ID >= 0)
			{
				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

				if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[Guild].Leader_Slot == State)
				{
                    for (i = 0; i < 50; i++)
					{
						if (Guilds[Guild].Members[i].AccID == _ID)
						{
							Guilds[Guild].promote(i);
							i = 50;

							buffer[0] = 40;
							buffer[1] = 10;
							memcpy(&buffer[2], &Guilds[Guild].Members[i].AccID, sizeof(Guilds[Guild].Members[i].AccID));
							send(Clients[Guilds[Guild].Members[i].AccID].ClientSocket, buffer, 2 + sizeof(Guilds[Guild].Members[i].AccID), 0);
						}
					}
				}
			}
			break;

		case 9: // Demote
			
			printf("Guild Demote....\n");

			_ID = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(_ID), 0);

            memcpy(&_ID, buffer, sizeof(_ID));
			_ID = Client_GetID( _ID );

			if (_ID >= 0)
			{
				State = Clients[Player].State;
				Guild = Clients[Player].Acc->uChar->Guild;

				if (Guilds[Guild].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[Guild].Leader_Slot == State)
				{
                    for (i = 0; i < 50; i++)
					{
						if (Guilds[Guild].Members[i].AccID == _ID)
						{
							Guilds[Guild].demote(i);
							i = 50;

							buffer[0] = 40;
							buffer[1] = 11;
							send(Clients[Guilds[Guild].Members[i].AccID].ClientSocket, buffer, 2, 0);
						}
					}
				}
			}
			break;

		case 10: // Get player list
			PACKET_NEWGUILDMEMBER NewPlayer;

			Guild = Clients[Player].Acc->uChar->Guild;

			for (int g = 0; g < MaxGuilds; g++)
			{
				if (Guilds[Guild].Name == Guilds[g].Name)
				{
					for (int m = 0; m < 50; m++)
					{
						if (Guilds[Guild].Members[m].AccID) // Check if member slot is used
						{
							memset(&NewPlayer, 0, sizeof(NewPlayer));

							if (Guilds[Guild].Members[m].Active == 1) // Check player status				
								NewPlayer.Status[m] = true;
							else
								NewPlayer.Status[m] = false;
										
								strcpy(NewPlayer.Name[m], Guilds[Guild].Members[m].Name); // Get member name
								NewPlayer.ID[m] = Guilds[Guild].Members[m].AccID; // Get member ID
								NewPlayer.rank[m] = Guilds[Guild].Members[m].rank; // Get member rank

								buffer[0] = 40;
								buffer[1] = 15;
								memcpy(&buffer[2], &NewPlayer, sizeof(NewPlayer));
								send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewPlayer), 0); // Send info to player
						}
					}
				}	
				g = MaxGuilds;
			}
			break;

		}

		break;

	case 53: // Used map object

		short use_X, use_Y;
		recv(Clients[Player].ClientSocket, buffer, sizeof(use_X), 0);
		memcpy(&use_X, buffer, sizeof(use_X));

		recv(Clients[Player].ClientSocket, buffer, sizeof(use_Y), 0);
		memcpy(&use_Y, buffer, sizeof(use_Y));

		if (use_X - Clients[Player].uChar->X < -1 ||
			use_X - Clients[Player].uChar->X > 1 ||
			use_Y - Clients[Player].uChar->Y < -1 ||
			use_Y - Clients[Player].uChar->Y > 1)				
		break;

		hashx = use_X / 64;
		hashy = use_Y / 64;

		if (hashx < 0 || hashx >= 128 || hashy < 0 || hashy >= 128) break;

		obj_List = Objects[hashx][hashy];

		while (obj_List != NULL)
		{
			if (obj_List->X == use_X && obj_List->Y == use_Y)
			{
				switch (obj_List->Type)
				{
				case 28:
				case 29:
				case 55:
				case 56:
				case 58:
				case 2: // TELEPORT

					memset(&Clients[Player].KnowMonster, 0, sizeof(Clients[Player].KnowMonster));
					memset(&Clients[Player].KnowBody, 0, sizeof(Clients[Player].KnowBody));
					memset(&Clients[Player].KnowItem, 0, sizeof(Clients[Player].KnowItem));
					memset(&Clients[Player].KnowNPC, 0, sizeof(Clients[Player].KnowNPC));
					memset(&Clients[Player].KnowPlayer, 0, sizeof(Clients[Player].KnowPlayer));
					
					unsigned long accID;
					accID = Clients[Player].Acc->getAccID();

					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));
	
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
							}
						}
					}
	
					Clients[Player].uChar->X = obj_List->_X;
					Clients[Player].uChar->Y = obj_List->_Y;
	
					memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
					NewCoordinates.X = Clients[Player].uChar->X;
					NewCoordinates.Y = Clients[Player].uChar->Y;
					NewCoordinates.ID = Clients[Player].Acc->getAccID();
	
					// Inform player himself for moving around.
	
					buffer[0] = 10;
					buffer[1] = 1;
					memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
		
					send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);
	
					CheckPlayersOnScreen(Player, NewCoordinates);
					CheckMonstersOnScreen(Player);
					CheckNPCsOnScreen(Player);
					CheckItemsOnScreen(Player);
					
					break;
				case 72: 
				case 73:
				case 12: // Ladder down
				case 11: // Ladder up
		
					memset(&Clients[Player].KnowMonster, 0, sizeof(Clients[Player].KnowMonster));
					memset(&Clients[Player].KnowBody, 0, sizeof(Clients[Player].KnowBody));
					memset(&Clients[Player].KnowItem, 0, sizeof(Clients[Player].KnowItem));
					memset(&Clients[Player].KnowNPC, 0, sizeof(Clients[Player].KnowNPC));
					memset(&Clients[Player].KnowPlayer, 0, sizeof(Clients[Player].KnowPlayer));

					accID = Clients[Player].Acc->getAccID();

					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));
	
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
							}
						}
					}
	
					Clients[Player].uChar->X = obj_List->_X;
					Clients[Player].uChar->Y = obj_List->_Y;
	
					memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
					NewCoordinates.X = Clients[Player].uChar->X;
					NewCoordinates.Y = Clients[Player].uChar->Y;
					NewCoordinates.ID = Clients[Player].Acc->getAccID();
	
					// Inform player himself for moving around.
	
					buffer[0] = 10;
					buffer[1] = 1;
					memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
		
					send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);

					CheckPlayersOnScreen(Player, NewCoordinates);
					CheckMonstersOnScreen(Player);
					CheckNPCsOnScreen(Player);
					CheckItemsOnScreen(Player);

					break;
				case 4: // SPAWN

					Clients[Player].uChar->ShrineX = obj_List->X;
					Clients[Player].uChar->ShrineY = obj_List->Y;

					buffer[0] = 99;
					buffer[1] = 61;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
	
					break;
				case 10: // SIGN

					break;
				case 1: // DOOR

					for (i = 0; i < 25; i++)
					{
						if (!Objects_Use[i])
						{
							Objects_Use[i] = true;
			
							Objects_UseTime[i] = GetTickCount64();
	
							UseTargets[i] = obj_List;

							obj_List->use(Player);
							
							i = 25;
						}
					}

					break;
	
				case 3: // Teleportspawn
	
					accID = Clients[Player].Acc->getAccID();

					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));
	
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
							}
						}
					}
	
					Clients[Player].uChar->X = obj_List->_X;
					Clients[Player].uChar->Y = obj_List->_Y;
	
					memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
					NewCoordinates.X = Clients[Player].uChar->X;
					NewCoordinates.Y = Clients[Player].uChar->Y;
					NewCoordinates.ID = Clients[Player].Acc->getAccID();
	
					// Inform player himself for moving around.
	
					buffer[0] = 10;
					buffer[1] = 1;
					memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
		
					send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);
	
					Clients[Player].uChar->ShrineX = obj_List->_X;
					Clients[Player].uChar->ShrineY = obj_List->_Y;
	
					buffer[0] = 99;
					buffer[1] = 61;
					send(Clients[Player].ClientSocket, buffer, 2, 0);

					CheckPlayersOnScreen(Player, NewCoordinates);
					CheckMonstersOnScreen(Player);
					CheckNPCsOnScreen(Player);
					CheckItemsOnScreen(Player);

					break;		
				}
			}

			if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		break;


	case 54: // Secure trade

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		switch (buffer[0])
		{
		case 1: // Trade is locked

		case 2: // Trade is unlocked

			bool lockmode;
			if (buffer[0] == 1)
				lockmode = true;
			else lockmode = false;

			short curtrade;
			curtrade = -1;

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].TradeInProgress && (SecureTrade[i].Traders[0] == Player || SecureTrade[i].Traders[1] == Player))
				{
					curtrade = i;
					i = 10;
				}
			}

			if (curtrade == -1)
				break;

			SecureTrade[curtrade].LastAction = GetTickCount64();
			short t;
			
			if (SecureTrade[curtrade].Traders[0] == Player)
				t = 0;
			else t = 1;

			SecureTrade[curtrade].Lock[t] = lockmode;


			buffer[0] = 54;
			buffer[1] = 1;
			buffer[2] = 1;
			send(Clients[Player].ClientSocket, buffer, 3, 0);

			buffer[0] = 54;
			buffer[1] = 2;
			buffer[2] = 1;
			send(Clients[ SecureTrade[curtrade].Traders[t == 0 ? 1 : 0] ].ClientSocket, buffer, 3, 0);

			if (!SecureTrade[curtrade].Lock[0] || !SecureTrade[curtrade].Lock[1])
				break;


			SecureTrade[curtrade].do_trade();

			
			break;

		case 3: // Offer trade to someone

			short FreeTrade;
			FreeTrade = -1;
			long ID;

			recv(Clients[Player].ClientSocket, buffer, sizeof(ID), 0);
			memcpy(&ID, buffer, sizeof(ID));

			for (i = 0; i < 10; i++)
			{
				if (!SecureTrade[i].TradeInPending && !SecureTrade[i].TradeInProgress)
				{
					FreeTrade = i;
					i = 10;
				}
			}

			if (FreeTrade == -1)
			{
				printf("WARNING! No free secure trades left!");
				buffer[0] = 54;
				buffer[1] = 20;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

			SecureTrade[FreeTrade].LastAction = GetTickCount64();

			SecureTrade[FreeTrade].TradeInPending = true;
			SecureTrade[FreeTrade].Traders[0] = Player;
//			SecureTrade[FreeTrade].PendingTime = GetTickCount64();

			memset(&SecureTrade[FreeTrade].tradeitems, 0, sizeof(SecureTrade[FreeTrade].tradeitems));
			memset(&SecureTrade[FreeTrade].Lock, 0, sizeof(SecureTrade[FreeTrade].Lock));
			
			bool boo;
			boo = false;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (Clients[i].Acc->getAccID() == ID)
					{
						ID = i;
						boo = true;
						i = Max_Connections;
					}
				}
			}


			if (!boo) SecureTrade[FreeTrade].Abort();

			SecureTrade[FreeTrade].Traders[1] = ID;

			buffer[0] = 54;
			buffer[1] = 10;
			memcpy(&buffer[2], &Clients[Player].uChar->Name, sizeof(Clients[Player].uChar->Name));
			send(Clients[Clients[ID].Acc->getAccID()].ClientSocket, buffer, 2 + sizeof(Clients[Player].uChar->Name), 0);

			break;

		case 4: // Someone accepted a trade offered to him

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].Traders[1] == Player && SecureTrade[i].TradeInPending)
				{
					// This is the trade.

					SecureTrade[i].LastAction = GetTickCount64();

					buffer[0] = 54;
					buffer[1] = 11;
					buffer[2] = strlen(Clients[SecureTrade[i].Traders[0]].uChar->Name);
					memcpy(&buffer[3], &Clients[SecureTrade[i].Traders[0]].uChar->Name, strlen(Clients[SecureTrade[i].Traders[0]].uChar->Name));
					send(Clients[Player].ClientSocket, buffer, 3 + strlen(Clients[SecureTrade[i].Traders[0]].uChar->Name), 0);

					buffer[0] = 54;
					buffer[1] = 11;
					buffer[2] = strlen(Clients[Player].uChar->Name);
					memcpy(&buffer[3], &Clients[Player].uChar->Name, strlen(Clients[Player].uChar->Name));
					send(Clients[SecureTrade[i].Traders[0]].ClientSocket, buffer, 3 + strlen(Clients[Player].uChar->Name), 0);

					SecureTrade[i].TradeInPending = false;
					SecureTrade[i].TradeInProgress = true;
					memset(&SecureTrade[i].Lock, 0, sizeof(SecureTrade[i].Lock));
					for (int yy = 0; yy < 16; yy++)
					{
						SecureTrade[i].tradeitems[0][yy] = -1;
						SecureTrade[i].tradeitems[1][yy] = -1;
					}
//					memset(&SecureTrade[i].tradeitems, -1, sizeof(SecureTrade[i].tradeitems));

					Clients[SecureTrade[i].Traders[0]].TradeinProgress = true;
					Clients[SecureTrade[i].Traders[1]].TradeinProgress = true;

					i = 10;
				}
			}

			break;

		case 5: // Player offers an item

			curtrade = -1;

			short id;
			id = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(id), 0);
			memcpy(&id, buffer, sizeof(id));

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].TradeInProgress && (SecureTrade[i].Traders[0] == Player || SecureTrade[i].Traders[1] == Player))
				{
					curtrade = i;
					i = 10;
				}
			}

			if (curtrade == -1)
				break;
			
			SecureTrade[curtrade].LastAction = GetTickCount64();

			if (SecureTrade[curtrade].Traders[0] == Player)
				t = 0;
			else t = 1;

			if (Clients[Player].uChar->Backpack[id].ID <= 0)
				break;


			if (!Items[Clients[Player].uChar->Backpack[id].ID].Used ||
				Items[Clients[Player].uChar->Backpack[id].ID].KnowID != id)
				break;

			printf("asdfasdf: KnowID = %d\n", id);

			long item;
			item = Clients[Player].uChar->Backpack[id].ID;


			short fslot;
			fslot = -1;
			bool err;
			err = false;

			for (i = 0; i < 8; i++)
			{
				if (SecureTrade[curtrade].tradeitems[t][i] == id)
				{
					err = true;
					i = 16;
				}
			}

			if (err) break;

			err = false;

			for (j = 0; j < 8; j++)
			{
				if (SecureTrade[curtrade].tradeitems[t][j] == -1)
				{
					err = true;
					SecureTrade[curtrade].tradeitems[t][j] = id;

					buffer[0] = 54;
					buffer[1] = 5;

					short f, b, i;
					unsigned long a;
					f = Items[item].Family;
					b = Items[item].Type;
					i = Items[item].ID;
					a = Items[item].Quantity;

					memcpy(&buffer[2], &f, sizeof(f));
					memcpy(&buffer[2 + sizeof(f)], &b, sizeof(b));
					memcpy(&buffer[2 + sizeof(f) + sizeof(b)], &i, sizeof(i));
					memcpy(&buffer[2 + 3 * sizeof(short)], &a, sizeof(a));
					short tt;
					tt = id;
					memcpy(&buffer[2 + 3 * sizeof(short) + sizeof(a)], &tt, sizeof(tt));

					send(Clients[SecureTrade[curtrade].Traders[t == 0 ? 1 : 0]].ClientSocket, buffer, 2 + 4 * sizeof(short) + sizeof(unsigned long), 0);


					printf("id = %d sent\n", id);
					buffer[0] = 54;
					buffer[1] = 4;
					memcpy(&buffer[2], &id, sizeof(id));

					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(id), 0);
					
					j = 8;
				}			
			}

			if (!err) break;

			memset(&SecureTrade[curtrade].Lock, 0, sizeof(SecureTrade[curtrade].Lock));

			buffer[0] = 54;
			buffer[1] = 3;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			send(Clients[SecureTrade[curtrade].Traders[t == 0 ? 1 : 0]].ClientSocket, buffer, 2, 0);

			break;

		case 6: // Item was removed from trade

			curtrade = -1;

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].TradeInProgress && (SecureTrade[i].Traders[0] == Player || SecureTrade[i].Traders[1] == Player))
				{
					curtrade = i;
					i = 10;
				}
			}

			if (curtrade == -1)
				break;
			
			SecureTrade[curtrade].LastAction = GetTickCount64();

			if (SecureTrade[curtrade].Traders[0] == Player)
				t = 0;
			else t = 1;

			id = -1;
			recv(Clients[Player].ClientSocket, buffer, sizeof(id), 0);
			memcpy(&id, buffer, sizeof(id));

			if (Clients[Player].uChar->Backpack[id].ID <= 0)
				break;

			if (!Items[Clients[Player].uChar->Backpack[id].ID].Used ||
				Items[Clients[Player].uChar->Backpack[id].ID].KnowID != id)
				break;


			item = Clients[Player].uChar->Backpack[id].ID;


			fslot = -1;
			err = false;

			for (i = 0; i < 16; i++)
			{
				if (SecureTrade[curtrade].tradeitems[t][i] == id)
				{
					err = true;
					i = 16;
				}
			}

			if (!err) break;

			for (i = 0; i < 16; i++)
			{
				if (SecureTrade[curtrade].tradeitems[t][i] == id)
				{
					SecureTrade[curtrade].tradeitems[t][i] = -1;

					buffer[0] = 54;
					buffer[1] = 4;
					memcpy(&buffer[2], &id, sizeof(id));
					send(Clients[Player].ClientSocket, buffer, 2 + sizeof(short), 0);
					send(Clients[SecureTrade[curtrade].Traders[t == 0 ? 1 : 0]].ClientSocket, buffer, 2 + sizeof(short), 0);
					
					i = 16;
				}			
			}

			memset(&SecureTrade[curtrade].Lock, 0, sizeof(SecureTrade[curtrade].Lock));

			buffer[0] = 54;
			buffer[1] = 3;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
			send(Clients[SecureTrade[curtrade].Traders[t == 0 ? 1 : 0]].ClientSocket, buffer, 2, 0);

			break;

		case 7: // Abort trade

			curtrade = -1;

			for (i = 0; i < 10; i++)
			{
				if (SecureTrade[i].TradeInProgress && (SecureTrade[i].Traders[0] == Player || SecureTrade[i].Traders[1] == Player))
				{
					curtrade = i;
					i = 10;
				}
			}

			if (curtrade == -1)
				break;

			SecureTrade[curtrade].Abort();

			break;
			

		default: break;
		}

		break;
	//////////////////////////////////////////
	//		HERES THE MINING
	//////////////////////////////////////////
	case 55: // Mining

		PACKET_MINEAT MineAt;
		memset(&MineAt, 0, sizeof(MineAt));

		recv(Clients[Player].ClientSocket, buffer, sizeof(MineAt), 0);
		memcpy(&MineAt, buffer, sizeof(MineAt));

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		if (Clients[Player].uChar->Backpack[MineAt.WithItem].ID == 0 || !Items[Clients[Player].uChar->Backpack[MineAt.WithItem].ID].Used) break;

		if (Items[Clients[Player].uChar->Backpack[MineAt.WithItem].ID].KnowID != MineAt.WithItem)
			break;

		short tmx, tmy;
		tmx = MineAt._x;
		tmy = MineAt._y;

		ox = Clients[Player].uChar->X;
		oy = Clients[Player].uChar->Y;
		tx = MineAt._x;
		ty = MineAt._y;

		if (ox - tx < -1 ||
			ox - tx > 1  ||
			oy - ty < -1 ||
			oy - ty > 1)				
		break;

		if (Items[Clients[Player].uChar->Backpack[MineAt.WithItem].ID].Family == FAMILY_USEABLE &&
			Items[Clients[Player].uChar->Backpack[MineAt.WithItem].ID].Type == TYPE_USEABLEITEM)
		{
			long item;
			item = Clients[Player].uChar->Backpack[MineAt.WithItem].ID;

			short _ore;
				_ore = -1;

			short _suc, Skill;
			_suc = 0;
			Skill = Clients[Player].uChar->Skills[15].Level;
			
			if ( Map.Chunks[0][ Map.ChunkMap[ tmx >> 4 ][ tmy >> 4 ] ][ tmx % 16 ][ tmy % 16] == 72)
			{
				_suc = 10;
				_suc += ((float)Skill) * 0.9;
				_ore = 4;
			}
			else if ( Map.Chunks[0][ Map.ChunkMap[ tmx >> 4 ][ tmy >> 4 ] ][ tmx % 16 ][ tmy % 16] == 150)
			{
				if (Skill < 30)
				{
					_ore = -2;
				}
				else
				{
					_suc = 10;
					_suc += ((float)Skill - 20) * 0.3;
					_ore = 5;
				}
			}
			else if ( Map.Chunks[0][ Map.ChunkMap[ tmx >> 4 ][ tmy >> 4 ] ][ tmx % 16 ][ tmy % 16] == 114)
			{
				if (Skill < 60)
				{
					_ore = -2;
				}
				else
				{
					_suc = 5;
					_suc += ((float)Skill - 50) * 0.25;
					_ore = 6;
				}
			}
			else if ( Map.Chunks[0][ Map.ChunkMap[ tmx >> 4 ][ tmy >> 4 ] ][ tmx % 16 ][ tmy % 16] == 457)
			{
				if (Skill < 90)
				{
					_ore = -2;
				}
				else
				{
					_suc = 5;
					_suc += ((float)Skill - 80) * 0.5;
					_ore = 7;
				}
			}
			
			if (_ore == -1)
			{
				buffer[0] = 99;
				buffer[1] = 55;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

			if ( Ores[tmx >> 4][ tmy >> 4] <= 0)
			{
				buffer[0] = 99;
				buffer[1] = 56;
				send(Clients[Player].ClientSocket, buffer, 2, 0);		
				break;
			}

			Ores[tmx >> 4][ tmy >> 4]--;

			Clients[Player].lst_tradeskill = GetTickCount64();

			Items[item].Durability--;

			InformNewItemDurability(Player, Items[item].KnowID);
                        
			if (Items[item].Durability <= 0)
			{
				Clients[Player].t_useitembroke = true;
				Clients[Player].t_useitemid = item;
			}
			else Clients[Player].t_useitembroke = false;

			if (_suc >= rand() % 100 && _ore != -2)
			{
				Clients[Player].t_tradesucceed = true;				
				if (Useables[Items[item].ID].DamMin <= 0)
					Clients[Player].t_recvam = 1;
				else
				{
					Clients[Player].t_recvam = rand() % (Useables[Items[item].ID].DamMax - Useables[Items[item].ID].DamMin) + 1;
					Clients[Player].t_recvam += Useables[Items[item].ID].DamMin;
				}
				if (Clients[Player].t_recvam <= 0 ) Clients[Player].t_recvam = 1;

				Clients[Player].t_tarid = _ore;
				Clients[Player].t_recvexp = Clients[Player].t_recvam * 10 * (Clients[Player].t_tarid > 4 ? Clients[Player].t_tarid - 3 : 1) + Skill * 2;
			}
			else
			{
				Clients[Player].t_tradesucceed = false;
				Clients[Player].t_recvam = 0;
				Clients[Player].t_tarid = -1;
				Clients[Player].t_recvexp = (Clients[Player].t_recvam * 10 * (Clients[Player].t_tarid > 4 ? Clients[Player].t_tarid - 3 : 1) + Skill * 2) / 3;
			}

			Clients[Player].tradeskill_inuse = true;
			Clients[Player].t_useskill = 1;

			Clients[Player].lst_tradeskill = GetTickCount64();

			buffer[0] = 99; 
			buffer[1] = 59;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
		}

		break;
	//////////////////////////////////////////
	//	HERES THE SMELTING
	//////////////////////////////////////////
	case 56: // Smelting.

		PACKET_SMELTAT SmeltAt;
		memset(&SmeltAt, 0, sizeof(SmeltAt));

		recv(Clients[Player].ClientSocket, buffer, sizeof(SmeltAt), 0);

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		memcpy(&SmeltAt, buffer, sizeof(SmeltAt));

		if (Clients[Player].uChar->Backpack[SmeltAt.WithItem].ID == 0 || !Items[Clients[Player].uChar->Backpack[SmeltAt.WithItem].ID].Used) break;
		if (Items[Clients[Player].uChar->Backpack[SmeltAt.WithItem].ID].KnowID != SmeltAt.WithItem)
			break;

		long item;
		item = Clients[Player].uChar->Backpack[SmeltAt.WithItem].ID;

		obj_List = Objects[SmeltAt.x / 64][SmeltAt.y / 64];
		
		bool con;
		con = false;

		while (obj_List != NULL && !con)
		{
			if (obj_List->X == SmeltAt.x && obj_List->Y == SmeltAt.y && ObjectTypes[obj_List->Type].UseType == use_SMELT)
				con = true;
		
			if (!con)
				obj_List = obj_List->next;
		}

		if (!con) break;

		short _tx, _ty;
		_tx = Clients[Player].uChar->X;
		_ty = Clients[Player].uChar->Y;
				
		short _x, _y;
		_x = obj_List->X;
		_y = obj_List->Y;

		if (_tx - _x < -1 || _tx - _x > 1 || _ty - _y < -1 || _ty - _y > 1) break;

		if (Items[item].Type == TYPE_COLLECTABLE &&
			Collectables[Items[item].ID].use_type == use_SMELT)
		{
			Clients[Player].lst_tradeskill = GetTickCount64();

			short _suc;
			_suc = 0;

			if (rand() % 100 <= 10)
				_suc = 100;
			else if (rand() % 100 <= 25)
				_suc = 75;
			else if (rand() % 100 <= 30)
				_suc = 50;
			else if (rand() % 100 <= 50)
                _suc = 25;

            long HowMany;
			long QTY;
			long origQTY = Items[item].Quantity;
				if (SmeltAt.am == 0)
					QTY = 10;
				else if (SmeltAt.am == 1) 
					QTY = 25;
				else if (SmeltAt.am == 2) 
					QTY = 50;
				else if (SmeltAt.am == 3) 
					QTY = 100;
				else if (SmeltAt.am == 4) 
					QTY = Items[item].Quantity;

			if (QTY >= Items[item].Quantity)
			{
				QTY = Items[item].Quantity;
			} 

			HowMany = QTY;

			HowMany *= (1 - ((float)_suc / 100));

			long HowManyFailed;

			HowManyFailed = QTY - HowMany;

			if (HowManyFailed < 0) HowManyFailed = 0;

			Clients[Player].t_recvam = HowMany;
			Clients[Player].t_failam = HowManyFailed;

			Clients[Player].tradeskill_inuse = true;
			Clients[Player].t_useskill = 2;

			Clients[Player].t_useitemid = item;

			Clients[Player].t_recvexp = (Clients[Player].t_recvam * 10 * (Clients[Player].t_tarid > 4 ? Clients[Player].t_tarid - 3 : 1) * 2) / 3;

			if (QTY < origQTY) // give back rest
			{ 
				This = GetFreeBackpackSlot(Player);
				if (This == -1) break;

				TempID = GetFreeItemID();
				if (TempID == -1) break;

				ItemStruct NewItemC;
				NewItemC.Family		= FAMILY_COLLECTABLE;
				NewItemC.ID			= SmeltAt.id;
				NewItemC.Type		= TYPE_COLLECTABLE;
				NewItemC.Quantity	= origQTY - QTY;
				NewItemC.Used		= true;
				NewItemC.x			= rand() % 100;
				NewItemC.y			= rand() % 100;
				NewItemC.KnowID		= This;

				memset(&tmp, 0, sizeof(tmp));
				printf("returned leftover: ID[%d], Type[%d], Amount[%lu]\n", NewItemC.ID, NewItemC.Type, NewItemC.Quantity);

				memcpy(&Items[TempID], &NewItemC, sizeof(NewItemC));
                    
				Clients[Player].uChar->Backpack[This].ID = TempID;

				Items[Clients[Player].uChar->Backpack[This].ID].KnowID = This;				

				InformPlayerofItem(Player, Clients[Player].uChar->Backpack[This].ID, ItemBackpack);
			}

			buffer[0] = 99; 
			buffer[1] = 73;
			send(Clients[Player].ClientSocket, buffer, 2, 0);
		}

		break;


	//////////////////////////////////////////
	//		HERES THE FORGING
	//////////////////////////////////////////
	case 58: // Forging item

		PACKET_FORGE Forge;

		recv(Clients[Player].ClientSocket, buffer, sizeof(Forge), 0);

		if (GetTickCount64() - Clients[Player].lst_tradeskill < 3000)
			break;

		memcpy(&Forge, buffer, sizeof(Forge));

		if (Forge.Family == 0 || Forge.Type == 0 || Forge.ID < 0) break;

		if (Clients[Player].uChar->Backpack[Forge.WithItem_ID].ID == 0 || 
			Items[Clients[Player].uChar->Backpack[Forge.WithItem_ID].ID].KnowID != Forge.WithItem_ID)
			break;

		Clients[Player].lst_tradeskill = GetTickCount64();

		if (Items[Clients[Player].uChar->Backpack[Forge.WithItem_ID].ID].Quantity <= 0) break;

		long Item;
			 Item = Clients[Player].uChar->Backpack[Forge.WithItem_ID].ID;

		bool Ero;
		Ero = false;
		if (Collectables[Items[Item].ID].use_type == use_FORGE && 
			Items[Item].Type == TYPE_COLLECTABLE && 
			Items[Item].Family == FAMILY_COLLECTABLE) Ero = true;
           
		if (!Ero) break;

		short Skill, skill;

		Skill = Clients[Player].uChar->Skills[16].Level;
		skill = Skill;

		short _suc;

		_suc = Skill + Clients[Player].Acc->tChar.Int;
		if (Skill < 1) Skill = 1;

		if (Skill > 100) Skill = 100;

		buffer[0] = 99; 
		buffer[1] = 74;
		send(Clients[Player].ClientSocket, buffer, 2, 0);

		if (Forge.Family == FAMILY_COLLECTABLE || Forge.Family == FAMILY_USEABLE)
			break;

		if (Forge.Family != FAMILY_WEAPON && Forge.Family != FAMILY_ARMOR)
			break;

		short bsReq; bsReq = (Forge.Family == FAMILY_WEAPON ? Weapons[Forge.Type][Forge.ID].bs_req : Armors[Forge.Type - 11][Forge.ID].bs_req);
		short bsAm;  bsAm  = (Forge.Family == FAMILY_WEAPON ? Weapons[Forge.Type][Forge.ID].bs_amount : Armors[Forge.Type - 11][Forge.ID].bs_amount);
		short bsMT;  bsMT  = (Forge.Family == FAMILY_WEAPON ? Weapons[Forge.Type][Forge.ID].bs_metaltype : Armors[Forge.Type - 11][Forge.ID].bs_metaltype);
		short reqMT; reqMT = Items[Item].Family == FAMILY_COLLECTABLE ? Collectables[Items[Item].ID].bs_metaltype : -1;
		short bsDif; bsDif = (Forge.Family == FAMILY_WEAPON ? Weapons[Forge.Type][Forge.ID].bs_difficulty : Armors[Forge.Type - 11][Forge.ID].bs_difficulty);

		short result;

		if (reqMT == 0 || bsMT == 0) 
			break;
		if (bsAm <= 0) 
			break;
		if (Skill < bsReq)
			break;
		if (Items[Item].Quantity < bsAm)
			break;
		if (reqMT != bsMT)
			break;


		result = 50 - bsDif + Skill/2;
		
		if (result < 0) result = 0;
		if (result > 100) result = 100;

		if (rand() % 100 > result)
		{
			Clients[Player].tradeskill_inuse = true;
			Clients[Player].t_useskill = 3;
			Clients[Player].t_useitemid = Forge.WithItem_ID;
			Clients[Player].t_tradesucceed = false;
			Clients[Player].t_recvexp = (bsAm * 5 * (bsMT - 7) + Skill) / 3;
			Clients[Player].t_useitem = Item;
			Clients[Player].t_cost = bsAm;

			Clients[Player].t_failam = (Forge.Family == FAMILY_WEAPON ? Weapons[Forge.Type][Forge.ID].bs_amount / 2 : Armors[Forge.Type - 11][Forge.ID].bs_amount / 2);
		}
		else
		{
			Clients[Player].t_recvexp = bsAm * 5 * (bsMT - 7) + Skill;
			Clients[Player].tradeskill_inuse = true;
			Clients[Player].t_useskill = 3;
			Clients[Player].t_useitemid = Forge.WithItem_ID;
			Clients[Player].t_tradesucceed = true;
			Clients[Player].t_tarid = Forge.ID;
			Clients[Player].t_tartype = Forge.Type;
			Clients[Player].t_tarfam = Forge.Family;
			Clients[Player].t_cost = bsAm;
			Clients[Player].t_useitem = Item;
		}

		break;


	//////////////////////////////////////////
	///			GM SHIT
	//////////////////////////////////////////
	case 60: // GM Things
		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		switch (buffer[0])
		{
		case 1: // GM Mode on

			if (Clients[Player].uChar->DevMode >= 1)
			{
				//Clients[Player].logfile << Date << " " << Time << " [ACTIVATE]: GM Mode " << Clients[Player].uChar->DevMode << endl;;
				GMACTION << Time << ": " << Clients[Player].uChar->Name << " activated level " << Clients[Player].uChar->DevMode << "moderator mode.\n";

				printf("%s: %s activated Level %d moderator Mode.\n", Time, Clients[Player].uChar->Name, Clients[Player].uChar->DevMode);
				Clients[Player].GMMode = true;

				buffer[0] = 60;
				buffer[1] = 1;
				buffer[2] = Clients[Player].uChar->DevMode;
				send(Clients[Player].ClientSocket, buffer, 3, 0);

				InformGMofOnlineList( Player );

			}
           
			break;

		case 2: // Invisibility on

			bool Err;
				Err = false;

			char tmp[80];

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode >= 1)
			{
				unsigned long accID;
				accID = Clients[Player].Acc->getAccID();
				if (!Clients[Player].Inv)
				{
					//Clients[Player].logfile << Date << " " << Time << " [ACTIVATE]: Invisibility " << endl;
					memset(&tmp, 0, sizeof(tmp));
					sprintf(tmp, "%s %s: [ACTIVATE]: Invisibility\n", Date, Time);
					Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

					printf("%s: %s activated Invisibility.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].Inv = true;
					Err = true;

					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));
					
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								if (Clients[Player].uChar->X - Clients[i].uChar->X >= -11 &&
									Clients[Player].uChar->X - Clients[i].uChar->X <= 11 &&
									Clients[Player].uChar->Y - Clients[i].uChar->Y >= -11 &&
									Clients[Player].uChar->Y - Clients[i].uChar->Y <= 11 &&
									Clients[Player].uChar->DevMode > Clients[i].uChar->DevMode)
								{
									Clients[i].KnowPlayer[Player] = false;
									send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
								}
							}
						}
					}
				}
				if (Clients[Player].Inv && !Err)
				{
					memset(&tmp, 0, sizeof(tmp));
					sprintf(tmp, "%s %s: [DEACTIVATE]: Invisibility\n", Date, Time);
					Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
					
					//Clients[Player].logfile << Date << " " << Time << " [DEACTIVATE]: Invisibility " <<
					
					printf("%s: %s deactivated Invisibility.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].Inv = false;

					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));
						
					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);							
							}
						}
			
						Clients[i].KnowPlayer[Player] = false;
					}
					
					memset(&NewCoordinates, 0, sizeof(NewCoordinates)); //Entered in by Radiankh

					CheckPlayersOnScreen(Player, NewCoordinates);
					CheckItemsOnScreen(Player);
					CheckMonstersOnScreen(Player);
					CheckNPCsOnScreen(Player);

				}

				buffer[0] = 60;
				buffer[1] = 2;
				if (Clients[Player].Inv)
				buffer[2] = 1;
				if (!Clients[Player].Inv)
				buffer[2] = 0;
				send(Clients[Player].ClientSocket, buffer, 3, 0);
			}
			break;
		case 4: // Normal mode listening on/off

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode >= 1)
			{
				if (!Clients[Player].ListenNormal)
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is listening to serverwide normal chat.\n";
					printf("%s: %s is now listening serverwide normal chat.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenNormal = true;
				}
				else
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is no longer listening to serverwide normal chat.\n";
					printf("%s: %s is no longer listening serverwide normal chat.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenNormal = false;
				}

				buffer[0] = 60;
				buffer[1] = 4;
				if (Clients[Player].ListenNormal)
				buffer[2] = 1;
				if (!Clients[Player].ListenNormal)
				buffer[2] = 0;
				send(Clients[Player].ClientSocket, buffer, 3, 0);
			}

			break;
		case 49: // Guild listening mode on/off

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode >= 1)
			{
				if (!Clients[Player].ListenGuild)
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is listening to serverwide guild chat.\n";
					printf("%s: %s is now listening serverwide normal chat.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenGuild = true;
				}
				else
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is no longer listening to serverwide guild chat.\n";
					printf("%s: %s is no longer listening serverwide normal chat.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenGuild = false;
				}

				buffer[0] = 60;
				buffer[1] = 49;
				if (Clients[Player].ListenGuild)
				buffer[2] = 1;
				if (!Clients[Player].ListenGuild)
				buffer[2] = 0;
				send(Clients[Player].ClientSocket, buffer, 3, 0);
			}

			break;
		case 5: // Whisper

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 1)
			{
				if (!Clients[Player].ListenWhisper)
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is listening to serverwide whisper chat.\n";
					printf("%s: %s activated Listening Whisper mode.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenWhisper = true;
				}
				else
				{
					GMACTION << Time << ": " << Clients[Player].uChar->Name << " is no longer listening to serverwide whisper chat.\n";
					printf("%s: %s deactivated Listening Whisper mode.\n", Time, Clients[Player].uChar->Name);
					Clients[Player].ListenWhisper = false;
				}

				buffer[0] = 60;
				buffer[1] = 5;
				if (Clients[Player].ListenWhisper)
				buffer[2] = 1;
				if (!Clients[Player].ListenWhisper)
				buffer[2] = 0;
				send(Clients[Player].ClientSocket, buffer, 3, 0);
			}

			break;
		case 6: // Ask stats. Needed dev status is higher than EWC

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 1)
			{

				PACKET_ASKSTATS AskStats;
				memset(&AskStats, 0, sizeof(AskStats));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(AskStats), 0);
				memcpy(&AskStats, buffer, sizeof(AskStats));

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{

						if (!strcmp(Clients[i].uChar->Name, AskStats.Name))
						{
							PACKET_INFOSTATS NewInfo;
							memset(&NewInfo, 0, sizeof(NewInfo));

							Err = true;

							NewInfo.Cha = Clients[i].uChar->Cha;
							NewInfo.Con = Clients[i].uChar->Con;
							NewInfo.Dex = Clients[i].uChar->Dex;
							NewInfo.Int = Clients[i].uChar->Int;
							NewInfo.Str = Clients[i].uChar->Str;
							NewInfo.Lck = Clients[i].uChar->Lck;
							NewInfo.Level = Clients[i].uChar->Level;
							NewInfo.HPLeft = Clients[i].uChar->HPLeft;
							NewInfo.HPMax = Clients[i].Acc->Calc_hpMax();
							strcpy(NewInfo.Name, Clients[i].uChar->Name);
							strcpy(NewInfo.AccName, Clients[i].Acc->getAccName());
							NewInfo.Exp = Clients[i].uChar->Exp;

							buffer[0] = 60;
							buffer[1] = 6;
							memcpy(&buffer[2], &NewInfo, sizeof(NewInfo));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewInfo), 0);
	
							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [REQUEST]: Stats of %s\n", Date, Time, AskStats.Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							//Clients[Player].logfile << Date << " " << Time << " [REQUEST]: Stats of " << AskStats.Name << endl;
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}
			}


			break;

		case 10: // Teleport self

			if (Clients[Player].uChar->DevMode < 1) break;

			PACKET_TELEPORTSELF Teleport;
			recv(Clients[Player].ClientSocket, buffer, sizeof(Teleport), 0);

			memcpy(&Teleport, buffer, sizeof(Teleport));

			memset(&NewCoordinates, 0, sizeof(NewCoordinates));

			NewCoordinates.X = Teleport.X;
			NewCoordinates.Y = Teleport.Y;
			NewCoordinates.ID = Clients[Player].Acc->getAccID();

			Clients[Player].uChar->X = Teleport.X;
			Clients[Player].uChar->Y = Teleport.Y;

			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [TELEPORT]: %d, %d\n", Date, Time, Teleport.X, Teleport.Y);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

			//Clients[Player].logfile << Date << " " << Time << " [TELEPORT]: " << Teleport.X << ", " << Teleport.Y << endl;

			// Inform player himself for moving around.

			buffer[0] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

			send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);

			unsigned long accID;
			accID = Clients[Player].Acc->getAccID();

			buffer[0] = 11;
			buffer[1] = 1;
			memcpy(&buffer[2], &accID, sizeof(accID));
					
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (i != Player)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);							
					}
				}
			
				Clients[i].KnowPlayer[Player] = false;
			}

			CheckPlayersOnScreen(Player, NewCoordinates);
			CheckItemsOnScreen(Player);
			CheckMonstersOnScreen(Player);
			CheckNPCsOnScreen(Player);

			break;


		case 11: // teleport other

			if (Clients[Player].uChar->DevMode < 1) break;

			short TargetPlayer;
			TargetPlayer = -1;
			
			PACKET_TELEPORTOTHER TeleportOther;
			memset(&TeleportOther, 0, sizeof(TeleportOther));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(TeleportOther), 0);
			memcpy(&TeleportOther, buffer, sizeof(TeleportOther));

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State < 2)
				{
					if (!strcmp(Clients[i].uChar->Name, TeleportOther.Name))
					{
						Err = true;

						TargetPlayer = i;
						i = Max_Connections;
					}
				}
			}


			unsigned long accID2;
			accID = Clients[Player].Acc->getAccID();

			if (TargetPlayer > -1)
			{
				accID2 = Clients[TargetPlayer].Acc->getAccID();

				buffer[0] = 11;
				buffer[1] = 1;
				memcpy(&buffer[2], &accID, sizeof(accID));

				buffer2[0] = 11;
				buffer2[1] = 1;
				memcpy(&buffer2[2], &accID2, sizeof(accID2));
					
				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (i != Player)
						{
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);							
							Clients[i].KnowPlayer[Player] = false;
						}
						if (i != TargetPlayer)
						{
							send(Clients[i].ClientSocket, buffer2, 2 + sizeof(accID2), 0);							
							Clients[i].KnowPlayer[TargetPlayer] = false;
						}
					}				
				}

				memset(&NewCoordinates, 0, sizeof(NewCoordinates));

				NewCoordinates.X = TeleportOther.X;
				NewCoordinates.Y = TeleportOther.Y;
				NewCoordinates.ID = Clients[Player].Acc->getAccID();

				Clients[Player].uChar->X = TeleportOther.X;
				Clients[Player].uChar->Y = TeleportOther.Y;

				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: [TELEPORT OTHER]: %s to %d, %d\n", Date, Time, Clients[TargetPlayer].uChar->Name, TeleportOther.X, TeleportOther.Y);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

				//Clients[Player].logfile << Date << " " << Time << " [TELEPORT OTHER]: Teleport " << Clients[TargetPlayer].uChar->Name << " to " << TeleportOther.X << ", " << TeleportOther.Y << endl;

				GMLOG << Clients[Player].uChar->Name << " teleported " << Clients[TargetPlayer].uChar->Name << " to " << TeleportOther.X << "," << TeleportOther.Y << "." << endl;


				// Inform player himself for moving around.

				buffer[0] = 10;
				buffer[1] = 1;
				memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

				send(Clients[Player].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);

				memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
				NewCoordinates.X = TeleportOther.X;
				NewCoordinates.Y = TeleportOther.Y;
				NewCoordinates.ID = Clients[TargetPlayer].Acc->getAccID();
	
				Clients[TargetPlayer].uChar->X = TeleportOther.X;
				Clients[TargetPlayer].uChar->Y = TeleportOther.Y;
	
				// Inform player himself for moving around.
	
				buffer[0] = 10;
				buffer[1] = 1;
				memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

				send(Clients[TargetPlayer].ClientSocket, buffer, 2 + sizeof(NewCoordinates), 0);

				CheckPlayersOnScreen(Player, NewCoordinates);
				CheckItemsOnScreen(Player);
				CheckMonstersOnScreen(Player);
				CheckNPCsOnScreen(Player);

				CheckPlayersOnScreen(TargetPlayer, NewCoordinates);
				CheckItemsOnScreen(TargetPlayer);
				CheckMonstersOnScreen(TargetPlayer);
				CheckNPCsOnScreen(TargetPlayer);			
			}


            break;
			
			short NewID;

		case 20: // teleport self to someone's coords.

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 2)
			{

				PACKET_TELEPORTTOOTHER ToOther;
				memset(&ToOther, 0, sizeof(ToOther));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(ToOther), 0);
				memcpy(&ToOther, buffer, sizeof(ToOther));				
					memset(&NewCoordinates, 0, sizeof(NewCoordinates));

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{

						if (!strcmp(Clients[i].uChar->Name, ToOther.Name))
						{
							
							if (Clients[i].TradeinProgress || Clients[i].BrowsingShop || Clients[i].BrowsingBank){
								buffer[0] = 99;
								buffer[1] = 62;
								send(Clients[Player].ClientSocket, buffer, 2, 0);

								break;
							}

							PACKET_BACK_TELEPORTTOOTHER BackTo;
							memset(&BackTo, 0, sizeof(BackTo));

							Err = true;

							BackTo.X = Clients[i].uChar->X;
							BackTo.Y = Clients[i].uChar->Y;
							
							NewCoordinates.X = BackTo.X;
							NewCoordinates.Y = BackTo.Y;
							NewCoordinates.ID = Clients[i].Acc->getAccID();

							buffer[0] = 60;
							buffer[1] = 8;
							memcpy(&buffer[2], &BackTo, sizeof(BackTo));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(BackTo), 0);
	
							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [TELEPORT TO]: coords %d, %d\n", Date, Time, Clients[Player].uChar->X, Clients[Player].uChar->Y);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

				CheckPlayersOnScreen(Player, NewCoordinates);
				CheckItemsOnScreen(Player);
				CheckMonstersOnScreen(Player);
				CheckNPCsOnScreen(Player);

			}

			break;

		case 22: // Jail

			if (Clients[Player].uChar->DevMode < 1) break;

			recv(Clients[Player].ClientSocket, buffer, sizeof(NewID), 0);

			memcpy(&NewID, buffer, sizeof(NewID));

			NewID = Client_GetID( NewID );
			if (NewID == -1) break;

			memset(&NewCoordinates, 0, sizeof(NewCoordinates));

			NewCoordinates.X = 83 + LastJail * 3;
			NewCoordinates.Y = 83;
			NewCoordinates.ID = Clients[NewID].Acc->getAccID();

			Clients[NewID].uChar->X = 83 + LastJail * 3;
			Clients[NewID].uChar->Y = 83;

			LastJail++;
			if (LastJail > 2) LastJail = 0;

			// Inform player himself for moving around.

			buffer[0] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

			send(Clients[NewID].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);

			accID = Clients[NewID].Acc->getAccID();

			buffer[0] = 11;
			buffer[1] = 1;
			memcpy(&buffer[2], &accID, sizeof(accID));
					
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{

					if (i != NewID)
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);							
					}
				}
			
				Clients[i].KnowPlayer[NewID] = false;
			}

			break;

		case 23: // Change Rank

			if (Clients[Player].uChar->DevMode < 6) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}
				
				PACKET_CHANGERANK change;
				memset(&change, 0, sizeof(change));

				recv(Clients[Player].ClientSocket, buffer, sizeof(change), 0);
				memcpy(&change, buffer, sizeof(change));				

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (!strcmp(Clients[i].uChar->Name, change.Name))
						{
							Err = true;
							
							if (change.Rank == 0)
							{														
								Clients[i].uChar->DevMode = 0; // Player
								Clients[i].uChar->Reputation = 5;
								Clients[i].GMMode = false;

							} else if (change.Rank == 1){
								Clients[i].uChar->DevMode = 1; // EWC
								Clients[i].uChar->Reputation = 10;
								Clients[i].GMMode = false;
								
							} else if (change.Rank == 2){					
								Clients[i].uChar->DevMode = 2; // GMIT
								Clients[i].uChar->Reputation = 4;								
								Clients[i].uChar->Level = 205;								
								Clients[i].uChar->Str = 100;
								Clients[i].uChar->Dex = 100;
								Clients[i].uChar->Con = 100;
								Clients[i].uChar->Int = 100;
								Clients[i].uChar->Skills[0].Level = 100;
								Clients[i].uChar->Skills[1].Level = 100;
								Clients[i].uChar->Skills[2].Level = 100;
								Clients[i].uChar->Skills[3].Level = 100;
								Clients[i].uChar->Skills[4].Level = 100;
								Clients[i].uChar->Skills[15].Level = 100;
								Clients[i].uChar->Skills[16].Level = 100;
								Clients[i].Acc->tChar.HPMax = Clients[i].Acc->Calc_hpMax();
								Clients[i].GMMode = true;

							} else if (change.Rank == 3){					
								Clients[i].uChar->DevMode = 3; // GM
								Clients[i].uChar->Reputation = 4;
								Clients[i].GMMode = true;

							} else if (change.Rank == 4){	
								Clients[i].uChar->DevMode = 4; // GM Lead
								Clients[i].uChar->Reputation = 4;
								Clients[i].GMMode = true;

							} else if (change.Rank == 5){	
								Clients[i].uChar->DevMode = 5; // DEV
								Clients[i].uChar->Reputation = 4;
								Clients[i].GMMode = true;
							}																	

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [%s Changed %s rank to %d]\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name, change.Rank);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							GMACTION << Clients[Player].uChar->Name << " Changed %s" << Clients[i].uChar->Name << " rank to %d" << change.Rank << ".\n";

						/*
							buffer[0] = 99;
							buffer[1] = 78;
							send(Clients[i].ClientSocket, buffer, 2, 0);

							Disconnect( i );
						*/
							i = Max_Connections;
						}
					}
				}

				if (!Err)
				{
					printf("%s was not found\n", change.Name);
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break; 

		case 24: // Demote player

			break;

		case 25:

			unsigned long am;
			short typ, idi;
			recv(Clients[Player].ClientSocket, buffer, sizeof(idi), 0);
			memcpy(&idi, buffer, sizeof(idi));
			recv(Clients[Player].ClientSocket, buffer, sizeof(typ), 0);
			memcpy(&typ, buffer, sizeof(typ));
			recv(Clients[Player].ClientSocket, buffer, sizeof(am), 0);
			memcpy(&am, buffer, sizeof(am));

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 2)
			{
				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: [CREATE ITEM]: ID[%d], Type[%d], Am[%lu]\n", Date, Time, idi, typ, am);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
				//Clients[Player].logfile << Date << " " << Time << " [CREATE ITEM]: [ID:" << idi << "] [Type:" << typ << "] [Amount:" << am << "]" << endl;
				printf("Create item: ID[%d], Type[%d], Amount[%lu]\n", idi, typ, am);

				ItemStruct NewItemC;
				if (typ < TYPE_ARMOR)
				{
					NewItemC.Family = FAMILY_WEAPON;
					NewItemC.Durability = 10;
				} else if (typ >= TYPE_ARMOR && typ < TYPE_COLLECTABLE){
					NewItemC.Family = FAMILY_ARMOR;
					NewItemC.Durability = 10;
				} else if (typ == TYPE_COLLECTABLE){					
					NewItemC.Family = FAMILY_COLLECTABLE;
					NewItemC.Durability = 1;
				} else if (typ == TYPE_USEABLEITEM){
					NewItemC.Family = FAMILY_USEABLE;					
					NewItemC.Durability = 1;
				}

				NewItemC.ID			= idi;
				NewItemC.Quantity	= am;
				NewItemC.Type		= typ;
				NewItemC.x			= rand() % 100;
				NewItemC.y			= rand() % 100;
				NewItemC.Used		= true;

				This = GetFreeBackpackSlot(Player);
				if (This == -1) break;

				TempID = GetFreeItemID();
				if (TempID == -1) break;


				memcpy(&Items[TempID], &NewItemC, sizeof(NewItemC));
                    
				Clients[Player].uChar->Backpack[This].ID = TempID;

				Items[Clients[Player].uChar->Backpack[This].ID].KnowID = This;

				InformPlayerofItem(Player, Clients[Player].uChar->Backpack[This].ID, ItemBackpack);

				buffer[0] = 31; 
				buffer[1] = 4;
				send(Clients[Player].ClientSocket, buffer, 2, 0);

				GMLOG << Clients[Player].uChar->Name << " created item: [" << idi << "][" << typ << "][am: " << am << "]" << endl;
                
				CheckStats(Player);
			}

			break;
		case 100:
			short nstr;
			recv(Clients[Player].ClientSocket, buffer, sizeof(nstr), 0);
			memcpy(&nstr, buffer, sizeof(nstr));
			Clients[Player].uChar->Str = nstr;
			break;
		case 101:
			char namez[25];
			recv(Clients[Player].ClientSocket, buffer, sizeof(namez), 0);
			memcpy(&namez, buffer, sizeof(namez));
			for(int k=0;k<150;k++)
			{
				if(k!=Player && Clients[k].State > 0)
				{
					if(!strcmp(Clients[k].uChar->Name, namez))
					{
						memset(&Clients[k].KnowMonster, 0, sizeof(Clients[k].KnowMonster));
						memset(&Clients[k].KnowBody, 0, sizeof(Clients[k].KnowBody));
						memset(&Clients[k].KnowItem, 0, sizeof(Clients[k].KnowItem));
						memset(&Clients[k].KnowNPC, 0, sizeof(Clients[k].KnowNPC));
						memset(&Clients[k].KnowPlayer, 0, sizeof(Clients[k].KnowPlayer));
					
						unsigned long accID;
						accID = Clients[k].Acc->getAccID();

						buffer[0] = 11;
						buffer[1] = 1;
						memcpy(&buffer[2], &accID, sizeof(accID));
	
						for (j = 0; j < Max_Connections; j++)
						{
							if (Clients[j].Used && Clients[j].State != 5)
							{
								if (j != k)
								{
									send(Clients[j].ClientSocket, buffer, 2 + sizeof(accID), 0);
								}
							}
						}
	
						Clients[k].uChar->X = Clients[Player].uChar->X;
						Clients[k].uChar->Y = Clients[Player].uChar->Y;
	
						memset(&NewCoordinates, 0, sizeof(NewCoordinates));
	
						NewCoordinates.X = Clients[k].uChar->X;
						NewCoordinates.Y = Clients[k].uChar->Y;
						NewCoordinates.ID = Clients[k].Acc->getAccID();
	
						// Inform player himself for moving around.
		
						buffer[0] = 10;
						buffer[1] = 1;
						memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));
		
						send(Clients[k].ClientSocket, buffer, 2+sizeof(NewCoordinates), 0);
	
						CheckPlayersOnScreen(k, NewCoordinates);
						CheckMonstersOnScreen(k);
						CheckNPCsOnScreen(k);
						CheckItemsOnScreen(k);

						GMACTION << Clients[Player].uChar->Name << " teleported "<< Clients[k].uChar->Name << "to himself!\n";
						break;
					}
				}
			}
			break;
		case 26: // kick from server

			if (Clients[Player].uChar->DevMode < 1) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			TargetPlayer = -1;			
			short ID_;

			PACKET_KICKFROM kickChar;
			memset(&kickChar, 0, sizeof(kickChar));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(kickChar), 0);
			memcpy(&kickChar, buffer, sizeof(kickChar));

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, kickChar.Name))
					{
						Err = true;

						TargetPlayer = i;
						i = Max_Connections;
					}
				}
			}

			accID = Clients[Player].Acc->getAccID();

			if (TargetPlayer > -1)
			{
				accID2 = Clients[TargetPlayer].Acc->getAccID();

				buffer[0] = 99;
				buffer[1] = 3;
				memcpy(&buffer2[2], &accID2, sizeof(accID2));
					
				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State < 2)
					{
						if (i != Player)
						{
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID2), 0);							
							Clients[i].KnowPlayer[TargetPlayer] = false;
						}
					}				
				}		

				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [Kicked]: %s\n", Date, Time, Clients[Player].uChar->Name, Clients[TargetPlayer].uChar->Name);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

				GMACTION << Date <<  ", " << Time << " - " << Clients[Player].uChar->Name << " [kicked] " << Clients[TargetPlayer].uChar->Name << " from the server." << endl;

				ID_ = Client_GetID(Clients[TargetPlayer].Acc->getAccID() );
				Disconnect(ID_);
			}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;
		case 27: // ban from server

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			TargetPlayer = -1;		

			PACKET_BANFROM banChar;
			memset(&banChar, 0, sizeof(banChar));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(banChar), 0);
			memcpy(&banChar, buffer, sizeof(banChar));

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, banChar.Name))
					{
						Err = true;

						TargetPlayer = i;
						i = Max_Connections;
					}
				}
			}

			accID = Clients[Player].Acc->getAccID();

			if (TargetPlayer > -1)
			{
				accID2 = Clients[TargetPlayer].Acc->getAccID();

				buffer[0] = 99;
				buffer[1] = 4;
				memcpy(&buffer2[2], &accID2, sizeof(accID2));
					
				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State < 2)
					{
						if (i != Player)
						{
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID2), 0);							
							Clients[i].KnowPlayer[TargetPlayer] = false;
						}
					}				
				}		

				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [Banned]: %s\n", Date, Time, Clients[Player].uChar->Name, Clients[TargetPlayer].uChar->Name);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

				GMACTION << Date <<  ", " << Time << " - " << Clients[Player].uChar->Name << " [Banned] " << Clients[TargetPlayer].uChar->Name << " from the server." << endl;
				
				ID_ = Client_GetID(Clients[TargetPlayer].Acc->getAccID() );
					Clients[TargetPlayer].Acc->Ban();
				Disconnect(ID_);
			}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;

		case 28: // Spawn some monsters

			PACKET_SPAWNMONSTERS Spawnit;
			recv(Clients[Player].ClientSocket, buffer, sizeof(Spawnit), 0);
			memcpy(&Spawnit, buffer, sizeof(Spawnit));

			if (Clients[Player].uChar->DevMode < 2) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}		

			for (i = 0; i < 3; i++)
			{
				for (int j = 0; j < Spawnit.AM[i]; j++)
				{
					if (Spawnit.AM[i] > -1)
					{
						SpawnMonster(Clients[Player].uChar->X, Clients[Player].uChar->Y, Spawnit.ID[i]);
					}
				}
			}

			break;

		case 30: // Give jail time

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			short amm;
			recv(Clients[Player].ClientSocket, buffer, 1, 0);
			amm = buffer[0];

			recv(Clients[Player].ClientSocket, buffer, sizeof(ID_), 0);

			memcpy(&ID_, buffer, sizeof(ID_));

			ID_ = Client_GetID( ID_ );
			if (ID_ == -1) break;

			buffer[0] = 99;
			buffer[1] = 8;
			send(Clients[ID_].ClientSocket, buffer, 2, 0);

			buffer[0] = 99;
			buffer[1] = 7;
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			Clients[ID_].uChar->CrimCount = amm;
			Clients[ID_].report_repupoints();

			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [Give %d counts]: %s\n", Date, Time, amm, Clients[ID_].uChar->Name);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
			//Clients[Player].logfile << Date << " " << Time << " [BAN]: Banned " << Clients[ID_].uChar->Name << endl;

			GMACTION << Clients[Player].uChar->Name << " gave " << amm << " counts to " << Clients[ID_].uChar->Name << ".\n";

			break;

		case 31: // Jail

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			TargetPlayer = -1;	

			PACKET_JAIL jailChar;
			memset(&jailChar, 0, sizeof(jailChar));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(jailChar), 0);
			memcpy(&jailChar, buffer, sizeof(jailChar));

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, jailChar.Name))
					{
						Err = true;

						TargetPlayer = i;
						i = Max_Connections;
					}
				}
			}


			if (TargetPlayer > -1)
			{
				ID_ = Client_GetID(Clients[TargetPlayer].Acc->getAccID() );

				JailPlayer(ID_);

				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [Jailed] %s\n", Date, Time, Clients[Player].uChar->Name, Clients[TargetPlayer].uChar->Name);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

				GMACTION << Clients[Player].uChar->Name << " jailed " << Clients[TargetPlayer].uChar->Name << ".\n";
			}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}


			break;

		case 32: // Release from Jail

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			TargetPlayer = -1;	

			PACKET_UNJAIL unJailChar;
			memset(&unJailChar, 0, sizeof(unJailChar));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(unJailChar), 0);
			memcpy(&unJailChar, buffer, sizeof(unJailChar));

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, unJailChar.Name))
					{
						Err = true;

						TargetPlayer = i;
						i = Max_Connections;
					}
				}
			}


			if (TargetPlayer > -1)
			{
				ID_ = Client_GetID(Clients[TargetPlayer].Acc->getAccID() );
	
				Clients[ID_].uChar->CrimCount = 0;
					ReleaseFromJail(ID_);
				Clients[ID_].LstJailtime = 0;

				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [UnJailed] %s\n", Date, Time, Clients[Player].uChar->Name, Clients[TargetPlayer].uChar->Name);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

				GMACTION << Clients[Player].uChar->Name << " UnJailed " << Clients[TargetPlayer].uChar->Name << ".\n";
			}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}


			break;

		case 35: // Whisper Player

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 1)
			{
				PACKET_WHISPERTO whisperto;
				memset(&whisperto, 0, sizeof(whisperto));
				recv(Clients[Player].ClientSocket, buffer, sizeof(whisperto), 0);
				memcpy(&whisperto, buffer, sizeof(whisperto));

				short pl;
				pl = -1;

                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (!strcmp(Clients[i].uChar->Name, whisperto.name))
						{
							pl = i;
							i = Max_Connections;
						}
					}
				}

				if (pl == -1)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
					break;
				}

				PACKET_CHATMESSAGE NewPacketWhisp;
				memset(&NewPacketWhisp, 0, sizeof(NewPacketWhisp));

				sprintf(NewPacket.Message, "¥***(%s: ¥%s)***", Clients[Player].uChar->Name, whisperto.msg);
				sprintf(NewPacketWhisp.Message, "¥***([%s] %s: ¥%s)***", Clients[pl].uChar->Name, Clients[Player].uChar->Name, whisperto.msg);

				NewPacket.MsgType = 11;
				NewPacketWhisp.MsgType = 11;

				buffer[0] = 9;
				buffer[1] = 0;
				memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));
				send(Clients[pl].ClientSocket, buffer, sizeof(NewPacket), 0);

				memset(&buffer[2], 0, sizeof(buffer[2]));
				
				buffer[0] = 9;
				buffer[1] = 0;
				memcpy(&buffer[2], &NewPacketWhisp, sizeof(NewPacketWhisp));
				send(Clients[Player].ClientSocket, buffer, sizeof(NewPacketWhisp), 0);
				
				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [Whisper] %s: %s\n", Date, Time, Clients[Player].uChar->Name, Clients[pl].uChar->Name, whisperto.msg);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
				printf(tmp);

				GMACTION << Clients[Player].uChar->Name << " Whisper " << Clients[pl].uChar->Name << ": " << whisperto.msg <<".\n";
			}

			break;

		case 36: // Follow

			if (Clients[Player].uChar->DevMode < 1) break;

			// Make sure we r not following anyone else
				for (int k = 0; k < Max_Connections; k++)
				{
					for (int x = 0; x < Max_Connections; x++)
					{
						if (Clients[k].beingfollowed[x] == Player)
						{
							Clients[k].beingfollowed[x] = -1;
						}
					}
				}

				Clients[Player].isfollowing = 0;

				PACKET_FOLLOW strFollow;
				memset(&strFollow, 0, sizeof(strFollow));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(strFollow), 0);
				memcpy(&strFollow, buffer, sizeof(strFollow));				
					memset(&NewCoordinates, 0, sizeof(NewCoordinates));

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{
						if (!strcmp(Clients[i].uChar->Name, strFollow.Name))
						{
							for (int x = 0; x < Max_Connections; x++)
							{
								if (Clients[i].TradeinProgress || Clients[i].BrowsingShop || Clients[i].BrowsingBank){
									buffer[0] = 99;
									buffer[1] = 62;
									send(Clients[Player].ClientSocket, buffer, 2, 0);

									break;
								}

								if (Clients[i].beingfollowed[x] == -1)
								{
									Clients[i].beingfollowed[x] = Player;
								}
							}

							Clients[Player].isfollowing = Clients[i].Acc->getAccID();

							PACKET_BACK_TELEPORTTOOTHER BackTo;
							memset(&BackTo, 0, sizeof(BackTo));

							Err = true;

							BackTo.X = Clients[i].uChar->X;
							BackTo.Y = Clients[i].uChar->Y;
							
							NewCoordinates.X = BackTo.X;
							NewCoordinates.Y = BackTo.Y;
							NewCoordinates.ID = Clients[i].Acc->getAccID();

							buffer[0] = 60;
							buffer[1] = 8;
							memcpy(&buffer[2], &BackTo, sizeof(BackTo));
							send(Clients[Player].ClientSocket, buffer, 2 + sizeof(BackTo), 0);

							// Turn player invisible 
							Clients[Player].Inv = true;
							buffer[0] = 60;
							buffer[1] = 2;
							buffer[2] = 1;
							send(Clients[Player].ClientSocket, buffer, 3, 0);

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [Follow]: %s\n", Date, Time, Clients[i].uChar->Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
							GMACTION << Clients[Player].uChar->Name << " started following " << Clients[i].uChar->Name << ".\n";

							CheckPlayersOnScreen(Player, NewCoordinates);
							CheckItemsOnScreen(Player);
							CheckMonstersOnScreen(Player);
							CheckNPCsOnScreen(Player);
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;
		case 37: // Stop Following

			if (Clients[Player].uChar->DevMode < 1) break;

			for (int k = 0; k < Max_Connections; k++)
			{
				for (int x = 0; x < Max_Connections; x++)
				{
					if (Clients[k].beingfollowed[x] == Player)
					{
						Clients[k].beingfollowed[x] = -1;
					}
				}
			}

			Clients[Player].isfollowing = 0;
	
			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [Stopped Following]\n", Date, Time);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
			GMACTION << Clients[Player].uChar->Name << " started following....\n";


			break;
		case 38: // Guild message

			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode > 1)
			{
				PACKET_GUILDMSGTO guildmsgto;
				memset(&guildmsgto, 0, sizeof(guildmsgto));
				recv(Clients[Player].ClientSocket, buffer, sizeof(guildmsgto), 0);
				memcpy(&guildmsgto, buffer, sizeof(guildmsgto));

				short pl;
				pl = -1;

                for (i = 0; i < MaxGuilds; i++)
				{
					if (!strcmp(Guilds[i].Tag, guildmsgto.tag))
					{
						printf("found guild %s\n", Guilds[i].Name);
						pl = i;
						i = MaxGuilds;
					}
				}

				if (pl == -1)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
					break;
				}

				PACKET_CHATMESSAGE nppacket;
				memset(&nppacket, 0, sizeof(nppacket));
				
				sprintf(NewPacket.Message, "æ[Guild] æ%s: ï%s", Clients[Player].uChar->Name, guildmsgto.msg);
				sprintf(nppacket.Message, "æ[%s] %s", Guilds[pl].Tag, NewPacket.Message);
				nppacket.MsgType = 3;

				NewPacket.MsgType = 3; 

				buffer[0] = 9;
				buffer[1] = 0;
				memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));

				buffer2[0] = 9;
				buffer2[1] = 0;
				memcpy(&buffer2[2], &nppacket, sizeof(nppacket));

				for (int i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (Clients[i].Acc->uChar->Guild == pl)
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
						if (Clients[i].ListenGuild)
							send(Clients[i].ClientSocket, buffer2, 2 + sizeof(nppacket), 0);
					}				
				}

//				send(Clients[Player].ClientSocket, buffer, sizeof(NewPacket), 0);
			}

			break;
		case 40:

			Clients[Player].uChar->CrimCount = -1;

			break;

		case 41:  // Name change

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

				PACKET_NAMECHANGE nName;
				memset(&nName, 0, sizeof(nName));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(nName), 0);
				memcpy(&nName, buffer, sizeof(nName));

				bool charExists;
				charExists = false;

				if(AccManager->CharNameTaken(nName.name2))
					charExists = true;
	
				if (charExists)
				{
					buffer[0] = 99;
					buffer[1] = 30;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
					break;
				}

                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (!strcmp(Clients[i].uChar->Name, nName.name1))
						{
							strcpy(Clients[i].uChar->Name, nName.name2);
	
							buffer[0] = 99;
							buffer[1] = 31;
							send(Clients[Player].ClientSocket, buffer, 2, 0);

							buffer[1] = 3;
							send(Clients[i].ClientSocket, buffer, 2, 0);

							i = Max_Connections;
						}
					}
				}

			break;

		case 42: // Kill Player

			if (Clients[Player].uChar->DevMode < 2) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			PACKET_KILL gmKill;
			memset(&gmKill, 0, sizeof(gmKill));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(gmKill), 0);
				memcpy(&gmKill, buffer, sizeof(gmKill));		

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used)
				{
					if (!strcmp(Clients[i].uChar->Name, gmKill.Name))
					{
						// Send new hit pic to players
						
						NewPic.Pic = 0;
						NewPic.X = Clients[i].uChar->X;
						NewPic.Y = Clients[i].uChar->Y;
						
						buffer[0] = 21;
						buffer[1] = 1;
						memcpy(&buffer[2], &NewPic, sizeof(NewPic));

						for (j = 0; j < Max_Connections; j++)
						{
							if (Clients[j].Used && Clients[j].State != 5)
							{
								ox = Clients[j].uChar->X;
								oy = Clients[j].uChar->Y;
					
								if (IsInRange(ox, oy, tx, ty))
								{
									send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
								}	
							}
						}

						// Send combat text to target player

						memset(&NewCT, 0, sizeof(NewCT));

						buffer[0] = 97;
						buffer[1] = 1;

						NewCT.Amount = Clients[i].Acc->Calc_hpMax();
						NewCT.Type = 25;
						NewCT.FromTo = Clients[Player].Acc->getAccID();
						NewCT.SkillType = 1;

						memcpy(&buffer[2], &NewCT, sizeof(NewCT));
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCT), 0);
						memset(&NewCT, 0, sizeof(NewCT));

						buffer[0] = 97;
						buffer[1] = 1;

						NewCT.Amount = Clients[i].Acc->Calc_hpMax();
						NewCT.Type = 24;
						NewCT.FromTo = Clients[i].Acc->getAccID();
						NewCT.SkillType = 1;

						memcpy(&buffer[2], &NewCT, sizeof(NewCT));

						send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

						// Reduce player's HP

						Clients[i].uChar->HPLeft -= Clients[i].Acc->Calc_hpMax();

						if ( Clients[i].uChar->HPLeft <= 0)
						{
							// Player died.

							Clients[i].uChar->HPLeft = Clients[i].Acc->Calc_hpMax() * 0.75;

							if (Clients[i].uChar->Wanted)//1435,4699
							{
								Clients[i].CrimDeath();
							}

							buffer[0] = 11;
							buffer[1] = 1;

							short oldx, oldy;

							short newx, newy;

							oldx = Clients[i].uChar->X;
							oldy = Clients[i].uChar->Y;


							newx = Clients[i].uChar->ShrineX;
							newy = Clients[i].uChar->ShrineY;

							Clients[i].uChar->X = newx;
							Clients[i].uChar->Y = newy;

							CreateBody(i, oldx, oldy);

							Clients[i].uChar->Wanted = false;

							Clients[i].PLTarget = -1;

							PACKET_COORDINATES NewCoords;
							memset(&NewCoords, 0, sizeof(NewCoords));
							NewCoords.X = newx;
							NewCoords.Y = newy;
							NewCoords.ID = Clients[i].Acc->getAccID();

							buffer[0] = 10;
							buffer[1] = 1;
							memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
							send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

							CheckPlayersOnScreen(i, NewCoords);
							CheckItemsOnScreen(i);
							CheckMonstersOnScreen(i);
							CheckNPCsOnScreen(i);
						}

						// Inform players of new HP

						memset(&NewHP, 0, sizeof(NewHP));

						NewHP.KnowID = Clients[i].Acc->getAccID();
						NewHP.NewHP = Clients[i].uChar->HPLeft;
						NewHP.Type = 0;

						buffer[0] = 20;
						buffer[1] = 3;

						memcpy(&buffer[2], &NewHP, sizeof(NewHP));

						ox = Clients[i].uChar->X;
						oy = Clients[i].uChar->Y;

						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewHP), 0);

						for (int q = 0; q < Max_Connections; q++)
						{
							if (Clients[q].Used && Clients[q].State != 5)
							{
								tx = Clients[q].uChar->X;
								ty = Clients[q].uChar->Y;
								if (IsInRange(ox, oy, tx, ty))
								{
									if (Clients[q].KnowPlayer[Clients[i].Know_ID] || q == i)
										send(Clients[q].ClientSocket, buffer, 2 + sizeof( NewHP), 0);			
								}
							}
						}

						Err = true;

						memset(&tmp, 0, sizeof(tmp));
						sprintf(tmp, "%s %s: %s: [killed] %s\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
						Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

						GMLOG2 << Date <<  ", " << Time << " - " << Clients[Player].uChar->Name << " [killed] " << Clients[i].uChar->Name << " ." << endl;
					}
				}
			}

			if (!Err)
			{
				buffer[0] = 99;
				buffer[1] = 60;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
			}

			break;

		case 43: // Heal Player
	
			if (Clients[Player].uChar->DevMode < 2) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

			PACKET_HEAL gmHeal;
			memset(&gmHeal, 0, sizeof(gmHeal));
	
			recv(Clients[Player].ClientSocket, buffer, sizeof(gmHeal), 0);
				memcpy(&gmHeal, buffer, sizeof(gmHeal));		

			Err = false;
			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used)
				{
					if (!strcmp(Clients[i].uChar->Name, gmHeal.Name))
					{
						// Send combat text to target player

						memset(&NewCT, 0, sizeof(NewCT));

						buffer[0] = 97;
						buffer[1] = 1;

						NewCT.Amount = Clients[i].Acc->Calc_hpMax();
						NewCT.Type = 23;
						NewCT.FromTo = Clients[i].Acc->getAccID();
						NewCT.SkillType = 1;

						memcpy(&buffer[2], &NewCT, sizeof(NewCT));

						send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

						memset(&NewCT, 0, sizeof(NewCT));

						buffer[0] = 97;
						buffer[1] = 1;

						NewCT.Amount = Clients[i].Acc->Calc_hpMax();
						NewCT.Type = 26;
						NewCT.FromTo = Clients[Player].Acc->getAccID();
						NewCT.SkillType = 1;

						memcpy(&buffer[2], &NewCT, sizeof(NewCT));

						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

						// Heal players HP

						Clients[i].uChar->HPLeft = Clients[i].Acc->Calc_hpMax();

						// Inform players of new HP

						memset(&NewHP, 0, sizeof(NewHP));

						NewHP.KnowID = Clients[i].Acc->getAccID();
						NewHP.NewHP = Clients[i].uChar->HPLeft;
						NewHP.Type = 0;

						buffer[0] = 20;
						buffer[1] = 3;

						memcpy(&buffer[2], &NewHP, sizeof(NewHP));

						ox = Clients[i].uChar->X;
						oy = Clients[i].uChar->Y;

						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewHP), 0);

						for (int q = 0; q < Max_Connections; q++)
						{
							if (Clients[q].Used && Clients[q].State != 5)
							{
								tx = Clients[q].uChar->X;
								ty = Clients[q].uChar->Y;
								if (IsInRange(ox, oy, tx, ty))
								{
									if (Clients[q].KnowPlayer[Clients[i].Know_ID] || q == i)
										send(Clients[q].ClientSocket, buffer, 2 + sizeof( NewHP), 0);			
								}
							}
						}

						Err = true;

						memset(&tmp, 0, sizeof(tmp));
						sprintf(tmp, "%s %s: %s: [Healed] %s\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
						Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

						GMLOG2 << Date <<  ", " << Time << " - " << Clients[Player].uChar->Name << " [Healed] " << Clients[i].uChar->Name << " ." << endl;
					}
				}
			}

			if (!Err)
			{
				buffer[0] = 99;
				buffer[1] = 60;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
			}

			break;

		case 44: // set players map black

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

				PACKET_REESETMAP mapSet;
				memset(&mapSet, 0, sizeof(mapSet));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(mapSet), 0);
				memcpy(&mapSet, buffer, sizeof(mapSet));				

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{

						if (!strcmp(Clients[i].uChar->Name, mapSet.Name))
						{
							Err = true;

								buffer[0] = 64;
								buffer[1] = 1;	
								send(Clients[i].ClientSocket, buffer, 2, 0);

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [%s Reset %s's map]\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							GMACTION << Clients[Player].uChar->Name << " Reset %s's map" << Clients[i].uChar->Name << ".\n";
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;

		case 45: // set players map to cleared

			if (Clients[Player].uChar->DevMode < 3) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

				PACKET_CLEARMAP mapClear;
				memset(&mapClear, 0, sizeof(mapClear));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(mapClear), 0);
				memcpy(&mapClear, buffer, sizeof(mapClear));				

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{

						if (!strcmp(Clients[i].uChar->Name, mapClear.Name))
						{
							Err = true;

								buffer[0] = 64;
								buffer[1] = 2;	
								send(Clients[i].ClientSocket, buffer, 2, 0);

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [%s Cleared %s's map]\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							GMACTION << Clients[Player].uChar->Name << " Cleared %s's map" << Clients[i].uChar->Name << ".\n";
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;

		case 46: // Mute Player

			if (Clients[Player].uChar->DevMode < 1) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

				PACKET_MUTE muteName;
				memset(&muteName, 0, sizeof(muteName));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(muteName), 0);
				memcpy(&muteName, buffer, sizeof(muteName));				

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{

						if (!strcmp(Clients[i].uChar->Name, muteName.Name))
						{
							Err = true;
							Clients[i].is_mute = true;

								buffer[0] = 99;
								buffer[1] = 93;	
								send(Clients[i].ClientSocket, buffer, 2, 0);

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [%s Muted %s]\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							GMACTION << Clients[Player].uChar->Name << " Muted %s" << Clients[i].uChar->Name << ".\n";
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;

		case 47: // Unmute Player

			if (Clients[Player].uChar->DevMode < 1) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

				PACKET_UNMUTE unmuteName;
				memset(&unmuteName, 0, sizeof(unmuteName));
	
				recv(Clients[Player].ClientSocket, buffer, sizeof(unmuteName), 0);
				memcpy(&unmuteName, buffer, sizeof(unmuteName));				

				Err = false;
                for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used)
					{

						if (!strcmp(Clients[i].uChar->Name, unmuteName.Name))
						{
							Err = true;
							Clients[i].is_mute = false;

								buffer[0] = 99;
								buffer[1] = 95;	
								send(Clients[i].ClientSocket, buffer, 2, 0);

							memset(&tmp, 0, sizeof(tmp));
							sprintf(tmp, "%s %s: [%s Unmuted %s]\n", Date, Time, Clients[Player].uChar->Name, Clients[i].uChar->Name);
							Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();

							GMACTION << Clients[Player].uChar->Name << " Unmuted %s" << Clients[i].uChar->Name << ".\n";
						}
					}
				}

				if (!Err)
				{
					buffer[0] = 99;
					buffer[1] = 60;
					send(Clients[Player].ClientSocket, buffer, 2, 0);
				}

			break;

		case 50: // PM
			
			PACKET_WHISPERTO whisperto;
			memset(&whisperto, 0, sizeof(whisperto));
			recv(Clients[Player].ClientSocket, buffer, sizeof(whisperto), 0);
			memcpy(&whisperto, buffer, sizeof(whisperto));

			short pl;
			pl = -1;

               for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					if (!strcmp(Clients[i].uChar->Name, whisperto.name))
					{
						pl = i;
						i = Max_Connections;
					}
				}
			}

			if (pl == -1)
			{
				buffer[0] = 99;
				buffer[1] = 60;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

			if (!strcmp(Clients[Player].uChar->Name, whisperto.name)) // Can't PM yourself!
			{
				buffer[0] = 99;
				buffer[1] = 77;
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}

			PACKET_CHATMESSAGE NewPacketWhisp;
			memset(&NewPacketWhisp, 0, sizeof(NewPacketWhisp));

			sprintf(NewPacket.Message, "¥[PM] %s: ¥%s", Clients[Player].uChar->Name, whisperto.msg);
			sprintf(NewPacketWhisp.Message, "¥[PM] %s: ¥%s", Clients[pl].uChar->Name,whisperto.msg);

			NewPacket.MsgType = 11;
			NewPacketWhisp.MsgType = 11;

			buffer[0] = 9;
			buffer[1] = 0;
			memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));
			send(Clients[pl].ClientSocket, buffer, sizeof(NewPacket), 0);

			memset(&buffer[2], 0, sizeof(buffer[2]));
				
			buffer[0] = 9;
			buffer[1] = 0;
			memcpy(&buffer[2], &NewPacketWhisp, sizeof(NewPacketWhisp));
			send(Clients[Player].ClientSocket, buffer, sizeof(NewPacketWhisp), 0);
				
				memset(&tmp, 0, sizeof(tmp));
				sprintf(tmp, "%s %s: %s [PM] %s: %s\n", Date, Time, Clients[Player].uChar->Name, Clients[pl].uChar->Name, whisperto.msg);
				Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
				printf(tmp);

			break;

		case 90: // Force server save

			memset(&tmp, 0, sizeof(tmp));
			sprintf(tmp, "%s %s: [FORCE SRVR SAVE]\n", Date, Time);
			Clients[Player].logfile->write(tmp, strlen(tmp)); Clients[Player].logfile->flush();
			//Clients[Player].logfile << Date << " " << Time << " [FORCED]: Server save." << endl;

			AccManager->SaveAccounts();
			saveguilds();
			SaveItems();

			break;

		case 99: //Reset Stats
			if (Clients[Player].GMMode && Clients[Player].uChar->DevMode < 5) 
			{
				buffer[0] = 99;
				buffer[1] = 16;	
				send(Clients[Player].ClientSocket, buffer, 2, 0);
				break;
			}	

				PACKET_RESETSTATS ResetStats;
				memset(&ResetStats, 0, sizeof(ResetStats));
		
				recv(Clients[Player].ClientSocket, buffer, sizeof(ResetStats), 0);
				memcpy(&ResetStats, buffer, sizeof(ResetStats));

				AccManager->ResetStats(ResetStats.Name, ResetStats.Slot);

			break;
			
		}
		break;

	case 98: // Timeout & Timein

		Clients[Player].LstMsg = GetTickCount64();

		buffer[0] = 98;
		buffer[1] = 0;
		if (Clients[Player].Used && Clients[Player].State != 5)
		send(Clients[Player].ClientSocket, buffer, 2, 0);

		break;

/*
	case 125: // Bunnkia Tools To-Do

		recv(Clients[Player].ClientSocket, buffer, 1, 0);

		if (!Clients[Player].authed && buffer[0] != 11 && buffer[0] != 2) Disconnect(Player);

		switch (buffer[0])
		{
		case 1: // Request some account

			recv(Clients[Player].ClientSocket, buffer, 1, 0);

			switch (buffer[0])
			{
			case 1: // Request account by accountname
	
				char Stringi[25];
				memset(&Stringi, 0, sizeof(Stringi));
			

				recv(Clients[Player].ClientSocket, buffer, sizeof(Stringi), 0);

				memcpy(&Stringi, buffer, sizeof(Stringi));

				printf("Server tools from IP [%s] requested account [%s] info.\n", ClientIp, Stringi);

				Account *acc;
				acc = AccManager->GetByName(Stringi);
				if (acc == NULL) 
				{
					printf("Failed, account couldn't be found.\n");
					break;
				}
				else printf("Succeed, info sent.\n");

				AccountData *tmpacc;


				tmpacc = acc->GetAccPointer();

				buffer[0] = 2;				
				accToEdit.IsPremium			= tmpacc->IsPremium;
				accToEdit.IsGolden			= tmpacc->IsGolden;
				accToEdit.ID				= acc->getAccID();
				accToEdit.IsBanned			= tmpacc->IsBanned;
				strcpy(accToEdit.AccName,	tmpacc->AccName);
				strcpy(accToEdit.AccPass,	tmpacc->AccPass);
				CharacterData *dChar[2];
				dChar[0] = acc->GetCharPointer(0);
				dChar[1] = acc->GetCharPointer(1);

				if (dChar[0]->Used)
					strcpy(accToEdit.CharNames[0], dChar[0]->Name);

				if (dChar[1]->Used)
					strcpy(accToEdit.CharNames[1], dChar[1]->Name);
				
				memcpy(&buffer[1], &accToEdit, sizeof(accToEdit));
				send(Clients[Player].ClientSocket, buffer, 1 + sizeof(accToEdit), 0);

				break;
			case 2: // Request account by char name

				break;
			case 3: // Request account by ID
	
				break;
			case 4: // Requesting character

				memset(&Stringi, 0, sizeof(Stringi));
			

				recv(Clients[Player].ClientSocket, buffer, sizeof(Stringi), 0);

				memcpy(&Stringi, buffer, sizeof(Stringi));

				printf("Server tools from IP [%s] requested character [%s] info.\n", ClientIp, Stringi);


				acc = AccManager->GetByCharName(Stringi);
				if (acc == NULL) break;

				charToEdit.charslot = -1;

				CharacterData *uChar;
				uChar = acc->GetCharPointer(0);
				if (uChar != NULL)
					if (!strcmp(uChar->Name, Stringi))
						charToEdit.charslot = 0;
				uChar = acc->GetCharPointer(1);
				if (uChar != NULL)
					if (!strcmp(uChar->Name, Stringi))
						charToEdit.charslot = 1;

				if (charToEdit.charslot == -1) break;

				charToEdit.accid		= acc->getAccID();
				charToEdit.Avatar		= uChar->Avatar;
				charToEdit.Cha			= uChar->Cha;
				charToEdit.Con			= uChar->Con;
				charToEdit.CrimCount	= uChar->CrimCount;
				charToEdit.DevMode		= uChar->DevMode;
				charToEdit.Dex			= uChar->Dex;
				charToEdit.Exp			= uChar->Exp;
				charToEdit.HPLeft		= uChar->HPLeft;
				charToEdit.Int			= uChar->Int;
				charToEdit.KilledMonsters = uChar->KilledMonsters;
				charToEdit.Lck			= uChar->Lck;
				charToEdit.Level		= uChar->Level;
				charToEdit.Reputation	= uChar->Reputation;
				charToEdit.ShrineX		= uChar->ShrineX;
				charToEdit.ShrineY		= uChar->ShrineY;
				charToEdit.StatPoints	= uChar->StatPoints;
				charToEdit.Str			= uChar->Str;
				charToEdit.Wanted		= uChar->Wanted;
				charToEdit.X			= uChar->X;
				charToEdit.Y			= uChar->Y;
				strcpy(charToEdit.Name, uChar->Name);
				memcpy(&charToEdit.Skills, &uChar->Skills, sizeof(uChar->Skills));

				buffer4[0] = 4;
				memcpy(&buffer4[1], &charToEdit, sizeof(charToEdit));
				send(Clients[Player].ClientSocket, buffer4, 1 + sizeof(charToEdit), 0);


				break;
			case 5:

				recv(Clients[Player].ClientSocket, buffer4, sizeof(charToEdit), 0);

				memcpy(&charToEdit, buffer4, sizeof(charToEdit));

				acc = AccManager->GetByID(charToEdit.accid);
				if (acc == NULL) break;

				uChar = acc->GetCharPointer(charToEdit.charslot);
				if (uChar == NULL) break;

				uChar->Avatar			= charToEdit.Avatar; 
				uChar->Cha				= charToEdit.Cha; 
				uChar->Con				= charToEdit.Con; 
				uChar->CrimCount		= charToEdit.CrimCount; 
				uChar->DevMode			= charToEdit.DevMode; 
				uChar->Dex				= charToEdit.Dex; 
				uChar->Exp				= charToEdit.Exp; 
				uChar->HPLeft			= charToEdit.HPLeft;
				uChar->Int				= charToEdit.Int;
				uChar->KilledMonsters	= charToEdit.KilledMonsters; 
				uChar->Lck				= charToEdit.Lck; 
				uChar->Level			= charToEdit.Level; 
				uChar->Reputation		= charToEdit.Reputation; 
				uChar->ShrineX			= charToEdit.ShrineX; 
				uChar->ShrineY			= charToEdit.ShrineY; 
				uChar->StatPoints		= charToEdit.StatPoints; 
				uChar->Str				= charToEdit.Str; 
				uChar->Wanted			= charToEdit.Wanted; 
				uChar->X				= charToEdit.X; 
				uChar->Y				= charToEdit.Y; 
				strcpy(uChar->Name,		charToEdit.Name);
				memcpy(&uChar->Skills, &charToEdit.Skills, sizeof(charToEdit.Skills));

				break;
			case 6: // account info by charname

				memset(&Stringi, 0, sizeof(Stringi));
			

				recv(Clients[Player].ClientSocket, buffer, sizeof(Stringi), 0);

				memcpy(&Stringi, buffer, sizeof(Stringi));

				printf("Server tools from IP [%s] requested account [%s] info.\n", ClientIp, Stringi);

				short accnum = -1;
				short charnum = -1;

				for (i = 0; i < Max_Connections; i++)
				{
					if (!strcmp(Accounts[i].Char[0].Name, Stringi))
					{
						accnum = i;
						charnum = 0;
						i = m_Accounts;
					}
					if (!strcmp(Accounts[i].Char[1].Name, Stringi))
					{
						accnum = i;
						charnum = 1;
						i = m_Accounts;
					}
				}

				if (accnum == -1 || charnum == -1) break;

				buffer[0] = 6;
				buffer[1] = strlen(Accounts[accnum].AccName);
				buffer[2] = strlen(Accounts[accnum].Char[0].Name);
				buffer[3] = strlen(Accounts[accnum].Char[1].Name);
				memcpy(&buffer[4], &Accounts[accnum].AccName, buffer[1]);
				memcpy(&buffer[4 + buffer[1]], &Accounts[accnum].Char[0].Name, buffer[2]);
				memcpy(&buffer[4 + buffer[1] + buffer[2]], &Accounts[accnum].Char[1].Name, buffer[3]);
				send(Clients[Player].ClientSocket, buffer, 4 + buffer[1] + buffer[2] + buffer[3], 0);

				break;
			}

			break;

		case 2: // Inform server that timeout won't work on this "Client"

			Clients[Player].TimeoutNo = true;
			printf("Client %d was set to TimeoutNo\n", Player);

			break;

		case 3: // Inform server that timeout will again work on this "Client"

			Clients[Player].TimeoutNo = false;
			printf("Client %d's TimeoutNo flag was removed\n", Player);

			break;
		case 4: // Update account on the server

			memset(&accToEdit, 0, sizeof(accToEdit));

			recv(Clients[Player].ClientSocket, buffer, sizeof(accToEdit), 0);
			memcpy(&accToEdit, buffer, sizeof(accToEdit));


			Account *acc;
			acc = AccManager->GetByID(accToEdit.ID);

			if (acc == NULL) break;

			AccountData *data;
			data = acc->GetAccPointer();

			data->IsPremium			= accToEdit.IsPremium;
			data->IsGolden			= accToEdit.IsGolden;
			data->IsBanned			= accToEdit.IsBanned;
			strcpy(data->AccName,	accToEdit.AccName);
			strcpy(data->AccPass,	accToEdit.AccPass);

			printf("Bunnkia Tools: Account %s updated\n", accToEdit.AccName);

			break;

		case 5: // Ping pong reply

			printf("Ping? Pong! Reply from Server tools, using client %d\n", Player);
            Clients[Player].LstMsg = GetTickCount64();

			break;
		case 6: // Kick all from server

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used) Disconnect(i);
			}

			break;
		case 7: // Kick all Server tools from the server

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].TimeoutNo) Disconnect(i);
			}

			break;
		case 8: // Force save

			AccManager->SaveAccounts();
			
			Autosavetime = GetTickCount64();

			break;

		case 9:

			for (i = 0; i < MaxBackpackItems; i++)
			{
				if (Clients[Player].uChar->Backpack[i] != 0)
				{
					memset(&Items[Clients[Player].uChar->Backpack[i]], 0, sizeof(Items[Clients[Player].uChar->Backpack[i]]));
					Clients[Player].uChar->Backpack[i] = 0;
				}
			}
			for (i = 0; i < MaxBankItems; i++)
			{
				if (Clients[Player].uChar->Bank[i] != 0)
				{
					memset(&Items[Clients[Player].uChar->Bank[i]], 0, sizeof(Items[Clients[Player].uChar->Bank[i]]));
					Clients[Player].uChar->Bank[i] = 0;
				}
			}
			for (i = 0; i < MaxWornItems; i++)
			{
				if (Clients[Player].uChar->Worn[i] != 0)
				{
					memset(&Items[Clients[Player].uChar->Backpack[i]], 0, sizeof(Items[Clients[Player].uChar->Worn[i]]));
					Clients[Player].uChar->Worn[i] = 0;
				}
			}
			
			break;

		case 10: // server message from tools
			
//			recv(Clients[Player].ClientSocket, buffer, sizeof(message), 0);
			NewPacket.MsgType = 10;
			memset(&NewPacket.Message, 0, sizeof(NewPacket.Message));
			memcpy(&NewPacket.Message, buffer, sizeof(message));

			buffer[0] = 9;
			buffer[1] = 0;

			for (i = 0; i < Max_Connections; i++)
			{
				if (Clients[i].Used && Clients[i].State != 5)
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
				}
			}

			normalchat << Time << "[Srvr msg]: " << NewMsg << endl;

			printf("%s: [Server message]: %s\n", Time, NewPacket.Message);

			break;

		case 11: // auth

			char name[20], pass[20];
			memset(&name, 0, sizeof(name));
			memset(&pass, 0, sizeof(pass));

			recv(Clients[Player].ClientSocket, buffer, sizeof(name), 0);
			memcpy(&name, buffer, sizeof(name));
			recv(Clients[Player].ClientSocket, buffer, sizeof(pass), 0);
			memcpy(&pass, buffer, sizeof(pass));

			printf("Bunnkia Tools trying to auth\n");

			printf("%s, %s\n", name, pass);
			
			for (i = 0 ; i < 3; i++)
			{
				if (!strcmp(auth[i].name, name))
				{
					if (!strcmp(auth[i].pass, pass))
					{
						printf("Bunnkia Tools authed from ip %s\n", ClientIp);
						printf("User: %s, password: %s\n", name, pass);
						Clients[Player].authed = true;
						buffer[0] = 3;
						send(Clients[Player].ClientSocket, buffer, 1, 0);
						i = 3;
					}
				}
			}



			break;
		case 12: // search item

			short type, id;

			recv(Clients[Player].ClientSocket, buffer, sizeof(type), 0);
			memcpy(&type, buffer, sizeof(type));

			recv(Clients[Player].ClientSocket, buffer, sizeof(id), 0);
			memcpy(&id, buffer, sizeof(id));

			buffer[0] = 5;
			buffer[1] = 1;
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			for (i = 0; i < m_Accounts; i++)
			{
				if (Accounts[i].ID > 0)
				{
					for (int j = 0; j < 2; j++)
					{
						if (Accounts[i].Char[j].Used)
						{
							for (int k = 0; k < MaxBankItems; k++)
							{
								if (Accounts[i].Char[j].Bank[k] > 0)
								{
									if (Items[Accounts[i].Char[j].Bank[k]].Type == type &&
										Items[Accounts[i].Char[j].Bank[k]].ID == id &&
										Items[Accounts[i].Char[j].Bank[k]].Used)
									{
										buffer[0] = 5;
										buffer[1] = 3;
										buffer[2] = 1;
										memcpy(&buffer[3], &Accounts[i].Char[j].Name, sizeof(Accounts[i].Char[j].Name));
										memcpy(&buffer[3 + sizeof(Accounts[i].Char[j].Name)], &Accounts[i].AccName, sizeof(Accounts[i].AccName));
										send(Clients[Player].ClientSocket, buffer, 3 + sizeof(Accounts[i].Char[j].Name) + sizeof(Accounts[i].AccName), 0);								
									}								
								}							
							}
							for (k = 0; k < MaxBackpackItems; k++)
							{
								if (Accounts[i].Char[j].Backpack[k] > 0)
								{
									if (Items[Accounts[i].Char[j].Backpack[k]].Type == type &&
										Items[Accounts[i].Char[j].Backpack[k]].ID == id &&
										Items[Accounts[i].Char[j].Backpack[k]].Used)
									{
										buffer[0] = 5;
										buffer[1] = 3;
										buffer[2] = 2;
										memcpy(&buffer[3], &Accounts[i].Char[j].Name, sizeof(Accounts[i].Char[j].Name));
										memcpy(&buffer[3 + sizeof(Accounts[i].Char[j].Name)], &Accounts[i].AccName, sizeof(Accounts[i].AccName));
										send(Clients[Player].ClientSocket, buffer, 3 + sizeof(Accounts[i].Char[j].Name) + sizeof(Accounts[i].AccName), 0);								
									}								
								}							
							}
							for (k = 0; k < MaxWornItems; k++)
							{
								if (Accounts[i].Char[j].Worn[k] > 0)
								{
									if (Items[Accounts[i].Char[j].Worn[k]].Type == type &&
										Items[Accounts[i].Char[j].Worn[k]].ID == id &&
										Items[Accounts[i].Char[j].Worn[k]].Used)
									{
										buffer[0] = 5;
										buffer[1] = 3;
										buffer[2] = 3;
										memcpy(&buffer[3], &Accounts[i].Char[j].Name, sizeof(Accounts[i].Char[j].Name));
										memcpy(&buffer[3 + sizeof(Accounts[i].Char[j].Name)], &Accounts[i].AccName, sizeof(Accounts[i].AccName));
										send(Clients[Player].ClientSocket, buffer, 3 + sizeof(Accounts[i].Char[j].Name) + sizeof(Accounts[i].AccName), 0);								
									}								
								}							
							}
						}
					}
				}
			}

			buffer[0] = 5;
			buffer[1] = 2;
			send(Clients[Player].ClientSocket, buffer, 2, 0);

			break;
		
		}

		break;
*/
/////////////////////////
//		DISCONNECT?
////////////////////////
	case 120: // Disconnect

		//-----------------------------------------
		// Inform other players about this timeout
		//-----------------------------------------

		unsigned long accID;
		accID = Clients[Player].Acc->getAccID();

		Clients[Player].is_logging = true; // Set player logout toggle
		
		recv(Clients[Player].ClientSocket, buffer, 2, 0); 

		if (buffer[0] == 1) // Check for safe zone
			Clients[Player].LstLogTime = GetTickCount64() + 11000; // Set the logout timer
		else			
			Clients[Player].LstLogTime = GetTickCount64(); // Set the logout timer
		
		if (Clients[Player].uChar->DevMode > 1) // No logout timer for GMs
			Clients[Player].LstLogTime = GetTickCount64() + 11000; // Set the logout timer	


		if (Clients[Player].tradeskill_inuse == false) // Make sure they are not tradeskilling
		{
			if (Clients[Player].is_logging)
			{
				if (GetTickCount64() - Clients[Player].LstLogTime >= 10000) // Check if timer is 10 seconds or more
				{
					buffer[0] = 11;
					buffer[1] = 1;
					memcpy(&buffer[2], &accID, sizeof(accID));

					for (i = 0; i < Max_Connections; i++)
					{
						if (Clients[i].Used && Clients[i].State != 5)
						{
							if (i != Player)
							{
								send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);							
							}
						}

						Clients[i].KnowPlayer[Player] = false;
					}

					Disconnect( Player);
				}
			} else {
				Clients[Player].LstLogTime = GetTickCount64();
			}
		}

		break;
	default:

		//Disconnect(s);
		printf("This Client Has Disconnected: %s\n", ClientIp);

		return;

	break;
	}

	return;
}

bool IsInRange(short int plx, short int ply, short int tmpx, short int tmpy)
{
	if ( plx - tmpx >= -11 && plx - tmpx <= 11 &&
		 ply - tmpy >= -11 && ply - tmpy <= 11)
		 return true;
	return false;
}

bool IsInRange2(short int plx, short int ply, short int tmpx, short int tmpy)
{
	if ( plx - tmpx <= -12 || plx - tmpx >= 12 ||
		 ply - tmpy <= -12 || ply - tmpy >= 12)
		 return true;
	return false;
}


bool IsValid(short int tx2, short int ty2)
{
	if ( Tiles[ Map.Chunks[0][ Map.ChunkMap[ tx2 >> 4 ][ ty2 >> 4 ] ][ tx2 % 16 ][ ty2 % 16]].Block == 1) //grass layer
		return false;
	if ( Map.Chunks[1][ Map.ChunkMap[ tx2 >> 4 ][ ty2 >> 4 ] ][ tx2 % 16 ][ ty2 % 16] != 0 && Tiles[ Map.Chunks[1][ Map.ChunkMap[ tx2 >> 4 ][ ty2 >> 4 ] ][ tx2 % 16 ][ ty2 % 16]].Block == 1) //objects
		return false;

	return true;
}


// Check if there's message coming from some client.
void Check_Message()
{
	if (ClientsConnected == 0) return;

	fd_set input_set, exc_set;
	int s, nfds;

	timeout.tv_sec=0;
	timeout.tv_usec=0;
	
    FD_ZERO(&input_set);
    FD_ZERO(&exc_set);
    nfds = 0;

	int i;
  for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used)
		{
			FD_SET(Clients[i].ClientSocket, &input_set);
			FD_SET(Clients[i].ClientSocket, &exc_set);
			nfds++;
		}
	}

	s = select(nfds+1,&input_set,NULL,&exc_set,&timeout);

	if (s > 0)
	{
		for( i = 0; i < Max_Connections; i++)
		{
			if (FD_ISSET(Clients[i].ClientSocket, &exc_set))
			{
				Disconnect( i );
			}
			if (FD_ISSET(Clients[i].ClientSocket, &input_set))
			{
				Get_Message( i );
			}
		}
	}
}


//---------------------
// Get free "Know ID".
//---------------------
// This is obsolete, I used this like year ago, I think....
short int Player_GetFreeID()
{
	int i1;
	for (i1 = 0; i1 < Max_Connections; i1++)
	{
		if (!UsedPlayerID[i1])
		{
			UsedPlayerID[i1] = true;
			return i1;
		}	
	}
	printf("Player_GetFreeID() failed, no free IDs left.\n");
	return -1;
}

//----------------------
// Free used "Know ID".
//-----------------------
// As well as this.
void Player_FreeID(short int This)
{
	UsedPlayerID[This] = false;
}

// Obsolete, I think. It's now checked in Clients[].Acc->InUse()
bool IsAccountAlreadyInUse(short AccID)
{
	int Count = 0;

	int i;
	for (i = 1; i < Max_Connections; i++)
	{
		if (Clients[i].Acc->getAccID() == AccID && Clients[i].Used)
			Count++;
	}
    if (Count > 1) return true;
	else return false;
}


long GetFreeStaticAccountID()
{
	LastStaticID++;
	return LastStaticID;
}

void SaveItems()
{
	buffer[0] = 99;
	buffer[1] = 99;
	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used)
		{
			Clients[i].LstMsg = GetTickCount64();
			send(Clients[i].ClientSocket, buffer, 2, 0);
		}
	}

	printf("Begin save...\n");

	fstream filet;

	filet.open("items.vsf", ios::out | ios::binary);

	if (filet.is_open())
	{
		filet.write((char *)&LastTakenID, sizeof(LastTakenID));
		
		for (i = 0; i < m_Items; i++)
		{
			filet.write((char *)&Items[i], sizeof(Items[i]));
		}
		filet.close();
	}
	

	filet.open("ground.vsf", ios::out | ios::binary);
	
	if (filet.is_open())
	{
		for (i = 0; i < 256; i++)		
		{
			filet.write((char *)&Ground[i], sizeof(Ground[i]));
		}
		filet.close();
	}

	filet.open("bodies.vsf", ios::out | ios::binary);
	
	if (filet.is_open())
	{
		for (i = 0; i < m_Bodies; i++)		
		{
			filet.write((char *)&Body[i], sizeof(Body[i]));
		}
		filet.close();
	}

	buffer[0] = 99;
	buffer[1] = 100;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used)
		{
			Clients[i].LstMsg = GetTickCount64();
			send(Clients[i].ClientSocket, buffer, 2, 0);
		}
	}
}

// Created this function to obsolete "Bank hack", which was invented by Wipe.
// Wipe just sent open bank packet to client, that client would open the bank even 
// as server didn't send the packet. This is also blocked from clientside now.
bool IsNearBanker(short x, short y)
{
	short ox = x;
	short oy = y;
	short tx, ty;

	short hshx[9], hshy[9];
	hshx[0] = ox / 64;
	hshy[0] = oy / 64;
	hshx[1] = hshx[0] -1;
	hshy[1] = hshy[0] -1; 
	hshx[2] = hshx[0] -1;
	hshy[2] = hshy[0]; 
	hshx[3] = hshx[0] -1;
	hshy[3] = hshy[0] +1; 
	hshx[4] = hshx[0];
	hshy[4] = hshy[0] -1; 
	hshx[5] = hshx[0];
	hshy[5] = hshy[0] +1; 
	hshx[6] = hshx[0] +1;
	hshy[6] = hshy[0] -1; 
	hshx[7] = hshx[0] +1;
	hshy[7] = hshy[0]; 
	hshx[8] = hshx[0] +1;
	hshy[8] = hshy[0] +1; 

	NPCClass *npc;
	npc = NULL;

	for (int i = 0; i < 9; i++)
	{
		if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
		{
			npc_List = NPC[hshx[i]][hshy[i]];

			while (npc_List != NULL)
			{
				tx = npc_List->X;
				ty = npc_List->Y;

				if (npc_List->Type == 16 && tx - ox > -11 && tx - ox < 11 && ty - oy > -11 && ty - oy < 11)
					return true;
	
				npc_List = npc_List->next;
			}			
		}
	}
	return false;
}