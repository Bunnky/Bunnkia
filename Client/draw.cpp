
#include "main.h"

#pragma warning(disable:4244)

extern SDL_Event event;

extern unsigned char GMMode;
extern unsigned char Invisibility;
extern unsigned char ListenNormal;
extern unsigned char ListenWhisper;
extern unsigned char ListenGuild;
extern unsigned char WaterFrame;

extern int GameState;
extern int MX, MY;
extern int ChosenSkill;

extern bool Tab;
extern bool Shift;
extern bool Identify;
extern bool BrowseSkills;

extern unsigned long WaterFrameTime;
extern unsigned long ExpTables[MaxLevel];
extern unsigned long SkillExpTables[MaxLevel];

unsigned long ObjectFrametime;

unsigned char Frames[3][10];
//==============================================================================
//
//	Draw the screen
//
//==============================================================================
void DrawClass::DrawBG()
{
//	memcpy(&Screen.screen->pixels, &G.layout->pixels, sizeof(G.layout->pixels));
	SDL_BlitSurface(G.layout, NULL, Screen.screen, NULL);
}

unsigned char WaterposFrames[4][4];

//==============================================================================
//
//	Draw the actual world
//
//==============================================================================
void DrawClass::DrawWorld()
{
	int x,y;
	int x1,y1;

	x1 = pldatabase->own->x;
	y1 = pldatabase->own->y;

	int i, j;
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 21; j++)
		{
			x = x1 - 10 + i;
			y = y1 - 10 + j;

			if (Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] == 1 || Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] == 728)
				SDL_BlitSurface(G.tiles, &sdl.TileRect[ Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] + Frames[2][4] ]/*WaterposFrames[ (x % 16) % 4 ][ (y % 16) % 4] ]*/, Screen.screen, &sdl.Main[i][j]);
			else
				SDL_BlitSurface(G.tiles, &sdl.TileRect[ Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] ], Screen.screen, &sdl.Main[i][j]);

			if (Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] != 0)
			{
				SDL_BlitSurface(G.tiles, &sdl.TileRect[ Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ] ] [ x % 16 ] [ y % 16 ] ], Screen.screen, &sdl.Main[i][j]);
			}

			if (MapInfo.Visible[i][j] < 4)
				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[LOS], Screen.screen, &sdl.Main[i][j]);

		}
	} 

	short hshx[9], hshy[9];	
	hshx[0] = pldatabase->own->x / 32;
	hshy[0] = pldatabase->own->y / 32;
	hshx[1] = hshx[0] -1;
	hshy[1] = hshy[0] -1; 
	hshx[2] = hshx[0] -1;
	hshy[2] = hshy[0]; 
	hshx[3] = hshx[0] -1;
	hshy[3] = hshy[0] +1; 
	hshx[4] = hshx[0];
	hshy[4] = hshy[0] -1; 
	hshx[5] = hshx[0];
	hshy[5] = hshy[0] +1; 
	hshx[6] = hshx[0] +1;
	hshy[6] = hshy[0] -1; 
	hshx[7] = hshx[0] +1;
	hshy[7] = hshy[0]; 
	hshx[8] = hshx[0] +1;
	hshy[8] = hshy[0] +1;

	short ox, oy;

	for (i = 0; i < 9; i++)
	{
		if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 256 && hshy[i] < 256)
		{
			obj_List = Objects[hshx[i]][hshy[i]];
	
			while (obj_List != NULL)
			{
				ox = obj_List->X;
				oy = obj_List->Y;

				if (IsInRange(ox, oy, pldatabase->own->x, pldatabase->own->y))
				{
					if (x1 - ox >= -10 && x1 - ox <= 10 &&
						y1 - oy >= -10 && y1 - oy <= 10 && obj_List->On && MapInfo.Visible[-x1 + ox + 10][ -y1 + oy + 10] > 3)
					{
						SDL_BlitSurface(G.objects, &ObjectTypes[ obj_List->Type ].Frames[ Frames[ObjectTypes[obj_List->Type].AnimationType][ObjectTypes[obj_List->Type].num_Frames ]], Screen.screen, &sdl.Main[-x1 + ox + 10][-y1 + oy + 10]);
					}
				}
				obj_List = obj_List->next;
			}
		}
	}

}





void CheckFrame()
{
	if ( SDL_GetTicks() - ObjectFrametime > 100)
	{
		int i;
		for (i = 1; i < 10; i++)
		{
			Frames[2][i]++;
			if (Frames[2][i] >= i) Frames[2][i] = 0;

			Frames[1][i] = rand() % i;
			Frames[0][i] = 3;
		}
		
		ObjectFrametime = SDL_GetTicks();
	}
}





//==============================================================================
//
//	Draw the FPS [LOL] + some other things
//
//==============================================================================
void DrawClass::DrawFPS()
{
	char tmp[120];
	
	sprintf(tmp, "FPS: %d", Client.FPS);
	sdl.text(tmp, 375, 8, col_white);	

	//------------------------------------------------------------------------------
	//	Draw some info text on the screen
	//------------------------------------------------------------------------------
	if (!Writing.Writing)
	{
		
		if (Shift)
			sdl.text("unpile", 8, 415, col_white);
		else sdl.text("unpile", 8, 415, col_gray);
		
		if (Identify)
			sdl.text("identify", 380, 415, col_purple);
		else
			sdl.text("identify", 380, 415, col_gray);

		if (Client.AttackMode == PEACE)
			sdl.text("normal", 380, 403, col_green);
		else sdl.text("combat", 380, 403, col_red);	
		
	}

	//------------------------------------------------------------------------------
	//	Draw GMMode text on the screen
	//------------------------------------------------------------------------------
	if (GMMode > 1)
	{
		sdl.text("GM Mode", 580, 218, col_green);

		if (Invisibility)
			sdl.text("Invisible", 580, 206, col_green);

		if (ListenNormal == 1)
			sdl.text("Listen normal", 580, 194, col_green);

		if (ListenWhisper == 1)
			sdl.text("Listen whisper", 580, 182, col_green);

		if (ListenGuild == 1)
			sdl.text("Listen guild", 580, 170, col_green);

	}

	//------------------------------------------------------------------------------
	//	Draw chat up/down buttons
	//------------------------------------------------------------------------------
		// Up
		sdl.dst.x = 10;
		sdl.dst.y = 435;
		sdl.dst.h = 15;
		sdl.dst.w = 15;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);

		// Down
		sdl.dst.x = 10;
		sdl.dst.y = 575;
		sdl.dst.h = 15;
		sdl.dst.w = 15;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);

	//------------------------------------------------------------------------------
	//	Draw pidgeon if in Safe Zone
	//------------------------------------------------------------------------------
	if (SafeZones[ pldatabase->own->x >> 4 ][ pldatabase->own->y >> 4 ] > 0)
	{
		sdl.dst.x = 775;
		sdl.dst.y = 260;
		sdl.dst.h = 16;
		sdl.dst.w = 16;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ PIDGEON ], Screen.screen, &sdl.dst);
	}

	//------------------------------------------------------------------------------
	//	Draw target protection on/off
	//------------------------------------------------------------------------------
	if (Client.TargetProtectionMode)
	{
		sdl.dst.x = 775;
		sdl.dst.y = 280;
		sdl.dst.h = 16;
		sdl.dst.w = 16;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ tgprotection ], Screen.screen, &sdl.dst);
	}

	if (MX >= 775 && MX <= 791 &&
		MY >= 280 && MY <= 296 && Client.TargetProtectionMode)
	{
		int len;
		Uint32 Color = SDL_MapRGB(Screen.screen->format, 0, 0, 0);
		len = BFont_TextWidth("Target protection");

		int x, y;
		x = MX + 10;
		y = MY + 15;

		if (x + len > 800) x = 800 - len;
		if (y > 590) y = 590;

		sdl.set_rect(x, y, len, 10);
			
		SDL_FillRect(Screen.screen, &sdl.dst, Color);
		sdl.text("Target protection", x, y, col_white);
	} else
	if (MX >= 775 && MX <= 791 &&
		MY >= 260 && MY <= 276 && SafeZones[ pldatabase->own->x >> 4 ][ pldatabase->own->y >> 4 ] > 0)
	{
		int len;
		Uint32 Color = SDL_MapRGB(Screen.screen->format, 0, 0, 0);
		len = BFont_TextWidth("Guarded zone");

		int x, y;
		x = MX + 10;
		y = MY + 15;

		if (x + len > 800) x = 800 - len;
		if (y > 590) y = 590;

		sdl.set_rect(x, y, len, 10);
		
		SDL_FillRect(Screen.screen, &sdl.dst, Color);
		sdl.text("Guarded zone", x, y, col_white);
	}

	//------------------------------------------------------------------------------
	//	Draw skill orbs when level-up
	//------------------------------------------------------------------------------
	if (P.StatPoints > 0)
	{
		if (SDL_GetTicks() - Draw.OrbStateTime > 150)
		{
			if (Draw.OrbStateChange != -1 && Draw.OrbStateChange != 1) Draw.OrbStateChange = 1;
			Draw.OrbState += Draw.OrbStateChange;
			if (Draw.OrbState > 2) Draw.OrbStateChange = -1;
			if (Draw.OrbState < 1) Draw.OrbStateChange = 1;
			Draw.OrbStateTime = SDL_GetTicks();
		}

		if (!shop.on)
		{
			// Stat orbs.
		
			short stats[4];
			stats[0] = P.Str;
			stats[1] = P.Dex;
			stats[2] = P.Con;
			stats[3] = P.Int;

			int i;
			for (i = 0; i < 4; i++)
			{
				if (stats[i] < 100)
				{
					sdl.dst.x = 435;
					sdl.dst.y = 115 + i * 12;
					sdl.dst.h = 8;
					sdl.dst.w = 4;

					SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ORB + Draw.OrbState], Screen.screen, &sdl.dst);
				}
			}		
		}
        
		//------------------------------------------------------------------------------
		//	Draw amount of orbs player has.
		//------------------------------------------------------------------------------
		sdl.dst.x = 760;
		sdl.dst.y = 15;
		sdl.dst.h = 8;
		sdl.dst.w = 4;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ORB + Draw.OrbState], Screen.screen, &sdl.dst);
		sprintf(tmp, "%d", P.StatPoints);
		sdl.text(tmp, 764, 14, col_white);
//		BFont_PrintStringFont(Screen.screen, F.white, 764, 14, "%d", P.StatPoints);

	}

	//------------------------------------------------------------------------------
	//	Draw close [x] button
	//------------------------------------------------------------------------------
	sdl.dst.x = 793;
	sdl.dst.y = 0;
	sdl.dst.h = 7;
	sdl.dst.w = 7;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[EXIT], Screen.screen, &sdl.dst);

	SDL_Rect src;
	src.x = 0;
	src.y = 313;
	src.h = 4;

	int HowMany;

	sdl.dst.x = 439;
	sdl.dst.y = 232;
	sdl.dst.h = sdl.Rect_addgf[ alig_1 ].h;
	sdl.dst.w = sdl.Rect_addgf[ alig_1 ].w;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[alig_1], Screen.screen, &sdl.dst);

	HowMany = ((float)P.KilledMonsters / 8000) * 346;

	if (HowMany > 346) HowMany = 346;

	sdl.dst.x = 439 + HowMany;
	sdl.dst.y = 232;
	sdl.dst.h = 8;
	sdl.dst.w = 3;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[alig_pointer], Screen.screen, &sdl.dst);

	//------------------------------------------------------------------------------
	//	Draw exp meter and alignment bar 
	//------------------------------------------------------------------------------
	sdl.dst.x = 439;
	sdl.dst.y = 239;
	sdl.dst.h = sdl.Rect_addgf[ expo_1 ].h;
	sdl.dst.w = sdl.Rect_addgf[ expo_1 ].w;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[expo_1], Screen.screen, &sdl.dst);

	sdl.dst.x = 440;
	sdl.dst.y = 240;


	long Temp;
	Temp = ExpTables[P.Level + 1] - P.Exp;

	if (Temp < 0)
	{
		HowMany = 346;
	}
	else
	{
		HowMany = 346 * (((double)P.Exp - ExpTables[P.Level]) / (ExpTables[P.Level +1]-ExpTables[P.Level]));
	}
	
	if (HowMany > 346) HowMany = 346;

	src.w = HowMany;

	sdl.dst.h = src.h;
	sdl.dst.w = src.w;

	SDL_BlitSurface(G.addgf, &src, Screen.screen, &sdl.dst);
}

//==============================================================================
//
//	Draw player corpse
//
//==============================================================================
void DrawClass::DrawOpenBody()
{
	BodyClass *body;
	body = bodydatabase->getbyid(v_Body_Open);
	if (body == NULL) return;

	sdl.dst.x = v_Body.X; sdl.dst.y = v_Body.Y; sdl.dst.h = Backpack_h; sdl.dst.w = Backpack_w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BACKPACK], Screen.screen, &sdl.dst);

	int x1 = -BFont_TextWidth("Body") / 2 + Backpack_w / 2;


	sdl.text("Body", v_Body.X + x1, v_Body.Y + 4, col_white);

	int i;
	for (i = 0; i < 84; i++)
	{
		if (body->Items[i].Used)
		{
			sdl.dst.x = body->Items[i].x + v_Body.X + 3;	
			sdl.dst.y = body->Items[i].y + v_Body.Y + 17;	
			sdl.dst.h = 20;
			sdl.dst.w = 20;

			DrawItem(body->Items[i].Family, body->Items[i].Type, body->Items[i].ID);
		}
	}
}


//==============================================================================
//
//	Draw backpack
//
//==============================================================================
void DrawClass::DrawBackpack()
{
	sdl.dst.x = Backpack.X; sdl.dst.y = Backpack.Y; sdl.dst.h = Backpack_h; sdl.dst.w = Backpack_w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BACKPACK], Screen.screen, &sdl.dst);

	int x1 = -BFont_TextWidth("Backpack") / 2 + Backpack_w / 2;

	sdl.text("Backpack", Backpack.X + x1, Backpack.Y + 4, col_white);

	if (Backpack.Docked)
	{
		sdl.dst.x = Backpack.X + 4;
		sdl.dst.y = Backpack.Y + 4;
        
		sdl.dst.h = 13;
		sdl.dst.w = 11;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[LOCK], Screen.screen, &sdl.dst);
	}

	int i;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpack[i].Used)
		{
			sdl.dst.x = iBackpack[i].x + Backpack.X + 3;	
			sdl.dst.y = iBackpack[i].y + Backpack.Y + 17;	
			sdl.dst.h = 20;
			sdl.dst.w = 20;
	
			DrawItem( iBackpack[i].Family, iBackpack[i].Type, iBackpack[i].ID);
		}
	}
}





//==============================================================================
//
//	Draw items
//
//==============================================================================
void DrawItem( short family, short type, short ID)
{	
	if (family == FAMILY_WEAPON)
	{
		if (Weapons[type][ID].Animated)
			SDL_BlitSurface(G.items, &Weapons[ type ][ ID ].Frames[ Frames[Weapons[ type ][ ID ].anim_Type][ Weapons[ type ][ ID ].num_Frames] ], Screen.screen, &sdl.dst);
		else
			SDL_BlitSurface(G.items, &Weapons[ type ][ ID ].Frames[ 0 ], Screen.screen, &sdl.dst);
	}

	if (family == FAMILY_ARMOR)
	{
		if (Armors[type-11][ID].Animated)
			SDL_BlitSurface(G.items, &Armors[ type -11 ][ ID ].Frames[ Frames[Armors[ type -11 ][ ID ].anim_Type][Armors[ type -11 ][ ID ].num_Frames] ], Screen.screen, &sdl.dst);
		else
			SDL_BlitSurface(G.items, &Armors[ type -11 ][ ID ].Frames[ 0 ], Screen.screen, &sdl.dst);
	}

	if (family == FAMILY_COLLECTABLE)
	{
		if (Collectables[ID].Animated)
			SDL_BlitSurface(G.items, &Collectables[ ID ].Frames[ Frames[Collectables[ ID ].anim_Type][Collectables[ ID ].num_Frames] ], Screen.screen, &sdl.dst);
		else
			SDL_BlitSurface(G.items, &Collectables[ ID ].Frames[ 0 ], Screen.screen, &sdl.dst);
	}

	if (family == FAMILY_USEABLE)
	{
		if (Useables[ ID ].Animated)
			SDL_BlitSurface(G.items, &Useables[ ID ].Frames[ Frames[Useables[ ID ].anim_Type][Useables[ ID ].num_Frames] ], Screen.screen, &sdl.dst);
		else
			SDL_BlitSurface(G.items, &Useables[ ID ].Frames[ 0 ], Screen.screen, &sdl.dst);
	}
}





//==============================================================================
//
//	Draw ground items
//
//==============================================================================
void DrawClass::DrawGroundItems()
{
	int i;
	for (i = 0; i < MaxItems; i++)
	{
		if (iGround[i].Used)
		{
			if (pldatabase->own->x - iGround[i].x > 11 ||
				pldatabase->own->x - iGround[i].x < -11 ||
				pldatabase->own->y - iGround[i].y > 11 ||
				pldatabase->own->y - iGround[i].y < -11)
			{
		        Item_Remove( i, ItemGround );
			}
			else
			if (pldatabase->own->x - iGround[i].x < 11 &&
				pldatabase->own->x - iGround[i].x > -11 &&
				pldatabase->own->y - iGround[i].y < 11 &&
				pldatabase->own->y - iGround[i].y > -11 && MapInfo.Visible[-pldatabase->own->x + iGround[i].x + 10][-pldatabase->own->y + iGround[i].y + 10] > 3)
			{
				sdl.dst.x = (-pldatabase->own->x + iGround[i].x + 10) * 20 + 7;
				sdl.dst.y = (-pldatabase->own->y + iGround[i].y + 10) * 20 + 7;
				sdl.dst.h = 20;
				sdl.dst.w = 20;

				DrawItem(iGround[i].Family, iGround[i].Type, iGround[i].ID);
			}
		}
	}
}


//==============================================================================
//
//	Draw splitting stack
//
//==============================================================================
void DrawClass::DrawSplit()
{
	sdl.dst.x = 305;
	sdl.dst.y = 250;
	sdl.dst.h = 50;
	sdl.dst.w = 160;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[SPLIT], Screen.screen, &sdl.dst);

	sdl.text(Dragging.Txt, 330 - BFont_TextWidth(Dragging.Txt)/2, 265, col_white);
}


//==============================================================================
//
//	Draw target box
//
//==============================================================================
void DrawClass::DrawTarget()
{
	if (SDL_GetTicks() - Target.FrameTime > 100)
	{
		if (Target.FramePlus == 0 && Target.Frame < 3) Target.FramePlus = 1;
		if (Target.FramePlus == 0 && Target.Frame == 3) Target.FramePlus = -1;		
		Target.Frame += Target.FramePlus;
		if (Target.Frame > 2) Target.FramePlus = -1;
		if (Target.Frame == 0) Target.FramePlus = 1;
		Target.FrameTime = SDL_GetTicks();
	}

	if (Target.Type == PLAYER && Target.PlTarget != NULL)
	{
		int x1,y1,ox,oy;

		ox = pldatabase->own->x;
		oy = pldatabase->own->y;

		
		x1 = Target.PlTarget->x;
		y1 = Target.PlTarget->y;
	
		if (ox - x1 > -12 && ox - x1 < 12 && oy - y1 > -12 && oy - y1 < 12)
		{
			if (ox - x1 > -11 && ox - x1 < 11 && oy - y1 > -11 && oy - y1 < 11)
			{
				sdl.dst.x = (-pldatabase->own->x + Target.PlTarget->x + 10) * 20 + 6;
				sdl.dst.y = (-pldatabase->own->y + Target.PlTarget->y + 10) * 20 + 6;		
				sdl.dst.h = 22;
				sdl.dst.w = 22;
		
				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[TARGET + Target.Frame], Screen.screen, &sdl.dst);

				int 
				HowMany = 0;
				HowMany = 20 * ((double)Target.PlTarget->hpleft / (double)Target.PlTarget->hpmax);

                int i2;
                for (i2 = 0; i2 < HowMany; i2++)
				{
					PutPixel(Screen.screen, 7 + (-pldatabase->own->x + Target.PlTarget->x + 10) * 20 + i2, 28 + (-pldatabase->own->y + Target.PlTarget->y + 10) * 20, SDL_MapRGB(Screen.screen->format, 235, 235, 235));
				}
			}				
		}
		else	
		{
			Target.PlTarget = NULL;
		}
	}

	if (Target.Type == MONSTER && Target.MonTarget != NULL)
	{
		int x1,y1,ox,oy;

		ox = pldatabase->own->x;
		oy = pldatabase->own->y;

		x1 = Target.MonTarget->x;
		y1 = Target.MonTarget->y;

		int HowMany, i2;
		char tmp[120];

		if (ox - x1 > -12 && ox - x1 < 12 && oy - y1 > -12 && oy - y1 < 12)
		{
			if (ox - x1 > -11 && ox - x1 < 11 && oy - y1 > -11 && oy - y1 < 11)
			{
				sdl.dst.x = (-pldatabase->own->x + Target.MonTarget->x + 10) * 20 + 6;
				sdl.dst.y = (-pldatabase->own->y + Target.MonTarget->y + 10) * 20 + 6;		
				sdl.dst.h = 24;
				sdl.dst.w = 22;
		
				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[TARGET + Target.Frame], Screen.screen, &sdl.dst);

				HowMany = 0;
				HowMany = 20 * ((double)Target.MonTarget->hpleft / (double)MonsterTypes[Target.MonTarget->type].HPMax);
         
                for (i2 = 0; i2 < HowMany; i2++)
				{
					PutPixel(Screen.screen, 7 + (-pldatabase->own->x + Target.MonTarget->x + 10) * 20 + i2, 28 + (-pldatabase->own->y + Target.MonTarget->y + 10) * 20, SDL_MapRGB(Screen.screen->format, 235, 235, 235));
				}
					// Display NPC health				
					sprintf(tmp, "%d", Target.MonTarget->hpleft);
					sdl.text(tmp, (-pldatabase->own->x + Target.MonTarget->x + 10) * 20 + 16 - BFont_TextWidth(tmp) / 2, 32 + (-pldatabase->own->y + Target.MonTarget->y + 10) * 20, col_white);
			}				
		}
		else	
		{
			Target.DrawTarget = false;
		}
	}

	if (Target.Type == T_NPC && Target.NPCTarget != NULL)
	{
		int x1,y1,ox,oy;

		ox = pldatabase->own->x;
		oy = pldatabase->own->y;

		
		x1 = Target.NPCTarget->x;
		y1 = Target.NPCTarget->y;
			
		if (ox - x1 > -12 && ox - x1 < 12 && oy - y1 > -12 && oy - y1 < 12)
		{
			if (ox - x1 > -11 && ox - x1 < 11 && oy - y1 > -11 && oy - y1 < 11)
			{
				sdl.dst.x = (-pldatabase->own->x + Target.NPCTarget->x + 10) * 20 + 6;
				sdl.dst.y = (-pldatabase->own->y + Target.NPCTarget->y + 10) * 20 + 6;		
				sdl.dst.h = 22;
				sdl.dst.w = 22;
		
				SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[TARGET + Target.Frame], Screen.screen, &sdl.dst);
			}				
		}
		else	
		{
			Target.DrawTarget = false;
		}
	}
}


//==============================================================================
//
//	Draw help menu
//
//==============================================================================
void DrawClass::Help()
{
	SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

	sdl.text("Help Menu", 400, 50, col_grey);

	sdl.text("- Keyboard commands -", 60 , 80, col_grey);
	sdl.text("Movement: WASD/Arrow Keys", 40 , 100, col_white);
	sdl.text("Chat Start: (T)", 40 , 120, col_white);
	sdl.text("Chat Send: (Enter)", 40 , 140, col_white);
	sdl.text("Chat Toggle: (Tab)", 40 , 160, col_white);
	sdl.text("Chat Maximize: (E)", 40 , 180, col_white);
	sdl.text("Skill Menu: (K)", 40 , 200, col_white);
	sdl.text("MiniMap: (M)", 40 , 220, col_white);
	sdl.text("MiniMap Maximize: (F11)", 40 , 240, col_white);
	sdl.text("Target Protection: (F3)", 40 , 260, col_white);
	sdl.text("Combat Text: (F5)", 40 , 280, col_white);
	sdl.text("ScreenShot: (F12)", 40 , 300, col_white);
	sdl.text("Screen Toggle: (F10)", 40 , 320, col_white);
	sdl.text("Help Toggle: (F1)", 40 , 340, col_white);
}


//==============================================================================
//
//	Draw options menu
//
//==============================================================================
void DrawClass::Opt()
{
	SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

	sdl.text("Game Options", 350, 50, col_orange);

	sdl.text("- Chat Toggles -", 200 , 120, col_orange);
	sdl.text("Say Chat -", 200 , 150, col_white);
		if (Client.SayOn == true)
		{
			sdl.text("On", 210 + BFont_TextWidth("Say Chat -") , 150, col_green);
			sdl.dst.x = 240 + BFont_TextWidth("Say Chat -");
			sdl.dst.y = 145;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 210 + BFont_TextWidth("Say Chat -") , 150, col_red);
			sdl.dst.x = 240 + BFont_TextWidth("Say Chat -");
			sdl.dst.y = 145;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);		
		}
	sdl.text("Whisper Chat -", 200 , 180, col_white);
		if (Client.WhisOn == true)
		{
			sdl.text("On", 210 + BFont_TextWidth("Whisper Chat -") , 180, col_green);
			sdl.dst.x = 240 + BFont_TextWidth("Whisper Chat -");
			sdl.dst.y = 175;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 210 + BFont_TextWidth("Whisper Chat -") , 180, col_red);
			sdl.dst.x = 240 + + BFont_TextWidth("Whisper Chat -");
			sdl.dst.y = 175;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);		
		}
	sdl.text("Global Chat -", 200 , 210, col_white);
		if (Client.GlobOn == true)
		{
			sdl.text("On", 210 + BFont_TextWidth("Global Chat -") , 210, col_green);
			sdl.dst.x = 240 + BFont_TextWidth("Global Chat -");
			sdl.dst.y = 205;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 210 + BFont_TextWidth("Global Chat -") , 210, col_red);
			sdl.dst.x = 240 + BFont_TextWidth("Global Chat -");
			sdl.dst.y = 205;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);		
		}


	sdl.text("- Movement Toggles -", 450 , 120, col_orange);
	sdl.text("WASD Movement -", 450 , 150, col_white);
		if (Client.WASD == true)
		{
			sdl.text("On", 460 + BFont_TextWidth("WASD Movement -") , 150, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("WASD Movement -");
			sdl.dst.y = 145;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 460 + BFont_TextWidth("WASD Movement -") , 150, col_red);
			sdl.dst.x = 490 + BFont_TextWidth("WASD Movement -");
			sdl.dst.y = 145;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		}

	sdl.text("- Graphics Toggles -", 200 , 270, col_orange);
	sdl.text("Own Name -", 200 , 300, col_white);
		if (Client.OwnName == true)
		{
			sdl.text("On", 210 + BFont_TextWidth("Own Name -") , 300, col_green);
			sdl.dst.x = 240 + BFont_TextWidth("Own Name -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 210 + BFont_TextWidth("Own Name -") , 300, col_red);
			sdl.dst.x = 240 + BFont_TextWidth("Own Name -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		}
	sdl.text("Player Names -", 200 , 330, col_white);
		if (Client.PlNames == true)
		{
			sdl.text("On", 210 + BFont_TextWidth("Player Names -") , 330, col_green);
			sdl.dst.x = 240 + BFont_TextWidth("Player Names -");
			sdl.dst.y = 325;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		} else {
			sdl.text("Off", 210 + BFont_TextWidth("Player Names -") , 330, col_red);
			sdl.dst.x = 240 + BFont_TextWidth("Player Names -");
			sdl.dst.y = 325;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		}

	sdl.text("- Crafting Toggles -", 450 , 270, col_orange);
	sdl.text("Amount Per -", 450 , 300, col_white);
		if (Client.CraftPer == 0)
		{
			sdl.text("10", 460 + BFont_TextWidth("Amount Per -") , 300, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("Amount Per -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		} else if (Client.CraftPer == 1){
			sdl.text("25", 460 + BFont_TextWidth("Amount Per -") , 300, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("Amount Per -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		} else if (Client.CraftPer == 2){
			sdl.text("50", 460 + BFont_TextWidth("Amount Per -") , 300, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("Amount Per -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		} else if (Client.CraftPer == 3){
			sdl.text("100", 460 + BFont_TextWidth("Amount Per -") , 300, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("Amount Per -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sup_inactive ], Screen.screen, &sdl.dst);
		} else if (Client.CraftPer == 4){
			sdl.text("All", 460 + BFont_TextWidth("Amount Per -") , 300, col_green);
			sdl.dst.x = 490 + BFont_TextWidth("Amount Per -");
			sdl.dst.y = 295;
			sdl.dst.h = 15;
			sdl.dst.w = 15;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[ secure_sdown_inactive ], Screen.screen, &sdl.dst);
		}
}


//==============================================================================
//
//	Draw kicked screen
//
//==============================================================================
void DrawClass::Kicked()
{

	SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

	sdl.text("YOU HAVE BEEN KICKED FROM THE GAME.", 400  - BFont_TextWidth("YOU HAVE BEEN KICKED FROM THE GAME.")/2, 200, col_grey);


	sdl.text("Click the screen...", 400 - BFont_TextWidth("Click the screen...")/2, 250, col_green);
	
}


//==============================================================================
//
//	Draw banned screen
//
//==============================================================================
void DrawClass::Banned()
{

	SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);
	
	sdl.text("YOU HAVE BEEN BANNED FROM THE GAME.", 400 - BFont_TextWidth("YOU HAVE BEEN BANNED FROM THE GAME.")/2, 200, col_grey);	


	sdl.text("Click the screen...", 400 - BFont_TextWidth("Click the screen...")/2, 250, col_green);
	
}


//==============================================================================
//
//	Draw rank screen
//
//==============================================================================
void DrawClass::Ranked()
{

	SDL_BlitSurface(Login.bGround, NULL, Screen.screen, NULL);

	sdl.text("YOUR RANK HAS BEEN CHANGED!.", 400  - BFont_TextWidth("YOUR RANK HAS BEEN CHANGED!.")/2, 200, col_grey);


	sdl.text("Click the screen...", 400 - BFont_TextWidth("Click the screen...")/2, 250, col_green);
	
}


//==============================================================================
//
//	Draw temp pics
//
//==============================================================================
void DrawClass::DrawTmpPics()
{
	int i4;
	short ox, oy, tx, ty;
	ox = pldatabase->own->x;
	oy = pldatabase->own->y;
	for (i4 = 0; i4 < 10; i4++)
	{
		if (TmpPics[i4].Used == 1)
		{
			if (SDL_GetTicks() - TmpPics[i4].FrameTime > 250)
			{
				if (TmpPics[i4].Pic == 11)
				{
					if (SDL_GetTicks() - TmpPics[i4].FrameTime > 1000)
					{
						memset(&TmpPics[i4], 0, sizeof(TmpPics[i4]));
					}
				}
				else
				{
					memset(&TmpPics[i4], 0, sizeof(TmpPics[i4]));
				}
			}		
			else
			{
				tx = TmpPics[i4].X;
				ty = TmpPics[i4].Y;
				if (ox - tx > -11 && ox - tx < 11 && oy - ty > -11 && oy - ty < 11 && MapInfo.Visible[-pldatabase->own->x + tx + 10][-pldatabase->own->y + ty + 10] > 3)
				{
					sdl.dst.x = (-pldatabase->own->x + tx + 10) * 20 + 7;
					sdl.dst.y = (-pldatabase->own->y + ty + 10) * 20 + 7;
					sdl.dst.h = 20;
					sdl.dst.w = 20;
                    
					SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[TmpPics[i4].Pic + HIT], Screen.screen, &sdl.dst);
				}
			}
		}
	}
}


//==============================================================================
//
//	Draw dragging items
//
//==============================================================================
void DrawClass::DrawDraggingItem()
{
	sdl.dst.x = MX - 10;	
	sdl.dst.y = MY - 10;	
	sdl.dst.h = 20;
	sdl.dst.w = 20;
	
	DrawItem(Dragging.TargetItem->Family, Dragging.TargetItem->Type, Dragging.TargetItem->ID);
}


//==============================================================================
//
//	Draw bank box
//
//==============================================================================
void DrawClass::DrawBank()
{
	int x1 = -BFont_TextWidth("Bank") / 2 + Bank_w / 2;


	sdl.dst.x = Bank.X; sdl.dst.y = Bank.Y; sdl.dst.h = Bank_h; sdl.dst.w = Bank_w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BANK], Screen.screen, &sdl.dst);

	sdl.text("Bank", Bank.X + x1, Bank.Y + 4, col_white);

	//------------------------------------------------------------------------------
	//	Locked bank window
	//------------------------------------------------------------------------------
	if (Bank.Docked)
	{
		sdl.dst.x = Bank.X + 4;
		sdl.dst.y = Bank.Y + 4;
        
		sdl.dst.h = 13;
		sdl.dst.w = 11;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[LOCK], Screen.screen, &sdl.dst);
	}

	int i;
	for (i = 0; i < MaxBankItems; i++)
	{
		if (iBank[i].Used)
		{

			sdl.dst.x = iBank[i].x + Bank.X + 3;	
			sdl.dst.y = iBank[i].y + Bank.Y + 17;	
			sdl.dst.h = 20;
			sdl.dst.w = 20;

			DrawItem(iBank[i].Family, iBank[i].Type, iBank[i].ID);
		}
	}	
}


//==============================================================================
//
//	Draw equipped items
//
//==============================================================================
void DrawClass::DrawWearedItems()
{
	int i;
	for (i = 0; i < MaxWearedItems; i++)
	{
		if (iWeared[i].Used)
		{
			switch (iWeared[i].Type)
			{
			case TYPE_ARMOR:
				sdl.dst.x = PlateX; sdl.dst.y = PlateY;
				break;
			case TYPE_HELM:
				sdl.dst.x = HelmX; sdl.dst.y = HelmY;
				break;
			case TYPE_SHIELD:
				sdl.dst.x = ShieldX; sdl.dst.y = ShieldY;
				break;
			case TYPE_LEGS:
				sdl.dst.x = LegsX; sdl.dst.y = LegsY;
				break;
			case TYPE_GAUNTS:
				sdl.dst.x = GauntsX; sdl.dst.y = GauntsY;
				break;
			case TYPE_RING1:
				sdl.dst.x = RingX; sdl.dst.y = RingY;
				break;
			case TYPE_AMULET:
				sdl.dst.x = AmuletX; sdl.dst.y = AmuletY;
				break;
			case TYPE_ROBE:
				sdl.dst.x = RobeX; sdl.dst.y = RobeY;
				break;
			}

			if (iWeared[i].Family == FAMILY_WEAPON)
			{
				sdl.dst.x = WeaponX; sdl.dst.y = WeaponY;
			}

			sdl.dst.h = 20;
			sdl.dst.w = 20;

			DrawItem(iWeared[i].Family, iWeared[i].Type, iWeared[i].ID);
		}
	}
}


//==============================================================================
//
//	Draw identify box [DISABLED?]
//
//==============================================================================
void DrawClass::DrawIdentify()
{
/*	sdl.dst.x = 681;
	sdl.dst.y = 450;
	sdl.dst.w = 119;
	sdl.dst.h = 150;

	SDL_BlitSurface(G.Identify, NULL, Screen.screen, &sdl.dst);

	char x1[30];
	sprintf(x1, "æ%s Å%s", Client.IDTag, Client.IDName);
	sdl.text(x1, 740 - BFont_TextWidth(x1)/2, 441, col_yellow);

	int i;
	for (i = 0; i < 20; i++)
	{
		if (Client.Family[i] != 0 && Client.Type[i] != 0)
		{
			switch (Client.Type[i])
			{
			case TYPE_ARMOR:
				sdl.dst.x = IDPlateX; sdl.dst.y = IDPlateY;
				break;
			case TYPE_HELM:
				sdl.dst.x = IDHelmX; sdl.dst.y = IDHelmY;
				break;
			case TYPE_SHIELD:
				sdl.dst.x = IDShieldX; sdl.dst.y = IDShieldY;
				break;
			case TYPE_LEGS:
				sdl.dst.x = IDLegsX; sdl.dst.y = IDLegsY;
				break;
			case TYPE_GAUNTS:
				sdl.dst.x = IDGauntsX; sdl.dst.y = IDGauntsY;
				break;
			case TYPE_RING1:
				sdl.dst.x = IDRingX; sdl.dst.y = IDRingY;
				break;
			case TYPE_AMULET:
				sdl.dst.x = IDAmuletX; sdl.dst.y = IDAmuletY;
				break;
			case TYPE_ROBE:
				sdl.dst.x = IDRobeX; sdl.dst.y = IDRobeY;
				break;
			}

			if (Client.Family[i] == FAMILY_WEAPON)
			{
				sdl.dst.x = IDWeaponX; sdl.dst.y = IDWeaponY;
			}

			sdl.dst.h = 20;
			sdl.dst.w = 20;

			sdl.dst.x += 681;
			sdl.dst.y += 450;

			DrawItem(Client.Family[i], Client.Type[i], Client.ID[i]);
		}
	}*/
}
/*
void DrawClass::DrawMinimap()
{
	sdl.dst.x = Minimap.X; 
	sdl.dst.y = Minimap.Y; 
	
	if (!Minimap.Size)
	{
		sdl.dst.h = Minimap_h; 
		sdl.dst.w = Minimap_w;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[MINIMAP], Screen.screen, &sdl.dst);
	}
	else
	{
		sdl.dst.h = Minimap_h_BIG; 
		sdl.dst.w = Minimap_w_BIG;
		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[MINIMAP_BIG], Screen.screen, &sdl.dst);
	}

	int i, j;

	int x1, y1, x2, y2, x3, y3;
	x1 = pldatabase->own->x;
	y1 = pldatabase->own->y;
	int x, y;

	if (Minimap.Size)
	{
		for (i = 0; i < 161; i++)
		{
			for (j = 0; j < 161; j++)
			{
				x = x1 + i -80;
				y = y1 + j -80;

				if (x < 0) x = 0;
				if (y < 0) y = 0;				

				if (MapInfo.MapVisibility[(x-50) >> 4][(y-50) >> 4] == true)
				{
					if (Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ] != 0)
					{
						PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].r, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].g, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].b ));
					}
					else
					{
						PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].r, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].g, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x >> 4 ] [ y >> 4 ]] [ x % 16 ] [ y % 16 ]].b ));
					}
				}
				else 
				{
					PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 0, 0, 0 )); 
				}	
			}
		}
		PutPixel(Screen.screen, Minimap.X + 85, Minimap.Y + 85, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 84, Minimap.Y + 84, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 86, Minimap.Y + 84, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 84, Minimap.Y + 86, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 86, Minimap.Y + 86, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	}
	else
	{
		for (i = 0; i < 101; i++)
		{
			for (j = 0; j < 101; j++)
			{
				x = x1 + i -50;
				y = y1 + j -50;
				x2 = x >> 4;
				y2 = y >> 4;
				x3 = x % 16;
				y3 = y % 16;

				if (x < 0) x = 0;
				if (y < 0) y = 0;	

				if (MapInfo.MapVisibility[(x - 50) >> 4][(y - 50) >> 4] == true)
				{
					if (Map.Chunks[1][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ] != 0)
					{
						PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].r, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].g, 
										Tiles[ Map.Chunks[1][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].b ));
					}
					else
					{
						PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].r, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].g, 
										Tiles[ Map.Chunks[0][ Map.ChunkMap[ x2 ] [ y2 ]] [ x3 ] [ y3 ]].b ));
					}
				}
				else 
				{
					PutPixel(Screen.screen, Minimap.X + 3 + i, Minimap.Y + 3 + j, SDL_MapRGB( Screen.screen->format, 0, 0, 0 )); 
				}	
			}
		}
		PutPixel(Screen.screen, Minimap.X + 55, Minimap.Y + 55, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 54, Minimap.Y + 54, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 56, Minimap.Y + 54, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 54, Minimap.Y + 56, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
		PutPixel(Screen.screen, Minimap.X + 56, Minimap.Y + 56, SDL_MapRGB(Screen.screen->format, 255, 0, 0));
	}
}
*/


//==============================================================================
//
//	Draw stats
//
//==============================================================================
void DrawClass::DrawStats()
{
	char tmp[120];
	short tmpcol;
	
	if (pldatabase->own->wanted) tmpcol = col_gray;
	else tmpcol = pldatabase->own->reputation;

	if (strlen(pldatabase->own->tag) > 0)
		sprintf(tmp, "æ%s  Å%sÉ  level Ä%dÅ É%s", pldatabase->own->tag, pldatabase->own->name, P.Level, Races[P.Race].Name);
	else
		sprintf(tmp, "Å%sÉ, level Ä%d É%s", pldatabase->own->name, P.Level, Races[P.Race].Name);

	sdl.text(tmp, 600 - BFont_TextWidth(tmp) / 2, 11, tmpcol);
	
	sdl.text("HP:", 440, 50, col_yellow);
	
	sdl.text("Exp:", 440, 62, col_yellow);

	sdl.text("Statistics", 440, 98, col_yellow);

	sdl.text("Str:", 440, 115, col_yellow);

	sdl.text("Dex:", 440, 127, col_yellow);

	sdl.text("Con:", 440, 139, col_yellow);

	sdl.text("Int:", 440, 151, col_yellow);


	sdl.text("Resistances", 540, 98, col_yellow);

	sdl.text("Fire:", 540, 115, col_yellow);

	sdl.text("Water:", 540, 127, col_yellow);

	sdl.text("Air:", 540, 139, col_yellow);

	sdl.text("Earth:", 540, 151, col_yellow);



	sdl.text("Armor:", 440, 182, col_yellow);

	sdl.text("Weight:", 440, 194, col_yellow);

	sdl.text("Damage:", 440, 206, col_yellow);



	sprintf(tmp, "%d / %d", P.HPLeft, P.HPMax);
	sdl.text(tmp, 480, 50, col_white);

	sprintf(tmp, "%lu", P.Exp);
	sdl.text(tmp, 480, 62, col_white);
	
	sprintf(tmp, "%d", P.Str);
	sdl.text(tmp, 480, 115, col_white);

	sprintf(tmp, "%d", P.Dex);
	sdl.text(tmp, 480, 127, col_white);

	sprintf(tmp, "%d", P.Con);
	sdl.text(tmp, 480, 139, col_white);

	sprintf(tmp, "%d", P.Int);
	sdl.text(tmp, 480, 151, col_white);


	sprintf(tmp, "%d", P.fire);
	sdl.text(tmp, 590, 115, col_white);

	sprintf(tmp, "%d", P.water);
	sdl.text(tmp, 590, 127, col_white);

	sprintf(tmp, "%d", P.air);
	sdl.text(tmp, 590, 139, col_white);

	sprintf(tmp, "%d", P.earth);
	sdl.text(tmp, 590, 151, col_white);



	sprintf(tmp, "%d", P.AC);
	sdl.text(tmp, 495, 182, col_white);

	sprintf(tmp, "%d / %d", P.Weight, P.WeightMax);
	sdl.text(tmp, 495, 194, col_white);

	sprintf(tmp, "%d - %d", P.DamMin, P.DamMax);
	sdl.text(tmp, 495, 206, col_white);

	if (IsPre || GMMode > 0)
	{
		sdl.text("Coords:", 440, 218, col_yellow);

		sprintf(tmp, "%d, %d", pldatabase->own->x, pldatabase->own->y);
		sdl.text(tmp, 495, 218, col_white);
	}
}


//==============================================================================
//
//	Draw NPC names [DISABLED]
//
//==============================================================================
void DrawClass::DrawCharacters()
{

	// NPCs' names

	//for (int i = 0; i < MaxNPC; i++)
	//{
	//	if (NPC[i].Used)
	//	{
	//		x1 = -pldatabase->own->x + NPC[i].X + 10;
	//		y1 = -pldatabase->own->y + NPC[i].Y + 10;

	//		if (MapInfo.Visible[-pldatabase->own->x + NPC[i].X + 10][-pldatabase->own->y + NPC[i].Y + 10])
	//		{
	//			Draw.PrintReputationText(x1 * 20 + 17, y1 * 20, 0, 5, NPCTypes[NPC[i].Type].Name, 0, 0);
	//		}
	//	}	
	//}
}


//==============================================================================
//
//	Draw chat box
//
//==============================================================================
void DrawClass::DrawChatBox()
{
	char tmp[120];

	sdl.dst.h = 44;
	sdl.dst.w = 600;
	sdl.dst.x = 0;
	sdl.dst.y = 388;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[CHATBOX], Screen.screen, &sdl.dst);

	
	if (Client.SayOn)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 11;
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_W], Screen.screen, &sdl.dst);
	}

	
	if (Client.WhisOn)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 24;
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_N], Screen.screen, &sdl.dst);
	}

	/*
	sdl.dst.h = 11;
	sdl.dst.w = 11;
	sdl.dst.x = 76;
	sdl.dst.y = 410;
	*/

	if (Client.GlobOn)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 37;
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_SC], Screen.screen, &sdl.dst);
	}


	if (P.Guild > 0)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 50;				
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_G], Screen.screen, &sdl.dst); 
	}

	if (GMMode > 1)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 63;
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_GM], Screen.screen, &sdl.dst);
	}

	if (GMMode > 0)
	{
		sdl.dst.h = 11;
		sdl.dst.w = 11;
		sdl.dst.x = 76;
		sdl.dst.y = 410;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BUTTON_WC], Screen.screen, &sdl.dst);
	}

	if (Client.SayOn && Writing.ChattingMode == 1) // Normal
	{
		sdl.text("Say", 11, 396, col_yellow);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_yellow);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_yellow);
				
	}
	if (Client.WhisOn && Writing.ChattingMode == 2) // Whisper
	{
		sdl.text("Whisper", 11, 396, col_purple);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_purple);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_purple);

	}
	if (Client.GlobOn && Writing.ChattingMode == 3) // Global
	{
		sdl.text("Global", 11, 396, col_orange);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_orange);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_orange);

	}
	if (Writing.ChattingMode == 4) // Guild
	{
		sdl.text("Guild", 11, 396, col_blue);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_blue);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_blue);

	}
	if (Writing.ChattingMode == 5 && GMMode > 1) // GM
	{
		sdl.text("GM", 11, 396, col_grey);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_grey);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_grey);


	}
	if (Writing.ChattingMode == 10 && GMMode > 2) // server
	{
		sdl.text("Server", 11, 396, col_red);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_red);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_red);

	}
	if (Writing.ChattingMode == 6 && GMMode > 0) // EWC
	{
		sdl.text("EWC", 11, 396, col_lightblue);
		if (!Writing.WritingError)
		{
			sprintf(tmp, "%sÉ|", Writing.Txt);
			sdl.text(tmp, 105, 410, col_lightblue);
		}
		else
			sdl.text(Writing.Txt, 105, 410, col_lightblue);
	}
}






//==============================================================================
//
//	Bytes -> Pixel
//
//==============================================================================
void PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 GetPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}







//==============================================================================
//
//	Update MessageBox
//
//==============================================================================
void MessageBoxClass::Update()
{
	int i;
	for (i = 48; i > 0; i--)
	{
		strcpy(Msg[i], Msg[i-1]);
		MsgType[i] = MsgType[i-1];
	}

	strcpy(Msg[0], NewMsg);
	MsgType[0] = NewType;
}


//==============================================================================
//
//	New Message
//
//==============================================================================
void MessageBoxClass::NewMessage(char const msg[180], short MsgType)
{
	strcpy(NewMsg, msg);
    NewType = MsgType;
	Update();
}


//==============================================================================
//
//	Print the message
//
//==============================================================================
void MessageBoxClass::Print()
{
	//int j;
	int k;
	int num;

	/*if (Expanded) 
	{
		sdl.draw(G.MBExpanded, 0, 0);

		j = 49;
	}
	*/

	k = scroll;

	for (int i = 0; i < 13; i++)
	{
		num = i + k;
		if (MsgType[num] != NULL)
		{
			switch (MsgType[num])
			{
			case 1: // Gray
				sdl.text(Msg[num], 30, 579 - i * 12, col_gray);
				break;
			case 2: // Green
				sdl.text(Msg[num], 30, 579 - i * 12, col_grey);
				break;
			case 3: // Purple
				sdl.text(Msg[num], 30, 579 - i * 12, col_purple);
				break;
			case 4: // Yellow
				sdl.text(Msg[num], 30, 579 - i * 12, col_yellow);
				break;
			case 5: // Blue
				sdl.text(Msg[num], 30, 579 - i * 12, col_blue);
				break;
			case 6: // Red
				sdl.text(Msg[num], 30, 579 - i * 12, col_red);
				break;
			case 7: // White
				sdl.text(Msg[num], 30, 579 - i * 12, col_white);
				break;
			case 8: // Lightgreen
				sdl.text(Msg[num], 30, 579 - i * 12, col_greenish);
				break;
			case 9: // LightBlue
				sdl.text(Msg[num], 30, 579 - i * 12, col_lightblue);
				break;
			case 10: // Blue
				sdl.text(Msg[num], 30, 579 - i * 12, col_blue);
				break;
			case 11: // GM Green
				sdl.text(Msg[num], 30, 579 - i * 12, col_green);
			case 12: // Orange
				sdl.text(Msg[num], 30, 579 - i * 12, col_orange);
				break;
			}
		}
	}
}







//==============================================================================
//
//	Combat text time
//
//==============================================================================
void CombatTextClass::TimeUpdate()
{
	if (used[0] == 0) return;

	int i;

	for (i = 0; i < 10; i++)
	{
		if (used[i] != 0)
		{
			strcpy(msg[i], msg[i+1]);
			msgtype[i] = msgtype[i+1];
		}
		if (i == 9 && used[9] == 1) { used[9] = 0; memset(msg[9], 0, sizeof(msg[9])); }
		if (used[i] == 0) break;
	}
	lstupdate = SDL_GetTicks();
}


//==============================================================================
//
//	Update combat text
//
//==============================================================================
void CombatTextClass::Update()
{
	int i, j;
	for (i = 0; i < 10; i++)
	{
		if (used[i] == 0)
		{
			strcpy(msg[i], newmsg);
			msgtype[i]  = newmsgtype;

			used[i] = 1;
			i = 10;
		}
		if (i == 9 && used[i] == 1)
		{
			for (j = 0; j < 9; j++)
			{
				strcpy(msg[j], msg[j+1]);
				msgtype[j] = msgtype[j+1];
			}
			strcpy(msg[9], newmsg);
			msgtype[9] = newmsgtype;
			i = 10;
		}
	}
	lstupdate = SDL_GetTicks();
}


//==============================================================================
//
//	Print combat text
//
//==============================================================================
void CombatTextClass::Print()
{
	int i;
	for (i = 0; i < 10; i++)
	{
		if (used[i] == 1)
		{
			switch (msgtype[i])
			{
			case 1: // Gray
				sdl.text(msg[i], 12, 9 + i * 10, col_gray);
				break;
			case 2: // Green
				sdl.text(msg[i], 12, 9 + i * 10, col_grey);
				break;
			case 3: // Yellow
				sdl.text(msg[i], 12, 9 + i * 10, col_yellow);
				break;
			case 4: // Blue
				sdl.text(msg[i], 12, 9 + i * 10, col_blue);
				break;
			case 5: // Red
				sdl.text(msg[i], 12, 9 + i * 10, col_red);
				break;
			case 6: // Purple
				sdl.text(msg[i], 12, 9 + i * 10, col_darkpurple);
				break;
			case 7: // White
				sdl.text(msg[i], 12, 9 + i * 10, col_white);
				break;
			}
		}
	}
}






//==============================================================================
//
//	Draw login box
//
//==============================================================================
void LoginClass::DrawPics()
{
	SDL_BlitSurface(bGround, NULL, Screen.screen, NULL);

    sdl.dst.x = 250; 
	sdl.dst.y = 150;
	sdl.dst.h = 300;
	sdl.dst.w = 300;
	SDL_BlitSurface(fGround, NULL, Screen.screen, &sdl.dst);
}


//==============================================================================
//
//	Draw login box text
//
//==============================================================================
void LoginClass::DrawTexts()
{
	char tmp[120];
	if (GameState == 6)
	{
		if (Writing.Mode == 10)
		{
			sprintf(tmp, "%sÉ|", AccName);
			sdl.text(tmp, 398, 211, col_grey);
		}
		else sdl.text(AccName, 398, 211, col_gray);
			
		if (Writing.Mode == 11)
		{
			sprintf(tmp, "%sÉ|", AccPass);
			sdl.text(tmp, 398, 243, col_grey);
		}
		else sdl.text(AccPass, 398, 243, col_gray);
	}
	else
	{
		if (Writing.Mode == 10)
		{
			sprintf(tmp, "%sÉ|", AccName);
			sdl.text(tmp, 398, 261, col_grey);
		}
		else sdl.text(AccName, 398, 261, col_gray);
	
		int i;
		for (i = 0; i < Writing.LoginHandler[1]; i++)
		{
			if (Writing.Mode == 11)
				sdl.text("*", 398 + i * 6, 293, col_grey);
			else
				sdl.text("*", 398 + i * 6, 293, col_gray);
		}
		if (Writing.Mode == 11)
			sdl.text("|", 398 + Writing.LoginHandler[1]*6, 293, col_white);
	}
}


//==============================================================================
//
//	Prepare writing mode
//
//==============================================================================
void LoginClass::Prepare()
{
	Writing.Mode = 10; // Account name writing mode.

	CurrentChoice = 0;
}


//==============================================================================
//
//	Draw selection box around chosen char
//
//==============================================================================
void LoginClass::DrawCharChoosePics()
{
	SDL_BlitSurface(bGround, NULL, Screen.screen, NULL);

	sdl.dst.x = 150;
	sdl.dst.y = 100;
	sdl.dst.h = 400;
	sdl.dst.w = 500;

	SDL_BlitSurface(charchoose, NULL, Screen.screen, &sdl.dst);

	// First char is selected
	if (CurrentChoice == 0) 
	sdl.dst.x = 177;
	// Second char is selected
	if (CurrentChoice == 1)
	sdl.dst.x = 455;

	sdl.dst.y = 186;

	sdl.dst.h = 146;
	sdl.dst.w = 168;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[CHARCHOOSESELECTED], Screen.screen, &sdl.dst);
}


//==============================================================================
//
//	Draw char info text
//
//==============================================================================
void LoginClass::DrawCharChooseTexts()
{
	char tmp[120];

	if (CharChooseInfo.Used[0] == 1)
	{
//		if (CharChooseInfo.Wanted[0])
//		{
//			sprintf(tmp, "æ%s Å%s", CharChooseInfo.Tag[0], CharChooseInfo.Name[0]);
//			sdl.text(tmp, 261-BFont_TextWidth(tmp)/2, 200, col_gray);
//		}
//		else
		{
			sprintf(tmp, "Å%s", CharChooseInfo.Name[0]);
			sdl.text(tmp, 261-BFont_TextWidth(tmp)/2, 200, col_white);
		}
//		Draw.PrintReputationText(261, 211, CharChooseInfo.Wanted[0], CharChooseInfo.Reputation[0], CharChooseInfo.Name[0], CharChooseInfo.IsInGuild[0], CharChooseInfo.Tag[0]);

		sdl.text("Level:", 184, 256, col_white);
		sdl.text("HP:", 184, 285, col_white);

		sprintf(tmp, "%d", CharChooseInfo.Level[0]);
		sdl.text(tmp, 242, 256, col_yellow);

		sprintf(tmp, "%d / %d", CharChooseInfo.hp[0], CharChooseInfo.hpmax[0]);
		sdl.text(tmp, 242, 285, col_yellow);
	}

	if (CharChooseInfo.Used[1] == 1)
	{
//		if (CharChooseInfo.Wanted[1])
//		{
//			sprintf(tmp, "æ%s Å%s", CharChooseInfo.Tag[1], CharChooseInfo.Name[1]);
//			sdl.text(tmp, 540-BFont_TextWidth(tmp)/2, 200, col_gray);
//		}
//		else
		{
			sprintf(tmp, "Å%s", CharChooseInfo.Name[1]);
			sdl.text(tmp, 540-BFont_TextWidth(tmp)/2, 200, col_white);
		}
//		Draw.PrintReputationText(261, 211, CharChooseInfo.Wanted[0], CharChooseInfo.Reputation[0], CharChooseInfo.Name[0], CharChooseInfo.IsInGuild[0], CharChooseInfo.Tag[0]);

		sdl.text("Level:", 462, 256, col_white);
		sdl.text("HP:", 462, 285, col_white);

		sprintf(tmp, "%d", CharChooseInfo.Level[1]);
		sdl.text(tmp, 520, 256, col_yellow);

		sprintf(tmp, "%d / %d", CharChooseInfo.hp[1], CharChooseInfo.hpmax[1]);
		sdl.text(tmp, 520, 285, col_yellow);
	}

	if (CharChooseInfo.Used[0] == 0)
	{
		sdl.text("Create new character", 261 - BFont_TextWidth("Create new character")/2, 258, col_white);
//		BFont_PrintStringFont(Screen.screen, F.white, 261 - BFont_TextWidth("Create new character")/2, 258, "Create new character");
	}

	if (CharChooseInfo.Used[1] == 0)
	{
		sdl.text("Create new character", 540 - BFont_TextWidth("Create new character")/2, 258, col_white);
//		BFont_PrintStringFont(Screen.screen, F.white, 540 - BFont_TextWidth("Create new character")/2, 258, "Create new character");
	}
}





//==============================================================================
//
//	Draw identify text
//
//==============================================================================
void IdentifyItem(ItemClass *This)
{
	if (This->Family == FAMILY_WEAPON)
	{
		sprintf(MB.NewMsg, "You see ì%sÅ.", Weapons[This->Type][This->ID].Name);
		MB.NewType = 7;
		MB.Update();

		sprintf(MB.NewMsg, "Durability: ì%d", This->Durability);//, Weapons[This->Type-1][This->ID].);
		MB.NewType = 7;
		MB.Update();

		if (P.Int > 40)
		{
			sprintf(MB.NewMsg, "You know that the weapon has damage: ì%dÅ - ì%dÅ", Weapons[This->Type][This->ID].DamMin, Weapons[This->Type][This->ID].DamMax);
			MB.NewType = 7;
			MB.Update();
		}
		if (P.Int > 60)
		{
			if (Weapons[This->Type][This->ID].ElementalDamageType != NULL)
			{
				sprintf(MB.NewMsg, "You notice that this weapon is a ì%sÅ elemental weapon.", elem_Info[Weapons[This->Type][This->ID].ElementalDamageType]);
				MB.NewType = 7;
				MB.Update();
			}
		}

		if (GMMode > 3)
		{
			sprintf(MB.NewMsg, "Type %d, ID %d, Family %d, Quantity %d, Durability %d, X %d, Y = %d, KnowID %d, Used %d", This->Type, This->ID, This->Family, This->Quantity, This->Durability, This->x, This->y, This->KnowID, This->Used);			
			MB.NewType = 7;
			MB.Update();
		}
	}
	if (This->Family == FAMILY_ARMOR)
	{
		sprintf(MB.NewMsg, "You see ì%sÅ.", Armors[This->Type-11][This->ID].Name);
		MB.NewType = 7;
		MB.Update();

		sprintf(MB.NewMsg, "Durability: %d", This->Durability);//, Weapons[This->Type-1][This->ID].);
		MB.NewType = 7;
		MB.Update();

		if (P.Int > 40)
		{
			sprintf(MB.NewMsg, "You know that this armor has %d armor class points.", Armors[This->Type-11][This->ID].AC);
			MB.NewType = 7;
			MB.Update();
		}

		if (P.Int > 60)
		{
			if (Armors[This->Type-11][This->ID].ElementalProtection != NULL)
			{
				sprintf(MB.NewMsg, "You notice that this armor has %s elemental protection.", elem_Info[Armors[This->Type-11][This->ID].ElementalProtection]);
				MB.NewType = 7;
				MB.Update();
			}
		}
		
		if (GMMode > 2)
		{
			sprintf(MB.NewMsg, "Type = %d, ID = %d, Family = %d, Quantity = %d, Durability = %d, X = %d, Y = %d, KnowID = %d, Used = %d", This->Type, This->ID, This->Family, This->Quantity, This->Durability, This->x, This->y, This->KnowID, This->Used);
			MB.NewType = 7;
			MB.Update();
		}
	}
	if (This->Family == FAMILY_COLLECTABLE)
	{
		if (This->Quantity > 1)
		{
			sprintf(MB.NewMsg, "You see (Ä%luÅ) %ss.", This->Quantity, Collectables[This->ID].Name);
			MB.NewType = 7;
			MB.Update();
		}
		else
		{
			sprintf(MB.NewMsg, "You see (Ä%luÅ) %s.", This->Quantity, Collectables[This->ID].Name);
			MB.NewType = 7;
			MB.Update();
		}
		if (GMMode > 3)
		{
			sprintf(MB.NewMsg, "Type = %d, ID = %d, Family = %d, Quantity = %d, Durability = %d, X = %d, Y = %d, KnowID = %d, Used = %d", This->Type, This->ID, This->Family, This->Quantity, This->Durability, This->x, This->y, This->KnowID, This->Used);
			MB.NewType = 7;
			MB.Update();
		}
	}
	if (This->Family == FAMILY_USEABLE)
	{
		sprintf(MB.NewMsg, "You see ì%sÅ.", Useables[This->ID].Name);
		MB.NewType = 7;
		MB.Update();

		sprintf(MB.NewMsg, "Durability: %d", This->Durability);
		MB.NewType = 7;
		MB.Update();
		if (GMMode > 3)
		{
			sprintf(MB.NewMsg, "Type = %d, ID = %d, Family = %d, Quantity = %d, Durability = %d, X = %d, Y = %d, KnowID = %d, Used = %d", This->Type, This->ID, This->Family, This->Quantity, This->Durability, This->x, This->y, This->KnowID, This->Used);
			MB.NewType = 7;
			MB.Update();
		}
	}
}





//==============================================================================
//
//	Draw MouseOver info [DISABLED]
//
//==============================================================================
void DrawClass::MouseOver()
{
/*	Uint16 mx, my;

	mx = MX;
	my = MY;

	if (BrowseSkills && mx >= 410 && my > 67 && my < 162)
	{
		// Later check if skills filter is on.

		int Tmp;

		Tmp = (my-55) / 12;
		if (Tmp < 0) return;
		if (Tmp > MaxSkills) return;
        		

		if (my >= 115 && my <= 138) return;

        
		int i;
		for (i = 0; i < 375; i++)
		{
			PutPixel(Screen.screen, 410 + i, 65 + Tmp*12, SDL_MapRGB(Screen.screen->format, 0, 110, 220));
		}

	
	}
*/
	//if (Shop.On)

}




//==============================================================================
//
//	Draw horizontal line [FIND THIS]
//
//==============================================================================
void DrawClass::Horizontal_Underline( short FromX, short ToX, short Y, unsigned char r, unsigned char g, unsigned char b)
{
	short _x;
	_x = ToX - FromX;
	if (_x < 0)
	{
		short _tx;
		_tx = FromX;
		FromX = ToX;
		ToX = _tx;
		_x *= -1;
	}
	if (_x == 0) return;
    for (int _i = 0; _i < _x; _i++)
	{
		PutPixel(Screen.screen, FromX + _i, Y, SDL_MapRGB(Screen.screen->format, r, g, b));	
	}
}


//==============================================================================
//
//	Draw items based on certain criteria
//
//==============================================================================
void DrawClass::d_items()
{
	sdl.dst.x = Backpack.X; sdl.dst.y = Backpack.Y; sdl.dst.h = Backpack_h; sdl.dst.w = Backpack_w;
	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BACKPACK], Screen.screen, &sdl.dst);

	int x1 = -BFont_TextWidth("Backpack") / 2 + Backpack_w / 2;

	sdl.text("Backpack", Backpack.X + x1, Backpack.Y + 4, col_white);
//	BFont_PrintStringFont(Screen.screen, F.white, Backpack.X + x1, Backpack.Y + 4, "Backpack");
	
	//------------------------------------------------------------------------------
	//	Find BP position, and place items there
	//------------------------------------------------------------------------------
	if (Backpack.Docked)
	{
		sdl.dst.x = Backpack.X + 4;
		sdl.dst.y = Backpack.Y + 4;
        
		sdl.dst.h = 13;
		sdl.dst.w = 11;

		SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[LOCK], Screen.screen, &sdl.dst);
	}

	int i;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		//------------------------------------------------------------------------------
		//	If BP is not full, place items inside
		//------------------------------------------------------------------------------
		if (iBackpack[i].Used)
		{
			sdl.dst.x = iBackpack[i].x + Backpack.X + 3;	
			sdl.dst.y = iBackpack[i].y + Backpack.Y + 17;	
			sdl.dst.h = 20;
			sdl.dst.w = 20;
	
			DrawItem( iBackpack[i].Family, iBackpack[i].Type, iBackpack[i].ID);
		}

		//------------------------------------------------------------------------------
		//	Place items in equip slots
		//------------------------------------------------------------------------------
		if (i < MaxWearedItems)
		{
			if (iWeared[i].Used)
			{
				switch (iWeared[i].Type)
				{
				case TYPE_ARMOR:
					sdl.dst.x = PlateX; sdl.dst.y = PlateY;
					break;
				case TYPE_HELM:
					sdl.dst.x = HelmX; sdl.dst.y = HelmY;
					break;
				case TYPE_SHIELD:
					sdl.dst.x = ShieldX; sdl.dst.y = ShieldY;
					break;
				case TYPE_LEGS:
					sdl.dst.x = LegsX; sdl.dst.y = LegsY;
					break;
				case TYPE_GAUNTS:
					sdl.dst.x = GauntsX; sdl.dst.y = GauntsY;
					break;
				case TYPE_RING1:
					sdl.dst.x = RingX; sdl.dst.y = RingY;
					break;
				case TYPE_AMULET:
					sdl.dst.x = AmuletX; sdl.dst.y = AmuletY;
					break;
				case TYPE_ROBE:
					sdl.dst.x = RobeX; sdl.dst.y = RobeY;
					break;
				}	

				if (iWeared[i].Family == FAMILY_WEAPON)
				{
					sdl.dst.x = WeaponX; sdl.dst.y = WeaponY;
				}
	
				sdl.dst.h = 20;
				sdl.dst.w = 20;

				DrawItem(iWeared[i].Family, iWeared[i].Type, iWeared[i].ID);
			}
		}

		//------------------------------------------------------------------------------
		//	Place items on the ground
		//------------------------------------------------------------------------------
		if (i < MaxItems)
		{
			if (iGround[i].Used)
			{
				if (pldatabase->own->x - iGround[i].x > 11 ||
					pldatabase->own->x - iGround[i].x < -11 ||
					pldatabase->own->y - iGround[i].y > 11 ||
					pldatabase->own->y - iGround[i].y < -11)
				{
					Item_Remove( i, ItemGround );
				}
				else
				if (pldatabase->own->x - iGround[i].x < 11 &&
					pldatabase->own->x - iGround[i].x > -11 &&
					pldatabase->own->y - iGround[i].y < 11 &&
					pldatabase->own->y - iGround[i].y > -11 && MapInfo.Visible[-pldatabase->own->x + iGround[i].x + 10][-pldatabase->own->y + iGround[i].y + 10] > 3)
				{
					sdl.dst.x = (-pldatabase->own->x + iGround[i].x + 10) * 20 + 7;
					sdl.dst.y = (-pldatabase->own->y + iGround[i].y + 10) * 20 + 7;
					sdl.dst.h = 20;
					sdl.dst.w = 20;	

					DrawItem(iGround[i].Family, iGround[i].Type, iGround[i].ID);
				}
			}
		}
	}
}




//==============================================================================
//
//	Draw everything
//
//==============================================================================
void DrawClass::drawAll()
{
}