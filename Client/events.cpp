#include "main.h"

#pragma warning(disable:4244)
#pragma warning(disable:4018)

extern SDL_Event event;
extern SOCKET Socket;

extern unsigned char GMMode;
extern unsigned char Invisibility;

extern bool UP,DOWN,LEFT,RIGHT;
extern bool isKicked;
extern bool isBanned;
extern bool Tab;
extern bool Shift;
extern bool Alt;
extern bool Identify;
extern bool Use;
extern bool BrowseOpt;
extern bool IdentifySomeone;

extern char *buffer;

extern int MX, MY;
extern int ChosenSkill;

bool FirstBuyClick = false;
bool ExitClick = false;

unsigned long ExitClickTime = 0;

void MouseOver()
{
}

//==============================================================================
//
//	On RMB Down
//
//==============================================================================
void MouseRightButtonDown()
{
	Uint16 mx, my;

	mx = event.motion.x;
	my = event.motion.y;

	if (mx <= Minimap->X + Minimap_w && mx >= Minimap->X && my <= Minimap->Y + Minimap_h && my >= Minimap->Y && Minimap->On && !Client.MouseMoving)
	{
	/*	Minimap.Size = true;

		if (Minimap.X + Minimap_w_BIG > 800) Minimap.X = 800 - Minimap_w_BIG;
		if (Minimap.Y + Minimap_h_BIG > 600) Minimap.Y = 600 - Minimap_h_BIG;
	*/		
		Minimap->Size = !Minimap->Size;

		return;
	}


	if (BrowseOpt == true)
	{
		return;
	}

	if (Minimap->Size && GMMode > 1)
	{
		if (mx >= 7 && mx <= 791 && my >= 5 && my <= 597)
		{
			PACKET_TELEPORTSELF Teleport;

			Teleport.X = (mx-7) + (pldatabase->own->x - 392);
			Teleport.Y = (my-5) + (pldatabase->own->y - 296);

			buffer[0] = 60;
			buffer[1] = 10;
			memcpy(&buffer[2], &Teleport, sizeof(Teleport));

			send(Socket, buffer, 2 + sizeof(Teleport), 0);
			//Minimap->Size = false;
		}
	} else if (Minimap->Size && GMMode <= 1)
	{
		Minimap->Size = false;
	}

	if ( mx >= 7 && mx <= 427 && my >= 7 && my <= 427)
	{
		if (mx <= Backpack.X + Backpack_w && mx >= Backpack.X && my <= Backpack.Y + Backpack_h && my >= Backpack.Y && Backpack.On && !Client.MouseMoving)
		{
			return;
		}
		if (mx <= Bank.X + Backpack_w && mx >= Bank.X && my <= Bank.Y + Backpack_h && my >= Bank.Y && Bank.On && !Client.MouseMoving)
		{
			return;
		}
    	if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245)
			return;

		if (GMMode > 0)
		{
/*			PlayerNode *tmp;
			tmp = pldatabase->players;

			short tx, ty;

			tx = (mx - 7) / 20 + pldatabase->own->x - 10;
			ty = (my - 7) / 20 + pldatabase->own->y - 10;

			while (tmp)
			{
				if (tmp->x == tx && tmp->y == ty)
				{
					GMTool->dropDownMenu(tmp->id, mx, my);
					tmp = NULL;
				}
		
				if (tmp) tmp = tmp->next;
			}*/
		}
		
		Client.MouseMoving = true;
	}
}


//==============================================================================
//
//	On RMB Up
//
//==============================================================================
void MouseRightButtonUp()
{
	Uint16 mx, my;

	mx = event.motion.x;
	my = event.motion.y;
	
	bool Err = false, Err2 = false;;

	if (Client.MouseMoving)
	{
		Client.MouseMoving = false;
		Err2 = true;
	}

//	if (Minimap.Size)
//		Minimap.Size = false;

	if (Writing.Writing && Writing.Mode == 1)
	{
		if (my >= 410 && my <= 421 && mx >= 76 && mx <= 87) // Change to serverwide chat
		{
            ListenToServerChat = !ListenToServerChat;
		}
	}


	if (mx <= Backpack.X + Backpack_w && mx >= Backpack.X && my <= Backpack.Y + Backpack_h && my >= Backpack.Y && Backpack.On && !Err2)
	{	
		bool Error = false;

		// Let's check if we used an item.

		// Here will be check if player starts to drag any item.
	
		mx -= Backpack.X; mx -= 3;
		my -= Backpack.Y; my -= 17;
		if (my < 0) my = 0;
		if (mx < 0) mx = 0;


		Uint32 Pixel;
	
		Uint8 r, g, b;

		short x1, y1;


		int i;
		for (i = MaxBackpackItems-1; i >= 0; i--)
		{
			if (iBackpack[i].Used)
			{
				if (mx - iBackpack[i].x >= 0 &&
					mx - iBackpack[i].x < 20 &&
					my - iBackpack[i].y >= 0 &&
					my - iBackpack[i].y < 20)
				{	
	
					if (iBackpack[i].Family == FAMILY_WEAPON)
						x1 = mx - iBackpack[i].x + Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].Frames[ Frames[Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].anim_Type][Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_ARMOR)
						x1 = mx - iBackpack[i].x + Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].Frames[ Frames[Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].anim_Type][Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_COLLECTABLE)
						x1 = mx - iBackpack[i].x + Collectables[ iBackpack[i].ID ].Frames[ Frames[Collectables[ iBackpack[i].ID ].anim_Type][Collectables[ iBackpack[i].ID ].num_Frames] ].x;
					if (iBackpack[i].Family == FAMILY_USEABLE)
						x1 = mx - iBackpack[i].x + Useables[ iBackpack[i].ID ].Frames[ Frames[Useables[ iBackpack[i].ID ].anim_Type][Useables[ iBackpack[i].ID ].num_Frames] ].x;

					if (iBackpack[i].Family == FAMILY_WEAPON)
						y1 = my - iBackpack[i].y + Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].Frames[ Frames[Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].anim_Type][Weapons[ iBackpack[i].Type ][ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_ARMOR)
						y1 = my - iBackpack[i].y + Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].Frames[ Frames[Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].anim_Type][Armors[ iBackpack[i].Type -11 ][ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_COLLECTABLE)
						y1 = my - iBackpack[i].y + Collectables[ iBackpack[i].ID ].Frames[ Frames[Collectables[ iBackpack[i].ID ].anim_Type][Collectables[ iBackpack[i].ID ].num_Frames] ].y;
					if (iBackpack[i].Family == FAMILY_USEABLE)
						y1 = my - iBackpack[i].y + Useables[ iBackpack[i].ID ].Frames[ Frames[Useables[ iBackpack[i].ID ].anim_Type][Useables[ iBackpack[i].ID ].num_Frames] ].y;

					SDL_LockSurface(G.items);
					Pixel = GetPixel(G.items, x1, y1);
					SDL_UnlockSurface(G.items);
	
		            SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);
						
	
					if (r != 255 || g != 0 || b != 255)
					{
						if (iBackpack[i].Type == TYPE_COLLECTABLE)
						{
							if (Collectables[iBackpack[i].ID].CureType > 0 && iBackpack[i].Quantity > 0)
							{
	                            // We used this item.
								
								buffer[0] = 21;
								memcpy(&buffer[1], &iBackpack[i].KnowID, sizeof(iBackpack[i].KnowID));
								send(Socket, buffer, 1 + sizeof(iBackpack[i].KnowID), 0);
	
								Error = true;
							}					
						}
						if (iBackpack[i].Family == FAMILY_ARMOR || iBackpack[i].Family == FAMILY_WEAPON)
						{
							buffer[0] = 23;
							memcpy(&buffer[1], &iBackpack[i].KnowID, sizeof(iBackpack[i].KnowID));
							send(Socket, buffer, 1 + sizeof(iBackpack[i].KnowID), 0);
	
							Error = true;
						}

						if (iBackpack[i].Family == FAMILY_USEABLE)
						{
							if (Useables[iBackpack[i].ID].use_type == use_MINE)
							{
								Use = true;
								UseTarget = i;
	
								sprintf(MB.NewMsg, "Where do you wish to mine at?");
								MB.NewType = 2;
								MB.Update();
	
								Error = true;
							}
						}

						if (iBackpack[i].Family == FAMILY_USEABLE)
						{
							if (Useables[iBackpack[i].ID].use_type != use_FORGE) break;

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
								UseTarget = i;
							}
							if (!Er)
							{
								sprintf(MB.NewMsg, "You must find anvil to forge at!");
								MB.NewType = 2;
								MB.Update();
							}

							Error = true;
						}
						if (iBackpack[i].Family == FAMILY_COLLECTABLE)
						{
							if (Collectables[iBackpack[i].ID].use_type == use_SMELT)
							{
								Use = true;
								UseTarget = i;

								sprintf(MB.NewMsg, "Choose object where you wish to smelt at.");
								MB.NewType = 2;
								MB.Update();

								Error = true;
							}
						}
					}

					if (Error) i = 0;
				}
			}
		}			



		if (!Error)
		{
			if (Backpack.Docked)
			{
				Err = true;
				Backpack.Docked = false;		
			}
			else
			{
				Backpack.Docked = true;			
			}
		}
		return;
	}

	if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245 && !Err2)
	{
		short tmx, tmy;
			
		tmx = mx - 186;
		tmy = my - 186;

		short tx, ty;

		tx = tmx / 20 - 1;
		ty = tmy / 20 - 1;

		tmx = pldatabase->own->x + tx;
		tmy = pldatabase->own->y + ty;

		short hshx = tmx / 32, hshy = tmy / 32;

		obj_List = Objects[hshx][hshy];

		while (obj_List != NULL)
		{
			if (obj_List->X == tmx && obj_List->Y == tmy && obj_List->On)
			{
				if (obj_List->Type == 10)
				{
					sprintf(MB.NewMsg, "%s", cStrings->getString(obj_List->Text));
					MB.NewType = 7;
					MB.Update();
					return;
				}
				else
				{
					buffer[0] = 53;
					memcpy(&buffer[1], &tmx, sizeof(tmx));
					memcpy(&buffer[1 + sizeof(tmx)], &tmy, sizeof(tmy));
					send(Socket, buffer, 1 + sizeof (tmx) + sizeof (tmy), 0);
					return;
				}
				obj_List = NULL;
			}
			
			if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		BodyClass *body;
		body = bodydatabase->body;

		while (body)
		{
			if (body->x == tmx && body->y == tmy)
			{
				buffer[0] = 27;
				buffer[1] = 1;				
				memcpy(&buffer[2], &body->id, sizeof(body->id));
				send(Socket, buffer, 2 + sizeof(body->id), 0);

				v_Body.On = true;
				v_Body_Open = body->id;
				return;
			}
			body = body->next;		
		}

		int i;
		for (i = 0; i < MaxItems; i++)
		{
			if (iGround[i].Used)
			{
				if (iGround[i].x == tmx && iGround[i].y == tmy)
				{
                    buffer[0] = 24;
					memcpy(&buffer[1], &iGround[i].KnowID, sizeof(iGround[i].KnowID));

					send(Socket, buffer, 1 + sizeof(iGround[i].KnowID), 0);
					return;					
				}	
			}
		}
	}
	
	if (mx <= Bank.X + Bank_w && mx >= Bank.X && my <= Bank.Y + Bank_h && my >= Bank.Y && Bank.On && !Err2)
	{
		Bank.On = false;
        return;        	
	}

	if (mx >= v_Body.X && mx <= v_Body.X + Backpack_w && my >= v_Body.Y && my <= v_Body.Y + Backpack_h && v_Body.On)
	{
		mx -= v_Body.X; mx -= 3;
		my -= v_Body.Y; my -= 17;
		if (my < 0) my = 0;
		if (mx < 0) mx = 0;


		Uint32 Pixel;
	
		Uint8 r, g, b;

		short x1, y1;

		BodyClass *body;
		body = bodydatabase->getbyid(v_Body_Open);
		if (body == NULL) return;

		
		for (short i = 83; i >= 0; i--)
		{
			if (body->Items[i].Used)
			{
				if (mx - body->Items[i].x >= 0 &&
					mx - body->Items[i].x < 20 &&
					my - body->Items[i].y >= 0 &&
					my - body->Items[i].y < 20)
				{	
	
					if (body->Items[i].Family == FAMILY_WEAPON)
						x1 = mx - body->Items[i].x + Weapons[ body->Items[i].Type ][ body->Items[i].ID ].Frames[ Frames[Weapons[ body->Items[i].Type ][ body->Items[i].ID ].anim_Type][Weapons[ body->Items[i].Type ][ body->Items[i].ID ].num_Frames] ].x;
					if (body->Items[i].Family == FAMILY_ARMOR)
						x1 = mx - body->Items[i].x + Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].Frames[ Frames[Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].anim_Type][Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].num_Frames] ].x;
					if (body->Items[i].Family == FAMILY_COLLECTABLE)
						x1 = mx - body->Items[i].x + Collectables[ body->Items[i].ID ].Frames[ Frames[Collectables[ body->Items[i].ID ].anim_Type][Collectables[ body->Items[i].ID ].num_Frames] ].x;
					if (body->Items[i].Family == FAMILY_USEABLE)
						x1 = mx - body->Items[i].x + Useables[ body->Items[i].ID ].Frames[ Frames[Useables[ body->Items[i].ID ].anim_Type][Useables[ body->Items[i].ID ].num_Frames] ].x;

					if (body->Items[i].Family == FAMILY_WEAPON)
						y1 = my - body->Items[i].y + Weapons[ body->Items[i].Type ][ body->Items[i].ID ].Frames[ Frames[Weapons[ body->Items[i].Type ][ body->Items[i].ID ].anim_Type][Weapons[ body->Items[i].Type ][ body->Items[i].ID ].num_Frames] ].y;
					if (body->Items[i].Family == FAMILY_ARMOR)
						y1 = my - body->Items[i].y + Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].Frames[ Frames[Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].anim_Type][Armors[ body->Items[i].Type -11 ][ body->Items[i].ID ].num_Frames] ].y;
					if (body->Items[i].Family == FAMILY_COLLECTABLE)
						y1 = my - body->Items[i].y + Collectables[ body->Items[i].ID ].Frames[ Frames[Collectables[ body->Items[i].ID ].anim_Type][Collectables[ body->Items[i].ID ].num_Frames] ].y;
					if (body->Items[i].Family == FAMILY_USEABLE)
						y1 = my - body->Items[i].y + Useables[ body->Items[i].ID ].Frames[ Frames[Useables[ body->Items[i].ID ].anim_Type][Useables[ body->Items[i].ID ].num_Frames] ].y;

					SDL_LockSurface(G.items);
					Pixel = GetPixel(G.items, x1, y1);
					SDL_UnlockSurface(G.items);
	
		            SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);
						
	
					if (r != 255 || g != 0 || b != 255)
					{
                        buffer[0] = 27;
						buffer[1] = 2;
						memcpy(&buffer[2], &body->id, sizeof(body->id));
						memcpy(&buffer[4], &i, sizeof(i));
						send(Socket, buffer, 2 + sizeof(body->id) + sizeof(i), 0);
						i = -1;
					}
				}
			}
		}
		return;
	}


	if (mx >= 433 && mx <= 439 && my >= 115 && my <= 163 && P.StatPoints > 0 && !Err2)
	{
		int i;
		for (i = 0; i < 4; i++)
		{
			if (my >= 115 + i * 12 && my <= 123 + i * 12)
			{
                buffer[0] = 22;
				buffer[1] = i + 1;
				send(Socket, buffer, 2, 0);

				return;
			}
		}
	}
}


//==============================================================================
//
//	On LMB Down
//
//==============================================================================
void MouseButtonDown()
{
	Uint16 mx, my;

	mx = event.motion.x;
	my = event.motion.y;

	// Check if they are being kicked or banned
	if (isKicked) exit(1);
	if (isBanned) exit(1);

	// Mousebutton was pressed down, check if player starts to drag anything.
	// This is order for dragging:
	// 0) Split window's buttons
	// 1) Backpack
	// 2) Bank
	// 3) Minimap
	// 4) Identifywindow
	// 5) Item on playscreen
	// 6) Item weared
	//############################

//	if (Minimap.Size)
//		Minimap.Size = false;


	if (BrowseOpt == true)
	{
		return;
	}

	identify* tmp;
	tmp = idatabase->list;
	while (tmp)
	{
		if (tmp->ison())
		{
			Dragging.Start(IDENTIFY);
			tmp = NULL;
		}

		if (tmp) tmp = tmp->next;
	}


	bool Target2Error = false;
	bool TargetError = false;
	short tx, ty;


	MonsterClass* mon;
	mon = mondatabase->monsters;

	while (mon)
	{
		tx = (-pldatabase->own->x + mon->x + 10) * 20 + 7;
		ty = (-pldatabase->own->y + mon->y + 10) * 20 + 7;



		if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && Client.AttackMode == ATTACK && MapInfo.Visible[-pldatabase->own->x + mon->x + 10][-pldatabase->own->y + mon->y + 10] > 3)
		{
			Target.DrawTarget = true;
			Target.MonTarget = mon;
			Target.PlTarget = NULL;
			Target.NPCTarget = NULL;
			Target.Type = MONSTER;


			Target.Frame = 0;
			Target.FramePlus = 1;
			Target.FrameTime = SDL_GetTicks();
			Target.TarID = mon->knowid;

			buffer[0] = 30;
			buffer[1] = 1;
			memcpy(&buffer[2], &Target.TarID, sizeof(Target.TarID));

			send(Socket, buffer, 2 + sizeof(Target.TarID), 0);

			TargetError = true;
			Target2Error = true;
			mon = NULL;
		}
		else if (Client.AttackMode == PEACE && Identify && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + mon->x + 10][-pldatabase->own->y + mon->y + 10] > 3)
		{
			// Identify monster.

			sprintf(MB.NewMsg, "You identify %s, level %d monster.", MonsterTypes[mon->type].Name, MonsterTypes[mon->type].MonsterLevel);
			MB.NewType = 7;
			MB.Update();
			if (P.Int > 20)
			{
				sprintf(MB.NewMsg, "[Moving speed]: %d   [Attack speed]: %d", MonsterTypes[mon->type].MovingSpeed, MonsterTypes[mon->type].AttackSpeed);
				MB.NewType = 7;
				MB.Update();
			}
			if (P.Int > 40)
			{
				sprintf(MB.NewMsg, "[HP]: %d / %d", mon->hpleft, MonsterTypes[mon->type].HPMax);
				MB.NewType = 7;
				MB.Update();
			}
			if (P.Int > 60)
			{
				sprintf(MB.NewMsg, "[Damage]: %d - %d", MonsterTypes[mon->type].DamMin, MonsterTypes[mon->type].DamMax);
				MB.NewType = 7;
				MB.Update();
			}
			if (P.Int > 80)
			{
				sprintf(MB.NewMsg, "[AC]: %d   [Dex]: %d", MonsterTypes[mon->type].AC, MonsterTypes[mon->type].Dex);
				MB.NewType = 7;
				MB.Update();
			}
			TargetError = true;
			Target2Error = true;
			mon = NULL;
		}
		else if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20)
		{
			TargetError = true;
			mon = NULL;
		}

		if (mon != NULL) mon = mon->next;
	}

	if (!TargetError)
	{
		NPCClass* np;
		np = npcdatabase->npc;

		while (np)
		{
			tx = (-pldatabase->own->x + np->x + 10) * 20 + 7;
			ty = (-pldatabase->own->y + np->y + 10) * 20 + 7;

			if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && Client.AttackMode == ATTACK && MapInfo.Visible[-pldatabase->own->x + np->x + 10][-pldatabase->own->y + np->y + 10] > 3)
			{
				// NPC targetted.

				if (Client.TargetProtectionMode)
				{
					sprintf(MB.NewMsg, "Target protection mode prevents you from targetting this NPC.");
					MB.NewType = 7;
					MB.Update();
				}
				else
				{
					Target.DrawTarget = true;
					Target.MonTarget = NULL;
					Target.PlTarget = NULL;
					Target.NPCTarget = np;
					Target.Type = T_NPC;


					Target.Frame = 0;
					Target.FramePlus = 1;
					Target.FrameTime = SDL_GetTicks();
					Target.TarID = np->id;

					buffer[0] = 30;
					buffer[1] = 3;
					memcpy(&buffer[2], &Target.TarID, sizeof(Target.TarID));

					send(Socket, buffer, 2 + sizeof(Target.TarID), 0);
				}

				TargetError = true;
				Target2Error = true;
				np = NULL;
			}
			else if (Client.AttackMode == PEACE && !Identify && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + np->x + 10][-pldatabase->own->y + np->y + 10] > 3)
			{
				// Start conv. with NPC, if close enough.

				if (pldatabase->own->x - np->x > -3 && pldatabase->own->x - np->x < 3 &&
					pldatabase->own->y - np->y > -3 && pldatabase->own->y - np->y < 3)
				{
					buffer[0] = 31;
					memcpy(&buffer[1], &np->id, sizeof(np->id));
					send(Socket, buffer, 1 + sizeof(np->id), 0);
				}
				Target2Error = true;
				TargetError = true;
				np = NULL;
			}
			else if (Client.AttackMode == PEACE && Identify && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + np->x + 10][-pldatabase->own->y + np->y + 10] > 3)
			{
				// Identify NPC

				sprintf(MB.NewMsg, "You see ò%s.", NPCTypes[np->type].Name);
				MB.NewType = 7;
				MB.Update();

				if (P.Int > 20)
				{
					sprintf(MB.NewMsg, "[HP]: %d", np->hp);
					MB.NewType = 7;
					MB.Update();
				}

				TargetError = true;
				Target2Error = true;
				np = NULL;
			}
			else if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20)
			{
				TargetError = true;
				np = NULL;
			}

			if (np != NULL) np = np->next;
		}
	}

	if (!TargetError)
	{
		PlayerNode* pl;
		pl = pldatabase->players;

		while (pl)
		{
			tx = (-pldatabase->own->x + pl->x + 10) * 20;
			ty = (-pldatabase->own->y + pl->y + 10) * 20;

			if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && pl != pldatabase->own &&
				Client.AttackMode == ATTACK && !Identify && MapInfo.Visible[-pldatabase->own->x + pl->x + 10][-pldatabase->own->y + pl->y + 10] > 3)
			{
				if (Client.TargetProtectionMode && !pl->wanted)
				{
					strcpy(MB.NewMsg, "Target protection mode prevents you from targeting this Player.");
					MB.NewType = 7;
					MB.Update();
				}
				else
				{
					Target.PlTarget = pl;
					Target.MonTarget = NULL;
					Target.NPCTarget = NULL;
					Target.Type = PLAYER;

					Target.DrawTarget = true;

					Target.Frame = 0;
					Target.FramePlus = 1;
					Target.FrameTime = SDL_GetTicks();
					Target.TarID = pl->id;

					buffer[0] = 30;
					buffer[1] = 2;
					memcpy(&buffer[2], &Target.TarID, sizeof(Target.TarID));

					send(Socket, buffer, 2 + sizeof(Target.TarID), 0);
				}

				pl = NULL;

				TargetError = true;
				Target2Error = true;
			}
			else if (Client.AttackMode == PEACE && Identify && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + pl->x + 10][-pldatabase->own->y + pl->y + 10] > 3)
			{
				// Identify player.

				if (pl != pldatabase->own)
				{
					buffer[0] = 25;
					memcpy(&buffer[1], &pl->id, sizeof(pl->id));
					send(Socket, buffer, 1 + sizeof(pl->id), 0);
				}
				else
				{
					buffer[0] = 26;
					send(Socket, buffer, 1, 0);
				}

				pl = NULL;

				TargetError = true;
				Target2Error = true;
			}
			else if (Client.AttackMode == PEACE && Shift && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + pl->x + 10][-pldatabase->own->y + pl->y + 10] > 3)
			{
				pl = NULL;

				TargetError = true;
				Target2Error = true;
			}
			else if (Client.AttackMode == PEACE && Alt && mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20 && MapInfo.Visible[-pldatabase->own->x + pl->x + 10][-pldatabase->own->y + pl->y + 10] > 3)
			{
				pl = NULL;

				TargetError = true;
				Target2Error = true;
			}
			else if (mx >= tx && mx <= tx + 20 && my >= ty && my <= ty + 20)
				TargetError = true;

			if (pl != NULL) pl = pl->next;
		}
	}


	if (TargetError) return;

	if (!Dragging.Split && !Use)
	{
		Uint32 Pixel;

		Uint8 r, g, b;

		short x1, y1;

		// Check for bank

		if (mx <= Bank.X + Bank_w && mx >= Bank.X && my <= Bank.Y + Bank_h && my >= Bank.Y && Bank.On)
		{
			// Here will be check if player starts to drag any item.
			mx -= Bank.X; mx -= 3;
			my -= Bank.Y; my -= 17;

			int i;
			for (i = MaxBankItems - 1; i >= 0; i--)
			{
				if (iBank[i].Used)
				{
					if (mx - iBank[i].x >= 0 &&
						mx - iBank[i].x < 20 &&
						my - iBank[i].y >= 0 &&
						my - iBank[i].y < 20)
					{
						if (iBank[i].Family == FAMILY_WEAPON)
							x1 = mx - iBank[i].x + Weapons[iBank[i].Type][iBank[i].ID].Frames[Frames[Weapons[iBank[i].Type][iBank[i].ID].anim_Type][Weapons[iBank[i].Type][iBank[i].ID].num_Frames]].x;
						if (iBank[i].Family == FAMILY_ARMOR)
							x1 = mx - iBank[i].x + Armors[iBank[i].Type - 11][iBank[i].ID].Frames[Frames[Armors[iBank[i].Type - 11][iBank[i].ID].anim_Type][Armors[iBank[i].Type - 11][iBank[i].ID].num_Frames]].x;
						if (iBank[i].Family == FAMILY_COLLECTABLE)
							x1 = mx - iBank[i].x + Collectables[iBank[i].ID].Frames[Frames[Collectables[iBank[i].ID].anim_Type][Collectables[iBank[i].ID].num_Frames]].x;
						if (iBank[i].Family == FAMILY_USEABLE)
							x1 = mx - iBank[i].x + Useables[iBank[i].ID].Frames[Frames[Useables[iBank[i].ID].anim_Type][Useables[iBank[i].ID].num_Frames]].x;

						if (iBank[i].Family == FAMILY_WEAPON)
							y1 = my - iBank[i].y + Weapons[iBank[i].Type][iBank[i].ID].Frames[Frames[Weapons[iBank[i].Type][iBank[i].ID].anim_Type][Weapons[iBank[i].Type][iBank[i].ID].num_Frames]].y;
						if (iBank[i].Family == FAMILY_ARMOR)
							y1 = my - iBank[i].y + Armors[iBank[i].Type - 11][iBank[i].ID].Frames[Frames[Armors[iBank[i].Type - 11][iBank[i].ID].anim_Type][Armors[iBank[i].Type - 11][iBank[i].ID].num_Frames]].y;
						if (iBank[i].Family == FAMILY_COLLECTABLE)
							y1 = my - iBank[i].y + Collectables[iBank[i].ID].Frames[Frames[Collectables[iBank[i].ID].anim_Type][Collectables[iBank[i].ID].num_Frames]].y;
						if (iBank[i].Family == FAMILY_USEABLE)
							y1 = my - iBank[i].y + Useables[iBank[i].ID].Frames[Frames[Useables[iBank[i].ID].anim_Type][Useables[iBank[i].ID].num_Frames]].y;

						Pixel = GetPixel(G.items, x1, y1);

						SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);

						if (r != 255 || g != 0 || b != 255)
						{
							if (Identify)
							{
								IdentifyItem(&iBank[i]);
							}

							if (!Identify)
							{
								// We found item to drag!

								if (iBank[i].Type == TYPE_COLLECTABLE && Shift)
								{
									if (iBank[i].Quantity > 1)
									{
										Dragging.Split = true;
										Dragging.SplitAmount = iBank[i].Quantity;
										Dragging.FromList = ItemBank;
									}
								}

								Dragging.TargetItem = &iBank[i];
								Dragging.FromList = ItemBank;
								Dragging.Start(ITEM);
							}
							i = -1;
						}
					}
				}
			}

			if (Dragging.TargetItem == NULL && !Identify)

				Dragging.Start(BANK);

			return; // Return prevents mouseclick from starting to drag other things under backpack.
		}




		if (mx <= Minimap->X + Minimap_w && mx >= Minimap->X && my <= Minimap->Y + Minimap_h && my >= Minimap->Y)
		{
			Dragging.Start(MINIMAP);
			return;
		}

		if (mx <= Backpack.X + Backpack_w && mx >= Backpack.X && my <= Backpack.Y + Backpack_h && my >= Backpack.Y && Backpack.On)
		{
			// Here will be check if player starts to drag any item.

			mx -= Backpack.X; mx -= 3;
			my -= Backpack.Y; my -= 17;
			if (my < 0) my = 0;
			if (mx < 0) mx = 0;


			int i;
			for (i = MaxBackpackItems - 1; i >= 0; i--)
			{
				if (iBackpack[i].Used)
				{
					if (mx - iBackpack[i].x >= 0 &&
						mx - iBackpack[i].x < 20 &&
						my - iBackpack[i].y >= 0 &&
						my - iBackpack[i].y < 20)
					{

						if (iBackpack[i].Family == FAMILY_WEAPON)
							x1 = mx - iBackpack[i].x + Weapons[iBackpack[i].Type][iBackpack[i].ID].Frames[Frames[Weapons[iBackpack[i].Type][iBackpack[i].ID].anim_Type][Weapons[iBackpack[i].Type][iBackpack[i].ID].num_Frames]].x;
						if (iBackpack[i].Family == FAMILY_ARMOR)
							x1 = mx - iBackpack[i].x + Armors[iBackpack[i].Type - 11][iBackpack[i].ID].Frames[Frames[Armors[iBackpack[i].Type - 11][iBackpack[i].ID].anim_Type][Armors[iBackpack[i].Type - 11][iBackpack[i].ID].num_Frames]].x;
						if (iBackpack[i].Family == FAMILY_COLLECTABLE)
							x1 = mx - iBackpack[i].x + Collectables[iBackpack[i].ID].Frames[Frames[Collectables[iBackpack[i].ID].anim_Type][Collectables[iBackpack[i].ID].num_Frames]].x;
						if (iBackpack[i].Family == FAMILY_USEABLE)
							x1 = mx - iBackpack[i].x + Useables[iBackpack[i].ID].Frames[Frames[Useables[iBackpack[i].ID].anim_Type][Useables[iBackpack[i].ID].num_Frames]].x;

						if (iBackpack[i].Family == FAMILY_WEAPON)
							y1 = my - iBackpack[i].y + Weapons[iBackpack[i].Type][iBackpack[i].ID].Frames[Frames[Weapons[iBackpack[i].Type][iBackpack[i].ID].anim_Type][Weapons[iBackpack[i].Type][iBackpack[i].ID].num_Frames]].y;
						if (iBackpack[i].Family == FAMILY_ARMOR)
							y1 = my - iBackpack[i].y + Armors[iBackpack[i].Type - 11][iBackpack[i].ID].Frames[Frames[Armors[iBackpack[i].Type - 11][iBackpack[i].ID].anim_Type][Armors[iBackpack[i].Type - 11][iBackpack[i].ID].num_Frames]].y;
						if (iBackpack[i].Family == FAMILY_COLLECTABLE)
							y1 = my - iBackpack[i].y + Collectables[iBackpack[i].ID].Frames[Frames[Collectables[iBackpack[i].ID].anim_Type][Collectables[iBackpack[i].ID].num_Frames]].y;
						if (iBackpack[i].Family == FAMILY_USEABLE)
							y1 = my - iBackpack[i].y + Useables[iBackpack[i].ID].Frames[Frames[Useables[iBackpack[i].ID].anim_Type][Useables[iBackpack[i].ID].num_Frames]].y;

						Pixel = GetPixel(G.items, x1, y1);

						SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);


						if (r != 255 || g != 0 || b != 255)
						{
							if (Identify)
							{
								IdentifyItem(&iBackpack[i]);
							}
							if (!Identify)
							{
								// We found item to drag!
								Dragging.TargetItem = &iBackpack[i];
								Dragging.FromList = ItemBackpack;

								if (iBackpack[i].Type == TYPE_COLLECTABLE && Shift)
								{
									if (iBackpack[i].Quantity > 1)
									{
										Dragging.Split = true;
										Dragging.SplitAmount = iBackpack[i].Quantity;
										Dragging.FromList = ItemBackpack;
									}
								}

								Dragging.Start(ITEM);
							}
							i = -1;
						}
					}
				}
			}

			if (Dragging.TargetItem == NULL && !Backpack.Docked && !Identify)
				// Otherwise he will start dragging backpack itself.

				Dragging.Start(BACKPACK);

			return; // Return prevents mouseclick from starting to drag other things under backpack.
		}

		if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245 && !Identify)
		{
			// Check if we start dragging something from playscreen.

			short tmx, tmy;

			tmx = mx - 186;
			tmy = my - 186;

			short tx, ty;

			tx = tmx / 20 - 1;
			ty = tmy / 20 - 1;

			tmx = pldatabase->own->x + tx;
			tmy = pldatabase->own->y + ty;


			int i;
			for (i = 0; i < MaxItems; i++)
			{
				if (iGround[i].Used)
				{
					if (iGround[i].x == tmx && iGround[i].y == tmy)
					{
						Dragging.TargetItem = &iGround[i];
						Dragging.FromList = ItemGround;

						Dragging.Start(ITEM);

						if (iGround[i].Type == TYPE_COLLECTABLE && Shift)
						{
							if (iGround[i].Quantity > 1)
							{
								Dragging.Split = true;
								Dragging.SplitAmount = iGround[i].Quantity;
								Dragging.FromList = ItemGround;
							}
						}
					}
				}
			}
			return;
		}

		int i;
		if (mx >= WeaponX - 2 && mx <= WeaponX + 22 && my >= WeaponY - 2 && my <= WeaponY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Family == FAMILY_WEAPON)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= PlateX - 2 && mx <= PlateX + 22 && my >= PlateY - 2 && my <= PlateY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_ARMOR)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= HelmX - 2 && mx <= HelmX + 22 && my >= HelmY - 2 && my <= HelmY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_HELM)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= ShieldX - 2 && mx <= ShieldX + 22 && my >= ShieldY - 2 && my <= ShieldY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_SHIELD)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;

		}
		if (mx >= LegsX - 2 && mx <= LegsX + 22 && my >= LegsY - 2 && my <= LegsY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_LEGS)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= GauntsX - 2 && mx <= GauntsX + 22 && my >= GauntsY - 2 && my <= GauntsY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_GAUNTS)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= RingX - 2 && mx <= RingX + 22 && my >= RingY - 2 && my <= RingY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_RING1)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= AmuletX - 2 && mx <= AmuletX + 22 && my >= AmuletY - 2 && my <= AmuletY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_AMULET)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
		if (mx >= RobeX - 2 && mx <= RobeX + 22 && my >= RobeY - 2 && my <= RobeY + 22)
		{
			for (i = 0; i < MaxWearedItems; i++)
			{
				if (iWeared[i].Used)
				{
					if (iWeared[i].Type == TYPE_ROBE)
					{
						if (Identify)
						{
							IdentifyItem(&iWeared[i]);
						}
						else
						{
							Dragging.TargetItem = &iWeared[i];
							Dragging.FromList = ItemWeared;
							Dragging.Start(ITEM);
						}
					}
				}
			}
			return;
		}
	}

	if (!Target2Error && !Identify)
	{
		// No target found, remove it from server and client.

		if (Target.PlTarget != NULL || Target.MonTarget != NULL || Target.NPCTarget != NULL)
		{
			buffer[0] = 30;
			buffer[1] = 4;

			send(Socket, buffer, 2, 0);

			Target.NPCTarget = NULL;
			Target.PlTarget = NULL;
			Target.MonTarget = NULL;
			Target.DrawTarget = false;
			Target.TarID = -1;
			Target.Type = -1;
		}
	}

	// 5)	
	// Chat scroll buttons			

	if (mx >= 10 && mx <= 22 && my >= 425 && my <= 455) // Go to older chat
	{
		int num;
		bool canAdd = true;
		for (int i = 0; i < 2000; i++)
		{
			num = i + MB.scroll;
			if (MB.MsgType[MB.scroll + 13] == NULL)
			{
				canAdd = false;
				i = 2000;
			}
		}

		if (canAdd)
		{
			MB.scroll++;
			MB.Print();
		}
	}
	if (mx >= 10 && mx <= 22 && my >= 570 && my <= 600) // Go to newer chat
	{
		MB.scroll--;
		if (MB.scroll <= 0)
			MB.scroll = 0;
		MB.Print();
	}
}


//==============================================================================
//
//	On LMB Up
//
//==============================================================================
void MouseButtonUp()
{
	int mx, my;

	bool TargetError = false;
	bool Target2Error = false;

	mx = event.motion.x;
	my = event.motion.y;

	// Dragging was released. If there was no dragging, we check if player tries to identify
	// someone or press button.
	// Order to check these:
	// 1) Was dragging released?
	// 2) Did player press exp/Alignment bar?
	// 3) Did we get new attacking target?
	// 4) Was some button clicked?
	//#################################

//	if (Minimap.Size)
//		Minimap.Size = false;

	// First check if the Options screen is up
	if (BrowseOpt == true)
	{
		if (mx >= 240 + BFont_TextWidth("Say Chat -") && mx <= 255 + BFont_TextWidth("Say Chat -") && my >= 145 && my <= 160) // Say Toggle
		{
			if (Client.SayOn == true)
				Client.SayOn = false;
			else
				Client.SayOn = true;
		}

		if (mx >= 240 + BFont_TextWidth("Whisper Chat -") && mx <= 255 + BFont_TextWidth("Whisper Chat -") && my >= 175 && my <= 190) // Whisper Toggle
		{
			if (Client.WhisOn == true)
				Client.WhisOn = false;
			else
				Client.WhisOn = true;
		}

		if (mx >= 240 + BFont_TextWidth("Global Chat -") && mx <= 255 + BFont_TextWidth("Global Chat -") && my >= 205 && my <= 220) // Global Toggle
		{
			if (Client.GlobOn == true)
				Client.GlobOn = false;
			else
				Client.GlobOn = true;
		}


		if (mx >= 490 + BFont_TextWidth("WASD Movement -") && mx <= 505 + BFont_TextWidth("WASD Movement -") && my >= 145 && my <= 160) // WASD Toggle
		{
			if (Client.WASD == true)
				Client.WASD = false;
			else
				Client.WASD = true;
		}


		if (mx >= 240 + BFont_TextWidth("Own Name -") && mx <= 255 + BFont_TextWidth("Own Name -") && my >= 295 && my <= 310) // Own Name Toggle
		{
			if (Client.OwnName == true)
				Client.OwnName = false;
			else
				Client.OwnName = true;
		}
		if (mx >= 240 + BFont_TextWidth("Player Names -") && mx <= 255 + BFont_TextWidth("Player Names -") && my >= 325 && my <= 340) // Player Names Toggle
		{
			if (Client.PlNames == true)
				Client.PlNames = false;
			else
				Client.PlNames = true;
		}


		if (mx >= 490 + BFont_TextWidth("Amount Per -") && mx <= 505 + BFont_TextWidth("Amount Per -") && my >= 295 && my <= 310) // Craft Per Toggle
		{
			if (Client.CraftPer == 0)
				Client.CraftPer = 1;
			else if (Client.CraftPer == 1)
				Client.CraftPer = 2;
			else if (Client.CraftPer == 2)
				Client.CraftPer = 3;
			else if (Client.CraftPer == 3)
				Client.CraftPer = 4;
			else if (Client.CraftPer == 4)
				Client.CraftPer = 0;
		}

		return;
	}

	if (!Use && UseTarget != -1) UseTarget = -1;

	if (Use && UseTarget != -1 && UseType != 10)
	{
		if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245)
		{
			short tmx, tmy;

			tmx = mx - 186;
			tmy = my - 186;

			short tx, ty;

			tx = tmx / 20 - 1;
			ty = tmy / 20 - 1;

			tmx = pldatabase->own->x + tx;
			tmy = pldatabase->own->y + ty;

			if (iBackpack[UseTarget].Type == TYPE_USEABLEITEM)
			{
				if ((Map.Chunks[0][Map.ChunkMap[tmx >> 4][tmy >> 4]][tmx % 16][tmy % 16] == 72 ||
					Map.Chunks[0][Map.ChunkMap[tmx >> 4][tmy >> 4]][tmx % 16][tmy % 16] == 114 ||
					Map.Chunks[0][Map.ChunkMap[tmx >> 4][tmy >> 4]][tmx % 16][tmy % 16] == 150 ||
					Map.Chunks[0][Map.ChunkMap[tmx >> 4][tmy >> 4]][tmx % 16][tmy % 16] == 457)
					&&
					Useables[iBackpack[UseTarget].ID].use_type == use_MINE)
				{

					PACKET_MINEAT MineAt;
					memset(&MineAt, 0, sizeof(MineAt));

					MineAt._x = tmx;
					MineAt._y = tmy;
					MineAt.WithItem = iBackpack[UseTarget].KnowID;

					buffer[0] = 55;
					memcpy(&buffer[1], &MineAt, sizeof(MineAt));
					send(Socket, buffer, 1 + sizeof(MineAt), 0);
				}
			}
			if (iBackpack[UseTarget].Type == TYPE_COLLECTABLE)
			{
				if (Collectables[iBackpack[UseTarget].ID].use_type == use_SMELT)
				{
					obj_List = Objects[tmx / 32][tmy / 32];

					while (obj_List != NULL)
					{
						if (obj_List->X == tmx && obj_List->Y == tmy && ObjectTypes[obj_List->Type].UseType == use_SMELT)
						{
							PACKET_SMELTAT SmeltAt;

							SmeltAt.WithItem = UseTarget;
							SmeltAt.x = tmx;
							SmeltAt.y = tmy;
							SmeltAt.am = Client.CraftPer;
							SmeltAt.id = iBackpack[UseTarget].ID;

							buffer[0] = 56;
							memcpy(&buffer[1], &SmeltAt, sizeof(SmeltAt));
							send(Socket, buffer, 1 + sizeof(SmeltAt), 0);
						}

						if (obj_List != NULL)
							obj_List = obj_List->next;
					}
				}
			}
		}

		if (iBackpack[UseTarget].Type == TYPE_USEABLEITEM)
		{
			if (mx <= Backpack.X + Backpack_w && mx >= Backpack.X && my <= Backpack.Y + Backpack_h && my >= Backpack.Y && Backpack.On
				&& Useables[iBackpack[UseTarget].ID].use_type == use_FORGE)
			{
				bool Error = false;

				mx -= Backpack.X; mx -= 3;
				my -= Backpack.Y; my -= 17;
				if (my < 0) my = 0;
				if (mx < 0) mx = 0;


				Uint32 Pixel;

				Uint8 r, g, b;

				short x1, y1;

				int i;
				for (i = MaxBackpackItems - 1; i >= 0; i--)
				{
					if (iBackpack[i].Used)
					{
						if (mx - iBackpack[i].x >= 0 &&
							mx - iBackpack[i].x < 20 &&
							my - iBackpack[i].y >= 0 &&
							my - iBackpack[i].y < 20)
						{

							if (iBackpack[i].Family == FAMILY_WEAPON)
								x1 = mx - iBackpack[i].x + Weapons[iBackpack[i].Type][iBackpack[i].ID].Frames[Frames[Weapons[iBackpack[i].Type][iBackpack[i].ID].anim_Type][Weapons[iBackpack[i].Type][iBackpack[i].ID].num_Frames]].x;
							if (iBackpack[i].Family == FAMILY_ARMOR)
								x1 = mx - iBackpack[i].x + Armors[iBackpack[i].Type - 11][iBackpack[i].ID].Frames[Frames[Armors[iBackpack[i].Type - 11][iBackpack[i].ID].anim_Type][Armors[iBackpack[i].Type - 11][iBackpack[i].ID].num_Frames]].x;
							if (iBackpack[i].Family == FAMILY_COLLECTABLE)
								x1 = mx - iBackpack[i].x + Collectables[iBackpack[i].ID].Frames[Frames[Collectables[iBackpack[i].ID].anim_Type][Collectables[iBackpack[i].ID].num_Frames]].x;
							if (iBackpack[i].Family == FAMILY_USEABLE)
								x1 = mx - iBackpack[i].x + Useables[iBackpack[i].ID].Frames[Frames[Useables[iBackpack[i].ID].anim_Type][Useables[iBackpack[i].ID].num_Frames]].x;

							if (iBackpack[i].Family == FAMILY_WEAPON)
								y1 = my - iBackpack[i].y + Weapons[iBackpack[i].Type][iBackpack[i].ID].Frames[Frames[Weapons[iBackpack[i].Type][iBackpack[i].ID].anim_Type][Weapons[iBackpack[i].Type][iBackpack[i].ID].num_Frames]].y;
							if (iBackpack[i].Family == FAMILY_ARMOR)
								y1 = my - iBackpack[i].y + Armors[iBackpack[i].Type - 11][iBackpack[i].ID].Frames[Frames[Armors[iBackpack[i].Type - 11][iBackpack[i].ID].anim_Type][Armors[iBackpack[i].Type - 11][iBackpack[i].ID].num_Frames]].y;
							if (iBackpack[i].Family == FAMILY_COLLECTABLE)
								y1 = my - iBackpack[i].y + Collectables[iBackpack[i].ID].Frames[Frames[Collectables[iBackpack[i].ID].anim_Type][Collectables[iBackpack[i].ID].num_Frames]].y;
							if (iBackpack[i].Family == FAMILY_USEABLE)
								y1 = my - iBackpack[i].y + Useables[iBackpack[i].ID].Frames[Frames[Useables[iBackpack[i].ID].anim_Type][Useables[iBackpack[i].ID].num_Frames]].y;

							Pixel = GetPixel(G.items, x1, y1);

							SDL_GetRGB(Pixel, G.items->format, &r, &g, &b);


							if (r != 255 || g != 0 || b != 255)
							{
								if (iBackpack[i].Type == TYPE_COLLECTABLE)
								{
									if (Collectables[iBackpack[i].ID].use_type == use_FORGE)
									{
										BS->init(&iBackpack[i]);
										i = 0;
									}
								}
							}
						}
					}
				}
			}
		}

		Use = false;
		UseTarget = -1;
		return;
	}

	if (Use && UseType == 10)
	{
		if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245)
		{
			short tmx, tmy;

			tmx = mx - 186;
			tmy = my - 186;

			short tx, ty;

			tx = tmx / 20;
			ty = tmy / 20;

			tmx = pldatabase->own->x - 1 + tx;
			tmy = pldatabase->own->y - 1 + ty;

			PlayerNode* lst;
			lst = pldatabase->getlast();
			if (lst == NULL) return;

			fprintf(stderr, "test3, tmx = %d, tmy = %d\n", tmx, tmy);


			while (lst)
			{
				if (lst->x == tmx && lst->y == tmy && lst != pldatabase->own)
				{
					st->informserver_offertrade(lst->id);

					sprintf(MB.NewMsg, "You offer %s a trade.", lst->name);
					MB.NewType = 2;
					MB.Update();

					lst = NULL;
				}

				if (lst != NULL) lst = lst->prev;
			}
		}
	}

	if (Use)
	{
		Use = false;
		UseTarget = -1;
		UseType = 0;
	}


	// 1)

	if (Dragging.On)
	{
		Dragging.Release();
		Dragging.Stop();
		return;
	}

	// 2)

	if (mx >= 439 && mx <= 785 && my >= 240 && my <= 246)
	{
		long Temp;
		Temp = ExpTables[P.Level + 1] - P.Exp;
		if (Temp > 0)
			sprintf(MB.NewMsg, "Ä%luÅ experience required to level Ä%dÅ. [ Ä%luÅ / Ä%luÅ ].", ExpTables[P.Level + 1] - P.Exp, P.Level + 1, P.Exp, ExpTables[P.Level + 1]);
		else
			sprintf(MB.NewMsg, "Ä0Å experience required to level Ä%dÅ. [ Ä%luÅ / Ä%luÅ ].", P.Level + 1, P.Exp, ExpTables[P.Level + 1]);

		MB.NewType = 7;
		MB.Update();

		return;
	}

	if (mx >= 439 && mx <= 785 && my >= 232 && my <= 238)
	{
		long Temp;
		Temp = AlignmentTables[P.Reputation + 1] - P.KilledMonsters;
		if (P.Reputation >= 8)
			sprintf(MB.NewMsg, "Your reputation: Ä%sÅ. You have commited Ä%dÅ crimes.", align_names[P.Reputation], P.CrimCount);
		else if (Temp > 0)
			sprintf(MB.NewMsg, "Your reputation: Ä%sÅ. Ä%luÅ good deeds required for better reputation. You have commited Ä%dÅ crimes.", align_names[P.Reputation], Temp, P.CrimCount);
		else
			sprintf(MB.NewMsg, "Your reputation: Ä%sÅ. Ä0Å good deeds required for better reputation. You have commited Ä%dÅ crimes.", align_names[P.Reputation], P.CrimCount);

		MB.NewType = 7;
		MB.Update();

		return;
	}

	// Let's check if player is raising stat.

//	if (mx >=  && mx <=  && my >=  && my <=  )
//	{



//	}

	if (mx >= 793 && my <= 7)
	{
		if (ExitClick && SDL_GetTicks() - ExitClickTime < 4000)
		{
			buffer[0] = 120;
			send(Socket, buffer, 1, 0);
			CleanUp();
		}
		else
		{
			ExitClick = true;
			ExitClickTime = SDL_GetTicks();

			sprintf(MB.NewMsg, "Are you sure you want to quit?");
			MB.NewType = 7;
			MB.Update();
		}
	}

	// 3)

	if (!TargetError && FirstBuyClick) FirstBuyClick = false;

	if (!TargetError)
	{
		short ox, oy, tx, ty;
		ox = pldatabase->own->x - 10 + (MX - 7) / 20;
		oy = pldatabase->own->y - 10 + (MY - 7) / 20;

		int i;
		for (i = MaxItems - 1; i >= 0; i--)
		{
			if (iGround[i].Used)
			{
				tx = iGround[i].x;
				ty = iGround[i].y;

				if (ox == tx && oy == ty)
				{
					IdentifyItem(&iGround[i]);

					Target2Error = true;
					TargetError = true;

					i = -1;
				}
			}
		}
	}

	if (!TargetError)
	{
		short ox, oy, tx, ty;
		ox = pldatabase->own->x - 10 + (MX - 7) / 20;
		oy = pldatabase->own->y - 10 + (MY - 7) / 20;

		obj_List = Objects[ox / 32][oy / 32];

		while (obj_List != NULL)
		{
			if (obj_List->X == ox && obj_List->Y == oy && Identify && MapInfo.Visible[-pldatabase->own->x + obj_List->X + 10][-pldatabase->own->y + obj_List->Y + 10] > 3)
			{
				sprintf(MB.NewMsg, "You see %s.", ObjectTypes[obj_List->Type].Name);
				MB.NewType = 7;
				MB.Update();


				Target2Error = true;
				TargetError = true;

				obj_List = NULL;
			}

			if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		BodyClass* body;
		body = bodydatabase->body;

		while (body)
		{
			tx = body->x;
			ty = body->y;

			if (ox == tx && oy == ty && Identify && MapInfo.Visible[-pldatabase->own->x + tx + 10][-pldatabase->own->y + ty + 10] > 3)
			{
				if (body->decaystate == 0)
				{
					sprintf(MB.NewMsg, "You see body of %s.", body->ownername);
					MB.NewType = 7;
					MB.Update();
				}
				else
				{
					sprintf(MB.NewMsg, "You see decaying body of %s.", body->ownername);
					MB.NewType = 7;
					MB.Update();
				}
				if (body->looted)
				{
					sprintf(MB.NewMsg, "You notice that this body has been looted by %s.", body->lootername);
					MB.NewType = 6;
					MB.Update();
				}
				Target2Error = true;
				TargetError = true;
				body = NULL;
			}

			if (body != NULL) body = body->next;
		}
	}

	// 4)
	// ChattingMode buttons.

	if (Writing.Writing && Writing.Mode == 1)
	{
		if (my >= 410 && my <= 421)
		{
			if (Client.SayOn && mx >= 11 && mx <= 22) // Change to normal
			{
				Writing.ChattingMode = 1;
			}
			if (Client.WhisOn && mx >= 24 && mx <= 35) // Change to whisper
			{
				Writing.ChattingMode = 2;
			}
			if (Client.GlobOn && mx >= 37 && mx <= 48) // Change to global
			{
				Writing.ChattingMode = 3;
			}
			if (mx >= 54 && mx <= 65 && P.Guild > 0) // Change to guild
			{
				Writing.ChattingMode = 4;
			}
			if (mx >= 67 && mx <= 78 && GMMode > 2) // Change to GM chat
			{
				Writing.ChattingMode = 5;
			}
			if (mx >= 80 && mx <= 91 && GMMode > 0) // Change to welcoming commitee button
			{
				Writing.ChattingMode = 6;
			}
		}
	}

	// 5)	
	// Chat scroll buttons			

	if (mx >= 10 && mx <= 22 && my >= 425 && my <= 455) // Go to older chat
	{
		int num;
		bool canAdd = true;
		for (int i = 0; i < 2000; i++)
		{
			num = i + MB.scroll;
			if (MB.MsgType[MB.scroll + 13] == NULL)
			{
				canAdd = false;
				i = 2000;
			}
		}

		if (canAdd)
		{
			MB.scroll++;
			MB.Print();
		}
	}
	if (mx >= 10 && mx <= 22 && my >= 570 && my <= 600) // Go to newer chat
	{
		MB.scroll--;
		if (MB.scroll <= 0)
			MB.scroll = 0;
		MB.Print();
	}
}


//==============================================================================
//
//	Open players body
//
//==============================================================================
void OpenOwnBody()
{
	BodyClass *body;
	body = bodydatabase->getlast();
	if (body == NULL) return;

	while (body)
	{
		if (its(pldatabase->own->x, body->x) <= 1 && its(pldatabase->own->y, body->y) <= 1 && body->realid == pldatabase->own->id)
		{
			buffer[0] = 27;
			buffer[1] = 1;				
			memcpy(&buffer[2], &body->id, sizeof(body->id));
			send(Socket, buffer, 2 + sizeof(body->id), 0);

			v_Body.On = true;
			v_Body_Open = body->id;
			return;
		}
	
		if (body != NULL) body = body->prev;
	}
}



//==============================================================================
//
//	Remove items from memory
//
//==============================================================================
void Item_Remove(short This, int FromList)
{	
	switch (FromList)
	{
	case ItemBackpack:

		memset(&iBackpack[This], 0, sizeof(iBackpack[This]));

		break;
	case ItemBank:
        
		memset(&iBank[This], 0, sizeof(iBank[This]));

		break;
	case ItemGround:
  
		memset(&iGround[This], 0, sizeof(iGround[This]));

		break;
	case ItemWeared:

		memset(&iWeared[This], 0, sizeof(iWeared[This]));

		break;
	}

}


//==============================================================================
//
//	Add items to memory
//
//==============================================================================
void Item_Add(ItemClass This, int TargetList)
{	
	int i;
	switch (TargetList)
	{
	case ItemBackpack:

		for (i = 0; i < MaxBackpackItems; i++)
		{
			if (!iBackpack[i].Used)
			{
				memcpy(&iBackpack[i], &This, sizeof(This));
				iBackpack[i].Used = true;
				i = MaxBackpackItems;
			}
		}
		
		break;
	case ItemBank:

		for (i = 0; i < MaxBankItems; i++)
		{
			if (!iBank[i].Used)
			{
				memcpy(&iBank[i], &This, sizeof(This));
				iBank[i].Used = true;
				i = MaxBankItems;
			}
		}

		break;
	case ItemWeared:

		for (i = 0; i < MaxWearedItems; i++)
		{
			if (!iWeared[i].Used)
			{
				memcpy(&iWeared[i], &This, sizeof(This));
				iWeared[i].Used = true;
				i = MaxWearedItems;
			}
		}

		break;
	case ItemGround:

		for (i = 0; i < MaxItems; i++)
		{
			if (!iGround[i].Used)
			{
				memcpy(&iGround[i], &This, sizeof(This));
				iGround[i].Used = true;
				i = MaxItems;
			}
		}

		break;

	default:
		printf("Error at Item_Add() -> Target list was not valid.\n");
		break;
	}
}


//==============================================================================
//
//	Check things on Keyboard button UP
//
//==============================================================================
void WritingClass::KeyboardButtonUp(int key, int unic)
{
	int i;

	if (key == SDLK_RETURN && Mode == 2)
	{
		if (atoi(Dragging.Txt) >= 1 && Dragging.TargetItem->Quantity >= 2)// && !shop.pending)
		{

			unsigned long Quantity;
			Quantity = atoi(Dragging.Txt);

			if (Quantity == Dragging.TargetItem->Quantity)
			{
				Dragging.Split = false;
				Dragging.Stop();
				Writing = false;
				Stop();
				return;
			}

			memset(&ItemMove, 0, sizeof(ItemMove));

			ItemMove.Amount = Quantity;
			ItemMove.Case = 14;
			ItemMove.ItemID = Dragging.ItemID;
			ItemMove.ToList = Dragging.ToList;
			ItemMove.FromList = Dragging.FromList;
			if (Dragging.ToList == ItemGround)
			{			
				ItemMove.x = Dragging.spx;
				ItemMove.y = Dragging.spy;
			}
			else
			{
				ItemMove.x = Dragging.spx > 100 ? rand() % 100 : Dragging.spx;
				ItemMove.y = Dragging.spy > 100 ? rand() % 100 : Dragging.spy;
			}
			
			buffer[0] = 20;
			memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
			send (Socket, buffer, 1 + sizeof(ItemMove), 0);

			Dragging.Split = false;
			Dragging.Stop();
			Writing = false;
			Stop();
			return;
		}
		Stop();
		return;
	}

	if (key == SDLK_RETURN && (shop.pending && shop.pending_mode == shopmode_pendingamount))
	{
		if (atoi(shop.txt) >= 1)
		{
			bool good = true;
			short count = 0;

			shop_itemlist *tmp2;
			tmp2 = shop.i_items;

			while (tmp2)
			{
				count++;
				tmp2 = tmp2->next;
			}

			if (count >= 8) 
			{
				shop.pending = false;
				shop.pending_mode = shopmode_nopending;
				shop.getcost();
				Stop();
				return;
			}

			tmp2 = new shop_itemlist;
			tmp2->idinlist = shop.idinlist;
			tmp2->quantity = atoi(shop.txt);

			if (shop.sellmode == shopmode_sell)	
			{				
				tmp2->item = &shop.bitems[tmp2->idinlist];
				if (tmp2->quantity > iBackpack[tmp2->idinlist].Quantity)
					tmp2->quantity = iBackpack[tmp2->idinlist].Quantity;
			}
			else
				tmp2->item = &shop.items[tmp2->idinlist];

			tmp2->addto();
		}
		shop.pending = false;
		shop.pending_mode = shopmode_nopending;
		shop.getcost();
		Stop();
	}

	if (key == SDLK_ESCAPE && (shop.pending && shop.pending_mode == shopmode_pendingamount))
	{
		shop.pending = false;
		shop.pending_mode = shopmode_nopending;
		Stop();
	}

	if (key == SDLK_RETURN && (Mode == 40 || Mode == 41))
	{
		if (KeyHandler < 10 && Mode == 40) 
		{
			sprintf(MB.NewMsg, "Guild name must be at least 10 letters long!");
			MB.NewType = 7;
			MB.Update();
	
			Writing = false;
			Mode = 0;
			WritingError = false;
			KeyHandler = 0;
			memset(&Txt, 0, sizeof(Txt));
			return;
		}
		if (KeyHandler < 2 && Mode == 41)
		{
			sprintf(MB.NewMsg, "Guild tag must be at least 2 letters long!");
			MB.NewType = 7;
			MB.Update();
	
			Writing = false;
			Mode = 0;
			WritingError = false;
			KeyHandler = 0;
			memset(&Txt, 0, sizeof(Txt));
			return;
		}

		if (KeyHandler > 3 && Mode == 41)
		{
			Writing = false;
			Mode = 0;
			WritingError = false;
			KeyHandler = 0;
			memset(&Txt, 0, sizeof(Txt));
			return;
		}

		if (Mode == 40)
		{
			PACKET_NEWGUILDNAME NewName;
			memset(&NewName, 0, sizeof(NewName));

			sprintf(NewName.Name, Clan->Txt);

            buffer[0] = 50;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewName, sizeof(NewName));
			send(Socket, buffer, 2 + sizeof(NewName), 0);
		}
		if (Mode == 41)
		{
			PACKET_NEWGUILDTAG NewTag;
			memset(&NewTag, 0, sizeof(NewTag));

			sprintf(NewTag.Tag, Clan->Txt);

            buffer[0] = 50;
			buffer[1] = 2;
			memcpy(&buffer[2], &NewTag, sizeof(NewTag));
			send(Socket, buffer, 2 + sizeof(NewTag), 0);		
		}
	
		Writing = false;
		Mode = 0;
		WritingError = false;
		KeyHandler = 0;
		memset(&Txt, 0, sizeof(Txt));
		return;
	}

	if (key == SDLK_RETURN && Mode == 1)
	{
		bool Error=false;

		if (Txt[0] == '/')
		{
			char command[15];
			memset(&command, 0, sizeof(command));
			for (i = 0; i < 15; i++)
			{
				if (Txt[i+1] == NULL || Txt[i+1] == 32) i = 15;
				else
				command[i] = Txt[i+1];
			}

			if (!strcmp(command, "t"))
			{
				char name[25];
				memset(&name, 0, sizeof(name));
				short pointer = 1;
				for (i = 0; i < 45; i++)
				{
					if (Txt[pointer+2] == ':')
					{
						i = 45;
						pointer++;
					}
					else name[i] = Txt[pointer+2];
					pointer++;
				}

				char msg[90];
				memset(&msg, 0, sizeof(msg));

				for (i = 0; i < 90; i++)
				{
					if (Txt[pointer+2] == 0)
					{
						i = 90;
					}
					else msg[i] = Txt[pointer+2];
					pointer++;			
				}

				PACKET_WHISPERTO whisperto;

				strcpy(whisperto.name, name);
				strcpy(whisperto.msg, msg);

				buffer[0] = 60;
				buffer[1] = 50;
				memcpy(&buffer[2], &whisperto, sizeof(whisperto));
				send(Socket, buffer, 2 + sizeof(whisperto), 0);
			}
		}

		if (Txt[0] == '&' && GMMode > 1)
		{
			char tag[5];
			memset(&tag, 0, sizeof(tag));
			short pointer = 1;
			for (i = 0; i < 5; i++)
			{
				if (Txt[pointer] == ':')
				{
					i = 5;
					pointer++;
				}
				else tag[i] = Txt[pointer];
				pointer++;
			}

			char msg[90];
			memset(&msg, 0, sizeof(msg));

			for (i = 0; i < 90; i++)
			{
				if (Txt[pointer] == 0)
				{
					i = 90;
				}
				else
				{
					msg[i] = Txt[pointer];
				}
				pointer++;			
			}

			PACKET_GUILDMSGTO guildmsgto;

			strcpy(guildmsgto.msg, msg);
			strcpy(guildmsgto.tag, tag);

			buffer[0] = 60;
			buffer[1] = 38;
			memcpy(&buffer[2], &guildmsgto, sizeof(guildmsgto));
			send(Socket, buffer, 2 + sizeof(guildmsgto), 0);

			Error = true;
			Stop();
		}


		// Scan for ] commands, if first letter of the Text is ]
		if (Txt[0] == ']')
		{
			//ParseCommand();

			char command[15];
			memset(&command, 0, sizeof(command));
			for (i = 0; i < 15; i++)
			{
				if (Txt[i+1] == NULL || Txt[i+1] == 32) i = 15;
				else
				command[i] = Txt[i+1];
			}

//			if (!strcmp(command, "NEWEMAIL")
//				ParseNewEmail();
			
//			if (!strcmp(command, "NEWPASSWORD")
//				ParseNewPass();

			if (!strcmp(command, "h") && GMMode > 1)
			{
				buffer[0] = 60;
				buffer[1] = 2;
				send(Socket, buffer, 2, 0);

				Error = true;
			}

			if (!strcmp(command, "t") && GMMode > 1)
			{
				char name[25];
				memset(&name, 0, sizeof(name));
				short pointer = 1;
				for (i = 0; i < 45; i++)
				{
					if (Txt[pointer+2] == ':')
					{
						i = 45;
						pointer++;
					}
					else name[i] = Txt[pointer+2];
					pointer++;
				}

				char msg[90];
				memset(&msg, 0, sizeof(msg));

				for (i = 0; i < 90; i++)
				{
					if (Txt[pointer+2] == 0)
					{
						i = 90;
					}
					else msg[i] = Txt[pointer+2];
					pointer++;			
				}

				PACKET_WHISPERTO whisperto;

				strcpy(whisperto.name, name);
				strcpy(whisperto.msg, msg);

				buffer[0] = 60;
				buffer[1] = 35;
				memcpy(&buffer[2], &whisperto, sizeof(whisperto));
				send(Socket, buffer, 2 + sizeof(whisperto), 0);

				Error = true;
				Stop();
			}

			if (!strcmp(command, "ln") && GMMode > 1)
			{
				buffer[0] = 60;
				buffer[1] = 4;
				send(Socket, buffer, 2, 0);

				Error = true;
			}
			if (!strcmp(command, "lg") && GMMode > 1)
			{
				buffer[0] = 60;
				buffer[1] = 49;
				send(Socket, buffer, 2, 0);

				Error = true;
			}

			if (!strcmp(command, "lw") && GMMode > 1)
			{
				buffer[0] = 60;
				buffer[1] = 5;
				send(Socket, buffer, 2, 0);

				Error = true;
			}

			if (!strcmp(command, "reset0") && GMMode > 4)
			{
				char command2[65];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 65; i++)
				{
					if (Txt[i+6] == NULL) i = 65;
					else
					command2[i] = Txt[i+6];
				}

				fprintf(stderr, "Command2 = %s", command2);

				PACKET_RESETSTATS ResetStats;
				memset(&ResetStats, 0, sizeof(ResetStats));

				sprintf(ResetStats.Name, "%s", command2);

				ResetStats.Slot = 0;

				buffer[0] = 60;
				buffer[1] = 99;
				memcpy(&buffer[2], &ResetStats, sizeof(ResetStats));
				send(Socket, buffer, 2 + sizeof(ResetStats), 0);

				Error = true;
			}

			if (!strcmp(command, "reset1") && GMMode > 4)
			{
				char command2[25];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 25; i++)
				{
					if (Txt[i+6] == NULL) i = 25;
					else
					command2[i] = Txt[i+6];
				}

				fprintf(stderr, "Command2 = %s", command2);

				PACKET_RESETSTATS ResetStats;
				memset(&ResetStats, 0, sizeof(ResetStats));

				sprintf(ResetStats.Name, "%s", command2);

				ResetStats.Slot = 1;

				buffer[0] = 60;
				buffer[1] = 99;
				memcpy(&buffer[2], &ResetStats, sizeof(ResetStats));
				send(Socket, buffer, 2 + sizeof(ResetStats), 0);

				Error = true;
			}


			if (!strcmp(command, "ci") && GMMode > 2)
			{
				unsigned long am=0;
				short id=0, type=0;
				int sta;
				int i1 = 0;
				sta = 0;
				char command2[25];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 25; i++)
				{
					if (Txt[i+3] == SDLK_COMMA) 
					{
						if (sta == 0)
						type = atoi(command2);
						if (sta == 1)
						id = atoi(command2);

						memset(&command2, 0, sizeof(command2));
						i1 = i + 1;
						sta++;
					}
					else
					command2[i - i1] = Txt[i+3];
					if (Txt[i+3] == NULL) i = 25;
				}

				am = atoi(command2);

				buffer[0] = 60;
				buffer[1] = 25;
				memcpy(&buffer[2], &id, sizeof(id));
				memcpy(&buffer[2 + sizeof(id)], &type, sizeof(type));
				memcpy(&buffer[2 + sizeof(id) + sizeof(type)], &am, sizeof(am));
                send(Socket, buffer, 2 + sizeof(id) + sizeof(type) + sizeof(am), 0);

				Error = true;
			}

			if (!strcmp(command, "w") && GMMode > 1)
			{
				short CX, CY;
				CX = 100; CY = 100;
				int i1;
				i1 = 0;
				char command2[25];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 25; i++)
				{
					if (Txt[i+2] == SDLK_COMMA) 
					{
						CX = atoi(command2);
						memset(&command2, 0, sizeof(command2));
						i1 = i + 1;
					}
					else
					command2[i - i1] = Txt[i+2];
					if (Txt[i+2] == NULL) i = 25;
				}
				CY = atoi(command2);

				PACKET_TELEPORTSELF Teleport;

				Teleport.X = CX;
				Teleport.Y = CY;

				buffer[0] = 60;
				buffer[1] = 10;
				memcpy(&buffer[2], &Teleport, sizeof(Teleport));
				send(Socket, buffer, 2 + sizeof(Teleport), 0);			

				Error = true;
			}

			if (!strcmp(command, "name") && GMMode > 3)
			{
				char name1[25];
				char name2[25];
				int sta;
				int i1 = 0;
				sta = 0;
				char command2[65];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 65; i++)
				{
					if (Txt[i+6] == SDLK_COMMA) 
					{
						if (sta == 0)
						strcpy(name1, command2);
						if (sta == 1)
						strcpy(name2, command2);

						memset(&command2, 0, sizeof(command2));
						i1 = i + 1;
						sta++;
					}
					else
					command2[i - i1] = Txt[i+6];
					if (Txt[i+6] == NULL) i = 65;
				}
					strcpy(name2, command2);

				PACKET_NAMECHANGE nName;
				memset(&nName, 0, sizeof(nName));

				strcpy(nName.name1, name1);
				strcpy(nName.name2, name2);

				buffer[0] = 60;
				buffer[1] = 41;
				memcpy(&buffer[2], &nName, sizeof(nName));
				send(Socket, buffer, 2 + sizeof(nName), 0);

				Error = true;
			}

			if (!strcmp(command, "all") && GMMode > 2) // Server messages
			{
				ChattingMode = 10;
				Error = true;
			}

			if (!strcmp(command, "mouse"))
			{
				Client.ShowMouseXY = !Client.ShowMouseXY;
				Error = true;
			}
			if (!strcmp(command, "remove_fog") && GMMode > 4)
			{
				memset(&MapInfo.MapVisibility, 1, sizeof(MapInfo.MapVisibility));
				SaveFogOfWar();
				Error = true;
			}
			if (!strcmp(command, "add_fog") && GMMode > 4)
			{
				memset(&MapInfo.MapVisibility, 0, sizeof(MapInfo.MapVisibility));
				SaveFogOfWar();
				Error = true;
			}
			if (!strcmp(command, "forcesave") && GMMode > 4)
			{
				buffer[0] = 60;
				buffer[1] = 90;
				send(Socket, buffer, 2, 0);
				Error = true;
			}

			if (!strcmp(command, "clearmap") && GMMode > 3)
			{
				char name[25];
				memset(&name, 0, sizeof(name));
				char command2[45];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 45; i++)
				{
					if (Txt[i+10] == NULL) i = 45;
					else
					command2[i] = Txt[i+10];
				}

				PACKET_CLEARMAP clearMap;

				strcpy(clearMap.Name, command2);

				buffer[0] = 60;
				buffer[1] = 45;
				memcpy(&buffer[2], &clearMap, sizeof(clearMap));
				send(Socket, buffer, 2 + sizeof(clearMap), 0);

				Error = true;
			}

			if (!strcmp(command, "resetmap") && GMMode > 3)
			{
				char name[25];
				memset(&name, 0, sizeof(name));
				char command2[45];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 45; i++)
				{
					if (Txt[i+9] == NULL) i = 45;
					else
					command2[i] = Txt[i+9];
				}

				PACKET_REESETMAP resetMap;

				strcpy(resetMap.Name, command2);

				buffer[0] = 60;
				buffer[1] = 45;
				memcpy(&buffer[2], &resetMap, sizeof(resetMap));
				send(Socket, buffer, 2 + sizeof(resetMap), 0);

				Error = true;
			}

			if (!strcmp(command, "rank") && GMMode > 5)
			{
				char name[25];
				short rank;
				int sta;
				int i1 = 0;
				sta = 0;
				char command2[65];
				memset(&command2, 0, sizeof(command2));
				for (i = 0; i < 65; i++)
				{
					if (Txt[i+6] == SDLK_COMMA) 
					{
						if (sta == 0)
						strcpy(name, command2);
						if (sta == 1)
						rank = atoi(command2);

						memset(&command2, 0, sizeof(command2));
						i1 = i + 1;
						sta++;
					}
					else
					command2[i - i1] = Txt[i+6];
					if (Txt[i+6] == NULL) i = 65;
				}
					rank = atoi(command2);


				Info->newInfoMsg(name);
				Info->newInfoMsg(command2);

				PACKET_CHANGERANK ChangeRank;

				strcpy(ChangeRank.Name , name);
				ChangeRank.Rank = rank;

				buffer[0] = 60;
				buffer[1] = 23;
				memcpy(&buffer[2], &ChangeRank, sizeof(ChangeRank));
				send(Socket, buffer, 2 + sizeof(ChangeRank), 0);			

				Error = true;
			}


			if (!strcmp(command, "help") && GMMode > 1)
			{
				Info->newInfoMsg(" ");
				Info->newInfoMsg("- GM Commands -");
				Info->newInfoMsg("]help");
				Info->newInfoMsg("]h");
				Info->newInfoMsg("]t");
				Info->newInfoMsg("]ln");
				Info->newInfoMsg("]lw");
				Info->newInfoMsg("]lg");
				Info->newInfoMsg("]los");
				Info->newInfoMsg("]w x,y");
				
				if (GMMode > 2)
				{
					Info->newInfoMsg(" ");
					Info->newInfoMsg("]all");
					Info->newInfoMsg("]ci Type,ID,Am");
				}

				if (GMMode > 3)
				{
					Info->newInfoMsg(" ");
					Info->newInfoMsg("]name playername,newname");						
					Info->newInfoMsg("]clearmap playername");
					Info->newInfoMsg("]resetmap playername");
				}
				
				if (GMMode > 4)
				{
					Info->newInfoMsg(" ");
					Info->newInfoMsg("- DEV Commands -");
					Info->newInfoMsg("]reset0 accountname");
					Info->newInfoMsg("]reset1 accountname");
					Info->newInfoMsg("]remove_fog");
					Info->newInfoMsg("]add_fog");
					Info->newInfoMsg("]forcesave");
				}

				if (GMMode > 5)
				{				
					Info->newInfoMsg(" ");
					Info->newInfoMsg("]rank player,rank");
				}

				Error = true;
			}

			if (Error)
				Stop();
		}

        if (Error) return;		


		// Send new chat message to server.

		if (Txt[0] == NULL){ Stop();  return;}


		PACKET_CHATMESSAGE NewPacket;

		memset(&NewPacket, 0, sizeof(NewPacket));

		switch (ChattingMode)
		{
		case 1: sprintf(NewPacket.Message, "ò%s", Txt); break;
		case 2: sprintf(NewPacket.Message, "î%s", Txt); break;
		case 3: sprintf(NewPacket.Message, "ì%s", Txt); break;
		case 4: sprintf(NewPacket.Message, "ï%s", Txt); break;
		default: sprintf(NewPacket.Message, "%s", Txt); break;
		}

		// Check if that chat is off	
		if (ChattingMode == 1)
		{
			if (!Client.SayOn) 
			{		
				Writing = false;
				Mode = 0;
				WritingError = false;
				KeyHandler = 0;
				memset(&Txt, 0, sizeof(Txt));
				return;
			}
		}
		if (ChattingMode == 2)		
		{
			if (!Client.WhisOn)
			{		
				Writing = false;
				Mode = 0;
				WritingError = false;
				KeyHandler = 0;
				memset(&Txt, 0, sizeof(Txt));
				return;
			}
		}
		if (ChattingMode == 3)	
		{
			if (!Client.GlobOn)
			{		
				Writing = false;
				Mode = 0;
				WritingError = false;
				KeyHandler = 0;
				memset(&Txt, 0, sizeof(Txt));
				return;
			}
		}

		NewPacket.MsgType = ChattingMode;

		memcpy(&buffer[1], &NewPacket, sizeof(NewPacket));

		buffer[0] = 9;		

		send(Socket, buffer, 1 + sizeof(NewPacket), 0);        

		Writing = false;
		Mode = 0;
		WritingError = false;
		KeyHandler = 0;
		memset(&Txt, 0, sizeof(Txt));
		return;
	}

/*	if (key == SDLK_BACKSPACE)
	{
		WritingError = false;
		KeyHandler--;

        if (KeyHandler < 0) 
			KeyHandler = 0;

		Txt[KeyHandler] = NULL;

	
        return;
	}
*/
	if (key == SDLK_ESCAPE && Mode == 1)
	{
		Writing = false;
		Mode = 0;
		WritingError = false;
		KeyHandler = 0;
		memset(&Txt, 0, sizeof(Txt));
		return;
	}

	if (key == SDLK_RSHIFT ||
		key == SDLK_LSHIFT)
		return;

	if (key == SDLK_TAB)
	{
		if (Mode == 10) { Mode = 11; return; }
		if (Mode == 11) { Mode = 10; return; }	
	}

	if (shop.pending && shop.pending_mode == shopmode_pendingamount)
	{
		if ( KeyProbhited(key, unic) && !WritingError)
		{
			KeyHandler = strlen(shop.txt);

			shop.txt[KeyHandler] = (char)unic;

//			if ( KeyHandler + 1 >= 50 )
//				WritingError = true;

			if (atol(shop.txt) > 100) 
				sprintf(shop.txt, "100");
			if (atol(shop.txt) <= 0)
				sprintf(shop.txt, "0");
		}
		return;
	}

	switch( Mode )
	{
	//------------------------------------------------------------------------------
	//	Chat [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 1:
		if ( KeyProbhited(key, unic) && !WritingError)
		{
			Txt[KeyHandler] = (char)unic;

			KeyHandler++;

			if ( KeyHandler > 99 )
				WritingError = true;
		}

		break;

	//------------------------------------------------------------------------------
	//  Split (amount) [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 2:
		if ( KeyProbhited(key, unic) && !WritingError)
		{
			Dragging.Txt[KeyHandler] = (char)unic;

			KeyHandler++;

			if ( KeyHandler > 10 )
				WritingError = true;

			if (atoi(Dragging.Txt) > Dragging.TargetItem->Quantity)
			{
				sprintf(Dragging.Txt, "%lu", Dragging.TargetItem->Quantity);
				KeyHandler = strlen(Dragging.Txt);
			}
			if (atoi(Dragging.Txt) <= 0)
			{
				sprintf(Dragging.Txt, "0");
				KeyHandler = 1;
			}
		}

		break;

	//------------------------------------------------------------------------------
	//  Account Name [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 10:
		if ( KeyProbhited(key, unic) && !LoginError[0])
		{
			Login.AccName[LoginHandler[0]] = (char)unic;

			LoginHandler[0]++;

			if ( LoginHandler[0] > 18)
				LoginError[0] = true;
		}
		break;

	//------------------------------------------------------------------------------
	//  Account Password [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 11: 
		if ( KeyProbhited(key, unic) && !LoginError[1])
		{
			Login.AccPass[LoginHandler[1]] = (char)unic;

			LoginHandler[1]++;

			if ( LoginHandler[1] > 18)
				LoginError[1] = true;
		}
		break;

	//------------------------------------------------------------------------------
	//  Character Name [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 12:

		if ( KeyProbhited(key, unic) && !LoginError[2])
		{
			Login.CharName[LoginHandler[2]] = (char)unic;

			LoginHandler[2]++;

			if ( LoginHandler[2] > 18)
				LoginError[2] = true;
		}
		break;

	//------------------------------------------------------------------------------
	//  Choose an amount [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 20: 

		if ( KeyProbhited(key, unic) && !WritingError)
		{
			KeyHandler = strlen(shop.txt);

			shop.txt[KeyHandler+1] = (char)unic;

			if ( KeyHandler + 1 >= 50 )
				WritingError = true;

			if (atol(shop.txt) > 100) 
				sprintf(shop.txt, "100");			
		}
		break;

	//------------------------------------------------------------------------------
	//  Guild name [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 40:

		if ( KeyProbhited(key, unic) && !WritingError)
		{
			Clan->Txt[KeyHandler] = (char)unic;

			KeyHandler++;
		}

		break;

	//------------------------------------------------------------------------------
	//  Guild tag [PROHIBITED CHARACTERS]
	//------------------------------------------------------------------------------
	case 41:

		if ( KeyProbhited(key, unic) && !WritingError)
		{
			Clan->Txt[KeyHandler] = (char)unic;

			KeyHandler++;
		}
		break;
	}
}


//==============================================================================
//
//	Text character limitations
//
//==============================================================================
bool WritingClass::KeyProbhited(int k, int unic)
{
	bool i1 = false;
	if (Mode == 1 || Mode == 10 || Mode == 11 || Mode == 40 || Mode == 41)
/*---------------
	1 = Chat
	10 = AccName
	11 = AccPW
	40 = G.Name
	41 = G.Tag
---------------*/
	{
		if ( k == SDLK_a ||
			 k == SDLK_b ||
			 k == SDLK_c ||
			 k == SDLK_d ||
			 k == SDLK_e ||
			 k == SDLK_f ||
			 k == SDLK_g ||
			 k == SDLK_h ||
			 k == SDLK_i ||
			 k == SDLK_j ||
			 k == SDLK_k ||
			 k == SDLK_l ||
			 k == SDLK_m ||
			 k == SDLK_n ||
			 k == SDLK_o ||
			 k == SDLK_p ||
			 k == SDLK_q ||
			 k == SDLK_r ||
			 k == SDLK_s ||
			 k == SDLK_t ||
			 k == SDLK_u ||
			 k == SDLK_v ||
			 k == SDLK_w ||
			 k == SDLK_x ||
			 k == SDLK_y ||
			 k == SDLK_z ||
			 k == SDLK_SPACE)
			 i1 = true;

		if ( k == SDLK_1 ||
			 k == SDLK_2 ||
			 k == SDLK_3 ||
			 k == SDLK_4 ||
			 k == SDLK_5 ||
			 k == SDLK_6 ||
			 k == SDLK_7 ||
			 k == SDLK_8 ||
			 k == SDLK_9 ||
			 k == SDLK_0)
			 i1 = true;

		if ( unic == SDLK_MINUS ||
			 unic == SDLK_UNDERSCORE ||
             unic == SDLK_PERIOD ||
			 unic == SDLK_COMMA ||
			 unic == SDLK_QUOTE ||
			 unic == SDLK_SLASH ||
			 unic == SDLK_PLUS ||
			 unic == SDLK_QUESTION ||
			 unic == SDLK_COLON ||
			 unic == SDLK_SEMICOLON ||
			 unic == SDLK_EQUALS ||
			 unic == SDLK_LEFTBRACKET ||
			 unic == SDLK_RIGHTBRACKET ||
			 unic == SDLK_QUOTEDBL ||
			 unic == SDLK_LESS ||
			 unic == SDLK_GREATER ||
			 unic == SDLK_BACKSLASH)
			 i1 = true;

	}

	if (Mode == 12)
/*---------------
12 = Char Name
---------------*/
	{
		if ( k == SDLK_a ||
			 k == SDLK_b ||
			 k == SDLK_c ||
			 k == SDLK_d ||
			 k == SDLK_e ||
			 k == SDLK_f ||
			 k == SDLK_g ||
			 k == SDLK_h ||
			 k == SDLK_i ||
			 k == SDLK_j ||
			 k == SDLK_k ||
			 k == SDLK_l ||
			 k == SDLK_m ||
			 k == SDLK_n ||
			 k == SDLK_o ||
			 k == SDLK_p ||
			 k == SDLK_q ||
			 k == SDLK_r ||
			 k == SDLK_s ||
			 k == SDLK_t ||
			 k == SDLK_u ||
			 k == SDLK_v ||
			 k == SDLK_w ||
			 k == SDLK_x ||
			 k == SDLK_y ||
			 k == SDLK_z ||
			 k == SDLK_SPACE)
			 i1 = true;

		if ( k == SDLK_1 ||
			 k == SDLK_2 ||
			 k == SDLK_3 ||
			 k == SDLK_4 ||
			 k == SDLK_5 ||
			 k == SDLK_6 ||
			 k == SDLK_7 ||
			 k == SDLK_8 ||
			 k == SDLK_9 ||
			 k == SDLK_0)
			 i1 = true;

		if ( unic == SDLK_MINUS ||
			 unic == SDLK_UNDERSCORE ||
			 unic == SDLK_QUOTE)
			 i1 = true;
	}

	if (Mode == 2 || (shop.pending && shop.pending_mode == shopmode_pendingamount))
/*---------------------------
2 = Stack Amount; Shop Amount
----------------------------*/
	{
		if ( k == SDLK_1 ||
			 k == SDLK_2 ||
			 k == SDLK_3 ||
			 k == SDLK_4 ||
			 k == SDLK_5 ||
			 k == SDLK_6 ||
			 k == SDLK_7 ||
			 k == SDLK_8 ||
			 k == SDLK_9 ||
			 k == SDLK_0)
			 i1 = true;
	}

	return i1;
}


void WritingClass::BP()
{
//	if (SDL_GetTicks() - LastBP_time > 250)

	WritingError = false;
	memset(&LoginError, 0, sizeof(LoginError));

	if (Mode == 1)
	{
		KeyHandler--;
		
		if (KeyHandler < 0) 
			KeyHandler = 0;

		Txt[KeyHandler] = NULL;	
	}
	if (Mode == 40 || Mode == 41)
	{
		KeyHandler--;
		
		if (KeyHandler < 0) 
			KeyHandler = 0;

		Clan->Txt[KeyHandler] = NULL;	
	}
	if (Mode == 2)
	{
		KeyHandler--;
		
		if (KeyHandler < 0) 
			KeyHandler = 0;

		Dragging.Txt[KeyHandler] = NULL;	

		if (atoi(Dragging.Txt) < 0 || KeyHandler == 0) { sprintf(Dragging.Txt, "0"); KeyHandler = 0; }
	}
	
	if (Mode == 10 || Mode == 11 || Mode == 12)
	{
		LoginHandler[Mode-10]--;

		if (LoginHandler[Mode-10] < 0)
			LoginHandler[Mode-10] = 0;

        if (Mode == 10)
			Login.AccName[LoginHandler[0]] = NULL;
		if (Mode == 11)
			Login.AccPass[LoginHandler[1]] = NULL;
		if (Mode == 12)
			Login.CharName[LoginHandler[2]] = NULL;
	}
	if (shop.pending && shop.pending_mode == shopmode_pendingamount)
	{
		KeyHandler = strlen(shop.txt);
		if (atol(shop.txt) < 0 || KeyHandler == 0) 
		{
			sprintf(shop.txt, "0");
			return;
		}
		
		shop.txt[KeyHandler-1] = NULL;	

	}
	BP_time = SDL_GetTicks();
}	

void WritingClass::Start(int Modetus)
{
	Writing = true;
	Mode = Modetus;
	memset(&Txt, 0, sizeof(Txt));
	
	if (Mode == 2)
	{
		sprintf(Dragging.Txt, "%d", Dragging.TargetItem->Quantity);
		KeyHandler = strlen (Dragging.Txt);
	}
	WritingError = false;
}


void WritingClass::CheckTab()
{
	bool a1 = false;

	if (ChattingMode == 1 && Client.WhisOn)
	{
		ChattingMode = 2;
		a1 = true;
	}	
	if (ChattingMode == 1 && !Client.WhisOn && Client.GlobOn)
	{
		ChattingMode = 3;
		a1 = true;
	}
	if (ChattingMode == 1 && !Client.WhisOn && !Client.GlobOn && !a1 && P.Guild > 0)
	{
		ChattingMode = 4;
		a1 = true;
	}
	if (ChattingMode == 1 && !Client.WhisOn && !Client.GlobOn && !a1 && P.Guild == 0 && GMMode > 1)
	{
		ChattingMode = 5;
		a1 = true;
	}
	if (ChattingMode == 1 && !Client.WhisOn && !Client.GlobOn && !a1 && P.Guild == 0 && GMMode == 1)
	{
		ChattingMode = 6;
		a1 = true;
	}
	if (ChattingMode == 1 && !Client.WhisOn && !Client.GlobOn && !a1 && P.Guild == 0 && GMMode == 0)
	{
		ChattingMode = 1;
		a1 = true;
	}


	if (ChattingMode == 2 && !a1 && Client.GlobOn)
	{
		ChattingMode = 3;
		a1 = true;
	}
	if (ChattingMode == 2 && !Client.GlobOn && !a1 && P.Guild > 0)
	{
		ChattingMode = 4;
		a1 = true;
	}
	if (ChattingMode == 2 && !Client.GlobOn && !a1 && P.Guild == 0 && GMMode > 1)
	{
		ChattingMode = 5;
		a1 = true;
	}
	if (ChattingMode == 2 && !Client.GlobOn && !a1 && P.Guild == 0 && GMMode == 1)
	{
		ChattingMode = 6;
		a1 = true;
	}
	if (ChattingMode == 2 && !Client.GlobOn && !a1 && P.Guild == 0 && !Client.SayOn && GMMode == 0)
	{
		ChattingMode = 2;
		a1 = true;
	}
	if (ChattingMode == 2 && !Client.GlobOn && !a1 && P.Guild == 0 && Client.SayOn && GMMode == 0)
	{
		ChattingMode = 1;
		a1 = true;
	}


	if (ChattingMode == 3 && !a1 && P.Guild > 0)
	{
		ChattingMode = 4;
		a1 = true;
	}
	if (ChattingMode == 3 && !a1 && GMMode == 0 && P.Guild == 0)
	{
		ChattingMode = 1;
		a1 = true;
	}
	if (ChattingMode == 3 && !a1 && GMMode > 1 && P.Guild == 0)
	{
		ChattingMode = 5;
		a1 = true;
	}
	if (ChattingMode == 3 && !a1 && GMMode == 1 && P.Guild == 0)
	{
		ChattingMode = 6;
		a1 = true;
	}

	if (ChattingMode == 4 && !a1 && GMMode == 0 && Client.SayOn)
	{
		ChattingMode = 1;
		a1 = true;
	}
	if (ChattingMode == 4 && !a1 && GMMode == 0 && !Client.SayOn && Client.WhisOn)
	{
		ChattingMode = 2;
		a1 = true;
	}
	if (ChattingMode == 4 && !a1 && GMMode == 0 && !Client.SayOn && !Client.WhisOn && Client.GlobOn)
	{
		ChattingMode = 3;
		a1 = true;
	}
	
	if (ChattingMode == 6 && !a1 && Client.SayOn)
	{
		ChattingMode = 1;
		a1 = true;
	}	
	if (ChattingMode == 6 && !a1 && !Client.SayOn && Client.WhisOn)
	{
		ChattingMode = 2;
		a1 = true;
	}
	if (ChattingMode == 6 && !a1 && !Client.SayOn && !Client.WhisOn && Client.GlobOn)
	{
		ChattingMode = 3;
		a1 = true;
	}
	if (ChattingMode == 6 && !a1 && !Client.SayOn && !Client.WhisOn && !Client.GlobOn)
	{
		ChattingMode = 5;
		a1 = true;
	}

	if (ChattingMode == 4 && !a1 && GMMode > 1)
	{
		ChattingMode = 5;
		a1 = true;
	}
	if (ChattingMode == 5 && !a1 && GMMode > 0)
	{
		ChattingMode = 6;
		a1 = true;
	}				
	if (ChattingMode == 5 && !a1 && Client.SayOn)
	{
		ChattingMode = 1;
		a1 = true;
	}	
	if (ChattingMode == 5 && !a1 && !Client.SayOn && Client.WhisOn)
	{
		ChattingMode = 2;
		a1 = true;
	}
	if (ChattingMode == 5 && !a1 && !Client.SayOn && !Client.WhisOn && Client.GlobOn)
	{
		ChattingMode = 3;
		a1 = true;
	}
	if (ChattingMode == 5 && !a1 && !Client.SayOn && !Client.WhisOn && !Client.GlobOn)
	{
		ChattingMode = 6;
		a1 = true;
	}
}

void ClientClass::KeyboardMove()
{
	bool a1 = false;

	if (shop.on) 
	{
		shop.uninit();
	}
	/*
	if (GMTool->bBackpackView || GMTool->bBankView)
	{
		GMTool->bBackpackView = false;
		GMTool->bBankView = false;
	}
	*/
	
	if (BS->on) BS->on = false;

	if (idatabase->list != NULL) idatabase->clearlist();
	if (Bank.On) Bank.On = false;
	if (Dragging.On) Dragging.Stop();
	if (Dragging.Split) 
	{
		Dragging.Split = false;
		Writing.Stop();
	}
	if (Dragging.TargetItem != NULL) Dragging.TargetItem = NULL;
	if (v_Body.On) v_Body.On = false;
	if (Use) Use = false;

	if (st->on)
	{
		st->tradeabort();
		st->clear();
	}

	if (!BrowseOpt && DOWN && RIGHT && !a1)
	{
		// Move to 3

		buffer[0] = 10;
		buffer[1] = 3;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (!BrowseOpt && DOWN && LEFT && !a1)
	{
		// Move to 1

		buffer[0] = 10;
		buffer[1] = 1;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (!BrowseOpt && LEFT && UP && !a1)
	{
		// Move to 7
		buffer[0] = 10;
		buffer[1] = 7;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (!BrowseOpt && UP && RIGHT && !a1)
	{
		// Move to 9

		buffer[0] = 10;
		buffer[1] = 9;

		send(Socket, buffer, 2, 0);


        a1 = true;	
	}

	if (!a1)
	{
		if (!BrowseOpt && UP && !a1)
		{		
			//Move to 8

			buffer[0] = 10;
			buffer[1] = 8;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (!BrowseOpt && DOWN && !a1)
		{
			//Move to 2

			buffer[0] = 10;
			buffer[1] = 2;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (!BrowseOpt && LEFT && !a1)
		{
			//Move to 4

			buffer[0] = 10;
			buffer[1] = 4;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (!BrowseOpt && RIGHT && !a1)
		{
			//Move to 6

			buffer[0] = 10;
			buffer[1] = 6;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}	
	}
}

void ClientClass::Mousemove()
{
	bool a1 = false;

	Uint16 mx, my;
	mx = MX; 
	my = MY;

	if (mx >= 7 && my >= 7 && mx <= 427 && my <= 427)
		a1 = true;

	if (!a1) return;
	a1 = false;

	if (GMTool->bBackpackView || GMTool->bBankView)
	{
		GMTool->bBackpackView = false;
		GMTool->bBankView = false;
	}

	if (shop.on)
		return;
	if (st->on) return;

	if (idatabase->list != NULL) idatabase->clearlist();
	if (Bank.On) Bank.On = false;
	if (Dragging.On) Dragging.Stop();
	if (Dragging.Split) 
	{
		Dragging.Split = false;
		Writing.Stop();
	}
	if (Dragging.TargetItem != NULL) Dragging.TargetItem = NULL;
	if (v_Body.On) v_Body.On = false;
	if (Use) Use = false;

	int dir = GetDir();

	buffer[0] = 10;
	buffer[1] = dir;

	send(Socket, buffer, 2, 0);


/*	if (mx >= 281 && mx <= 427 && my >= 281 && my <= 427)
	{
		// Move to 3

		buffer[0] = 10;
		buffer[1] = 3;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (mx >= 7 && mx <= 150 && my >= 281 && my <= 427 && !a1)
	{
		// Move to 1

		buffer[0] = 10;
		buffer[1] = 1;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (mx >= 7 && mx <= 150 && my >= 7 && my <= 150 && !a1)
	{
		// Move to 7
		buffer[0] = 10;
		buffer[1] = 7;

		send(Socket, buffer, 2, 0);

        a1 = true;	
	}
	if (mx >= 281 && mx <= 427 && my >= 7 && my <= 150 && !a1)
	{
		// Move to 9

		buffer[0] = 10;
		buffer[1] = 9;

		send(Socket, buffer, 2, 0);


        a1 = true;	
	}

	if (!a1)
	{
		if (mx >= 150 && mx <= 281 && my >= 7 && my <= 150)
		{		
			//Move to 8

			buffer[0] = 10;
			buffer[1] = 8;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (mx >= 150 && mx <= 281 && my >= 281 && my <= 427 && !a1)
		{
			//Move to 2

			buffer[0] = 10;
			buffer[1] = 2;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (mx >= 7 && mx <= 150 && my >= 150 && my <= 281 && !a1)
		{
			//Move to 4

			buffer[0] = 10;
			buffer[1] = 4;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}
		if (mx >= 281 && mx <= 427 && my >= 150 && my <= 281 && !a1)
		{
			//Move to 6

			buffer[0] = 10;
			buffer[1] = 6;

			send(Socket, buffer, 2, 0);

			a1 = true;
		}	
	} */
}


