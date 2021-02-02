#ifndef events_h
#define events_h

#include "main.h"




void MouseRightButtonUp();
void MouseButtonUpSkills();
void MouseRightButtonDown();



//----------------------------------------------------------------------------
//
class TargetClass
{
public:

	short Type; 

	MonsterClass*MonTarget;
	PlayerNode	*PlTarget;
	NPCClass	*NPCTarget;

	short TarID; // for checking if he went out of the screen.

	bool DrawTarget;

	unsigned char Frame;
	short FramePlus;
	unsigned long FrameTime;
};

extern TargetClass Target;
//------------------------------------------------------------------


//----------------------------------------------------------------------------
//
struct WritingClass
{
	int Mode;
	int ChattingMode;

	bool Writing;

	char Txt[100];

	int KeyHandler;
	
	int LoginHandler[3];

	bool LoginError[3];

	bool WritingError;

	bool BP_on, firstBP;
	unsigned long BP_time;
	unsigned long LastBP_time;

	bool KeyProbhited(int k, int unic);
	void KeyboardButtonUp(int key, int unic);

	void BP();
	void CheckTab();
	void Start( int Modetus);
	void Stop()
	{
		Writing = false;
		memset(&Txt, 0, sizeof(Txt));
		WritingError = false;
		firstBP = false;
		Mode = 0;
	}

	//void ParseNewEmail();
	void ParseNewPass();
};

extern WritingClass Writing;
//------------------------------------------------------------------

/*
void WritingClass::ParseNewEmail()
{
	int parsedto = 10;
	char pass[25];
	memset(&pass, 0, sizeof(pass));

	char email[25];
	memset(&email, 0, sizeof(email));

	for (int i = parsedto; i < 100; i++)
	{
		if (Txt[i] == 32) { parsedto = i + 1; i = 100; }
		else
			pass[i - parsedto] = Txt[i];		
	}

	for (i = parsedto; i < 100; i++)
	{
		if (Txt[i] == 32 || Txt[i] == 0) { i = 100; }
		else 
			email[i - parsedto] = Txt[i];
	}


}
*/

//----------------------------------------------------------------------------
//

#endif