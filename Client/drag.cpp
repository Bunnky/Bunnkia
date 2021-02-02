#include "main.h"

//==============================================================================
//
//	Drag Handling [START/CLICK]
//
//==============================================================================
void DraggingClass::Start (int MODE) 
{ 
	On = true; 
	Mode = MODE; 

	if (Mode != ITEM)
	{
		switch (Mode)
		{
		case BACKPACK:
			x = event.motion.x - Backpack.X;
			y = event.motion.y - Backpack.Y;
			break;

		case MINIMAP:
			x = event.motion.x - Minimap->X;
			y = event.motion.y - Minimap->Y;
			break;

		case BANK:
			x = event.motion.x - Bank.X;
			y = event.motion.y - Bank.Y;
			break;

		case IDENTIFY:
			iddrag = idatabase->ison();
			if (iddrag == NULL)
			{
				On = false;
				Mode = 0;
				return;
			}
			iddrag->makelast();
			x = event.motion.x - iddrag->x;
			y = event.motion.y - iddrag->y;
			break;
		}		
	}
}

//==============================================================================
//
//	Drag Handling [ON RELEASE]
//
//==============================================================================
void DraggingClass::Release()
{
	if (Mode == ITEM)
	{
		bool Error = false;

	// Dragging was released.
	// This is order for checking where item was dropped:
	// 1) Backpack
	// 2) Bank
	// <!>3) Minimap
	// <!>4) Identifywindow
	// 5) Item on playscreen
	//
	// In <!> cases dragging fails.
	//#############################

		short mx, my;

		mx = event.motion.x;
		my = event.motion.y;

		if (mx <= Backpack.X + Backpack_w && mx >= Backpack.X && my <= Backpack.Y + Backpack_h && my >= Backpack.Y && Backpack.On)
		{
			if (Dragging.Split)
			{

				Dragging.spx = MX-Backpack.X-13;
				Dragging.spy = MY-Backpack.Y-13;

				Dragging.ItemID = Dragging.TargetItem->KnowID;
				Dragging.ToList = ItemBackpack;

				Writing.Start(2);

				return;
			}

			// Here needs to be checked if item was piled.

			if (Dragging.TargetItem->Type == TYPE_COLLECTABLE)
			{
				short tx, ty, ox, oy;
				ox = mx - Backpack.X - 3;
				oy = my - Backpack.Y - 17;


				int i;
				for (i = MaxBackpackItems-1; i >= 0; i--)
				{
					if (iBackpack[i].Used)
					{
						tx = iBackpack[i].x;
						ty = iBackpack[i].y;
	
						if (iBackpack[i].Type == TYPE_COLLECTABLE && iBackpack[i].ID == Dragging.TargetItem->ID && i != Dragging.TargetItem->KnowID)
						{
							if (ox - tx >= 0 && ox - tx < 20 && oy - ty >= 0 && oy - ty < 20)
							{	
		                        memset(&ItemMove, 0, sizeof(ItemMove));
	
								ItemMove.FromList = Dragging.FromList;
								ItemMove.ToList = ItemBackpack;
	
								ItemMove.x = MX-Backpack.X+7;
								ItemMove.y = MY-Backpack.Y-7;
								ItemMove.ItemID = Dragging.TargetItem->KnowID;
								ItemMove.Amount = iBackpack[i].KnowID;
	
								if (ItemMove.x < 0) ItemMove.x = 0;
								if (ItemMove.y < 0) ItemMove.y = 0;					
	
	
								ItemMove.Case = 15;
	
								buffer[0] = 20;
								memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
								send (Socket, buffer, 1 + sizeof(ItemMove), 0);

								Dragging.Stop();
	
								return;
								i = 0;
							}
						}
					}
				}			
			}

			printf("Stop dragging in Backpack\n");
			memset(&ItemMove, 0, sizeof(ItemMove));

			switch (Dragging.FromList)
			{
			case ItemBackpack:
				ItemMove.Case = 13;
				break;
			case ItemGround:
				ItemMove.Case = 10;
				break;
			case ItemBank:
				ItemMove.Case = 8;
				break;
			case ItemWeared:
				ItemMove.Case = 1;
				break;				
			}

			ItemMove.ToList = ItemBackpack;

			ItemMove.x = MX-Backpack.X-13;
			ItemMove.y = MY-Backpack.Y-27;
			ItemMove.ItemID = Dragging.TargetItem->KnowID;

			if (ItemMove.x < 0) ItemMove.x = 0;
			if (ItemMove.y < 0) ItemMove.y = 0;
			
			

			buffer[0] = 20;
			memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
			send (Socket, buffer, 1 + sizeof(ItemMove), 0);

			Dragging.Stop();
			return;
		}        	
		if (mx <= Bank.X + Bank_w && mx >= Bank.X && my <= Bank.Y + Bank_h && my >= Bank.Y && Bank.On)
		{
			if (Dragging.Split)
			{
				Dragging.spx = MX-Bank.X-13;
				Dragging.spy = MX-Bank.Y-27;

				Dragging.ItemID = Dragging.TargetItem->KnowID;
				Dragging.ToList = ItemBank;

				Writing.Start(2);

				return;
			}

			// Here needs to be checked if item was piled.

			if (Dragging.TargetItem->Type == TYPE_COLLECTABLE)
			{
				short tx, ty, ox, oy;
				ox = mx - Bank.X - 3;
				oy = my - Bank.Y - 17;


				int i;
				for (i = MaxBankItems-1; i >= 0; i--)
				{
					if (iBank[i].Used)
					{
						tx = iBank[i].x;
						ty = iBank[i].y;
	
						if (iBank[i].Type == TYPE_COLLECTABLE && iBank[i].ID == Dragging.TargetItem->ID && i != Dragging.TargetItem->KnowID)
						{
							if (ox - tx >= 0 && ox - tx < 20 && oy - ty >= 0 && oy - ty < 20)
							{
								fprintf(stderr, "Pileable item found.\n");
	
		                        memset(&ItemMove, 0, sizeof(ItemMove));
	
								ItemMove.FromList = Dragging.FromList;
								ItemMove.ToList = ItemBank;
	
								ItemMove.x = MX-Bank.X+7;
								ItemMove.y = MY-Bank.Y-7;
								ItemMove.ItemID = Dragging.TargetItem->KnowID;
								ItemMove.Amount = iBank[i].KnowID;
	
								if (ItemMove.x < 0) ItemMove.x = 0;
								if (ItemMove.y < 0) ItemMove.y = 0;					
	
	
								ItemMove.Case = 15;
	
								buffer[0] = 20;
								memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
								send (Socket, buffer, 1 + sizeof(ItemMove), 0);
	
								i = -1;
							}
						}
					}
				}			
			}

			printf("Stop dragging in Bank\n");
			memset(&ItemMove, 0, sizeof(ItemMove));

			switch (Dragging.FromList)
			{
			case ItemBackpack:
				ItemMove.Case = 7;
				break;
			case ItemGround:
				ItemMove.Case = 12;
				break;
			case ItemBank:
				ItemMove.Case = 13;
				break;
			case ItemWeared:
				ItemMove.Case = 3;
				break;				
			}

			ItemMove.ToList = ItemBank;

			ItemMove.x = MX-Bank.X-13;
			ItemMove.y = MY-Bank.Y-27;
			ItemMove.ItemID = Dragging.TargetItem->KnowID;

			if (ItemMove.x < 0) ItemMove.x = 0;
			if (ItemMove.y < 0) ItemMove.y = 0;
			
			

			buffer[0] = 20;
			memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
			send (Socket, buffer, 1 + sizeof(ItemMove), 0);

			Dragging.Stop();
			return;
		}    

		if (mx <= Minimap->X + Minimap_w && mx >= Minimap->X && my <= Minimap->Y + Minimap_h && my >= Minimap->Y)
		{
			Dragging.Stop();
			return;
		}

		if (mx >= 186 && my >= 186 && mx <= 245 && my <= 245)
		{
			if (Dragging.Split)
			{
				Dragging.spx = pldatabase->own->x + (((mx-186) / 20)-1);
				Dragging.spy = pldatabase->own->y + (((my-186) / 20)-1);

				Dragging.ItemID = Dragging.TargetItem->KnowID;
				Dragging.ToList = ItemGround;

				Writing.Start(2);

				return;
			}

			// Here needs to be checked if item was piled.

			if (Dragging.TargetItem->Type == TYPE_COLLECTABLE)
			{

				short tx, ty, ox, oy;
				ox = pldatabase->own->x + (((mx-186) / 20)-1);
				oy = pldatabase->own->x + (((mx-186) / 20)-1);

				int i;
				for (i = MaxItems-1; i >= 0; i--)
				{
					if (iGround[i].Used)
					{
						tx = iGround[i].x;
						ty = iGround[i].y;
	
						if (iGround[i].Type == TYPE_COLLECTABLE && iGround[i].ID == Dragging.TargetItem->ID)
						{
							if (ox == tx && oy == ty)
							{
						        memset(&ItemMove, 0, sizeof(ItemMove));
	
								ItemMove.FromList = Dragging.FromList;
								ItemMove.ToList = ItemGround;
	
								ItemMove.x = iGround[i].x;
								ItemMove.y = iGround[i].y;
								ItemMove.ItemID = Dragging.TargetItem->KnowID;
								ItemMove.Amount = Dragging.TargetItem->Quantity;

								if (ItemMove.x < 0) ItemMove.x = 0;
								if (ItemMove.y < 0) ItemMove.y = 0;					
	
	
								ItemMove.Case = 15;
	
								buffer[0] = 20;
								memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
								send (Socket, buffer, 1 + sizeof(ItemMove), 0);

								return;	
							}
						}
					}
				}			
			}

			memset(&ItemMove, 0, sizeof(ItemMove));

			// Dragging ends to ground, begins from:

			switch (Dragging.FromList)
			{
			case ItemBackpack:
				ItemMove.Case = 9;
				break;
			case ItemGround:
				ItemMove.Case = 13;
				break;
			case ItemBank:
				ItemMove.Case = 11;
				break;
			case ItemWeared:
				ItemMove.Case = 5;
				break;				
			}

			ItemMove.ToList = ItemGround;

			ItemMove.x = pldatabase->own->x + (((mx-186) / 20)-1);
			ItemMove.y = pldatabase->own->y + (((my-186) / 20)-1);
			ItemMove.ItemID = Dragging.TargetItem->KnowID;

			if (ItemMove.x < 0) ItemMove.x = 0;
			if (ItemMove.y < 0) ItemMove.y = 0;

			buffer[0] = 20;
			memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
			send (Socket, buffer, 1 + sizeof(ItemMove), 0);

			Dragging.Stop();
			return;
		}
		if (mx >= 672 && mx <= 781 && my >= 67 && my <= 198)
		{
			memset(&ItemMove, 0, sizeof(ItemMove));

			// Dragging ends to equipment, begins from:

			switch (Dragging.FromList)
			{
			case ItemBackpack:
				ItemMove.Case = 2;
				break;
			case ItemGround:
				ItemMove.Case = 6;
				break;
			case ItemBank:
				ItemMove.Case = 4;
				break;
			case ItemWeared:
				return;
				break;				
			}

			ItemMove.x = 0;
			ItemMove.y = 0;
			ItemMove.ItemID = Dragging.TargetItem->KnowID;

			buffer[0] = 20;
			memcpy(&buffer[1], &ItemMove, sizeof(ItemMove));
			send (Socket, buffer, 1 + sizeof(ItemMove), 0);

			Dragging.Stop();
			return;
		}
	}
}
