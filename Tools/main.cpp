#include <stdio.h>
//#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <winsock.h>
#include <time.h>
#include <conio.h>

using namespace std;

#pragma comment(lib, "wsock32.lib")


#include "main.h"

#define nomessage			0
#define quit				1
#define invalid_command		2
#define invalid_hostname	3

#define nomode				0
#define accountmode			1
#define faccountmode		2
#define charmode			3

#define m_line_length		80


void p_accinfo();
short getfreeacc();
short getfreechar();



char				*buffer = new char[1024];
AccountTool			AccTool;
CToolsClass			*C = new CToolsClass;
ItemSearch			ISearch;
CharacterTool		CharTool;

CTimer *TIMER = new CTimer;



NetworkingClass *N = new NetworkingClass;


AccountToEdit acc[100];
bool acc_used[100];
CharacterToEdit Char[100];
bool char_used[100];
int last_recv;
int i;
int towhere;
int requestwhat;


int main()
{

	printf("Bunnkia tools 1.3\n");
	printf("================\n");

	printf("sizeof(CharacterToEdit) = %d\n", sizeof(CharacterToEdit));

	memset(&char_used, 0, sizeof(char_used));


	bool Quit = false;
	short maincommand = 0;
	bool comm = false;;

	C->lst_received = -1;
	C->editaccount = -1;
	C->connected = false;

	char txt[80];
	int handler;
	handler = 0;
	memset(&txt, 0, sizeof(txt));
	int a;

	while (!Quit)
	{
		N->check_connection();

		Sleep(1);

	//===========================================	
		comm = false;
		if (_kbhit())
		{
			a = _getch();
			if (a == 13)// || getch() == 32)
				comm = true;
			else if (a == 32)
				comm = true;
		}
		if (comm)
		{
			C->newcommand();
//			if (C->editingmode == nomode)
			if (C->editingmode == accountmode)
			cout << acc[C->editaccount].AccName << ".acc # ";
			else if (C->editingmode == charmode)
			cout << Char[C->editc].Name << ".char # ";
			else
			cout << "# ";

			memset(&C->command, 0, sizeof(C->command));
			cin.getline(C->command_1, 80, '\n');

			if (strlen(C->command_1) > 0)
			{
					
		//		printf("-> %s\n", C->command);
	
		//		maincommand = C->parse_command();

				C->parse_command();
			
				maincommand = C->execute_sub_command();
				C->execute_command(maincommand);
			}
		}
	}

	if (C) delete C;

	buffer[0] = 125;
	buffer[1] = 3;
	send(N->socket, buffer, 2, 0);

	if (N) delete N;	
	return 1;
}

//==============================================================================
//
//	Read commands
//
//==============================================================================
int CToolsClass::parse_command()
{
	char com_1[80];
	memset(&com_1, 0, sizeof(com_1));

	int i;
	for (i = parsed_to; i < 80; i++)
	{
		if (command_1[i] == NULL) return 0;

		if (command_1[i] != 32)
			command[lst_command][i-parsed_to] = command_1[i];
		if (command_1[i] == 32)
		{
			parsed_to = i+1;
			com_len[lst_command] = strlen(command[lst_command]);
			lst_command++;			
		}
	}
	
	return 1;
}
/*
static char *functions[] =
{
	"connect",			// 0
	"auth",				// 1
	""
};


int CToolsClass::parse_command()
{
	char *func;

	short parsed_to;

	// First of all parse the function call
	
	char *ptr;
	ptr = &command_1[0];

	while (ptr != '(')
	{
		command[1][
		
		ptr++;
	}
}

*/


//==============================================================================
//
//	Execute sub-commands
//
//==============================================================================
int CToolsClass::execute_sub_command()
{
	//------------------------------------------------------------------------------
	//	quit
	//------------------------------------------------------------------------------
	if (!strcmp("quit", command[0]))
	{
		return quit;
	}

	//------------------------------------------------------------------------------
	//	help
	//------------------------------------------------------------------------------
	if (!strcmp("help", command[0]) || !strcmp("?", command[0]))
	{
		p_help();
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	auth
	//------------------------------------------------------------------------------
	if (!strcmp("auth", command[0]))
	{
		char name[20] = "strawberry", pass[20] = "cookies";

		memset(&name, 0, sizeof(name));
		memset(&pass, 0, sizeof(pass));
		strcpy_s(name, command[1]);
		strcpy_s(pass, command[2]);
		printf("auth: %s %s\n", name, pass);

		buffer[0] = 125;
		buffer[1] = 11;
		memcpy(&buffer[2], &name, sizeof(name));
		memcpy(&buffer[2 + sizeof(name)], &pass, sizeof(pass));

		int temp = send(N->socket, buffer, 2 + sizeof(name) + sizeof(pass), 0);
        printf("datasent: %d", temp);

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	connect
	//------------------------------------------------------------------------------
	if (!strcmp("connect", command[0]))
	{
		char connect_to[16];
		strcpy_s(connect_to, command[1]);

		if (!N->connect_to(connect_to))
			printf("Failed to connect to %s\n", command[1]);
		else
		{
			printf("connected to %s in port 1337\n", command[1]);
			C->connected = true;
		}

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	disconnect
	//------------------------------------------------------------------------------
	if (!strcmp("disconnect", command[0]))
	{
		if (!C->connected) 
		{
			printf("unable to execute this command, there's no connection to server!\n");
			return nomessage;
		}
		buffer[0] = 125;
		buffer[1] = 3;
		send(N->socket, buffer, 2, 0);
		printf("you have disconnected from the server\n");
		return nomessage;
	}

	if (!C->authed)
	{
		printf("please auth!\n");
		printf("write: auth yourname yourpass\n");
		return nomessage;
	}

	if (editingmode == charmode)
	{
		CharTool.check_command();

        return nomessage;
	}

	if (editingmode == accountmode)
	{
		AccTool.check_command();

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	start_timer [BROKEN?]
	//------------------------------------------------------------------------------
	if (!strcmp("start_timer", command[0]))
	{
		TIMER->Start();
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	timer
	//------------------------------------------------------------------------------
	if (!strcmp("timer", command[0]))
	{
		TIMER->GetTime();
		printf("Timer: %s\n", TIMER->time_);
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	invalid command
	//------------------------------------------------------------------------------
	if (!strcmp("invalid command", command[0]))
	{
		return invalid_command;
	}

	//------------------------------------------------------------------------------
	//	servermessage
	//------------------------------------------------------------------------------
	if (!strcmp("servermessage", command[0]))
	{
		char srvrmsg[80];
		memset(&srvrmsg, 0, sizeof(srvrmsg));
		cout << "enter servermessage # ";
		cin.getline(srvrmsg, 80, '\n');
		buffer[0] = 125;
		buffer[1] = 10;
		memcpy(&buffer[2], &srvrmsg, sizeof(srvrmsg));
		send(N->socket, buffer, 2 + sizeof(srvrmsg), 0);
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	forcedsave
	//------------------------------------------------------------------------------
	if (!strcmp("forcedsave", command[0]))
	{
		buffer[0] = 125;
		buffer[1] = 8;
		send(N->socket, buffer, 2, 0);
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	notools!
	//------------------------------------------------------------------------------
	if (!strcmp("notools!", command[0]))
	{
		buffer[0] = 125;
		buffer[1] = 7;
		send(N->socket, buffer, 2, 0);
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	serverstatus
	//------------------------------------------------------------------------------
	if (!strcmp("serverstatus", command[0]))
	{
		if (!C->connected) 
		{
			printf("unable to execute this command, there's no connection to server!\n");
			return nomessage;
		}
		//ask infopacket from server

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	listaccounts
	//------------------------------------------------------------------------------
	if (!strcmp("listaccounts", command[0]))
	{
		printf("==== Bunnkia tools == accounts received from the server ========================\n");
		printf("edit ID: account name\n");

		short counter = 0;
		for(int i = 0; i < 100; i++)
		{
			if (acc[i].ID > 0)
			{
				printf("%d: %s\n", i, acc[i].AccName);
				counter++;
			}
		}
		if (counter == 0)
			printf("no received accounts in memory!\n");
		else
			printf("end of list\n");
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	listcharacters
	//------------------------------------------------------------------------------
	if (!strcmp("listcharacters", command[0]))
	{
		printf("==== Bunnkia tools == characters received from the server ========================\n");
		printf("edit ID: character name\n");

		short counter = 0;
		for(int i = 0; i < 100; i++)
		{
			if (Char[i].accid > 0)
			{
				printf("%d: %s\n", i, Char[i].Name);
				counter++;
			}
		}
		if (counter == 0)
			printf("no received characters in memory!\n");
		else
			printf("end of list\n");
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	NULL
	//------------------------------------------------------------------------------
	if (!strcmp("", command[0]))
	{
		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	request
	//------------------------------------------------------------------------------
	if (!strcmp("request", command[0]))
	{
		if (!C->connected) 
			{
				printf("unable to execute this command, there's no connection to server!\n");
				return nomessage;
			}
			//request account
			char tosend[25];
			memset(&tosend, 0, sizeof(tosend));
			for (i = 0; i < towhere; i++)
			{
				if (command[1][i] == '_')
					tosend[i] = ' ';
				else
					tosend[i] = command[1][i];
			}

			printf("requesting character %s from the server\n", tosend);

			buffer[0] = 125;
			buffer[1] = 1;
			buffer[2] = 4;
			memcpy(&buffer[3], &tosend, sizeof(tosend));
			send(N->socket, buffer, 3 + sizeof(tosend), 0);

		if (requestwhat == 1)
		{			
			if (!C->connected) 
			{
				printf("unable to execute this command, there's no connection to server!\n");
				return nomessage;
			}
			//request account
			char tosend[25];
			memset(&tosend, 0, sizeof(tosend));
			for (i = 0; i < towhere; i++)
			{
				if (command[1][i] == '_')
					tosend[i] = ' ';
				else
					tosend[i] = command[1][i];
			}

			printf("requesting account %s from the server\n", tosend);

			buffer[0] = 125;
			buffer[1] = 1;
			buffer[2] = 1;
			memcpy(&buffer[3], &tosend, sizeof(tosend));
			send(N->socket, buffer, 3 + sizeof(tosend), 0);
		}
		if (requestwhat == 2)
		{
			// request char
		}
		if (requestwhat == 3)
		{
			// request guild
			if (!strcmp("guilds.sav", command[1]))
			{

			}
		}
		if (requestwhat == 4)
		{
			if (!C->connected) 
			{
				printf("unable to execute this command, there's no connection to server!\n");
				return nomessage;
			}
			//request account
			char tosend[25];
			memset(&tosend, 0, sizeof(tosend));
			for (i = 0; i < towhere; i++)
			{
				if (command[1][i] == '_')
					tosend[i] = ' ';
				else
					tosend[i] = command[1][i];
			}

			printf("requesting acccount info %s from the server\cn", tosend);

			buffer[0] = 125;
			buffer[1] = 1;
			buffer[2] = 6;
			memcpy(&buffer[3], &tosend, sizeof(tosend));
			send(N->socket, buffer, 3 + sizeof(tosend), 0);
		}

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	edit
	//------------------------------------------------------------------------------
	if (!strcmp("edit", command[0]))
	{
		if (command[1][0] != NULL)
		{			
			editaccount = atoi(command[1]);
			if (acc[editaccount].ID == 0) 
			{
				editaccount = 0;
				printf("account was not found\n");
				return nomessage;
			}
			editingmode = 1;
			lst_received = -1;
			printf("editing account %s\n", acc[editaccount].AccName);
			printf("type accinfo to display account info.\n");
		}
		else
		{
			if (C->lst_received == -1)
			{
				printf("no account received after last edit\n");
				return nomessage;
			}
			editaccount = lst_received;
			editingmode = 1;
			lst_received = -1;
			printf("editing account %s\n", acc[editaccount].AccName);
			printf("type accinfo to display account info.\n");
		}

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	cedit
	//------------------------------------------------------------------------------
	if (!strcmp("cedit", command[0]))
	{
		if (command[1][0] != NULL)
		{			
			editc = atoi(command[1]);
			if (Char[editc].accid == 0) 
			{
				editc = 0;
				printf("character was not found\n");
				return nomessage;
			}
			editingmode = charmode;
			lst_receivedc = -1;
			printf("editing character %s\n", Char[editc].Name);
			printf("type charinfo to display character info.\n");
		}
		else
		{
			if (C->lst_receivedc == -1)
			{
				printf("no characters received after last edit\n");
				return nomessage;
			}
            editc = lst_receivedc;
			editaccount = lst_receivedc;
			editingmode = charmode;
			lst_receivedc = -1;
			printf("editing character %s\n", Char[editc].Name);
			printf("type charinfo to display account info.\n");
		}

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	sitem - Search Item
	//------------------------------------------------------------------------------
	if (!strcmp("sitem", command[0]))
	{
		short type, id;
		
		type = atoi(command[1]);
		id = atoi(command[2]);
		ISearch.type = type;
		ISearch.id = id;

		buffer[0] = 125;
		buffer[1] = 12;
		memcpy(&buffer[2], &type, sizeof(type));
		memcpy(&buffer[2 + sizeof(type)], &id, sizeof(id));
		send(N->socket, buffer, 2 + sizeof(type) + sizeof(id), 0);

		return nomessage;
	}

	//------------------------------------------------------------------------------
	//	results
	//------------------------------------------------------------------------------
	if (!strcmp("results", command[0]))
	{
		ISearch.p_results();
		return nomessage;
	}
	
	return invalid_command;
}


//==============================================================================
//
//	Execute commands
//
//==============================================================================
int CToolsClass::execute_command(short command)
{
	if (command == quit)
	{
		delete C;
		delete N;
		exit(1);
	}
	if (command == invalid_command)
	{
		printf("Invalid command: \"%s\"\n", C->command);		
	}
	if (command == invalid_hostname)
	{
		printf("Invalid hostname: \"%s\"\n", C->command[1]);
	}
	
	return 0;
}


bool NetworkingClass::connect_to(char Addr[16])
{
	if(WSAStartup(MAKEWORD(2, 2), &N->wsaData) != 0)
	{
		return false;
	}

	N->h = gethostbyname(Addr);

	if (N->h == NULL) 
	{
		printf("h = gethostbyname() FAILED.\n");
		return false;
	}

	N->socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if(N->socket < 0)
	{
		printf("socket = socket() FAILED.\n");
		return false;
	}

	N->SOCKADDR_IN.sin_family = AF_INET;
	N->SOCKADDR_IN.sin_port = htons(1337);
	N->SOCKADDR_IN.sin_addr = *((struct in_addr *)N->h->h_addr);
	memset(&N->SOCKADDR_IN.sin_zero, 0, 8);

	if (connect(N->socket, (struct sockaddr *)&N->SOCKADDR_IN, sizeof(struct sockaddr)) == -1)
	{
		WSACleanup();
		printf("connect() FAILED;\n");
		return false;
	}

	buffer[0] = 125;
	buffer[1] = 2;
	send(N->socket, buffer, 2, 0);

	return true;
}


void NetworkingClass::check_connection()
{
	fd_set input_set;
	int s, nfds;

	timeval timeout; 

	timeout.tv_sec=0; // Used for Timeout
	timeout.tv_usec=0;

    FD_ZERO(&input_set);
	
	FD_SET(N->socket, &input_set);
	nfds = N->socket;

	s = select(NULL+1,&input_set,NULL,NULL,&timeout);

	if (s > 0)
	{
		if (FD_ISSET(N->socket, &input_set))
		{
			N->get_message();
		}
	}
}

void NetworkingClass::get_message()
{
	buffer[0] = 0;
	buffer[1] = 0;
	recv(N->socket, buffer, 1, 0);

    switch (buffer[0])
	{
	case 1: // 

		recv(N->socket, buffer, 1, 0);
		printf("received 2 bytes from the server\n");

		break;
	case 2: // Account info received.

		short n;
		n = recv(N->socket, buffer, sizeof(AccountToEdit), 0);
		printf("received %d bytes from the server\n", n + 1);
		

		short ID;
		ID = getfreeacc();

		memcpy(&acc[ID], buffer, sizeof(acc[ID]));

		printf("Received account %s info, type \"edit\" to edit it.\n", acc[ID].AccName);
		
		C->lst_received = ID;

		break;
	case 3: // auth

		C->authed = true;
		printf("received 1 byte(s) from the server\n");
		printf("succesfully authed\n");

		break;
	case 4: // char received

		n = recv(N->socket, buffer, sizeof(CharacterToEdit), 0);
		printf("received %d bytes from the server\n", n + 1);
		
		ID = getfreechar();

		printf("ID = %d\n", ID);
		memcpy(&Char[ID], buffer, sizeof(CharacterToEdit));

		printf("Received character %s info, type \"cedit\" to edit it.\n", Char[ID].Name);
		
		C->lst_receivedc = ID;

		break;

	case 5: // Search item command results.

		recv(N->socket, buffer, 1, 0);

		switch (buffer[0])
		{
		case 1: // Search started.

			cout << "Server: Requested search started." << endl;

			break;

		case 2: // Search stopped

			cout << "Server: Search finished, found " << ISearch.results << " matches." << endl;

			break;

		case 3: // New search result.

			ItemSearchResult *New;
			New = new ItemSearchResult;

			recv(N->socket, buffer, 1, 0); // Info in where the char has the item, bank, backpack or weared.

			recv(N->socket, buffer, sizeof(New->name), 0);
			memcpy(&New->name, buffer, sizeof(New->name));

			recv(N->socket, buffer, sizeof(New->accname), 0);
			memcpy(&New->accname, buffer, sizeof(New->accname));

			if (ISearch.list == NULL)
			{
				New->next = NULL;
				ISearch.list = New;
			}
			else
			{
				New->next = ISearch.list;
				ISearch.list = New;
			}
			ISearch.results++;

			break;		
		}



		break;

	case 6:

		short acclen, c1len, c2len;

		recv(N->socket, buffer, 1, 0);
		acclen = buffer[0];
		recv(N->socket, buffer, 1, 0);
		c1len = buffer[0];
		recv(N->socket, buffer, 1, 0);
		c2len = buffer[0];


		printf("Account info:\n");
		char tmpstring[40];
		recv(N->socket, buffer, acclen, 0);
		memcpy(&tmpstring, buffer, acclen);
		printf("Account name: %s\n", tmpstring);

		recv(N->socket, buffer, c1len, 0);
		memcpy(&tmpstring, buffer, c1len);
		printf("Char 1 name: %s\n", tmpstring);

		recv(N->socket, buffer, c2len, 0);
		memcpy(&tmpstring, buffer, c2len);
		printf("Char 2 name: %s\n", tmpstring);

		break;

	default:

		break;
	
	}
}


void p_accinfo()
{
	printf("Account info: %s\n", acc[C->editaccount].AccName);
	printf("================================================================================\n");	
	printf("General info:\n");
	printf("\n");
	printf("Account name   : %s\n", acc[C->editaccount].AccName);
	printf("Account pass   : %s\n", acc[C->editaccount].AccPass);
	printf("Account e-mail : %s\n", acc[C->editaccount].Email);
	printf("Account ID     : %d\n", acc[C->editaccount].ID);	
	printf("\n");
	if(acc[C->editaccount].IsBanned)
		printf("[x] Account banned\n");
	else
		printf("[ ] Account banned\n");

	if(acc[C->editaccount].IsPremium)
		printf("[x] Premium account\n");
	else
		printf("[ ] Premium account\n");

	if(acc[C->editaccount].IsGolden)
		printf("[x] Gold account\n");
	else
		printf("[ ] Gold account\n");
	printf("\n");
}

void p_help()
{
	printf("================================================================================\n");
	printf("Bunnkia tools help\n");
	printf("================================================================================\n");
	printf("quit           -       Quit the Program                                         \n");
	printf("help           -       This Help Menu                                           \n");
	printf("connect        -       Connect to Server                                        \n");
	printf("auth           -       Authorize                                                \n");
	printf("disconnect     -       Disconnect from Server                                   \n");
	printf("start_timer    -       Start a timer                                            \n");
	printf("timer          -       ???????                                                  \n");
	printf("servermessage  -       Send a message to Server                                 \n");
	printf("forcedsave     -       Force a save                                             \n");
	printf("notools        -       ???????                                                  \n");
	printf("serverstatus   -       Get the status of Server                                 \n");
	printf("listaccounts   -       List accounts (current?)                                 \n");
	printf("request        -       Requests characters from Server                          \n");
	printf("edit           -       ???????                                                  \n");
	printf("cedit          -       ???????                                                  \n");
	printf("sitem          -       ???????                                                  \n");
	printf("results        -       ???????                                                  \n");
	printf("================================================================================\n");
}


short getfreechar()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		if (!char_used[i])
		{
			char_used[i] = true;
			return i;
		}
	}
	return -1;
}	

short getfreeacc()
{
	int i;
	for (i = 0; i < 100; i++)
	{
		if (!acc_used[i])
		{
			acc_used[i] = true;
			return i;
		}
	}
	return -1;
}	


void p_status()
{
	printf("=== Bunnkia tools == status ==========================");
	if (C->connected)
	{
		printf("Connected to server\n");
	}

}

void CTimer::Start()
{
//	conn_time = SDL_GetTicks();
	GetTime();
}


void CTimer::GetTime()
{
	unsigned long tmptime = NULL;
	unsigned long tmpleft = NULL;
//	tmptime = SDL_GetTicks() - conn_time;

	short secs, mins, hours;
	char sec[2], min[2], hour[2];

	hours = tmptime / 3600000;
	tmpleft = tmptime % 3600000;

	mins = tmpleft / 60;
	tmpleft -= mins * 60;
		
	secs = tmpleft;

	if (secs < 10) sprintf_s(sec, "0%d", secs);
	else sprintf_s(sec, "%d", secs);
	if (mins < 10) sprintf_s(min, "0%d", mins);
	else sprintf_s(min, "%d", mins);
	if (hours < 10) sprintf_s(hour, "0%d", hours);
	else sprintf_s(hour, "%d", hours);

	sprintf_s(time_, "%d:%d:%d", hour, min, sec);
}


void p_charinfo()
{
	printf("character info: %s\n", Char[C->editc].Name);
	printf("=============================================================\n\n");
	printf("Name             : %s\n", Char[C->editc].Name);
	printf("Dev. mode        : %d\n", Char[C->editc].DevMode);
	printf("Exp              : %lu\n", Char[C->editc].Exp);
	printf("Level            : %d\n", Char[C->editc].Level);
	printf("Stat points      : %d\n", Char[C->editc].StatPoints);
	printf("Avatar           : %d\n", Char[C->editc].Avatar);
	printf("HP               : %d\n", Char[C->editc].HPLeft);
	printf("Race             : %d\n", Char[C->editc].Race);
	printf("X, Y             : %d, %d\n", Char[C->editc].X, Char[C->editc].Y);
	printf("ShrineX, ShrineY : %d, %d\n", Char[C->editc].ShrineX, Char[C->editc].ShrineY);
	printf("Reputation       : %d\n", Char[C->editc].Reputation);
	printf("Rep. points      : %lu\n", Char[C->editc].KilledMonsters);
	printf("Crim points      : %lu\n", Char[C->editc].CrimCount);
	printf("Str = %d, Dex = %d, Con = %d, Int = %d, Lck = %d, Cha = %d\n", Char[C->editc].Str, Char[C->editc].Dex, Char[C->editc].Con, Char[C->editc].Int, Char[C->editc].Lck, Char[C->editc].Cha);
}


void ItemSearch::p_results()
{
	if (list == NULL) return;
	cout << "Results of item search for item: type [" << type << "], id [" << id << "]." << endl;
	
	ItemSearchResult *tmp;
	tmp = list;

	while (tmp)
	{
		cout << "Found: " << tmp->name << " has the item. Account name: " << tmp->accname << ".acc" << endl;
	
		tmp = tmp->next;
	}

	while (list)
	{
		tmp = list->next;
		delete list;
		list = tmp;
	}
	list = NULL;
}
