#include "main.h"

#pragma warning(disable:4018)

void LoadRaces()
{
	char ck[15];

	ck[9] = 86; ck[2] = 73; ck[12] = 104; ck[5] = 28; ck[14] = 55; ck[6] = 93; ck[0] = 14; ck[4] = 111; ck[7] = 39;	ck[10] = 35; ck[1] = 112; ck[8] = 87; ck[11] = 83; ck[13] = 125; ck[3] = 38;
	
	ifstream iFile;

	iFile.open("data\\data100.dat", ios::binary);

	if (!iFile.is_open())
	{
		memset(&Races, 0, sizeof(Races));
	
		return;
	}

	unsigned long dlen = sizeof(Races);
	char *data = new char[dlen];

	iFile.read((char *)data, dlen);

	short p = 0;
	for (int i = 0; i < dlen; i++, p++)
	{
		if (data[i] != 0)
			data[i] ^= ck[p];
		
		if (p == 14) p = 0;
	}

	memcpy(&Races, data, dlen);

	delete data;

	iFile.close();
}