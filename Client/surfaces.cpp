#include "main.h"


void GraphsClass::load_fonts()
{
	fonts[col_black] = BFont_LoadFont("font.dat");
	fonts[col_black] = BFont_SetFontColor(fonts[col_black], 254, 0, 0);

	fonts[col_red] = BFont_LoadFont("font.dat");
	fonts[col_red] = BFont_SetFontColor(fonts[col_red], 190, 0, 0);

	fonts[col_darkred] = BFont_LoadFont("font.dat");
	fonts[col_darkred] = BFont_SetFontColor(fonts[col_darkred], 160, 0, 0);

	fonts[col_orange] = BFont_LoadFont("font.dat");
	fonts[col_orange] = BFont_SetFontColor(fonts[col_orange], 150, 150, 7);

	fonts[col_yellow] = BFont_LoadFont("font.dat");
	fonts[col_yellow] = BFont_SetFontColor(fonts[col_yellow], 200, 200, 10);

	fonts[col_greenish] = BFont_LoadFont("font.dat");
	fonts[col_greenish] = BFont_SetFontColor(fonts[col_greenish], 0, 140, 0);

	fonts[col_green] = BFont_LoadFont("font.dat");
	fonts[col_green] = BFont_SetFontColor(fonts[col_green], 0, 190, 0);

	fonts[col_grey] = BFont_LoadFont("font.dat");
	fonts[col_grey] = BFont_SetFontColor(fonts[col_grey], 0, 220, 0);

	fonts[col_white] = BFont_LoadFont("font.dat");
	fonts[col_white] = BFont_SetFontColor(fonts[col_white], 220, 220, 220);

	fonts[col_blue] = BFont_LoadFont("font.dat");
	fonts[col_blue] = BFont_SetFontColor(fonts[col_blue], 50, 50, 255);

	fonts[col_gray] = BFont_LoadFont("font.dat");
	fonts[col_gray] = BFont_SetFontColor(fonts[col_gray], 120, 120, 120);
	
	fonts[col_purple] = BFont_LoadFont("font.dat");
	fonts[col_purple] = BFont_SetFontColor(fonts[col_purple], 200, 0, 255);

	fonts[col_lightblue] = BFont_LoadFont("font.dat");
	fonts[col_lightblue] = BFont_SetFontColor(fonts[col_lightblue], 100, 100, 255);
	
	fonts[col_darkpurple] = BFont_LoadFont("font.dat");
	fonts[col_darkpurple] = BFont_SetFontColor(fonts[col_darkpurple], 165, 0, 190);

	BFont_SetCurrentFont(fonts[col_white]);
}
