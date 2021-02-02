#pragma once

class textlink
{
public:

	textlink(class textlinkcontrol *base2, char txt2[120], short x2, short y2, short h, short w, short action2, long id);
	~textlink();

	textlink *prev, *next;
	class textlinkcontrol *base;


	char txt[120];


	short action;
	short height, width; // Needs to be initialized
	
	bool inv;

	short x, y;

	long ID;


	void addto();
	void remove();
	void draw();
	bool ison(short x, short y);
	void pressed();
	void info();
};



class textlinkcontrol
{
public:

	textlinkcontrol();
	~textlinkcontrol();

	textlink *textlinks;

	
	void clear();
	void mouseclick();
	void draw();
	textlink *getbyid(long id);

	void switchinv(long id, bool state);

	void NewTextlink(char txt2[120], short x2, short y2, short h, short w, short action2, long id);
};
