#include "main.h"


extern SecureTradeClass SecureTrade[];


// Dont' ask.. I don't know what I was thinking when writing this file..


void SecureTradeClass::Abort()
{
	TradeInPending = false;
	TradeInProgress = false;

	Clients[Traders[0]].TradeinProgress = false;
	Clients[Traders[1]].TradeinProgress = false;

	buffer[0] = 54;
	buffer[1] = 21;
	send(Clients[Traders[0]].ClientSocket, buffer, 2, 0);
	send(Clients[Traders[1]].ClientSocket, buffer, 2, 0);

	memset(&Traders, 0, sizeof(Traders));

	return;
}


void SecureTradeClass::do_trade()
{
	bool failed;
	failed = false;
	// Trade will fail if traders don't have enough free slots in backpack.

	short c[2];
	c[0] = 0;
	c[1] = 0;
	short d[2];
	d[0] = 0;
	d[1] = 0;


	for (int i = 0; i < MaxBackpackItems; i++)
	{
		if (Clients[Traders[0]].uChar->Backpack[i].ID == 0)
			c[0]++;
		if (Clients[Traders[1]].uChar->Backpack[i].ID == 0)
			c[1]++;
		if (i < 16)
		{
			if (tradeitems[0][i] != -1)
				d[0]++;
			if (tradeitems[1][i] != -1)
				d[1]++;
		}
	}

	if (c[0] < d[1] || c[1] < d[0])
		failed = true;

	if (failed)
	{
		buffer[0] = 54;
		buffer[1] = 20;
		send(Clients[Traders[0]].ClientSocket, buffer, 2, 0);
		send(Clients[Traders[1]].ClientSocket, buffer, 2, 0);

		TradeInProgress = false;
		TradeInPending = false;
		memset(&Traders, 0, sizeof(Traders));

		return;
	}

	long temp[16];
	short freeslot;
	memset(&temp, -1, sizeof(temp));

	for (int i = 0; i < 8; i++)
	{
		if (tradeitems[0][i] != -1)
		{
			temp[i] = Clients[Traders[0]].uChar->Backpack[tradeitems[0][i]].ID;
			RemoveItemFromPlayer(Traders[0], ItemBackpack, tradeitems[0][i]);
			Clients[Traders[0]].uChar->Backpack[tradeitems[0][i]].ID = 0;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		if (tradeitems[1][i] != -1)
		{
			freeslot = -1;
			freeslot = GetFreeBackpackSlot(Traders[0]);
			if (freeslot != -1)
			{
				Clients[Traders[0]].uChar->Backpack[freeslot] = 
				Clients[Traders[1]].uChar->Backpack[tradeitems[1][i]];
				Items[Clients[Traders[0]].uChar->Backpack[freeslot].ID].KnowID = freeslot;


				RemoveItemFromPlayer(Traders[1], ItemBackpack, tradeitems[1][i]);
				Clients[Traders[1]].uChar->Backpack[tradeitems[1][i]].ID = 0;
				InformPlayerofItem(Traders[0], Clients[Traders[0]].uChar->Backpack[freeslot].ID, ItemBackpack);
			}
		}	
	}

	for (int i = 0; i < 8; i++)
	{
		if (temp[i] != -1)
		{
			freeslot = GetFreeBackpackSlot(Traders[1]);
			if (freeslot != -1)
			{
				Clients[Traders[1]].uChar->Backpack[freeslot].ID = temp[i];
				Items[temp[i]].KnowID = freeslot;

				InformPlayerofItem(Traders[1], temp[i], ItemBackpack);				
			}
		}
	}

	buffer[0] = 54;
	buffer[1] = 8;
	send(Clients[Traders[0]].ClientSocket, buffer, 2, 0);
	send(Clients[Traders[1]].ClientSocket, buffer, 2, 0);

	Clients[Traders[0]].TradeinProgress = false;
	Clients[Traders[1]].TradeinProgress = false;

	TradeInProgress = false;
	TradeInPending = false;
	memset(&Traders, 0, sizeof(Traders));
}

void SecureTradeClass::AbortbyID(short ID)
{
	for (int i = 0; i < 10; i++)
	{
		if ((SecureTrade[i].Traders[0] == ID || SecureTrade[i].Traders[1] == ID) &&
			(SecureTrade[i].TradeInPending || SecureTrade[i].TradeInProgress))
			SecureTrade[i].Abort();
	}
}
