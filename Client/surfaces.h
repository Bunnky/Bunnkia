#ifndef surfaces_h
#define surfaces_h



//####################
// Bunnkia color tables:
// 0-Evil (bonus reputation)
// 1-Red (Evil)
// 2-Dark Red
// 3-Orange
// 4-Yellow (Neutral)
// 5-Slightly green
// 6-Green
// 7-Grey (Good)
// 8-White (bonus reputation)
// 10-Blue (GM)
//####################


#define col_black		0
#define col_red			1
#define col_darkred		2
#define col_orange		3
#define col_yellow		4
#define col_greenish	5
#define col_green		6
#define col_grey		7
#define col_white		8
#define col_blue		10
#define col_gray		11
#define col_purple		12
#define col_lightblue	13
#define col_darkpurple	14


class GraphsClass
{
public:
	SDL_Surface *tiles;
	SDL_Surface *avatars;
	SDL_Surface *layout;
	SDL_Surface *addgf;
	SDL_Surface *items;
	SDL_Surface *skills;
	SDL_Surface *shop;
	SDL_Surface *Identify;
	SDL_Surface *List;
	SDL_Surface *Cursors;
	SDL_Surface *objects;
	SDL_Surface *MBExpanded;

	SDL_Surface *chunkback;
	SDL_Surface *mmChunkmap;

	BFont_Info *fonts[20];

	void load_fonts();
};
extern GraphsClass G;



#endif