#include "main.h"


//===============================================
void BodyDatabase::drawbody()
{
	int x1, y1, tx, ty,
		ox = pldatabase->own->x,
		oy = pldatabase->own->y, i2;

	BodyClass *tmp;
	bd = body;

	while (bd)
	{
		tmp = bd->next;

		tx = bd->x;
		ty = bd->y;

		if (ox - tx > 11 || ox - tx < -11 || oy - ty > 11 || oy - ty < -11)
		{
			bd->removefrom();
		}
		else if (ox - tx < 11 && ox - tx > -11 && oy - ty < 11 && oy - ty > -11)
		{
			x1 = -ox + tx + 10;
			y1 = -oy + ty + 10;
								
			i2 = BODY_DECAY1 + bd->decaystate;

			sdl.dst.x = x1 * 20 + 7;
			sdl.dst.y = y1 * 20 + 7;
			sdl.dst.h = 20;
			sdl.dst.w = 20;	

			if (ox - tx > -11 && ox - tx < 11 && oy - ty > -11 && oy - ty < 11 && MapInfo.Visible[ x1 ][ y1 ] > 3)
				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ i2 ], Screen.screen, &sdl.dst);
		}		
	
		bd = tmp;
	}
}
//===============================================



//===============================================
void BodyDatabase::clearlist()
{
	if (body)
	{
		BodyClass *tmp;
		while (body)
		{
			tmp = body->next;
			delete body;
			body = tmp;
		}
		body = NULL;
	}
}
//===============================================


//===============================================
BodyClass *BodyDatabase::getbyid(short id)
{
	bd = body;
	while (bd)
	{
		if (bd->id == id)
			return bd;
	
		bd = bd->next;
	}
	return NULL;
}
//===============================================


//===============================================
BodyClass *BodyDatabase::getlast()
{
	bd = body;
	while (bd)
	{
		if (bd->next == NULL)
			return bd;
	
		bd = bd->next;
	}
	return NULL;
}
//===============================================


//===============================================
void BodyClass::addto()
{
	if (database->body == NULL)
	{
		next = NULL;
		prev = NULL;
		database->body = this;
	}
	else
	{
		next = database->body;
		prev = NULL;
		database->body->prev = this;
		database->body = this;			
	}
}
//===============================================


//===============================================
void BodyClass::removefrom()
{
	if (database->body == this)
	{
		if (database->body->next != NULL)
			database->body->next->prev = NULL;

		database->body = database->body->next;
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