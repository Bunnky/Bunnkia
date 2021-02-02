#ifndef npc_h
#define npc_h

#include "main.h"



struct NPCTypesStruct
{
	char Name[25];

	bool Used, print_name;


	bool Walking, fly, ghost;
	short WalkingType;
	short WalkingRange;

	bool is_shop;
	bool is_guard;
	short alignment;


    short dam_min, dam_max;
	short max_hp;

	short atk_type;

	short dex, con, AC;


	short speed;
	short atk_speed;

	bool Animated;

	char conv[80];
	bool adv_conv;
	short adv_conv_id;


	SDL_Rect Frames[9];
    
	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

extern NPCTypesStruct NPCTypes[m_NPCTypes];
//------------------------------------------------------------------



//------------------------------------------------------------------
//
class NPCClass
{
public:
	
	NPCClass *next, *prev;
	class NPCDatabase *database;

	short x,y, id, type, hp, conv_id;


	void addto();
	void removefrom();

	//==========================================
	short frame;
	unsigned long frametime;

	void anim_start();
	void anim_advance();
	//==========================================
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
class NPCDatabase
{
public:

	NPCClass *npc;
	NPCClass *np;

	NPCClass *getbyid(short id);
	NPCClass *getlast();
	void clearlist();
	void drawnpc();
};
extern NPCDatabase *npcdatabase;
//------------------------------------------------------------------




#endif