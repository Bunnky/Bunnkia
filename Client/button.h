#ifndef button_h
#define button_h

#include "main.h"


class button
{
public:

	button(class buttoncontrol *base2, short x2, short y2, short h, short w, short pic2, short action2);
	~button();

	button *prev, *next;
	class buttoncontrol *base;


	short pic;
	short action;
	short height, width; // Needs to be initialized

	short x, y;


	void addto();
	void remove();
	void draw();
	bool ison(short x, short y);
	void pressed();
};


class buttoncontrol
{
public:

	buttoncontrol();
	~buttoncontrol();

	button *buttons;

	
	void clear();
	void mouseclick();
	void draw();
};



#endif