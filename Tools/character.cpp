#include <stdio.h>
//#include <fstream.h>
#include <stdlib.h>
#include <iostream>
#include <winsock.h>
#include <time.h>


#include "main.h"

extern NetworkingClass	*N;



struct SkillsStruct Skills[20] = 
{
	{ 1, "unarmed" },
	{ 1, "swords" },
	{ 1, "axes" },
	{ 1, "blunts" },
	{ 1, "polearms" },
	{ 0, "bows" },
	{ 0, "combat tactics" },
	{ 0, "playerkilling tactics" },
	{ 0, "example skill 1" },
	{ 0, "example skill 2" },
	{ 0, "example skill 3" },
	{ 0, "example skill 4" },
	{ 0, "example skill 5" },
	{ 0, "example skill 6" },
	{ 0, "example skill 7" },
	{ 1, "Mining" },
	{ 1, "Forging" },
	{ 0, "gfs" },
	{ 0, "example skill 11" },
	{ 0, "" },
};





void CharacterTool::check_command()
{
	if (!strcmp("charinfo", C->command[0]))
	{
		p_charinfo();
	}
	if (!strcmp("infoskills", C->command[0]))
	{
		printf("==== character %s's skills:\n");
		printf("Skill edit ID, skill name, skill amount, skill exp\n");

		for (int i = 0; i < 20; i++)
		{
			printf("%d: %s, %d, %lu\n", i, Skills[i].Name, Char[C->editc].Skills[i].Level, Char[C->editc].Skills[i].Exp);
		}
	}
	if (!strcmp("exit", C->command[0]))
	{
		C->editingmode = 0;
		C->editc = -1;
		printf("not editing character anymore\n");
	}
	if (!strcmp("name", C->command[0]))
	{
		strcpy_s(Char[C->editc].Name, C->command[1]);
		for (int i = 0 ; i < 20; i++)
		{
			if (Char[C->editc].Name[i] == '_')
				Char[C->editc].Name[i] = ' ';
		}
		printf("changed\n");
	}
	if (!strcmp("str", C->command[0]))
	{
		Char[C->editc].Str = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("dex", C->command[0]))
	{
		Char[C->editc].Dex = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("con", C->command[0]))
	{
		Char[C->editc].Con = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("int", C->command[0]))
	{
		Char[C->editc].Int = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("lck", C->command[0]))
	{
		Char[C->editc].Lck = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("cha", C->command[0]))
	{
		Char[C->editc].Cha = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("level", C->command[0]))
	{
		Char[C->editc].Level = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("exp", C->command[0]))
	{
		Char[C->editc].Exp = atol(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("avatar", C->command[0]))
	{
		Char[C->editc].Avatar = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("crimcount", C->command[0]))
	{
		Char[C->editc].CrimCount = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("devmode", C->command[0]))
	{
		Char[C->editc].DevMode = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("hp", C->command[0]))
	{
		Char[C->editc].HPLeft = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("Race", C->command[0]))
	{
		Char[C->editc].Race = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("repupoints", C->command[0]))
	{
		Char[C->editc].KilledMonsters = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("reputation", C->command[0]))
	{
		Char[C->editc].Reputation = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("shrine", C->command[0]))
	{
		Char[C->editc].ShrineX = atoi(C->command[1]);
		Char[C->editc].ShrineY = atoi(C->command[2]);
		printf("changed\n");
	}
	if (!strcmp("statpoints", C->command[0]))
	{
		Char[C->editc].StatPoints = atoi(C->command[1]);
		printf("changed\n");
	}
	if (!strcmp("wanted", C->command[0]))
	{
		Char[C->editc].Wanted = !Char[C->editc].Wanted;
		printf("changed\n");
	}
	if (!strcmp("coords", C->command[0]))
	{
		Char[C->editc].X = atoi(C->command[1]);
		Char[C->editc].Y = atoi(C->command[2]);
		printf("changed\n");
	}
	if (!strcmp("skilllevel", C->command[0]))
	{			
		Char[C->editc].Skills[atoi(C->command[1])].Level = atoi(C->command[2]);
	}
	if (!strcmp("skillexp", C->command[0]))
	{
		Char[C->editc].Skills[atoi(C->command[1])].Exp = atoi(C->command[2]);
	}
	if (!strcmp("resetstats", C->command[0]))
	{
		Char[C->editc].StatPoints = 15;
		Char[C->editc].Int = 5;
		Char[C->editc].Con = 5;
		Char[C->editc].Dex = 5;
		Char[C->editc].Str = 5;
		Char[C->editc].Level = 1;
	}
	if (!strcmp("update", C->command[0]))
	{
		buffer[0] = 125;
		buffer[1] = 1;
		buffer[2] = 5;
		memcpy(&buffer[3], &Char[C->editc], sizeof(Char[C->editc]));
		send(N->socket, buffer, 3 + sizeof(Char[C->editc]), 0);
	}
	p_charinfo();
}


void CToolsClass::newcommand()
{
	parsed_to = 0;
	memset(&command, 0, sizeof(command));
	subcommand = 0;
	lst_command = 0;
}

