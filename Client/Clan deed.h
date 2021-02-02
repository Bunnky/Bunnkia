
#ifndef clan_deed_h
#define clan_deed_h


#include "main.h"

//----------------------------------------------------------------------
// Deed class
class DeedClass
{
public:

	DeedClass();
	~DeedClass();

	buttoncontrol *buttons;

	bool IsLeader;
	short own_rank;

	char Name[45];
	char Tag[4];

	char Txt[45];

	bool On;

	bool UsedSlot[50];
	char Names[50][25];
	short IDs[50];
	bool Status[50];

	short ranks[50];

    short Selected;
    
	short SelectedAction;

	short Scroll;


	bool Wait_Action;
	short Action_Target;
	void Execute_Action_Add();
	void Execute_Action_Remove();
	void Execute_Action_Disband();
	void Execute_Action_Transfer();

	void Deed();
	void D_Names();
	void Actions();
	void MouseClick();
};

extern DeedClass *Clan;
//----------------------------------------------------------------------


#endif