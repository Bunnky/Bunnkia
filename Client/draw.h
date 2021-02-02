
#ifndef draw_h
#define draw_h


#include "main.h"


#define m_TmpPics					10



void CheckFrame();
void DrawItem( short family, short type, short ID);
void Draw_GuildBuy();
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);


//------------------------------------------------------------------
//
struct DrawClass
{
	unsigned char OrbState;
	unsigned long OrbStateTime;
	char OrbStateChange;

	void drawAll();
	void DrawBG();
	void DrawFPS();
	void DrawBackpack();
	void DrawMinimap();
	void DrawWorld();
	void DrawStats();
	void DrawChatBox();
	void DrawCharacters();
	void DrawBank();
	void DrawWearedItems();
	void DrawGroundItems();
	void DrawDraggingItem();
	void DrawTarget();
	void DrawTmpPics();
	void DrawSplit();
	void DrawSkills();
	void MouseOver();
	void DrawIdentify();
	void Help();
	void Opt();
	void Kicked();
	void Banned();
	void Ranked();
	void DrawOpenBody();
	void Horizontal_Underline( short FromX, short ToX, short Y, unsigned char r, unsigned char g, unsigned char b);
	void PrintReputationText (int x, int y, bool Wanted, int Reputation, char *name, bool guild, char *tag);

	/* New functions. */
	void d_items();
};

extern DrawClass Draw;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
//------------------------------------------------------------------


//------------------------------------------------------------------
//
//------------------------------------------------------------------


//------------------------------------------------------------------
//
class MessageBoxClass
{
public:

	char NewMsg[120];
	char Msg[49][120];
	unsigned char MsgType[49];
	unsigned char ChatMsgType[49];
	unsigned char NewType;

	bool Expanded;

	int scroll;

	void Update();
	void Print();
	void NewMessage(char const msg[120], short MsgType);
};

extern MessageBoxClass MB;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct CombatTextClass
{ 
	char msg[10][40];
	int msgtype[10];
	char newmsg[40];
	int newmsgtype;

	int used[10];

	int lstupdate;

	void Update();
	void Print();
	void TimeUpdate();
};

extern CombatTextClass CT;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct MiscPicClass
{
	unsigned char Used;

	short Pic;

	short X,Y;

	unsigned long FrameTime;
};

extern MiscPicClass TmpPics[m_TmpPics];
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct PACKET_MiscPicClass
{
	short Pic;

	short X,Y;
};
//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct NewCombatTextClass
{
	short Amount;
	short Type;
	short SkillType;
	short FromTo;	
};
//------------------------------------------------------------------


#define WeaponX				754
#define WeaponY				123
#define PlateX				716
#define PlateY				112
#define HelmX				716
#define HelmY				72
#define ShieldX				678
#define ShieldY				123
#define LegsX				716
#define LegsY				171
#define GauntsX				678
#define GauntsY				84
#define RingX				678
#define RingY				159
#define AmuletX				754
#define AmuletY				84
#define RobeX				754
#define RobeY				159



/*
#define IDWeaponX			87
#define IDWeaponY			70
#define IDPlateX			49
#define IDPlateY			58
#define IDHelmX				49
#define IDHelmY				19
#define IDShieldX			11
#define IDShieldY			70
#define IDLegsX				49
#define IDLegsY				118
#define IDGauntsX			11
#define IDGauntsY			31
#define IDRingX				11
#define IDRingY				106
#define IDAmuletX			87
#define IDAmuletY			31
#define IDRobeX				87
#define IDRobeY				106
*/



#endif