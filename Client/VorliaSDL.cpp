#include "main.h"

extern int GameState;

void SDLClass::text(char const txt[120], short x, short y, short color)
{
	if (color > 19 || color < 0) return;

	short font = color;
	short len = strlen(txt);
	for (int i = 0; i < len; i++)
	{
		if (txt[i] == 'ô') font = col_gray;
		else if (txt[i] == 'Æ') font = col_red;
		else if (txt[i] == 'æ') font = col_blue;
		else if (txt[i] == 'É') font = col_white;
		else if (txt[i] == 'Ä') font = col_purple;
		else if (txt[i] == 'ì') font = col_orange;
		else if (txt[i] == 'ò') font = col_yellow;
		else if (txt[i] == 'î') font = col_darkpurple;
		else if (txt[i] == 'ï') font = col_lightblue;
		else if (txt[i] == 'Å') font = color;
		else if (txt[i] == '¥') font = col_green;
		
		else
		x  += BFont_PutCharFont(Screen.screen,G.fonts[font],x, y, txt[i]);
	}
}

void SDLClass::draw(SDL_Surface *surface, short x, short y)
{
	dst.x = x;
	dst.y = y;
	dst.w = surface->w;
	dst.h = surface->h;
	SDL_BlitSurface(surface, NULL, Screen.screen, &dst);
}

void SDLClass::draw(SDL_Surface *surface, short x, short y, SDL_Rect *src)
{
	dst.x = x;
	dst.y = y;
	dst.w = src->w;
	dst.h = src->h;
	SDL_BlitSurface(surface, src, Screen.screen, &dst);
}


//		if (text != "ô" && text != "Æ" && text != "æ" && text != "É" && text != "Ä" && text != "Å" && text != "ì" && text != "î" && text != "ï")

bool ScreenClass::SwitchFullscreen()
{
//	SDL_QuitSubSystem(SDL_INIT_VIDEO);
//	SDL_InitSubSystem(SDL_INIT_VIDEO);

	if (Fullscreen)
	{
//		SDL_WM_ToggleFullScreen(fscreen);
		fscreen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE);
		Fullscreen = false;
	}
	else
	{
//		SDL_WM_ToggleFullScreen(fscreen);
		fscreen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE|SDL_FULLSCREEN);
		Fullscreen = true;
	}

	return true;
}


void ScreenClass::Flush() 
{ 
//	memcpy(&fscreen->pixels, &screen->pixels, sizeof(screen->pixels));
	SDL_BlitSurface(screen, NULL, fscreen, NULL);
//	if (GameState != 5)
	SDL_Flip(fscreen);
//	else
//	SDL_UpdateRect(fscreen, 0, 0, 0, 0);
}

void ScreenClass::shot()
{
	bool fFound = false;
	short fNumber = -1;
	short sNumber = 0;
	ifstream File;
	char fPath[100];	

	short hunds = 0, tens = 0, ones = 0;


	while (!fFound && sNumber < 1000) 
	{
		memset(&fPath, 0, sizeof(fPath));
		sprintf(fPath, "Screenshots\\Eldoria%03d.bmp", sNumber);
		
		File.open(fPath, ios::in);
		if(!File)
		{
			//dont create
		}
		else
		{
			File.close();
			File.open(fPath, ios_base::out); // reopen for write
		}
		if (!File.is_open())
		{
			fNumber = sNumber;
			fFound = true;
		}
		else
			sNumber++;

		if (sNumber == 1000)
		{
			MB.NewMessage("1000 files in screenshot directory, move some.", 2);
		}

		if (File.is_open())
			File.close();
	}

	if (!fFound)
		return;



	SDL_Surface *tmp;
	tmp = sdl.create_backsurface(800, 600, 24);
	if (tmp == NULL) return;

	SDL_BlitSurface(fscreen, NULL, tmp, NULL);

	SDL_SaveBMP(tmp, fPath);

	SDL_FreeSurface(tmp);

	char tmp2[150];
	memset(&tmp2, 0, sizeof(tmp2));

	sprintf(tmp2, "Screenshot: %s", fPath);
	MB.NewMessage(tmp2, 2);
}


SDL_Surface *SDLClass::create_backsurface(unsigned short width, unsigned short height, short bpp)
{
	unsigned long rmask, gmask, bmask, amask;
	SDL_Surface *tmp, *tmp2;
	
	if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	else
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}

	tmp = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, bpp, rmask, gmask, bmask, amask);

	if (tmp == NULL)
		return NULL;

	tmp2 = SDL_DisplayFormat( tmp );
	SDL_FreeSurface( tmp );

	return tmp2;
}

