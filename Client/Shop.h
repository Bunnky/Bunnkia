
#ifndef shop_h
#define shop_h


#include "main.h"


#define m_storeitems		100


#define shopmode_sell				1
#define shopmode_buy				2
#define shopmode_sortby_price		100
#define shopmode_sortby_none		101
#define shopmode_nopending			200
#define shopmode_pendingguildbuy	201
#define shopmode_pendingamount		202



struct shopitem
{	
	bool in_use;

	short type, family, id, realid;

	long quantity;
};


class shop_itemlist
{
public:
	shop_itemlist *next;
	shop_itemlist *prev;
	shop_itemlist *inlist;
	
	shopitem *item;
	short idinlist;
	unsigned long quantity;

	void removefrom();

	void addto(shop_itemlist *after);
	void addto();
};



class shopclass
{
public:

	bool on, pending;	

	char tmpl[120];
	char txt[50];

	short sellmode, s_scroll, b_scroll, pending_mode, list_sortedby, idinlist;

	unsigned long o_money, s_cost;

	shopitem items[m_storeitems];
	shopitem bitems[MaxBackpackItems];

	shop_itemlist *l_items;
	shop_itemlist *i_items;

	void init();
	void uninit();
	void initstate();
	void draw();
	//============================
	void emptylist();
	void createsell();
	void createlist();
	void clist_sell();
	void clist_buy();
	void clist_create_byprice();
	void clist_create_nosort();	
	//============================
	unsigned long getprice(shopitem *item);
	unsigned long getmoney();
	unsigned long getcost();

	bool iscurrency(ItemClass *item);

	char *getname(shopitem *item);

	void mouseclick();
	void keybpress();
};


extern shopclass shop;
//---------------------------------------------------------------------------



#endif
