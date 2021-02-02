
#ifndef gmtool_h
#define gmtool_h


#include "main.h"



class onlinePlayer
{
public:

	onlinePlayer(class OnlineListClass *base);
	~onlinePlayer();

	onlinePlayer *Next, *Prev;

	class OnlineListClass *base;

	char Name[25];
	long ID;
	int devMode;
	int specMode;
	int X;
	int Y;
	
	void addTo();
	void remove();
};


//----------------------------------------------------------
// Online list class, only for GMs currently
class OnlineListClass
{
public:

	OnlineListClass();
	~OnlineListClass();

	textlinkcontrol *textlinks;
	textlinkcontrol *mtextlinks;
	textlinkcontrol *spmodelinks;
	textlinkcontrol *simodelinks;
	textlinkcontrol *olmodelinks;

	buttoncontrol *buttons;
	buttoncontrol *spbuttons;
	buttoncontrol *sibuttons;
	buttoncontrol *olbuttons;
	onlinePlayer *oPlayers;

	short ToolMode;

	short AM[3];
	short ID[3];

	unsigned long itemAm;
	short itemID;
	short itemType;

	bool Changemode;
	short Changetarget;

	int onlineAmount;

	void d_Spawnmode();
	void d_Spawnitems();
	void d_Changemode();

	void Mouseclick_OListmode();
	void Mouseclick_SPmode();
	void Mouseclick_SImode();

	bool ShowJoins;
	bool ShowLeaves;
	bool ShowNewPls;

	bool On;

	onlinePlayer *Selected;

	short SelectedAction;

    short Scroll;

    void Draw_bGround();
	void Draw_Names();
	void Draw_Actions();

	ItemClass iBankItems[MaxBankItems];
	ItemClass iBackpackItems[MaxBackpackItems];
	
	bool bBankView, bBackpackView;
	char cIdOwnerName[25];

	void dBankView();
	void dBackpackView();
	void Mouseclick();
	void Mouseover(ItemClass Item, int t);

	void addPlayer(PACKET_NEWONLINEPLAYER NewPlayer);
	onlinePlayer *GetByID(long ID);
	void removeByID(long ID);

	void dropDownMenu(long id, short mx, short my);
	bool dDMenu;
	short ddx, ddy;
	void dDDMenu();
	void mbutRelease(short mx, short my);


	bool raceChange;
	void dRaceChange();
	void mClickRaceChange();
};

extern OnlineListClass *GMTool;
//----------------------------------------------------------


const char * const cmds[] =
{
	"Backpack",
	"View stats",
	"Ban from srvr",
	"Jail",
	"Disconnect"
};


/*
void OnlineListClass::dRaceChange()
{

}

void OnlineListClass::mClickRaceChange()
{

}
*/


#endif