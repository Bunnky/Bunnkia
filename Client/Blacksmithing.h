
#ifndef blacksmithing_h
#define blacksmithing_h


#include "main.h"



class bs_itemlist
{
public:
	bs_itemlist *next;
	bs_itemlist *prev;
	class BlackSmithingClass *base;

	short type, id, family;	

	void addto();
	void removefrom();
};


//------------------------------------------------------------------------
// Blacksmithing class
class BlackSmithingClass
{
public:

	BlackSmithingClass();
	~BlackSmithingClass();

	buttoncontrol *buttons;

	bool on;
	short type, family;
	short oltype, olfamily, olid;
	short itype, skill;
	short scroll;
	bs_itemlist *selected;

	ItemClass *target_ingots;
	bs_itemlist *items;
	
	void init(ItemClass *item);
	void uninit();
	void draw();
	void mouseclick();

	void clearlist();
	void sort();
	void sort_weapon();
	void sort_armor();

	void forge();

	void ItemInfo(short infotype, short fam, short type, short id, short x, short y, short col);
};

extern BlackSmithingClass *BS;
//------------------------------------------------------------------------




#endif