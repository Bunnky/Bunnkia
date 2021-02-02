#pragma once

struct Avatar
{
	char Name[25];

	bool Normal;
	bool Premium;
	bool Golden;

	short AnimationType, NumFrames, AnimationSpeed;
	RECT Frames[9];
};


struct sdlAvatar
{
	char Name[25];

	bool Normal;
	bool Premium;
	bool Golden;

	short AnimationType, NumFrames, AnimationSpeed;
	SDL_Rect Frames[9];
};


struct RaceInfoLoad
{
	char Name[25];

	short StatLimits[4];
	short StartingStats[4];
	short SkillLimits[30];
	short ResistanceBonus[6];

	Avatar Avatars[2][5];
};


struct RaceInfo
{
	char Name[25];

	short StatLimits[4];
	short StartingStats[4];
	short SkillLimits[30];
	short ResistanceBonus[6];

	sdlAvatar Avatars[2][5];
};

class RaceClass
{
public:

	RaceClass();
	~RaceClass();

	textlinkcontrol	*textlinks; 

	short chosenRace, chosenAvatar, curAv;

	void dRaces();
	VOID LoadRaces();

	void mClick();
};

