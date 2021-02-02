//---------------------------------------------------//
//   Quick key slot source file ( quickslots.cpp )   //
//													 //
//   Includes functions of quick key slot class      //
//						   ( class QuickSlotClass )  //
//													 //
//   (c) Jussi Witick								 //
//---------------------------------------------------//


#include "main.h"


//-------------------------------------------------------------------------------
// Function to visually draw quick-key slots to screen
void QuickSlotClass::Draw()
{
	sdl.dst.x = 433;
	sdl.dst.y = 416;
	sdl.dst.h = 26;
	sdl.dst.w = 358;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[QUICKSLOTS], Screen.screen, &sdl.dst);

	int i;
	for (i = 0; i < 10; i++)
	{
		if (QuickSlotTargets[i] != -1)
		{
			if (QuickSlotTypes[i] == 1)
			{
				if (!iBackpack[QuickSlotTargets[i]].Used) 
				{
					FreeSlot(i);
				}
				else
				{
					sdl.dst.x = 435 + i * 37;
					sdl.dst.y = 418;
					sdl.dst.h = 20;
					sdl.dst.w = 20;
	
					if (iBackpack[QuickSlotTargets[i]].Family == FAMILY_WEAPON)
						SDL_BlitSurface(G.items, &Weapons[ iBackpack[QuickSlotTargets[i]].Type ][ iBackpack[QuickSlotTargets[i]].ID ].Frames[ Frames[Weapons[ iBackpack[QuickSlotTargets[i]].Type ][ iBackpack[QuickSlotTargets[i]].ID ].anim_Type][Weapons[ iBackpack[QuickSlotTargets[i]].Type ][ iBackpack[QuickSlotTargets[i]].ID ].num_Frames] ], Screen.screen, &sdl.dst);
					if (iBackpack[QuickSlotTargets[i]].Family == FAMILY_ARMOR)
						SDL_BlitSurface(G.items, &Armors[ iBackpack[QuickSlotTargets[i]].Type -11 ][ iBackpack[QuickSlotTargets[i]].ID ].Frames[ Frames[Armors[ iBackpack[QuickSlotTargets[i]].Type -11 ][ iBackpack[QuickSlotTargets[i]].ID ].anim_Type][Armors[ iBackpack[QuickSlotTargets[i]].Type -11 ][ iBackpack[QuickSlotTargets[i]].ID ].num_Frames] ], Screen.screen, &sdl.dst);
					if (iBackpack[QuickSlotTargets[i]].Family == FAMILY_COLLECTABLE)
						SDL_BlitSurface(G.items, &Collectables[ iBackpack[QuickSlotTargets[i]].ID ].Frames[ Frames[Collectables[ iBackpack[QuickSlotTargets[i]].ID ].anim_Type][Collectables[ iBackpack[QuickSlotTargets[i]].ID ].num_Frames] ], Screen.screen, &sdl.dst);
					if (iBackpack[QuickSlotTargets[i]].Family == FAMILY_USEABLE)
						SDL_BlitSurface(G.items, &Useables[ iBackpack[QuickSlotTargets[i]].ID ].Frames[ Frames[Useables[ iBackpack[QuickSlotTargets[i]].ID ].anim_Type][Useables[ iBackpack[QuickSlotTargets[i]].ID ].num_Frames] ], Screen.screen, &sdl.dst);
				}
			}
		}	
	}
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//
void QuickSlotClass::FreeSlot(short Slot)
{
	QuickSlotTargets[Slot] = -1;
	QuickSlotTypes[Slot] = -1;
}
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------
//
void QuickSlotClass::SetSlot(short Slot, short Type, short Target)
{
	QuickSlotTargets[Slot] = Target;
	QuickSlotTypes[Slot] = Type;
}
//-------------------------------------------------------------------------------

void QuickSlotClass::CheckQuickSlots( int key)
{	
	int Number;

	if (key == SDLK_1) Number = 0;
	if (key == SDLK_2) Number = 1;
	if (key == SDLK_3) Number = 2;
	if (key == SDLK_4) Number = 3;
	if (key == SDLK_5) Number = 4;
	if (key == SDLK_6) Number = 5;
	if (key == SDLK_7) Number = 6;
	if (key == SDLK_8) Number = 7;
	if (key == SDLK_9) Number = 8;
	if (key == SDLK_0) Number = 9;

	Uint32 Pixel;
	Uint8 r=0,g=0,b=0;
	int x1,y1;
	bool Fail = false;

	if (MX >= Backpack.X && MX <= Backpack.X + Backpack_w && MY >= Backpack.Y && MY <= Backpack.Y + Backpack_h)
	{
		bool er;
		int i;
		for (i = MaxBackpackItems-1; i >= 0; i--)
		{
			er = false;

			if (iBackpack[i].Used)
			{
				if ((MX-Backpack.X-3) - iBackpack[i].x >= 0 &&
					(MX-Backpack.X-3) - iBackpack[i].x < 20 &&
					(MY-Backpack.Y-17) - iBackpack[i].y >= 0 &&
					(MY-Backpack.Y-17) - iBackpack[i].y < 20 && is_slotitem(iBackpack[i].ID, iBackpack[i].Type, iBackpack[i].Family))
				{	
					if (iBackpack[i].Family == FAMILY_WEAPON)
						x1 = MX - iBackpack[i].x + Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].Frames[ Frames[Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].anim_Type][Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_ARMOR)
						x1 = MX - iBackpack[i].x + Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].Frames[ Frames[Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].anim_Type][Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_COLLECTABLE)
						x1 = MX - iBackpack[i].x + Collectables[ iBackpack[i].ID ].Frames[ Frames[Collectables[ iBackpack[i].ID ].anim_Type][Collectables[ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_USEABLE)
						x1 = MX - iBackpack[i].x + Useables[ iBackpack[i].ID ].Frames[ Frames[Useables[ iBackpack[i].ID ].anim_Type][Useables[ iBackpack[i].ID ].num_Frames] ].x;
		
					if (iBackpack[i].Family == FAMILY_WEAPON)
						y1 = MY - iBackpack[i].y + Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].Frames[ Frames[Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].anim_Type][Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_ARMOR)
						y1 = MY - iBackpack[i].y + Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].Frames[ Frames[Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].anim_Type][Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_COLLECTABLE)
						y1 = MY - iBackpack[i].y + Collectables[ iBackpack[i].ID ].Frames[ Frames[Collectables[ iBackpack[i].ID ].anim_Type][Collectables[ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_USEABLE)
						y1 = MY - iBackpack[i].y + Useables[ iBackpack[i].ID ].Frames[ Frames[Useables[ iBackpack[i].ID ].anim_Type][Useables[ iBackpack[i].ID ].num_Frames] ].y;


					SDL_LockSurface(G.items);
					Pixel = GetPixel(G.items, x1, y1);
					SDL_UnlockSurface(G.items);

					SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);

					if (r != 255 || g != 0 || b != 255)
					{
						Fail = true;
						SetSlot(Number, 1, i);

						for (int j = 0; j < 10; j++)
						{
							if (QuickSlotTargets[j] == i && j != Number)
							{
								FreeSlot(j);
							}
						}
						i = 0;
					}
				}
			}
		}
	}
	if (!Fail)
	{
		if (QuickSlotTargets[Number] != -1)
		{
			if (QuickSlotTypes[Number] == 1) // Backpack item
			{
				if (iBackpack[QuickSlotTargets[Number]].Type == TYPE_USEABLEITEM)
				{
					Use = true;
					UseTarget = QuickSlotTargets[Number];

					sprintf(MB.NewMsg, "Where do you wish to mine at?");
					MB.NewType = 2;
					MB.Update();
				}

				if (iBackpack[QuickSlotTargets[Number]].Type == TYPE_COLLECTABLE)
				{
					if (Collectables[iBackpack[QuickSlotTargets[Number]].ID].CureType > 0 && iBackpack[QuickSlotTargets[Number]].Quantity > 0)
					{
						buffer[0] = 21;
						memcpy(&buffer[1], &iBackpack[QuickSlotTargets[Number]].KnowID, sizeof(iBackpack[QuickSlotTargets[Number]].KnowID));
						send(Socket, buffer, 1 + sizeof(iBackpack[QuickSlotTargets[Number]].KnowID), 0);
					}
				}
				if (iBackpack[QuickSlotTargets[Number]].Family == FAMILY_USEABLE)
				{
					if (Useables[iBackpack[QuickSlotTargets[Number]].ID].use_type != use_FORGE) return;
		
					bool Er;
					Er = false;


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
					for (int j = 0; j < 9; j++)
					{
						if (hshx[j] >= 0 && hshy[j] >= 0)
						{
							obj_List = Objects[hshx[j]][hshy[j]];

							while (obj_List != NULL)
							{
								ox = obj_List->X;
								oy = obj_List->Y;
								if (its(pldatabase->own->x, ox) <= 1 && its(pldatabase->own->y, oy) <= 1 && ObjectTypes[obj_List->Type].UseType == use_FORGE)
								{
									Er = true;
									j = 9;
									obj_List = NULL;
								}										
			
								if (obj_List != NULL)
									obj_List = obj_List->next;	
							}			
						}
					}

					if (Er)
					{
						sprintf(MB.NewMsg, "Choose ingots you wish to forge.");
						MB.NewType = 2;
						MB.Update();

						Use = true;
						UseTarget = QuickSlotTargets[Number];
					}
					if (!Er)
					{
						sprintf(MB.NewMsg, "You must find anvil to forge at!");
						MB.NewType = 2;
						MB.Update();
					}
				}
			}
		}
	}
}

bool QuickSlotClass::is_slotitem(short id, short type, short family)
{
	if (family == fam_use)
	{
		if (id <= 3) return true;
	}
	if (family == fam_col)
	{
		if (id == 1 || id == 2 || id == 3 || id == 19 || id == 45 || id == 46 || id == 47) return true;
	}
	return false;
}