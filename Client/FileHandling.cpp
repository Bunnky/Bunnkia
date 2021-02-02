
#include "main.h"

//==============================================================================
//
//	Load all the datafiles
//
//==============================================================================
void LoadMap()
{	
	//fprintf(stderr, "Loading map");
	fstream filet;
	char stringi[25];

	//------------------------------------------------------------------------------
	//	Loading Map
	//------------------------------------------------------------------------------
	filet.open("data/map.dat", ios::in | ios::binary);
	if(!filet.is_open()) 
	{
		fprintf(stderr, "Loading map...failed.\n");
		exit(1);
	}
	else 
	{
		filet.read((char *)&stringi, sizeof(stringi));
		filet.read((char *)&Map, sizeof(Map));			
		filet.close();
		fprintf(stderr, "Loading map...done.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading SafeZones
	//------------------------------------------------------------------------------
	filet.open("data/safezones.dat", ios::in | ios::binary);
	if(!filet.is_open()) 
	{
		printf("Loading Safezones...failed.\n");
		exit(1);
	}
	else 
	{
//		filet.read((char *)&stringi, sizeof(stringi));
		filet.read((char *)&SafeZones, sizeof(SafeZones));			
		filet.close();
		printf("Loading Safezones...done.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading Tiles
	//------------------------------------------------------------------------------
	filet.open("data/data02.dat", ios::in|ios::binary);
	if(!filet.is_open()) 
	{
		printf("Loading Tiles...failed, quitting.\n");
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
		printf("Loading Tiles...done.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading FoW
	//------------------------------------------------------------------------------
	filet.open("data/Data01.dat", ios::in | ios::binary);
	if(!filet.is_open()) 
	{
		printf("Loading Fog of War...failed, reseting it to 0.\n");
		memset(&MapInfo.MapVisibility, 0, sizeof(MapInfo.MapVisibility));
		SaveFogOfWar();
		if (filet.is_open()) filet.close();
	}
	else 
	{
		filet.read((char *)&MapInfo.MapVisibility, sizeof(MapInfo.MapVisibility));			
		filet.close();
		printf("Loading Fog of War...done.\n");
	}



	char *data;



	//------------------------------------------------------------------------------
	//	Loading NPC types
	//------------------------------------------------------------------------------
	filet.open ("data/data04.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&NPCTypes, sizeof(NPCTypes));			

		printf("Decrypting NPCTypes...");
		data = new char[sizeof(NPCTypes)];
		memcpy(data, &NPCTypes, sizeof(NPCTypes));

		for (int i = 0; i < sizeof(NPCTypes); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&NPCTypes, data, sizeof(NPCTypes));
		delete[] data;
		
		filet.close();
		printf("Loading NPCs...done.\n");
	}
	else
	{
		memset(&NPCTypes, 0, sizeof(NPCTypes));
		printf("Loading NPCs..failed.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading Monster types
	//------------------------------------------------------------------------------
	filet.open ("data/data03.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&MonsterTypes, sizeof(MonsterTypes));			

		printf("Decrypting Monster types...");
		data = new char[sizeof(MonsterTypes)];
		memcpy(data, &MonsterTypes, sizeof(MonsterTypes));

		for (int i = 0; i < sizeof(MonsterTypes); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&MonsterTypes, data, sizeof(MonsterTypes));
		delete[] data;
		
		filet.close();
		printf("Loading MonsterTypes...done.\n");
	}
	else
	{
		memset(&MonsterTypes, 0, sizeof(MonsterTypes));
		printf("Loading MonsterTypes...failed.\n");
	}

	LoadObjects();

	//------------------------------------------------------------------------------
	//	Loading Object types
	//------------------------------------------------------------------------------
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
		printf("Loading Objecttypes...done.\n");
	}
	else
	{
		printf("Loading Objecttypes...failed, quitting.\n");
		exit(1);
	}

	//------------------------------------------------------------------------------
	//	Loading Weapon types
	//------------------------------------------------------------------------------
	filet.open ("data/data05.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Weapons, sizeof(Weapons));			
		filet.close();

		printf("Decrypting Weapontypes...");

		data = new char[sizeof(Weapons)];
		memcpy(data, &Weapons, sizeof(Weapons));

		for (int i = 0; i < sizeof(Weapons); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Weapons, data, sizeof(Weapons));
		delete[] data;
		
		printf("Loading Weapontypes...done.\n");
	}
	else
	{
		memset(&Weapons, 0, sizeof(Weapons));
		printf("Loading Weapontypes...failed.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading Armor types
	//------------------------------------------------------------------------------
	filet.open ("data/data06.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Armors, sizeof(Armors));			

		printf("Decrypting Armortypes...");
		data = new char[sizeof(Armors)];
		memcpy(data, &Armors, sizeof(Armors));

		for (int i = 0; i < sizeof(Armors); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Armors, data, sizeof(Armors));
		delete[] data;


		filet.close();
		printf("Loading armortypes...done.\n");
	}
	else
	{
		memset(&Armors, 0, sizeof(Armors));
		printf("Loading armortypes...failed.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading Collectable types
	//------------------------------------------------------------------------------
	filet.open ("data/data07.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Collectables, sizeof(Collectables));	

		printf("Decrypting Collectabletypes...");
		data = new char[sizeof(Collectables)];
		memcpy(data, &Collectables, sizeof(Collectables));

		for (int i = 0; i < sizeof(Collectables); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Collectables, data, sizeof(Collectables));
		delete[] data;
		
		filet.close();
		printf("Loading collectables...done.\n");
	}
	else
	{
		memset(&Collectables, 0, sizeof(Collectables));
		printf("Loading collectables...failed.\n");
	}

	//------------------------------------------------------------------------------
	//	Loading Useable types
	//------------------------------------------------------------------------------
	filet.open ("data/data08.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.read((char *)&Useables, sizeof(Useables));			

		printf("Decrypting Useableitem types...");
		data = new char[sizeof(Useables)];
		memcpy(data, &Useables, sizeof(Useables));

		for (int i = 0; i < sizeof(Useables); i++)
		{
			data[i] ^= 212;		
		}

		memcpy(&Useables, data, sizeof(Useables));
		delete[] data;
		
		filet.close();
		printf("Loading Useables...done.\n");
	}
	else
	{
		memset(&Useables, 0, sizeof(Useables));
		printf("Loading Useables...failed.\n");
	}


}


//==============================================================================
//
//	Save Map's fog of war
//
//==============================================================================
void SaveFogOfWar()
{
	fstream filet;

	filet.open("data/Data01.dat", ios::in | ios::binary);

	if (filet.is_open())
	{
		filet.write((char *)&MapInfo.MapVisibility, sizeof(MapInfo.MapVisibility));
		filet.close();
	}
}


//==============================================================================
//
//	Load client settings from .ini
//
//==============================================================================
void loadSettings() {
  ifstream LSets("settings.ini", ios::in);

  if (!LSets.is_open()) {
    cout << "ERROR: settings.ini cannot be loaded!\nLoading default settings.\n";

	Screen.Fullscreen = 0;

	Backpack.X = 440; Backpack.Y = 264;

	Minimap->X = 640; Minimap->Y = 264;

	Client.SayOn = true;
	Client.WhisOn = true;
	Client.GlobOn = true;

	Client.WASD = true;

	Client.OwnName = true;
	Client.PlNames = true;

	Client.CraftPer = 0;

	Backpack.Docked = false; Minimap->Docked = false;

	return;
  }

  int i = 0;
  char buffer[24] = "\0";

  char sbuf[4] = "\0";

  char currentchar;

  do {
    memset(buffer, 0, 24);

    do {
      currentchar = LSets.get();
      buffer[i] = currentchar;
      i++;
    } while (LSets.good() && i <= 23 && currentchar != '\n' && currentchar != '=');
    i = 0;

    if (!strcmp(buffer, "Fullscreen=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Screen.Fullscreen = false;
	  else
	  Screen.Fullscreen = true;
    }

    if (!strcmp(buffer, "BackpackX=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Backpack.X = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "BackpackY=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Backpack.Y = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "BPDocked=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Backpack.Docked = false;
	  else
	  Backpack.Docked = true;
    }

    if (!strcmp(buffer, "MinimapX=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Minimap->X = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "MinimapY=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Minimap->Y = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "MMDocked=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Minimap->Docked = false;
	  else
	  Minimap->Docked = true;
    }


    if (!strcmp(buffer, "BankX=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Bank.X = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "BankY=")) {
      do {
        currentchar = LSets.get();
        if (currentchar != '\n')
          sbuf[i] = currentchar;
        i++;
      } while (currentchar != '\n' && i <= 3 && LSets.good());
      i = 0;

      Bank.Y = atoi(sbuf);
      memset(sbuf, 0, 4);
    }

    if (!strcmp(buffer, "BankDocked=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Bank.Docked = false;
	  else
	  Bank.Docked = true;
    }

    if (!strcmp(buffer, "MinimapOn=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Minimap->On = false;
	  else
	  Minimap->On = true;
    }

    if (!strcmp(buffer, "BackpackOn=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Backpack.On = false;
	  else
	  Backpack.On = true;
    }

    if (!strcmp(buffer, "TarProtection=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.TargetProtectionMode = false;
	  else
	  Client.TargetProtectionMode = true;
    }

    if (!strcmp(buffer, "SayOn=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.SayOn = false;
	  else
	  Client.SayOn = true;
    }

    if (!strcmp(buffer, "WhisOn=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.WhisOn = false;
	  else
	  Client.WhisOn = true;
    }

    if (!strcmp(buffer, "GlobOn=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.GlobOn = false;
	  else
	  Client.GlobOn = true;
    }

	if (Client.SayOn == NULL)
	{
		Client.SayOn = true;
		Client.WhisOn = true;
		Client.GlobOn = true;
	}	

    if (!strcmp(buffer, "WASD=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.WASD = false;
	  else
	  Client.WASD = true;
    }

	if (Client.WASD == NULL)
	{
		Client.WASD = true;
	}

    if (!strcmp(buffer, "OwnName=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.OwnName = false;
	  else
	  Client.OwnName = true;
    }

    if (!strcmp(buffer, "PlNames=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      Client.PlNames = false;
	  else
	  Client.PlNames = true;
    }

	if (Client.OwnName == NULL)
	{
		Client.OwnName = true;
		Client.PlNames = true;
	}

    if (!strcmp(buffer, "CraftPer=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
		Client.CraftPer = 0;
	  else if (atoi(sbuf) == 1)
		Client.CraftPer = 1;
	  else if (atoi(sbuf) == 2)
		Client.CraftPer = 2;
	  else if (atoi(sbuf) == 3)
		Client.CraftPer = 3;
	  else if (atoi(sbuf) == 4)
		Client.CraftPer = 4;
	  else		  
		Client.CraftPer = 0;
    }

	if (Client.CraftPer == NULL)
	{
		Client.CraftPer = 0;
	}
/*
    if (!strcmp(buffer, "TexturedMouse=")) {
      sbuf[0] = LSets.get();
	  if (atoi(sbuf) == 0)
      TexturedMouse = false;
	  else
	  TexturedMouse = true;
    }
*/

  } while (LSets.good());

  LSets.close();

	Minimap->Docked = false;
	Minimap->On = true;
	Minimap->X = 640;
	Minimap->Y = 264;

  return;
}


//==============================================================================
//
//	Save client settings to .ini
//
//==============================================================================
void saveSettings() {
  ofstream SSets("settings.ini", ios::out);

  if (!SSets.is_open()) {
    cout << "ERROR: settings.ini cannot be saved!\n";
    return;
  }

  SSets << "-Bunnkia settings file-" << endl;
  SSets << "=======================" << endl;

  SSets << "Fullscreen=" << Screen.Fullscreen << endl;
  SSets << " Some window positions:" << endl;
  SSets << "BackpackX=" << Backpack.X << endl;
  SSets << "BackpackY=" << Backpack.Y << endl;
  SSets << "BPDocked="  << Backpack.Docked << endl;

  SSets << "MinimapX=" << Minimap->X << endl;
  SSets << "MinimapY=" << Minimap->Y << endl;
  SSets << "MMDocked=" << Minimap->Docked << endl;

  SSets << "BankX=" << Bank.X << endl;
  SSets << "BankY=" << Bank.Y << endl;
  SSets << "BankDocked=" << Bank.Docked << endl;
  
  SSets << "MinimapOn=" << Minimap->On << endl;
  SSets << "BackpackOn=" << Backpack.On << endl;

  SSets << "TarProtection=" << Client.TargetProtectionMode << endl;

  SSets << "SayOn=" << Client.SayOn << endl;  
  SSets << "WhisOn=" << Client.WhisOn << endl;  
  SSets << "GlobOn=" << Client.GlobOn << endl;

  SSets << "WASD=" << Client.WASD << endl;

  SSets << "OwnName=" << Client.OwnName << endl; 
  SSets << "PlNames=" << Client.PlNames << endl;

  SSets << "CraftPer=" << Client.CraftPer << endl;

 //  SSets << "TexturedMouse=" << TexturedMouse << endl;

  SSets.close();
   
	if (pldatabase) 
	{
		pldatabase->clearlist();
		delete pldatabase;
		pldatabase = NULL;
	}
	if (mondatabase) 
	{
		mondatabase->clearlist();
		delete mondatabase;
		mondatabase = NULL;
	}
	if (npcdatabase)
	{
		npcdatabase->clearlist();
		delete npcdatabase;
		npcdatabase = NULL;
	}
	if (bodydatabase)
	{
		bodydatabase->clearlist();
		delete bodydatabase;
		bodydatabase = NULL;
	}
}


//==============================================================================
//
//	Load Objects into templist/hashtable
//
//==============================================================================
void LoadObjects()
{
	printf("\nProgram: Load objects-\n");
	memset(&Objects, 0, sizeof(Objects));
	obj_List = NULL;

	fstream file;

	file.open("data/Objects.dat", ios::in | ios::binary);

	if (!file.is_open())
	{
		fprintf(stderr, "\"data/Objects.dat\" couldn't be opened for loading. (in, binary)\n");
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
		memset(NewObject, 0, sizeof(struct MapObjects));
		
		file.read((char *)&NewObjectToLoad, sizeof( NewObjectToLoad));

		NewObject->_X = NewObjectToLoad._X;
		NewObject->_Y = NewObjectToLoad._Y;
		NewObject->Text = NewObjectToLoad.Text;
		NewObject->Type = NewObjectToLoad.Type;
		NewObject->X = NewObjectToLoad.X;
		NewObject->Y = NewObjectToLoad.Y;
		NewObject->On = true;
//		memcpy(&NewObject->Container, &NewObjectToLoad.Container, sizeof(NewObject->Container));

		//printf("diudiu\n");

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
	
	//printf("ok\n");

	printf("--Divide objects from templist to hashtables lists-");

	short hshx, hshy;
	obj_List = tmplist;
	MapObjects *tmptmp;
	tmptmp = NULL;
	bool f_add = false;
	while (obj_List != NULL)
	{
		f_add = false;
        hshx = obj_List->X / 32;
		hshy = obj_List->Y / 32;

		if (Objects[hshx][hshy] == NULL)
		{
			tmptmp = obj_List->next;
			Objects[hshx][hshy] = obj_List;
			Objects[hshx][hshy]->next = NULL;

			//printf("1-");
			obj_List = tmptmp;
			f_add = true;
		}
		else
		{
			tmptmp = obj_List->next;
			obj_List->next = Objects[hshx][hshy];
			Objects[hshx][hshy] = obj_List;

			//printf("2-");
			obj_List = tmptmp;
			f_add = true;
		}

		if (!f_add)
		obj_List = obj_List->next;
	}

	//printf("\nok\n");


/*	printf("--Remove templist-");
	obj_List = tmplist;
	while (obj_List != NULL)
	{
		tmptmp = obj_List->next;

		delete obj_List;
        
		obj_List = tmptmp;
	}*/
	if (file.is_open()) file.close();
	//printf("ok\n");
	//printf("All ok\n");
}

