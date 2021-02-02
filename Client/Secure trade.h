#ifndef secure_trade_h
#define secure_trade_h

#include "main.h"


class secure_itemlist
{
public:
	class SecureTradeClass *base;

	secure_itemlist *next;
	secure_itemlist *prev;

	ItemClass *tar;
	bool inlist;

	void addto();
	void remove();
};


class SecureTradeClass
{
public:

	SecureTradeClass();
	~SecureTradeClass();

	bool	on;
	bool	locks[2];

	bool	tradepending;
	unsigned long petime;

	char	tradersname[25];

	secure_itemlist *ownitems;
	short	scroll;

	buttoncontrol *buttons;

	secure_itemlist *offer;
	secure_itemlist *get;


	void	accept();


	void	trade_pend();

	void	draw();
	void	c_oitems();	
	void	d_ownitems();
	void	d_offeritems();
	void	d_mouseover(ItemClass *Item);

	void	init();
	void	clear();

	void	mouseclick();

	void	informserver_offertrade(long id);
	void	informserver_accepttrade();

	void	inform_newoffer(short knowid);
	void	inform_removeoffer(short knowid);
	void	add_hisoffer(short family, short type, short id, unsigned long amount, short knowid);
	void	remove_hisoffer(short knowid);
	void	add_ownoffer(short knowid);
	void	remove_ownoffer(short knowid);
	void	tradeabort();
	void	create_buttons();
};

extern SecureTradeClass *st;





#endif