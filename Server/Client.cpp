
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

extern unsigned long ExpTables[MaxLevel];
extern unsigned long SkillExpTables[MaxLevel];

extern PACKET_MONSTER		NewMonster;
extern PACKET_NPC			NewNPC;
extern PACKET_STATS			NewStats;
extern PACKET_WEIGHTANDAC	NewWeight;
//extern PACKET_STATS			NewStats; Check this later
extern PACKET_BODYINFOSTRUCT NewBodyInfo;
extern PACKET_NEWRESISTANCE NewRes;

extern MiscPicClass			NewPic;
extern NewCombatTextClass	NewCT;
extern NewHPClass			NewHP;

extern char *buffer;
extern char *buffer2;
extern char *buffer3;

extern short LastCreated;

extern short SafeZones[512][512];


extern GuildClass Guilds[MaxGuilds];

extern int ClientsConnected;

extern BodyClass Body[m_Bodies];

extern PACKET_BODY NewBody;
extern PACKET_NEWDECAY NewDecay;

extern bool Debug;
extern unsigned long GroundTime[];

extern SecureTradeClass SecureTrade[];


//-----------------------------------------------------------------
//
void InformPlayerofHisAllItems(short This)
{
	int S = Clients[This].State;
	int it;

	// Backpack

	int i;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (Clients[This].uChar->Backpack[i].ID != 0)
		{
			it = Clients[This].uChar->Backpack[i].ID;

			if (Items[it].Type == 0 || Items[it].Family == 0)
			{
				printf("Istant!!!!\n");
				memset(&Items[it], 0, sizeof(Items[it]));
				Clients[This].uChar->Backpack[i].ID = 0;
			}
			else
			{
				Items[it].KnowID = i;
				if (Items[it].x >= 120) Items[it].x = rand() % 120;
				if (Items[it].x < 0)	Items[it].x = rand() % 120;
				if (Items[it].y >= 120) Items[it].y = rand() % 100;
				if (Items[it].y < 0)	Items[it].y = rand() % 100;
			
				buffer[0] = 12;
				buffer[1] = 2;
	
			    memcpy(&buffer[2], &Items[it], sizeof(Items[it]));
	
				send(Clients[This].ClientSocket, buffer, 2 + sizeof(Items[it]), 0);
			}
		}
	}


	for (i = 0; i < MaxWearedItems; i++)
	{
		if (Clients[This].uChar->Worn[i].ID != 0)
		{
			Items[Clients[This].uChar->Worn[i].ID].KnowID = i;
			buffer[0] = 12;
			buffer[1] = 3;

			memcpy(&buffer[2], &Items[Clients[This].uChar->Worn[i].ID], sizeof(Items[Clients[This].uChar->Worn[i].ID]));

	        send(Clients[This].ClientSocket, buffer, 2 + sizeof(Items[Clients[This].uChar->Worn[i].ID]), 0);
		}
	}

	// Bank

	for (i = 0; i < MaxBankItems; i++)
	{
		if (Clients[This].uChar->Bank[i].ID != 0)
		{
			it = Clients[This].uChar->Bank[i].ID;
			Items[it].KnowID = i;
			if (Items[it].x > 245) Items[it].x = rand() % 245;
			if (Items[it].x < 0)	Items[it].x = rand() % 245;
			if (Items[it].y >= 120) Items[it].y = rand() % 100;
			if (Items[it].y < 0)	Items[it].y = rand() % 100;

			buffer[0] = 12;
			buffer[1] = 4;
	
			memcpy(&buffer[2], &Items[it], sizeof(Items[it]));
	
			send(Clients[This].ClientSocket, buffer, 2 + sizeof(Items[it]), 0);
		}
	}
}
//-----------------------------------------------------------------


short Client_GetID(short ID)
{
	for (int i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
			if (Clients[i].Acc->getAccID() == ID)
				return i;
	}
	return -1;
}


//-----------------------------------------------------------------
//
void CheckStats(short This)
{
	Clients[This].Acc->getStats();


	// v  Old style, before 0.32
/*	unsigned short TmpAC = 0, TmpWeight = 0, TmpDamMin = 0, TmpDamMax = 0;


	int S = Clients[This].State;

	Clients[This].Cha = Accounts[Clients[This].AccountID].Char[S].Cha;
	Clients[This].Str = Accounts[Clients[This].AccountID].Char[S].Str;
	Clients[This].Lck = Accounts[Clients[This].AccountID].Char[S].Lck;
	Clients[This].Con = Accounts[Clients[This].AccountID].Char[S].Con;
	Clients[This].Dex = Accounts[Clients[This].AccountID].Char[S].Dex;
	Clients[This].Int = Accounts[Clients[This].AccountID].Char[S].Int;

	Clients[This].prot_Air = 0;
	Clients[This].prot_Fire = 0;
	Clients[This].prot_Earth = 0;
	Clients[This].prot_Water = 0;


	long item;

	int i, am;
	for (i = 0; i < MaxWornItems; i++)
	{
		if (Accounts[Clients[This].AccountID].Char[S].Worn[i] != 0)
		{
			item = Accounts[Clients[This].AccountID].Char[S].Worn[i];
			if (Items[item].Family == 2)
			{
				TmpAC += Armors[Items[item].Type-11][Items[item].ID].AC;
				TmpWeight += Armors[Items[item].Type-11][Items[item].ID].Weight;

				if (Armors[Items[item].Type-11][Items[item].ID].Speciality != 0)
				{
					am =    Armors[Items[item].Type-11][Items[item].ID].SpecialityAmount;
			        switch (Armors[Items[item].Type-11][Items[item].ID].Speciality)
					{
					case boost_str:
						Clients[This].Str += am;						
						break;
					case boost_dex:
						Clients[This].Dex += am;						
						break;
					case boost_con:
						Clients[This].Con += am;						
						break;
					case boost_lck:
//						Clients[This].Lck += am;						
						break;
					case boost_int:
						Clients[This].Int += am;						
						break;
					case boost_cha:
//						Clients[This].Cha += am;						
						break;
					}
				}
				if (Armors[Items[item].Type-11][Items[item].ID].ElementalProtection != 0)
				{
					am =    Armors[Items[item].Type-11][Items[item].ID].ElementalProtectionAmount;
			        switch (Armors[Items[item].Type-11][Items[item].ID].ElementalProtection)
					{
					case elem_FIRE:

						Clients[This].prot_Fire += am;
						
						break;
					case elem_AIR:

						Clients[This].prot_Air += am;
						
						break;
					case elem_EARTH:

						Clients[This].prot_Earth += am;
						
						break;
					case elem_WATER:

						Clients[This].prot_Water += am;
						
						break;
					}
				}
			}
			if (Items[item].Family == FAMILY_WEAPON)
			{
				TmpWeight += Weapons[Items[item].Type][Items[item].ID].Weight;
				if (Weapons[Items[item].Type][Items[item].ID].Speciality != 0)
				{
					am =    Weapons[Items[item].Type][Items[item].ID].SpecialityAmount;
			        switch (Weapons[Items[item].Type][Items[item].ID].Speciality)
					{
					case boost_str:
						Clients[This].Str += am;						
						break;
					case boost_dex:
						Clients[This].Dex += am;						
						break;
					case boost_con:
						Clients[This].Con += am;						
						break;
					case boost_lck:
//						Clients[This].Lck += am;						
						break;
					case boost_int:
						Clients[This].Int += am;						
						break;
					case boost_cha:
//						Clients[This].Cha += am;						
						break;
					}
				}
			}
		}
	}


	// Backpack

	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (Accounts[Clients[This].AccountID].Char[S].Backpack[i] > 0)
		{
			if (Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Family == FAMILY_WEAPON)
			{
				TmpWeight += Weapons[Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Type][Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].ID].Weight;
			}
			if (Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Family == FAMILY_ARMOR)
			{
				TmpWeight += Armors[Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Type-11][Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].ID].Weight;
			}
			if (Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Family == FAMILY_USEABLE)
			{
				TmpWeight += Useables[Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].ID].Weight;
			}
			if (Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Family == FAMILY_COLLECTABLE)
			{
				TmpWeight += Collectables[Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].ID].Weight * Items[Accounts[Clients[This].AccountID].Char[S].Backpack[i]].Quantity;
			}
		}
	}


	Clients[This].Weapon = Clients[This].uChar->Worn[0];

	if (Clients[This].Weapon > 0)
	{
		TmpDamMin = Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].DamMin;
		TmpDamMax = Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].DamMax;

		// Add formula bonuses to here also

		TmpDamMin *= (1 + (float)Clients[This].uChar->SkillLevels[Items[Clients[This].Weapon].Type]/100 + (float)Clients[This].Str/400);
		TmpDamMax *= (1 + (float)Clients[This].uChar->SkillLevels[Items[Clients[This].Weapon].Type]/100 + (float)Clients[This].Str/400);

        ////
	}
	else
	{
		TmpDamMin = 1;
		TmpDamMax = 2;
	}

	TmpAC += Clients[This].Dex >> 1;

	Clients[This].AC = TmpAC;
	Clients[This].Weight = TmpWeight;	
	Clients[This].WeightMax = 10 + 5 * Clients[This].Str;
	Clients[This].uChar->HPMax = GetHPMax(This);
	
	buffer[0] = 30;
	buffer[1] = 6;
	memcpy(&buffer[2], &Clients[This].uChar->HPMax, sizeof(Clients[This].uChar->HPMax));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(Clients[This].uChar->HPMax), 0);

    //Inform everything to client.

	memset(&NewWeight, 0, sizeof(NewWeight));

	NewWeight.AC = Clients[This].AC;
	NewWeight.Weight = Clients[This].Weight;
	NewWeight.WeightMax = Clients[This].WeightMax;

	buffer[0] = 30;
	buffer[1] = 1;

	memcpy(&buffer[2], &NewWeight, sizeof(NewWeight));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewWeight), 0);

	memset(&NewStats, 0, sizeof(NewStats));

	NewStats.Con = Clients[This].Con;
	NewStats.Dex = Clients[This].Dex;
	NewStats.Int = Clients[This].Int;
	NewStats.Str = Clients[This].Str;
	NewStats.DamMax = TmpDamMax;
	NewStats.DamMin = TmpDamMin;	
	

	buffer[0] = 30;
	buffer[1] = 2;

	memcpy(&buffer[2], &NewStats, sizeof(NewStats));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewStats), 0);


	NewRes.fire  = Clients[This].prot_Fire;
	NewRes.air	 = Clients[This].prot_Air;
	NewRes.earth = Clients[This].prot_Earth;
	NewRes.water = Clients[This].prot_Water;
	
	buffer[0] = 30;
	buffer[1] = 8;
	memcpy(&buffer[2], &NewRes, sizeof(NewRes));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewRes), 0);*/
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void CheckStats_base(short This)
{
	memset(&NewStats_base, 0, sizeof(NewStats_base));

	NewStats_base.Con = Clients[This].Acc->tChar.Con;
	NewStats_base.Dex = Clients[This].Acc->tChar.Dex;
	NewStats_base.Int = Clients[This].Acc->tChar.Int;
	NewStats_base.Str = Clients[This].Acc->tChar.Str;
	NewStats_base.StatPoints = Clients[This].uChar->StatPoints;

	buffer[0] = 30;
	buffer[1] = 5;

	memcpy(&buffer[2], &NewStats_base, sizeof(NewStats_base));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewStats_base), 0);
}
//-----------------------------------------------------------------



//-----------------------------------------------------------------
//
void PlayerAction_AttackToMonster( short This)
{
	// There has got to be a better way to handle all attacking things. These work, but doesn't _look_ nice.

	// First check if it is possible for player to attack the monster.

	short ox, oy, tx, ty;
    
	ox = Clients[This].uChar->X;
	oy = Clients[This].uChar->Y;

	tx = Clients[This].MonTarget->X;

	ty = Clients[This].MonTarget->Y;

	if (ox - tx > -2 && ox - tx < 2 &&
		oy - ty > -2 && oy - ty < 2)
	{
		// Let's ATTACK!
		Clients[This].LstAtk = GetTickCount();

		// Check Tohit

		bool ToHit;
		ToHit = false;

		int ToHitRoll =0;
		int ToHitDefence = MonsterTypes[Clients[This].MonTarget->Type].AC;
		int ToHitRollDefence = 0;
		
		ToHitRoll = ((0.7 * (float)Clients[This].uChar->Skills[ Items[ Clients[This].Weapon ].Type].Level) + 
					(0.3 * (float)Clients[This].Acc->tChar.Dex)) * 2;
		
		if (ToHitDefence <= 0) ToHitDefence = 1;
		if (rand() % ToHitDefence < ToHitRoll) ToHit = true;
		else ToHit = false;

		if (rand() % 100 < 5) ToHit = false;

		if (ToHit)
		{
			if (Clients[This].MonTarget->Target != &Clients[This] && Clients[This].MonTarget->Target != NULL)
			{
				short x3,y3,t3,j3;
				
				x3 = Clients[This].MonTarget->X;
                t3 = Clients[This].MonTarget->Target->uChar->X;
				y3 = Clients[This].MonTarget->Y;
                j3 = Clients[This].MonTarget->Target->uChar->Y;

				if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1 && !Clients[This].GMMode)
				{
					Clients[This].MonTarget->Target = &Clients[This];
				}
			}

			if (Clients[This].MonTarget->Target == NULL && Clients[This].MonTarget->NPCTarget != NULL)
			{
				short x3,y3,t3,j3;
				
				x3 = Clients[This].MonTarget->X;
                t3 = Clients[This].MonTarget->NPCTarget->X;
				y3 = Clients[This].MonTarget->Y;
                j3 = Clients[This].MonTarget->NPCTarget->Y;

				if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1 && !Clients[This].GMMode)
				{
					Clients[This].MonTarget->Target = &Clients[This];
					Clients[This].MonTarget->NPCTarget = NULL;
				}
			}

			// Calcucate Damage

			short Damage;
			
			if (Clients[This].Weapon > 0 && Items[Clients[This].Weapon].Type > 0 && Items[Clients[This].Weapon].Type < TYPE_ARMOR)
			{
				Damage = rand() % (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].DamMax - Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].DamMin) +1;
				Damage += Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].DamMin;

				Damage *= (1 + (float)Clients[This].uChar->Skills[ Items[ Clients[This].Weapon ].Type ].Level / 100 + (float)Clients[This].Acc->tChar.Str/400);
			}
			else
			{
				Damage = rand() % 2 +1;				
			}

/*			Reduce = Damage * (1 - (float)MonsterTypes[Clients[This].MonTarget->Type].AC/600);
			Reduce = Damage - Reduce;

			if (Reduce > Damage) Reduce = Damage-1;
			if (Reduce < 0) Reduce = 0;

			Damage -= Reduce; */


			if (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].ElementalDamageType != NULL)
			{
				if (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].ElementalDamageType == elem_FIRE)
				{
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_FIRE)
						Damage *= 1 - (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_WATER)
						Damage *= 1 + (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
				}
				if (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].ElementalDamageType == elem_AIR)
				{
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_AIR)
						Damage *= 1 - (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_EARTH)
						Damage *= 1 + (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
				}
				if (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].ElementalDamageType == elem_EARTH)
				{
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_EARTH)
						Damage *= 1 - (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_AIR)
						Damage *= 1 + (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
				}
				if (Weapons[Items[Clients[This].Weapon].Type][Items[Clients[This].Weapon].ID].ElementalDamageType == elem_WATER)
				{
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_WATER)
						Damage *= 1 - (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
					if (MonsterTypes[Clients[This].MonTarget->Type].ElementalDefence == elem_FIRE)
						Damage *= 1 + (((float)MonsterTypes[Clients[This].MonTarget->Type].DefenceAmount) / 100);
				}			
			}


			if (Damage < 1) return;


			if (Clients[This].MonTarget->HPLeft - Damage < 0)
				Damage = Clients[This].MonTarget->HPLeft;

			Clients[This].MonTarget->HPLeft -= Damage;

			// Send combat text to target player
	
			memset(&NewCT, 0, sizeof(NewCT));
	
			buffer[0] = 97;
			buffer[1] = 1;
	
			NewCT.Amount = Damage;
			NewCT.Type = CT_DAMAGEDEALT;
			NewCT.FromTo = Clients[This].MonTarget->Type;
			NewCT.SkillType = 1;
	
			memcpy(&buffer[2], &NewCT, sizeof(NewCT));
	
			send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

			// Send new hit pic to players
			
			NewPic.Pic = 0;
			NewPic.X = tx;
			NewPic.Y = ty;
			
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
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewPic), 0);			
				}
			}

			if (Clients[This].MonTarget->HPLeft > 0)				
			{
				// Send new amount of HP to all clients in visual range.

				InformNewMonsterHP(Clients[This].MonTarget);
			}


			if (Clients[This].MonTarget->HPLeft <= 0)
			{               
				Clients[This].uChar->KilledMonsters++;
				//if (Clients[This].uChar->KilledMonsters > 30000) Clients[This].uChar->KilledMonsters = 30000;

				if (Check_Aligment(This))
				{
					Clients[This].report_reputation();
					Clients[This].report_repupoints();
					InformMassofNewAligment(This);
				}
				else Clients[This].report_repupoints();

				// Inform about dead monster.

				buffer[0] = 20;
				buffer[1] = 2;

				memcpy(&buffer[2], &Clients[This].MonTarget->KnowID, sizeof(Clients[This].MonTarget->KnowID));

				for (i = 0; i < Max_Connections; i++)
				{
					if (Clients[i].Used && Clients[i].State != 5)
					{
						if (Clients[i].MonTarget == Clients[This].MonTarget && This != i)
							Clients[i].MonTarget = NULL;

						Clients[i].KnowMonster[Clients[This].MonTarget->KnowID] = false;
						send(Clients[i].ClientSocket, buffer, 2 + sizeof(Clients[This].MonTarget->KnowID), 0);
					}
				}

				DropLoot(Clients[This].MonTarget, This);

				KillMonster(Clients[This].MonTarget);
				Clients[This].MonTarget = NULL;

			}

			// Calcucate exp

			short Exp_Gained;
			Exp_Gained = Damage;

			bool LvlGain = false;

			if (Clients[This].uChar->Level != MaxLevel)
			{
				Exp_Gained *= (1 + (float)Clients[This].Acc->tChar.Int / 50);
				
				Clients[This].uChar->Exp += Exp_Gained;

				LvlGain = CheckLevelGain(This);

				if (LvlGain)
					InformPlayerOfLevelUp(This);

				LvlGain = false;
			}

			if (Clients[This].Weapon > 0)
			Clients[This].uChar->Skills[Items[Clients[This].Weapon].Type].Exp += Exp_Gained;
			else 
			Clients[This].uChar->Skills[0].Exp += Exp_Gained;


			if (Clients[This].Weapon > 0)
				LvlGain = CheckSkillLevelGain(This, Items[Clients[This].Weapon].Type);
			else
				LvlGain = CheckSkillLevelGain(This, 0);

			if (LvlGain)
			{
				if (Clients[This].Weapon > 0)
					InformPlayerOfNewSkill(This, Items[Clients[This].Weapon].Type);
				else
					InformPlayerOfNewSkill(This, 0);
			}

			// Send combat text to target player

			memset(&NewCT, 0, sizeof(NewCT));
	
			if (Clients[This].uChar->Skills[Items[Clients[This].Weapon].Type].Level != 100)
			{
				buffer[0] = 97;
				buffer[1] = 1;
			
				NewCT.Amount = Exp_Gained;
				NewCT.Type = CT_EXPRECEIVED;
				if (Clients[This].Weapon > 0)
				NewCT.SkillType = Items[Clients[This].Weapon].Type;
				else NewCT.SkillType = 0;
			
				memcpy(&buffer[2], &NewCT, sizeof(NewCT));
			
				send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCT), 0);
			} else {
				if (Clients[This].uChar->Level != MaxLevel)
				{
					buffer[0] = 97;
					buffer[1] = 1;
				
					NewCT.Amount = Exp_Gained;
					NewCT.Type = CT_EXPRECEIVED_MAX;
					if (Clients[This].Weapon > 0)
					NewCT.SkillType = Items[Clients[This].Weapon].Type;
					else NewCT.SkillType = 0;
				
					memcpy(&buffer[2], &NewCT, sizeof(NewCT));
				
					send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewCT), 0);		
				}
			}

			// Inform new amount of exp to player.
			if (Clients[This].uChar->Level != MaxLevel)
			{
				buffer[0] = 11;
				buffer[1] = 2;

				memcpy(&buffer[2], &Clients[This].uChar->Exp, sizeof(Clients[This].uChar->Exp));
	                
				send(Clients[This].ClientSocket, buffer, 2 + sizeof(Clients[This].uChar->Exp), 0);
			}
		
		}
		else
		{
			// Missed.
	
			// Send miss pic to players
	
			NewPic.Pic = 1;
			NewPic.X = Clients[This].MonTarget->X;
			NewPic.Y = Clients[This].MonTarget->Y;
			
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
}
//-----------------------------------------------------------------

void KillMonster(MonsterClass *This)
{
	short hshx, hshy;
	hshx = This->X / 64;
	hshy = This->Y / 64;

	if (This == Monsters[hshx][hshy])
	{
		Monsters[hshx][hshy] = Monsters[hshx][hshy]->next;
		if (This->Used_Spawn != NULL)
			This->Used_Spawn->Used = false;
		UsedMonsterID[ This->KnowID] = false;
		delete This;
	}
	else
	{
		MonsterClass *tmptmp = nullptr;
	
		mon_List = Monsters[hshx][hshy];

		while (mon_List != NULL)
		{	
			if (mon_List == This)
			{
				tmptmp->next = mon_List->next;

				if (This->Used_Spawn != NULL)
					This->Used_Spawn->Used = false;
				UsedMonsterID[ This->KnowID] = false;

                delete This;
			
				mon_List = NULL;
			}        
	
			if (mon_List != NULL)
			{
				tmptmp = mon_List;
				mon_List = mon_List->next;
			}
		}
	}
}

//-----------------------------------------------------------------
//
void PlayerAction_AttackToPlayer( short i)
{
	Clients[i].LstAtk = GetTickCount();
	
	int Target = Clients[i].PLTarget;

	if (!Clients[Target].Used || Clients[Target].State == 5) 
	{
		Clients[i].PLTarget = 0;
	}

	bool ToHit = false;

	int j;

	short ox, oy, tx, ty;
    
	ox = Clients[i].uChar->X;
	oy = Clients[i].uChar->Y;

	tx = Clients[Target].uChar->X;
	ty = Clients[Target].uChar->Y;

	if (ox - tx < -1 || ox - tx > 1 ||
		oy - ty < -1 || oy - ty > 1)
	{
		return;
	}

	if (SafeZones[ ox >> 4 ][ oy >> 4 ] > 0)
	{
		if (!Clients[i].GMMode && !Clients[Target].uChar->Wanted) return;

		if (Clients[i].uChar->Level <= _NEWBIE_PROTECTION) return;

		if (SafeZones[ ox >> 4 ][ oy >> 4 ] == 2) return;	
	}

	if (Clients[i].uChar->Level <= _NEWBIE_PROTECTION) return;

	if (!Clients[Target].uChar->Wanted)
	{
		Clients[i].uChar->Wanted = true;
		InformMassofNewCriminalStatus( i );
	}

	int ToHitRoll = 0;
	int lvldif = Clients[i].uChar->Level - Clients[Target].uChar->Level;
	if (lvldif < 0) lvldif *= -2;
	else
		lvldif = 0;
	
	int ToHitDefence = Clients[Target].Acc->tChar.AC * 0.5 + lvldif;

//	if (Clients[i].Weapon > 0)
	ToHitRoll = (0.7 * (float)Clients[i].uChar->Skills[Items[Clients[i].Weapon].Type].Level) +
				(0.3 * (float)Clients[i].Acc->tChar.Dex);


	if (ToHitDefence <= 0) ToHitDefence = 1;
	if (rand() % ToHitDefence < ToHitRoll)
		ToHit = true;
	else ToHit = false;

	tx = Clients[Target].uChar->X;
	ty = Clients[Target].uChar->Y;
	memset(&NewPic, 0, sizeof(NewPic));

	if (ToHit)
	{
		// We hit the target.

		// Check Damage

		short Damage;
			
		if (Clients[i].Weapon > 0 && Items[Clients[i].Weapon].Type > 0)
		{
			Damage = rand() % (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMax - Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMin) +1;
			Damage += Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMin;

			Damage *= (1 + (float)Clients[i].uChar->Skills[Items[Clients[i].Weapon].Type].Level / 100 + (float)Clients[i].Acc->tChar.Str / 400);

		}
		else
		{
			Damage = rand() % 2 + 1;				
		}


		if (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].ElementalDamageType != NULL)
		{
			if (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].ElementalDamageType == elem_FIRE)
			{
				if (Clients[Target].Acc->tChar.prot_Fire > 0)
					Damage *= 1 - (((float)Clients[Target].Acc->tChar.prot_Fire) / 100);
				if (Clients[Target].Acc->tChar.prot_Water > 0)
					Damage *= 1 + (((float)Clients[Target].Acc->tChar.prot_Water) / 100);
			}
			if (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].ElementalDamageType == elem_AIR)
			{
				if (Clients[Target].Acc->tChar.prot_Air > 0)
					Damage *= 1 - (((float)Clients[Target].Acc->tChar.prot_Air) / 100);
				if (Clients[Target].Acc->tChar.prot_Earth > 0)
					Damage *= 1 + (((float)Clients[Target].Acc->tChar.prot_Earth) / 100);
			}
			if (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].ElementalDamageType == elem_EARTH)
			{
				if (Clients[Target].Acc->tChar.prot_Air > 0)
					Damage *= 1 + (((float)Clients[Target].Acc->tChar.prot_Air) / 100);
				if (Clients[Target].Acc->tChar.prot_Earth > 0)
					Damage *= 1 - (((float)Clients[Target].Acc->tChar.prot_Earth) / 100);
			}
			if (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].ElementalDamageType == elem_WATER)
			{
				if (Clients[Target].Acc->tChar.prot_Fire > 0)
					Damage *= 1 + (((float)Clients[Target].Acc->tChar.prot_Fire) / 100);
				if (Clients[Target].Acc->tChar.prot_Water > 0)
					Damage *= 1 - (((float)Clients[Target].Acc->tChar.prot_Water) / 100);
			}			
		}

/*		short Reduce;
		Reduce = Damage * (1 - (float)Clients[Target].AC/300);
		Reduce = Damage - Reduce;

		if (Reduce > Damage) Reduce = Damage-1;
		if (Reduce < 0) Reduce = 0;

		Damage -= Reduce;

		if (Damage < 1) return; */
		
		if (Clients[Target].is_logging)
		{			
			Clients[Target].LstLogTime = 0;
			Clients[Target].LstLogTime = GetTickCount(); // Change targets logout timer
		}

		// Send new hit pic to players
		
		NewPic.Pic = 0;
		NewPic.X = Clients[Target].uChar->X;
		NewPic.Y = Clients[Target].uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));


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
		NewCT.Type = 21;
		NewCT.FromTo = Clients[i].Acc->getAccID();
		NewCT.SkillType = 1;

		memcpy(&buffer[2], &NewCT, sizeof(NewCT));

		send(Clients[Target].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

		memset(&NewCT, 0, sizeof(NewCT));

		buffer[0] = 97;
		buffer[1] = 1;

		NewCT.Amount = Damage;
		NewCT.Type = 23;
		NewCT.FromTo = Clients[Target].Acc->getAccID();
		NewCT.SkillType = 1;

		memcpy(&buffer[2], &NewCT, sizeof(NewCT));

		send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

        // Reduce player's HP

		Clients[Target].uChar->HPLeft -= Damage;

		if ( Clients[Target].uChar->HPLeft <= 0)
		{
			// Player died.

			Clients[Target].uChar->HPLeft = Clients[Target].Acc->tChar.HPMax * 0.75;

			if (!Clients[i].GMMode)
			{
				if (!Clients[Target].uChar->Wanted) Clients[i].uChar->KilledMonsters -= Clients[Target].uChar->Reputation * 200;
				if (Clients[i].uChar->KilledMonsters < 0) Clients[i].uChar->KilledMonsters = 0;
	
				if (!Clients[Target].uChar->Wanted) Clients[i].uChar->CrimCount += 3;
				
				if (Check_Aligment(i))
				{	
					Clients[i].report_reputation();
					Clients[i].report_repupoints();
					InformMassofNewAligment(i);
				}
				else Clients[i].report_repupoints();

				if (Clients[Target].uChar->Wanted)
				{//1435,4699
					Clients[Target].CrimDeath();
				}
			}


			buffer[0] = 11;
			buffer[1] = 1;

			short oldx, oldy;

			short newx, newy;

			oldx = Clients[Target].uChar->X;
			oldy = Clients[Target].uChar->Y;


			newx = Clients[Target].uChar->ShrineX;
			newy = Clients[Target].uChar->ShrineY;

			Clients[Target].uChar->X = newx;
			Clients[Target].uChar->Y = newy;

			CreateBody(Target, oldx, oldy);

			Clients[Target].uChar->Wanted = false;

			Clients[i].PLTarget = -1;

			PACKET_COORDINATES NewCoords;
			memset(&NewCoords, 0, sizeof(NewCoords));
			NewCoords.X = newx;
			NewCoords.Y = newy;
			NewCoords.ID = Clients[Target].Acc->getAccID();

			buffer[0] = 10;
			buffer[1] = 1;
			memcpy(&buffer[2], &NewCoords, sizeof(NewCoords));
			send(Clients[Target].ClientSocket, buffer, 2 + sizeof(NewCoords), 0);

			CheckPlayersOnScreen(Target, NewCoords);
			CheckItemsOnScreen(Target);
			CheckMonstersOnScreen(Target);
			CheckNPCsOnScreen(Target);
		}

		// Inform player of their new HP
		// Inform players of new HP

		memset(&NewHP, 0, sizeof(NewHP));

		NewHP.KnowID = Clients[Target].Acc->getAccID();
		NewHP.NewHP = Clients[Target].uChar->HPLeft;
		NewHP.Type = 0;

		buffer[0] = 20;
		buffer[1] = 3;

		memcpy(&buffer[2], &NewHP, sizeof(NewHP));

		ox = Clients[Target].uChar->X;
		oy = Clients[Target].uChar->Y;

		send(Clients[Target].ClientSocket, buffer, 2 + sizeof(NewHP), 0);

		for (i = 0; i < Max_Connections; i++)
		{
			if (Clients[i].Used && Clients[i].State != 5)
			{
				tx = Clients[i].uChar->X;
				ty = Clients[i].uChar->Y;
				if (IsInRange(ox, oy, tx, ty))
				{
					if (Clients[i].KnowPlayer[Clients[Target].Know_ID] || i == Target)
						send(Clients[i].ClientSocket, buffer, 2 + sizeof( NewHP), 0);			
				}
			}
		}
	}
	else
	{
		// Missed.

		// Send miss pic to players

		NewPic.Pic = 1;
		NewPic.X = Clients[Target].uChar->X;
		NewPic.Y = Clients[Target].uChar->Y;
		
        buffer[0] = 21;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewPic, sizeof(NewPic));

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


void ClientClass::CrimDeath()
{
	// This code checks for closest crim spawn, and spawns player there.
	// It works simply - checking x + y differences between player and spawn point and compares
	// them to other spawns. Has worked perfectly so far.

	CrimSpawnList *closest = NULL;
	short sum = 30000, sum2;

	short x, y, xdif, ydif;

	x = Acc->uChar->X;
	y = Acc->uChar->Y;	


	CrimSpawnList *tmp;
	tmp = crimSpawns;
	while (tmp)
	{
		xdif = x - tmp->X;
		ydif = y - tmp->Y;

		if (xdif < 0) xdif *= -1;
		if (ydif < 0) ydif *= -1;

		sum2 = xdif + ydif;

		if (sum2 <= sum)
		{
			closest = tmp;
			sum = sum2;
		}

		tmp = tmp->Next;
	}

	Acc->uChar->ShrineX = closest->X;
	Acc->uChar->ShrineY = closest->Y;
}


//-----------------------------------------------------------------
//
void InformPlayerOfHisSkills(short This)
{
	int i;
	PACKET_SKILL NewSkill;
	for (i = 0; i < MaxSkills; i++)
	{
		if (Clients[This].uChar->Skills[i].Level > 0)
		{
            memset(&NewSkill, 0, sizeof(NewSkill));
			
			NewSkill.SkillExp	= Clients[This].uChar->Skills[i].Exp;
			NewSkill.SkillLevel = Clients[This].uChar->Skills[i].Level;
			NewSkill.This		= i;		

			buffer[0] = 11;
			buffer[1] = 3;

			memcpy(&buffer[2], &NewSkill, sizeof(NewSkill));

			send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewSkill), 0);
		}	
	}
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
bool CheckLevelGain(short This)
{
	if (Clients[This].uChar->Level >= LVL_MAX) return false;
	if (Clients[This].uChar->Exp >= ExpTables[Clients[This].uChar->Level + 1])
	{
		Clients[This].uChar->Level++;
		Clients[This].uChar->StatPoints += 2;

		Clients[This].Acc->tChar.HPMax = Clients[This].Acc->Calc_hpMax();
		return true;
	}
	return false;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
short GetHPMax(short This)
{
	short NewHPMax;
	NewHPMax = 50 + Clients[This].uChar->Level * 15 + Clients[This].Acc->tChar.Con * 10;
	return NewHPMax;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
bool CheckSkillLevelGain(short This, short Skill)
{
	if ( Clients[This].uChar->Skills[Skill].Exp >= SkillExpTables[Clients[This].uChar->Skills[Skill].Level + 1])
	{
        Clients[This].uChar->Skills[Skill].Level++;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformPlayerOfLevelUp(short This)
{
    buffer[0] = 30;
	buffer[1] = 3;

	PACKET_LEVELUP NewLevel;
	memset(&NewLevel, 0, sizeof(NewLevel));

	NewLevel.Level = Clients[This].uChar->Level;
	NewLevel.Statpoints = Clients[This].uChar->StatPoints;
	//NewLevel.HPMax = Clients[This].Acc->tChar.HPMax;
	NewLevel.HPMax = Clients[This].Acc->Calc_hpMax();

	memcpy(&buffer[2], &NewLevel, sizeof(NewLevel));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewLevel), 0);
}
//-----------------------------------------------------------------




//-----------------------------------------------------------------
//
void InformPlayerOfNewSkill(short This, short ThisSkill)
{
	PACKET_SKILLUP NewSkill;
	memset(&NewSkill, 0, sizeof(NewSkill));
	NewSkill.SkillID = ThisSkill;
	NewSkill.NewSkill = Clients[This].uChar->Skills[ThisSkill].Level;

	buffer[0] = 30;
	buffer[1] = 4;
	memcpy(&buffer[2], &NewSkill, sizeof(NewSkill));

	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewSkill), 0);
}
//-----------------------------------------------------------------



void CalcClientsConnected()
{
	short Joo, GMz[7];
	Joo = 0;
	memset(&GMz, 0, sizeof(GMz));
	ClientsConnectedtoShow = 0;	

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].uChar->DevMode == 0)
				ClientsConnectedtoShow++;

			Joo++;
			if (Clients[i].GMMode)
				GMz[Clients[i].uChar->DevMode]++;
		}
	}

	ClientsConnected = Joo;
	printf("\nClients connected: %d\nEWCs: %d, GMITs 2: %d, GMs 3: %d, Lead GMs: %d, Devs: %d, Lead Devs %d\n", Joo, GMz[1], GMz[2], GMz[3], GMz[4], GMz[5], GMz[6]);
}


void ResetStats(int This, int ThisChar)
{
	// This function works as the only place where you can do "updates" to characters between updates.
	// It's run at each login, so you can save all old changes, so if someone doesn't log in for like
	// 10 months, he still has to go through the changes.
	
	if (Clients[This].uChar->CharUpdate <= 1)
	{
		Clients[This].uChar->KilledMonsters -= 6500;
		if (Clients[This].uChar->KilledMonsters < 0) Clients[This].uChar->KilledMonsters = 0;		
	}

	// For example, update 0.30 removed all belaron and thorium ores from players and dropped
	// all potions player had, to ground (because of potionbug). Here's example how it's done:
	if (Clients[This].uChar->CharUpdate <= 3)
	{
		long Item;
		long Tmpid;
		for (int i = 0; i < MaxBackpackItems; i++)
		{
			Item = Clients[This].uChar->Backpack[i].ID;
			// Drop potions
			if (Items[Item].Type == TYPE_COLLECTABLE &&
				Items[Item].ID == 19)
			{
				Clients[This].uChar->Backpack[i].ID = 0;
				Tmpid = GetFreeGroundSlot();
				if (Tmpid != -1)
				{
					Ground[Tmpid] = Item;
					GroundTime[Tmpid] = GetTickCount();
				}
			}

			// Remove ores

			if (Items[Item].Type == TYPE_COLLECTABLE &&
				(Items[Item].ID == 6 || Items[Item].ID == 7))
			{
				Clients[This].uChar->Backpack[i].ID = 0;
				memset(&Items[Item], 0, sizeof(Items[Item]));
			}
		}
		for (int i = 0; i < MaxBankItems; i++)
		{
			Item = Clients[This].uChar->Bank[i].ID;

			// Remove ores

			if (Items[Item].Type == TYPE_COLLECTABLE &&
				(Items[Item].ID == 6 || Items[Item].ID == 7))
			{
				Clients[This].uChar->Bank[i].ID = 0;
				memset(&Items[Item], 0, sizeof(Items[Item]));
			}
		}
	}
	//================


	if (Clients[This].uChar->CharUpdate <= 6)
	{
		if (Clients[This].uChar->Dex < 5)
			Clients[This].uChar->Dex = 5;
		if (Clients[This].uChar->Con < 5)
			Clients[This].uChar->Con = 5;
		if (Clients[This].uChar->Str < 5)
			Clients[This].uChar->Str = 5;
		if (Clients[This].uChar->Int < 5)
			Clients[This].uChar->Int = 5;

		int shouldbe = 0;
		shouldbe = Clients[This].uChar->Level * 2 + 28;
		int has = 0;
		has = Clients[This].uChar->Dex + Clients[This].uChar->Con + Clients[This].uChar->Str + Clients[This].uChar->Int + Clients[This].uChar->StatPoints;

		if (has < shouldbe) Clients[This].uChar->StatPoints += (shouldbe - has);
	}

	if (Clients[This].uChar->CharUpdate <= 8) // Reset Stats
	{
		if (Clients[This].uChar->DevMode >= 1)
		{
			Clients[This].uChar->Level = 205;
			Clients[This].uChar->Str = 100;
			Clients[This].uChar->Dex = 100;
			Clients[This].uChar->Con = 100;
			Clients[This].uChar->Int = 100;
			Clients[This].uChar->StatPoints = 0;
			Clients[This].Acc->tChar.HPMax = Clients[This].Acc->Calc_hpMax();	
		
		} else {
			Clients[This].uChar->Str = 5;
			Clients[This].uChar->Dex = 5;
			Clients[This].uChar->Con = 5;
			Clients[This].uChar->Int = 5;
			Clients[This].uChar->StatPoints = Clients[This].uChar->Level * 2 + 8;
			Clients[This].Acc->tChar.HPMax = Clients[This].Acc->Calc_hpMax();		
		}
	}

	if (Clients[This].uChar->CharUpdate <= 11) // reset map to black
	{
		if (Clients[This].uChar->DevMode >= 2)
		{
			buffer[0] = 64;
			buffer[1] = 2;	
			send(Clients[This].ClientSocket, buffer, 2, 0);
		} else {
			buffer[0] = 64;
			buffer[1] = 1;	
			send(Clients[This].ClientSocket, buffer, 2, 0);
		}
	}

	
	if (Clients[This].uChar->CharUpdate <= 17) // v0.06
	{
		long Temp;

		if (Clients[This].uChar->DevMode >= 1)
		{			
			Clients[This].uChar->Level = 205; // Max out level
			Clients[This].uChar->Exp = 1750000000;

			Clients[This].uChar->Skills[0].Level = 100; // Max out skills
			Clients[This].uChar->Skills[0].Exp	= 80000000;
			Clients[This].uChar->Skills[1].Level = 100;
			Clients[This].uChar->Skills[1].Exp	= 80000000;
			Clients[This].uChar->Skills[2].Level = 100;
			Clients[This].uChar->Skills[2].Exp	= 80000000;
			Clients[This].uChar->Skills[3].Level = 100;
			Clients[This].uChar->Skills[3].Exp	= 80000000;
			Clients[This].uChar->Skills[4].Level = 100;
			Clients[This].uChar->Skills[4].Exp	= 80000000;
			Clients[This].uChar->Skills[15].Level = 100;
			Clients[This].uChar->Skills[15].Exp	= 80000000;
			Clients[This].uChar->Skills[16].Level = 100;
			Clients[This].uChar->Skills[16].Exp	= 80000000;

			Clients[This].uChar->Reputation = 8; // Divine alignment
			Clients[This].uChar->KilledMonsters = 20000;

			// Setup GM items
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[0].ID = Temp;

				Items[Temp].Type = TYPE_SWORD;
				Items[Temp].ID = 54;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = 1;
				Items[Temp].x = 25;
				Items[Temp].y = 20;
				Items[Temp].Durability = 10;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 0;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[1].ID = Temp;

				Items[Temp].Type = TYPE_ARMOR;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 20;
				Items[Temp].Durability = 10;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[2].ID = Temp;

				Items[Temp].Type = TYPE_GAUNTS;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 20;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[3].ID = Temp;

				Items[Temp].Type = TYPE_HELM;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 50;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[4].ID = Temp;

				Items[Temp].Type = TYPE_SHIELD;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 50;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[5].ID = Temp;

				Items[Temp].Type = TYPE_LEGS;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 20;
				Items[Temp].y = 75;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[6].ID = Temp;

				Items[Temp].Type = TYPE_RING1;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 75;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[7].ID = Temp;

				Items[Temp].Type = TYPE_AMULET;
				Items[Temp].ID = 52;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 75;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[8].ID = Temp;

				Items[Temp].Type = TYPE_COLLECTABLE;
				Items[Temp].ID = 0;
				Items[Temp].Quantity = 1000000;
				Items[Temp].Family = FAMILY_COLLECTABLE;
				Items[Temp].x = 75;
				Items[Temp].y = 125;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[9].ID = Temp;

				Items[Temp].Type = TYPE_COLLECTABLE;
				Items[Temp].ID = 47;
				Items[Temp].Quantity = 100;
				Items[Temp].Family = FAMILY_COLLECTABLE;
				Items[Temp].x = 50;
				Items[Temp].y = 125;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}


			buffer[0] = 64;
			buffer[1] = 2;	
			send(Clients[This].ClientSocket, buffer, 2, 0);
		}

		if (Clients[This].uChar->Level < 51) // Setup levels for wiped accounts
		{
			Clients[This].uChar->Level = 51; // Make level 55
			Clients[This].uChar->Exp = 11000000;

			Clients[This].uChar->Str = 5;
			Clients[This].uChar->Dex = 5;
			Clients[This].uChar->Con = 5;
			Clients[This].uChar->Int = 5;
			Clients[This].uChar->StatPoints = Clients[This].uChar->Level * 2 + 8;
			Clients[This].Acc->tChar.HPMax = Clients[This].Acc->Calc_hpMax();	

			Clients[This].uChar->Skills[0].Level = 50; // Make all skills level 50
			Clients[This].uChar->Skills[0].Exp	= 1900000;
			Clients[This].uChar->Skills[1].Level = 50;
			Clients[This].uChar->Skills[1].Exp	= 1900000;
			Clients[This].uChar->Skills[2].Level = 50;
			Clients[This].uChar->Skills[2].Exp	= 1900000;
			Clients[This].uChar->Skills[3].Level = 50;
			Clients[This].uChar->Skills[3].Exp	= 1900000;
			Clients[This].uChar->Skills[4].Level = 50;
			Clients[This].uChar->Skills[4].Exp	= 1900000;
			Clients[This].uChar->Skills[15].Level = 50;
			Clients[This].uChar->Skills[15].Exp	= 1900000;
			Clients[This].uChar->Skills[16].Level = 50;
			Clients[This].uChar->Skills[16].Exp	= 1900000;
			
			Clients[This].uChar->Reputation = 4; // neuteal alignment
			Clients[This].uChar->KilledMonsters = 3500;

			// Setup starter items
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[0].ID = Temp;

				Items[Temp].Type = TYPE_SWORD;
				Items[Temp].ID = 39;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = 1;
				Items[Temp].x = 25;
				Items[Temp].y = 20;
				Items[Temp].Durability = 10;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 0;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[1].ID = Temp;

				Items[Temp].Type = TYPE_SWORD;
				Items[Temp].ID = 42;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = 1;
				Items[Temp].x = 35;
				Items[Temp].y = 30;
				Items[Temp].Durability = 10;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 0;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[2].ID = Temp;

				Items[Temp].Type = TYPE_ARMOR;
				Items[Temp].ID = 29;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 20;
				Items[Temp].Durability = 10;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[3].ID = Temp;

				Items[Temp].Type = TYPE_GAUNTS;
				Items[Temp].ID = 29;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 20;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[4].ID = Temp;

				Items[Temp].Type = TYPE_HELM;
				Items[Temp].ID = 33;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 50;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[5].ID = Temp;

				Items[Temp].Type = TYPE_SHIELD;
				Items[Temp].ID = 28;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 50;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[6].ID = Temp;

				Items[Temp].Type = TYPE_LEGS;
				Items[Temp].ID = 53;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 20;
				Items[Temp].y = 75;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[7].ID = Temp;

				Items[Temp].Type = TYPE_RING1;
				Items[Temp].ID = 31;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 50;
				Items[Temp].y = 75;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[8].ID = Temp;

				Items[Temp].Type = TYPE_AMULET;
				Items[Temp].ID = 37;
				Items[Temp].Quantity = 1;
				Items[Temp].Family = FAMILY_ARMOR;
				Items[Temp].x = 75;
				Items[Temp].y = 75;
				Items[Temp].Durability = 0;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[9].ID = Temp;

				Items[Temp].Type = TYPE_COLLECTABLE;
				Items[Temp].ID = 0;
				Items[Temp].Quantity = 100000;
				Items[Temp].Family = FAMILY_COLLECTABLE;
				Items[Temp].x = 75;
				Items[Temp].y = 125;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}
			Temp = GetFreeItemID();
			if (Temp != -1)
			{
				Clients[This].uChar->Backpack[10].ID = Temp;

				Items[Temp].Type = TYPE_COLLECTABLE;
				Items[Temp].ID = 46;
				Items[Temp].Quantity = 50;
				Items[Temp].Family = FAMILY_COLLECTABLE;
				Items[Temp].x = 50;
				Items[Temp].y = 125;
				Items[Temp].Durability = 1;
				Items[Temp].Used = true;
				Items[Temp].KnowID = 1;
			}


			buffer[0] = 64;
			buffer[1] = 1;	
			send(Clients[This].ClientSocket, buffer, 2, 0);
		}
	}

	Clients[This].Acc->tChar.HPMax = Clients[This].Acc->Calc_hpMax();

	if (Debug) printf("reset done.\n");
}

void DoItemWipe(short This) // Yickes. :)
{
/*	int i;
    for (i = 0; i < MaxBankItems; i++)
	{
		Clients[This].uChar->Bank[i] = 0;
		if (i < MaxBackpackItems)
			Clients[This].uChar->Backpack[i] = 0;
		if (i < MaxWornItems)
			Clients[This].uChar->Worn[i] = 0;
	}

	long Amount = 400;

	for (i = 1; i <= Clients[This].uChar->Level; i++)
	{
        Amount += (500 + (i-1)*100);
		if (i % 5 == 0) 
			Amount += i * 1000;
		if (i % 25 == 0)
			Amount += i * 10000;
	}

	ItemStruct NewItemC;
	NewItemC.Durability = 1;
	NewItemC.Family = FAMILY_COLLECTABLE;
	NewItemC.ID = 0;
	NewItemC.Quantity = Amount;
	NewItemC.Type = TYPE_COLLECTABLE;
	NewItemC.x = rand() % 100;
	NewItemC.y = rand() % 100;
	NewItemC.Used = true;

	short This2;
	This2 = GetFreeBackpackSlot(This);
	if (This2 == -1) return;

	long TempID;
	TempID = GetFreeItemID();
	if (TempID == -1) return;

	memcpy(&Items[TempID], &NewItemC, sizeof(NewItemC));
                    
	Clients[This].uChar->Backpack[This2] =
		TempID;

	Items[Clients[This].uChar->Backpack[This2]].KnowID = This2; */
}



void InformMassofNewAligment( int This )
{
	unsigned long ble = Clients[This].Acc->getAccID();
	buffer[0] = 8;
	buffer[1] = 2;
	buffer[2] = Clients[This].uChar->Reputation;
	memcpy(&buffer[3], &ble, sizeof(ble));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].State != 5 && Clients[i].Used && This != i)
		{
			if (IsInRange(Clients[i].uChar->X, Clients[i].uChar->Y, Clients[This].uChar->X, Clients[This].uChar->Y))
				send(Clients[i].ClientSocket, buffer, 3 + sizeof(ble), 0);				
		}
	}
}

void ClientClass::report_reputation()
{
	unsigned short repu = uChar->Reputation;
	buffer[0] = 8;
	buffer[1] = 1;
	memcpy(&buffer[2], &repu, sizeof(repu));
	send(ClientSocket, buffer, 2 + sizeof(repu), 0);    
}

void ClientClass::report_repupoints()
{
	long repupoints = uChar->KilledMonsters;
	buffer[0] = 8;
	buffer[1] = 3;
	memcpy(&buffer[2], &repupoints, sizeof(repupoints));
	send(ClientSocket, buffer, 2 + sizeof(repupoints), 0);    
}



void InformMassofNewCriminalStatus2(ClientClass *This)
{
	int accID = This->Acc->getAccID();
	buffer[0] = 33;
	buffer[1] = This->uChar->Wanted + 1;
	memcpy(&buffer[2], &accID, sizeof(accID));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (IsInRange( Clients[i].uChar->X, Clients[i].uChar->Y,
						   This->uChar->X, This->uChar->Y))
			{
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
			}		
		}	
	}	
}

void InformSelfofCrimStatus(short Player)
{
	int accID = Clients[Player].Acc->getAccID();
	buffer[0] = 33;
	buffer[1] = Clients[Player].uChar->Wanted + 1;
	memcpy(&buffer[2], &accID, sizeof(accID));
	send(Clients[Player].ClientSocket, buffer, 2 + sizeof(accID), 0);	
}

void InformMassofNewCriminalStatus( short This )
{
	int accID = Clients[This].Acc->getAccID();
	buffer[0] = 33;
	buffer[1] = Clients[This].uChar->Wanted + 1;
	memcpy(&buffer[2], &accID, sizeof(accID));

	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (IsInRange( Clients[i].uChar->X, Clients[i].uChar->Y,
						   Clients[This].uChar->X, Clients[This].uChar->Y))
			{
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(accID), 0);
			}		
		}	
	}	
}

void InformGMofOnlineList( int This )
{
	PACKET_NEWONLINEPLAYER NewPlayer;
	int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5 && i != This)
		{
			if (Clients[i].uChar->DevMode <= Clients[This].uChar->DevMode)
			{
				memset(&NewPlayer, 0, sizeof(NewPlayer));
	
				NewPlayer.ID = Clients[i].Acc->getAccID();
				NewPlayer.DevMode = Clients[i].uChar->DevMode;
				NewPlayer.X = Clients[i].uChar->X;
				NewPlayer.Y = Clients[i].uChar->Y;
	
				strcpy(NewPlayer.Name, Clients[i].uChar->Name);
			    
				buffer[0] = 60;
				buffer[1] = 60;
				memcpy(&buffer[2], &NewPlayer, sizeof(NewPlayer));
				send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewPlayer), 0);
			}
		}	
	}
}

void InformGMofNewOnlinePlayer( int This, int ThisPlayer )
{
	PACKET_NEWONLINEPLAYER NewPlayer;

	memset(&NewPlayer, 0, sizeof(NewPlayer));

	NewPlayer.ID = Clients[ThisPlayer].Acc->getAccID();
	NewPlayer.DevMode = Clients[ThisPlayer].uChar->DevMode;
	NewPlayer.X = Clients[ThisPlayer].uChar->X;
	NewPlayer.Y = Clients[ThisPlayer].uChar->Y;
    
	strcpy(NewPlayer.Name, Clients[ThisPlayer].uChar->Name);
	buffer[0] = 60;
	buffer[1] = 60;
	memcpy(&buffer[2], &NewPlayer, sizeof(NewPlayer));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewPlayer), 0);
}

void InformGMofOnlineAmount( short ThisGM )
{
	short Client_ = ClientsConnected;
	buffer2[0] = 60;
	buffer2[1] = 7;
	memcpy(&buffer2[2], &Client_, sizeof(Client_));
	send(Clients[ThisGM].ClientSocket, buffer2, 2 + sizeof(Client_), 0);
}


void Check_Guild( short Player, short Slot )
{
	Clients[Player].Acc->uChar->Guild = 0;
	memset(&Clients[Player].Acc->uChar->Tag, 0, sizeof(Clients[Player].Acc->uChar->Tag));
	int i; int j;
	for (i = 1; i < MaxGuilds; i++)
	{
		if (Guilds[i].Active == 1)
		{
			for (j = 0; j < 50; j++)
			{
				if (Guilds[i].Members[j].AccID == Clients[Player].Acc->getAccID() && 
					Guilds[i].Members[j].Slot == Slot && Guilds[i].Members[j].Active == 1)
				{
					Clients[Player].Acc->uChar->Guild = i;
					strcpy(Clients[Player].Acc->uChar->Tag, Guilds[i].Tag);
					if (Guilds[i].Members[j].rank > 1)
					{
						Clients[Player].IsLeader = true;
					}
					else Clients[Player].IsLeader = false;
					i = MaxGuilds;
					j = 50;
				}			
			}
		}	
	}

    if ( Clients[Player].Acc->uChar->Guild == 0)
	{
		for (i = 1; i < MaxGuilds; i++)
		{
			if (Guilds[i].Active == 1)
			{
				if (Guilds[i].Leader_AccID == Clients[Player].Acc->getAccID() &&
					Guilds[i].Leader_Slot == Slot)
				{
					Clients[Player].Acc->uChar->Guild = i;
					strcpy(Clients[Player].Acc->uChar->Tag, Guilds[i].Tag);
					Clients[Player].IsLeader = true;
					i = MaxGuilds;
				}
			}		
		}
	} 
}

void InformGuildLeader( short This )
{
	short State = Clients[This].State;
	short Guild = Clients[This].Acc->uChar->Guild;
	
	short rank = -1;
	short member;

	for (int i = 0; i < 50; i++)
	{
		if (Guilds[Guild].Members[i].AccID == Clients[This].Acc->getAccID() &&
			Guilds[Guild].Members[i].Slot  == State)
		{
			rank = Guilds[Guild].Members[i].rank;
			member = i;
			i = 50;
		}
	}

	if (Guilds[Guild].Leader_AccID == Clients[This].Acc->getAccID() &&
		Guilds[Guild].Leader_Slot  == State)
		rank = 4;

	// Guild ranks
	// 0 - Recruit
	// 1 - Memeber
	// 2 - Elder
	// 3 - Councilor
	// 4 - Leader

	// Send confirmation to client that he is guild's leader.
	buffer[0] = 40;
	buffer[1] = 3;
	memcpy(&buffer[2], &rank, sizeof(rank));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(rank), 0);

	PACKET_NEWGUILDNAME NewName;

	strcpy(NewName.Name, Guilds[Guild].Name);

	buffer[0] = 40;
	buffer[1] = 20;
	memcpy(&buffer[2], &NewName, sizeof(NewName));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewName), 0);

	PACKET_NEWGUILDTAG NewTag;

	strcpy(NewTag.Tag, Guilds[Guild].Tag);			

	buffer[0] = 40;
	buffer[1] = 19;
	memcpy(&buffer[2], &NewTag, sizeof(NewTag));
	send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewTag), 0);

	PACKET_NEWGUILDMEMBER NewMember;

		memset(&NewMember, 0, sizeof(NewMember));
							
		NewMember.Status[1] = true;
		NewMember.ID[1] = Clients[This].Acc->getAccID();
		strcpy(NewMember.Name[1], Clients[This].Acc->uChar->Name);
		NewMember.rank[1] = rank;	

		buffer[0] = 40;
		buffer[1] = 1;
		memcpy(&buffer[2], &NewMember, sizeof(NewMember));
		send(Clients[This].ClientSocket, buffer, 2 + sizeof(NewMember), 0);   

}

void CheckGuildPlayers( int This )
{
	PACKET_NEWGUILDMEMBER NewGuildPlayer;
	memset(&NewGuildPlayer, 0, sizeof(NewGuildPlayer));

	short Guild = Clients[This].Acc->uChar->Guild;

	for (int g = 0; g < MaxGuilds; g++)
	{
		if (Guilds[Guild].Name == Guilds[g].Name)
		{
			for (int m = 0; m < 50; m++)
			{
				if (Guilds[Guild].Members[m].AccID) // Check if member slot is used
				{
					if (Guilds[Guild].Members[m].Active == 1) // Check member status				
						NewGuildPlayer.Status[m] = true;
					else
						NewGuildPlayer.Status[m] = false;
								
					strcpy(NewGuildPlayer.Name[m], Guilds[Guild].Members[m].Name); // get member name
					NewGuildPlayer.ID[m] = Guilds[Guild].Members[m].AccID; // get member ID
					NewGuildPlayer.rank[m] = Guilds[Guild].Members[m].rank; // get member rank
				}
			}	
			g = MaxGuilds;
		}
	}

	for (int z = 0; z < Max_Connections; z++) // Check all online players
	{
		if (Clients[z].Used && Clients[z].State != 5) // Make sure member is ingame
		{
			if (Clients[z].Acc->uChar->Guild == Guild) // Check if in same guild
			{				    
				buffer[0] = 40;
				buffer[1] = 15;
				memcpy(&buffer[2], &NewGuildPlayer, sizeof(NewGuildPlayer));
				send(Clients[z].ClientSocket, buffer, 2 + sizeof(NewGuildPlayer), 0); // send info to member
			}
		}
	}

}

void BuyGuild(short This, short i)
{
	Guilds[i].Active = 1;
	Guilds[i].Leader_AccID = Clients[This].Acc->getAccID();
	Guilds[i].Leader_Slot = Clients[This].State;
	sprintf(Guilds[i].Name, "Guild_%d", i);
	sprintf(Guilds[i].Tag, "G%d", i);

	memset(&Guilds[i].Members, 0, sizeof(Guilds[i].Members));

	strcpy(Guilds[i].Members[1].Name, Clients[This].uChar->Name); // get member name
	Guilds[i].Members[1].AccID = Clients[This].Acc->getAccID(); // get member ID
	Guilds[i].Members[1].rank = 4; // get member rank

	Clients[This].Acc->uChar->Guild = i;

	// Send info to player of new guild
	Clients[This].Acc->informGuild();

    InformGuildLeader(This);

	saveguilds();
}

void PlayerAction_AttackToNPC( short i)
{
	short ox, oy, tx, ty;

	NPCClass *Target = Clients[i].NPCTarget;
    
	ox = Clients[i].uChar->X;
	oy = Clients[i].uChar->Y;

	tx = Target->X;
	ty = Target->Y;


	if (SafeZones[ ox >> 4 ][ oy >> 4 ] > 1)
		return;	

	if (ox - tx > -2 && ox - tx < 2 &&
		oy - ty > -2 && oy - ty < 2)
	{
		Clients[i].uChar->Wanted = true;
		InformMassofNewAligment( i);
		// Let's ATTACK!
		Clients[i].LstAtk = GetTickCount();

		// Check Tohit

		bool ToHit;
		ToHit = false;


		int ToHitRoll =0;
		int ToHitDefence =0;
		int ToHitRollDefence =400;			


		if (Clients[i].Weapon > 0 && Items[Clients[i].Weapon].Type > 0)
			ToHitRoll = 2 * Clients[i].uChar->Dex + 2 * Clients[i].uChar->Skills[Items[Clients[i].Weapon].Type].Level;
		else
			ToHitRoll = 2 * Clients[i].uChar->Dex + 2 * Clients[i].uChar->Skills[0].Level;

		ToHitDefence = 3 * 200;

		ToHitRollDefence -= ToHitDefence;

		if (ToHitRoll > rand() % ToHitRollDefence)
		{
			ToHit = true;	
		}

		if (rand() % 100 <= 25)
			ToHit = true;

		// if Tohit succeed {

		if (ToHit)
		{
			// We hit the target! Check if this player isn't Monster's target and Monster isn't near it's target, change this player to be monster's target.

/*			if (Target->NPCPLTarget != -1)
			{
				if (i != Target->NPCPLTarget && Target->NPCPLTarget != -1 && Target->NPCPLTarget != 0 && NPCTypes[Target->Type].is_guard)
				{
					short x3,y3,t3,j3;
					
					x3 = Target->X;
	                t3 = Accounts[ Clients[ Target->NPCPLTarget ].AccountID].Char[Clients[ Target->NPCPLTarget].State].X;					
					y3 = Target->Y;
			        j3 = Accounts[ Clients[ Target->NPCPLTarget ].AccountID].Char[Clients[ Target->NPCPLTarget].State].Y;
	
					if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1)
					{
						Target->NPCPLTarget = Accounts[Clients[i].AccountID].ID;
					}
				}
			}
			else 
			if (Target->NPCPLTarget != -1 && Target->NPCMonsterTarget != NULL && NPCTypes[Target->Type].is_guard)
			{
				short x3,y3,t3,j3;
					
				x3 = Target->X;
                t3 = Target->NPCMonsterTarget->X;
				y3 = Target->Y;
		        j3 = Target->NPCMonsterTarget->Y;
	
				if (x3 - t3 < -1 || x3 - t3 > 1 || y3 - j3 < -1 || y3 - j3 > 1)
				{
					Target->NPCPLTarget = i;
				}
			}*/

			// Calcucate Damage

			short Damage;
			
			if (Clients[i].Weapon > 0 && Items[Clients[i].Weapon].Type > 0)
			{
				Damage = rand() % (Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMax - Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMin);
				Damage += Weapons[Items[Clients[i].Weapon].Type][Items[Clients[i].Weapon].ID].DamMin;

				Damage *= (1 + (float)Clients[i].uChar->Skills[Items[Clients[i].Weapon].Type].Level / 100 + (float)Clients[i].uChar->Str/400);
			}
			else
			{
				Damage = rand() % 2 +1;				
			}

/*			Reduce = Damage * (1 - 300/600);
			Reduce = Damage - Reduce;

			if (Reduce > Damage) Reduce = Damage-1;
			if (Reduce < 0) Reduce = 0;

			Damage -= Reduce;

			if (Damage < 1) return; */


			if (Target->HPLeft - Damage < 0)
				Damage = Target->HPLeft;

			Target->HPLeft -= Damage;

			// Send combat text to target player
	
			memset(&NewCT, 0, sizeof(NewCT));
	
			buffer[0] = 97;
			buffer[1] = 1;
	
			NewCT.Amount = Damage;
			NewCT.Type = 20;
			NewCT.FromTo = Target->Type;
			NewCT.SkillType = 1;
	
			memcpy(&buffer[2], &NewCT, sizeof(NewCT));
	
			send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewCT), 0);

			// Send new hit pic to players
			
			NewPic.Pic = 0;
			NewPic.X = tx;
			NewPic.Y = ty;
			
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

			if (Target->HPLeft > 0)				
			{
				// Send new amount of HP to all clients in visual range.

//				InformNewMonsterHP(Clients[This].MonTarget);
			}


			if (Target->HPLeft <= 0)
			{
				Clients[i].NPCTarget = NULL;
               
				Clients[i].uChar->KilledMonsters -= 500;
				if (Clients[i].uChar->KilledMonsters < 0) Clients[i].uChar->KilledMonsters = 0;

				Clients[i].uChar->CrimCount += 3;

				if (Check_Aligment(i))
				{
					Clients[i].report_reputation();
					InformMassofNewAligment(i);
				}
				Clients[i].report_repupoints();

				// Inform about dead NPC.

		//		Target->Alive = DEAD;
		//		Target->LstDeath = GetTickCount();
				Target->NPCPLTarget = -1;
				Target->NPCMonsterTarget = NULL;


				short hshx[9], hshy[9];
				hshx[0] = Target->X / 64;
				hshy[0] = Target->Y / 64;

				hshx[1] = hshx[0] -1; hshy[1] = hshy[0] -1; hshx[2] = hshx[0] -1;hshy[2] = hshy[0]; hshx[3] = hshx[0] -1;hshy[3] = hshy[0] +1; hshx[4] = hshx[0];hshy[4] = hshy[0] -1; hshx[5] = hshx[0];hshy[5] = hshy[0] +1; hshx[6] = hshx[0] +1;	hshy[6] = hshy[0] -1; hshx[7] = hshx[0] +1;hshy[7] = hshy[0]; hshx[8] = hshx[0] +1;	hshy[8] = hshy[0] +1; 

				int i;
				for (i = 0; i < 9; i++)
				{
					if (hshx[i] >= 0 && hshy[i] >= 0 && hshx[i] < 128 && hshy[i] < 128)
					{
						mon2_List = Monsters[hshx[i]][hshy[i]];
	
						while (mon2_List != NULL)
						{
							if (mon2_List->NPCTarget == Target)
								mon2_List->NPCTarget = NULL;
	
							if (mon2_List != NULL)
								mon2_List = mon2_List->next;	
						}
					}			
				}


				buffer[0] = 20;
				buffer[1] = 7;

				memcpy(&buffer[2], &Target->ID, sizeof(Target->ID));

				for (j = 0; j < Max_Connections; j++)
				{
					if (Clients[j].Used && Clients[j].State != 5)
					{
						if (Clients[j].KnowNPC[Target->ID])
						{
							Clients[j].KnowNPC[Target->ID] = false;
							send(Clients[j].ClientSocket, buffer, 2 + sizeof(Target->ID), 0);
						}
						if (Clients[j].NPCTarget == Target && j != i)
							Clients[j].NPCTarget = NULL;
					}
				}

				NPCAction_Died(Target);
			}

			// No exp from NPCs.
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
}


void CreateBody(short This, short ToX, short ToY)
{
	short ThisBody = -1;
	int i;
	for (i = 0; i < m_Bodies; i++)
	{
		if (!Body[i].Used)
		{
			Body[i].Used = true;
			Body[i].DecayState = 0;
			Body[i].DecayStateTime = GetTickCount();

			Body[i].Looted = false;
			Body[i].Owner_ID = Clients[This].Acc->getAccID();
			Body[i].Owner_Slot = Clients[This].State;

			Body[i].X = ToX;
			Body[i].Y = ToY;
			strcpy(Body[i].OwnerName, Clients[This].uChar->Name);
			Body[i].LooterCrim = Clients[This].uChar->Wanted;

			Body[i].Guild = -1;

			if (Clients[This].Acc->uChar->Guild != 0)
				Body[i].Guild = Clients[This].Acc->uChar->Guild;
				

			ThisBody = i;
			i = m_Bodies;
		}
	}
    
	if (ThisBody == -1) return;

	if (Clients[This].uChar->Level < _NEWBIE_PROTECTION)
		return;

//	if (SafeZones[ToX >> 4][ToY >> 4] == 1 && !Clients[This].uChar->Wanted) return;
	if (SafeZones[ToX >> 4][ToY >> 4] == 2)
		return;


	if (Clients[This].TradeinProgress) SecureTrade[0].AbortbyID(This);


	//-------------------------------------
	// Drop items

	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (Clients[This].uChar->Backpack[i].ID != 0)
		{   			
			Body[ThisBody].Items[i] = Clients[This].uChar->Backpack[i].ID;
			Items[Body[ThisBody].Items[i]].KnowID = i;
			
			Items[Body[ThisBody].Items[i]].x = rand() % 100;
			Items[Body[ThisBody].Items[i]].y = rand() % 100;
			Clients[This].uChar->Backpack[i].ID = 0;
		}	
	}


	bool Items_[MaxWearedItems];
	short count = 0;
	memset(&Items_, 0, sizeof(Items_));

	for (i = 0; i < MaxWearedItems; i++)
	{
		if (Clients[This].uChar->Worn[i].ID != 0)
		{
			Items_[i] = true;
			count++;
/*			memcpy(&Body[ThisBody].Items[i + MaxBackpackItems], &Clients[This].Worn[Clients[This].State][i], sizeof(Clients[This].Worn[Clients[This].State][i]));
			Body[ThisBody].Items[i + MaxBackpackItems].KnowID = i + MaxBackpackItems;
			Body[ThisBody].Items[i + MaxBackpackItems].x = rand() % 100;
			Body[ThisBody].Items[i + MaxBackpackItems].y = rand() % 100;
            memset(&Clients[This].Worn[Clients[This].State][i], 0, sizeof(Clients[This].Worn[Clients[This].State][i]));*/
		}	
	}

	bool yea = true;
	short dam = 0, nd = 0;
	short times = 0, p, c[3];
	c[0] = -1;
	c[1] = -1;
	c[2] = -1;

	if (Clients[This].uChar->Reputation == 0)
		nd = 2;
	else if (Clients[This].uChar->Reputation == 8)
		nd = 1;
	else
	{
		nd= rand() % 2;
		nd++;
	}	

	if (Clients[This].Acc->uChar->Wanted)
		nd = 3;
	

	while (yea && times < 30)
	{
        p = rand() % MaxWearedItems;

		if (Items_[p]) 
		{
			Items_[p] = false;
			c[dam] = p;
			dam++;
			if (dam == nd)
				yea = false;
		}
        
		times++;
	}

	if (!yea)
	{
		for (int i = 0; i < nd; i++)
		{
			if (c[i] > -1)
			{
				Body[ThisBody].Items[MaxBackpackItems+1 + i] = Clients[This].uChar->Worn[c[i]].ID;
				Items[Body[ThisBody].Items[1 + MaxBackpackItems + i]].KnowID = 1 + MaxBackpackItems + i;
				Items[Body[ThisBody].Items[1 + MaxBackpackItems + i]].x = rand() % 100;
				Items[Body[ThisBody].Items[1 + MaxBackpackItems + i]].y = rand() % 100;

				RemoveItemFromPlayer(This, ItemWeared, c[i]);

				Clients[This].uChar->Worn[c[i]].ID = 0;
			}
		}
	}

	CheckStats(This);

	// This will remove all items player have.
	buffer[0] = 12;
	buffer[1] = 18;
	send(Clients[This].ClientSocket, buffer, 2, 0);
	//-------------------------------------

	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			CheckBodiesOnScreen(i);
		}
	}
}

void RemoveBody( int ThisBody)
{
	short TmpID; long TempID;
	bool Failed;
	int i;
	for (i = 0; i < 84; i++)
	{
		if (Body[ThisBody].Items[i] != 0)
		{
			Failed = false;
			TempID = Body[ThisBody].Items[i];
//			CreateItemToGround(Body[ThisBody].Items[i]);
			TmpID = GetFreeGroundSlot();
			if (TmpID == -1) Failed = true;

			if (!Failed)
			{
				Ground[TmpID] = TempID;
				Items[Ground[TmpID]].x = Body[ThisBody].X;
				Items[Ground[TmpID]].y = Body[ThisBody].Y;
				Items[Ground[TmpID]].KnowID = TmpID;
				GroundTime[TmpID] = GetTickCount();
            
				InformMassofItem(TmpID);
			}
			else
				Items[TempID].Used = false;

            Body[ThisBody].Items[i] = 0;
		}
	}

	memset(&Body[ThisBody], 0, sizeof(Body[ThisBody]));
}

void mass_InformNewLooter(short BodyID)
{
	memset(&NewBodyInfo, 0, sizeof(NewBodyInfo));

	NewBodyInfo.BodyID = BodyID;
	NewBodyInfo.Looted = true;
	strcpy(NewBodyInfo.Name, Body[BodyID].LooterName);

	buffer[0] = 22;
	buffer[1] = 2;
	memcpy(&buffer[2], &NewBodyInfo, sizeof(NewBodyInfo));

    int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (IsInRange(Clients[i].uChar->X, Clients[i].uChar->Y, Body[BodyID].X, Body[BodyID].Y))
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewBodyInfo), 0);			
		}
	}
}

void Player_Inform_GotLooted(char Name[25], short WhatBody)
{
    int i;
	for (i = 0; i < Max_Connections; i++)
	{
		if (Clients[i].Used && Clients[i].State != 5)
		{
			if (Clients[i].Acc->getAccID() == Body[WhatBody].Owner_ID &&
				Clients[i].State == Body[WhatBody].Owner_Slot)
			{
                memset(&NewBodyInfo, 0, sizeof(NewBodyInfo));
				strcpy(NewBodyInfo.Name, Name);
				buffer[0] = 22;
				buffer[1] = 6;
				memcpy(&buffer[2], &NewBodyInfo, sizeof(NewBodyInfo));
				send(Clients[i].ClientSocket, buffer, 2 + sizeof(NewBodyInfo), 0);

				i = Max_Connections;
			}
		}
	}
}

bool Check_Requirement(short This, short ReqType, short ReqAmount, short ThisSkill)
{
	if (ReqAmount <= 0) return true;

	switch (ReqType)
	{
	case req_STR:
		if (Clients[This].Acc->tChar.Str >= ReqAmount)
			return true;
		else
			return false;
	case req_DEX:
		if (Clients[This].Acc->tChar.Dex >= ReqAmount)
			return true;
		else
			return false;
	case req_CON:
		if (Clients[This].Acc->tChar.Con >= ReqAmount)
			return true;
		else
			return false;
	case req_INT:
		if (Clients[This].Acc->tChar.Int >= ReqAmount)
			return true;
		else
			return false;
	case req_LVL:
		if (Clients[This].uChar->Level >= ReqAmount)
			return true;
		else
			return false;
	case req_SKL:
		if (Clients[This].uChar->Skills[ThisSkill].Level >= ReqAmount)
			return true;
		else
			return false;
	case req_REP:
		if (Clients[This].uChar->Reputation <= ReqAmount)
			return true;
		else
			return false;
	case req_REP2:
		if (Clients[This].uChar->Reputation >= ReqAmount)
			return true;
		else
			return false;
	default: return true;
	}
	return true;
}

bool Check_Aligment(short This)
{
	short old_alig, new_alig = -1;

	old_alig = Clients[This].uChar->Reputation;

    int i;
	for (i = 0; i < 9; i++)
	{
		if (Clients[This].uChar->KilledMonsters >= AligmentTables[i])
			new_alig = i;
	}

/*	if (Clients[This].uChar->KilledMonsters < 500)// &&
//		(Accounts[Clients[This].AccountID].HasBonusAccess || Accounts[Clients[This].AccountID].HasDonaterAccess))
		new_alig = 0;

	if (Clients[This].uChar->KilledMonsters >= AligmentTables[8])// && 
//		(Accounts[Clients[This].AccountID].HasBonusAccess || Accounts[Clients[This].AccountID].HasDonaterAccess))
		new_alig = 8;
*/
	if (Clients[This].uChar->DevMode > 1)
	{
		new_alig = 4;
		Clients[This].uChar->Reputation = new_alig;
		Clients[This].uChar->Wanted = false;
		return true;
	} else if (Clients[This].uChar->DevMode == 1)
	{
		new_alig = 10;
		Clients[This].uChar->Reputation = new_alig;
		Clients[This].uChar->Wanted = false;
		return true;
	}

	if (new_alig == -1 && old_alig != 0) new_alig = 4;

	Clients[This].uChar->Reputation = new_alig;

	if (old_alig != new_alig) return true;
	else return false;
}




/*void SecureTradeClass::ChangeLock(short Trader, bool Lock)
{
	Locks[Trader] = false;

	buffer[0] = 54;
	buffer[1] = 1;
	buffer[2] = Lock;
	send(Clients[Traders[Trader]].ClientSocket, buffer, 3, 0);

	buffer[0] = 54;
	buffer[1] = 2;
	buffer[2] = Lock;
	send(Clients[Traders[!Trader]].ClientSocket, buffer, 3, 0);

}*/


void ClientClass::use_skill()
{
	if (t_useskill != 1 && t_useskill != 2 && t_useskill != 3)
	{
		tradeskill_inuse = false;
		t_useskill = -1;
		return;
	}
	if (t_useskill == 1)
	{
		mined();
		tradeskill_inuse = false;
		t_useskill = -1;
		return;
	}
	if (t_useskill == 2)
	{
		smelted();
		tradeskill_inuse = false;
		t_useskill = -1;
		return;
	}
	if (t_useskill == 3)
	{
		forged();
		tradeskill_inuse = false;
		t_useskill = -1;
		return;
	}
}

void ClientClass::mined()
{
	if (Debug) printf("Begin mine,");

	if (!t_tradesucceed)
	{
		// Mining failed, but exp was received.
	
		if (Debug) printf("failed,");

		buffer[0] = 99;
		buffer[1] = 58;
		send(ClientSocket, buffer, 2, 0);
	}
	else if (t_recvam > 0)
	{
		if (Debug) printf("succeed");
		bool piled;
		piled = false;
		int j;
		for (j = 0; j < MaxBackpackItems; j++)
		{
			if (Items[uChar->Backpack[j].ID].Type == TYPE_COLLECTABLE &&
				Items[uChar->Backpack[j].ID].ID == t_tarid)
			{
				if (Debug) printf("found existing,");

				Items[uChar->Backpack[j].ID].Quantity += t_recvam;
	
				InformPlayerOfNewAmount(Know_ID, ItemBackpack, j, Items[uChar->Backpack[j].ID].Quantity);
				piled = true;
	
				buffer[0] = 99;
				buffer[1] = 57;
				short _tm;
				_tm = t_recvam;
				memcpy(&buffer[2], &_tm, sizeof(_tm));
				send(ClientSocket, buffer, 2 + sizeof(_tm), 0);

				//if (Debug) printf("added to existing,");

				j = MaxBackpackItems;
			}
		}
	                            
		if (!piled)
		{
			if (Debug) printf("no existing so creating new,");

			ItemStruct NewItem;
			memset(&NewItem, 0, sizeof(NewItem));
			NewItem.Family = FAMILY_COLLECTABLE;
			NewItem.ID = t_tarid;
			NewItem.Type = TYPE_COLLECTABLE;
			NewItem.Quantity = t_recvam;
			NewItem.Used = true;
			NewItem.x = rand() % 100;
			NewItem.y = rand() % 100;

			long TempID;
			short TmpID;
			TmpID = GetFreeBackpackSlot(Know_ID);
			if (TmpID == -1) return;
			TempID = GetFreeItemID();
			if (TempID == -1) return;

			memcpy(&Items[TempID], &NewItem, sizeof(NewItem));

			uChar->Backpack[TmpID].ID = TempID;
			Items[TempID].KnowID = TmpID;

			InformPlayerofItem(Know_ID, TempID, ItemBackpack);

			
			buffer[0] = 99;
			buffer[1] = 57;
			short _tm;
			_tm = t_recvam;
			memcpy(&buffer[2], &_tm, sizeof(_tm));
			send(ClientSocket, buffer, 2 + sizeof(_tm), 0);

			if (Debug) printf("new created,");
		}
	}
	
	if (Debug) printf("inform exp,");
	
	// Inform new amount of exp to player.
	if (uChar->Level != MaxLevel)
	{
		t_recvexp *= (1 + (float)Acc->tChar.Int / 50);				
		uChar->Exp += t_recvexp;

		buffer[0] = 11;
		buffer[1] = 2;

		memcpy(&buffer[2], &uChar->Exp, sizeof(uChar->Exp));
	                
		send(ClientSocket, buffer, 2 + sizeof(uChar->Exp), 0);
	}
	
	bool LvlGain = false;

	if (Debug) printf("check level,");

	LvlGain = CheckLevelGain(Know_ID);

	if (LvlGain)
		InformPlayerOfLevelUp(Know_ID);

	LvlGain = false;

	uChar->Skills[15].Exp += t_recvexp;

	if (Debug) printf("check skill,");

	LvlGain = CheckSkillLevelGain(Know_ID, 15);

	if (LvlGain)
	{
		InformPlayerOfNewSkill(Know_ID, 15);
	}

	if (Debug) printf("send combatmessage,");

	// Send combat text to target player

	memset(&NewCT, 0, sizeof(NewCT));

		if (uChar->Skills[15].Level != 100)
		{	
			buffer[0] = 97;
			buffer[1] = 1;
			
			NewCT.Amount = t_recvexp;
			NewCT.Type = CT_EXPRECEIVED;
			NewCT.SkillType = 15;
					
			memcpy(&buffer[2], &NewCT, sizeof(NewCT));
					
			send(ClientSocket, buffer, 2 + sizeof(NewCT), 0);
		} else {
			if (uChar->Level != MaxLevel)
			{
				buffer[0] = 97;
				buffer[1] = 1;
				
				NewCT.Amount = t_recvexp;
				NewCT.Type = CT_EXPRECEIVED_MAX;
				NewCT.SkillType = 15;
				
				memcpy(&buffer[2], &NewCT, sizeof(NewCT));
				
				send(ClientSocket, buffer, 2 + sizeof(NewCT), 0);		
			}
		}

	if (t_useitembroke)
	{
		if (uChar->Backpack[Items[t_useitemid].KnowID].ID != t_useitemid)
			return;

		if (Debug) printf("pickaxe broke,");
		buffer[0] = 99;
		buffer[1] = 54;
		memcpy(&buffer[2], &Items[t_useitemid].KnowID, sizeof(Items[t_useitemid].KnowID));
						
		send(ClientSocket, buffer, 2 + sizeof(Items[t_useitemid].KnowID), 0);

		RemoveItemFromPlayer(Know_ID, ItemBackpack, Items[t_useitemid].KnowID);
		uChar->Backpack[Items[t_useitemid].KnowID].ID = 0;		
		memset(&Items[t_useitemid], 0, sizeof(Items[t_useitemid]));
	}

	CheckStats(Know_ID);

	if (Debug) printf("mine done.\n");
}

void ClientClass::smelted()
{
	if (Debug) printf("Begin smelt,");

	if (Items[t_useitemid].Quantity == t_failam)
	{
		// Smelting totally failed

		if (Debug) printf("smelt totally failed,");
                            
		buffer[0] = 99;
		buffer[1] = 70;
		send(ClientSocket, buffer, 2, 0);

		RemoveItemFromPlayer(Know_ID, ItemBackpack, Items[t_useitemid].KnowID);
		uChar->Backpack[Items[t_useitemid].KnowID].ID = 0;
		memset(&Items[t_useitemid], 0, sizeof(Items[t_useitemid]));
	}
	else if (t_recvam > 0)
	{
		if (Debug) printf("smelt partially/fully succeed,");
		bool Error_Ignot;
		Error_Ignot = false;
		int j;
		for (j = 0; j < MaxBackpackItems; j++)
		{
			if (uChar->Backpack[j].ID > 0)
			{
				if (Items[t_useitemid].Type == TYPE_COLLECTABLE &&
					Items[uChar->Backpack[j].ID].ID == Items[t_useitemid].ID + 4)
				{
					if (Debug) printf("found existing,");
					Items[uChar->Backpack[j].ID].Quantity += t_recvam;

					InformPlayerOfNewAmount(Know_ID, ItemBackpack, j, Items[uChar->Backpack[j].ID].Quantity);
					Error_Ignot = true;

					if (Debug) printf("added existing,");

					j = MaxBackpackItems;
				}
			}
		}
                           
		if (!Error_Ignot)
		{
			if (Debug) printf("found new,");

			long TempID;
			short TmpID;
			TmpID = GetFreeBackpackSlot(Know_ID);
			if (TmpID == -1) return;
			TempID = GetFreeItemID();
			if (TempID == -1) return;

			ItemStruct NewItem;
			memset(&NewItem, 0, sizeof(NewItem));
			NewItem.Family = FAMILY_COLLECTABLE;
			NewItem.ID = Items[t_useitemid].ID + 4;
			NewItem.Type = TYPE_COLLECTABLE;
			NewItem.Quantity = t_recvam;
			NewItem.Used = true;
			NewItem.x = rand() % 100;
			NewItem.y = rand() % 100;
			NewItem.KnowID = TmpID;

			memcpy(&Items[TempID], &NewItem, sizeof(NewItem));
			uChar->Backpack[TmpID].ID = TempID;

			InformPlayerofItem(Know_ID, TempID, ItemBackpack);
		}


		buffer[0] = 99;
		buffer[1] = 71;
		short _tm;
		_tm = t_recvam;
		memcpy(&buffer[2], &_tm, sizeof(_tm));
		send(ClientSocket, buffer, 2 + sizeof(_tm), 0);


		buffer[0] = 99;
		buffer[1] = 72;
		_tm = t_failam;
		memcpy(&buffer[2], &_tm, sizeof(_tm));
		send(ClientSocket, buffer, 2 + sizeof(_tm), 0);


		RemoveItemFromPlayer(Know_ID, ItemBackpack, Items[t_useitemid].KnowID);
		uChar->Backpack[Items[t_useitemid].KnowID].ID = 0;
		memset(&Items[t_useitemid], 0, sizeof(Items[t_useitemid]));
	}

	CheckStats(Know_ID);

	if (Debug) printf("smelt done.\n");
}

void ClientClass::forged()
{
	if (!t_tradesucceed)
	{
		// Forging failed
		buffer[0] = 99;
		buffer[1] = 75;
		send(ClientSocket, buffer, 2, 0);

		if (Items[t_useitem].Quantity > (t_cost / 2 > 0 ? t_cost / 2 : 1))
		{
			Items[t_useitem].Quantity -= (t_cost / 2 > 0 ? t_cost / 2 : 1);
			InformPlayerOfNewAmount(Know_ID, ItemBackpack, t_useitemid, Items[t_useitem].Quantity);
		}
		else
		{
			memset(&Items[t_useitem], 0, sizeof(Items[t_useitem]));
			uChar->Backpack[t_useitemid].ID = 0;
			RemoveItemFromPlayer(Know_ID, ItemBackpack, t_useitemid);
		}
	}
	else
	{
		//========================
		// Create forged item.
		long TempID;
		short TmpID;
		TmpID = GetFreeBackpackSlot(Know_ID);
		if (TmpID == -1)
		{
			// Not enough space in backpack, abort forging.

			printf("Backpack is out of space.\n");
			
			return;
		}

		TempID = GetFreeItemID();
		if (TempID == -1) 
		{
			printf("ITEM ARRAY IS OUT OF SPACE!!!\n");
			return;
		}

		ItemStruct NewItem;
		memset(&NewItem, 0, sizeof(NewItem));
		NewItem.Family = t_tarfam;
		NewItem.ID = t_tarid;
		NewItem.Type = t_tartype;
		NewItem.Quantity = 1;
		NewItem.Used = true;
		NewItem.KnowID = TmpID;
		NewItem.x = rand() % 100;
		NewItem.y = rand() % 100;

		memcpy(&Items[TempID], &NewItem, sizeof(NewItem));
		uChar->Backpack[TmpID].ID = TempID;

		InformPlayerofItem(Know_ID, TempID, ItemBackpack);
		//========================


		//========================
		// Remove used ingots.
		if (Items[t_useitem].Quantity > t_cost)
		{
			Items[t_useitem].Quantity -= t_cost;
			InformPlayerOfNewAmount(Know_ID, ItemBackpack, t_useitemid, Items[t_useitem].Quantity);
		}
		else
		{
			memset(&Items[t_useitem], 0, sizeof(Items[t_useitem]));
			uChar->Backpack[t_useitemid].ID = 0;
			RemoveItemFromPlayer(Know_ID, ItemBackpack, t_useitemid);
		}
		//========================


		buffer[0] = 99;
		buffer[1] = 76;
		send(ClientSocket, buffer, 2, 0);
	}

	//========================
	// Check exp.
	if (uChar->Level != MaxLevel)
	{
		t_recvexp *= (1 + (float)Acc->tChar.Int / 100 );
		uChar->Exp += t_recvexp;

		buffer[0] = 11;
		buffer[1] = 2;

		memcpy(&buffer[2], &uChar->Exp, sizeof(uChar->Exp));
	                
		send(ClientSocket, buffer, 2 + sizeof(uChar->Exp), 0);
	}

	//========================

	//========================
	// Check for level up.
	if (CheckLevelGain(Know_ID))
		InformPlayerOfLevelUp(Know_ID);
	//========================

	//========================
	// Check skill exp.
	uChar->Skills[16].Exp += t_recvexp;
	if (CheckSkillLevelGain(Know_ID, 16))
		InformPlayerOfNewSkill(Know_ID, 16);		
	//========================


	//========================
	// Inform CT
	memset(&NewCT, 0, sizeof(NewCT));		
		if (uChar->Skills[16].Level != 100)
		{	
			buffer[0] = 97;
			buffer[1] = 1;
			
			NewCT.Amount = t_recvexp;
			NewCT.Type = CT_EXPRECEIVED;
			NewCT.SkillType = 16;
					
			memcpy(&buffer[2], &NewCT, sizeof(NewCT));
					
			send(ClientSocket, buffer, 2 + sizeof(NewCT), 0);
		} else {
			if (uChar->Level != MaxLevel)
			{
				buffer[0] = 97;
				buffer[1] = 1;
				
				NewCT.Amount = t_recvexp;
				NewCT.Type = CT_EXPRECEIVED_MAX;
				NewCT.SkillType = 16;
				
				memcpy(&buffer[2], &NewCT, sizeof(NewCT));
				
				send(ClientSocket, buffer, 2 + sizeof(NewCT), 0);		
			}
		}
	//========================

	CheckStats(Know_ID);
}


int GuildClass::demote(int member)
{
	Members[member].rank--;
	if (Members[member].rank < 0)
	{
		Members[member].rank = 0;
		return -1;
	}	
	return 0;
}


int GuildClass::promote(int member)
{
	if (amount_ranks(Members[member].rank + 1) > 3 && Members[member].rank + 1 > 3) return -1; 
	Members[member].rank++;
	if (Members[member].rank >= 3)
	{
		Members[member].rank = 3;
		return -1;
	}
	return 0;
}

int GuildClass::amount_ranks(int rank)
{
	short count = 0;
	for (int i = 0; i < 50; i++)
	{
		if (Members[i].rank == rank || rank == -1)
			count++;
	}

	return count;
}


void ClientClass::infoMsg(char Msg[120])
{
	PACKET_NEWINFOMSG NewInfo;
	strcpy(NewInfo.Msg, Msg);
	buffer[0] = 100;
	buffer[1] = 0;
	memcpy(&buffer[2], &NewInfo, sizeof(NewInfo));
	send(ClientSocket, buffer, 2 + sizeof(NewInfo), 0);
}
