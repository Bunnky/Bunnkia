#include "main.h"

short idxy[10][2] = 
{
	{ 87, 70  },
	{ 49, 58  },
	{ 49, 19  },
	{ 11, 70  },
	{ 49, 118 },
	{ 11, 31  },
	{ 11, 106 },
	{ 87, 31  },
	{ 87, 106 },
};


void identify::draw()
{
	sdl.set_rect(x, y, 119, 150); 
	SDL_BlitSurface(G.Identify, NULL, Screen.screen, &sdl.dst);


	char x1[30];
	sprintf(x1, "æ%s Å%s", tag, name);
	sdl.text(x1, x + 60 - BFont_TextWidth(x1)/2, y + 5, reputation);

	ItemClass *item;
	item = items->items;

	sdl.dst.h = 20; sdl.dst.w = 20;

	while (item) 
	{
		sdl.dst.x = (item->Type < TYPE_ARMOR ? idxy[0][0] : idxy[ item->Type - 10 ][0] ) + x;
		sdl.dst.y = (item->Type < TYPE_ARMOR ? idxy[0][1] : idxy[ item->Type - 10 ][1] ) + y;

		DrawItem(item->Family, item->Type, item->ID);

		if (item) item = item->next;
	}
	mouseover();
}

identify::identify()
{
	items = new ItemDatabase;
	items->items = NULL;
}

identify::~identify()
{
	items->clearlist();
	delete items;
	items = NULL;
}

void identify::removeitems()
{
/*	ItemClass *tmp;
	while (items)
	{
		tmp = items->next;
		delete items;
		items = tmp;
	}
	items = NULL; */
}

void identify::makelast()
{
	removefrom();
	addto();
}


void identify::mouseover()
{
	ItemClass *item;
	item = items->items;

	short type;

	while (item)
	{
		type = item->Type;
		if ( (MX >= idxy[type < TYPE_ARMOR ? 0 : type - 10][0] + x		) &&
			 (MX <= idxy[type < TYPE_ARMOR ? 0 : type - 10][0] + x + 20  ) && 
			 (MY >= idxy[type < TYPE_ARMOR ? 0 : type - 10][1] + y		) &&
			 (MY <= idxy[type < TYPE_ARMOR ? 0 : type - 10][1] + y + 20  ))
		{
			int len;
			Uint32 Color = SDL_MapRGB(Screen.screen->format, 0, 0, 0);
			len = BFont_TextWidth( (item->Family == FAMILY_WEAPON ? Weapons[ item->Type ][ item->ID ].Name : Armors[ item->Type - 11 ][ item->ID ].Name) );

//			sdl.set_rect(x + 50 - len / 2 - 1 > 800 ? x + 50 - len / 2 - 1 : 800 - len, x + 20, 13, len + 2);
//			sdl.dst = { (MX - len / 2 - 1 > 800 ? MX - len / 2 - 1 : 800 - len), MY + 20, 13, len + 2 };
			
//			SDL_FillRect(Screen.screen, &sdl.dst, Color);

			sdl.text( (item->Family == FAMILY_WEAPON ? Weapons[ item->Type ][ item->ID ].Name : Armors[ item->Type - 11 ][ item->ID ].Name), x + 60- len / 2, sdl.dst.y + 70, col_white);

			item = NULL;
		}

		if (item) item = item->next;
	}
}


bool identify::ison()
{
	short tx, ty, h, w;
	tx = MX - x;
	ty = MY - y;
	h = G.Identify->h;
	w = G.Identify->w;

	return ( tx > 0 && ty > 0 && tx < w && ty < h ? true : false );
}


identify *identifydatabase::ison()
{
	identify *tmp;
	tmp = list;
	while (tmp)
	{
		if (tmp->ison())
		{
			return tmp;		
		}

		if (tmp) tmp = tmp->next;	
	}
	return NULL;
}


void identify::remove()
{
	if (database->list == this)
	{
		database->list = database->list->next;
		if (database->list != NULL)
			database->list->prev = NULL;
	}
	else
	{
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;
	}
	delete this;
}

void identify::removefrom()
{
	if (database->list == this)
	{
		database->list = database->list->next;
		if (database->list != NULL)
		database->list->prev = NULL;
	}
	else
	{
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;
	}
}

void identify::addto()
{
	if (database->list == NULL)
	{
		prev = NULL;
		next = NULL;
		database->list = this;
	}
	else
	{
		identify *last;
		last = database->getlast();
		next = NULL;
		prev = last;
		last->next = this;
	}
}

bool identifydatabase::mouseclick()
{
	identify *last, *tmp;
	last = getlast();
	tmp = last;

	while (tmp)
	{
		if (tmp != last)
		{
			tmp->remove();
//			tmp->makelast();
			return true;
		}
		
		if (tmp) tmp = tmp->prev;
	}
	return false;
}

bool identifydatabase::rightclick()
{
	identify *ids;
	ids = getlast();
	while (ids)
	{
		if (ids->ison())
		{
			ids->remove();			
			return true;
		}
	
		if (ids) ids = ids->prev;
	}

	return false;
}

void identifydatabase::draw()
{
	identify *ids;
	ids = list;
	while (ids)
	{
		ids->draw();
		ids = ids->next;
	}
}

void identifydatabase::clearlist()
{
	identify *tmp;
	while (list)
	{
		tmp = list->next;
		delete list;
		list = tmp;
	}
}

identify *identifydatabase::getbyid(long id)
{
	identify *ids;
	ids = list;
	while (ids)
	{
		if (ids->id == id)
			return ids;

		if (ids) ids = ids->next;
	}
	return NULL;
}


void identifydatabase::mouseover()
{
	identify *ids;
	ids = getlast();
	while (ids)
	{
		if (ids->ison())
		{
			ids->mouseover();
			ids = NULL;
		}
		
		if (ids) ids = ids->prev;
	}
}

identify *identifydatabase::getlast()
{
	identify *tmp;
	tmp = list;
	while (tmp)
	{
		if (tmp->next == NULL)
			return tmp;
		
		if (tmp) tmp = tmp->next;
	}
	return NULL;
}

identify *identifydatabase::getfirst()
{
	return list;
}
