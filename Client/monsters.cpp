#include "main.h"

#pragma warning(disable:4018)
//===============================================
void MonsterDatabase::drawmon()
{
	int x1, y1, tx, ty,
		ox = pldatabase->own->x,
		oy = pldatabase->own->y, i2;
	
	MonsterClass *tmp;
	mon = monsters;

	while (mon)
	{
		tmp = mon->next;

		mon->anim_advance();

		tx = mon->x;
		ty = mon->y;

		if (ox - tx > 11 || ox - tx < -11 || oy - ty > 11 || oy - ty < -11)
		{
			mon->removefrom();
		}
		else if (ox - tx < 11 && ox - tx > -11 && oy - ty < 11 && oy - ty > -11)
		{
			x1 = -ox + tx + 10;
			y1 = -oy + ty + 10;
							
			i2 = mon->frame;

			if (ox - tx > -11 && ox - tx < 11 && oy - ty > -11 && oy - ty < 11 && MapInfo.Visible[ x1 ][ y1 ] > 3)
				SDL_BlitSurface(G.avatars, &MonsterTypes[ mon->type ].Frames[ i2 ] , Screen.screen, &sdl.Main[x1][y1]);
		}
		
		mon = tmp;
	}
}
//===============================================


//===============================================
void MonsterDatabase::clearlist()
{
	MonsterClass *tmp;
	while (monsters)
	{
		tmp = monsters->next;
		delete monsters;
		monsters = tmp;
	}
	monsters = NULL;
}
//===============================================


//===============================================
MonsterClass *MonsterDatabase::getbyid(short id)
{
	mon = monsters;
	while (mon)
	{
		if (mon->knowid == id)
			return mon;
	
		mon = mon->next;
	}
	return NULL;
}
//===============================================


//===============================================
MonsterClass *MonsterDatabase::getlast()
{
	mon = monsters;
	while (mon)
	{
		if (mon->next == NULL)
			return mon;
	
		mon = mon->next;
	}
	return NULL;
}
//===============================================



//===============================================
void MonsterClass::anim_start()
{
	if (MonsterTypes[type].Animated)
	{
		frame = rand() % MonsterTypes[type].num_Frames;
		frametime = SDL_GetTicks();
	}
	else frame = 0;
}
//===============================================


//===============================================
void MonsterClass::anim_advance()
{
	if (MonsterTypes[type].Animated)
	{
		if (SDL_GetTicks() - frametime > 800 - rand() % 550)
		{
			if (MonsterTypes[type].num_Frames != 0)
				frame = rand() % MonsterTypes[type].num_Frames;
			else
				frame = 0;
	
			frametime = SDL_GetTicks();
		}
	}
	else frame = 0;
}
//===============================================


//===============================================
void MonsterClass::addto()
{
	if (database->monsters == NULL)
	{
		next = NULL;
		prev = NULL;
		database->monsters = this;
	}
	else
	{
		next = database->monsters;
		prev = NULL;
		database->monsters->prev = this;
		database->monsters = this;			
	}
}
//===============================================


//===============================================
void MonsterClass::removefrom()
{
	if (database->monsters == this)
	{
		if (database->monsters->next != NULL)
			database->monsters->next->prev = NULL;

		database->monsters = database->monsters->next;
	}
	else
	{
		if (next != NULL)
			next->prev = prev;
		if (prev != NULL)
			prev->next = next;
	}
	
	delete this;
}
//===============================================
