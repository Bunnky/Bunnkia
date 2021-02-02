#include "main.h"

#pragma warning(disable:4244)
#pragma warning(disable:4018)

void textedit::keypress(short key)
{
	if (checkbox) { database->focus = NULL; return; }

	if (key == VK_TAB)
	{
		database->setfocus(linkID);
		return;
	}

	short len;
	len = strlen(txt);

	if (len >= str_mlen) return;

	if (key == 8)
		txt[len - 1 >= 0 ? len - 1 : 0] = 0;
	else txt[len] = key;
}



void texteditdatabase::setfocus(long id)
{
	textedit *tmp;
	tmp = list;
	while (tmp)
	{
		if (tmp->ID == id)
		{
			focus = tmp;
			return;
		}
	
		if (tmp) tmp = tmp->next;
	}
	focus = NULL;
	return;
}

void textedit::setlink(long id)
{
	linkID = id;
}


void textedit::draw()
{
	if (checkbox)
	{
		sdl.text(title, x + 25, y + 2, col_white);
		sdl.draw(G.addgf, x, y, &sdl.Rect_addgf[addgf_checkbox]);

		bool t;
		memcpy(&t, btar, sizeof(bool));
		if (t) sdl.text("X", x + 3, y + 1, col_white);

		return;
	}
	//////////////////////////////////////////////////////////
	//   NO NEED TO DRAW THE TEXT BOX @ Account Creation
	//sdl.draw(G.addgf, x, y, &sdl.Rect_addgf[addgf_textedit]);

	sdl.text(title, x - BFont_TextWidth(title) - 5, y, col_white);

	
	if (this == database->focus)
	{
		char *txt2;
		txt2 = new char[strlen(txt) + 2];
		memset(txt2, 0, strlen(txt) + 2);

		if (ispass)
		{
			for (int i = 0; i < strlen(txt); i++)
				txt2[i] = '*';
			txt2[strlen(txt)]	= '|';
			txt2[strlen(txt)+1] = 0;
		}
		else sprintf(txt2, "%s|", txt);

		sdl.text(txt2, x + 2, y, col_grey);

		delete txt2;
	}
	else
	{
		char *txt2;
		txt2 = new char[strlen(txt) + 1];
		memset(txt2, 0, strlen(txt) + 1);

		if (ispass)
		{
			for (int i = 0; i < strlen(txt); i++)
				txt2[i] = '*';
			txt2[strlen(txt)] = 0;
		}
		else sprintf(txt2, "%s", txt);

		sdl.text(txt2, x + 2, y, col_greenish);

		delete txt2;
	}
}


bool textedit::ison(short x2, short y2)
{
	short tx, ty;
	tx = x2 - x;
	ty = y2 - y;
	return (tx > 0 && tx < w && ty > 0 && ty < h ? true : false);
}

void textedit::checked()
{
	if (!checkbox) return;

	bool tmp;
	memcpy(&tmp, btar, sizeof(bool));
	
	tmp = !tmp;

	memcpy(btar, &tmp, sizeof(bool));
}


void textedit::addto() 
{
	textedit *t;
	t = database->list;
	if (t != NULL)
	{
		while (t->next != NULL)
			t = t->next;

		prev = t;
		next = NULL;
		t->next = this;
	}
	else
	{
		next = NULL;
		prev = NULL;
		database->list = this;
	}
}

/*void textedit::addtofirst() 
{
	prev = NULL;
	if (database->list == NULL)
	{
		next = NULL;
		database->list = this;
	}
	else
	{
		next = database->list;
		database->list->prev = this;
		database->list = this;	
	}
}*/


void textedit::removefrom()
{
	if (database->list == this)
	{		
		database->list = database->list->next;
		database->list->prev = NULL;
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


void texteditdatabase::clearlist()
{
	textedit *tmp;
	while (list)
	{
		tmp = list->next;
		delete list;
		list = tmp;
	}
	list = NULL;
}


void texteditdatabase::mouseclick(short x, short y)
{
	textedit *te;
	te = list;
	focus = NULL;
	while (te)
	{
		if (te->checkbox && te->ison(x, y))
			te->checked();

		else if (!te->checkbox && te->ison(x, y))
			focus = te;
	
		te = te->next;
	}
}


void texteditdatabase::draw()
{
	textedit *te;
	te = list;
	while (te)
	{
		te->draw();		
		te = te->next;	
	}
}


textedit::textedit(class texteditdatabase *_database, short _x, short _y, short _h, short _w, char *_title, bool no_letters, bool no_numbers, short limit_len, bool check_box, bool is_pass, long id)
{
	database = _database;
	x = _x;
	y = _y;
	h = _h;
	w = _w;
	title = new char[strlen(_title) + 1];
	memset(title, 0, strlen(_title) + 1);
	strcpy(title, _title);

	memset(&txt, 0, sizeof(txt));

	nletters = no_letters;
	nnumbers = no_numbers;
	checkbox = check_box;
	str_mlen = limit_len;
	ispass	 = is_pass;
	ttype = 0;
	ID = id;
}


textedit::~textedit()
{
	delete title;
}

void texteditdatabase::update()
{
	switch (focus->ttype)
	{
	case tbool:

		int hep;
		bool jep;
		hep = atoi(focus->txt);
		if (hep != 0) jep = true;
		else jep = false;
		
		memcpy(focus->btar, &jep, sizeof(jep));

		break;

	case tchar:
	case tuchar:

		memset(focus->ctar, 0, strlen(focus->ctar));
		memcpy(focus->ctar, &focus->txt, strlen(focus->txt));

		break;


	case tshort:

		hep = atoi(focus->txt);
		short jep2;
		jep2 = hep;
		memcpy(focus->star, &jep2, sizeof(jep2));

		break;

	case tushort:

		hep = atoi(focus->txt);
		unsigned short jep3;
		jep3 = hep;
		memcpy(focus->ustar, &jep3, sizeof(jep3));
		break;

	case tlong:

		hep = atoi(focus->txt);
		long jep4;
		jep4 = hep;
		memcpy(focus->ltar, &jep4, sizeof(jep4));
		break;
		
	case tulong:

		hep = atoi(focus->txt);
		unsigned long jep5;
		jep5 = hep;
		memcpy(focus->ultar, &jep5, sizeof(jep5));

		break;
	}
}

void textedit::target(short type, void *target)
{
	ttype = type;

	memset(&txt, 0, sizeof(txt));

	switch (type)
	{
	case tbool:
		btar = (bool *)target;
		
		bool b;
		memcpy(&b, btar, sizeof(b));

		sprintf(txt, "%d", b);

		break;
	case tchar:
		ctar = (char *)target;
		int len;
			len = strlen(ctar);

		memcpy(&txt, ctar, len);

		break;
	case tuchar:
		uctar = (unsigned char *)target;

		len = strlen((char *)uctar);

		memcpy(&txt, uctar, len);

		break;
	case tshort:
		star = (short *)target;

		short s;
		memcpy(&s, star, sizeof(s));

		sprintf(txt, "%d", s);

		break;
	case tushort:
		ustar = (unsigned short *)target;

		unsigned short us;
		memcpy(&us, ustar, sizeof(us));

		sprintf(txt, "%d", us);

		break;
	case tlong:
		ltar = (long *)target;

		long l;
		memcpy(&l, ltar, sizeof(l));

		sprintf(txt, "%d", l);

		break;
	case tulong:
		ultar = (unsigned long *)target;

		unsigned long ul;
		memcpy(&ul, ultar, sizeof(ul));

		sprintf(txt, "%d", ul);

		break;
	}
}


texteditdatabase::texteditdatabase()
{
	list = NULL;
	focus = NULL;
}


texteditdatabase::~texteditdatabase()
{
	clearlist();
}


void texteditdatabase::NewTextedit(char *title, short x, short y, short w, short h, bool nletters, bool nnumbers, short limitlen, bool checkbox, bool ispass, long id, long linkid)
{
	textedit *New;
	New = new textedit(this, x, y, h, w, title, nletters, nnumbers, limitlen, checkbox, ispass, id);
	New->setlink(linkid);
	New->addto();
}

textedit* texteditdatabase::getbyid(long id)
{
	textedit *tmp;
	tmp = list;
	while (tmp)
	{
		if (tmp->ID == id)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}