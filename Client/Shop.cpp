//------------------------------------------------------//
//   Shopping  source file ( Shop.cpp )					//
//														//
//   Includes functions of Shop class					//
//						   ( class ShopClass )			//
//														//
//   (c) Jussi Witick									//
//------------------------------------------------------//


#include "main.h"


//--------------------------------------------------------------------------
//


void shopclass::draw()
{
	if (!shop.on)
	{
		if (shop.pending)
		{
			if (shop.pending_mode == shopmode_pendingguildbuy)
			{
				sdl.dst.x = 400 - sdl.Rect_addgf[ YESNOBOX ].w / 2;
				sdl.dst.y = 300 - sdl.Rect_addgf[ YESNOBOX ].h / 2;
				sdl.dst.h = sdl.Rect_addgf[ YESNOBOX ].h;
				sdl.dst.w = sdl.Rect_addgf[ YESNOBOX ].w;

				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[YESNOBOX], Screen.screen, &sdl.dst);

				sdl.text("Do you want to start a new guild?", 400 - BFont_TextWidth("Do you want to start a new guild?") / 2, 280, col_grey);

				char x1[25];
				strcpy(x1, "Cost: Ä500,000Ågp");
				sdl.text(x1, 400 - BFont_TextWidth(x1) / 2, 292, col_white);

				int tmpcol;

				if (MX >= 370 && MX <= 370 + BFont_TextWidth("Yes") && MY >= 320 && MY <= 332)
					tmpcol = col_grey;
				else tmpcol = col_greenish;

				sdl.text("Yes", 370, 320, tmpcol);

				if (MX >= 430 && MX <= 430 + BFont_TextWidth("No") && MY >= 320 && MY <= 332)
					tmpcol = col_red;
				else tmpcol = col_darkred;

				sdl.text("No", 430, 320, tmpcol);
			}		
		}
		return;
	}


	sdl.draw(G.shop, 427, 0);

	if (sellmode == shopmode_buy)
		sdl.text("Buy", 602 - BFont_TextWidth("Buy") / 2, 45, col_grey);
	if (sellmode == shopmode_sell)
		sdl.text("Sell", 602 - BFont_TextWidth("Sell") / 2, 45, col_grey);

		
	shop_itemlist *tmpi;
	tmpi = l_items;	

	short counter = 0, realcounter = 0, tmpcol = col_green;
	bool fail;

	while (tmpi && realcounter < 10)
	{
		tmpcol = col_greenish;
		fail = false;
		if (counter < b_scroll) fail = true;

/*		if (sellmode == shopmode_sell)
		{
			tmpii = i_items;
			while (tmpii)
			{
				if (tmpii->idinlist == tmpi->idinlist) 
				{
					fail = true;
					tmpii = NULL;
				}
				if (tmpii != NULL) tmpii = tmpii->next;
			}
		}*/
		
		if (!fail) 
		{
			sdl.dst.x = 454;
			sdl.dst.y = 109 + realcounter * 44;
			sdl.dst.h = 20;
			sdl.dst.w = 20;
			DrawItem(tmpi->item->family, tmpi->item->type, tmpi->item->id);
		

			if (MX >= 450 && MX <= 780 && MY >= 105 + realcounter * 44 && MY <= 135 + realcounter * 44)
				tmpcol = col_grey;

			if (sellmode == shopmode_sell && tmpi->item->family == fam_col)
			{
				sprintf(tmpl, "%s (Ä%luÅ)", getname(tmpi->item), tmpi->item->quantity);
				sdl.text(tmpl, 500, 115 + counter * 44, tmpcol);
			}
			else
			{
				sdl.text(getname(tmpi->item), 500, 115 + realcounter * 44, tmpcol);
			}
			sprintf(tmpl, "%lu", getprice(tmpi->item));
			sdl.text(tmpl, 710, 115 + realcounter * 44, col_purple);

			realcounter++;
		}
		counter++;
		tmpi = tmpi->next;
	}

	counter = 0;
	tmpi = shop.i_items;
	while (tmpi && counter < 8)
	{

		sdl.dst.x = 483 + counter * 34;
		sdl.dst.y = 563;
		DrawItem(tmpi->item->family, tmpi->item->type, tmpi->item->id);

		if (tmpi->item->family == fam_col)
		{
//			memset(&tmpl, 0, sizeof(tmpl));
			sprintf(tmpl, "(Ä%luÅ)", tmpi->quantity);
			sdl.text(tmpl, 483 + counter * 34/* - BFont_TextWidth(tmpl)/2*/, 553, col_white);
		}
		
		tmpi = tmpi->next;
		counter++;
	}





	sprintf(tmpl, "Your money: Ä%lu", o_money);
	sdl.text(tmpl, 647, 541, col_white);

	if (sellmode == shopmode_sell)
		sprintf(tmpl, "Sell for: Ä%lu", s_cost);
	else sprintf(tmpl, "Cost: Ä%lu", s_cost);
	sdl.text(tmpl, 482, 541, col_white);

	if (pending && pending_mode == shopmode_pendingamount)
	{
		sdl.dst.x = 400 - sdl.Rect_addgf[ SPLIT ].w / 2;
		sdl.dst.y = 300 - sdl.Rect_addgf[ SPLIT ].h / 2;
		sdl.dst.w = sdl.Rect_addgf[ SPLIT ].w;
		sdl.dst.h = sdl.Rect_addgf[ SPLIT ].h;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ SPLIT ], Screen.screen, &sdl.dst);
        
		sprintf(tmpl, "%sÉ|", txt);
		sdl.text(tmpl, 400 - BFont_TextWidth(txt)/2, 294, col_grey);
	}
}



unsigned long shopclass::getcost()
{
	if (i_items == NULL) return (s_cost = 0);

	shop_itemlist *tmp;
	tmp = i_items;
	unsigned long tmpmoney = 0;

	while (tmp)
	{
		tmp->item->quantity = tmp->quantity;
		tmpmoney += getprice(tmp->item);
		tmp->item->quantity = 0;
	
		tmp = tmp->next;
	}
	return (s_cost = tmpmoney);
}



unsigned long shopclass::getmoney()
{
	unsigned long tmpmoney = 0;
	for (int i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			if (iscurrency(&iBackpack[i]))
				tmpmoney += iBackpack[i].Quantity;
		}
	}
	return (o_money = tmpmoney);
}

bool shopclass::iscurrency(ItemClass *item)
{
	if (item->Type == TYPE_COLLECTABLE &&
		item->Family == fam_col &&
		item->ID == 0) return true;

	if (item->Family == FAMILY_WEAPON)
	{
		if (Weapons[item->Type][item->ID].Level == 100)
			return true;
	}

	if (item->Family == FAMILY_ARMOR)
	{
		if (Armors[item->Type-11][item->ID].Level == 100)
			return true;
	}

	return false;
}



void shopclass::init()
{
	if (l_items != NULL)
		emptylist();
	l_items = NULL;
	pending = false;
	sellmode = shopmode_buy;
	s_scroll = 0;
	b_scroll = 0;
	pending_mode = shopmode_nopending;
	list_sortedby = shopmode_sortby_price;
	o_money = 0;
	s_cost = 0;
	memset(&items, 0, sizeof(items));
	memset(&bitems, 0, sizeof(bitems));

	getmoney();

	for (int i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			bitems[i].family = iBackpack[i].Family;
			bitems[i].id = iBackpack[i].ID;
			bitems[i].in_use = true;
			bitems[i].quantity = iBackpack[i].Quantity;
			bitems[i].realid = i;
			bitems[i].type = iBackpack[i].Type;
		}
	}
}

void shopclass::initstate()
{
	if (l_items != NULL || i_items != NULL)
		emptylist();
	l_items = NULL;
	i_items = NULL;

	s_scroll = 0;
	b_scroll = 0;
	pending_mode = shopmode_nopending;
	list_sortedby = shopmode_sortby_price;
	o_money = 0;
	s_cost = 0;

	memset(&bitems, 0, sizeof(bitems));
	
	for (int i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			bitems[i].family = iBackpack[i].Family;
			bitems[i].id = iBackpack[i].ID;
			bitems[i].in_use = true;
			bitems[i].quantity = iBackpack[i].Quantity;
			bitems[i].realid = iBackpack[i].KnowID;
			bitems[i].type = iBackpack[i].Type;
		}
	}

	createlist();

	getmoney();	
}

void shopclass::uninit()
{
	init();
	on = false;
}

void shopclass::createlist()
{
	if (sellmode == shopmode_sell)
		clist_sell();
	if (sellmode == shopmode_buy)
		clist_buy();
}

void shopclass::clist_sell()
{
	if (l_items)
		emptylist();

	shop_itemlist *tmpi;
	bool skip;
	int i;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			skip = false;
			tmpi = l_items;
			while (tmpi)
			{
				if (i == tmpi->idinlist)
				{
					skip = true;
					tmpi = NULL;
				}

				if (tmpi != NULL) tmpi = tmpi->next;
			}

			if (!skip)
			{
				if (!iscurrency(&iBackpack[i]))
				{
					shop_itemlist *newitem;
					newitem = new shop_itemlist;
					newitem->idinlist = i;
					newitem->next = NULL;
					newitem->prev = NULL;
					newitem->item = &bitems[i];
					newitem->quantity = 0;
					
					shop_itemlist *tmp, *tmp2;
					tmp = l_items;
					tmp2 = tmp;
					
					if (l_items == NULL)
						newitem->addto(NULL);
					else
					{
						while (tmp)
						{							
							if (getprice(&bitems[newitem->idinlist]) <= getprice(&bitems[tmp->idinlist]))
							{
								newitem->addto(tmp2);
								tmp = NULL;
							}
							else if (tmp->next == NULL)
							{
								newitem->addto(tmp);
								tmp = NULL;
							}

							if (tmp != NULL) 
							{
								tmp2 = tmp;
								tmp = tmp->next;
							}
						}
					}
				}
			}
		}
	}
}


void shopclass::clist_buy()
{
	if (list_sortedby == shopmode_sortby_price)
		clist_create_byprice();
//	if (list_sortedby == shopmode_sortby_none) // Let's not use this optioni yet.. Just use sorting algorithm.
//		clist_create_nosort();
}


void shopclass::clist_create_byprice()
{
	if (l_items)
		emptylist();

	shop_itemlist *tmpi;
	bool skip;
	int i;
	for (i = 0; i < m_storeitems /*m_Store_Items*/; i++)
	{
		if (items[i].in_use)
		{
			skip = false;
			tmpi = i_items;
			while (tmpi)
			{
				if (i == tmpi->idinlist)
				{
					skip = true;
					tmpi = NULL;
				}

				if (tmpi != NULL) tmpi = tmpi->next;
			}

			if (!skip)
			{
				if (getprice(&items[i]) > 0)
				{
					shop_itemlist *newitem;
					newitem = new shop_itemlist;
					newitem->idinlist = i;
					newitem->next = NULL;
					newitem->prev = NULL;
					newitem->item = &items[i];
					newitem->quantity = 0;
					
					shop_itemlist *tmp, *tmp2;
					tmp = l_items;
					tmp2 = tmp;
					
					if (l_items == NULL)
						newitem->addto(NULL);
					else
					{
						while (tmp)
						{							
							if (getprice(&items[newitem->idinlist]) <= getprice(&items[tmp->idinlist]))
							{
								newitem->addto(tmp2);
								tmp = NULL;
							}
							else if (tmp->next == NULL)
							{
								newitem->addto(tmp);
								tmp = NULL;
							}

							if (tmp != NULL) 
							{
								tmp2 = tmp;
								tmp = tmp->next;
							}
						}
					}
				}
			}
		}
	}
}

unsigned long shopclass::getprice(shopitem *item)
{
	switch (item->family)
	{
	case fam_weapon:
		if (sellmode == shopmode_buy) return Weapons[item->type][item->id].Value;
		else return Weapons[item->type][item->id].Value / 2;
		break;		
	case fam_armor:
		if (sellmode == shopmode_buy) return Armors[item->type-11][item->id].Value;
		else return Armors[item->type-11][item->id].Value / 2;
		break;		
	case fam_col:
		if (sellmode == shopmode_buy) return (Collectables[item->id].Value * (item->quantity > 0 ? item->quantity : 1));
		else return ((Collectables[item->id].Value / 2) * (item->quantity > 0 ? item->quantity : 1));
		break;		
	case fam_use:
		if (sellmode == shopmode_buy) return Useables[item->id].Value;
		else return Useables[item->id].Value / 2;
		break;		
	}
	return 0;
}

char *shopclass::getname(shopitem *item)
{
	switch (item->family)
	{
	case fam_weapon:
		return Weapons[item->type][item->id].Name;
		break;		
	case fam_armor:
		return Armors[item->type-11][item->id].Name;
		break;		
	case fam_col:
		return Collectables[item->id].Name;
		break;		
	case fam_use:
		return Useables[item->id].Name;
		break;		
	}
	return 0;
}


void shopclass::emptylist()
{
	shop_itemlist *tmp;
	while (i_items != NULL)
	{
		tmp = i_items->next;
		delete this;
		i_items = tmp;
	}
	while (l_items != NULL)
	{
		tmp = l_items->next;
		delete this;
		l_items = tmp;
	}
}



void shop_itemlist::addto()
{
	if (shop.i_items == NULL)
	{
		next = NULL;
		prev = NULL;
		inlist = shop.i_items;
		shop.i_items = this;
	}
	else
	{
		shop.i_items->prev = this;
		next = shop.i_items;
		prev = NULL;
		shop.i_items = this;
		inlist = shop.i_items;
	}
}


void shop_itemlist::addto(shop_itemlist *after)
{
	if (after == NULL)
	{
		if (shop.l_items != NULL)
			next = shop.l_items;
		else next = NULL;
		shop.l_items = this;
		prev = NULL;
	}
	else
	{
		if (after->next != NULL)
		{
			after->next->prev = this;
			next = after->next;		
		}
		else next = NULL;
		after->next = this;
		prev = after;
	}
}

void shop_itemlist::removefrom()//bool fl)
{
//	if (fl)
//	{
		if (shop.i_items == this)
		{
			shop.i_items = shop.i_items->next;
			delete this;
		}
		else
		{
			if (next != NULL)
				next->prev = prev;
			if (prev != NULL)
				prev->next = next;

			delete this;
		}
//	}
/*	else
	{
		if (shop.l_items == this)
		{
			shop.l_items = shop.l_items->next;
			delete this;
		}
		else
		{
			if (next != NULL)
				next->prev = prev;
			if (prev != NULL)
				prev->next = next;
			delete this;
		}
	} */
}


void shopclass::mouseclick()
{
	if (!shop.on)
	{
		if (shop.pending)
		{
			if (shop.pending_mode == shopmode_pendingguildbuy)
			{
				if (MX >= 370 && MX <= 370 + BFont_TextWidth("Yes") && MY >= 320 && MY <= 332)
				{
					shop.pending = false;
					shop.pending_mode = shopmode_nopending;

					buffer[0] = 32;
					send(Socket, buffer, 1, 0);
				}
				if (MX >= 430 && MX <= 430 + BFont_TextWidth("No") && MY >= 320 && MY <= 332)
				{
					shop.pending = false;
					shop.pending_mode = shopmode_nopending;
				}
			}
		}
		return;
	}

	if (MX >= 435 && MX <= 463 && MY >= 563 && MY <= 590)
	{
		// Buy button. *sigh*

		PACKET_BUY newbuy;
		memset(&newbuy.Amount, 0, sizeof(newbuy.Amount));
		memset(&newbuy.ID, -1, sizeof(newbuy.ID));

		shop_itemlist *tmpi;
		tmpi = i_items;
		short counter = 0;

		while (tmpi)
		{
			if (sellmode == shopmode_buy)
				newbuy.ID[counter] = items[tmpi->idinlist].realid;
			else newbuy.ID[counter] = bitems[tmpi->idinlist].realid;

			newbuy.Amount[counter] = tmpi->quantity;
			counter++;
			tmpi = tmpi->next;
		}

		if (sellmode == shopmode_buy)
			buffer[0] = 40;
		else buffer[0] = 41;
		memcpy(&buffer[1], &newbuy, sizeof(newbuy));
		send(Socket, buffer, 1 + sizeof(newbuy), 0);
		shop.uninit();
		return;
	}

	if (MX >= 456 && MX <= 474 && MY >= 78 && MY <= 98)
	{
		b_scroll -= 3;
		if (b_scroll < 0) b_scroll = 0;
		return;
	}

	if (MX >= 456 && MX <= 474 && MY >= 537 && MY <= 557)
	{
		b_scroll += 3;
		if (b_scroll > 96) b_scroll = 96;
		return;
	}

	if (MX >= 560 && MX <= 623 && MY >= 43 && MY <= 60)
	{
		if (sellmode == shopmode_buy) 
			sellmode = shopmode_sell;
		else
			sellmode = shopmode_buy;

		shop.initstate();
		return;
	}

	short press = -1;
	for (int i = 0; i < 10; i++)
	{
		if (MX >= 450 && MX <= 780 && MY >= 105 + i * 44 && MY <= 135 + i * 44)
		{
			press = i;
			i = 10;
		}
	}

	if (press != -1)
	{
		shop_itemlist *tmp;
		tmp = l_items;
		short counter = 0, realcounter = 0, minus = 0;
/*
		if (sellmode == shopmode_sell)
		{
			tmpi = l_items;
			while (tmpi && counter <= b_scroll + press)
			{
				tmp = i_items;
				while (tmp)
				{
					if (tmpi->idinlist == tmp->idinlist)
					{
						minus++;
						tmp = NULL;
					}
					if (tmp) tmp = tmp->next;
				}

				tmpi = tmpi->next;
				counter++;
			}
		}
*/
		counter = 0; realcounter = 0;
		tmp = l_items;
		while (tmp)
		{
			if (realcounter == press)
			{
				shop_itemlist *tmp2;
				tmp2 = i_items;
				short s = 0;

				while (tmp2)
				{
					if (tmp2->idinlist == tmp->idinlist && sellmode == shopmode_sell) s = 8;
					tmp2 = tmp2->next;
					s++;
				}
				if (s >= 8) return;

				if (tmp->item->family == fam_col)
				{
					if (sellmode == shopmode_sell && tmp->item->quantity <= 1)
					{
						tmp2 = new shop_itemlist;
						tmp2->idinlist = tmp->idinlist;
						tmp2->item = tmp->item;
						tmp2->quantity = tmp->item->quantity;
						tmp2->addto();
					}
					else
					{
						shop.pending = true;
						shop.pending_mode = shopmode_pendingamount;
						memset(&txt, 0, sizeof(txt));
						shop.idinlist = tmp->idinlist;
					}
				}
				else
				{
					tmp2 = new shop_itemlist;
					tmp2->idinlist = tmp->idinlist;
					tmp2->item = tmp->item;
					tmp2->quantity = 0;
					tmp2->addto();

//					if (sellmode == shopmode_sell) createsell();
				}
			
				tmp = NULL;
			}			
			
			if (tmp != NULL) tmp = tmp->next;
			
			counter++;

			if (counter > b_scroll)
//				if (minus > 0) minus--;
//					else 
					realcounter++;
		}

		getcost();
		return;
	}

	press = -1;
	for (int i = 0; i < 8; i++)
	{
		if (MX >= 480 + i * 35 && MX <= 510 + i * 35 && MY >= 560 && MY <= 589)
		{
			press = i;
			i = 8;
		}
	}
	if (press != -1)
	{
		shop_itemlist *tmp;
		tmp = shop.i_items;
		short counter = 0;

		while (tmp)
		{
			if (counter == press)
			{
				// Remove this item from the buyinglist.
				tmp->removefrom();
				tmp = NULL;
			}

			if (tmp != NULL) tmp = tmp->next;
			counter++;
		}
	}
	getcost();
	getmoney();
}

void shopclass::createsell()
{
/*	shop_itemlist *tmp;
	tmp = l_items;
	while (tmp)
	{
		tmp = l_items->next;
		delete l_items;
		l_items = tmp;
	}

	for (int i = 0; i < */
}