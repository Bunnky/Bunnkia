//---------------------------------------------------//
//   GM tool source file ( GMTool.cpp )			     //
//													 //
//   Includes functions of Online List Class         //
//						   ( class OnlineListClass)  //
//													 //
//   (c) Jussi Witick								 //
//---------------------------------------------------//


#include "main.h"


OnlineListClass::OnlineListClass()
{
//	memset(&IDs, -1, sizeof(IDs));
//	memset(&Names, 0, sizeof(Names));
//	memset(&UsedSlot, 0, sizeof(UsedSlot));
	memset(&ID, 0, sizeof(ID));
	memset(&AM, -1, sizeof(AM));

	textlinks = new textlinkcontrol;
	mtextlinks = new textlinkcontrol;
	spmodelinks = new textlinkcontrol;
	simodelinks = new textlinkcontrol; 
	olmodelinks = new textlinkcontrol; 

	buttons = new buttoncontrol;
	spbuttons = new buttoncontrol;
	sibuttons = new buttoncontrol;
	olbuttons = new buttoncontrol;

	button *n;
	n = new button(buttons, 774, 89, 15, 15, secure_sup_inactive, 12);
	n->addto();
	n = NULL;
	n = new button(buttons, 774, 540, 15, 15, secure_sdown_inactive, 13);
	n->addto();
	n = NULL;
	n = new button(buttons, 774, 6, 15, 15, secure_bexit_inactive, 14);
	n->addto();
	n = NULL;
	n = new button(buttons, 431, 90, 15, 30, button_all_inactive, 15);
	n->addto();


	n = NULL;
	n = new button(spbuttons, 440, 420, 15, 45, button_change, 100);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 440, 440, 15, 45, button_change, 101);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 440, 460, 15, 45, button_change, 102);
	n->addto();


	n = NULL;
	n = new button(spbuttons, 600, 420, 15, 15, secure_sdown_inactive, 103);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 600, 440, 15, 15, secure_sdown_inactive, 104);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 600, 460, 15, 15, secure_sdown_inactive, 105);
	n->addto();

	n = NULL;
	n = new button(spbuttons, 675, 420, 15, 15, secure_sup_inactive, 106);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 675, 440, 15, 15, secure_sup_inactive, 107);
	n->addto();
	n = NULL;
	n = new button(spbuttons, 675, 460, 15, 15, secure_sup_inactive, 108);
	n->addto();

	n = NULL;
	n = new button(sibuttons, 540, 415, 15, 15, secure_sdown_inactive, 109);
	n->addto();
	n = NULL;
	n = new button(sibuttons, 540, 445, 15, 15, secure_sdown_inactive, 110);
	n->addto();
	n = NULL;
	n = new button(sibuttons, 540, 476, 15, 15, secure_sdown_inactive, 111);
	n->addto();

	n = NULL;
	n = new button(sibuttons, 620, 415, 15, 15, secure_sup_inactive, 112);
	n->addto();
	n = NULL;
	n = new button(sibuttons, 620, 445, 15, 15, secure_sup_inactive, 113);
	n->addto();
	n = NULL;
	n = new button(sibuttons, 620, 476, 15, 15, secure_sup_inactive, 114);
	n->addto();
	

	textlinks->NewTextlink((char*)"5min", 550, 75, 12, BFont_TextWidth("5min"), 6, 100001);
	textlinks->NewTextlink((char*)"10min", 590, 75, 12, BFont_TextWidth("10min"), 7, 100001);
	textlinks->NewTextlink((char*)"20min", 630, 75, 12, BFont_TextWidth("20min"), 8, 100001);
	textlinks->NewTextlink((char*)"30min", 670, 75, 12, BFont_TextWidth("30min"), 9, 100001);
	textlinks->NewTextlink((char*)"60min", 710, 75, 12, BFont_TextWidth("60min"), 10, 100001);

	textlinks->NewTextlink((char*)"Jail", 470, 90, 12, BFont_TextWidth("Jail"), 1, 100002);
	textlinks->NewTextlink((char*)"UnJail", 500, 90, 12, BFont_TextWidth("UnJail"), 2, 100002);

	mtextlinks->NewTextlink((char*)"Online list", 540, 10, 12, BFont_TextWidth("Online list"), 3, 100003);
	mtextlinks->NewTextlink((char*)"Spawn mobs", 640, 10, 12, BFont_TextWidth("Spawn mobs"), 4, 100004);
	mtextlinks->NewTextlink((char*)"Spawn item", 540, 30, 12, BFont_TextWidth("Spawn item"), 5, 100005);

	spmodelinks->NewTextlink((char*)"Spawn them!", 435, 550, 12, BFont_TextWidth("Spawn them!"), 11, 100006);
	simodelinks->NewTextlink((char*)"Create Item!", 435, 550, 12, BFont_TextWidth("Create Item!"), 12, 100007);

	On = false;

	onlineAmount = -1;

	ShowJoins = true;
	ShowLeaves = true;
	oPlayers = NULL;
	ToolMode = 0;
}


OnlineListClass::~OnlineListClass()
{
	delete buttons;
}



//----------------------------------------------------------------------------------
//
void OnlineListClass::Mouseclick()
{
	mtextlinks->mouseclick();
	
	switch (ToolMode)
	{
	case 0:

		Mouseclick_OListmode();

		break;

	case 1: 

		Mouseclick_SPmode();

		break;		

	case 2: 

		Mouseclick_SImode();

		break;	
	}
}
//----------------------------------------------------------------------------------

void OnlineListClass::Mouseclick_SImode()
{
	simodelinks->mouseclick();
	sibuttons->mouseclick();
}

void OnlineListClass::d_Spawnitems()
{
	simodelinks->draw();
	sibuttons->draw();

	sdl.text("- Weapons -", 440, 130, col_grey);
	sdl.text("Type 1 = Sword", 440, 150, col_grey);
	sdl.text("Type 2 = Axe", 440, 170, col_grey);
	sdl.text("Type 3 = Blunt", 440, 190, col_grey);
	sdl.text("Type 4 = Polarm", 440, 210, col_grey);

	sdl.text("- Gear -", 570, 230, col_grey);
	sdl.text("Type 11 = Armors", 570, 250, col_grey);
	sdl.text("Type 12 = Helms", 570, 270, col_grey);
	sdl.text("Type 13 = Shields", 570, 290, col_grey);
	sdl.text("Type 14 = Leggings", 570, 310, col_grey);
	sdl.text("Type 15 = Guants", 570, 330, col_grey);
	sdl.text("Type 16 = Rings", 570, 350, col_grey);
	sdl.text("Type 17 = Amulets", 570, 370, col_grey);

	sdl.text("- Other -", 640, 130, col_grey);
	sdl.text("Type 20 = Collectables", 640, 150, col_grey);
	sdl.text("Type 21 = Useables", 640, 170, col_grey);

	sdl.text("- Spawn Item -", 440, 390, col_grey);

	sdl.text("Item Type:", 440, 415, col_grey);	
		char txtType[200];
		sprintf(txtType, "%d", itemType);
		sdl.text(txtType, 580, 415, col_purple);

	sdl.text("Item ID: ", 440, 445, col_grey);
		char txtID[200];
		sprintf(txtID, "%d", itemID);
		sdl.text(txtID, 580, 445, col_purple);

	sdl.text("Item Amount:", 440, 475, col_grey);
		char txtAm[200];
		sprintf(txtAm, "%d", itemAm);
		sdl.text(txtAm, 580, 475, col_purple);
}

void OnlineListClass::Mouseclick_SPmode()
{
	spmodelinks->mouseclick();

	spbuttons->mouseclick();

	if (Changemode)
	{
		for (int i = 0; i < m_MonsterTypes; i++)
		{
			sdl.dst.x = 440 + ((i % 10) * 24);
			sdl.dst.y = 115 + ((i / 10) * 24);

			if (MX >= sdl.dst.x && MX <= sdl.dst.x + 20 && MY >= sdl.dst.y && MY <= sdl.dst.y + 20)
			{
				ID[Changetarget] = i;
				Changemode = false;
				Changetarget = -1;
				i = m_MonsterTypes;
			}	
		}
	}
}

void OnlineListClass::d_Spawnmode()
{
	spmodelinks->draw();
	spbuttons->draw();

	sdl.text("Spawn monsters", 440, 390, col_grey);
	sdl.text("Monster", 500, 405, col_grey);
	sdl.text("How many?", 600, 405, col_grey);

	for (int i = 0; i < m_MonsterTypes; i++)
	{
		sdl.dst.x = 440 + ((i % 10) * 24);
		sdl.dst.y = 115 + ((i / 10) * 24);
		sdl.dst.h = 20;
		sdl.dst.w = 20;
		SDL_BlitSurface(G.avatars, &MonsterTypes[i].Frames[0], Screen.screen, &sdl.dst);

		if (MX >= sdl.dst.x && MX <= sdl.dst.x + 20 && MY >= sdl.dst.y && MY <= sdl.dst.y + 20)
		{
			sdl.dst.x--;
			sdl.dst.y--;
			SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[selected_frame], Screen.screen, &sdl.dst);
		}	
	}

	for (int i = 0; i < 3; i++)
	{
		if (Changemode && Changetarget == i)
		{
			sdl.text("Choose", 500, 424 + i * 20, col_grey);
		}
		else
		{
			sdl.dst.x = 500;
			sdl.dst.y = 420 + i * 20;
			sdl.dst.h = 20;
			sdl.dst.w = 20;
			SDL_BlitSurface(G.avatars, &MonsterTypes[ID[i]].Frames[0], Screen.screen, &sdl.dst);
		}

		char txt[20];
		sprintf(txt, "%d", AM[i]);
		sdl.text(txt, 640, 420 + i * 20, col_purple);
	}
}


void OnlineListClass::Mouseclick_OListmode()
{
	buttons->mouseclick();
	textlinks->mouseclick();
	olmodelinks->mouseclick();
	olbuttons->mouseclick();


	Uint16 mx, my;

	mx = event.motion.x;
	my = event.motion.y;

/*	if (mx >= 574 && mx <= 590 && my >= 63 && my <= 80)
	{
		On = false;
		return;
	}
	if (mx >= 550 && mx <= 563 && my >= 75 && my <= 88)
	{
		GMTool.Scroll-=2;
		if (GMTool.Scroll < 0) GMTool.Scroll = 0;
		GMTool.GetListView();
	}
	if (mx >= 550 && mx <= 563 && my >= 458 && my <= 471)
	{
		GMTool.Scroll+=2;
		if (GMTool.Scroll > m_Connections - 33) GMTool.Scroll = 
			m_Connections -33;
		GMTool.GetListView();
	}*/

	if (mx >= 435 && mx <= 780 && my >= 118 && my <= 500)
	{
		int counter = 0, rcount = 0;
		char tmptxt[120];
	
		onlinePlayer *tmp;
		tmp = oPlayers;

		while (tmp && rcount < 35)
		{
			if (counter < Scroll)
				counter++;
			else
			{
				sprintf(tmptxt, "%d: %s", counter + rcount + 1, tmp->Name);
				if (MX >= 435 && MX <= 435 + BFont_TextWidth(tmptxt) && MY >= 114 + rcount * 12 && MY <= 124 + rcount * 12)
				{
					Selected = tmp;
					tmp = NULL;
				}
				rcount++;
			}
	
			if (tmp) tmp = tmp->Next;
		}
		return;
	}

		char str[200];

	if (MX >= 435 && MX <= 435 + BFont_TextWidth("Tele To") && MY >= 545 && MY <= 555 && Selected != NULL)
	{
		PACKET_TELEPORTTOOTHER ToOther;
		memset(&ToOther, 0, sizeof(ToOther));

		sprintf(ToOther.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 20;
		memcpy(&buffer[2], &ToOther, sizeof(ToOther));
		send(Socket, buffer, 2 + sizeof(ToOther), 0);

		return;
	}
	if (MX >= 505 && MX <= 505 + BFont_TextWidth("Tele") && MY >= 545 && MY <= 555 && Selected != NULL)
	{
		PACKET_TELEPORTOTHER TellyTo;
		memset(&TellyTo, 0, sizeof(TellyTo));

		sprintf(TellyTo.Name, "%s", Selected->Name);
		memcpy(&TellyTo.X, &pldatabase->own->x, sizeof(pldatabase->own->x));
		memcpy(&TellyTo.Y, &pldatabase->own->y, sizeof(pldatabase->own->y));

		buffer[0] = 60;
		buffer[1] = 11;
		memcpy(&buffer[2], &TellyTo, sizeof(TellyTo));
		send(Socket, buffer, 2 + sizeof(TellyTo), 0);

		return;
	}
	if (MX >= 575 && MX <= 575 + BFont_TextWidth("Kick") && MY >= 545 && MY <= 555 && Selected != NULL)
	{
		PACKET_KICKFROM kickChar;
		memset(&kickChar, 0, sizeof(kickChar));

		sprintf(kickChar.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 26;
		memcpy(&buffer[2], &kickChar, sizeof(kickChar));
		send(Socket, buffer, 2 + sizeof(kickChar), 0);

		return;
	}
		if (MX >= 645 && MX <= 645 + BFont_TextWidth("Ban") && MY >= 545 && MY <= 555 && Selected != NULL)
	{

		PACKET_BANFROM banChar;
		memset(&banChar, 0, sizeof(banChar));

		sprintf(banChar.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 27;
		memcpy(&buffer[2], &banChar, sizeof(banChar));
		send(Socket, buffer, 2 + sizeof(banChar), 0);

		return;
	}
	if (MX >= 720 && MX <= 720 + BFont_TextWidth("Follow") && MY >= 545 && MY <= 555 && Selected != NULL)
	{
		PACKET_FOLLOW strFollow;
		memset(&strFollow, 0, sizeof(strFollow));

		sprintf(strFollow.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 36;
		memcpy(&buffer[2], &strFollow, sizeof(strFollow));
		send(Socket, buffer, 2 + sizeof(strFollow), 0);

		return;
	}




	if (MX >= 435 && MX <= 435 + BFont_TextWidth("Unfollow") && MY >= 563 && MY <= 573 && Selected != NULL)
	{
		buffer[0] = 60;
		buffer[1] = 37;
		send(Socket, buffer, 2, 0);

		return;
	}
	if (MX >= 575 && MX <= 575 + BFont_TextWidth("Heal") && MY >= 563 && MY <= 573 && Selected != NULL)
	{
		PACKET_HEAL gmHeal;
		memset(&gmHeal, 0, sizeof(gmHeal));

		sprintf(gmHeal.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 43;
		memcpy(&buffer[2], &gmHeal, sizeof(gmHeal));
		send(Socket, buffer, 2 + sizeof(gmHeal), 0);

		return;
	}
	if (MX >= 720 && MX <= 720 + BFont_TextWidth("Kill") && MY >= 563 && MY <= 573 && Selected != NULL)
	{
		PACKET_KILL gmKill;
		memset(&gmKill, 0, sizeof(gmKill));

		sprintf(gmKill.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 42;
		memcpy(&buffer[2], &gmKill, sizeof(gmKill));
		send(Socket, buffer, 2 + sizeof(gmKill), 0);

		return;
	}



	if (MX >= 435 && MX <= 435 + BFont_TextWidth("Mute") && MY >= 580 && MY <= 590 && Selected != NULL)
	{
		PACKET_MUTE gmMute;
		memset(&gmMute, 0, sizeof(gmMute));

		sprintf(gmMute.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 46;
		memcpy(&buffer[2], &gmMute, sizeof(gmMute));
		send(Socket, buffer, 2 + sizeof(gmMute), 0);

		return;
	}
	if (MX >= 505 && MX <= 505 + BFont_TextWidth("Unmute") && MY >= 580 && MY <= 590 && Selected != NULL)
	{
		PACKET_UNMUTE gmUnMute;
		memset(&gmUnMute, 0, sizeof(gmUnMute));

		sprintf(gmUnMute.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 47;
		memcpy(&buffer[2], &gmUnMute, sizeof(gmUnMute));
		send(Socket, buffer, 2 + sizeof(gmUnMute), 0);

		return;
	}
	if (MX >= 575 && MX <= 575 + BFont_TextWidth("Backpack") && MY >= 580 && MY <= 590 && Selected != NULL)
	{				
		memset(&GMTool->iBankItems,		0, sizeof(GMTool->iBankItems));
		memset(&GMTool->iBackpackItems, 0, sizeof(GMTool->iBackpackItems));

		if (GMTool->bBackpackView == true)
		{
			GMTool->bBackpackView	= false;
			GMTool->bBankView		= false;		
		} else {
			GMTool->bBackpackView	= true;
			GMTool->bBankView		= false;
		}

		buffer[0] = 28;
		memcpy(&buffer[1], &GMTool->Selected->ID, sizeof(GMTool->Selected->ID));
		send(Socket, buffer, 1 + sizeof(GMTool->Selected->ID), 0);

		return;
	}
	if (MX >= 645 && MX <= 645 + BFont_TextWidth("Bank") && MY >= 580 && MY <= 590 && Selected != NULL)
	{
		memset(&GMTool->iBankItems,		0, sizeof(GMTool->iBankItems));
		memset(&GMTool->iBackpackItems, 0, sizeof(GMTool->iBackpackItems));

		if (GMTool->bBankView == true)
		{
			GMTool->bBackpackView	= false;
			GMTool->bBankView		= false;		
		} else {
			GMTool->bBackpackView	= false;
			GMTool->bBankView		= true;
		}

		buffer[0] = 29;
		memcpy(&buffer[1], &GMTool->Selected->ID, sizeof(GMTool->Selected->ID));
		send(Socket, buffer, 1 + sizeof(GMTool->Selected->ID), 0);

		return;
	}
	if (MX >= 720 && MX <= 720 + BFont_TextWidth("Info") && MY >= 580 && MY <= 590 && Selected != NULL)
	{
		PACKET_ASKSTATS AskStats;
		memset(&AskStats, 0, sizeof(AskStats));

		sprintf(AskStats.Name, "%s", Selected->Name);

		buffer[0] = 60;
		buffer[1] = 6;
		memcpy(&buffer[2], &AskStats, sizeof(AskStats));
		send(Socket, buffer, 2 + sizeof(AskStats), 0);

		return;
	}

}


void OnlineListClass::Draw_bGround()
{
	sdl.dst.x = 425;
	sdl.dst.y = 0;
	sdl.dst.h = 490;
	sdl.dst.w = 375;

	SDL_BlitSurface(G.List, NULL, Screen.screen, &sdl.dst);

	buttons->draw();
	mtextlinks->draw();
	olmodelinks->draw();
	olbuttons->draw();

	sdl.text("- GM Tools -", 435, 10, col_white);

	if (ToolMode == 1) 
	{
		d_Spawnmode();
		return;
	} else if (ToolMode == 2) 
	{
		d_Spawnitems();
		return;
	}

	Draw_Names();
	Draw_Actions();

	textlinks->draw();

	char tmp[120];
	sprintf(tmp, "Showing: Ä%dÅ -> Ä%dÅ", Scroll + 1, Scroll + 33);
	sdl.text(tmp, 430, 60, col_white);

	sdl.text("Give some counts:", 431, 75, col_white);

	sprintf(tmp, "Clients online: Ä%d/%dÅ", onlineAmount, m_Connections);
	sdl.text(tmp, 540, 60, col_white);
}

void OnlineListClass::Draw_Names()
{
	int counter = 0, rcount = 0;
	int col;
	char tmptxt[120];

	onlinePlayer *tmp;
	tmp = oPlayers;

	while (tmp && rcount < 35)
	{
		if (counter < Scroll)
			counter++;
		else
		{
			sprintf(tmptxt, "%d: %s", counter + rcount + 1, tmp->Name);
			if (MX >= 435 && MX <= 435 + BFont_TextWidth(tmptxt) && MY >= 114 + rcount * 12 && MY <= 124 + rcount * 12)
				col = col_grey;
			else
			{
				if (tmp == Selected)
					col = col_white;
				else col = col_gray;
			}

			sdl.text(tmptxt, 435, 114 + rcount * 12, col);
			rcount++;
		}
	
		tmp = tmp->Next;
	}
}

/*int OnlineListClass::GetListNumber(short Select)
{
	for (int i = 0; i < m_Connections; i++)
	{
		if (IDs[i] == Select) return i;
	}		
}*/

void OnlineListClass::Draw_Actions()
{
	// Actions:
	short tmpcol;
	if (Selected != NULL)
	{
			
		if (MX >= 435 && MX <= 435 + BFont_TextWidth("Tele To") && MY >= 545 && MY <= 555)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Tele To", 435, 545, tmpcol);

		if (MX >= 505 && MX <= 505 + BFont_TextWidth("Tele") && MY >= 545 && MY <= 555)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Tele", 505, 545, tmpcol);

		if (MX >= 575 && MX <= 575 + BFont_TextWidth("Kick") && MY >= 545 && MY <= 555)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Kick", 575, 545, tmpcol);

		if (MX >= 645 && MX <= 645 + BFont_TextWidth("Ban") && MY >= 545 && MY <= 555)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Ban", 640, 545, tmpcol);
			
		if (MX >= 720 && MX <= 720 + BFont_TextWidth("Follow") && MY >= 545 && MY <= 555)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Follow", 720, 545, tmpcol);


			
		if (MX >= 435 && MX <= 435 + BFont_TextWidth("Unfollow") && MY >= 563 && MY <= 573)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Unfollow", 435, 563, tmpcol);
		
		if (MX >= 575 && MX <= 575 + BFont_TextWidth("Heal") && MY >= 563 && MY <= 573)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Heal", 575, 563, tmpcol);

		if (MX >= 720 && MX <= 720 + BFont_TextWidth("Kill") && MY >= 563 && MY <= 573)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Kill", 720, 563, tmpcol);


			
		if (MX >= 435 && MX <= 435 + BFont_TextWidth("Mute") && MY >= 580 && MY <= 590)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Mute", 435, 580, tmpcol);

		if (MX >= 505 && MX <= 505 + BFont_TextWidth("UnMute") && MY >= 580 && MY <= 590)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("UnMute", 505, 580, tmpcol);

		if (MX >= 575 && MX <= 575 + BFont_TextWidth("Backpack") && MY >= 580 && MY <= 590)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Backpack", 575, 580, tmpcol);

		if (MX >= 645 && MX <= 645 + BFont_TextWidth("Bank") && MY >= 580 && MY <= 590)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Bank", 645, 580, tmpcol);

		if (MX >= 720 && MX <= 720 + BFont_TextWidth("Info") && MY >= 580 && MY <= 590)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text("Info", 720, 580, tmpcol);
	}
}


void OnlineListClass::dBackpackView()
{
	sdl.dst.x = 500;
	sdl.dst.y = 300;
	sdl.dst.w = Backpack_w;
	sdl.dst.h = Backpack_h;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BACKPACK], Screen.screen, &sdl.dst);	

	char x1[30];
	sprintf(x1, "%s's backpack", cIdOwnerName);

	int x2 = -BFont_TextWidth(x1) / 2 + Backpack_w / 2;

	sdl.text(x1, 500 + x2, 302, col_grey);

	int i, Thisturn = 0;
	for (i = 0; i < MaxBackpackItems; i++)
	{
		if (iBackpackItems[i].Used)
		{
			sdl.dst.h = 20;
			sdl.dst.w = 20;

			sdl.dst.x = iBackpackItems[i].x + 500 + 5;
			sdl.dst.y = iBackpackItems[i].y + 300 + 17;

			DrawItem(iBackpackItems[i].Family, iBackpackItems[i].Type, iBackpackItems[i].ID);

			if (MX >= iBackpackItems[i].x + 500 + 5 && MX <= iBackpackItems[i].x + 520 + 5 &&
				MY >= iBackpackItems[i].y + 300 + 17 && MY <= iBackpackItems[i].y + 320 + 17 && Identify)
			{
				Mouseover(iBackpackItems[i], Thisturn);
				Thisturn++;
			}
		}
	}
}

void OnlineListClass::dBankView()
{
	sdl.dst.x = 500;
	sdl.dst.y = 300;
	sdl.dst.w = Bank_w;
	sdl.dst.h = Bank_h;

	SDL_BlitSurface(G.addgf, &sdl.Rect_addgf[BANK], Screen.screen, &sdl.dst);

	char x1[30];
	sprintf(x1, "%s's bank", cIdOwnerName);

	int x2 = -BFont_TextWidth(x1) / 2 + Backpack_w / 2;

	sdl.text(x1, 500 + x2, 302, col_grey);

	int i, Thisturn = 0;
	for (i = 0; i < MaxBankItems; i++)
	{
		if (iBankItems[i].Used)
		{			
			sdl.dst.h = 20;
			sdl.dst.w = 20;

			sdl.dst.x = iBankItems[i].x + 500 + 5;
			sdl.dst.y = iBankItems[i].y + 300 + 17;

			DrawItem(iBankItems[i].Family, iBankItems[i].Type, iBankItems[i].ID);
			
			if (MX >= iBankItems[i].x + 500 + 5 && MX <= iBankItems[i].x + 520 + 5 &&
				MY >= iBankItems[i].y + 300 + 17 && MY <= iBankItems[i].y + 320 + 17 && Identify)
			{				
				Mouseover(iBankItems[i], Thisturn);
				Thisturn++;
			}
		}
	}
}

void OnlineListClass::Mouseover(ItemClass Item, int t)
{
	int len;
	char str[120];
	memset(&str, 0, sizeof(str));

	switch (Item.Family)
	{
	case FAMILY_WEAPON:

		sprintf(str, "%s", Weapons[ Item.Type ][ Item.ID ].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 570 - len / 2, 450, col_white);

		break;

	case FAMILY_ARMOR:

		sprintf(str, "%s", Armors[ Item.Type - 11 ][ Item.ID ].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 570 - len / 2, 450, col_white);

		break;

	case FAMILY_COLLECTABLE:

		char str[120];
		
		sprintf(str, "%s [Ä%luÅ]", Collectables[Item.ID].Name, Item.Quantity);
		
		len = BFont_TextWidth( str );
				
		sdl.text( str, 570 - len / 2, 450, col_white);

		break;

	case FAMILY_USEABLE:

		sprintf(str, "%s", Useables[Item.ID].Name);

		len = BFont_TextWidth( str );
		
		sdl.text( str, 570 - len / 2, 450, col_white);

		break;
	}
}

/*
void OnlineListClass::GetListView()
{
	memset(&ListView, -1, sizeof(ListView));

	int i = 0, times = 0, skipped = 0;
	while (i < m_Connections && times < 33)
	{
		if (skipped < Scroll) skipped++;
		else
		{
			if (UsedSlot[i])
			{
				ListView[times] = i;
				times++;
			}
		}
		i++;
	}
}
*/


void OnlineListClass::addPlayer(PACKET_NEWONLINEPLAYER NewPlayer)
{
	if (onlineAmount != -1)
		Info->newInfoMsg("User %s has connected. (%d users online)", NewPlayer.Name, onlineAmount);

	onlinePlayer *New = new onlinePlayer(this);

	New->base = this;

	New->ID = NewPlayer.ID;
	New->X = NewPlayer.X;
	New->Y = NewPlayer.Y;

	strcpy(New->Name, NewPlayer.Name);

	New->addTo();
}


void OnlineListClass::removeByID(long ID)
{
	onlinePlayer *tmp;
	tmp = oPlayers;
	while (tmp)
	{
		if (tmp->ID == ID)
		{
			Info->newInfoMsg("User %s has disconnected. (%d users online)", tmp->Name, onlineAmount);

			tmp->remove();
			tmp = NULL;
		}
		if (tmp != NULL) tmp = tmp->Next;
	}
}


void onlinePlayer::remove()
{
	if (base->oPlayers == this)
	{
		base->oPlayers = base->oPlayers->Next;
		if (base->oPlayers != NULL)
			base->oPlayers->Prev = NULL;
	}
	else
	{
		if (Prev != NULL)
			Prev->Next = Next;
		if (Next != NULL)
			Next->Prev = Prev;
	}
	delete this;
}

void onlinePlayer::addTo()
{
	if (base->oPlayers == NULL)
	{
		Prev = NULL;
		Next = NULL;
		X = 0;
		Y = 0;
		base->oPlayers = this;
	}
	else
	{
		onlinePlayer *tmp, *pre;
		
		tmp = base->oPlayers;
		pre = NULL;

		bool added = false;

		// PIENI BUGI, koska ensimmäisen nimen NEXT on aina NULL, niin ensimmäisenä listaan lisätty on aina viimesenä
		// Mahdollinen korjaus, koita saaha tmp->Next == NULL tarkastus pois
		while (tmp)
		{
			if ((int)tolower(Name[0]) < (int)tolower(tmp->Name[0]) || tmp->Next == NULL)
			{
				Prev = pre;

				Next = tmp;

				if (pre != NULL)
				{
					pre->Next = this;
				}
				else 
				{
					base->oPlayers->Prev = this;
					base->oPlayers = this;
				}

				tmp->Prev = this;
			
				added = true;
				tmp = NULL;
			}
			else if ((int)tolower(Name[0]) == (int)tolower(tmp->Name[0]))
			{
				if ((int)tolower(Name[1]) < (int)tolower(tmp->Name[1]) || tmp->Next == NULL)
				{
					Prev = pre; 

					Next = tmp;

					if (pre != NULL)
					{
						pre->Next = this;
					}
					else 
					{
						base->oPlayers->Prev = this;
						base->oPlayers = this;
					}

					tmp->Prev = this;
			
					added = true;
					tmp = NULL;
				}
			}

			if (!added)
			{
				pre = tmp;
				if (tmp != NULL)
					tmp = tmp->Next;
			}
			else tmp = NULL;
		}
	}
}

onlinePlayer::onlinePlayer(class OnlineListClass *base)
{
	Next = NULL;
	Prev = NULL;
	memset(&Name, 0, sizeof(Name));
	devMode = 0;
	specMode = 0;
	X = 0;
	Y = 0;
}

onlinePlayer::~onlinePlayer()
{	
}



void OnlineListClass::dropDownMenu(long id, short mx, short my)
{
	dDMenu = true;
	ddx = mx;
	ddy = my;
}

void OnlineListClass::dDDMenu()
{
	int commands = 5;
	
	short height, width;
	height = 66;
	width = BFont_TextWidth("Ban from srvr") + 10;

	short tmx, tmy;
	tmx = ddx;
	tmy = ddy - height;

	sdl.set_rect(tmx, tmy, width, height);
			
	SDL_FillRect(Screen.screen, &sdl.dst, SDL_MapRGB(Screen.screen->format, 0, 0, 0));

	for (int i = 0; i < 5; i++)
	{
		if (MX >= ddx + 3 && MX <= ddx + 3 + BFont_TextWidth(cmds[i]) && MY >= ddy + 3 - (60 - i * 12) && MY <= ddy + 15 - (60 - i * 12))
			sdl.text(cmds[i], ddx + 3, ddy - (60 - i * 12), col_white);
		else
			sdl.text(cmds[i], ddx + 3, ddy - (60 - i * 12), col_grey);
	}
}
