#include "main.h"

void MiniMap::createBack()
{
	int x, y, curchunk;
	x = pldatabase->own->x;
	y = pldatabase->own->y;
	
	if (SDL_MUSTLOCK(G.chunkback))
		SDL_LockSurface(G.chunkback);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (!MapInfo.MapVisibility[(x >> 4) + (i - 4)][(y >> 4) + (j - 4)])
				curchunk = 4095;
			else
				curchunk = Map.ChunkMap[(x >> 4) + (i - 4)][(y >> 4) + (j - 4)];

			sdl.src.x = (curchunk % 1024) << 4;
			sdl.src.y = (curchunk / 1024) << 4;
			sdl.src.h = 16;
			sdl.src.w = 16;

			sdl.dst.x = i << 4;
			sdl.dst.y = j << 4;
			sdl.dst.h = 16;
			sdl.dst.w = 16;

			SDL_BlitSurface(G.chunkback, &sdl.src, G.mmChunkmap, &sdl.dst);
		}
	}
	if (SDL_MUSTLOCK(G.chunkback))
		SDL_UnlockSurface(G.chunkback);

	SDL_UpdateRect(G.mmChunkmap, 0, 0, 0, 0);
}

void MiniMap::draw()
{
	sdl.dst.x = Minimap->X;
	sdl.dst.y = Minimap->Y;	
	sdl.dst.h = Minimap_h; 
	sdl.dst.w = Minimap_w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[MINIMAP], Screen.screen, &sdl.dst);

	int x, y;
	x = pldatabase->own->x;
	y = pldatabase->own->y;

	sdl.src.x = 64 + (x % 16) - 49;
	sdl.src.y = 64 + (y % 16) - 49;
	sdl.src.h = 101;
	sdl.src.w = 101;

	sdl.dst.x = X + 3;
	sdl.dst.y = Y + 3;
	sdl.dst.h = 101;
	sdl.dst.w = 101;

	SDL_BlitSurface(G.mmChunkmap, &sdl.src, Screen.screen, &sdl.dst);

	PutPixel(Screen.screen, Minimap->X + 55, Minimap->Y + 55, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	PutPixel(Screen.screen, Minimap->X + 54, Minimap->Y + 54, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	PutPixel(Screen.screen, Minimap->X + 56, Minimap->Y + 54, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	PutPixel(Screen.screen, Minimap->X + 54, Minimap->Y + 56, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	PutPixel(Screen.screen, Minimap->X + 56, Minimap->Y + 56, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
}


void MiniMap::drawFull()
{
	int x, y, curchunk;
	x = pldatabase->own->x;
	y = pldatabase->own->y;

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 37; j++)
		{
			if (!MapInfo.MapVisibility[(x >> 4) + (i - 24)][(y >> 4) + (j - 18)])
				curchunk = 4095;
			else
				curchunk = Map.ChunkMap[(x >> 4) + (i - 24)][(y >> 4) + (j - 18)];

			sdl.src.x = (curchunk % 1024) << 4;
			sdl.src.y = (curchunk / 1024) << 4;
			sdl.src.h = 16;
			sdl.src.w = 16;

			sdl.dst.x = (i << 4);
			sdl.dst.y = (j << 4) + 4;
			sdl.dst.h = 16;
			sdl.dst.w = 16;

			SDL_BlitSurface(G.chunkback, &sdl.src, Screen.screen, &sdl.dst);						
		}
	}
}

MiniMap::MiniMap()
{
	Size = false;
}


MiniMap::~MiniMap()
{

}