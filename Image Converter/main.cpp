//#include <windows.h>
//#include <iostream>
#include <fstream>
#include <SDL.h>
using namespace std;

#pragma comment(lib, "sdl.lib")
#pragma comment(lib, "sdlmain.lib")

void Graphics();
void LoginGraphics();
void StaticGraphics();
void TileGraphics();
void ItemGraphics();
void AvatarGraphics();
void ClientGraphics();
void ObjectGraphics();	
void ConvertMBEXP();

//void convert_items();
//void convert_npctypes();



int main(int argv, char *argc[])
{
	Graphics();
//	convert_items();
//	convert_npctypes();
	return 0;
}

void Graphics()
{
	LoginGraphics();
	StaticGraphics();
	TileGraphics();
	ItemGraphics();
	AvatarGraphics();
	ClientGraphics();
	ObjectGraphics();	

	ConvertMBEXP();

	printf("                            -Finished!");
	SDL_Delay(1000);
}
//////////////////////////
//	gfdata0x.vdf
//	X		graphs in it.
//	0		login graphs
//	1		static graphs
//	2		tiles
//	3		items
//	4		avatars
//	5		addgf
//	6		objects
//  7		expanded MB
//////////////////////////
void ClientGraphics()
{
	SDL_Surface *tmp;

	printf("\nConverting Client Graphics");

	long it;

	ofstream fout("Coverted Graphics/gdata05.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata05.vdf for saving.\n");
		return;
	}

	printf("\nOpening addgf.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/addgf.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");

}

void ObjectGraphics()
{
	SDL_Surface *tmp;
	printf("\nConverting Object Graphics");

	long it;

	ofstream fout("Coverted Graphics/gdata06.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata06.vdf for saving.\n");
		return;
	}

	printf("\nOpening objects.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/objects.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");
}

void AvatarGraphics()
{
	SDL_Surface *tmp;
	printf("\nConverting Avatar Graphics");

	long it;

	ofstream fout("Coverted Graphics/gdata04.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata04.vdf for saving.\n");
		return;
	}

	printf("\nOpening avatars.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/avatars.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");
}

void ItemGraphics()
{
	SDL_Surface *tmp;

	printf("\nCoverting Item Graphics");

	long it;

	ofstream fout("Coverted Graphics/gdata03.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata03.vdf for saving.\n");
		return;
	}

	printf("\nOpening itemgraph.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/itemgraph.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");
}

void TileGraphics()
{
	SDL_Surface *tmp;
	printf("\nConverting Tile Graphics");

	long it;


	ofstream fout("Coverted Graphics/gdata02.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata02.vdf for saving.\n");
		return;
	}

	printf("\nOpening tiles.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/tiles.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");
}

void LoginGraphics()
{
	SDL_Surface *tmp;
	printf("Converting Login Graphics");

	long it;


	ofstream fout("Coverted Graphics/gdata00.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata00.vdf for saving.\n");
		return;
	}

	printf("\nOpening acccreation.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/acccreation.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening bg1.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/bg1.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening charchoose.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/charchoose.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening charcreation.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/charcreation.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening login.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/login.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening main.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/main.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");

	fout.close();
}

void StaticGraphics()
{
	SDL_Surface *tmp;
	printf("\nConverting Static Graphics");

	long it;


	ofstream fout("Coverted Graphics/gdata01.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata01.vdf for saving.\n");
		return;
	}

	printf("\nOpening identify.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/identify.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening layout.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/layout.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening List.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/List.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening mouse.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/mouse.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening shop.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/shop.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");


	printf("\nOpening skills.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/skills.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +");
}

void ConvertMBEXP()
{
	SDL_Surface *tmp;
	printf("\nConverting MBEXP(???)");

	long it;


	ofstream fout("Coverted Graphics/gdata07.vdf", ios::binary);
	if (!fout.is_open()) 
	{
		printf("Error: Failed to open gdata07.vdf for saving.\n");
		return;
	}

	printf("\nOpening mbexp.bmp..");
	tmp = SDL_LoadBMP("Images to Convert/mbexp.bmp");
	it = tmp->h; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	it = rand() % 10000000;
	fout.write((char *)&it, sizeof(it));
	it = tmp->w; it = it << 2;
	fout.write((char *)&it, sizeof(it));
	fout.write((char *)tmp->pixels, tmp->h * (tmp->w * 3));
	SDL_FreeSurface(tmp);
	printf("Done!   +\n");
}



























struct OLD_WeaponInfo
{
	char Name[25];

    short DamMin;
	short DamMax;

	unsigned long Value;

	short Weight;

	short Speed;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;
	short Level;

	short ElementalDamageType;
	short ElementalDamageMin;
	short ElementalDamageMax;

	short MaxDur;

	short MetalType;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];
};

struct OLD_ArmorInfo
{
	char Name[25];

	short AC;

	unsigned long Value;

	short Weight;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short Level;

	short Speciality;
	short SpecialityAmount;

	short ElementalProtection;
	short ElementalProtectionAmount;

	short MaxDur;

	short MetalType;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];
};

struct OLD_UseableItemStruct
{
	char Name[25];

    unsigned long Value;

	short Weight;
	short DamMin;
	short DamMax;

	short MaxDurability;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_type; // use_MINE, use_SMELT, use_FORGE, use_REPAIR, use_BOOST,
	short use_req_amnt;
	short use_req_type;
	short use_req_skill;
};

struct OLD_MiscItemStruct
{
	char Name[25];

	unsigned long Value;

	short Weight;
    short DamMin;
	short DamMax;

	short CureType;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;
};






//------------------------------------------------------------------
//
struct WeaponInfo
{
	char Name[25];

    short DamMin;
	short DamMax;

	unsigned long Value;

	short Weight;

	short Speed;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;
	short Level;

	short Speciality;
	short SpecialityAmount;
	
	short ElementalDamageType;
	short ElementalDamageMin;
	short ElementalDamageMax;

	short MaxDur;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	char reserved[100];
};

//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct ArmorInfo
{
	char Name[25];

	short AC;

	unsigned long Value;

	short Weight;

	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short Level;

	short Speciality;
	short SpecialityAmount;

	short ElementalProtection;
	short ElementalProtectionAmount;

	short MaxDur;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	char reserved[100];
};

//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct UseableItemStruct
{
	char Name[25];

    unsigned long Value;

	short Weight;
	short DamMin;
	short DamMax;

	short MaxDurability;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_type; // use_MINE, use_SMELT, use_FORGE, use_REPAIR, use_BOOST,
	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	char reserved[100];
};

//------------------------------------------------------------------


//------------------------------------------------------------------
//
struct MiscItemStruct
{
	char Name[25];

	unsigned long Value;

	short Weight;
    short DamMin;
	short DamMax;

	short CureType;

	short MaxDurability;

	bool Animated;

	short num_Frames;
	short anim_Speed;
	short anim_Type;
	SDL_Rect Frames[9];

	short use_type;
	short use_req_amnt;
	short use_req_type;
	short use_req_skill;

	short bs_metaltype;
	short bs_amount;
	short bs_difficulty;
	short bs_req;

	char reserved[100];
};


#define m_Item_Types				10
#define OLD_m_Items_per_type		70

#define m_Items_per_type			200



OLD_WeaponInfo			OLD_Weapons[m_Item_Types][OLD_m_Items_per_type];
OLD_ArmorInfo			OLD_Armors[m_Item_Types][OLD_m_Items_per_type];
OLD_MiscItemStruct		OLD_Collectables[OLD_m_Items_per_type];
OLD_UseableItemStruct	OLD_Useables[OLD_m_Items_per_type];


WeaponInfo			Weapons[m_Item_Types][m_Items_per_type];
ArmorInfo			Armors[m_Item_Types][m_Items_per_type];
MiscItemStruct		Collectables[m_Items_per_type];
UseableItemStruct	Useables[m_Items_per_type];




void convert_items()
{
	ifstream file;
	file.open("old_data/weapontypes.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_Weapons, sizeof(OLD_Weapons));
	file.close();


	file.open("old_data/armors.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_Armors, sizeof(OLD_Armors));
	file.close();


	file.open("old_data/collectables.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_Collectables, sizeof(OLD_Collectables));
	file.close();


	file.open("old_data/useables.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_Useables, sizeof(OLD_Useables));
	file.close();


	memset(&Weapons, 0, sizeof(Weapons));
	memset(&Armors, 0, sizeof(Armors));
	memset(&Collectables, 0, sizeof(Collectables));
	memset(&Useables, 0, sizeof(Useables));
	

	for (int i = 0; i < m_Item_Types; i++)
	{
		for (int j = 0; j < OLD_m_Items_per_type; j++)
		{
			// Convert weapons.
			Weapons[i][j].anim_Speed = OLD_Weapons[i][j].anim_Speed;
			Weapons[i][j].anim_Type = OLD_Weapons[i][j].anim_Type;
			Weapons[i][j].Animated = OLD_Weapons[i][j].Animated;
			Weapons[i][j].bs_metaltype = OLD_Weapons[i][j].MetalType;
			Weapons[i][j].use_req_amnt = OLD_Weapons[i][j].use_req_amnt;
			Weapons[i][j].use_req_type = OLD_Weapons[i][j].use_req_type;
			Weapons[i][j].use_req_skill = OLD_Weapons[i][j].use_req_skill;
			Weapons[i][j].Value = OLD_Weapons[i][j].Value;
			Weapons[i][j].Weight = OLD_Weapons[i][j].Weight;
			Weapons[i][j].DamMax = OLD_Weapons[i][j].DamMax;
			Weapons[i][j].DamMin = OLD_Weapons[i][j].DamMin;
			Weapons[i][j].ElementalDamageMax = OLD_Weapons[i][j].ElementalDamageMax;
			Weapons[i][j].ElementalDamageMin = OLD_Weapons[i][j].ElementalDamageMin;
			Weapons[i][j].ElementalDamageType = OLD_Weapons[i][j].ElementalDamageType;
			Weapons[i][j].Level = OLD_Weapons[i][j].Level;
			Weapons[i][j].MaxDur = OLD_Weapons[i][j].MaxDur;
			Weapons[i][j].num_Frames = OLD_Weapons[i][j].num_Frames;
			Weapons[i][j].Speed = OLD_Weapons[i][j].Speed;
			memcpy(&Weapons[i][j].Frames, &OLD_Weapons[i][j].Frames, sizeof(OLD_Weapons[i][j].Frames));
			strcpy_s(Weapons[i][j].Name, OLD_Weapons[i][j].Name);
			//================

			// Convert weapons.
			Armors[i][j].anim_Speed = OLD_Armors[i][j].anim_Speed;
			Armors[i][j].anim_Type = OLD_Armors[i][j].anim_Type;
			Armors[i][j].Animated = OLD_Armors[i][j].Animated;
			Armors[i][j].bs_metaltype = OLD_Armors[i][j].MetalType;
			Armors[i][j].use_req_amnt = OLD_Armors[i][j].use_req_amnt;
			Armors[i][j].use_req_type = OLD_Armors[i][j].use_req_type;
			Armors[i][j].use_req_skill = OLD_Armors[i][j].use_req_skill;
			Armors[i][j].Value = OLD_Armors[i][j].Value;
			Armors[i][j].Weight = OLD_Armors[i][j].Weight;
			Armors[i][j].AC = OLD_Armors[i][j].AC;
			Armors[i][j].Speciality = OLD_Armors[i][j].Speciality;
			Armors[i][j].SpecialityAmount = OLD_Armors[i][j].SpecialityAmount;
			Armors[i][j].ElementalProtection = OLD_Armors[i][j].ElementalProtection;
			Armors[i][j].ElementalProtectionAmount = OLD_Armors[i][j].ElementalProtectionAmount;
			Armors[i][j].Level = OLD_Armors[i][j].Level;
			Armors[i][j].MaxDur = OLD_Armors[i][j].MaxDur;
			Armors[i][j].num_Frames = OLD_Armors[i][j].num_Frames;
//			Armors[i][j].Speed = OLD_Armors[i][j].Speed;			
			memcpy(&Armors[i][j].Frames, &OLD_Armors[i][j].Frames, sizeof(OLD_Armors[i][j].Frames));
			strcpy_s(Armors[i][j].Name, OLD_Armors[i][j].Name);
			//================

		}
	}

	for (int i = 0; i < OLD_m_Items_per_type; i++)
	{
		Useables[i].anim_Speed = OLD_Useables[i].anim_Speed;
		Useables[i].anim_Type = OLD_Useables[i].anim_Type;
		Useables[i].Animated = OLD_Useables[i].Animated;
		Useables[i].DamMax = OLD_Useables[i].DamMax;
		Useables[i].DamMin = OLD_Useables[i].DamMin;
		Useables[i].MaxDurability = OLD_Useables[i].MaxDurability;
		Useables[i].num_Frames = OLD_Useables[i].num_Frames;
		Useables[i].use_req_amnt = OLD_Useables[i].use_req_amnt;
		Useables[i].use_req_skill = OLD_Useables[i].use_req_skill;
		Useables[i].use_req_type = OLD_Useables[i].use_req_type;
		Useables[i].use_type = OLD_Useables[i].use_type;
		Useables[i].Value = OLD_Useables[i].Value;
		Useables[i].Weight = OLD_Useables[i].Weight;
		memcpy(&Useables[i].Frames, &OLD_Useables[i].Frames, sizeof(OLD_Useables[i].Frames));
		strcpy_s(Useables[i].Name, OLD_Useables[i].Name);


		Collectables[i].anim_Speed = OLD_Collectables[i].anim_Speed;
		Collectables[i].anim_Type = OLD_Collectables[i].anim_Type;
		Collectables[i].Animated = OLD_Collectables[i].Animated;
		Collectables[i].DamMax = OLD_Collectables[i].DamMax;
		Collectables[i].DamMin = OLD_Collectables[i].DamMin;
//		Collectables[i].MaxDurability = OLD_Collectables[i].MaxDurability;
		Collectables[i].num_Frames = OLD_Collectables[i].num_Frames;
		Collectables[i].use_req_amnt = OLD_Collectables[i].use_req_amnt;
		Collectables[i].use_req_skill = OLD_Collectables[i].use_req_skill;
		Collectables[i].use_req_type = OLD_Collectables[i].use_req_type;
		Collectables[i].CureType = OLD_Collectables[i].CureType;
		Collectables[i].Value = OLD_Collectables[i].Value;
		Collectables[i].Weight = OLD_Collectables[i].Weight;
		memcpy(&Collectables[i].Frames, &OLD_Collectables[i].Frames, sizeof(OLD_Collectables[i].Frames));
		strcpy_s(Collectables[i].Name, OLD_Collectables[i].Name);
	}

	
	ofstream fout;

	fout.open("new_data/data05.dat", ios::binary);

	char *data = new char[sizeof(Weapons)];

	memcpy(data, &Weapons, sizeof(Weapons));

	for (int i = 0; i < sizeof(Weapons); i++)
	{
		data[i] ^= 212;
	}

	fout.write((char *)data, sizeof(Weapons));
	fout.close();
	delete data;


	fout.open("new_data/data06.dat", ios::binary);

	data = new char[sizeof(Armors)];

	memcpy(data, &Armors, sizeof(Armors));

	for (int i = 0; i < sizeof(Armors); i++)
	{
		data[i] ^= 212;	
	}

	fout.write((char *)data, sizeof(Armors));
	fout.close();
	delete data;


	fout.open("new_data/data07.dat", ios::binary);

	data = new char[sizeof(Collectables)];

	memcpy(data, &Collectables, sizeof(Collectables));

	for (int i = 0; i < sizeof(Collectables); i++)
	{
		data[i] ^= 212;	
	}

	fout.write((char *)data, sizeof(Collectables));
	fout.close();
	delete[] data;


	fout.open("new_data/data08.dat", ios::binary);

	data = new char[sizeof(Useables)];

	memcpy(data, &Useables, sizeof(Useables));

	for (int i = 0; i < sizeof(Useables); i++)
	{
		data[i] ^= 212;	
	}

	fout.write((char *)data, sizeof(Useables));
	fout.close();
	delete[] data;



}











#define OLD_m_NPCTypes					64
#define OLD_m_MonsterTypes				96

#define m_MonsterTypes					250
#define m_NPCTypes						150





struct LootStruct
{
	long DropProbability;
	short Type, ID, Family;
	unsigned long AmountMin, AmountMax;
};



//------------------------------------------------------------------
//
struct MonsterTypesStruct
{
	char Name[25];

	bool Used, ghost, fly, rnd_walk_off;
	short rnd_walk_range;

	short DamMin;
	short DamMax;
	short HPMax;
	short AC;
	short Dex, con;

	short MovingSpeed;
	short AttackSpeed;
    
	unsigned char MonsterLevel;

	short ElementalDefence;
	short DefenceAmount;

	short ElementalAttack;
	short AttackAmount;

	LootStruct Loot[10];

	SDL_Rect Frames[9];

	bool Animated;

	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

MonsterTypesStruct MonsterTypes[m_MonsterTypes];
//------------------------------------------------------------------





struct OLD_MonsterTypesStruct
{
	char Name[25];

	bool Used;

	short DamMin;
	short DamMax;
	short HPMax;
	short AC;
	short Dex;

	short MovingSpeed;
	short AttackSpeed;
    
	unsigned char MonsterLevel;

	short ElementalDefence;
	short DefenceAmount;

	short ElementalAttack;
	short AttackAmount;

	LootStruct Loot[10];

	char data[72];

	bool Animated;

	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;
};

OLD_MonsterTypesStruct OLD_MonsterTypes[OLD_m_MonsterTypes];







struct OLD_NPCTypesStruct
{
	char Name[25];

	bool Used;

	short Avatar;

	bool Walking;
	short WalkingType;
	short WalkingRange;

	bool is_shop;
	bool is_guard;

    short dam_min, dam_max;
	short max_hp;

	short regen_rate;

	short atk_type;

	short dex;

	short speed;
	short atk_speed;

	bool Animated;

	SDL_Rect Frames[9];
    
	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;
};

OLD_NPCTypesStruct OLD_NPCTypes[OLD_m_NPCTypes];



struct NPCTypesStruct
{
	char Name[25];

	bool Used, print_name;


	bool Walking, fly, ghost;
	short WalkingType;
	short WalkingRange;

	bool is_shop;
	bool is_guard;
	short alignment;


    short dam_min, dam_max;
	short max_hp;

	short atk_type;

	short dex, con, AC;


	short speed;
	short atk_speed;

	bool Animated;

	char conv[80];
	bool adv_conv;
	short adv_conv_id;


	SDL_Rect Frames[9];
    
	short anim_speed;
	unsigned char num_Frames;
	unsigned char anim_Type;

	char reserved[200];
};

NPCTypesStruct NPCTypes[m_NPCTypes];



void convert_npctypes()
{
	memset(&MonsterTypes, 0, sizeof(MonsterTypes));
	memset(&NPCTypes, 0, sizeof(NPCTypes));

	ifstream file;
	ofstream fout;

	file.open("old_data/monstertypes.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_MonsterTypes, sizeof(OLD_MonsterTypes));
	file.close();

	file.open("old_data/NPCTypes.dat", ios::binary);
	file.seekg(25, ios::beg);
	file.read((char *)&OLD_NPCTypes, sizeof(OLD_NPCTypes));
	file.close();

	for (int i = 0; i < OLD_m_MonsterTypes; i++)
	{
		MonsterTypes[i].AC = OLD_MonsterTypes[i].AC;
		MonsterTypes[i].anim_speed = OLD_MonsterTypes[i].anim_speed;
		MonsterTypes[i].anim_Type = OLD_MonsterTypes[i].anim_Type;
		MonsterTypes[i].Animated = OLD_MonsterTypes[i].Animated;
		MonsterTypes[i].AttackAmount = OLD_MonsterTypes[i].AttackAmount;
		MonsterTypes[i].AttackSpeed = OLD_MonsterTypes[i].AttackSpeed;
		MonsterTypes[i].DamMax = OLD_MonsterTypes[i].DamMax;
		MonsterTypes[i].DamMin = OLD_MonsterTypes[i].DamMin;
		MonsterTypes[i].DefenceAmount = OLD_MonsterTypes[i].DefenceAmount;
		MonsterTypes[i].Dex = OLD_MonsterTypes[i].Dex;
		MonsterTypes[i].ElementalAttack = OLD_MonsterTypes[i].ElementalAttack;
		MonsterTypes[i].ElementalDefence = OLD_MonsterTypes[i].ElementalDefence;
		MonsterTypes[i].HPMax = OLD_MonsterTypes[i].HPMax;
		memcpy(&MonsterTypes[i].Frames, &OLD_MonsterTypes[i].data, sizeof(MonsterTypes[i].Frames));
		memcpy(&MonsterTypes[i].Loot, &OLD_MonsterTypes[i].Loot, sizeof(MonsterTypes[i].Loot));
		MonsterTypes[i].MonsterLevel = OLD_MonsterTypes[i].MonsterLevel;
		MonsterTypes[i].MovingSpeed = OLD_MonsterTypes[i].MovingSpeed;
		strcpy_s(MonsterTypes[i].Name, OLD_MonsterTypes[i].Name);
		MonsterTypes[i].num_Frames = OLD_MonsterTypes[i].num_Frames;
		MonsterTypes[i].Used = OLD_MonsterTypes[i].Used;	
	}

	for (int i = 0; i < OLD_m_NPCTypes; i++)
	{
//		NPCTypes[i].AC = OLD_NPCTypes[i].AC;
		NPCTypes[i].anim_speed = OLD_NPCTypes[i].anim_speed;
		NPCTypes[i].anim_Type = OLD_NPCTypes[i].anim_Type;
		NPCTypes[i].Animated = OLD_NPCTypes[i].Animated;
		NPCTypes[i].atk_speed = OLD_NPCTypes[i].atk_speed;
		NPCTypes[i].atk_type = OLD_NPCTypes[i].atk_type;
		NPCTypes[i].dam_max = OLD_NPCTypes[i].dam_max;
		NPCTypes[i].dam_min = OLD_NPCTypes[i].dam_min;
		NPCTypes[i].is_guard = OLD_NPCTypes[i].is_guard;
		NPCTypes[i].is_shop = OLD_NPCTypes[i].is_shop;
		NPCTypes[i].dex = OLD_NPCTypes[i].dex;
//		NPCTypes[i].ElementalAttack = OLD_NPCTypes[i].ElementalAttack;
//		NPCTypes[i].ElementalDefence = OLD_NPCTypes[i].ElementalDefence;
		NPCTypes[i].speed = OLD_NPCTypes[i].speed;
		memcpy(&NPCTypes[i].Frames, &OLD_NPCTypes[i].Frames, sizeof(NPCTypes[i].Frames));
//		memcpy(&NPCTypes[i].Loot, &OLD_NPCTypes[i].Loot, sizeof(NPCTypes[i].Loot));
		NPCTypes[i].Walking = OLD_NPCTypes[i].Walking;
		NPCTypes[i].WalkingRange = OLD_NPCTypes[i].WalkingRange;
		NPCTypes[i].WalkingType = OLD_NPCTypes[i].WalkingType;
		strcpy_s(NPCTypes[i].Name, OLD_NPCTypes[i].Name);
		NPCTypes[i].num_Frames = OLD_NPCTypes[i].num_Frames;
		NPCTypes[i].Used = OLD_NPCTypes[i].Used;	
	}

	char *data;

	fout.open("new_data/data03.dat", ios::binary);

	data = new char[sizeof(MonsterTypes)];

	memcpy(data, &MonsterTypes, sizeof(MonsterTypes));

	for (int i = 0; i < sizeof(MonsterTypes); i++)
	{
		data[i] ^= 212;	
	}

	fout.write((char *)data, sizeof(MonsterTypes));
	fout.close();
	delete[] data;


	fout.open("new_data/data04.dat", ios::binary);

	data = new char[sizeof(NPCTypes)];

	memcpy(data, &NPCTypes, sizeof(NPCTypes));

	for (int i = 0; i < sizeof(NPCTypes); i++)
	{
		data[i] ^= 212;	
	}

	fout.write((char *)data, sizeof(NPCTypes));
	fout.close();
	delete[] data;

	printf("sizeof(MonsterTypes) = %d, sizeof(NPCTypes) = %d\n", sizeof(MonsterTypes), sizeof(NPCTypes));

}