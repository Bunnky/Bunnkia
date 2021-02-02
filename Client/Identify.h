#ifndef identify_h
#define identify_h

#include "main.h"


extern short idxy[10][2];


class identify
{
public:
	identify *next;
	identify *prev;

	identify();
	~identify();

	class identifydatabase *database;

	char name[25], tag[5];
	short reputation;

	short x, y;

	long id;
	
	ItemDatabase *items;
	

	void draw();
	void mouseover();
	bool ison();
	
	void addto();
	void remove();
	void removefrom();
	void removeitems();
	
	void makelast();
};



class identifydatabase
{
public:

	identify *list;

	void clearlist();
	identify *getlast();
	identify *getfirst();

	identify *ison();
	void draw();
	void mouseover();
	bool mouseclick();
	bool rightclick();
	identify *getbyid(long id);
};

extern identifydatabase *idatabase;



#endif