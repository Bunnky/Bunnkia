#ifndef body_h
#define body_h

#include "main.h"


//------------------------------------------------------------------
//
class BodyClass
{
public:
	BodyClass *next, *prev;
	class BodyDatabase *database;

	short decaystate, id, x, y, realid;

	bool looted;
	char lootername[25];
	char ownername[25];

	ItemClass Items[84];

	void addto();
	void removefrom();
};
//------------------------------------------------------------------



//------------------------------------------------------------------
//
class BodyDatabase
{
public:

	BodyClass *body;
	BodyClass *bd;


	BodyClass *getbyid(short id);
	BodyClass *getlast();
	
	void drawbody();
	void clearlist();	
};
extern BodyDatabase *bodydatabase;
//------------------------------------------------------------------




#endif