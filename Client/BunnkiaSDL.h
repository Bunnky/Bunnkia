#ifndef sdl_h
#define sdl_h

#include "main.h"


//------------------------------------------------------------------
//
class SDLClass
{
public:

	SDL_Surface * LoadImage(short width, short height, short depth, long starting_point);
	SDL_Surface * load_image(int i);
	SDL_Surface * create_backsurface(unsigned short width, unsigned short height, short bpp);

	long loaded;

	short mx, my;
	
	SDL_Rect Rect_addgf[80];

	SDL_Rect dst;
	SDL_Rect src;
	SDL_Rect oldst;

	SDL_Rect Main[21][21];
	SDL_Rect TileRect[m_Tiles];
	SDL_Rect CharRect[120];
	SDL_Rect cursors[20];

	short Cursor;
	short OldCursor;
	short UseCursorState;
	unsigned long UseCursorTime;

	SDL_Rect Avatars[80][4];

	void text(char const txt[120], short x, short y, short color);
	void draw(SDL_Surface *surface, short x, short y);
	void draw(SDL_Surface *surface, short x, short y, SDL_Rect *src);
	inline void set_rect(short x, short y, short w, short h)
	{
		dst.x = x;
		dst.y = y;
		dst.w = w;
		dst.h = h;
	}
};

extern SDLClass sdl;
//------------------------------------------------------------------


//------------------------------------------------------------------
//
class ScreenClass
{
public:

	bool Fullscreen;
	SDL_Surface *fscreen;
	SDL_Surface *screen;

	void Flush();
	bool SwitchFullscreen();
	void shot();
};

extern ScreenClass Screen;
//------------------------------------------------------------------


#endif