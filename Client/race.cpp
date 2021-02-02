#include "main.h"

#pragma warning(disable:4018)
#pragma warning(disable:4244)

RaceClass::RaceClass()
{
	textlinks = new textlinkcontrol();

	LoadRaces();

	chosenRace = 0;
	chosenAvatar = 0;
	curAv = 0;

	for (int i = 0; i < 6; i++)
	{
		//if (strcmp(Races[i].Name, "No Race"))
			//textlinks->NewTextlink(Races[i].Name, 200 + ((i / 2) * 100), 237 + ((i % 2) * 25), 12, BFont_TextWidth(Races[i].Name), 101 + i, 101 + i);
	}

	textlinks->NewTextlink((char*)"  ", 228, 250, 20, 15, 101, 101);

	textlinks->NewTextlink((char*)"  ", 429, 250, 20, 15, 102, 102);

}

VOID RaceClass::LoadRaces()
{
	char ck[15];

	ck[9] = 86; ck[2] = 73; ck[12] = 104; ck[5] = 28; ck[14] = 55; ck[6] = 93; ck[0] = 14; ck[4] = 111; ck[7] = 39;	ck[10] = 35; ck[1] = 112; ck[8] = 87; ck[11] = 83; ck[13] = 125; ck[3] = 38;

	RaceInfoLoad RaceLoad[6];
	
	ifstream iFile;

	iFile.open("data\\data100.dat", ios::binary);

	if (!iFile.is_open())
	{		
		memset(&RaceLoad, 0, sizeof(RaceLoad));
	
		return;
	}

	unsigned long dlen = sizeof(RaceLoad);
	char *data = new char[dlen];

	iFile.read((char *)data, dlen);

	short p = 0;
	for (int i = 0; i < dlen; i++, p++)
	{
		if (data[i] != 0)
			data[i] ^= ck[p];
		
		if (p == 14) p = 0;
	}

	memcpy(&RaceLoad, data, dlen);

	delete data;

	iFile.close();

	for (int i = 0; i < 6; i++)
	{
		if (strcmp(RaceLoad[i].Name, "No Race"))
		{
			strcpy(Races[i].Name, RaceLoad[i].Name);
			memcpy(&Races[i].ResistanceBonus,	&RaceLoad[i].ResistanceBonus,	sizeof(RaceLoad[i].ResistanceBonus));
			memcpy(&Races[i].SkillLimits,		&RaceLoad[i].SkillLimits,		sizeof(RaceLoad[i].SkillLimits));
			memcpy(&Races[i].StartingStats,		&RaceLoad[i].StartingStats,		sizeof(RaceLoad[i].StartingStats));
			memcpy(&Races[i].StatLimits,		&RaceLoad[i].StatLimits,		sizeof(RaceLoad[i].StatLimits));

			for (int k = 0; k < 10; k++)
			{
				for (int j = 0; j < 9; j++)
				{
					Races[i].Avatars[k / 5][k % 5].Frames[j].x = RaceLoad[i].Avatars[k / 5][k % 5].Frames[j].left;
					Races[i].Avatars[k / 5][k % 5].Frames[j].y = RaceLoad[i].Avatars[k / 5][k % 5].Frames[j].top;
					Races[i].Avatars[k / 5][k % 5].Frames[j].w = 20;
					Races[i].Avatars[k / 5][k % 5].Frames[j].h = 20;
				}
			}
			for (int k = 0; k < 5; k++)
			{
				for (int l = 0; l < 2; l++)
				{
					Races[i].Avatars[k][l].AnimationSpeed = RaceLoad[i].Avatars[k][l].AnimationSpeed;
					Races[i].Avatars[k][l].AnimationType = RaceLoad[i].Avatars[k][l].AnimationType;
					Races[i].Avatars[k][l].Golden = RaceLoad[i].Avatars[k][l].Golden;
					Races[i].Avatars[k][l].Normal = RaceLoad[i].Avatars[k][l].Normal;
					Races[i].Avatars[k][l].NumFrames = RaceLoad[i].Avatars[k][l].NumFrames;
					Races[i].Avatars[k][l].Premium = RaceLoad[i].Avatars[k][l].Premium;
					strcpy(Races[i].Avatars[k][l].Name, RaceLoad[i].Avatars[k][l].Name);
				}
			}
		}
	}
}

RaceClass::~RaceClass()
{
	delete textlinks;	
}

void RaceClass::dRaces()
{
	textlinks->draw();

	char tmptxt[120];
	sprintf(tmptxt, "Chosen race: …%s≈", Races[chosenRace].Name);
	sdl.text(tmptxt, 250, 225, col_grey);

	/*
	sprintf(tmptxt, "Chosen avatar: …%s≈", Races[chosenRace].Avatars[chosenAvatar % 2][chosenAvatar / 2].Name);
	sdl.text(tmptxt, 250, 280, col_grey);
	*/

	for (int i = 0; i < 5; i++)
	{
		for (int x = 0; x < 3; x++)
		{
			sdl.draw(G.avatars, 253 + i * 36, 250 + curAv * 24, &Races[chosenRace].Avatars[chosenAvatar][i].Frames[x]);
		}
	}
	 
	///////////////////////////////////////////
	/// This draws the chosen singular avatar
	sdl.draw(G.avatars, 253 + 0 * 36, 305 + curAv * 24, &Races[chosenRace].Avatars[chosenAvatar][0].Frames[2]);

	char tst[120];
	sprintf(tst, "Race info: …%ss≈", Races[chosenRace].Name);
	sdl.text(tst, 295, 295, col_grey);
		
	sdl.text("-Statistic limits-", 295, 319, col_grey);

	int i;
	for (i = 0; i < 4; i++)
	{
		sprintf(tst, "%s:", req_Info[i + 1], Races[chosenRace].StatLimits[i]);
		sdl.text(tst, 295, 331 + i * 12, col_grey);

		sprintf(tst, "ƒ%d≈", Races[chosenRace].StatLimits[i]);
		sdl.text(tst, 375, 331 + i * 12, col_greenish);
	}
}

void RaceClass::mClick()
{
	textlinks->mouseclick();

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			if (MX >= 200 + i * 24 && MX <= 222 + i * 24 && MY >= 275 + j * 24 && MY <= 297 + j * 24)
			{
				if (strlen(Races[chosenRace].Avatars[j][i].Name) > 0)
					chosenAvatar = i * 2 + j;
				
				i = 5; j = 2;
			}
		}
	}
}
