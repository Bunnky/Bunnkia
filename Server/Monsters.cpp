#include <iostream>
#include <fstream>
#include <time.h>


#include "main.h"


#pragma warning(disable:4018)
#pragma warning(disable:4244)
#pragma warning(disable:4996)

//-----------------
// Needed externs.
//-----------------

extern ClientClass Clients[Max_Connections];


extern bool UsedMonsterSpawns[MaxMonsterSpawns];

extern PACKET_MONSTER		NewMonster;
extern PACKET_NPC			NewNPC;

extern MiscPicClass			NewPic;
extern NewCombatTextClass	NewCT;
extern NewHPClass			NewHP;

extern char *buffer;
extern char *buffer2;
extern char *buffer3;

extern ofstream drops;
extern ofstream dropz;

extern char Time[];
extern char Date[];

extern short SafeZones[512][512];

extern bool Failsafe;

extern ofstream debuggi;

short blocktable[5][5];

short check_adv_dir(short tmx, short tmy);


//-----------------------------------------------------------------
//
/*void AddMonster( MonsterClass This)
{
	MonsterClass *New;// = new MonsterClass;

	tmp = (MonsterClass *)malloc ( sizeof(CLIENT_TYP));

	memset(New, 0, sizeof(New));

	memcpy(New, &This, sizeof(This));

	// List is empty, create it.
	if (Monsters_Beg == NULL) 
	{
        New->Next = NULL;
		New->Prev = NULL;
        Monsters_Beg = New;
	}
	else
	{
		New->Next = Monsters_Beg->Next;
		New->Prev = Monsters_Beg;

		if (New->Next != NULL)
			New->Next->Prev = New;

		Monsters_Beg->Next = New;
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void RemoveMonster( MonsterClass *This)
{	
	Monsters = Monsters_Beg;

	if (Monsters_Beg->Next == NULL) 
	{ 
		Monsters_Beg = NULL; 
		Monsters = NULL;
	}
	else
	{
		while (Monsters != NULL)
		{
			if (Monsters == This)
			{	
				// Remove tmp from the list.

				if (Monsters != Monsters_Beg)
				{
					Monsters[i].Prev->Next = This->Next;
				}
				else
				{
					if (Monsters_Beg->Next != NULL)
					{
						Monsters_Beg = Monsters_Beg->Next;
					}
					else
					{
						Monsters_Beg = NULL;
					}
				}


				if (This->Next != NULL)
				Monsters[i].Next->Prev = This->Prev;

				FreeMonsterID( This->KnowID);

				UsedMonsterSpawns[This->UsedSpawn] = false;

				Monsters = NULL;
			}
			else
			{
				Monsters = Monsters[i].Next;
			}
		}
	}
	delete This;
} */
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
short GetFreeMonsterID()
{
	int i;
	for (i = 0; i < m_MonsterIDs; i++)
	{
		if (!UsedMonsterID[i])
		{
			UsedMonsterID[i] = true;
			return i;
		}
	}
	printf("%s: Error at [GetFreeMonsterID]: No free IDs left!\n", Time);
	return -1;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void FreeMonsterID(short ThisSlot)
{
	UsedMonsterID[ThisSlot] = false;
}
//-----------------------------------------------------------------



void MonsterAction_Regenerate( MonsterClass *This )
{
	This->LstReg = GetTickCount();

	if (This->HPLeft >= MonsterTypes[This->Type].HPMax)
		return;

	short reg_am;
	reg_am = MonsterTypes[This->Type].con / 4;

	This->HPLeft += reg_am;
	if (This->HPLeft > MonsterTypes[This->Type].HPMax)
		This->HPLeft = MonsterTypes[This->Type].HPMax;
	
	InformNewMonsterHP(This);
}

void MonsterAction_Berserk( MonsterClass *This)
{
	short smallest, smallest_am, dif;
	smallest = -1;
	smallest_am = 100;
	dif = 0;

	short ox, oy, tx, ty, tmp;
	ox = This->X;
	oy = This->Y;

	for (int i = 0; i < Max_Connections; i++)
	{	
		if (Clients[i].Used && Clients[i].State != 5 && Clients[i].GMMode == 0)
		{
			dif = 0;			

			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;

			if (IsInRange(ox, oy, tx, ty))
			{
				tmp = ox - tx;
				dif += (tmp < 0 ? tmp * -1 : tmp);
				
				tmp = oy - ty;
				dif += (tmp < 0 ? tmp * -1 : tmp);

				if (smallest_am > dif)
				{
					smallest_am = dif;
					smallest = i;
				}			
			}		
		}
	}
	if (smallest != -1)
	{
		if (Clients[smallest].GMMode == 0)
			This->Target = &Clients[smallest];

		This->LstAttack = GetTickCount();
		This->LstMove = GetTickCount();
	}
	else
	{
		NPCClass *oldtarget = This->NPCTarget;
		short hshx[9], hshy[9];
		hshx[0] = ox / 64;
		hshy[0] = oy / 64;

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

		NPCClass *nearest = NULL;
		short calculaatio = 0, smallestam = 10000;

		int i;
		for (i = 0; i < 9; i++)
		{
			if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
			{
				npc2_List = NPC[hshx[i]][hshy[i]];
	
				while (npc2_List != NULL)
				{
					tx = npc2_List->X;
					ty = npc2_List->Y;

					if (IsInRange(ox, oy, tx, ty))
					{
						calculaatio = tx - ox + ty - oy;
						if (calculaatio < 0) calculaatio *= -1;

						if (calculaatio < smallestam && oldtarget != npc2_List)
						{
							nearest = npc2_List;
							smallestam = calculaatio;
						}
					}	

					if (npc2_List != NULL)
						npc2_List = npc2_List->next;	
				}
			}			
		}

		if (nearest != NULL)
		{
			This->Target = NULL;
			This->NPCTarget = nearest;

			This->LstAttack = GetTickCount();
			This->LstMove = GetTickCount();
		}
	}
}


//-----------------------------------------------------------------
//
void MonsterAction_HasTarget( MonsterClass *This)
{
	ClientClass *Target;
	NPCClass *npctarget;

	if (This->Target != NULL && This->NPCTarget == NULL)
	{
		Target = This->Target;
		This->NPCTarget = NULL;

		short ox,oy,tx,ty;

		ox = This->X; oy = This->Y; tx = Target->uChar->X; ty = Target->uChar->Y;
    
		if (GetTickCount() - This->LstMove > MonsterTypes[This->Type].MovingSpeed)
		{
			if (ox - tx < -1 || ox - tx > 1 ||
				oy - ty < -1 || oy - ty > 1)
			{
				MonsterAction_Move( This);
				This->LstMove = GetTickCount();
			}
			else if ((This->HPLeft <= 0.25 * MonsterTypes[This->Type].HPMax) && (ox - tx < -1 || ox - tx > 1 || oy - ty < -1 || oy - ty > 1))
				MonsterAction_Berserk( This );
		}
		if (GetTickCount() - This->LstAttack > MonsterTypes[This->Type].AttackSpeed)
		{
			if (ox - tx > -2 && ox - tx < 2 &&
				oy - ty > -2 && oy - ty < 2)
			{
				MonsterAction_Attack( This);
				This->LstAttack = GetTickCount();
			}
		}	
	}   
	else if (This->Target == NULL && This->NPCTarget != NULL)
	{
		npctarget = This->NPCTarget;
		This->Target = NULL;

		short ox,oy,tx,ty;

		ox = This->X; oy = This->Y; tx = npctarget->X; ty = npctarget->Y;
    
		if (GetTickCount() - This->LstMove > MonsterTypes[This->Type].MovingSpeed)
		{
			if (ox - tx < -1 || ox - tx > 1 ||
				oy - ty < -1 || oy - ty > 1)
			{
				MonsterAction_Move(This);
				This->LstMove = GetTickCount();
			}
		}
		if (GetTickCount() - This->LstAttack > MonsterTypes[This->Type].AttackSpeed)
		{
			if (ox - tx > -2 && ox - tx < 2 &&
				oy - ty > -2 && oy - ty < 2)
			{
				MonsterAction_AttackToNPC( This);
				This->LstAttack = GetTickCount();
			}
		}
		if ((This->Target != NULL || This->NPCTarget != NULL) && 
			(This->HPLeft <= 0.25 * MonsterTypes[This->Type].HPMax || GetTickCount() - This->LstAttack > 10000) 
			&& (ox - tx < -1 || ox - tx > 1 || oy - ty < -1 || oy - ty > 1))
			MonsterAction_Berserk( This );
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void MonsterAction_Attack( MonsterClass *This)
{
	bool ToHit = false;

	int ToHitRoll =0;
	int ToHitDefence =0;
	int ToHitRollDefence =400;			

	ToHitRoll = MonsterTypes[This->Type].Dex;

	ToHitDefence = Clients[This->Target->Know_ID].Acc->tChar.AC * 2;
	
	if (ToHitDefence <= 0) ToHitDefence = 1;
	if (rand() % ToHitDefence < ToHitRoll)
		ToHit = true;
	else ToHit = false;



	short ox, oy, tx, ty;
	tx = This->Target->uChar->X;
	ty = This->Target->uChar->Y;
	memset(&NewPic, 0, sizeof(NewPic));

	if (ToHit)
	{
		// We hit the target.

		// Check Damage

		short Damage;

		if (MonsterTypes[This->Type].DamMax <= 0) MonsterTypes[This->Type].DamMax = 2;
		if (MonsterTypes[This->Type].DamMin <= 0) MonsterTypes[This->Type].DamMin = 1;
		if (MonsterTypes[This->Type].DamMin == MonsterTypes[This->Type].DamMax) 
		{
			printf("Error monster at %d,%d\n", tx, ty);
			return;
		}

		
		Damage = rand() % (MonsterTypes[This->Type].DamMax - MonsterTypes[This->Type].DamMin) + 1;
		Damage += MonsterTypes[This->Type].DamMin;


/*		Reduce = Damage * (1 - (float)This->Target->AC/400);

		Reduce = Damage - Reduce;
		if (Reduce > Damage) Reduce = Damage-1;
		if (Reduce < 0) Reduce = 0;

		Damage -= Reduce; */


		if (MonsterTypes[This->Type].ElementalAttack != NULL)
		{
			if (MonsterTypes[This->Type].ElementalAttack == elem_FIRE)
			{
				if (This->Target->Acc->tChar.prot_Fire > 0)
					Damage *= 1 - (((float)This->Target->Acc->tChar.prot_Fire) / 100);
				if (This->Target->Acc->tChar.prot_Water > 0)
					Damage *= 1 + (((float)This->Target->Acc->tChar.prot_Water) / 100);
			}
			if (MonsterTypes[This->Type].ElementalAttack == elem_AIR)
			{
				if (This->Target->Acc->tChar.prot_Air > 0)
					Damage *= 1 - (((float)This->Target->Acc->tChar.prot_Air) / 100);
				if (This->Target->Acc->tChar.prot_Earth > 0)
					Damage *= 1 + (((float)This->Target->Acc->tChar.prot_Earth) / 100);
			}
			if (MonsterTypes[This->Type].ElementalAttack == elem_EARTH)
			{
				if (This->Target->Acc->tChar.prot_Air > 0)
					Damage *= 1 + (((float)This->Target->Acc->tChar.prot_Air) / 100);
				if (This->Target->Acc->tChar.prot_Earth > 0)
					Damage *= 1 - (((float)This->Target->Acc->tChar.prot_Earth) / 100);
			}
			if (MonsterTypes[This->Type].ElementalAttack == elem_WATER)
			{
				if (This->Target->Acc->tChar.prot_Fire > 0)
					Damage *= 1 + (((float)This->Target->Acc->tChar.prot_Fire) / 100);
				if (This->Target->Acc->tChar.prot_Water > 0)
					Damage *= 1 - (((float)This->Target->Acc->tChar.prot_Water) / 100);
			}			
		}


		if (Damage < 1) Damage = 1;


		// Send new hit pic to players
		
		NewPic.Pic = 0;
		NewPic.X = This->Target->uChar->X;
		NewPic.Y = This->Target->uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		
		int i;
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				ox = Clients[i].uChar->X;
				oy = Clients[i].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
				}	
			}
		}

		// Send combat text to target player

		memset(&NewCT, 0, sizeof(NewCT));

		buffer[0] = 97;
		buffer[1] = 1;

		NewCT.Amount = Damage;
		NewCT.Type = CT_DAMAGETAKEN;
		NewCT.FromTo = This->Type;
		NewCT.SkillType = 1;

		memcpy(&buffer[2], &NewCT, sizeof(NewCT));

		send(This->Target->ClientSocket, buffer, 2 + sizeof(NewCT), 0);

        // Reduce player's HP

		This->Target->uChar->HPLeft -= Damage;

		if ( This->Target->uChar->HPLeft <= 0)
		{
			// Player died.

			DeathPenalty(This->Target);

			This->Target->uChar->HPLeft = This->Target->Acc->tChar.HPMax * 0.75;

			if (This->Target->uChar->DevMode > 0) return;

			if (This->Target->Acc->uChar->Wanted)
			{ //1435,4699

				Clients[This->Target->Know_ID].CrimDeath();
			}


//			InformPlayerofNewAligment2(This->Target);

			This->Target->AttackTarget = -1;
			This->Target->MonTarget = NULL;
			This->Target->NPCTarget = NULL;
			This->Target->PLTarget = -1;
			
			buffer[0] = 11;
			buffer[1] = 1;

			short oldx, oldy;

			short newx, newy;

			oldx = This->Target->uChar->X;
			oldy = This->Target->uChar->Y;

			newx = This->Target->uChar->ShrineX;
			newy = This->Target->uChar->ShrineY;

			CreateBody( This->Target->Know_ID, oldx, oldy);

			This->Target->Acc->uChar->Wanted = false;

			InformMassofNewCriminalStatus2(This->Target);

			This->Target->uChar->X = newx;
			This->Target->uChar->Y = newy;

			PACKET_COORDINATES NewCoords;
			memset(&NewCoords, 0, sizeof(NewCoords));
			NewCoords.X = newx;
			NewCoords.Y = newy;
			NewCoords.ID = This->Target->Acc->getAccID();

			buffer[0] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
			send(This->Target->ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

			CheckPlayersOnScreen(This->Target->Know_ID, NewCoords);
			CheckItemsOnScreen(This->Target->Know_ID);
			CheckMonstersOnScreen(This->Target->Know_ID);
			CheckNPCsOnScreen(This->Target->Know_ID);

			buffer[0] = 99;
			buffer[1] = 5;
			memcpy(&buffer[2], &This->Type, sizeof(This->Type));
			send(This->Target->ClientSocket, buffer, 2 + sizeof(This->Type), 0);			
		}		
		
		// Inform player of new HP

		memset(&NewHP, 0, sizeof(NewHP));

		NewHP.KnowID = This->Target->Acc->getAccID();
		NewHP.NewHP = This->Target->uChar->HPLeft;
		NewHP.Type = 0;

		buffer[0] = 20;
		buffer[1] = 3;
		memcpy(&buffer[2], &NewHP, sizeof(NewHP));

		ox = This->Target->uChar->X;
		oy = This->Target->uChar->Y;

		send(This->Target->ClientSocket, buffer, 2 + sizeof(NewHP), 0);

		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				tx = Clients[i].uChar->X;
				ty = Clients[i].uChar->Y;
				if (IsInRange(ox, oy, tx, ty))
				{
					if (Clients[i].KnowPlayer[This->Target->Know_ID] || &Clients[i] == This->Target)
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewHP), 0);			
				}
			}
		}
	}
	else
	{
		// Missed.

		// Send miss pic to players

		NewPic.Pic = 1;
		NewPic.X = This->Target->uChar->X;
		NewPic.Y = This->Target->uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));


		int i;
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				ox = Clients[i].uChar->X;
				oy = Clients[i].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPic), 0);
				}
			}
		}
	}
}
//-----------------------------------------------------------------



void MonsterAction_AttackToNPC(MonsterClass *This)
{
	bool ToHit = false;

	int ToHitRoll =0;
	int ToHitDefence =0;
	int ToHitRollDefence =400;			

	ToHitRoll = MonsterTypes[This->Type].Dex;

	ToHitDefence = 150;
	
	if (ToHitDefence <= 0) ToHitDefence = 1;
	if (rand() % ToHitDefence < ToHitRoll)
		ToHit = true;
	else ToHit = false;



	short ox, oy, tx, ty;
	tx = This->NPCTarget->X;
	ty = This->NPCTarget->Y;
	memset(&NewPic, 0, sizeof(NewPic));

	if (ToHit)
	{
		// We hit the target.

		// Check Damage

		short Damage;

		if (MonsterTypes[This->Type].DamMax <= 0) MonsterTypes[This->Type].DamMax = 2;
		if (MonsterTypes[This->Type].DamMin <= 0) MonsterTypes[This->Type].DamMin = 1;
		if (MonsterTypes[This->Type].DamMin == MonsterTypes[This->Type].DamMax) 
		{
			printf("Error monster at %d,%d\n", tx, ty);
			return;
		}

		
		Damage = rand() % (MonsterTypes[This->Type].DamMax - MonsterTypes[This->Type].DamMin) + 1;
		Damage += MonsterTypes[This->Type].DamMin;


		if (Damage < 1) Damage = 1;


		// Send new hit pic to players
		
		NewPic.Pic = 0;
		NewPic.X = This->NPCTarget->X;
		NewPic.Y = This->NPCTarget->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		
		int i;
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				ox = Clients[i].uChar->X;
				oy = Clients[i].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
				}	
			}
		}



		if (This->NPCTarget->NPCPLTarget != -1 && This->NPCTarget->NPCMonsterTarget == NULL && NPCTypes[This->NPCTarget->Type].is_guard)
		{
			short x3,y3,t3,j3, id;

			id = Client_GetID(This->NPCTarget->NPCPLTarget);
					
			x3 = This->NPCTarget->X;
			t3 = Clients[ id ].uChar->X; // NPCPLTARGET EI OLE CLIENT ID, VAAN ACCID
			y3 = This->NPCTarget->Y;
	        j3 = Clients[ id ].uChar->Y;
	
			if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1)
			{
				This->NPCTarget->NPCMonsterTarget = This;
			} 
		}
		else 
		if (This->NPCTarget->NPCPLTarget == -1 && This->NPCTarget->NPCMonsterTarget != NULL && NPCTypes[This->NPCTarget->Type].is_guard)
		{			
			short x3,y3,t3,j3;
					
			x3 = This->NPCTarget->X;
			t3 = This->NPCTarget->NPCMonsterTarget->X;
			y3 = This->NPCTarget->Y;
	        j3 = This->NPCTarget->NPCMonsterTarget->Y;
	
			if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1 && This->NPCTarget->NPCMonsterTarget != This)
			{
				This->NPCTarget->NPCMonsterTarget = This;
			}
		}

        // Reduce player's HP

		This->NPCTarget->HPLeft -= Damage;

		if ( This->NPCTarget->HPLeft <= 0)
		{
			buffer[0] = 20;
			buffer[1] = 7;

			memcpy(&buffer[2], &This->NPCTarget->ID, sizeof(This->NPCTarget->ID));

			for (int j = 0; j < Max_Connections; j++)
			{
				if (Clients[j].Used && Clients[j].State != 5)
				{
					if (Clients[j].KnowNPC[This->NPCTarget->ID])
					{
						Clients[j].KnowNPC[This->NPCTarget->ID] = false;
						send(Clients[j].ClientSocket, buffer, 2 + sizeof(This->NPCTarget->ID), 0);
					}
					if (Clients[j].NPCTarget == This->NPCTarget)
						Clients[j].NPCTarget = NULL;
				}
			}

			short hshx[9], hshy[9];
			hshx[0] = This->NPCTarget->X / 64;
			hshy[0] = This->NPCTarget->Y / 64;

			hshx[1] = hshx[0] -1; hshy[1] = hshy[0] -1; hshx[2] = hshx[0] -1;hshy[2] = hshy[0]; hshx[3] = hshx[0] -1;hshy[3] = hshy[0] +1; hshx[4] = hshx[0];hshy[4] = hshy[0] -1; hshx[5] = hshx[0];hshy[5] = hshy[0] +1; hshx[6] = hshx[0] +1;	hshy[6] = hshy[0] -1; hshx[7] = hshx[0] +1;hshy[7] = hshy[0]; hshx[8] = hshx[0] +1;	hshy[8] = hshy[0] +1; 

			int i;
			for (i = 0; i < 9; i++)
			{
				if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
				{
					mon2_List = Monsters[hshx[i]][hshy[i]];
	
					while (mon2_List != NULL)
					{
						if (mon2_List->NPCTarget == This->NPCTarget && This != mon2_List)
							mon2_List->NPCTarget = NULL;

						if (mon2_List != NULL)
							mon2_List = mon2_List->next;	
					}
				}			
			}

			NPCAction_Died(This->NPCTarget);

			This->NPCTarget = NULL;
		}
	}
	else
	{
		// Missed.

		// Send miss pic to players

		NewPic.Pic = 1;
		NewPic.X = This->NPCTarget->X;
		NewPic.Y = This->NPCTarget->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));


		int i;
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				ox = Clients[i].uChar->X;
				oy = Clients[i].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPic), 0);
				}
			}
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void MonsterAction_Move( MonsterClass *This)
{
	if (MoveEnemy(This))
	{
		// Monster successfully moved, inform players.

		memset(&NewMonster, 0, sizeof(NewMonster));

		NewMonster.X = This->X;
		NewMonster.Y = This->Y;
		NewMonster.HPLeft = This->HPLeft;
		NewMonster.KnowID = This->KnowID;
		NewMonster.Type = This->Type;
        
		PACKET_COORDINATES NewCoords;
		memset(&NewCoords, 0, sizeof(NewCoords));

		NewCoords.X = This->X;
		NewCoords.Y = This->Y;
		NewCoords.ID = This->KnowID;		

		short ox, oy, tx, ty;

		tx = This->X;
		ty = This->Y;

		int i;
		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				ox = Clients[i].uChar->X;
				oy = Clients[i].uChar->Y;
		
				if (IsInRange(ox, oy, tx, ty)) // Player is in visual range of this monster.
				{
					if (Clients[i].KnowMonster[This->KnowID])
					{
						// Send new coords to players
		
						buffer[0] = 20;
						buffer[1] = 4;
			
						memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
			
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);			
					}
					else
					{
						// Send info about this monster to player.
			
						Clients[i].KnowMonster[This->KnowID] = true;
			
						buffer[0] = 20;
						buffer[1] = 1;
			
						memcpy(&buffer[2], &NewMonster, sizeof(NewMonster));
		
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewMonster), 0);			
					}
				}
				else
				{
					Clients[i].KnowMonster[This->KnowID] = false;
				}
			}	
		}
	}
	// Else, monster didn't move and nothing happened.
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void CheckMonsterSpawn()
{
	int a2;
	bool IsGood = false;

	MonsterSpawnStruct *spawn_List;
	spawn_List = MonsterSpawns;

	while (spawn_List != NULL)
	{
		if (!spawn_List->Used)
		{
			a2 = rand() % 100;

			if (a2 < Spawn_Rate)
			{
				//------------------------------
				// This monster will be spawned
				//------------------------------

                MonsterClass *NewMonster = new MonsterClass;
				spawn_List->Used = true;
				NewMonster->Used_Spawn = spawn_List;

				NewMonster->LstAttack = GetTickCount();
				NewMonster->LstMove = GetTickCount();
				NewMonster->LstReg = GetTickCount();

				NewMonster->Type = spawn_List->Type;
				NewMonster->Target = NULL;
				NewMonster->NPCTarget = NULL;
				NewMonster->KnowID = GetFreeMonsterID();

				if (NewMonster->KnowID == -1)
				{
					delete NewMonster;
					spawn_List->Used = false;
					return;
				}
				

				NewMonster->HPLeft = MonsterTypes[NewMonster->Type].HPMax;
				
				short rnd_x, rnd_y;
				rnd_x = 0;//spawn_List->MaxDist_x;
				rnd_y = 0;//spawn_List->MaxDist_y;

				if (rnd_x == 0 || rnd_y == 0)
				{
					rnd_x = 32;
					rnd_y = 32;
				}

				bool IsSpawned = false;
				
				short Count = 0;

				while (!IsSpawned && Count < 5)
				{
					NewMonster->X = spawn_List->X - ((rand() % rnd_x) + rnd_x / 2);
					NewMonster->Y = spawn_List->Y - ((rand() % rnd_y) + rnd_y / 2);

					if (IsValid(NewMonster->X, NewMonster->Y))
						IsSpawned = true;

					Count++;			
				}

				if (!IsSpawned)
				{
					NewMonster->X = spawn_List->X;
					NewMonster->Y = spawn_List->Y;
				}

				short hshx, hshy;
				hshx = NewMonster->X / 64;
				hshy = NewMonster->Y / 64;

				if (Monsters[hshx][hshy] == NULL)
				{
					NewMonster->next = NULL;
					Monsters[hshx][hshy] = NewMonster;
				}
				else
				{
					NewMonster->next = Monsters[hshx][hshy];
					Monsters[hshx][hshy] = NewMonster;				
				}
			}
		}
		spawn_List = spawn_List->next;
	}
}
//-----------------------------------------------------------------


void SpawnMonster(short x, short y, short id)
{
	MonsterClass *NewMon = new MonsterClass;
	NewMon->Used_Spawn = NULL;

	NewMon->LstAttack = GetTickCount();
	NewMon->LstMove = GetTickCount();
	NewMon->LstReg = GetTickCount();

	NewMon->Type = id;
	NewMon->Target = NULL;
	NewMon->NPCTarget = NULL;
	NewMon->KnowID = GetFreeMonsterID();

	if (NewMon->KnowID == -1)
	{
		delete NewMon;
		return;
	}
				
	NewMon->HPLeft = MonsterTypes[NewMon->Type].HPMax;
				
	short rnd_x, rnd_y;
	rnd_x = 0;//spawn_List->MaxDist_x;
	rnd_y = 0;//spawn_List->MaxDist_y;

	if (rnd_x == 0 || rnd_y == 0)
	{
		rnd_x = 32;
		rnd_y = 32;
	}

	bool IsSpawned = false;
		
	short Count = 0;

	while (!IsSpawned && Count < 5)
	{
		NewMon->X = x + (rand() % (rnd_x)) - rnd_x / 2;
		NewMon->Y = y + (rand() % (rnd_y)) - rnd_y / 2;

		if (IsValid(NewMon->X, NewMon->Y))
			IsSpawned = true;

		Count++;
	}

	if (!IsSpawned)
	{
		NewMon->X = y;
		NewMon->Y = x;
	}

	printf("Spawned monster %s at %d,%d\n", MonsterTypes[NewMon->Type].Name, NewMon->X, NewMon->Y);

	short hshx, hshy;
	hshx = NewMon->X / 64;
	hshy = NewMon->Y / 64;

	if (Monsters[hshx][hshy] == NULL)
	{
		NewMon->next = NULL;
		Monsters[hshx][hshy] = NewMon;
	}
	else
	{
		NewMon->next = Monsters[hshx][hshy];
		Monsters[hshx][hshy] = NewMon;				
	}

	InformPlayersofNewMonster(NewMon);
}



void InformPlayersofNewMonster(MonsterClass *NewMon)
{
	short ox, oy, tx, ty;

	ox = NewMon->X;
	oy = NewMon->Y;

	short hshx[9], hshy[9];
	hshx[0] = ox / 64;
	hshy[0] = oy / 64;
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


	buffer[0] = 20;
	buffer[1] = 1;
	
	memset(&NewMonster, 0, sizeof(NewMonster));

	NewMonster.HPLeft = NewMon->HPLeft;
	NewMonster.KnowID = NewMon->KnowID;
	NewMonster.Type = NewMon->Type;
	NewMonster.X = NewMon->X;
	NewMonster.Y = NewMon->Y;
		
	memcpy(&buffer[2], &NewMonster, sizeof(NewMonster));
	

	for (int i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].State != 5 && Clients[i].Used)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
		
			if (IsInRange(ox, oy, tx, ty))
			{
				if (!Clients[i].KnowMonster[NewMon->KnowID])
				{
					printf("Informing of %d\n", NewMon->KnowID);
					Clients[i].KnowMonster[NewMon->KnowID] = true;
		
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewMonster), 0);
				}
			}
			else
				Clients[i].KnowMonster[NewMon->KnowID] = false;
		}
	}
}



void CheckNPCSpawn()
{
	// Check NPC spawns

	NPCSpawnStruct *n;
	n = NPCSpawns;

	while (n)
	{
		if (!n->Used && GetTickCount() - n->LstSpawned > 180000)
		{

			//------------------------------
			// This NPC will be spawned
			//------------------------------

			NPCClass *NewNPC = new NPCClass;
			n->Used = true;
			NewNPC->UsedSpawn = n;

			NewNPC->LstAttack = GetTickCount();
			NewNPC->LstMove = GetTickCount();
			NewNPC->LstReg = GetTickCount();

			NewNPC->Type = n->Type;
			NewNPC->NPCMonsterTarget = NULL;
			NewNPC->NPCPLTarget = -1;
			NewNPC->ID = GetFreeNPCID();

			if (NewNPC->ID == -1)
			{
				delete NewNPC;
				n->Used = false;
				return;
			}
				

			NewNPC->HPLeft = NPCTypes[NewNPC->Type].max_hp;

			//NewNPC->conv_id = NPCTypes[NewNPC->Type].conv_id;
			NewNPC->conv_id = n->conv_id;
			NewNPC->ShopID  = n->ShopID;

			NewNPC->X = n->X;
			NewNPC->Y = n->Y;
			NewNPC->oX = n->X;
			NewNPC->oY = n->Y;

			short hshx, hshy;
			hshx = NewNPC->X / 64;
			hshy = NewNPC->Y / 64;

			if (NPC[hshx][hshy] == NULL)
			{
				NewNPC->next = NULL;
				NPC[hshx][hshy] = NewNPC;
			}
			else
			{
				NewNPC->next = NPC[hshx][hshy];
				NPC[hshx][hshy] = NewNPC;				
			}
		}
		n = n->next;
	}
}


short GetFreeNPCID()
{
	int i;
	for (i = 0; i < m_NPCIDs; i++)
	{
		if (!UsedNPCID[i])
		{
			UsedNPCID[i] = true;
			return i;
		}
	}
	printf("%s: Error at [GetFreeNPCID]: No free IDs left!\n", Time);
	return -1;
}

//-----------------------------------------------------------------
//
int Check_dir(short x, short y)
{
	// Jos monsteri yrittää väistää ylös tai vasemmalle
	if (x == -1 && y == -1) { return 7; }
	// Jos monsteri yrittää väistää ylös tai oikealle
	if (x == 1 && y == -1) { return 9; }
	// Jos monsteri yrittää väistää alas tai vasemmalle
	if (x == -1 && y == 1) { return 1; }
	// Jos monsteri yrittää väistää alas tai oikealle
	if (x == 1 && y == 1) { return 3; }
	// Ylös tai alas
	if (x == -1 && y == 0) { return 4; }
	if (x == 1 && y == 0) { return 4; }
	// Oikealle tai vasemmalle
	if (x == 0 && y == 1) { return 8; }
	if (x == 0 && y == -1) { return 8; }

	else return 0;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
bool MoveEnemy(MonsterClass *This)
{
	char xx, yy;
	int Error = 0;

	int ox, oy, tx, ty;

	if (This->Target != NULL)
	{
		ox = This->Target->uChar->X;
		oy = This->Target->uChar->Y;
	}
	else if (This->NPCTarget != NULL)
	{
		ox = This->NPCTarget->X;
		oy = This->NPCTarget->Y;
	}

	tx = This->X;
	ty = This->Y;
	
	// Remember old coords.

	short OldX = This->X;
	short OldY = This->Y;

	// First step, check the direction that monster should go to get near player.


	short tmx, tmy;
	tmx = ox - tx;
	xx = (tmx > 0 ? 1 : (tmx < 0 ? -1 : 0));
	tmy = oy - ty;
	yy = (tmy > 0 ? 1 : (tmy < 0 ? -1 : 0));


	// Save possible new coordinates.
	
	int NewX, NewY;
	
	NewX = tx + xx;
	NewY = ty + yy;

	// Check if monster can move to this tile, if not, then run step 2.
	
	if (!IsValid(NewX, NewY)) { Error = 1; }

	// Check if monster is trying to move onto other monster


	short hshx, hshy;
	hshx = NewX / 64;
	hshy = NewY / 64;

	mon2_List = Monsters[hshx][hshy];

	while (mon2_List != NULL)
	{
		if (mon2_List->X == NewX && mon2_List->Y == NewY)
		{
			Error = 1;
			mon2_List = NULL;
		}
	
		if (mon2_List != NULL)
			mon2_List = mon2_List->next;
	}

	// Is he trying to ram on player?


	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].uChar->X == NewX && Clients[i].uChar->Y == NewY && !Clients[i].Inv)
			{
				Error = 1;
				i = Max_Connections;
			}
		}
	}

	npc2_List = NPC[hshx][hshy];

	while (npc2_List != NULL)
	{
		if (npc2_List->X == NewX && npc2_List->Y == NewY)
		{
			Error = 1;
			npc2_List = NULL;
		}
	
		if (npc2_List != NULL)
			npc2_List = npc2_List->next;
	}

	obj_List = Objects[hshx][hshy];
	
	while (obj_List)
	{
		if (obj_List->X == NewX && obj_List->Y == NewY && obj_List->On)
		{
			if (obj_List->use(-1))
				Error = 2;
			else Error = 1;

			obj_List = NULL;
		}

		if (obj_List != NULL) obj_List = obj_List->next;
	}

	//--------
	// Step 2
	//--------

	if (Error == 1)
	{
		Error = 0;

		memset(&blocktable, 0, sizeof(blocktable));

		hshx = tx / 64;
		hshy = ty / 64;

		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 5; y++)
			{
				blocktable[x][y] = !IsValid(tx - 2 + x, ty - 2 + y);
			}
		}

		npc2_List = NPC[hshx][hshy];		
		while (npc2_List != NULL)
		{
			tmx = npc2_List->X - tx;
			tmy = npc2_List->Y - ty;

			if ((tmx < 0 ? tmx * -1 : tmx) <= 4 &&
				(tmy < 0 ? tmy * -1 : tmy) <= 4)
				blocktable[tmx + 2][tmy + 2] = 1;

			npc2_List = npc2_List->next;
		}

		mon2_List = Monsters[hshx][hshy];
		while (mon2_List != NULL)
		{
			tmx = mon2_List->X - tx;
			tmy = mon2_List->Y - ty;

			if ((tmx < 0 ? tmx * -1 : tmx) <= 4 &&
				(tmy < 0 ? tmy * -1 : tmy) <= 4)
				blocktable[tmx + 2][tmy + 2] = 1;

			mon2_List = mon2_List->next;
		}

		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				tmx = Clients[i].uChar->X - tx;
				tmy = Clients[i].uChar->Y - ty;

				if ((tmx < 0 ? tmx * -1 : tmx) <= 4 &&
					(tmy < 0 ? tmy * -1 : tmy) <= 4)
					blocktable[tmx + 2][tmy + 2] = 1;
			}
		}

		obj_List = Objects[hshx][hshy];
	
		while (obj_List)
		{
			tmx = obj_List->X - tx;
			tmy = obj_List->Y - ty;

			if ((tmx < 0 ? tmx * -1 : tmx) <= 4 &&
				(tmy < 0 ? tmy * -1 : tmy) <= 4 && obj_List->On)
				blocktable[tmx + 2][tmy + 2] = 1;

			obj_List = obj_List->next;
		}

		
		tmx = tx - ox;
		tmy = ty - oy;


		short dir;
		dir = -1;

		dir = check_adv_dir(tmx, tmy);
		if (dir == 5 || dir == -1)
			Error = 1;

		if (dir == 1 || dir == 2 || dir == 3)
			NewY = ty + 1;
		if (dir == 4 || dir == 6)
			NewY = ty;
		if (dir == 7 || dir == 8 || dir == 9)
			NewY = ty - 1;

		if (dir == 1 || dir == 4 || dir == 7)
			NewX = tx - 1;
		if (dir == 2 || dir == 8)
			NewX = tx;
		if (dir == 3 || dir == 6 || dir == 9)
			NewX = tx + 1;
	}

	if (!IsValid(NewX, NewY)) { Error = 1; }
	

	if (Error == 0) 
	{ 
		if (This->X / 64 != NewX / 64 || This->Y / 64 != NewY / 64) 
			Monster_MoveList(This, NewX, NewY);

		This->X = NewX; 
		This->Y = NewY; 
		return true;}
	else
	{ return false; }
}
//-----------------------------------------------------------------


short check_adv_dir(short tmx, short tmy)
{
	short tt = rand() % 2;
	if (tmy > 0 && tmx > 0)
	{	
		if (!blocktable[1][1])
			return 7;
		else if (tt == 0 && !blocktable[1][2])
			return 4;
		else if (tt == 1 && !blocktable[2][1])
			return 8;
		else if (rand() % 2 == 0 && !blocktable[1][3])
			return 1;
		else if (tt == 0 && !blocktable[2][3])
			return 2;
		else if (tt == 1 && !blocktable[3][1])
			return 9;
	}
	else if (tmy < 0 && tmx > 0)
	{			
		if (!blocktable[1][3])
			return 1;
		else if (tt == 0 && !blocktable[1][2])
			return 4;
		else if (tt == 1 && !blocktable[2][1])
			return 8;
		else if (rand() % 2 == 0 && !blocktable[1][1])
			return 7;
		else if (tt == 0 && !blocktable[2][3])
			return 2;
		else if (tt == 1 && !blocktable[3][3])
			return 3;
	}
	else if (tmy > 0 && tmx < 0)
	{
		if (!blocktable[3][1])
			return 9;						
		else if (tt == 0 && !blocktable[3][2])
			return 6;
		else if (tt == 1 && !blocktable[2][1])
			return 8;		
		else if (rand() % 2 == 0 && !blocktable[3][3])
			return 3;			
		else if (tt == 0 && !blocktable[2][3])
			return 2;			
		else if (tt == 1 && !blocktable[1][1])
			return 7;		
	}
	else if (tmy < 0 && tmx < 0)
	{
		if (!blocktable[3][3])
			return 3;
		else if (tt == 0 && !blocktable[3][2])
			return 6;
		else if (tt == 1 && !blocktable[2][3])
			return 2;		
		else if (rand() % 2 == 0 && !blocktable[3][1])
			return 9;			
		else if (tt == 0 && !blocktable[2][1])
			return 8;			
		else if (tt == 1 && !blocktable[1][3])
			return 1;		
	}
	if (tmy == 0 && tmx > 0)
	{
		if (!blocktable[1][2])
			return 4;
		else if (tt == 0 && !blocktable[1][1])
			return 7;
		else if (tt == 1 && !blocktable[1][3])
			return 1;
		else if (tt == 0 && !blocktable[2][1])
			return 8;
		else if (tt == 1 && !blocktable[2][3])
			return 2;
	}
	else if (tmy == 0 && tmx < 0)
	{
		if (!blocktable[3][2])
			return 6;
		else if (tt == 0 && !blocktable[3][1])
			return 9;
		else if (tt == 1 && !blocktable[3][3])
			return 3;
		else if (tt == 0 && !blocktable[2][1])
			return 8;
		else if (tt == 1 && !blocktable[2][3])
			return 2;
	}
	else if (tmy < 0 && tmx == 0)
	{
		if (!blocktable[2][3])
			return 2;
		else if (tt == 0 && !blocktable[3][3])
			return 3;
		else if (tt == 1 && !blocktable[1][3])
			return 1;
		else if (tt == 0 && !blocktable[1][2])
			return 4;
		else if (tt == 1 && !blocktable[3][2])
			return 6;
	}
	else if (tmy > 0 && tmx == 0)
	{
		if (!blocktable[2][1])
			return 8;
		else if (tt == 0 && !blocktable[3][1])
			return 9;
		else if (tt == 1 && !blocktable[1][1])
			return 7;
		else if (tt == 0 && !blocktable[1][2])
			return 4;
		else if (tt == 1 && !blocktable[3][2])
			return 6;
	}
	return 5;
}


bool Check_PosMove(short ToX, short ToY)
{
	if (!IsValid(ToX, ToY)) return false;

	short hshx, hshy;
	hshx = ToX / 64;
	hshy = ToY / 64;

	mon2_List = Monsters[hshx][hshy];

	while (mon2_List != NULL)
	{
		if (mon2_List->X == ToX && mon2_List->Y == ToY)
			return false;	
	
		mon2_List = mon2_List->next;
	}

	// Is he trying to ram on player?

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].uChar->X == ToX && Clients[i].uChar->Y == ToY  && !Clients[i].Inv)
			{
				return false;
			}
		}
	}

	npc2_List = NPC[hshx][hshy];

	while (npc2_List != NULL)
	{
		if (npc2_List->X == ToX && npc2_List->Y == ToY)
			return false;	
	
		npc2_List = npc2_List->next;
	}

	return true;
}




//-----------------------------------------------------------------
//
void CheckTarget( MonsterClass *This)
{
	short ox, oy, tx, ty;
	tx = This->X;
	ty = This->Y;

	bool False = false;
	int i;

	int Near = -1;
	short calculaatio = 0, smallestam = 10000;

	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			ox = Clients[i].uChar->X;
			oy = Clients[i].uChar->Y;
			if (IsInRange(ox, oy, tx, ty) && Clients[i].uChar->DevMode < 1)
			{
				calculaatio = tx - ox + ty - oy;
				if (calculaatio < 0) calculaatio *= -1;

				if (calculaatio < smallestam)
				{
					Near = i;
					smallestam = calculaatio;
				}
			}	
		}
	}
	if (Near != -1)
	{
		This->Target = &Clients[Near];
		False = true;
	}

	if (!False)
	{
		short hshx[9], hshy[9];
		hshx[0] = tx / 64;
		hshy[0] = ty / 64;

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

		NPCClass *nearest = NULL;
		short calculaatio = 0, smallestam = 10000;

		int i;
		for (i = 0; i < 9; i++)
		{
			if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
			{
				npc2_List = NPC[hshx[i]][hshy[i]];
	
				while (npc2_List != NULL)
				{
					ox = npc2_List->X;
					oy = npc2_List->Y;

					if (IsInRange(ox, oy, tx, ty))
					{
						calculaatio = tx - ox + ty - oy;
						if (calculaatio < 0) calculaatio *= -1;

						if (calculaatio < smallestam)
						{
							nearest = npc2_List;
							smallestam = calculaatio;
						}
					}	

					if (npc2_List != NULL)
						npc2_List = npc2_List->next;	
				}
			}			
		}

		if (nearest != NULL)
		{
			This->Target = NULL;
			This->NPCTarget = nearest;
			This->LstAttack = GetTickCount();
		}

	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//Informs all the clients in the range of the monster its new HP
//-----------------------------------------------------------------
void InformNewMonsterHP(MonsterClass *This)
{
	short ox, oy, tx, ty;

	//Get the Position of the Monster
	ox = This->X;
	oy = This->Y;

	int i;
	for (i = 0; i < Max_Connections; i++) //Cycle through all the Clients
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			//Get the Position of the Player
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
			
			if (IsInRange(ox, oy, tx, ty)) //If Player is in visual range of this monster.
			{
				if (Clients[i].KnowMonster[This->KnowID]) //I am guessing if the player knows this monster, Radinakh
				{
					buffer[0] = 20; //According to the client its for "Monster Things"
					buffer[1] = 3; //According to the client its for "Some Player/Monster have lost/gained HP"
	
					memset(&NewHP, 0, sizeof(NewHP)); //Set memory for the NewHP Packet
			
					NewHP.NewHP = This->HPLeft; //Set the new HP in the NewHP Packet
					NewHP.Type = 1; //Set the type to "1", which means a monster
					NewHP.KnowID = This->KnowID; //Set the ID in the NewHP Packet
	
					memcpy(&buffer[2], &NewHP, sizeof(NewHP)); //Copy the NewHP Packet to buffer[2]
	
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewHP), 0); //Send the Packet to the Client
				}
			}
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void CheckMonstersOnScreen(short This)
{
	short ox, oy, tx, ty;

	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	short hshx[9], hshy[9];
	hshx[0] = ox / 64;
	hshy[0] = oy / 64;
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

	int i;
	for (i = 0; i < 9; i++)
	{
		if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
		{
			mon2_List = Monsters[hshx[i]][hshy[i]];

			while (mon2_List != NULL)
			{
				tx = mon2_List->X;
				ty = mon2_List->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					if (!Clients[This].KnowMonster[mon2_List->KnowID])
					{
						Clients[This].KnowMonster[mon2_List->KnowID] = true;
	
						buffer[0] = 20;
						buffer[1] = 1;
	
						memset(&NewMonster, 0, sizeof(NewMonster));
	
						NewMonster.HPLeft = mon2_List->HPLeft;
						NewMonster.KnowID = mon2_List->KnowID;
						NewMonster.Type = mon2_List->Type;
						NewMonster.X = mon2_List->X;
						NewMonster.Y = mon2_List->Y;
		
						memcpy(&buffer[2], &NewMonster, sizeof(NewMonster));
		
						send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewMonster), 0);
					}
				}
				else
					Clients[This].KnowMonster[mon2_List->KnowID] = false;

				mon2_List = mon2_List->next;
			}
		}
	}
}
//-----------------------------------------------------------------



void CheckPlayersOnScreen(short Player, PACKET_COORDINATES NewCoordinates)
{
	// Inform other players for current player moving.

	buffer[0] = 10;
	buffer[1] = 2;

	memcpy(&buffer[2], &NewCoordinates, sizeof(NewCoordinates));

	short int tmpx, tmpy;
	short int plx, ply, i;

	plx = Clients[Player].uChar->X;
	ply = Clients[Player].uChar->Y;

	PACKET_PLAYERINFO NewInfoPacket;
	PACKET_PLAYERINFO NewInfoPacket2;
	memset(&NewInfoPacket, 0, sizeof(NewInfoPacket));

	NewInfoPacket.Avatar = Clients[Player].uChar->Avatar;
	NewInfoPacket.Race = Clients[Player].uChar->Race;
	NewInfoPacket.HPLeft = Clients[Player].uChar->HPLeft;
	//NewInfoPacket.HPMax = Clients[Player].uChar->HPMax;
	NewInfoPacket.HPMax = Clients[Player].Acc->Calc_hpMax();
	NewInfoPacket.ID = Clients[Player].Acc->getAccID();
	NewInfoPacket.Reputation = Clients[Player].uChar->Reputation;
	NewInfoPacket.Wanted = Clients[Player].Acc->uChar->Wanted;
	NewInfoPacket.X = Clients[Player].uChar->X;
	NewInfoPacket.Y = Clients[Player].uChar->Y;
	strcpy(NewInfoPacket.Name, Clients[Player].uChar->Name);
	strcpy(NewInfoPacket.Tag, Clients[Player].Acc->uChar->Tag);

	buffer2[0] = 10;
	buffer2[1] = 3;
	memcpy(&buffer2[2], &NewInfoPacket, sizeof(NewInfoPacket));

	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tmpx = Clients[i].uChar->X;
			tmpy = Clients[i].uChar->Y;

	        if (i != Player)
			{
				if (IsInRange2(plx, ply, tmpx, tmpy) && Clients[i].KnowPlayer[Player])
				{
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCoordinates), 0);
				}
	
				if (IsInRange(plx, ply, tmpx, tmpy))
				{
					if (Clients[i].KnowPlayer[Player] && (!Clients[Player].Inv || Clients[Player].uChar->DevMode <= Clients[i].uChar->DevMode))
					{
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCoordinates), 0);
					}
					if (!Clients[i].KnowPlayer[Player] && (!Clients[Player].Inv || Clients[Player].uChar->DevMode <= Clients[i].uChar->DevMode))
					{
						send(Clients[i].ClientSocket, buffer2, 2 + sizeof(NewInfoPacket), 0);
					}
	
					if (!Clients[Player].Inv || Clients[Player].uChar->DevMode <= Clients[i].uChar->DevMode)
					Clients[i].KnowPlayer[Player] = true;
	
					if (!Clients[Player].KnowPlayer[i] && (!Clients[i].Inv || Clients[Player].uChar->DevMode >= Clients[i].uChar->DevMode))
					{
						memset(&NewInfoPacket2, 0, sizeof(NewInfoPacket2));

						NewInfoPacket2.Avatar = Clients[i].uChar->Avatar;
						NewInfoPacket2.HPLeft = Clients[i].uChar->HPLeft;
						//NewInfoPacket2.HPMax = Clients[i].Acc->tChar.HPMax;
						NewInfoPacket2.HPMax = Clients[i].Acc->Calc_hpMax();
						NewInfoPacket2.ID = Clients[i].Acc->getAccID();
						NewInfoPacket2.Reputation = Clients[i].uChar->Reputation;
						NewInfoPacket2.Wanted = Clients[i].Acc->uChar->Wanted;
						NewInfoPacket2.X = Clients[i].uChar->X;
						NewInfoPacket2.Y = Clients[i].uChar->Y;
						strcpy(NewInfoPacket2.Name, Clients[i].uChar->Name);
						strcpy(NewInfoPacket2.Tag, Clients[i].Acc->uChar->Tag);
		
						buffer3[0] = 10;
						buffer3[1] = 3;
						memcpy(&buffer3[2], &NewInfoPacket2, sizeof(NewInfoPacket2));
						
						Clients[Player].KnowPlayer[i] = true;
	
						send(Clients[Player].ClientSocket, buffer3, 2 + sizeof(NewInfoPacket), 0);					
					}
				}		
				else
				{
					Clients[i].KnowPlayer[Player] = false;
					Clients[Player].KnowPlayer[i] = false;
				}
			}
		}
	}
}


//-----------------------------------------------------------------
//
void CheckNPCsOnScreen(short This)
{
	short ox, oy, tx, ty;

	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	short hshx[9], hshy[9];
	hshx[0] = ox / 64;
	hshy[0] = oy / 64;
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

	int i;
	for (i = 0; i < 9; i++)
	{
		if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
		{
			npc2_List = NPC[hshx[i]][hshy[i]];

			while (npc2_List != NULL)
			{
				tx = npc2_List->X;
				ty = npc2_List->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					if (!Clients[This].KnowNPC[npc2_List->ID])
					{
						Clients[This].KnowNPC[npc2_List->ID] = true;
		
						buffer[0] = 20;
						buffer[1] = 5;
		
						memset(&NewNPC, 0, sizeof(NewNPC));
			
						NewNPC.HP = npc2_List->HPLeft;
						NewNPC.ID = npc2_List->ID;
						NewNPC.X = npc2_List->X;
						NewNPC.Y = npc2_List->Y;
						NewNPC.conv_id = npc2_List->conv_id;
						NewNPC.type = npc2_List->Type;
								
						memcpy(&buffer[2], &NewNPC, sizeof(NewNPC));
		
						send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);			
					}
				}
				else
					Clients[This].KnowNPC[npc2_List->ID] = false;

				npc2_List = npc2_List->next;
			}
		}
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
MonsterClass *GetMonsterByID( short ID )
{
	int i, j;
	for (i = 0; i < 128; i++)
	{
		for (j = 0; j < 128; j++)
		{
			mon2_List = Monsters[i][j];

			while (mon2_List != NULL)
			{
				if (mon2_List->KnowID == ID)
				{
					return mon2_List;
				}
				
                if (mon2_List != NULL)
					mon2_List = mon2_List->next;
			}
		}
	}

	printf("%s: Error at [GetMonsterByID]: No Monster found by ID %d\n", Time, ID);
	return NULL;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void DropLoot(MonsterClass *This, int Luck)
{
//	if (SafeZones[This->X >> 4][This->Y >> 4]) return;
	int p;
	ItemStruct Tmp;
	memset(&Tmp, 0, sizeof(Tmp));
	int i;
	for (i = 0; i < 10; i++)
	{
		if (MonsterTypes[This->Type].Loot[i].DropProbability > 0)
		{
			//p = rand() % 100000;
			p = rand() % 10000;
			if (p <= (MonsterTypes[This->Type].Loot[i].DropProbability/* + Luck*/))
			{

				Tmp.Family = MonsterTypes[This->Type].Loot[i].Family;
				Tmp.ID = MonsterTypes[This->Type].Loot[i].ID;
				Tmp.Type = MonsterTypes[This->Type].Loot[i].Type;
				Tmp.x = This->X;
				Tmp.y = This->Y;
				Tmp.Used = true;

				if (Tmp.Family == FAMILY_WEAPON)
				{
					Tmp.Durability = Weapons[Tmp.Type][Tmp.ID].MaxDur; 
					if (i > 0 && Tmp.ID >= 26)
					{
						drops << Date << " " << Time << ": (" << MonsterTypes[This->Type].Name << ") " << Weapons[Tmp.Type][Tmp.ID].Name << " [" << Clients[Luck].uChar->Name << "]" << endl;
					}
				}
				if (Tmp.Family == FAMILY_ARMOR)
				{
					Tmp.Durability = Armors[Tmp.Type-11][Tmp.ID].MaxDur; 
					if (i > 0)
					{
						drops << Date << " " << Time << ": (" << MonsterTypes[This->Type].Name << ") " << Armors[Tmp.Type-11][Tmp.ID].Name << " [" << Clients[Luck].uChar->Name << "]" << endl;
					}
				}
				if (Tmp.Family == FAMILY_USEABLE)
					Tmp.Durability = Useables[Tmp.ID].MaxDurability;

				if (MonsterTypes[This->Type].Loot[i].Type == TYPE_COLLECTABLE)
				{
					if (MonsterTypes[This->Type].Loot[i].AmountMax <= 0 || MonsterTypes[This->Type].Loot[i].AmountMin <= 0 ||
						MonsterTypes[This->Type].Loot[i].AmountMax == MonsterTypes[This->Type].Loot[i].AmountMin)
						Tmp.Quantity = 1;
					else
					{
						Tmp.Quantity = rand() % (MonsterTypes[This->Type].Loot[i].AmountMax - MonsterTypes[This->Type].Loot[i].AmountMin);
						Tmp.Quantity += MonsterTypes[This->Type].Loot[i].AmountMin;
					}
				}
				else
				{
					Tmp.Quantity = 1;
				}

				CreateItemToGround(Tmp);
			}
		}
	}
}
//-----------------------------------------------------------------


void Monster_MoveList( MonsterClass *monster, short newx, short newy)
{
	short cur_hashx, cur_hashy, new_hashx, new_hashy;

	cur_hashx = monster->X / 64;
	cur_hashy = monster->Y / 64;

	new_hashx = newx / 64;
	new_hashy = newy / 64;

	mon2_List = Monsters[cur_hashx][cur_hashy];

	MonsterClass *tmptmp;
	tmptmp = NULL;

	if (mon2_List == monster)
	{
		Monsters[cur_hashx][cur_hashy] = Monsters[cur_hashx][cur_hashy]->next;		
	}
	else
	{
		while (mon2_List != NULL)
		{
			if (mon2_List == monster)
			{
				tmptmp->next = mon2_List->next;
				mon2_List = NULL;
			}
	
			if (mon2_List != NULL)
			{
				tmptmp = mon2_List;
				mon2_List = mon2_List->next;
			}
		}
	}

	if (Monsters[new_hashx][new_hashy] == NULL)
	{
		monster->next = NULL;
        Monsters[new_hashx][new_hashy] = monster;
	}
	else
	{
		monster->next = Monsters[new_hashx][new_hashy];
		Monsters[new_hashx][new_hashy] = monster;
	}
	Failsafe = true;
}



void NPCAction_Respawn( NPCClass *npc)
{
//	npc->Alive = ALIVE;

	npc->NPCPLTarget = -1;
	npc->NPCMonsterTarget = NULL;
	
//	npc->HPLeft = NPCTypes[ npc->Type ].max_hp;

	short ox, oy, tx, ty, old_x, old_y;
	
	old_x = npc->X;
	old_y = npc->Y;

	if (npc->X / 64 != npc->oX / 64 || npc->Y / 64 != npc->oY / 64) 
		NPC_MoveList(npc, npc->oX, npc->oY);

	npc->X = npc->oX;
	npc->Y = npc->oY;

	ox = npc->X;
	oy = npc->Y;

	MiscPicClass NewPic_toOldPos;
	
	NewPic.Pic = 11;
	NewPic.X = ox;
	NewPic.Y = oy;

	NewPic_toOldPos.Pic = 11;
	NewPic_toOldPos.X = old_x;
	NewPic_toOldPos.Y = old_y;
			
	buffer2[0] = 21;
	buffer2[1] = 1;
	memcpy(&buffer2[2], &NewPic, sizeof(NewPic));

	buffer3[0] = 21;
	buffer3[1] = 1;
	memcpy(&buffer3[2], &NewPic_toOldPos, sizeof(NewPic_toOldPos));

	buffer[0] = 20;
	buffer[1] = 5;

	NewNPC.HP = npc->HPLeft;
	NewNPC.ID = npc->ID;
	NewNPC.X = npc->X;
	NewNPC.Y = npc->Y;
	NewNPC.conv_id = npc->conv_id;
	NewNPC.type = npc->Type;
	memcpy(&buffer[2], &NewNPC, sizeof(NewNPC));

	int j;
	for (j = 0; j < Max_Connections; j++)
	{
		if (Clients[j].Used && Clients[j].State != 5)
		{
			Clients[j].KnowNPC[npc->ID] = true;
		
			tx = Clients[j].uChar->X;
			ty = Clients[j].uChar->Y;

			if (IsInRange(ox, oy, tx, ty))
			{	
				send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);		
				send(Clients[j].ClientSocket, buffer2, 2 + sizeof(NewPic), 0);			
			}
			if (IsInRange(old_x, old_y, tx, ty))
			{	
				send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);		
				send(Clients[j].ClientSocket, buffer3, 2 + sizeof(NewPic), 0);			
			}
		}
	}
}

void NPCAction_Died( NPCClass *npc)
{
	buffer[0] = 20;
	buffer[1] = 7;
	memcpy(&buffer[2], &npc->ID, sizeof(npc->ID));

	int j;
	for (j = 0; j < Max_Connections; j++)
	{
		if (Clients[j].Used && Clients[j].State != 5)
		{
			if (Clients[j].KnowNPC[npc->ID])
			{
				Clients[j].KnowNPC[npc->ID] = false;
				send(Clients[j].ClientSocket, buffer, 2 + sizeof(npc->ID), 0);
			}
		}
	}	

	
	short hshx, hshy;
	hshx = npc->X / 64;
	hshy = npc->Y / 64;

	if (npc == NPC[hshx][hshy])
	{
		NPC[hshx][hshy] = NPC[hshx][hshy]->next;
		npc->UsedSpawn->Used = false;
		UsedNPCID[ npc->ID ] = false;
		delete npc;
	}
	else
	{
		NPCClass *tmptmp{};
	
		npc_List = NPC[hshx][hshy];

		while (npc_List != NULL)
		{	
			if (npc_List == npc)
			{
				tmptmp->next = npc_List->next;

				npc->UsedSpawn->Used = false;
				UsedNPCID[ npc->ID ] = false;

                delete npc;
			
				npc_List = NULL;
			}        
	
			if (npc_List != NULL)
			{
				tmptmp = npc_List;
				npc_List = npc_List->next;
			}
		}
	}
/*
 
	npc->Alive = DEAD;
	npc->LstDeath = GetTickCount();
	*/
}

void NPCAction_WalkRandom( NPCClass *npc)
{
	npc->LstMove = GetTickCount();

	short x_plus, y_plus;
	x_plus = 0; y_plus = 0;

	short _rand;

	_rand = rand() % 7 + 1;

	if (_rand == 1) 
	{
		x_plus = -1;
		y_plus = 0;
	}
	if (_rand == 2) 
	{
		x_plus = 0;
		y_plus = -1;
	}
	if (_rand == 3) 
	{
		x_plus = 1;
		y_plus = 0;
	}
	if (_rand == 4) 
	{
		x_plus = 0;
		y_plus = 1;
	}

	if (x_plus != 0 || y_plus != 0)
	{	
		short NewX, NewY;
		NewX = npc->X + x_plus; NewY = npc->Y + y_plus;

		int Error = 0;

		if (!IsValid(NewX, NewY)) { return; }
	
		short hshx, hshy;
		hshx = NewX / 64;
		hshy = NewY / 64;

		mon2_List = Monsters[hshx][hshy];
        
		while (mon2_List != NULL)
		{
			if (mon2_List->X == NewX && mon2_List->Y == NewY)
				return;
			
			if (mon2_List != NULL)
				mon2_List = mon2_List->next;		
		}

		npc2_List = NPC[hshx][hshy];

		while (npc2_List != NULL)
		{
            if (npc2_List->X == NewX && npc2_List->Y == NewY &&
				npc->ID != npc2_List->ID)
				return;
				
			if (npc2_List != NULL)
				npc2_List = npc2_List->next;
		}

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				if (Clients[j].uChar->X == NewX && Clients[j].uChar->Y == NewY && !Clients[j].Inv)
					return;
			}
		}

		if (Error == 1) return;


		if (npc->X / 64 != NewX / 64 || npc->Y / 64 != NewY / 64) 
			NPC_MoveList(npc, NewX, NewY);

		npc->X += x_plus;
		npc->Y += y_plus;


		PACKET_COORDINATES NewCoords;
		memset(&NewCoords, 0, sizeof(NewCoords));

		NewCoords.X = npc->X;
		NewCoords.Y = npc->Y;
		NewCoords.ID = npc->ID;


		buffer[0] = 20;
		buffer[1] = 5;
	
		NewNPC.HP = npc->HPLeft;
		NewNPC.ID = npc->ID;
		NewNPC.X = npc->X;
		NewNPC.Y = npc->Y;
		NewNPC.conv_id = npc->conv_id;
		NewNPC.type = npc->Type;
		memcpy(&buffer[2], &NewNPC, sizeof(NewNPC));

		buffer2[0] = 20;
		buffer2[1] = 6;

		memcpy(&buffer2[2], &NewCoords, sizeof(NewCoords));

		short ox,oy,tx,ty;
		ox = npc->X;
		oy = npc->Y;
                
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				tx = Clients[j].uChar->X;
				ty = Clients[j].uChar->Y;


				if (!Clients[j].KnowNPC[npc->ID] && IsInRange(ox, oy, tx, ty))
				{
					Clients[j].KnowNPC[npc->ID] = true;
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);
				}

				if (Clients[j].KnowNPC[npc->ID])
				{
					send(Clients[j].ClientSocket, buffer2, 2 + sizeof(NewCoords), 0);
					if (!IsInRange(ox, oy, tx, ty))
						Clients[j].KnowNPC[npc->ID] = false;
				}
			}	
		}
	}
}


void NPCAction_HasTarget( NPCClass *npc)
{
	bool Monster;
	bool Player;

	Monster = false; Player = false;

	if (npc->NPCPLTarget != -1)
		Player = true;
	else
	if (npc->NPCMonsterTarget != NULL)
		Monster = true;

	ClientClass *PLTarget = NULL;
	MonsterClass *MonTarget;

	short ox,oy,tx,ty;

	if (Player)
	{
		PLTarget = NULL;
		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				if (npc->NPCPLTarget == Clients[j].Acc->getAccID())
				{
			        PLTarget = &Clients[j];                
					j = Max_Connections;
				}
			}
		}
        
		if( PLTarget == NULL)
		{
			npc->NPCPLTarget = -1;
			return;
		}

		ox = npc->X; 
		oy = npc->Y; 
		tx = PLTarget->uChar->X; 
		ty = PLTarget->uChar->Y;

		if (GetTickCount() - npc->LstMove > NPCTypes[ npc->Type ].speed)
		{
			if (ox - tx < -1 || ox - tx > 1 ||
				oy - ty < -1 || oy - ty > 1)
			{
				NPCAction_Move( npc, tx, ty);
				npc->LstMove = GetTickCount();
			}
		}
		if (GetTickCount() - npc->LstAttack > NPCTypes[ npc->Type ].atk_speed)
		{
			if (ox - tx > -2 && ox - tx < 2 &&
				oy - ty > -2 && oy - ty < 2)
			{
				NPCAction_AttackToPlayer( npc, PLTarget);
				npc->LstAttack = GetTickCount();
			}	
		} 
	}

	if (!Player && Monster)
	{		
		MonTarget = npc->NPCMonsterTarget;

		ox = npc->X; oy = npc->Y; tx = MonTarget->X; ty = MonTarget->Y;

		if (GetTickCount() - npc->LstMove > NPCTypes[ npc->Type ].speed)
		{
			if (ox - tx < -1 || ox - tx > 1 ||
				oy - ty < -1 || oy - ty > 1)
			{
				NPCAction_Move( npc, tx, ty);
				npc->LstMove = GetTickCount();
			}
		}
		if (GetTickCount() - npc->LstAttack > NPCTypes[ npc->Type ].atk_speed)
		{
			if (ox - tx > -2 && ox - tx < 2 &&
				oy - ty > -2 && oy - ty < 2)
			{
				NPCAction_AttackToMonster( npc, MonTarget);
				npc->LstAttack = GetTickCount();
			}	
		} 
	}
}

void NPCAction_WalkLimits( NPCClass *npc)
{
	npc->LstMove = GetTickCount();

	short x_plus, y_plus;
	x_plus = 0; y_plus = 0;

	short _rand;

	_rand = rand() % 7 + 1;

	if (_rand == 1) 
	{
		x_plus = -1;
		y_plus = 0;
	}
	if (_rand == 2) 
	{
		x_plus = 0;
		y_plus = -1;
	}
	if (_rand == 3) 
	{
		x_plus = 1;
		y_plus = 0;
	}
	if (_rand == 4) 
	{
		x_plus = 0;
		y_plus = 1;
	}

	if (x_plus != 0 || y_plus != 0)
	{	
		short NewX, NewY;
		NewX = npc->X + x_plus; NewY = npc->Y + y_plus;
		
		short nx, ny;
		nx = NewX - npc->oX;
		ny = NewY - npc->oY;
		
		if (nx < 0) nx *= -1;
		if (ny < 0) ny *= -1;

		if (nx > NPCTypes[ npc->Type ].WalkingRange) return;
		if (ny > NPCTypes[ npc->Type ].WalkingRange) return;

		int Error = 0;

		if (!IsValid(NewX, NewY)) { Error = 1; }
	
		short hshx, hshy;
		hshx = NewX / 64;
		hshy = NewY / 64;

		mon2_List = Monsters[hshx][hshy];
        
		while (mon2_List != NULL)
		{
			if (mon2_List->X == NewX && mon2_List->Y == NewY)
				return;
			
			if (mon2_List != NULL)
				mon2_List = mon2_List->next;		
		}

		npc2_List = NPC[hshx][hshy];

		while (npc2_List != NULL)
		{
            if (npc2_List->X == NewX && npc2_List->Y == NewY &&
				npc->ID != npc2_List->ID)
				return;
		
			if (npc2_List != NULL)
				npc2_List = npc2_List->next;
		}

		obj_List = Objects[hshx][hshy];

		while (obj_List != NULL)
		{
			if (obj_List->X == NewX && obj_List->Y == NewY)
				return;

			if (obj_List != NULL)
				obj_List = obj_List->next;
		}

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				if (Clients[j].uChar->X == NewX && Clients[j].uChar->Y == NewY && !Clients[j].Inv)
					return;
			}
		}

		if (Error == 1) return;

		if (npc->X / 64 != NewX / 64 || npc->Y / 64 != NewY / 64) 
			NPC_MoveList(npc, NewX, NewY);

		npc->X += x_plus;
		npc->Y += y_plus;

		PACKET_COORDINATES NewCoords;
		memset(&NewCoords, 0, sizeof(NewCoords));

		NewCoords.X = npc->X;
		NewCoords.Y = npc->Y;
		NewCoords.ID = npc->ID;


		buffer[0] = 20;
		buffer[1] = 5;
	
		NewNPC.HP = npc->HPLeft;
		NewNPC.ID = npc->ID;
		NewNPC.X = npc->X;
		NewNPC.Y = npc->Y;
		NewNPC.conv_id = npc->conv_id;
		NewNPC.type = npc->Type;
		memcpy(&buffer[2], &NewNPC, sizeof(NewNPC));

		buffer2[0] = 20;
		buffer2[1] = 6;

		memcpy(&buffer2[2], &NewCoords, sizeof(NewCoords));

		short ox,oy,tx,ty;
		ox = npc->X;
		oy = npc->Y;
                
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				tx = Clients[j].uChar->X;
				ty = Clients[j].uChar->Y;


				if (!Clients[j].KnowNPC[npc->ID] && IsInRange(ox, oy, tx, ty))
				{
					Clients[j].KnowNPC[npc->ID] = true;
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);
				}

				if (Clients[j].KnowNPC[npc->ID])
				{
					send(Clients[j].ClientSocket, buffer2, 2 + sizeof(NewCoords), 0);
					if (!IsInRange(ox, oy, tx, ty))
						Clients[j].KnowNPC[npc->ID] = false;
				}
			}	
		}
	}
}

void NPC_MoveList( NPCClass *npc, short newx, short newy)
{
	short cur_hashx, cur_hashy, new_hashx, new_hashy;

	cur_hashx = npc->X / 64;
	cur_hashy = npc->Y / 64;

	new_hashx = newx / 64;
	new_hashy = newy / 64;

	npc2_List = NPC[cur_hashx][cur_hashy];

	NPCClass *tmptmp;
	tmptmp = NULL;

	if (npc2_List == npc)
	{
		NPC[cur_hashx][cur_hashy] = NPC[cur_hashx][cur_hashy]->next;
	}
	else
	{
		while (npc2_List != NULL)
		{
	        if (npc2_List == npc)
			{
				tmptmp->next = npc2_List->next;
				npc2_List = NULL;
			}
		
			if (npc2_List != NULL)
			{	
				tmptmp = npc2_List;
				npc2_List = npc2_List->next;
			}
		}
	}

	npc->next = NPC[new_hashx][new_hashy];
	NPC[new_hashx][new_hashy] = npc;
}


void NPCAction_Move( NPCClass *npc, short tx, short ty)
{
	if (MoveNPC(npc, tx, ty))
	{
		// Monster successfully moved, inform players.

		PACKET_COORDINATES NewCoords;
		memset(&NewCoords, 0, sizeof(NewCoords));

		NewCoords.X = npc->X;
		NewCoords.Y = npc->Y;
		NewCoords.ID = npc->ID;


		buffer[0] = 20;
		buffer[1] = 5;

		memset(&NewNPC, 0, sizeof(NewNPC));

		NewNPC.HP = npc->HPLeft;
		NewNPC.ID = npc->ID;
		NewNPC.X = npc->X;
		NewNPC.Y = npc->Y;
		NewNPC.conv_id = npc->conv_id;
		NewNPC.type = npc->Type;
		memcpy(&buffer[2], &NewNPC, sizeof(NewNPC));

		buffer2[0] = 20;
		buffer2[1] = 6;

		memcpy(&buffer2[2], &NewCoords, sizeof(NewCoords));

		short ox,oy,tx,ty;
		ox = npc->X;
		oy = npc->Y;

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				tx = Clients[j].uChar->X;
				ty = Clients[j].uChar->Y;


				if (!Clients[j].KnowNPC[npc->ID] && IsInRange(ox, oy, tx, ty))
				{
					Clients[j].KnowNPC[npc->ID] = true;
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewNPC), 0);
				}

				if (Clients[j].KnowNPC[npc->ID])
				{
					send(Clients[j].ClientSocket, buffer2, 2 + sizeof(NewCoords), 0);
					if (!IsInRange(ox, oy, tx, ty))
						Clients[j].KnowNPC[npc->ID] = false;
				}
			}	
		}
	}
}

bool MoveNPC(NPCClass *npc, short ox, short oy)
{
	char xx, yy;
	int Error = 0;

	int tx, ty;

	tx = npc->X;
	ty = npc->Y;
	
	// Remember old coords.

	short OldX = npc->X;
	short OldY = npc->Y;

	// First step, check the direction that monster should go to get near player.

	if (ox > tx && oy > ty)
	{
	  	xx = 1;
	    yy = 1;
	}
	if (ox < tx && oy < ty)
	{
		xx = -1;
	   	yy = -1;
	}
	if (ox < tx && oy > ty)
	{
		xx = -1;
		yy = 1;
	}
	if (ox > tx && oy < ty)
	{
		xx = 1;
	  	yy = -1;
	}
	if (ox > tx && oy == ty)
	{
		xx = 1;
	    yy = 0;
	}
	if (ox < tx && oy == ty)
	{
		xx = -1;
		yy = 0;
	}
	if (ox == tx && oy > ty)
	{
	  	xx = 0;
	    yy = 1;
	}
	if (ox == tx && oy < ty)
	{
		xx = 0;
	   	yy = -1;
	}

	// Save possible new coordinates.
	
	int NewX, NewY;
	
	NewX = tx + xx;
	NewY = ty + yy;

	// Check if monster can move to this tile, if not, then run step 2.
	
	if (!IsValid(NewX, NewY)) { Error = 1; }

	// Check if monster is trying to move onto other monster

	short hshx, hshy;
	hshx = NewX / 64;
	hshy = NewY / 64;

	mon2_List = Monsters[hshx][hshy];
        
	while (mon2_List != NULL)
	{
		if (mon2_List->X == NewX && mon2_List->Y == NewY)
		{
			Error = 1;
			mon2_List = NULL;
		}
		
		if (mon2_List != NULL)
			mon2_List = mon2_List->next;		
	}

	npc2_List = NPC[hshx][hshy];

	while (npc2_List != NULL)
	{
		if (npc2_List->X == NewX && npc2_List->Y == NewY &&
			npc->ID != npc2_List->ID)
		{
			Error = 1;
			npc2_List = NULL;
		}
	
		if (npc2_List != NULL)
			npc2_List = npc2_List->next;
	}

	obj_List = Objects[hshx][hshy];
	
	while (obj_List)
	{
		if (obj_List->X == NewX && obj_List->Y == NewY && obj_List->On)
		{
			if (obj_List->use(-1))
				Error = 2;
			else Error = 1;

			obj_List = NULL;
		}

		if (obj_List != NULL) obj_List = obj_List->next;
	}

	int j;
	for (j = 0; j < Max_Connections; j++)
	{
		if (Clients[j].Used && Clients[j].State != 5)
		{
			if (Clients[j].uChar->X == NewX && Clients[j].uChar->Y == NewY && !Clients[j].Inv)
			{
				Error = 1;
				j = Max_Connections;
			}
		}
	}

	//--------
	// Step 2
	//--------

	if (Error == 1)
	{
		int MoveDir;
		int CheckDir;
		Error = 0;

		// Check where monster tried to move in Step 1
	
		CheckDir = Check_dir(xx, yy);

		// Randomize where monster will try to move in Step 2
		MoveDir = rand() % 3;

		// Monster won't move, so drop it.
		if (MoveDir == 0) { return false; }

		// Monster will move

		else
		{
			NewX = tx;
			NewY = ty;


			if (CheckDir == 7) // Monsut yrittää liikkua ylös tai vasemmalle
			{
				if (MoveDir == 1) // Monsut yrittää liikkua ylös
				{NewY--; NewX = OldX;}
				if (MoveDir == 2) // Monsut yrittää liikkua vasemmalle
				{NewX--; NewY = OldY;}
			}
			if (CheckDir == 9) // Monsut yrittää liikkua ylös tai oikealle
			{
				if (MoveDir == 1) // Monsut yrittää liikkua ylös
				{NewY--; NewX = OldX;}
				if (MoveDir == 2) // Monsut yrittää liikkua oikealle
				{NewX++; NewY = OldY;}
			}
			if (CheckDir == 1) // Monsut yrittää liikkua alas tai vasemmalle
			{
				if (MoveDir == 1) // Monsut yrittää liikkua alas
				{NewY++; NewX = OldX;}
				if (MoveDir == 2) // Monsut yrittää liikkua vasemmalle
				{NewX--; NewY = OldY;}
			}
			if (CheckDir == 3) // Monsut yrittää liikkua alas tai oikealle
			{
				if (MoveDir == 1) // Monsut yrittää liikkua alas
				{NewY++; NewX = OldX;}
				if (MoveDir == 2) // Monsut yrittää liikkua oikealle
				{NewX++; NewY = OldY;}
			}
			if (CheckDir == 4) // Monsut yrittää liikkua ylös tai alas
			{
				if (MoveDir == 1) // Monsut yrittää liikkua ylös
				{NewY--; NewX = OldX;}
				if (MoveDir == 2) // Monsut yrittää liikkua alas
				{NewY++; NewX = OldX;}
			}
			if (CheckDir == 8) // Monsut yrittää liikkua vasemmalle tai oikealle
			{
				if (MoveDir == 1) // Monsut yrittää liikkua vasemmalle
				{NewX--; NewY = OldY;}
				if (MoveDir == 2) // Monsut yrittää liikkua oikealle
				{NewX++; NewY = OldY;}
			}
		
			// Check if monster is trying to move onto other monster
	
			short hshx, hshy;
			hshx = NewX / 64;
			hshy = NewY / 64;
	
			mon2_List = Monsters[hshx][hshy];
	        
			while (mon2_List != NULL)
			{
				if (mon2_List->X == NewX && mon2_List->Y == NewY)
					return false;
				
				if (mon2_List != NULL)
					mon2_List = mon2_List->next;		
			}
	
			npc2_List = NPC[hshx][hshy];
	
			while (npc2_List != NULL)
			{
				if (npc2_List->X == NewX && npc2_List->Y == NewY)
					return false;
			
				if (npc2_List != NULL)
					npc2_List = npc2_List->next;
			}
		
			obj_List = Objects[hshx][hshy];
		
			while (obj_List != NULL)
			{
				if (obj_List->X == NewX && obj_List->Y == NewY)
					return false;
			
				if (obj_List != NULL)
					obj_List = obj_List->next;
			}

			for (j = 0; j < Max_Connections; j++)
			{
				if (Clients[j].Used && Clients[j].State != 5)
				{
					if (Clients[j].uChar->X == NewX && Clients[j].uChar->Y == NewY && !Clients[j].Inv)
						return false;
				}
			}
		}
	}

	if (!IsValid(NewX, NewY)) { Error = 1; }
	if (Error == 0) 
	{ 
		if (npc->X / 64 != NewX / 64 || npc->Y / 64 != NewY / 64) 
			NPC_MoveList(npc, NewX, NewY);

		npc->X = NewX; 
		npc->Y = NewY; 
		return true;}
	else
	{ return false; }
}

void CheckNPCTarget( NPCClass *npc )
{
	short ox, oy, tx, ty;
	tx = npc->X;
	ty = npc->Y;

	bool TargetFound;
		 TargetFound = false;

	int j;
	for (j = 0; j < Max_Connections; j++)
	{
		if (Clients[j].Used && Clients[j].State != 5)
		{
			ox = Clients[j].uChar->X;
			oy = Clients[j].uChar->Y;
			if (IsInRange(ox, oy, tx, ty) && Clients[j].uChar->DevMode < 1 && Clients[j].uChar->Wanted)
			{
				npc->NPCPLTarget = Clients[j].Acc->getAccID();
				npc->NPCMonsterTarget = NULL;
				npc->LstAttack = GetTickCount();
				TargetFound = true;
			    j = Max_Connections;
			}
			else if (IsInRange(ox, oy, tx, ty) && Clients[j].uChar->CrimCount >= 20 && Clients[j].uChar->DevMode < 1 && !Clients[j].uChar->Wanted)
			{
				JailPlayer(j);
				npc->LstAttack = GetTickCount();
				TargetFound = true;
			    j = Max_Connections;
			}
		}
	}

	if (!TargetFound)
	{
		short hshx[9], hshy[9];
		hshx[0] = tx / 64;
		hshy[0] = ty / 64;

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

		MonsterClass *nearest = NULL;
		short smallestam = 10000, calculaatio = 0;

		int i;
		for (i = 0; i < 9; i++)
		{
			if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
			{
				mon2_List = Monsters[hshx[i]][hshy[i]];
	
				while (mon2_List != NULL)
				{
					ox = mon2_List->X;
					oy = mon2_List->Y;
					if (IsInRange(ox, oy, tx, ty))
					{
						calculaatio = ox - tx + oy - ty;
						if (calculaatio < 0) calculaatio *= -1;

						if (calculaatio < smallestam)
						{
							nearest = mon2_List;
							smallestam = calculaatio;
						}
					}	

					if (mon2_List != NULL)
						mon2_List = mon2_List->next;	
				}
			}			
		}

		if (nearest != NULL)
		{
			npc->NPCMonsterTarget = nearest;
			npc->NPCPLTarget = -1;
			npc->LstAttack = GetTickCount();
		}
	}
}

void JailPlayer(short This)
{
	Clients[This].IsInJail = true;
	Clients[This].uChar->CrimCount *= -1;
	Clients[This].LstJailtime = GetTickCount();
	
	int ox, oy;
	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	Clients[This].uChar->X = 296;
	Clients[This].uChar->Y = 795;
	

	PACKET_CHATMESSAGE NewPacket;

	memset(&NewPacket, 0, sizeof(NewPacket));


	NewPacket.MsgType = 1;

	sprintf(NewPacket.Message, "ÆGuard: ï%sÆ, you will come with me!", Clients[This].uChar->Name);

	buffer[0] = 9;
	buffer[1] = 0;
	
	memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));


	int tx, ty, i;

	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			tx = Clients[i].uChar->X;
			ty = Clients[i].uChar->Y;
	
			if ((ox - tx >= -10 && ox - tx <= 10 &&
				oy - ty >= -10 && oy - ty <= 10) || Clients[i].ListenNormal)
			{
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);
			}
		}
	}

	sprintf(NewPacket.Message, "You have commited %d crimes, you are jailed for %d minutes.", Clients[This].uChar->CrimCount * -1, Clients[This].uChar->CrimCount * -1);

	buffer[0] = 9;
	buffer[1] = 0;
	
	memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);


	PACKET_COORDINATES NewCoords;
	
	NewCoords.ID = Clients[This].Acc->getAccID();
	NewCoords.X = Clients[This].uChar->X;
	NewCoords.Y = Clients[This].uChar->Y;

	// Tell player about new cords
		buffer[0] = 10;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

		CheckPlayersOnScreen(This, NewCoords);
		CheckItemsOnScreen(This);
		CheckMonstersOnScreen(This);
		CheckNPCsOnScreen(This);

	buffer[0] = 10;
	buffer[1] = 2;
	memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (i != This)
			{
				tx = Clients[i].uChar->X;
				ty = Clients[i].uChar->Y;

				if (IsInRange(ox, oy, tx, ty))
				{
					// Send only coords.
					    
					send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);
	
					if (!IsInRange(ox, oy, tx, ty))
					{
						Clients[i].KnowPlayer[ Clients[This].Know_ID] = false;
						Clients[This].KnowPlayer[ Clients[i].Know_ID] = false;
					}
				}
			}
		}
	}
}

void ReleaseFromJail( short This)
{
	Clients[This].IsInJail = false;	

	Clients[This].uChar->Wanted = false;

	InformMassofNewCriminalStatus(This);

	PACKET_CHATMESSAGE NewPacket;

	memset(&NewPacket, 0, sizeof(NewPacket));

	NewPacket.MsgType = 2;

	sprintf(NewPacket.Message, "You have been released from jail!", Clients[This].uChar->Name);

	buffer[0] = 9;
	buffer[1] = 0;
	
	memcpy(&buffer[2], &NewPacket, sizeof(NewPacket));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewPacket), 0);

	Clients[This].uChar->X = 280;
	Clients[This].uChar->Y = 780;

	PACKET_COORDINATES NewCoords;
	
	NewCoords.ID = Clients[This].Acc->getAccID();
	NewCoords.X = Clients[This].uChar->X;
	NewCoords.Y = Clients[This].uChar->Y;

		// Tell player about new cords
		buffer[0] = 10;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

		CheckPlayersOnScreen(This, NewCoords);
		CheckItemsOnScreen(This);
		CheckMonstersOnScreen(This);
		CheckNPCsOnScreen(This);

	buffer[0] = 10;
	buffer[1] = 2;
	memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);
}

void InformPlayerofNewCrimCounts(short This)
{
	buffer[0] = 30;
	buffer[1] = 7;
	memcpy(&buffer[2], &Clients[This].uChar->CrimCount, sizeof(Clients[This].uChar->CrimCount));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(Clients[This].uChar->CrimCount), 0);	
}

void NPCAction_AttackToPlayer( NPCClass *npc, ClientClass *Target )
{
	bool ToHit = false;


	int ToHitRoll =0;
	int ToHitDefence =0;
	int ToHitRollDefence =400;			

	ToHitRoll = 800;

	ToHitDefence = 3 * Target->Acc->tChar.Dex;

	ToHitRollDefence -= ToHitDefence;

	if (ToHitRollDefence <= 0) ToHitRollDefence = 1;

	if (ToHitRoll > rand() % ToHitRollDefence)
	{
		ToHit = true;	
	}

	if (rand() % 100 <= 25)
		ToHit = true;


	short ox, oy, tx, ty;
	tx = Target->uChar->X;
	ty = Target->uChar->Y;
	memset(&NewPic, 0, sizeof(NewPic));

	if (ToHit)
	{
		// We hit the target.

		// Check Damage

		short Damage;
		//short Reduce; edited by Radiankh

		Damage = rand() % (NPCTypes[npc->Type].dam_max - NPCTypes[npc->Type].dam_min);
		Damage += NPCTypes[npc->Type].dam_min;


		if (Clients[Target->AccountID].is_logging)
		{
			Clients[Target->AccountID].is_logging = false;
			Clients[Target->AccountID].is_logging = true;

			Clients[Target->AccountID].LstLogTime = 0;
			Clients[Target->AccountID].LstLogTime = GetTickCount(); // Change targets logout timer
		}

		// Send new hit pic to players
		
		NewPic.Pic = 0;
		NewPic.X = Target->uChar->X;
		NewPic.Y = Target->uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				ox = Clients[j].uChar->X;
				oy = Clients[j].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
				}	
			}
		}

		// Send combat text to target player

		memset(&NewCT, 0, sizeof(NewCT));

		buffer[0] = 97;
		buffer[1] = 1;

		NewCT.Amount = Damage;
		NewCT.Type = 22;
		NewCT.FromTo = npc->Type;
		NewCT.SkillType = 1;

		memcpy(&buffer[2], &NewCT, sizeof(NewCT));

		send(Target->ClientSocket, buffer, 2 + sizeof(NewCT), 0);

        // Reduce player's HP

		Target->uChar->HPLeft -= Damage;

		if ( Target->uChar->HPLeft <= 0)
		{
			// Player died.

			DeathPenalty(Target);

			short hshx[9], hshy[9]; 
			hshx[0] = tx / 64; 
			hshy[0] = ty / 64; 
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
	
			int i;
			for (i = 0; i < 9; i++)
			{
				if (hshx[i] >= 0 && hshy[i] > 0 && hshx[i] < 128 && hshy[i] < 128)
				{
					npc2_List = NPC[hshx[i]][hshy[i]];
	
					while (npc2_List != NULL)
					{
						if (npc2_List->NPCPLTarget == Target->Acc->getAccID())
							npc2_List->NPCPLTarget = -1;

						if (npc2_List != NULL)
							npc2_List = npc2_List->next;
					}
				}			
			}

			if (Target->uChar->Wanted)
			{//1435,4699
//				if (Target->uChar->CrimCount >= 20				

				Clients[Target->Know_ID].CrimDeath();

//				Target->uChar->ShrineX = 1435;
//				Target->uChar->ShrineY = 4699;
			}

			short oldx, oldy;

			short newx, newy;

			oldx = Target->uChar->X;
			oldy = Target->uChar->Y;

			newx = Target->uChar->ShrineX;
			newy = Target->uChar->ShrineY;

			CreateBody(Target->Know_ID, oldx, oldy);

			bool jailed = false;
			if (Target->uChar->Wanted && 
				Target->uChar->CrimCount >= 20)
			{
				JailPlayer(Target->Know_ID);
				jailed = true;
			}

		
			Target->NPCTarget = NULL;
			Target->PLTarget = -1;
			Target->MonTarget = NULL;

			npc->NPCPLTarget = -1;
			

			Target->uChar->HPLeft = Target->Acc->tChar.HPMax * 0.75;

			if (!jailed)
			{
				Target->uChar->Wanted = false;
				InformMassofNewCriminalStatus( Target->Know_ID);

				Target->uChar->X = newx;
				Target->uChar->Y = newy;

				PACKET_COORDINATES NewCoords;
				memset(&NewCoords, 0, sizeof(NewCoords));
				NewCoords.X = newx;
				NewCoords.Y = newy;
				NewCoords.ID = Target->Acc->getAccID();


				buffer[0] = 10;
				buffer[1] = 1;
				memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
				send(Target->ClientSocket, buffer, 2 + sizeof(NewCoords), 0);


				CheckPlayersOnScreen(Target->Know_ID, NewCoords);
				CheckItemsOnScreen(Target->Know_ID);
				CheckMonstersOnScreen(Target->Know_ID);
				CheckNPCsOnScreen(Target->Know_ID);
			}

			buffer[0] = 99;
			buffer[1] = 6;
			memcpy(&buffer[2], &npc->Type, sizeof(npc->Type));
			send(Target->ClientSocket, buffer, 2 + sizeof(npc->Type), 0);			
		}

		// Inform player of new HP

		memset(&NewHP, 0, sizeof(NewHP));

		NewHP.KnowID = Target->Acc->getAccID();
		NewHP.NewHP = Target->uChar->HPLeft;
		NewHP.Type = 0;

		buffer[0] = 20;
		buffer[1] = 3;

		memcpy(&buffer[2], &NewHP, sizeof(NewHP));

		ox = Target->uChar->X;
		oy = Target->uChar->Y;

		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				tx = Clients[j].uChar->X;
				ty = Clients[j].uChar->Y;
				if (IsInRange(ox, oy, tx, ty))
				{
					if (Clients[j].KnowPlayer[Target->Know_ID] || &Clients[j] == Target)
						send(Clients[j].ClientSocket, buffer, 2 + sizeof( NewHP), 0);
			
				}
			}
		}
	}
	else
	{
		// Missed.

		// Send miss pic to players

		NewPic.Pic = 1;
		NewPic.X = Target->uChar->X;
		NewPic.Y = Target->uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				ox = Clients[j].uChar->X;
				oy = Clients[j].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewPic), 0);
				}
			}
		}
	}
}

void DeathPenalty(ClientClass *Target)
{
	unsigned long PenaltyAmount = 0;
//	LevelUpExp = Target->uChar->Exp - ExpTables[Target->uChar->Level];
//	if (LevelUpExp == 0) return;
//	PenaltyAmount = 0.01 * Target->uChar->Exp;

	if (Target->uChar->Level < 50) return;

	if (Target->uChar->Level >= 50 &&
		Target->uChar->Level < 100)
		PenaltyAmount = 5000;

	if (Target->uChar->Level >= 100 &&
		Target->uChar->Level < 150)
		PenaltyAmount = 10000;

	if (Target->uChar->Level >= 150 &&
		Target->uChar->Level < 175)
		PenaltyAmount = 15000;

	if (Target->uChar->Level >= 175 &&
		Target->uChar->Level <= 200)
		PenaltyAmount = 1000;

	if (Target->uChar->Exp - PenaltyAmount < ExpTables[Target->uChar->Level])
		PenaltyAmount = Target->uChar->Exp - ExpTables[Target->uChar->Level];
	
	Target->uChar->Exp -= PenaltyAmount;
	
	buffer[0] = 99;
	buffer[1] = 20;
	memcpy(&buffer[2], &PenaltyAmount, sizeof(PenaltyAmount));
	send(Target->ClientSocket, buffer, 2 + sizeof(PenaltyAmount), 0);

	buffer[0] = 11;
	buffer[1] = 2;
	memcpy(&buffer[2], &Target->uChar->Exp, sizeof(Target->uChar->Exp));
	send(Target->ClientSocket, buffer, 2 + sizeof(Target->uChar->Exp), 0);
}


void NPCAction_AttackToMonster( NPCClass *npc, MonsterClass *Target )
{
	bool ToHit = false;


	int ToHitRoll =0;
	int ToHitDefence =0;
	int ToHitRollDefence =400;			

	ToHitRoll = 800;

	ToHitDefence = 3 * MonsterTypes[ Target->Type ].Dex;

	ToHitRollDefence -= ToHitDefence;

	if (ToHitRoll > rand() % ToHitRollDefence)
	{
		ToHit = true;	
	}

	if (rand() % 100 <= 25)
		ToHit = true;


	short ox, oy, tx, ty;
	tx = Target->X;
	ty = Target->Y;
	memset(&NewPic, 0, sizeof(NewPic));

	if (ToHit)
	{
		// We hit the target.

		// Check Damage

		short Damage, Reduce;

		Damage = rand() % (NPCTypes[npc->Type].dam_max - NPCTypes[npc->Type].dam_min);
		Damage += NPCTypes[npc->Type].dam_min;

		Reduce = Damage * (1 - (float)MonsterTypes[Target->Type].AC/600);

		Reduce = Damage - Reduce;
		if (Reduce > Damage) Reduce = Damage-1;
		if (Reduce < 0) Reduce = 0;

		Damage -= Reduce;
		

		// Send new hit pic to players
		
		NewPic.Pic = 0;
		NewPic.X = Target->X;
		NewPic.Y = Target->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				ox = Clients[j].uChar->X;
				oy = Clients[j].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
				}	
			}
		}

		if (Target->NPCTarget != NULL || Target->Target != NULL)
		{
			if (Target->Target != NULL)
			{
				short xx, yy, x2, y2;
				xx = Target->X;
				yy = Target->Y;
				x2 = Target->Target->uChar->X;
				y2 = Target->Target->uChar->X;

				if (xx - x2 < -1 || xx - x2 > 1 || yy - y2 < -1 || yy - y2 > 1)
				{
					Target->Target = NULL;
					Target->NPCTarget = npc;
				}
			}
			else if (Target->NPCTarget != NULL)
			{
				short xx, yy, x2, y2;
				xx = Target->X;
				yy = Target->Y;
				x2 = Target->NPCTarget->X;
				y2 = Target->NPCTarget->Y;

				if ((xx - x2 < -1 || xx - x2 > 1 || yy - y2 < -1 || yy - y2 > 1) && Target->NPCTarget != npc)
				{
					Target->Target = NULL;
					Target->NPCTarget = npc;
				}
			}
		}

		Target->HPLeft -= Damage;

		if (Target->HPLeft > 0)				
		{
			// Send new amount of HP to all clients in visual range.

			InformNewMonsterHP( Target );
		}


		if (Target->HPLeft <= 0)
		{
              
			// Inform about dead monster.

			buffer[0] = 20;
			buffer[1] = 2;

			memcpy(&buffer[2], &Target->KnowID, sizeof(Target->KnowID));

			for (j = 0; j < Max_Connections; j++)
			{
				if (Clients[j].Used && Clients[j].State != 5)
				{
					if (Clients[j].KnowMonster[Target->KnowID])
					{
						Clients[j].KnowMonster[Target->KnowID] = false;
						send(Clients[j].ClientSocket, buffer, 2 + sizeof(Target->KnowID), 0);
					}
					if (Clients[j].MonTarget == Target)
						Clients[j].MonTarget = NULL;
				}
			}

			short hshx[9], hshy[9]; hshx[0] = tx / 64; hshy[0] = ty / 64; hshx[1] = hshx[0] -1; hshx[1] = hshy[0] -1; hshx[2] = hshx[0] -1; hshx[2] = hshy[0]; hshx[3] = hshx[0] -1; hshx[3] = hshy[0] +1; hshx[4] = hshx[0]; hshx[4] = hshy[0] -1; hshx[5] = hshx[0]; hshy[5] = hshy[0] +1; hshx[6] = hshx[0] +1; hshy[6] = hshy[0] -1;  hshx[7] = hshx[0] +1; hshy[7] = hshy[0]; hshx[8] = hshx[0] +1; hshy[8] = hshy[0] +1; 
	
			int i;
			for (i = 0; i < 9; i++)
			{
				if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
				{
					npc2_List = NPC[hshx[i]][hshy[i]];
	
					while (npc2_List != NULL)
					{
						if (npc2_List->NPCMonsterTarget == Target)
						{
							npc2_List->NPCMonsterTarget = NULL;
						}
	
						if (npc2_List != NULL)
							npc2_List = npc2_List->next;
					}
				}			
			}
//			npc->NPCMonsterTarget = NULL;
			npc->NPCPLTarget =  -1;
			KillMonster( Target );
		}
	}
	else
	{
		// Missed.

		// Send miss pic to players

		NewPic.Pic = 1;
		NewPic.X = Target->X;
		NewPic.Y = Target->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

		int j;
		for (j = 0; j < Max_Connections; j++)
		{
			if (Clients[j].Used && Clients[j].State != 5)
			{
				ox = Clients[j].uChar->X;
				oy = Clients[j].uChar->Y;
	
				if (IsInRange(ox, oy, tx, ty))
				{
					send(Clients[j].ClientSocket, buffer, 2 + sizeof(NewPic), 0);
				}
			}
		}
	}
}

NPCClass *GetNPCByID(short ID)
{
	int i,j;
	for (i = 0; i < 128; i++)
	{
		for (j = 0; j < 128; j++)
		{
			npc2_List = NPC[i][j];

			while (npc2_List != NULL)
			{
				if (npc2_List->ID == ID)
					return npc2_List;
				
                if (npc2_List != NULL)
					npc2_List = npc2_List->next;
			}
		}
	}

	printf("%s: Error at [GetNPCByID]: No NPC found by ID %d\n", Time, ID);
	return NULL;
}























