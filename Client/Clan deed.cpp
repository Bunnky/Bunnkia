//------------------------------------------------------//
//   Deed source file ( Clan deed.cpp )					//
//														//
//   Includes functions of clan deed class				//
//						   ( class DeedClass )			//
//														//
//   (c) Jussi Witick									//
//------------------------------------------------------//


#include "main.h"

#pragma warning(disable:4244)

const char *guildranks[] =
{
	"Recruit",
	"Member",
	"Elder",
	"Council",
	"Leader"
};

//-----------------------------------------------------------------------------------
//
void DeedClass::MouseClick()
{
	buttons->mouseclick();

	Uint16 mx, my;

	mx = event.motion.x;
	my = event.motion.y;

	if (Wait_Action && SelectedAction == 0) Wait_Action = false;

	if (!Wait_Action && SelectedAction > 0 ) SelectedAction = 0;

	if (Wait_Action && SelectedAction)
	{
		fprintf(stderr, "debugs\n");
		if (MX >= 7 && MX <= 427 && MY >= 7 && MY <= 427)
		{
			short tx,ty;

			PlayerNode *tmp;
			tmp = pldatabase->getlast();

			fprintf(stderr, "Guild Add #1\n");
			while (tmp)
			{
				fprintf(stderr, "Guild Add #2\n");
				tx = (-pldatabase->own->x + tmp->x + 10) * 20;
				ty = (-pldatabase->own->y + tmp->y + 10) * 20;

				if ( mx - tx >= 0 && mx - tx <= 19 && my - ty >= 0 && my - ty <= 19)
				{
					fprintf(stderr, "Guild Add #3\n");
					if (tmp != pldatabase->own)
					{						
						fprintf(stderr, "Guild Add #5\n");
						
						sprintf(MB.NewMsg, "Request to add [Name]: %s to the guild", tmp->name);
						MB.NewType = 7;
						MB.Update();

						SelectedAction = tmp->id;
						Execute_Action_Add();
					} else {
						sprintf(MB.NewMsg, "No player selected!");
						MB.NewType = 7;
						MB.Update();
					}
					tmp = NULL;
				}

				if (tmp != NULL) tmp = tmp->prev;
			}
		}
		Wait_Action = false;
		SelectedAction = 0;
		Use = false;
		return;
	}
	

	if (mx >= 774 && mx <= 789 && my >= 7 && my <= 23)
	{
		On = false;
		SelectedAction = 0;
		Wait_Action = false;
		return;
	}

	if (mx >= 445 && mx <= 745 && my >= 120 && my <= 450)
	{
		int i;
		for (i = 0; i < 50; i++)
		{
			if (i + Scroll < 50)
			{
				if (mx >= 445 && mx <= 445 + BFont_TextWidth( Names[i + Scroll] ) && my >= 120 + i * 12 && my <= 130 + i * 12 && UsedSlot[i + Scroll])
				{
					Selected = i + Scroll;
					i = 50;
				}
			}			
		}
		return;
	}

	if (Clan->IsLeader)
	{
		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Add") && MY >= 550 && MY <= 560)
		{
			// Add

			Wait_Action = true;
			SelectedAction = 1;

			Use = true;
			
			return;
		}

		if (MX >= 480 && MX <= 480 + BFont_TextWidth("Remove") && MY >= 550 && MY <= 560)
		{
			// remove

			SelectedAction = Selected;
			Execute_Action_Remove();
			
			SelectedAction = 0;
			
			return;
		}

		if (mx >= 600 && mx <= 600 + BFont_TextWidth("Transfer") && my >= 562 && my <= 572)
		{
			// Transfer deed

			SelectedAction = Selected;
			Execute_Action_Transfer();
			
			SelectedAction = 0;
			
			return;
		}

		if (MX >= 600 && MX <= 600 + BFont_TextWidth("Disband") && MY >= 550 && MY <= 560)
		{
			// disband

			Execute_Action_Disband();

			return;
		}

		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Promote") && MY >= 562 && MY <= 572)
		{
			// promote

			SelectedAction = IDs[Selected];
		
			buffer[0] = 50;
			buffer[1] = 8;
			memcpy(&buffer[2], &SelectedAction, sizeof(SelectedAction));
			send(Socket, buffer, 2 + sizeof(SelectedAction), 0);

			SelectedAction = 0;
			
			return;
		}
		if (MX >= 500 && MX <= 500 + BFont_TextWidth("Demote") && MY >= 562 && MY <= 572)
		{
			// demote

			SelectedAction = IDs[Selected];
		
			buffer[0] = 50;
			buffer[1] = 9;
			memcpy(&buffer[2], &SelectedAction, sizeof(SelectedAction));
			send(Socket, buffer, 2 + sizeof(SelectedAction), 0);

			SelectedAction = 0;
			
			return;
		}
		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Clan name: ") + BFont_TextWidth(Name) 
			&& MY >= 15 && MY <= 25)
		{
			memset(&Txt, 0, sizeof(Txt));
			Writing.Writing = true;
			Writing.Mode = 40;
			Writing.KeyHandler = strlen(Name);
			strcpy(Txt, Name);
		}

		char x1[60];
		sprintf(x1, "Clan name: %s", Name);
		int _x1;
		_x1 = BFont_TextWidth(x1);

		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Clan tag: ") + BFont_TextWidth(Tag)
			&& MY >= 27 && MY <= 37)
		{
			memset(&Txt, 0, sizeof(Txt));
			Writing.Writing = true;
			Writing.Mode = 41;
			Writing.KeyHandler = strlen(Tag);
			strcpy(Txt, Tag);
		}
	} else {		
		if (MX >= 724 && MX <= 724 + BFont_TextWidth("Leave") && MY >= 572 && MY <= 582)
		{
			// Leave

			buffer[0] = 50;
			buffer[1] = 5;
			send(Socket, buffer, 2, 0);
			Clan->On = false;
			Use = false;
			Clan->Wait_Action = false;
			Clan->SelectedAction = 0;
			
			return;
		}
	}
}
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
void DeedClass::Deed()
{
	sdl.dst.x = 425;
	sdl.dst.y = 0;
	sdl.dst.h = 490;
	sdl.dst.w = 375;

	SDL_BlitSurface(G.List, NULL, Screen.screen, &sdl.dst);

	char tmp[120];

	if (Writing.Writing && Writing.Mode == 40)
	{
		char tmp[120];
		sprintf(tmp, "Clan name: %s|", Name);
		int _x1;
		_x1 = BFont_TextWidth(tmp);

		
		sdl.text("Clan name: ", 440, 15, col_white);
		sprintf(tmp, "%s|", Txt);
		sdl.text(tmp, 450 + BFont_TextWidth("Clan name: "), 15, col_grey);
	

		sdl.text("Clan tag: ", 440, 27, col_gray);
		sdl.text(Tag, 450 + BFont_TextWidth("Clan tag: "), 27, col_blue);
	}	
	else if (Writing.Writing && Writing.Mode == 41)
	{
		sprintf(tmp, "Clan name: ì%s", Name);
		int _x1;
		_x1 = BFont_TextWidth(tmp);

		sdl.text(tmp, 440, 15, col_gray);
	

		sdl.text("Clan tag: ", 440, 27, col_white);
		sprintf(tmp, "%s|", Txt);
		sdl.text(tmp, 450 + BFont_TextWidth("Clan tag: "), 27, col_grey);
	}
	else
	{
		int tmpcol = col_gray;
		sprintf(tmp, "Clan name: ì%s", Name);
		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Clan name: ") + BFont_TextWidth(Name) 
			&& MY >= 15 && MY <= 25)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text(tmp, 440, 15, tmpcol);
	
		int _x1;
		_x1 = BFont_TextWidth(tmp);
		
		sprintf(tmp, "Clan tag: æ%s", Tag);

		if (MX >= 440 && MX <= 440 + BFont_TextWidth(tmp)
			&& MY >= 27 && MY <= 37)
			tmpcol = col_grey;
		else tmpcol = col_gray;
		sdl.text(tmp, 440, 27, tmpcol);
	}

	int In = 0, i;
	for (i = 0; i < 50; i++)
	{
		if (UsedSlot[i])
			In++;
	}
	sprintf(tmp, "Members: Ä%dÅ / Ä50Å", In);
	sdl.text(tmp, 440, 39, col_grey);

	sprintf(tmp, "Your rank: Ä%sÅ", guildranks[own_rank]);
	sdl.text(tmp, 440, 51, col_grey);
}

//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
void DeedClass::D_Names()
{
	int counter = 0;
	int i;
	int tmpcol;
	for (i = 0; i < 50; i++)
	{		
		if (i >= Scroll)
		{
			if (counter < 32)
			{
				if (UsedSlot[i])
				{					
					if (Status[i] == true)
						tmpcol = col_green;
					else
						tmpcol = col_red;

					if (MX >= 445 && MX <= 445 + BFont_TextWidth(Names[i]) && MY >= 120 + counter * 12 && MY < 130 + counter * 12)
					{
						tmpcol = col_orange;
					} 
					else
					{
		                if (i == Selected)
							tmpcol = col_white;
					}

					sdl.text(Names[i], 445, 120 + counter * 12, tmpcol);
					counter++;
				}
			}
		}
	} 
}
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
void DeedClass::Actions()
{
	buttons->draw();

	// Actions:

//	Add, remove, disband, transfer

	int tmpcol;
	
	if (Clan->IsLeader)
	{
		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Add") && MY >= 550 && MY <= 560)
			tmpcol = col_grey;
		else if (Selected != -1)
			tmpcol = col_greenish;
		else tmpcol = col_gray;

		sdl.text("Add", 440, 550, tmpcol);


		if (MX >= 480 && MX <= 480 + BFont_TextWidth("Remove") && MY >= 550 && MY <= 560)
			tmpcol = col_greenish;
		else if (Selected != -1)
			tmpcol = col_grey;
		else tmpcol = col_gray;
			
		sdl.text("Remove", 480, 550, tmpcol);


		if (MX >= 600 && MX <= 600 + BFont_TextWidth("Disband") && MY >= 550 && MY <= 560)
			tmpcol = col_darkred;
		else 
			tmpcol = col_red;

		sdl.text("Disband", 600, 550, tmpcol);


		if (MX >= 440 && MX <= 440 + BFont_TextWidth("Promote") && MY >= 562 && MY <= 572)
			tmpcol = col_grey;
		else if (Selected != -1)
			tmpcol = col_greenish;
		else tmpcol = col_gray;

		sdl.text("Promote", 440, 562, tmpcol);


		if (MX >= 500 && MX <= 500 + BFont_TextWidth("Demote") && MY >= 562 && MY <= 572)
			tmpcol = col_greenish;
		else if (Selected != -1)
			tmpcol = col_grey;
		else tmpcol = col_gray;
			
		sdl.text("Demote", 500, 562, tmpcol);


		if (MX >= 600 && MX <= 600 + BFont_TextWidth("Transfer") && MY >= 562 && MY <= 572)
			tmpcol = col_lightblue;
		else if (Selected != -1)
			tmpcol = col_blue;
		else tmpcol = col_gray;

		sdl.text("Transfer", 600, 562, tmpcol);

		if (MX >= 600 && MX <= 600 + BFont_TextWidth("Transfer") && MY >= 562 && MY <= 572)
			tmpcol = col_lightblue;
		else if (Selected != -1)
			tmpcol = col_blue;
		else tmpcol = col_gray;

		sdl.text("Transfer", 600, 562, tmpcol);

	} else {
		if (MX >= 724 && MX <= 724 + BFont_TextWidth("Leave") && MY >= 572 && MY <= 582)
			tmpcol = col_darkred;
		else tmpcol = col_red;

		sdl.text("Leave", 724, 572, tmpcol);
	}

}
//-----------------------------------------------------------------------------------

void DeedClass::Execute_Action_Disband()
{
	buffer[0] = 50;
	buffer[1] = 6;
	send(Socket, buffer, 2, 0);
	On = false;
}

void DeedClass::Execute_Action_Add()
{
	buffer[0] = 50;
	buffer[1] = 3;
	memcpy(&buffer[2], &SelectedAction, sizeof(SelectedAction));
    send(Socket, buffer, 2 + sizeof(SelectedAction), 0);
	SelectedAction = 0;
}

void DeedClass::Execute_Action_Remove()
{
	buffer[0] = 50;
	buffer[1] = 4;
	memcpy(&buffer[2], &IDs[SelectedAction], sizeof(IDs[SelectedAction]));
    send(Socket, buffer, 2 + sizeof(IDs[SelectedAction]), 0);
	SelectedAction = 0;
}

void DeedClass::Execute_Action_Transfer()
{
	buffer[0] = 50;
	buffer[1] = 7;
	memcpy(&buffer[2], &IDs[SelectedAction], sizeof(IDs[SelectedAction]));
    send(Socket, buffer, 2 + sizeof(IDs[SelectedAction]), 0);
	SelectedAction = 0;
}


DeedClass::DeedClass()
{
	memset(&IDs, 0, sizeof(IDs));
	memset(&ranks, 0, sizeof(ranks));
	memset(&Names, 0, sizeof(Names));
	memset(&UsedSlot, 0, sizeof(UsedSlot));

	buttons = new buttoncontrol;

	button *n;
	n = new button(buttons, 774, 89, 15, 15, secure_sup_inactive, 10);
	n->addto();
	n = NULL;
	n = new button(buttons, 774, 540, 15, 15, secure_sdown_inactive, 11); // scroll up
	n->addto();

	Action_Target = -1;
	IsLeader = false;
	memset(&Name, 0, sizeof(Name));
	On = false;
	own_rank = 0;
	Scroll = 0;
	Selected = -1;
	SelectedAction = -1;
	Wait_Action = false;
	memset(&Tag, 0, sizeof(Tag));
}

DeedClass::~DeedClass()
{
	delete buttons;
}