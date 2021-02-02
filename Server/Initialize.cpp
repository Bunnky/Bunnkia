
#include <stdio.h>


#include "main.h"

extern BodyClass Body[];
extern AccountManager *AccManager;

void Setup()
{
	printf("==---------------------------------------------------------==\n");
	printf("Server compiled at: %s %s\n", __TIME__,__DATE__);
	printf("from file: %s\n", __FILE__);
	printf("Port: %d\n", PORT);
	printf("==---------------------------------------------------------==\n");

	printf("sizeof( PlayerNode ) = %d\n", sizeof(CharacterData));
	printf("sizeof( Account ) = %d\n", sizeof(AccountData));
	printf("sizeof( ClientClass ) = %d\n", sizeof(ClientClass));
	printf("sizeof( PACKET_PLAYERINFO ) = %d\n", sizeof(PACKET_PLAYERINFO));
	printf("sizeof( MonsterClass ) = %d\n", sizeof(MonsterClass));
	printf("sizeof( PACKET_CHARCHOOSE ) = %d\n", sizeof(PACKET_CHARCHOOSE));

	printf("FD_SETSIZE set at %d\n", FD_SETSIZE);

	fstream filet;

	LastStaticID = 10;
	LastTakenID = 1;

	memset(&Items, 0, sizeof(Items));
	memset(&Ground, 0, sizeof(Ground));

/*	filet.open("accounts.vsf", ios::in | ios::binary);

	int i;
	if (filet.is_open())
	{
		filet.read((char *)&LastStaticID, sizeof(LastStaticID));
		
		for (i = 0; i < m_Accounts; i++)		
		{
			filet.read((char *)&Accounts[i], sizeof(Accounts[i]));
		}
		filet.close();
	}

	int count = 0;
	for (i = 0; i < m_Accounts; i++)
	{
		if (Accounts[i].ID > 0)
			count++;	
	}
	printf("Used accounts: %d / %d\n", count, m_Accounts);
*/

	filet.open("ground.vsf", ios::in | ios::binary);
	
	int i, count;
	if (filet.is_open())
	{
		for (i = 0; i < 256; i++)		
		{
			filet.read((char *)&Ground[i], sizeof(Ground[i]));
		}
		filet.close();
	}

	filet.open("bodies.vsf", ios::in | ios::binary);
	
	if (filet.is_open())
	{
		for (i = 0; i < m_Bodies; i++)		
		{
			filet.read((char *)&Body[i], sizeof(Body[i]));
		}
		filet.close();
	}


	filet.open("items.vsf", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&LastTakenID, sizeof(LastTakenID));
		
		for (i = 0; i < m_Items; i++)
		{
			filet.read((char *)&Items[i], sizeof(Items[i]));
		}
		filet.close();
	}

	if (LastTakenID < 0) LastTakenID = 0;

	count = 0;
	for (i = 0; i < m_Items; i++)
	{
		if (Items[i].Used)
			count++;	
	}
	printf("Used items: %d / %d\n", count, m_Items);

/*
	bool IsUsed[m_Items];
	memset(&IsUsed, 0, sizeof(IsUsed));
	long count, count2 = 0;
	count = 0;
	printf("Repairing item lists");
	for (i = 1; i < m_Items; i++)
	{		
		if (Items[i].Used)// && !IsUsed[i])
		{
			count = 0;
			for (j = 0; j < m_Accounts; j++)
			{
				if (Accounts[j].ID > 0) 
				{
				for (int char_ = 0; char_ < 2; char_++)
				{
					if (Accounts[j].Char[char_].Used)
					{
					for (int b = 0; b < MaxBackpackItems; b++)
					{
						if (Accounts[j].Char[char_].Backpack[b] == i && count > 0)
						{
							Accounts[j].Char[char_].Backpack[b] = 0;
							printf("-er-");
						}
						else if (Accounts[j].Char[char_].Backpack[b] == i)
							count++;
					}
					for (int ba = 0; ba < MaxBankItems; ba++)
					{
						if (Accounts[j].Char[char_].Bank[b] == i && count > 0)
						{
							Accounts[j].Char[char_].Bank[b] = 0;
							printf("-er-");
						}
						else if (Accounts[j].Char[char_].Bank[b] == i)
							count++;
					}
					for (int we = 0; we < MaxWearedItems; we++)
					{
						if (Accounts[j].Char[char_].Weared[b] == i && count > 0)
						{
							Accounts[j].Char[char_].Weared[b] = 0;
							printf("-er-");
						}
						else if (Accounts[j].Char[char_].Weared[b] == i)
							count++;
					}
					}
				}
				}
			}

			IsUsed[i] = true;
		}

		if (count2 % 10000 == 0) printf(".");
		count2++;
	}
*/

/*	bool rimyyv = false;
	int j;
	for (j = 0; j < m_Accounts; j++)
	{
		if (Accounts[j].ID > 0)
		{
			for (int char_ = 0; char_ < 2; char_++)
			{
				if (Accounts[j].Char[char_].Used)
				{
				for (int b = 0; b < MaxBackpackItems; b++)
				{
					rimyyv = false;
					if (Accounts[j].Char[char_].Backpack[b] < 0 || Accounts[j].Char[char_].Backpack[b] >= m_Items)
					{
						Accounts[j].Char[char_].Backpack[b] = 0;
						printf("-er2-");
					}					
					if (!Items[Accounts[j].Char[char_].Backpack[b]].Used && Accounts[j].Char[char_].Backpack[b] != 0)
					{
						Accounts[j].Char[char_].Backpack[b] = 0;
						printf("-er3-");
					}
					if (Items[Accounts[j].Char[char_].Backpack[b]].Used)
					{
						if (Items[Accounts[j].Char[char_].Backpack[b]].Durability <= 0) Items[Accounts[j].Char[char_].Backpack[b]].Durability = 1;
						if (Items[Accounts[j].Char[char_].Backpack[b]].Family <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Backpack[b]].Type <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Backpack[b]].KnowID < 0) Items[Accounts[j].Char[char_].Backpack[b]].KnowID = b;
						if (Items[Accounts[j].Char[char_].Backpack[b]].Quantity <= 0) Items[Accounts[j].Char[char_].Backpack[b]].Quantity = 1;						
					}
					if (rimyyv) Accounts[j].Char[char_].Backpack[b] = 0;
				}
				for (b = 0; b < MaxBankItems; b++)
				{
					rimyyv = false;
					if (Accounts[j].Char[char_].Bank[b] < 0 || Accounts[j].Char[char_].Bank[b] >= m_Items)
					{
						Accounts[j].Char[char_].Bank[b] = 0;
						printf("-er-");
					}
					if (!Items[Accounts[j].Char[char_].Bank[b]].Used && Accounts[j].Char[char_].Bank[b] != 0)
					{
						Accounts[j].Char[char_].Bank[b] = 0;
						printf("-er3-");
					}
					if (Items[Accounts[j].Char[char_].Bank[b]].Used)
					{
						if (Items[Accounts[j].Char[char_].Bank[b]].Durability <= 0) Items[Accounts[j].Char[char_].Bank[b]].Durability = 1;
						if (Items[Accounts[j].Char[char_].Bank[b]].Family <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Bank[b]].Type <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Bank[b]].KnowID < 0) Items[Accounts[j].Char[char_].Bank[b]].KnowID = b;
						if (Items[Accounts[j].Char[char_].Bank[b]].Quantity <= 0) Items[Accounts[j].Char[char_].Bank[b]].Quantity = 1;
						if (Items[Accounts[j].Char[char_].Bank[b]].x > 120) Items[Accounts[j].Char[char_].Bank[b]].x = rand() % 100;
						if (Items[Accounts[j].Char[char_].Bank[b]].y > 120) Items[Accounts[j].Char[char_].Bank[b]].y = rand() % 100;
					}
					if (rimyyv) Accounts[j].Char[char_].Bank[b] = 0;
				}
				for (b = 0; b < MaxWearedItems; b++)
				{
					rimyyv = false;
					if (Accounts[j].Char[char_].Weared[b] < 0 || Accounts[j].Char[char_].Weared[b] >= m_Items)
					{
						Accounts[j].Char[char_].Weared[b] = 0;
						printf("-er-");
					}
					if (!Items[Accounts[j].Char[char_].Weared[b]].Used && Accounts[j].Char[char_].Weared[b] != 0)
					{
						Accounts[j].Char[char_].Weared[b] = 0;
						printf("-er3-");
					}
					if (Items[Accounts[j].Char[char_].Weared[b]].Used)
					{
						if (Items[Accounts[j].Char[char_].Weared[b]].Durability <= 0) Items[Accounts[j].Char[char_].Weared[b]].Durability = 1;
						if (Items[Accounts[j].Char[char_].Weared[b]].Family <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Weared[b]].Type <= 0) rimyyv = true;
						if (Items[Accounts[j].Char[char_].Weared[b]].KnowID < 0) Items[Accounts[j].Char[char_].Weared[b]].KnowID = b;
						if (Items[Accounts[j].Char[char_].Weared[b]].Quantity <= 0) Items[Accounts[j].Char[char_].Weared[b]].Quantity = 1;						
					}
					if (rimyyv) Accounts[j].Char[char_].Weared[b] = 0;
				}
				}
			}
		}
	}
*/


	printf("done.\n");

	char stringi[25];

	memset(&Clients, 0, sizeof(Clients));

	filet.open("data/map.dat", ios::in | ios::binary);
	if(!filet.is_open()) 
	{		
		printf("Loading map failed.\n");
		exit(1);
	}
	else 
	{
		filet.read((char *)&stringi, sizeof(stringi));
		filet.read((char *)&Map, sizeof(Map));			
		filet.close();
		printf("Loading map succeed.\n");
	}

	char *data;


	printf("Loading NPCs");
	// Load NPCs
	filet.open ("data/data04.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&NPCTypes, sizeof(NPCTypes));			

		printf(". Decrypt NPCTypes types...");
		data = new char[sizeof(NPCTypes)];
		memcpy(data, &NPCTypes, sizeof(NPCTypes));

		for (int i = 0; i < sizeof(NPCTypes); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&NPCTypes, data, sizeof(NPCTypes));
		delete[] data;
		
		filet.close();
		printf("done.\n");
	}
	else
	{
		memset(&NPCTypes, 0, sizeof(NPCTypes));
		printf("..failed.\n");
	}


	printf("Loading MonsterTypes");
	// Load NPCs
	filet.open ("data/data03.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&MonsterTypes, sizeof(MonsterTypes));			

		printf(". Decrypt Monster types...");
		data = new char[sizeof(MonsterTypes)];
		memcpy(data, &MonsterTypes, sizeof(MonsterTypes));

		for (int i = 0; i < sizeof(MonsterTypes); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&MonsterTypes, data, sizeof(MonsterTypes));
		delete[] data;
		
		filet.close();
		printf("done.\n");
	}
	else
	{
		memset(&MonsterTypes, 0, sizeof(MonsterTypes));
		printf("..failed.\n");
	}



	Load_NPCs_Lists();
	Load_Monsters_Lists();


	printf("Loading Tiles");

	filet.open("data/data02.dat", ios::in|ios::binary );
	if(!filet.is_open()) 
	{
		printf("...failed, quitting.\n");
		exit(1);
	}
	else 
	{
		char *data;
		data = new char[sizeof(Tiles)];
		filet.read((char *)data, sizeof(Tiles));
		
		for (int i = 0; i < sizeof(Tiles); i++)
			data[i] ^= 212;

		memcpy(&Tiles, data, sizeof(Tiles));

		delete[] data;

		filet.close();
		printf("...done.\n");
	}
	Tiles[0].Block = true;

	printf("Loading Objects");
	// Load NPCs
	filet.open ("data/data09.dat", ios::in|ios::binary);

	if (filet.is_open())
	{
		filet.seekg(25);
		char *data;
		data = new char[sizeof(ObjectTypes)];
		filet.read((char *)data, sizeof(ObjectTypes));
		
		for (int i = 0; i < sizeof(ObjectTypes); i++)
			data[i] ^= 212;

		memcpy(&ObjectTypes, data, sizeof(ObjectTypes));

		delete[] data;

		filet.close();
	}
	else
	{
		printf("...failed, quitting.\n");
		exit(1);
	}

	Load_Objects_obj_Lists();







	printf("Loading weapontypes");
	// Load Map
	filet.open ("data/data05.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Weapons, sizeof(Weapons));			
		filet.close();

		printf(". Decrypt weapontypes...");

		data = new char[sizeof(Weapons)];
		memcpy(data, &Weapons, sizeof(Weapons));

		for (int i = 0; i < sizeof(Weapons); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Weapons, data, sizeof(Weapons));
		delete[] data;
		
		printf("done.\n");
	}
	else
	{
		memset(&Weapons, 0, sizeof(Weapons));
		printf("..failed.\n");
	}

	printf("Loading armortypes");
	// Load Map
	filet.open ("data/data06.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Armors, sizeof(Armors));			

		printf(". Decrypt Armortypes...");
		data = new char[sizeof(Armors)];
		memcpy(data, &Armors, sizeof(Armors));

		for (int i = 0; i < sizeof(Armors); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Armors, data, sizeof(Armors));
		delete[] data;


		filet.close();
		printf("done.\n");
	}
	else
	{
		memset(&Armors, 0, sizeof(Armors));
		printf("..failed.\n");
	}

	printf("Loading collectables");
	// Load Map
	filet.open ("data/data07.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Collectables, sizeof(Collectables));	

		printf(". Decrypt collectabletypes...");
		data = new char[sizeof(Collectables)];
		memcpy(data, &Collectables, sizeof(Collectables));

		for (int i = 0; i < sizeof(Collectables); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Collectables, data, sizeof(Collectables));
		delete[] data;
		
		filet.close();
		printf("done.\n");
	}
	else
	{
		memset(&Collectables, 0, sizeof(Collectables));
		printf("..failed.\n");
	}

	printf("Loading Useables");

	filet.open ("data/data08.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Useables, sizeof(Useables));			

		printf(". Decrypt Useableitem types...");
		data = new char[sizeof(Useables)];
		memcpy(data, &Useables, sizeof(Useables));

		for (int i = 0; i < sizeof(Useables); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Useables, data, sizeof(Useables));
		delete[] data;
		
		filet.close();
		printf("done.\n");
	}
	else
	{
		memset(&Useables, 0, sizeof(Useables));
		printf("..failed.\n");
	}





	printf("Loading ShopStorages");
	// Load NPCs
	filet.open ("data/ShopStorages.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&stringi, sizeof(stringi));
		filet.read((char *)&ShopStorages, sizeof(ShopStorages));			
		filet.close();
		printf("...done.\n");
	}
	else
	{
		memset(&ShopStorages, 0, sizeof(ShopStorages));
		printf("..failed.\n");
	}


	WSADATA wsaData;


	Server_Address.sin_family = AF_INET;
	Server_Address.sin_addr.s_addr = INADDR_ANY;
	Server_Address.sin_port = htons(PORT);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("Error at WSAStartup().\n");
	}

	
	ListeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (bind(ListeningSocket, (struct sockaddr *)&Server_Address, sizeof(Server_Address)) == -1)
		printf("Error at bind()\n");

	

	listen(ListeningSocket, SOMAXCONN);
}




void Load_NPCs_Lists()
{
	int i, j;
	printf("\nProgram: Load NPC-\n");
	for (i = 0; i < 128; i++)
	{
		for (j = 0; j < 128; j++)
		{
			NPC[i][j] = NULL;
		}
	}
	npc_List = NULL;

	fstream file;

	file.open("data/NPCSpawns.dat", ios::in | ios::binary);

	if (!file.is_open())
	{
		printf("\"dat/NPCSpawns.dat\" couldn't be opened for loading. (in, binary)\n");
		return;
	}

	printf("--Load NPCs into templist-");

	NPCSpawnStruct2 n;

    while (file.good())
	{

		file.read((char *)&n, sizeof( struct NPCSpawnStruct2));

		if (file.good())
		{
			NPCSpawnStruct *m = new NPCSpawnStruct;

			m->X = n.X;
			m->Y = n.Y;
			m->conv_id = n.conv_id;
			m->MaxDist_x = n.MaxDist_x;
			m->MaxDist_y = n.MaxDist_y;
			m->ShopID = n.ShopID;
			m->Type = n.Type;
			m->Used = false;
			m->LstSpawned = 0;			

			if (NPCSpawns == NULL)
			{
				m->next = NULL;
				NPCSpawns = m;
			}
			else
			{
				m->next = NPCSpawns;
				NPCSpawns = m;				
			}
		}
	}	
	
/*	short hshx, hshy;
//	npc_List = tmplist;
	NPCSpawnStruct *tmptmp;
	tmptmp = NULL;
	short Count = 1;
	bool f_add = false;

	while (tmplist != NULL)
	{
		NPCClass *NewNPC = new NPCClass;

		f_add = false;
        hshx = tmplist->X / 64;
		hshy = tmplist->Y / 64;

		NewNPC->Alive = ALIVE;
		NewNPC->conv_id = tmplist->conv_id;
		NewNPC->LstAttack = GetTickCount();
		NewNPC->LstDeath = GetTickCount();
		NewNPC->LstMove = GetTickCount();
		NewNPC->MaxDist_x = tmplist->MaxDist_x;
		NewNPC->MaxDist_y = tmplist->MaxDist_y;
		NewNPC->NPCMonsterTarget = NULL;
		NewNPC->NPCPLTarget = -1;
		NewNPC->ShopID = tmplist->ShopID;
		NewNPC->Type = tmplist->Type;
		NewNPC->X = tmplist->X;
		NewNPC->Y = tmplist->Y;
		NewNPC->oX = tmplist->X;
		NewNPC->oY = tmplist->Y;
		NewNPC->HPLeft = NPCTypes[ tmplist->Type ].max_hp;
		NewNPC->ID = Count;

		if (NPC[hshx][hshy] == NULL)
		{
			tmptmp = tmplist->next;
			NPC[hshx][hshy] = NewNPC;
			NPC[hshx][hshy]->next = NULL;
			tmplist = tmptmp;
			f_add = true;
		}
		else
		{
			tmptmp = tmplist->next;
			NewNPC->next = NPC[hshx][hshy];
			NPC[hshx][hshy] = NewNPC;
			tmplist = tmptmp;
			f_add = true;
		}
        
		if (!f_add)
		tmplist = tmplist->next;
		Count++;
	}
*/
	printf("ok\n");

	if (file.is_open()) file.close();
	printf("ok\n");
	printf("All ok\n");
}



void Load_Monsters_Lists()
{
	printf("\nProgram: Load Monsters-\n");
	MonsterSpawns = NULL;

	fstream file;

	file.open("data/MonsterSpawns.dat", ios::in | ios::binary);

	if (!file.is_open())
	{
		printf("\"data/MonsterSpawns.dat\" couldn't be opened for loading. (in, binary)\n");
		return;
	}

	printf("--Load Monsters into templist-");

	MonsterSpawnStruct *tmplist;
	tmplist = NULL;

	MonsterSpawnStruct2 NewSpawn2;

    while (file.good())
	{
		memset(&NewSpawn2, 0, sizeof(NewSpawn2));
		file.read((char *)&NewSpawn2, sizeof( MonsterSpawnStruct2));

		if (file.good())
		{
			MonsterSpawnStruct *NewSpawn = new MonsterSpawnStruct;
	
			NewSpawn->MaxDist_x = NewSpawn2.MaxDist_x;
			NewSpawn->MaxDist_y = NewSpawn2.MaxDist_y;
			NewSpawn->Type = NewSpawn2.Type;
			NewSpawn->Used = false;
			NewSpawn->X = NewSpawn2.X;
			NewSpawn->Y = NewSpawn2.Y;

			if (tmplist == NULL)
			{
				NewSpawn->next = NULL;
				tmplist = NewSpawn;
			}
			else
			{
				NewSpawn->next = tmplist;
				tmplist = NewSpawn;
			}
		}
	}
	MonsterSpawns = tmplist;
	
	printf("ok\n");

/*	printf("--Remove templist-");
	mon_List = tmplist;
	while (mon_List != NULL)
	{
		tmptmp = mon_List->next;

		delete mon_List;
        
		mon_List = tmptmp;
	}*/
	if (file.is_open()) file.close();
	printf("ok\n");
	printf("All ok\n");
}

void Load_Objects_obj_Lists()
{
	printf("\nProgram: Load objects-\n");
	memset(&Objects, 0, sizeof(Objects));
	obj_List = NULL;

	fstream file;

	file.open("data/Objects.dat", ios::in | ios::binary);

	if (!file.is_open())
	{
		printf("\"dat/Objects.dat\" couldn't be opened for loading. (in, binary)\n");
		return;
	}

	printf("--Load objects into templist-");

	MapObjects *tmplist;
	tmplist = NULL;


	MapObjectsToSave NewObjectToLoad;

    while (file.good())
	{
		MapObjects *NewObject = new MapObjects;

		memset(&NewObjectToLoad, 0, sizeof(NewObjectToLoad));
		memset(NewObject, 0, sizeof(MapObjects));
		
		file.read((char *)&NewObjectToLoad, sizeof( NewObjectToLoad));

		NewObject->_X = NewObjectToLoad._X;
		NewObject->_Y = NewObjectToLoad._Y;
		NewObject->Text = NewObjectToLoad.Text;
		NewObject->Type = NewObjectToLoad.Type;
		NewObject->X = NewObjectToLoad.X;
		NewObject->Y = NewObjectToLoad.Y;
		NewObject->On = true;
//		memcpy(&NewObject->Container, &NewObjectToLoad.Container, sizeof(NewObject->Container));


		if (file.good())
		{
			if (tmplist == NULL)
			{
				NewObject->next = NULL;
				tmplist = NewObject;
			}
			else
			{
				NewObject->next = tmplist;
				tmplist = NewObject;
			}
		}
		else
		{
			delete NewObject;
		}
	}
	
	printf("ok\n");

	printf("--Divide objects from templist to hashtables lists-");

	short hshx, hshy;
	obj_List = tmplist;
	MapObjects *tmptmp;
	tmptmp = NULL;
	short Count = 0;
	bool f_add = false;
	while (obj_List != NULL)
	{
		f_add = false;
        hshx = obj_List->X / 64;
		hshy = obj_List->Y / 64;

		if (obj_List->Type == 32)
		{
			CrimSpawnList *newS;
			newS = new CrimSpawnList;

			newS->X = obj_List->X;
			newS->Y = obj_List->Y;

			if (crimSpawns == NULL)
			{
				newS->Next = NULL;
				newS->Prev = NULL;
				crimSpawns = newS;
			}
			else
			{
				newS->Next = crimSpawns;				
				newS->Prev = NULL;
				crimSpawns = newS;
			}			
		}

		if (Objects[hshx][hshy] == NULL)
		{
			tmptmp = obj_List->next;
			Objects[hshx][hshy] = obj_List;
			Objects[hshx][hshy]->next = NULL;

			obj_List = tmptmp;
			f_add = true;
		}
		else
		{
			tmptmp = obj_List->next;
			obj_List->next = Objects[hshx][hshy];
			Objects[hshx][hshy] = obj_List;

			obj_List = tmptmp;
			f_add = true;
		}
        
		if (!f_add)
		obj_List = obj_List->next;
		Count++;
	}

	printf("\nok\n");

	printf("All-Object count = %d\n", Count);

/*	printf("--Remove templist-");
	List = tmplist;
	while (List != NULL)
	{
		tmptmp = List->next;

		delete List;
        
		List = tmptmp;
	}*/
	if (file.is_open()) file.close();
	printf("All ok\n");
}














