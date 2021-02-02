#ifndef _skills_h
#define _skills_h

#include "main.h"



#define skilltype_combat		1
#define skilltype_defend		2
#define skilltype_trade			3
#define skilltype_knowledge		4

const char * const skilltypes[] =
{
	"no type",
	"Combat",
	"Defending",
	"Trade",
	"Knowledge"
};


struct skillinfo
{
	bool used;
	char name[30];
	short skilltype;
};


class skill_list
{
public:
	skill_list *next;
	short skill;
};



class skillsclass
{
public:
	bool v_skills;

	skill_list *l_list;
	short scroll; // not needed atm, only 20 skills exist.
	skill_list *chosen;
	char tmp[120];
	bool expanded[5];
	short col;

	void c_skill_list();
	void d_skillwindow();
	void d_skillnames();
	void d_statusbar();

	void mouseclick();
};

extern skillsclass skill;

extern skillinfo skills[];




#endif