#include "main.h"


#pragma warning(disable:4018)

//===============================================
void NPCDatabase::drawnpc()
{
	int x1,y1,tx,ty,
		ox=pldatabase->own->x,
		oy=pldatabase->own->y,
		i2;

	
	NPCClass *tmp;
	np = npc;

	while (np)
	{
		tmp = np->next;

		np->anim_advance();

		tx = np->x;
		ty = np->y;
	
		if (ox - tx > 11 || ox - tx < -11 || oy - ty > 11 || oy - ty < -11)
		{
			np->removefrom();
		}
		else if ( MapInfo.Visible[-ox + tx + 10][-oy + ty + 10] )
		{
			x1 = -ox + tx + 10;
			y1 = -oy + ty + 10;

			i2 = np->frame;

			if (ox - tx > -11 && ox - tx < 11 && oy - ty > -11 && oy - ty < 11 && MapInfo.Visible[ x1 ][ y1 ] > 3)
				SDL_BlitSurface(G.avatars, &NPCTypes[ np->type ].Frames[ i2 ], Screen.screen, &sdl.Main[x1][y1]);
		}
		
		np = tmp;
	}
}
//===============================================



//===============================================
void NPCDatabase::clearlist()
{
	NPCClass *tmp;
	while (npc)
	{
		tmp = npc->next;
		delete npc;
		npc = tmp;
	}
	npc = NULL;
}
//===============================================


//===============================================
NPCClass *NPCDatabase::getbyid(short id)
{
	np = npc;
	while (np)
	{
		if (np->id == id)
			return np;
	
		np = np->next;
	}
	return NULL;
}
//===============================================


//===============================================
NPCClass *NPCDatabase::getlast()
{
	np = npc;
	while (np)
	{
		if (np->next == NULL)
			return np;
	
		np = np->next;
	}
	return NULL;
}
//===============================================



//===============================================
void NPCClass::anim_start()
{
	if (NPCTypes[type].Animated)
	{
		frame = rand() % NPCTypes[type].num_Frames;
		frametime = SDL_GetTicks();
	}
	else frame = 0;
}
//===============================================


//===============================================
void NPCClass::anim_advance()
{
	if (NPCTypes[type].Animated)
	{
		if (SDL_GetTicks() - frametime > 800 - rand() % 550)
		{
			if (NPCTypes[type].num_Frames != 0)
				frame = rand() % NPCTypes[type].num_Frames;
			else
				frame = 0;
	
			frametime = SDL_GetTicks();
		}
	}
	else frame = 0;
}
//===============================================


//===============================================
void NPCClass::addto()
{
	if (database->npc == NULL)
	{
		next = NULL;
		prev = NULL;
		database->npc = this;
	}
	else
	{
		next = database->npc;
		prev = NULL;
		database->npc->prev = this;
		database->npc = this;			
	}
}
//===============================================


//===============================================
void NPCClass::removefrom()
{
	if (database->npc == this)
	{
		if (database->npc->next != NULL)
			database->npc->next->prev = NULL;

		database->npc = database->npc->next;
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

