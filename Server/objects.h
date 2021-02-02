#ifndef objects_h
#define objects_h

#include "main.h"


class MapObjects
{
public:

	MapObjects *next;

	bool On;

	short X,Y;

	short _X, _Y;

	unsigned long Container[8];

	short Type;

	short Text;


	bool use(short user);
	bool deuse(short user);
	bool is_blocked();
};


extern MapObjects		*Objects[128][128];
extern MapObjects		*obj_List;

struct MapObjectsToSave
{
	short X,Y;

	short _X, _Y;

	unsigned long Container[8];

	short Type;

	short Text;
};

struct ObjectsStruct
{
	bool Used;

	bool Openable;

	bool Block;
	bool VisBlock;

	unsigned long UseableWithItemID;

	char Name[25];
    
	short UseType;

	bool Animated;
	unsigned char AnimationType;

	unsigned char num_Frames;

	char data[72];
};

extern ObjectsStruct	ObjectTypes[ m_Object_Types ];



class CrimSpawnList
{
public:

	CrimSpawnList *Next, *Prev;

	short X, Y;
};

extern CrimSpawnList *crimSpawns;


#endif