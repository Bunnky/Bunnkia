#pragma once


void LoadRaces();


struct Avatar
{
	char Name[25];

	bool Normal;
	bool Premium;
	bool Golden;

	short AnimationType, NumFrames, AnimationSpeed;
	RECT Frames[9];
};


struct RaceInfo
{
	char Name[25];

	short StatLimits[4];
	short StartingStats[4];
	short SkillLimits[30];
	short ResistanceBonus[6];

	Avatar Avatars[2][5];
};

