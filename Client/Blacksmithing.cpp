//------------------------------------------------------//
//   Blacksmithing source file ( blacksmithing.cpp )	//
//														//
//   Includes functions of blacksmithing class			//
//						   ( class BlackSmithingClass ) //
//														//
//   (c) Jussi Witick									//
//------------------------------------------------------//


#include "main.h"

const char *familys[] =
{
	"null",
	"Weapons",
	"Armors",
	"Collectables",
	"Useables"
};

const char *types[] =
{
	"null",
	"Swords",
	"Axes",
	"Blunts",
	"Polearms",
	"",
	"",
	"",
	"",
	"",
	"",
	"Plates",
	"Helms",
	"Shields",
	"Leggings",
	"Gaunts",
	"Rings",
	"Amulets"
};

void BlackSmithingClass::mouseclick()
{
	buttons->mouseclick();

	int tmpcol = col_gray;
	for (int i = 0; i < 2; i++)
	{
		if (MX >= 435 + i * 60 && MX <= 435 + BFont_TextWidth(familys[i + 1]) + i * 60 && MY >= 70 && MY <= 80)
		{
			family = i + 1;
			sort();
			return;
		}
	}

	int xoff = 0;
	for (int i = (family == FAMILY_WEAPON ? 0 : 10); i < (family == FAMILY_WEAPON ? 5 : 17); i++)
	{
		if (MX >= 435 + xoff && MX <= 435 + BFont_TextWidth(types[i + 1]) + xoff && MY >= 90 && MY <= 100)
		{
			type = i + 1;
			sort();
			return;
		}

		xoff += BFont_TextWidth(types[i + 1]) + 10;
	}

	short skip = 0;
	bs_itemlist *tmp;
	tmp = items;
	short counter = 0;
	while (tmp)
	{
		if (skip >= scroll)
		{			
			if (MX >= 440 && MX <= 780 && MY >= 130 + counter * 30 && MY <= 150 + counter * 30)
			{
				selected = tmp;
				return;
			}

			counter++;
		}
		else skip++;
	
		if (counter > 12) tmp = NULL;
		else tmp = tmp->next;
	}

	if (MX >= 440 && MX <= 440 + BFont_TextWidth("Forge") && MY >= 550 && MY <= 560 && selected != NULL)
		forge();
}

void BlackSmithingClass::forge()
{
	buffer[0] = 58;
	PACKET_FORGE forge;
	forge.Family = family;
	forge.ID = selected->id;
	forge.Type = type;
	forge.WithItem_ID = target_ingots->KnowID;
	memcpy(&buffer[1], &forge, sizeof(forge));
	send(Socket, buffer, 1 + sizeof(forge), 0);

	on = false;
}


void BlackSmithingClass::draw()
{
	sdl.dst.x = 425;
	sdl.dst.y = 0;
	sdl.dst.h = 600;
	sdl.dst.w = 375;

	SDL_BlitSurface(G.List, NULL, Screen.screen, &sdl.dst);

	buttons->draw();

	int tmpcol = col_gray;
	for (int i = 0; i < 2; i++)
	{
		if (family == i + 1)
			tmpcol = col_grey;
		else if (MX >= 435 + i * 60 && MX <= 435 + BFont_TextWidth(familys[i + 1]) + i * 60 && MY >= 70 && MY <= 80)
			tmpcol = col_grey;
		else tmpcol = col_greenish;
 
		sdl.text(familys[i + 1], 435 + i * 60, 70, tmpcol);
	}

	int xoff = 0;
	for (int i = (family == FAMILY_WEAPON ? 0 : 10); i < (family == FAMILY_WEAPON ? 5 : 17); i++)
	{
		if (type == i + 1)
			tmpcol = col_grey;
		else if (MX >= 435 + xoff && MX <= 435 + BFont_TextWidth(types[i + 1]) + xoff && MY >= 90 && MY <= 100)
			tmpcol = col_grey;
		else tmpcol = col_greenish;

		sdl.text(types[i + 1], 435 + xoff, 90, tmpcol);
		xoff += BFont_TextWidth(types[i + 1]) + 10;
	}


	sdl.text("Blacksmithing menu", 435, 10, col_white);

	sdl.text("Item", 435, 118, col_white);
	sdl.text("Name", 480, 118, col_white);
	sdl.text("Cost", 765, 118, col_white);

	int skip = 0, counter = 0;
	tmpcol = col_gray;
	bs_itemlist *tmp;
	tmp = items;
	while (tmp)
	{
		if (skip >= scroll)
		{			
			sdl.dst.x = 440;
			sdl.dst.y = 130 + counter * 30;
			sdl.dst.h = 20;
			sdl.dst.w = 20;

			DrawItem(tmp->family, tmp->type, tmp->id);


			tmpcol = col_green;
			if ((MX >= 440 && MX <= 780 && MY >= 130 + counter * 30 && MY <= 150 + counter * 30) || selected == tmp)
				tmpcol = col_grey;

			ItemInfo(1, tmp->family, tmp->type, tmp->id, 470, 140 + counter * 30, tmpcol);

			if (tmpcol == col_grey)
				tmpcol = col_purple;
			else tmpcol = col_darkpurple;

			ItemInfo(2, tmp->family, tmp->type, tmp->id, 780, 140 + counter * 30, tmpcol);


			counter++;
		}
		else skip++;
	

		if (counter > 12) tmp = NULL;
		else tmp = tmp->next;
	}

	if (MX >= 440 && MX <= 440 + BFont_TextWidth("Forge") && MY >= 550 && MY <= 560)
		tmpcol = col_grey;
	else tmpcol = col_green;

	sdl.text("Forge", 440, 550, tmpcol);
}

void BlackSmithingClass::ItemInfo(short infotype, short fam, short type, short id, short x, short y, short col)
{
	if (fam == FAMILY_WEAPON)
	{
		switch (infotype)
		{
		case 1: // Name

			sdl.text(Weapons[type][id].Name, x, y, col);

			break;
		case 2:

			short offset;
			char tmp[20];
			sprintf(tmp, "%d", Weapons[type][id].bs_amount);
			offset = BFont_TextWidth(tmp);

			sdl.text(tmp, x - offset, y, col);

			break;
		}
	}

	if (fam == FAMILY_ARMOR)
	{
		switch (infotype)
		{
		case 1:

			sdl.text(Armors[type - 11][id].Name, x, y, col);

			break;
		case 2:

			short offset;
			char tmp[20];
			sprintf(tmp, "%d", Armors[type - 11][id].bs_amount);
			offset = BFont_TextWidth(tmp);

			sdl.text(tmp, x - offset, y, col);

			break;
		}
	}

	if (fam == FAMILY_COLLECTABLE)
	{
		switch (infotype)
		{
		case 1:

			break;
		case 2:

			break;
		}
	}

	if (fam == FAMILY_USEABLE)
	{
		switch (infotype)
		{
		case 1:

			break;
		case 2:

			break;
		}
	}
}


void BlackSmithingClass::sort()
{
	if (family == FAMILY_WEAPON)
		sort_weapon();
	if (family == FAMILY_ARMOR)
		sort_armor();
}

void BlackSmithingClass::sort_weapon()
{
	if (items) clearlist();
	int i;

	bs_itemlist *rndlist = NULL;
	bs_itemlist *bslist = NULL;

	BS->skill = P.Skills[16].Level;

	for (i = 0; i < m_Items_per_type; i++)
	{
		if (Weapons[type][i].bs_amount > 0 && Weapons[type][i].bs_metaltype == target_ingots->ID && BS->skill >= Weapons[type][i].bs_req)
		{
			bs_itemlist *inew = new bs_itemlist;
			inew->family = FAMILY_WEAPON;
			inew->type = type;
			inew->id = i;
			inew->base = BS;

			if (rndlist == NULL)
			{
				inew->prev = NULL;
				inew->next = NULL;
				rndlist = inew;			
			}
			else
			{
				if (Weapons[rndlist->type][rndlist->id].bs_amount > Weapons[inew->type][inew->id].bs_amount)
				{
					if (rndlist->next != NULL)
						rndlist->next->prev = inew;
					inew->prev = NULL;
					inew->next = rndlist->next;
					rndlist = inew;					
				}
				else
				{
					bslist = rndlist;
					while (bslist)
					{
						if (bslist->next == NULL)
						{
							inew->prev = bslist;
							inew->next = NULL;
							bslist->next = inew;
							bslist = NULL;
						}
						else if (Weapons[bslist->type][bslist->id].bs_amount < Weapons[inew->type][inew->id].bs_amount &&
							Weapons[bslist->next->type][bslist->next->id].bs_amount > Weapons[inew->type][inew->id].bs_amount)
						{
							if (bslist->next != NULL)
								bslist->next->prev = inew;
							inew->next = bslist->next;
							inew->prev = bslist;
							bslist->next = inew;
							bslist = NULL;
						}
				
					
						if (bslist != NULL) bslist = bslist->next;
					}
				}
			}
		}
	}
	items = rndlist;
}

void BlackSmithingClass::sort_armor()
{
	if (items) clearlist();

	int i;

	bs_itemlist *rndlist = NULL;
	bs_itemlist *bslist = NULL;

	for (i = 0; i < m_Items_per_type; i++)
	{
		if (Armors[type-11][i].bs_amount > 0 && Armors[type-11][i].bs_metaltype == target_ingots->ID && BS->skill >= Armors[type - 11][i].bs_req)
		{
			bs_itemlist *inew = new bs_itemlist;
			inew->family = FAMILY_ARMOR;
			inew->type = type;
			inew->id = i;
			inew->base = BS;

			if (rndlist == NULL)
			{
				inew->prev = NULL;
				inew->next = NULL;
				rndlist = inew;			
			}
			else
			{
				if (Armors[rndlist->type-11][rndlist->id].bs_amount > Armors[inew->type-11][inew->id].bs_amount)
				{
					if (rndlist->next != NULL)
						rndlist->next->prev = inew;
					inew->prev = NULL;
					inew->next = rndlist->next;
					rndlist = inew;					
				}
				else
				{
					bslist = rndlist;
					while (bslist)
					{
						if (bslist->next == NULL)
						{
							inew->prev = bslist;
							inew->next = NULL;
							bslist->next = inew;
							bslist = NULL;
						}
						else if (Armors[bslist->type-11][bslist->id].bs_amount < Armors[inew->type-11][inew->id].bs_amount &&
							Armors[bslist->next->type-11][bslist->next->id].bs_amount > Armors[inew->type-11][inew->id].bs_amount)
						{
							if (bslist->next != NULL)
								bslist->next->prev = inew;
							inew->next = bslist->next;
							inew->prev = bslist;
							bslist->next = inew;
							bslist = NULL;
						}
				
					
						if (bslist != NULL) bslist = bslist->next;
					}
				}				
			}
		}
	}
	items = rndlist;
}


void BlackSmithingClass::init(ItemClass *item)
{
	on = true;
	BS->family = FAMILY_WEAPON;
	BS->type = TYPE_SWORD;
	BS->items = NULL;
	BS->target_ingots = item;
	BS->skill = P.Skills[16].Level;
	scroll = 0;
	selected = 0;

	sort();
}




void BlackSmithingClass::clearlist()
{
	bs_itemlist *tmp;
	tmp = items;
	while (items)
	{
		tmp = items->next;
		delete items;
		items = tmp;
	}
}


//===============================================
void bs_itemlist::addto()
{
	if (base->items == NULL)
	{
		next = NULL;
		prev = NULL;
		base->items = this;
	}
	else
	{
		next = base->items;
		prev = NULL;
		base->items->prev = this;
		base->items = this;			
	}
}
//===============================================


//===============================================
void bs_itemlist::removefrom()
{
	if (base->items == this)
	{
		if (base->items->next != NULL)
			base->items->next->prev = NULL;

		base->items = base->items->next;
	}
	else
	{
		if (next != NULL)
			next->prev = prev;
		if (prev != NULL)
			prev->next = next;
	}
	
	delete this;
}
//===============================================

BlackSmithingClass::BlackSmithingClass()
{
	buttons = new buttoncontrol;

	selected = 0;

	button *n;
	n = new button(buttons, 774, 89, 15, 15, secure_sup_inactive, 16);
	n->addto();
	n = NULL;
	n = new button(buttons, 774, 540, 15, 15, secure_sdown_inactive, 17);
	n->addto();
}


BlackSmithingClass::~BlackSmithingClass()
{

}
