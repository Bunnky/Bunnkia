#ifndef main_h
#define main_h

#include "character.h"
#include "account.h"


void p_charinfo();
void p_help();

void p_accinfo();
void p_status();


extern char *buffer;


class CTimer
{
public:

	unsigned long conn_time;
	char time_[10];

	
	void GetTime();
	void Start();
};


class NetworkingClass
{
public:

	struct sockaddr_in SOCKADDR_IN;
	struct hostent *h;
	SOCKET socket;
    WSADATA wsaData;

	void check_connection();
	void get_message();
	bool connect_to(char Addr[16]);
};

class CToolsClass
{
public:

	bool connected;
	bool authed;

	char command[3][80];
	char command_1[80];
	short com_len[3];
	short lst_command;
	short subcommand;
	int parsed_to;

	int editingmode;
	int editaccount;
	int lst_received;
	int lst_receivedc;
	int editc;
	int lst_receivedchar;

	int parse_command();
	int execute_command(short command);
	int execute_sub_command();
	void newcommand();
};
extern CToolsClass *C;


struct ItemSearchResult
{
	ItemSearchResult *next;
	char name[25];
	char accname[20];
};


class ItemSearch
{
public:

	short type, id;

	long results;
	ItemSearchResult *list;

	void p_results();

};

extern ItemSearch ISearch;


#endif