#include <iostream>
#include <fstream>
#include <time.h>


#include "main.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)
#pragma warning(disable:4018)

//-----------------
// Needed externs.
//-----------------

extern ClientClass Clients[Max_Connections];


extern PACKET_ITEMMOVE		ItemMove;
extern PACKET_NEWAMOUNT		NewAmount;

extern char *buffer;
extern char *buffer2;

extern bool UsedItemID[MaxItems];
extern bool UsedPlayerID[Max_Connections];


extern short LastCreated;

extern unsigned long GroundTime[MaxItems];

PACKET_BODY NewBody;
PACKET_NEWDECAY NewDecay;

extern BodyClass Body[m_Bodies];

long LastTakenID = 0;

//-----------------------------------------------------------------
//
void CreateItemToGround ( ItemStruct This)
{
	long TempID;
	short TmpID;

	TmpID = GetFreeGroundSlot();
	if (TmpID == -1) return;

	TempID = GetFreeItemID();
	if (TempID == -1) return;

    memcpy(&Items[TempID], &This, sizeof(This));

	Ground[TmpID] = TempID;

	Items[Ground[TmpID]].KnowID = TmpID;
   
	GroundTime[TmpID] = GetTickCount64();

	short ox, oy, tx, ty;

	ox = This.x;
	oy = This.y;

	buffer[0] = 12;
	buffer[1] = 1;

	memcpy(&buffer[2], &Items[Ground[TmpID]], sizeof(Items[Ground[TmpID]]));


	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;

			if (IsInRange(ox, oy, tx, ty))
			{
				Clients[i].KnowItem[TmpID] = true;
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(Items[Ground[TmpID]]), 0);
			}
		}
	}
}
//-----------------------------------------------------------------


short GetFreeBackpackSlot(short This)
{
	short i;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (Clients[This].uChar->Backpack[i].ID == 0)
			return i;
	}
	return -1;
}

short GetFreeBankSlot(short This)
{
	short i;
	for (i = 0; i < MaxBankItems; i++)
	{
		if (Clients[This].uChar->Bank[i].ID == 0)
			return i;
	}
	return -1;
}


//-----------------------------------------------------------------
//
void RemoveItemFromMass( short ItemID )
{
	short ox, oy, tx, ty;

	ox = Items[Ground[ItemID]].x;
	oy = Items[Ground[ItemID]].y;

	buffer[0] = 12;
	buffer[1] = 11;

	memcpy(&buffer[2], &ItemID, sizeof(ItemID));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
	
			Clients[i].KnowItem[ItemID] = false;
	
			if (IsInRange(ox, oy, tx, ty)) // Player is in visual range of this item.
			{
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(ItemID), 0);
			}
		}	
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void RemoveItemFromPlayer( short This, int FromList, short KnowID)
{
	switch (FromList)
	{
	case ItemBackpack:
		
		buffer[0] = 12;
		buffer[1] = 12;

		memcpy(&buffer[2], &KnowID, sizeof(KnowID));

		send(Clients[This].ClientSocket, buffer, 2 + sizeof(KnowID), 0);

		break;
	case ItemBank:

		buffer[0] = 12;
		buffer[1] = 14;

		memcpy(&buffer[2], &KnowID, sizeof(KnowID));

		send(Clients[This].ClientSocket, buffer, 2 + sizeof(KnowID), 0);

		break;
	case ItemWeared:

		buffer[0] = 12;
		buffer[1] = 13;

		memcpy(&buffer[2], &KnowID, sizeof(KnowID));

		send(Clients[This].ClientSocket, buffer, 2 + sizeof(KnowID), 0);

		break;
	case ItemGround:
		
		buffer[0] = 12;
		buffer[1] = 11;

		memcpy(&buffer[2], &KnowID, sizeof(KnowID));

		send(Clients[This].ClientSocket, buffer, 2 + sizeof(KnowID), 0);
        
		break;
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformMassofItem( long ItemID)
{

	short ox, oy, tx, ty;

	ox = Items[Ground[ItemID]].x;
	oy = Items[Ground[ItemID]].y;

	buffer[0] = 12;
	buffer[1] = 1;
	
	memcpy(&buffer[2], &Items[Ground[ItemID]], sizeof(Items[Ground[ItemID]]));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;

			Clients[i].KnowItem[ItemID] = false;

			if (IsInRange(ox, oy, tx, ty))
			{
				Clients[i].KnowItem[ItemID] = true;
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(Items[Ground[ItemID]]), 0);
			}	
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformPlayerofItem( short This, long This2, int InList)
{
	if (This2 == NULL) return;
	ItemStruct New;
	memset(&New, 0, sizeof(New));

	New.Durability = Items[This2].Durability;
	New.Type = Items[This2].Type;
	New.ID = Items[This2].ID;
	New.Quantity = Items[This2].Quantity;
	New.x = Items[This2].x;
	New.y = Items[This2].y;
	New.Family = Items[This2].Family;
	New.Used = Items[This2].Used;
	New.KnowID = Items[This2].KnowID;

	switch (InList)
	{
	// ----------------------------------------------------------------
	// These are for informing player of his own new item.

	case ItemBackpack:

        buffer[0] = 12;
		buffer[1] = 2;
        
		memcpy(&buffer[2], &New, sizeof(New));
        
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(New), 0);

		break;
	case ItemBank:

        buffer[0] = 12;
		buffer[1] = 4;
        
		memcpy(&buffer[2], &New, sizeof(New));
        
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(New), 0);

		break;
	case ItemWeared:

        buffer[0] = 12;
		buffer[1] = 3;
        
		memcpy(&buffer[2], &New, sizeof(New));
        
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(New), 0);

		break;

	case ItemInBody:

        buffer[0] = 22;
		buffer[1] = 3;
        
		memcpy(&buffer[2], &New, sizeof(New));
        
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(New), 0);

		break;

	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void CheckItemsOnScreen(short This)
{
	short ox, oy, tx, ty;

	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	int i;
	for (i = 0; i < MaxItems; i++)
	{
		if (Ground[i] != 0)
		{
			tx = Items[Ground[i]].x;
			ty = Items[Ground[i]].y;
			
			if (IsInRange(ox, oy, tx, ty)) // Player is in visual range of this item.
			{
				if (!Clients[This].KnowItem[i])
				{
					// Send info about this item to player.
	
					Clients[This].KnowItem[i] = true;
	
					buffer[0] = 12;
					buffer[1] = 1;
	
					memcpy(&buffer[2], &Items[Ground[i]], sizeof(Items[Ground[i]]));
	
					send(Clients[This].ClientSocket, buffer, 2 + sizeof(ItemStruct), 0);
				}
			}
			else
			{
				Clients[This].KnowItem[i] = false;
			}
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
short GetFreeGroundSlot()
{
	int C;
	for (C = 0; C < MaxItems; C++)
	{
		if (Ground[C] == 0)
		{
			return C;
		}
	}
	return -1;
}
//-----------------------------------------------------------------



//-----------------------------------------------------------------
//
void InformMassOfNewAmount( short ID, unsigned long Amount)
{
	memset(&NewAmount, 0, sizeof(NewAmount));

	NewAmount.KnowID = ID;
	NewAmount.Amount = Amount;
	NewAmount.InList = ItemGround;

	buffer[0] = 13;
	buffer[1] = 1;
	memcpy(&buffer[2], &NewAmount, sizeof(NewAmount));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].KnowItem[NewAmount.KnowID])
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewAmount), 0);
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformPlayerOfNewAmount( short This, short InList, short ID, unsigned long Amount)
{
	memset(&NewAmount, 0, sizeof(NewAmount));

	NewAmount.InList = InList;
	NewAmount.KnowID = ID;
	NewAmount.Amount = Amount;

    buffer[0] = 13;
	buffer[1] = 1;
	memcpy(&buffer[2], &NewAmount, sizeof(NewAmount));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewAmount), 0);
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformPlayersofGroundItemsNewCoordinates( long This, short newx, short newy)
{
    ItemStruct tmp;
	memset(&tmp, 0, sizeof(tmp));

	memcpy(&tmp, &Items[Ground[This]], sizeof(Items[Ground[This]]));

	PACKET_COORDINATES NewCoords;
	memset(&NewCoords, 0, sizeof(NewCoords));

	NewCoords.X = newx;
	NewCoords.Y = newy;
	NewCoords.ID = This;

	short ox, oy, ox2, oy2, tx, ty;

	ox = Items[Ground[This]].x;
	oy = Items[Ground[This]].y;

	ox2 = newx;
	oy2 = newy;

	buffer[0] = 12;
	buffer[1] = 15;

	memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));

	buffer2[0] = 12;
	buffer2[1] = 1;

	memcpy(&buffer2[2], &tmp, sizeof(tmp));

//-------------------------------------------------------------------------------------------------------------
// Informing Clients of new coordinates of item on screen. There's few phases in doing this:
// 1) Inform players who are in visual range and will stay in visual range even after changed coords.
// 2) Remove item from players who aren't in visual range after changed coords.
// 3) Inform players of new item in visual range, who'll be in range of new coords but wasn't in range before.
//-------------------------------------------------------------------------------------------------------------

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
			
			if (IsInRange(ox, oy, tx, ty))
			{
				// 1) and 2) can be put into same if because Clients will automatically remove items out of sight.
	
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(PACKET_COORDINATES), 0);
	
				if (IsInRange(ox2, oy2, tx, ty))
					Clients[i].KnowItem[This] = true;
				else
					Clients[i].KnowItem[This] = false;
			}
			else 
			if (IsInRange(ox2, oy2, tx, ty))
			{
				// 3)
		
				Clients[i].KnowItem[This] = true;
	
				send(Clients[i].ClientSocket, buffer2, 2 + sizeof(tmp), 0);
			}	
		}
	}

}
//-----------------------------------------------------------------


void CheckBodiesOnScreen( short This)
{
	short ox, oy, tx, ty;

	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	int i;
	for (i = 0; i < m_Bodies; i++)
	{
		if (Body[i].Used)
		{
			tx = Body[i].X;
			ty = Body[i].Y;
			
			if (IsInRange(ox, oy, tx, ty))
			{
				if (!Clients[This].KnowBody[i])
				{
					Clients[This].KnowBody[i] = true;
	
					memset(&NewBody, 0, sizeof(NewBody));
                    NewBody.ID			= i;
					NewBody.DecayState	= Body[i].DecayState;
					NewBody.Looted		= Body[i].Looted;
					NewBody.X			= Body[i].X;
					NewBody.Y			= Body[i].Y;
					NewBody.RealID		= Body[i].Owner_ID;

					strcpy(NewBody.OwnerName, Body[i].OwnerName);

					if (NewBody.Looted)
						strcpy(NewBody.LooterName, Body[i].LooterName);

					buffer[0] = 22;
					buffer[1] = 1;
	
					memcpy(&buffer[2], &NewBody, sizeof(NewBody));
	
					send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewBody), 0);
				}
			}
			else
			{
				Clients[This].KnowBody[i] = false;
			}
		}
	}
}


void InformMassOfNewBodyDecay( short ThisBody)
{
	short ox,oy,tx,ty;

	ox = Body[ThisBody].X;
	oy = Body[ThisBody].Y;

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;

			if (IsInRange(ox, oy, tx, ty))
			{
				memset(&NewDecay, 0, sizeof(NewDecay));

				NewDecay.ID = ThisBody;
				NewDecay.NewDecay = Body[ThisBody].DecayState;

				buffer[0] = 22;
				buffer[1] = 5;
				memcpy(&buffer[2], &NewDecay, sizeof(NewDecay));

				send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewDecay), 0);
			}
			else
			{
				Clients[i].KnowBody[ThisBody] = false;
			}
		}
	}
}

void mass_RemoveBodyItem( short ThisBody, short ThisItem)
{
	short ox,oy,tx,ty;

	ox = Body[ThisBody].X;
	oy = Body[ThisBody].Y;

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
	
			if (IsInRange(ox, oy, tx, ty) && Clients[i].KnowBody[ThisBody])
			{
				buffer[0] = 22;
				buffer[1] = 4;
				memcpy(&buffer[2], &ThisItem, sizeof(ThisItem));

				send(Clients[i].ClientSocket, buffer, 2 + sizeof(ThisItem), 0);
			}
		}
	}
}

bool BodyClass::BodyLooted(short WhoLooted, short ThisBody)
{
	short BodyOwner = -1;
    
	Body[ThisBody].Looted = true;
	strcpy(Body[ThisBody].LooterName, Clients[WhoLooted].uChar->Name);

	// Inform the owner that he got looted.. If he is online.

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].State == Body[ThisBody].Owner_Slot && Clients[i].Acc->getAccID() == Body[ThisBody].Owner_ID)
			{
	            BodyOwner = i;
				i = Max_Connections;
			}
		}
	}

	if (BodyOwner == -1) return false;
	    
	return true;
}

void SplitItem(short Player)
{
	long TempID, This;
	short TmpID;

	switch (ItemMove.FromList)
	{
	case ItemBackpack:

		switch (ItemMove.ToList)
		{
		case ItemGround:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeGroundSlot();
			if (TmpID == -1) break;

			This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
			if (This <= 0) break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Ground[TmpID] = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;
			GroundTime[TmpID] = GetTickCount64();

			Items[This].Quantity -= ItemMove.Amount;

			Items[TempID].KnowID = TmpID;

			InformMassofItem( TmpID );

			break;

		case ItemBank:

			TempID = GetFreeItemID();
			if (TempID == -1) break;;

			TmpID = GetFreeBankSlot(Player);
			if (TmpID == -1) break;

			This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
			if (This == 0) break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Bank[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;
                    
			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBank);

			break;

		case ItemBackpack:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeBackpackSlot(Player);
			if (TmpID == -1) break;

			This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
			if (This == 0) break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Backpack[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;

			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBackpack);

			break;
		}
			
		break;
	case ItemBank:
		switch (ItemMove.ToList)
		{
		case ItemGround:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeGroundSlot();
			if (TmpID == -1) break;

			This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
			if (This == 0) break;
			
			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Ground[TmpID] = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;
			GroundTime[TmpID] = GetTickCount64();

			Items[This].Quantity -= ItemMove.Amount;

			Items[TempID].KnowID = TmpID;

			InformMassofItem( TmpID );

			break;
		case ItemBank:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeBankSlot(Player);
			if (TmpID == -1) break;

			This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
			if (This == 0) break;
			
			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Bank[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;
    
			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBank);

			break;

		case ItemBackpack:

			This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
			if (This == 0) break;


			if (Items[This].Family == FAMILY_WEAPON)
			{
				if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_ARMOR)
			{
				if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_COLLECTABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight * ItemMove.Amount > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_USEABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeBackpackSlot(Player);
			if (TmpID == -1) break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Backpack[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;

			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBackpack);

			break;
		}
		break;
	case ItemGround:

		switch (ItemMove.ToList)
		{
		case ItemGround:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeGroundSlot();
			if (TmpID == -1) break;

			This = Ground[ItemMove.ItemID];
			if (This <= 0 || This > MaxItems) break;

			if (Items[This].x - Clients[Player].uChar->X < -1 ||
				Items[This].x - Clients[Player].uChar->X > 1 ||
				Items[This].y - Clients[Player].uChar->Y < -1 ||
				Items[This].y - Clients[Player].uChar->Y > 1)
			break;


			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformMassOfNewAmount( ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Ground[TmpID] = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;
			GroundTime[TmpID] = GetTickCount64();

			Items[This].Quantity -= ItemMove.Amount;
                    
			Items[TempID].KnowID = TmpID;

			InformMassofItem( TmpID );

			break;
		case ItemBank:

			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeBankSlot(Player);
			if (TmpID == -1) break;

			This = Ground[ItemMove.ItemID];
			if (This == 0) break;

			if (Items[This].x - Clients[Player].uChar->X < -1 ||
				Items[This].x - Clients[Player].uChar->X > 1 ||
				Items[This].y - Clients[Player].uChar->Y < -1 ||
				Items[This].y - Clients[Player].uChar->Y > 1)
			break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformMassOfNewAmount( ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Bank[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;
                    
			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBank);

			break;

		case ItemBackpack:

			This = Ground[ItemMove.ItemID];
			if (This == 0) break;

			if (Items[This].Family == FAMILY_WEAPON)
			{
				if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_ARMOR)
			{
				if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_COLLECTABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight * ItemMove.Amount > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_USEABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}


			TempID = GetFreeItemID();
			if (TempID == -1) break;

			TmpID = GetFreeBackpackSlot(Player);
			if (TmpID == -1) break;

			if (Items[This].x - Clients[Player].uChar->X < -1 ||
				Items[This].x - Clients[Player].uChar->X > 1 ||
				Items[This].y - Clients[Player].uChar->Y < -1 ||
				Items[This].y - Clients[Player].uChar->Y > 1)
			break;

			if (Items[This].Quantity <= 1 || Items[This].Quantity <= ItemMove.Amount) break;
			if (Items[This].KnowID != ItemMove.ItemID) break;

			InformMassOfNewAmount( ItemMove.ItemID, Items[This].Quantity - ItemMove.Amount);

			memcpy(&Items[TempID], &Items[This], sizeof(Items[This]));
				
			Clients[Player].uChar->Backpack[TmpID].ID = TempID;
			Items[TempID].Quantity = ItemMove.Amount;
			Items[TempID].x = ItemMove.x;
			Items[TempID].y = ItemMove.y;
			Items[TempID].Used = true;

			Items[This].Quantity -= ItemMove.Amount;

			Items[TempID].KnowID = TmpID;

			InformPlayerofItem( Player, TempID, ItemBackpack);

			break;
		}
		break;
	}
}

void PileItem(short Player)
{
	long This, TempID;

	switch (ItemMove.FromList)
	{
	case ItemBackpack:

		This = Clients[Player].uChar->Backpack[ItemMove.ItemID].ID;
		if (Items[This].Family != FAMILY_COLLECTABLE || Items[This].KnowID != ItemMove.ItemID) break;

		switch (ItemMove.ToList)
		{
		case ItemBackpack:

			TempID = Clients[Player].uChar->Backpack[ItemMove.Amount].ID;
			
			if (This == TempID) break;

			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);
			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.Amount, Items[TempID].Quantity);
                    
			break;
		case ItemBank:

			TempID = Clients[Player].uChar->Bank[ItemMove.Amount].ID;
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);
			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.Amount, Items[TempID].Quantity);
            
			break;
		case ItemGround:

			TempID = Ground[ItemMove.Amount];
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Clients[Player].uChar->Backpack[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBackpack, ItemMove.ItemID);
			InformMassOfNewAmount(ItemMove.Amount, Items[TempID].Quantity);

			break;
		}

		break;

	case ItemBank:

		This = Clients[Player].uChar->Bank[ItemMove.ItemID].ID;
		if (Items[This].Family != FAMILY_COLLECTABLE || Items[This].KnowID != ItemMove.ItemID) break;

		switch (ItemMove.ToList)
		{
		case ItemBackpack:

			TempID = Clients[Player].uChar->Backpack[ItemMove.Amount].ID;
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			if (Clients[Player].Acc->tChar.Weight + Collectables[Items[TempID].ID].Weight * Items[TempID].Quantity > Clients[Player].Acc->tChar.WeightMax)
				break;

			Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);
			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.Amount, Items[TempID].Quantity);
                    
			break;
		case ItemBank:

			TempID = Clients[Player].uChar->Bank[ItemMove.Amount].ID;
			
			if (This == TempID) break;

			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);
			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.Amount, Items[TempID].Quantity);
            
			break;
		case ItemGround:

			TempID = Ground[ItemMove.Amount];
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Clients[Player].uChar->Bank[ItemMove.ItemID].ID = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromPlayer(Player, ItemBank, ItemMove.ItemID);
			InformMassOfNewAmount(ItemMove.Amount, Items[TempID].Quantity);

			break;
		}

		break;

	case ItemGround:

		This = Ground[ItemMove.ItemID];
		if (Items[This].Family != FAMILY_COLLECTABLE || Items[This].KnowID != ItemMove.ItemID) break;

		switch (ItemMove.ToList)
		{
		case ItemBackpack:

			TempID = Clients[Player].uChar->Backpack[ItemMove.Amount].ID;
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			if (Clients[Player].Acc->tChar.Weight + Collectables[Items[TempID].ID].Weight * Items[TempID].Quantity > Clients[Player].Acc->tChar.WeightMax)
				break;

			RemoveItemFromMass(ItemMove.ItemID);

			Ground[ItemMove.ItemID] = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			InformPlayerOfNewAmount(Player, ItemBackpack, ItemMove.Amount, Items[ItemMove.Amount].Quantity);
                    
			break;
		case ItemBank:

			TempID = Clients[Player].uChar->Bank[ItemMove.Amount].ID;
			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			if (Items[This].Family == FAMILY_WEAPON)
			{
				if (Clients[Player].Acc->tChar.Weight + Weapons[Items[This].Type][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_ARMOR)
			{
				if (Clients[Player].Acc->tChar.Weight + Armors[Items[This].Type-11][Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_COLLECTABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Collectables[Items[This].ID].Weight*Items[This].Quantity > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}
			if (Items[This].Family == FAMILY_USEABLE)
			{
				if (Clients[Player].Acc->tChar.Weight + Useables[Items[This].ID].Weight > Clients[Player].Acc->tChar.WeightMax) 
					break;
			}

			Ground[ItemMove.ItemID] = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromMass(ItemMove.ItemID);
			InformPlayerOfNewAmount(Player, ItemBank, ItemMove.Amount, Items[ItemMove.Amount].Quantity);
            
			break;
		case ItemGround:

			TempID = Ground[ItemMove.Amount];
			if (TempID <= 0) break;
			if (!Items[TempID].Used) break;
	
			if (This == TempID) break;

			if (Items[This].Family != Items[TempID].Family ||
				Items[This].Type != Items[TempID].Type ||
				Items[This].ID != Items[TempID].ID ||
				Items[TempID].Family != FAMILY_COLLECTABLE)
				break;

			if (Items[TempID].KnowID != ItemMove.Amount) break;

			Ground[ItemMove.ItemID] = 0;
					
			Items[TempID].Quantity += Items[This].Quantity;
			memset(&Items[This], 0, sizeof(Items[This]));

			RemoveItemFromMass(ItemMove.ItemID);
			InformMassOfNewAmount(ItemMove.Amount, Items[ItemMove.Amount].Quantity);

			break;
		}

		break;

	}
}

long GetFreeItemID()
{
	if (LastTakenID < 1000000)
	{
		if (!Items[LastTakenID + 1].Used)
		{
			LastTakenID++;
			return LastTakenID;
		}
	}
	long C;
	for (C = 1; C < m_Items; C++)
	{
		if (!Items[C].Used)
		{
			LastTakenID = C;
			return C;
		}
	}
	printf("WARNING! Allocated item memory has ran out!\n");
	return -1;	
}

void InformNewItemDurability(short Player, short ThisItem)
{
	memset(&NewAmount, 0, sizeof(NewAmount));

	NewAmount.InList = ItemBackpack;
	NewAmount.KnowID = ThisItem;
	NewAmount.Amount = Items[Clients[Player].uChar->Backpack[ThisItem].ID].Durability;

    buffer[0] = 13;
	buffer[1] = 1;
	memcpy(&buffer[2], &NewAmount, sizeof(NewAmount));

	send(Clients[Player].ClientSocket, buffer, 2 + sizeof(NewAmount), 0);
}