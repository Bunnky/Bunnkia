#ifndef textedit_h
#define textedit_h


//===========================
// ttypes
#define tbool			1
#define tchar			2
#define tuchar			3
#define tshort			4
#define tushort			5
#define tlong			6
#define tulong			7


class textedit
{
public:

	textedit(class texteditdatabase *database, short _x, short _y, short _h, short _w, char *_title, bool no_letters, bool no_numbers, short limit_len, bool check_box, bool is_pass, long id);

	~textedit();

	textedit *next;
	textedit *prev;
	class texteditdatabase *database;

	//-----------------------------------------

	long ID;
	long linkID;

	char *title;
	char txt[120];
	short str_mlen;
	bool nletters, nnumbers, checkbox, ispass;

	short x, y;
	short h, w;

	//-----------------------------------------

	short ttype;
	bool *btar;

	short *star;
	unsigned short *ustar;

	char  *ctar;
	unsigned char *uctar;

	long  *ltar;
	unsigned long *ultar;


	void addto();
	void removefrom();
	void keypress(short key);
	bool ison(short x2, short y2);
	void draw();
	void target(short type, void *target);
	void checked();
	void setlink(long id);
};




class texteditdatabase
{
public:

	texteditdatabase();

	~texteditdatabase();

	textedit *list;
	textedit *focus;

	void clearlist();
	void draw();
	void mouseclick(short x, short y);
	void update();
	void NewTextedit(char *title, short x, short y, short w, short h, bool nletters, bool nnumbers, short limitlen, bool checkbox, bool ispass, long id, long linkid);
	textedit *getbyid(long id);

	void setfocus(long id);
	void setfocus(textedit *ifocus);
};



#endif