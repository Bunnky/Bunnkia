#include "main.h"


bool MapObjects::deuse(short user)
{
	if (is_blocked()) return false;

	On = true;
	
	buffer[0] = 53;
	buffer[1] = 2;
	memcpy(&buffer[2], &X, sizeof(X));
	memcpy(&buffer[2 + sizeof(X)], &Y, sizeof(Y));
		
	for (int i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			send(Clients[i].ClientSocket, buffer, 2 + sizeof(X) + sizeof(Y), 0);
		}
	}
	return true;
}


bool MapObjects::use(short user)
{
	if (Type != 1) return false;

	On = false;

	buffer[0] = 53;
	buffer[1] = 1;
	memcpy(&buffer[2], &X, sizeof(X));
	memcpy(&buffer[2 + sizeof(X)], &Y, sizeof(Y));
	
	for (int i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			send(Clients[i].ClientSocket, buffer, 2 + sizeof (X) + sizeof (Y), 0);
		}	
	}
	        
	return true;
}

bool MapObjects::is_blocked()
{
	if (!IsValid(X, Y)) 
		return true;

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used)
		{
			if (Clients[i].State != 5)
			{
				if (Clients[i].uChar->X == X && Clients[i].uChar->Y == Y && !Clients[i].Inv)
					return true;
			}
		}
	}

	short hshx, hshy;
	hshx = X / 64;
	hshy = Y / 64;

	mon2_List = Monsters[hshx][hshy];

	while (mon2_List != NULL)
	{
		if (mon2_List->X == X && mon2_List->Y == Y)
			return true;
	
		if (mon2_List != NULL)
			mon2_List = mon2_List->next;
	}

	npc2_List = NPC[hshx][hshy];

	while (npc2_List != NULL)
	{
		if (npc2_List->X == X && npc2_List->Y == Y)
			return true;
	
		if (npc2_List != NULL)
			npc2_List = npc2_List->next;
	}


	for (i = 0; i < MaxItems; i++)
	{
		if (Ground[i] != 0)
		{
			if (Items[Ground[i]].x == X && Items[Ground[i]].y == Y)
				return true;
		}
	}
	return false;
}