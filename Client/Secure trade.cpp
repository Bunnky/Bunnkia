#include "main.h"



void SecureTradeClass::accept()
{
	buffer[0] = 54;
	buffer[1] = !locks[0];
	send(Socket, buffer, 2, 0);
}

void SecureTradeClass::informserver_offertrade(long ID)
{
	buffer[0] = 54;
	buffer[1] = 3;
	memcpy(&buffer[2], &ID, sizeof(ID));
	send(Socket, buffer, 2 + sizeof(ID), 0);
}

void SecureTradeClass::informserver_accepttrade()
{
	buffer[0] = 54;
	buffer[1] = 4;
	send(Socket, buffer, 2, 0);
}

/*inline bool SecureTradeClass::s_lock(short which)
{
	return (locks[which] = !locks[which]);
}*/


void secure_itemlist::addto()
{
	if (base->ownitems == NULL)
	{
		prev = NULL;
		next = NULL;
		base->ownitems = this;
	}
	else
	{
		next = base->ownitems;
		prev = NULL;
		base->ownitems->prev = this;
		base->ownitems = this;
	}
}

void secure_itemlist::remove()
{
	if (base->ownitems == this)
	{
		base->ownitems = base->ownitems->next;
		base->ownitems->prev = NULL;
	}
	else
	{
		if (prev != NULL) prev->next = next;
		if (next != NULL) next->prev = prev;
	}
	delete this;
}


void SecureTradeClass::add_ownoffer(short knowid)
{
	secure_itemlist *ni;
	ni = new secure_itemlist;

	for (int i = 0; i < MaxBackpackItems; i++)
		if (iBackpack[i].KnowID == knowid && iBackpack[i].Used)
			ni->tar = &iBackpack[i];

	if (offer == NULL)
	{
		ni->prev = NULL;
		ni->next = NULL;
		offer = ni;
	}
	else
	{
		ni->prev = NULL;
		ni->next = offer;
		offer->prev = ni;
		offer = ni;
	}

	secure_itemlist *ni2;
	ni2 = ownitems;
	while (ni2)
	{
		if (ni2->tar == ni->tar) 
		{
			ni2->inlist = false;
			ni2 = NULL;
		}
	
		if (ni2) ni2 = ni2->next;
	}
}


void SecureTradeClass::remove_ownoffer(short knowid)
{
	secure_itemlist *tmp;
	tmp = offer;

	while (tmp)
	{
		if (tmp->tar->KnowID == knowid)
		{
			secure_itemlist *ni;
			ni = ownitems;
			
			while (ni)
			{
				if (ni->tar == tmp->tar) 
				{
					ni->inlist = true;
					ni = NULL;
				}
				
				if (ni) ni = ni->next;
			}

			if (offer == tmp)
			{
				offer = offer->next;
				offer->prev = NULL;
			}
			else
			{
				if (tmp->prev != NULL)
					tmp->prev->next = tmp->next;
				if (tmp->next != NULL)
					tmp->next->prev = tmp->prev;
			}

			delete tmp;

			tmp = NULL;		
		}	
		
		if (tmp) tmp = tmp->next;
	}
}


void SecureTradeClass::remove_hisoffer(short knowid)
{
	secure_itemlist *tmp;
	tmp = get;

	while (tmp)
	{
		if (tmp->tar->KnowID == knowid)
		{
			delete tmp->tar;
			
			if (tmp == get)
			{
				get = get->next;
			}
			else
			{
				if (tmp->prev != NULL)
					tmp->prev->next = tmp->next;
				if (tmp->next != NULL)
					tmp->next->prev = tmp->prev;
			}

			delete tmp;
			tmp = NULL;
		}
		
		if (tmp) tmp = tmp->next;
	}	
}


void SecureTradeClass::add_hisoffer(short family, short type, short id, unsigned long amount, short knowid)
{
	secure_itemlist *newitem;
	newitem = new secure_itemlist;

	ItemClass *ni;
	ni = new ItemClass;
	ni->Family = family;
	ni->ID = id;
	ni->Type = type;
	ni->Quantity = amount;
	ni->KnowID = knowid;
	newitem->tar = ni;
	
	if (get == NULL)
	{
		newitem->next = NULL;
		newitem->prev = NULL;
		get = newitem;
	}
	else
	{
		newitem->next = get;
		newitem->prev = NULL;
		get->prev = newitem;
		get = newitem;	
	}
}


void SecureTradeClass::inform_newoffer(short knowid)
{
	buffer[0] = 54;
	buffer[1] = 5;
	memcpy(&buffer[2], &knowid, sizeof(knowid));
	send(Socket, buffer, 2 + sizeof(knowid), 0);
}

void SecureTradeClass::inform_removeoffer(short knowid)
{
	buffer[0] = 54;
	buffer[1] = 6;
	memcpy(&buffer[2], &knowid, sizeof(knowid));
	send(Socket, buffer, 2 + sizeof(knowid), 0);
}


void SecureTradeClass::mouseclick()
{
	buttons->mouseclick();

	scroll = 0;


	//=======================================
	// Check for player's backpack list.
	if (MX >= 452 && MX <= 513 &&
		MY >= 36 && MY <= 224)
	{
		//=======================================
		// Check for what item was truly pressed.
		short pressed = -1;
		for (int i = 0; i < 12; i++)
		{
			if (MX >= 456 + ((i % 2) * 33) &&
				MX <= 476 + ((i % 2) * 33) &&
				MY >= 40 + ((i / 2) * 33) &&
				MY <= 60 + ((i / 2) * 33))
			{
				pressed = i;
				i = 12;
			}
		}
		if (pressed == -1) return;

		//----------------------------------------

		short count = 0, tcount = 0;
		secure_itemlist *tmp;
		tmp = ownitems;
		while (tmp)
		{
			if (tmp->inlist)
			{
				if (count < scroll)
					count++;
				else
				{
					if (tcount == pressed)
					{
						inform_newoffer(tmp->tar->KnowID);
						
						tmp = NULL;
					}
					tcount++;
				}		
			}
			if (tmp) tmp = tmp->next;
		}
		//=======================================
			
		return;
	}
	//=======================================

	//=======================================
	// Check for players offerlist
/*	if (MX >= 623 && MX <= 738 &&
		MY >= 68 && MY <= 128)
	{
		//=======================================
		// Check for what item was truly pressed.
		short pressed = -1;
		for (int i = 0; i < 8; i++)
		{
			if (MX >= 623 + (i % 4) * 33 && MX <= 652 + (i % 4) * 33 &&
				MY >= 68 + (i >> 2) * 33 && MY <= 96 + (i >> 2) * 33)
			{
				pressed = i;
				i = 8;
			}
		}	
		if (pressed == -1) return;

		//---------------------------------------

		secure_itemlist *tmp;
		tmp = offer;
		short count = 0;

		while (tmp)
		{
			if (count == pressed)
			{
				inform_removeoffer(tmp->tar->KnowID);
				tmp = NULL;
			}

			count++;
			if (tmp) tmp = tmp->next;
		}
	}*/
	//=======================================
}


void SecureTradeClass::init()
{
	memset(&locks, 0, sizeof(locks));
	scroll = 0;
	
	clear();

	on = true;

	c_oitems();	
}


SecureTradeClass::SecureTradeClass()
{
	ownitems = NULL;
	buttons = new buttoncontrol;
	buttons->buttons = NULL;
	create_buttons();
	offer = NULL;
	get = NULL;
	on = false;
	memset(&locks, 0, sizeof(locks));
	tradepending = false;
	scroll = 0;
}

SecureTradeClass::~SecureTradeClass()
{
	delete buttons;
}



void SecureTradeClass::draw()
{
	//=======================================
	// Draw the background image.
	SDL_Rect dst;
	dst.x = 428;
	dst.y = 0;
	dst.h = sdl.Rect_addgf[ secure_bg ].h;
	dst.w = sdl.Rect_addgf[ secure_bg ].w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_bg ], Screen.screen, &dst);
	//=======================================

	sdl.text("You offer", 655, 50, col_grey);

	char str[50];
	sprintf(str, "%s offers", tradersname);

	sdl.text(str, 715 - BFont_TextWidth(tradersname), 145, col_grey);


	//=======================================
	// Draw the items opponent is offering.
	d_offeritems();
	//=======================================

	//=======================================
	// Draw own offered items, and own items.
	d_ownitems();
	//=======================================

	//=======================================
	// Draw secure trade buttons
	buttons->draw();
	//=======================================

	//=======================================
	// Draw mouseover textbox
//	d_mouseover();
	//=======================================	
}

void SecureTradeClass::d_ownitems()
{
	//=======================================
	// Draw the items we are offering first.
	secure_itemlist *tmp;
	tmp = offer;
	short count = 0;
	while (tmp)
	{		
//		sdl.dst.x = 456 + ((count % 2) * 33);
//		sdl.dst.y = 40 + ((count >> 2) * 33);
//		sdl.dst.h = 20;
//		sdl.dst.w = 20;

		sdl.dst.x = 628 + ((count % 4) * 33);
		sdl.dst.y = 72 + ((count >> 2) * 33);
		sdl.dst.h = 20;
		sdl.dst.w = 20;

		DrawItem(tmp->tar->Family, tmp->tar->Type, tmp->tar->ID);

		if (MX >= 628 + ((count % 4) * 33) && MX <= 648 + ((count %4) * 33) &&
			MY >= 72 + ((count >> 2) * 33) && MY <= 92 + ((count >> 2) * 33))
			d_mouseover(tmp->tar);
		
		tmp = tmp->next;
		count++;
	}

	if (locks[0])
	{
		sdl.dst.x = 623;
		sdl.dst.y = 68;
		sdl.dst.w = sdl.Rect_addgf[ secure_offerlock ].w;
		sdl.dst.h = sdl.Rect_addgf[ secure_offerlock ].h;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_offerlock ], Screen.screen, &sdl.dst);
	}
	//=======================================

	//=======================================
	// Draw own backpack items to the list.
	count = 0;
	short skip = 0;
	tmp = ownitems;
	while (tmp)
	{
		if (tmp->inlist)
		{
			if (skip < scroll)
				skip++;
			else
			{
				sdl.dst.x = 456 + ((count % 2) * 33);
				sdl.dst.y = 40 + ((count / 2) * 33);
				sdl.dst.h = 20;
				sdl.dst.w = 20;

//				sdl.dst.x = 767;
//				sdl.dst.y = 28 + count * 22;
//				sdl.dst.w = 20;
//				sdl.dst.h = 20;
				DrawItem(tmp->tar->Family, tmp->tar->Type, tmp->tar->ID);		

				count++;

				if (count == 12) tmp = NULL;
			}
		}		
		if (tmp) tmp = tmp->next;
	}
	//=======================================
}

void SecureTradeClass::d_offeritems()
{
	//=======================================
	// Draw the items opponent is offering.
	secure_itemlist *tmp;
	tmp = get;
	short count = 0;
	while (tmp)
	{
		sdl.dst.x = 628 + ((count % 4) * 33);
		sdl.dst.y = 168 + ((count >> 2) * 33);
		sdl.dst.h = 20;
		sdl.dst.w = 20;
		DrawItem(tmp->tar->Family, tmp->tar->Type, tmp->tar->ID);

		if (MX >= 628 + ((count % 4) * 33) && MX <= 648 + ((count %4) * 33) &&
			MY >= 168 + ((count >> 2) * 33) && MY <= 188 + ((count >> 2) * 33))
			d_mouseover(tmp->tar);
		
		tmp = tmp->next;
		count++;
	}

	if (locks[1])
	{
		sdl.dst.x = 624;
		sdl.dst.y = 164;
		sdl.dst.w = sdl.Rect_addgf[ secure_offerlock ].w;
		sdl.dst.h = sdl.Rect_addgf[ secure_offerlock ].h;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_offerlock ], Screen.screen, &sdl.dst);
	}
	//=======================================	
}


void SecureTradeClass::tradeabort()
{
	buffer[0] = 54;
	buffer[1] = 7;
	send(Socket, buffer, 2, 0);
}


void SecureTradeClass::c_oitems()
{
	//==========================
	// Clear the itemlist first. 
	secure_itemlist *tmp;
	
	while (ownitems)
	{
		tmp = ownitems->next;

		delete ownitems;
		
		ownitems = tmp;
	}
	//==========================

	for (int i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			secure_itemlist *newitem;
			newitem = new secure_itemlist;

			newitem->tar = &iBackpack[i];
			newitem->inlist = true;
			newitem->base = this;
			newitem->addto();						
		}	
	}
}

void SecureTradeClass::clear()
{
	//==========================
	// Clear the itemlists
	secure_itemlist *tmp;
	
	while (ownitems)
	{
		tmp = ownitems->next;

		delete ownitems;
		
		ownitems = tmp;
	}

	while (offer)
	{
		tmp = offer->next;
		delete offer->tar;
		delete offer;
		offer = tmp;
	}

	while (get)
	{
		tmp = get->next;
		delete get;
		get = tmp;
	}
	//==========================

	on = false;
}

void SecureTradeClass::create_buttons()
{//button(class buttoncontrol *base2, short x2, short y2, short h, short w, short pic2, short action2);
	button *n;
	n = new button(buttons, 770, 13, 15, 15, secure_bexit_inactive, 1); // exitbox
	n->addto();
	n = NULL;
	n = new button(buttons, 472, 19, 15, 15, secure_sup_inactive, 2); // scroll up
	n->addto();
	n = NULL;
	n = new button(buttons, 472, 229, 15, 15, secure_sdown_inactive, 3); // scroll down
	n->addto();
	n = NULL;
	n = new button(buttons, 770, 225, 15, 15, secure_lock_inactive, 4); // lock
	n->addto();

}

void SecureTradeClass::d_mouseover(ItemClass *Item)
{
	int len;
	char str[120];
	memset(&str, 0, sizeof(str));

	switch (Item->Family)
	{
	case FAMILY_WEAPON:

		sprintf(str, "%s", Weapons[ Item->Type ][ Item->ID ].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 600 - len / 2, 234, col_white);

		break;

	case FAMILY_ARMOR:

		sprintf(str, "%s", Armors[ Item->Type - 11 ][ Item->ID ].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 600 - len / 2, 234, col_white);

		break;

	case FAMILY_COLLECTABLE:

		char str[120];
		
		sprintf(str, "(Ä%luÅ) %ss", Item->Quantity, Collectables[Item->ID].Name);
		
		len = BFont_TextWidth( str );
				
		sdl.text( str, 600 - len / 2, 234, col_white);

		break;

	case FAMILY_USEABLE:

		sprintf(str, "%s", Useables[Item->ID].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 600 - len / 2, 234, col_white);

		break;
	}
}

