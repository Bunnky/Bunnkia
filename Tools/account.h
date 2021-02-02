#ifndef account_h
#define account_h

#include "main.h"


class AccountTool
{
public:


	void check_command();
};

extern AccountTool AccTool;







class AccountToEdit
{
public:

	bool InUse;
	bool IsBanned;
	bool IsPremium;
	bool IsGolden;

	char AccName[20];
	char AccPass[20];
	char Email[65];

	long cMinute, cHour, cDay, cMonth, cYear;

	unsigned long ID;

	char CharNames[2][25];
};




#endif
