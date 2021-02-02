#include "main.h"

#pragma warning(disable:4018)

//===============================================
void PlayerDatabase::clearlist()
{
	if (players)
	{
		PlayerNode *tmp;
		while (players)
		{
			tmp = players->next;
			delete players;
			players = tmp;
		}
		players = NULL;
	}
	//if (own)
	//{
	//	delete own;
	//	own = NULL;
	//}
}
//===============================================


//===============================================
PlayerNode *PlayerDatabase::getbyid(short id)
{
	pl = players;
	while (pl)
	{
		if (pl->id == id)
			return pl;
	
		pl = pl->next;
	}
	return NULL;
}
//===============================================


//===============================================
PlayerNode *PlayerDatabase::getlast()
{
	pl = players;
	while (pl)
	{
		if (pl->next == NULL)
			return pl;
	
		pl = pl->next;
	}
	return NULL;
}
//===============================================


//===============================================
void PlayerDatabase::drawpl()
{
	int x1, y1, tx, ty,
		ox = own->x,
		oy = own->y,
		i1, i2, i3;

	PlayerNode *tmp;
	pl = players;

	while (pl)
	{
		tmp = pl->next;
		pl->anim_advance();

		tx = pl->x;
		ty = pl->y;

		x1 = -ox + tx + 10;
		y1 = -oy + ty + 10;

		if (ox - tx > 11 || ox - tx < -11 || oy - ty > 11 || oy - ty < -11)
		{
			//--------------------------------------------
			// This players is out of screen, remove him.
			//--------------------------------------------
			pl->removefrom();
		}
		else if ( MapInfo.Visible[ x1 ][ y1 ] > 3 && ox - tx < 11 && ox - tx > -11 && oy - ty < 11 && oy - ty > -11)
		{
			i1 = pl->avatar;
			i2 = pl->frame;
			i3 = pl->race;
				
			SDL_BlitSurface(G.avatars, &Races[i3].Avatars[i1 / 2][i1 % 2].Frames[i2], Screen.screen, &sdl.Main[x1][y1]);
		}
		pl = tmp;
	}
}
//===============================================


//===============================================
void PlayerDatabase::drawnames()
{
	int x1,y1,tx,ty,
		ox = own->x,
		oy = own->y,
		tmpcol;
	char tmp[120];

	pl = players;

	while (pl)
	{
		tx = pl->x;
		ty = pl->y;

		x1 = -ox + tx + 10;
		y1 = -oy + ty + 10;

		if ( MapInfo.Visible[ x1 ][ y1 ] > 3 && ox - tx < 11 && ox - tx > -11 && oy - ty < 11 && oy - ty > -11)
		{
			if (pl->wanted)
				tmpcol = col_gray;
			else tmpcol = pl->reputation;

			if (strlen(pl->tag) > 0)
				sprintf(tmp, "æ%s  Å%s", pl->tag, pl->name);
			else
				sprintf(tmp, "  Å%s", pl->name);

			if (Client.PlNames == true && pl != own) // Check Player Names toggle
			{
				sdl.text(tmp, x1 * 20 + 10 - BFont_TextWidth(pl->name) / 2, y1 * 20 -12, tmpcol);
			} else if (Client.OwnName == true && pl == own) // Check own name toggle
			{			
				sdl.text(tmp, x1 * 20 + 10 - BFont_TextWidth(pl->name) / 2, y1 * 20 -12, tmpcol);
			}
		}
		pl = pl->next;
	}
}
//===============================================




//===============================================
void PlayerNode::addto()
{
	if (database->players == NULL)
	{
		next = NULL;
		prev = NULL;
		database->players = this;
		database->own = database->players;
	}
	else
	{
		next = database->players;
		prev = NULL;
		database->players->prev = this;
		database->players = this;			
	}
}
//===============================================


//===============================================
void PlayerNode::removefrom()
{
	if (database->own == this)
		database->own = NULL;

	if (database->players == this)
	{
		if (database->players->next != NULL)
			database->players->next->prev = NULL;

		database->players = database->players->next;
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


//===============================================
void PlayerNode::anim_start()
{
	frame = rand() % 4;
	frametime = SDL_GetTicks();
}
//===============================================


//===============================================
void PlayerNode::anim_advance()
{
	if (SDL_GetTicks() - frametime > 800 - rand() % 550)
	{
		frame = rand() % Races[race].Avatars[avatar / 4][avatar % 4].NumFrames;
		frametime = SDL_GetTicks();
	}
}
//===============================================
