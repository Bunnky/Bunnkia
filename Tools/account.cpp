#include <stdio.h>
//#include <fstream.h>
#include <stdlib.h>
#include <iostream>
#include <winsock.h>
#include <time.h>


#include "main.h"


extern char					*buffer;
extern AccountTool			AccTool;
extern CToolsClass			*C;
extern ItemSearch			ISearch;

extern NetworkingClass		*N;


extern AccountToEdit acc[100];
extern bool acc_used[100];
extern CharacterToEdit Char[100];
extern bool char_used[100];
extern int last_recv;

void AccountTool::check_command()
{
	if (!strcmp("update", C->command[0]))
	{
		buffer[0] = 125;
		buffer[1] = 4;
		memcpy(&buffer[2], &acc[C->editaccount], sizeof(acc[C->editaccount]));
		send(N->socket, buffer, 2 + sizeof(acc[C->editaccount]), 0);
	}
	if (!strcmp("accinfo", C->command[0]))
	{
		p_accinfo();
	}
	if (!strcmp("banned", C->command[0]))
	{
		acc[C->editaccount].IsBanned = !acc[C->editaccount].IsBanned;
		printf("changed.");
	}
	if (!strcmp("name", C->command[0]))
	{
		strcpy_s(acc[C->editaccount].AccName, C->command[1]);
		printf("changed.");
	}
	if (!strcmp("pass", C->command[0]))
	{
		strcpy_s(acc[C->editaccount].AccPass, C->command[1]);
		printf("changed.");
	}
	if (!strcmp("id", C->command[0]))
	{
		acc[C->editaccount].ID = atoi(C->command[1]);
		printf("changed.");
	}
	if (!strcmp("premium", C->command[0]))
	{
		acc[C->editaccount].IsPremium = !acc[C->editaccount].IsPremium;
		printf("changed.");
	}
	if (!strcmp("bonus", C->command[0]))
	{
		acc[C->editaccount].IsGolden = !acc[C->editaccount].IsGolden;
	}
	if (!strcmp("exit", C->command[0]))
	{
		C->editingmode = 0;
		C->editaccount = 0;			
		printf("not editing account anymore\n");
	}
}