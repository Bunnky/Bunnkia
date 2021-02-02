#ifndef players_h
#define players_h


#include "main.h"


//------------------------------------------------------------------
//
class PlayerNode
{
public:

	PlayerNode *next, *prev;
	class PlayerDatabase *database;

	bool wanted, isinguild;

	char name[25], tag[4];

	short avatar, reputation, x, y, hpmax, hpleft, race;

	long id;

	void addto();
	void removefrom();


	//=========================
	short frame;
	unsigned long frametime;

	void anim_start();
	void anim_advance();
	//=========================
};
extern PlayerNode *players;
//------------------------------------------------------------------





//------------------------------------------------------------------
class PlayerDatabase
{
public:

	PlayerNode *players;
	PlayerNode *own;
	PlayerNode *pl;

	PlayerNode *getbyid(short id);
	PlayerNode *getlast();
	void clearlist();
	void drawpl();
	void drawnames();
};
extern PlayerDatabase *pldatabase;
//------------------------------------------------------------------

#endif