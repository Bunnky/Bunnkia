#include "textlink.h"

#include "main.h"

#pragma warning(disable:4244)


void textlink::pressed()
{
	switch (action)
	{
	case 1:

		char str[200];

		if (GMTool->Selected == NULL) return;

		PACKET_JAIL gmJail;
		memset(&gmJail, 0, sizeof(gmJail));

		sprintf(gmJail.Name, "%s", GMTool->Selected->Name);

		buffer[0] = 60;
		buffer[1] = 31;
		memcpy(&buffer[2], &gmJail, sizeof(gmJail));
		send(Socket, buffer, 2 + sizeof(gmJail), 0);

		sprintf(str, "You request: Jail Ä%sÅ.", GMTool->Selected->Name);
		MB.NewMessage(str, 3); 

		break;

	case 2: 

		if (GMTool->Selected == NULL) return;

		PACKET_UNJAIL gmUnJail;
		memset(&gmUnJail, 0, sizeof(gmUnJail));

		sprintf(gmUnJail.Name, "%s", GMTool->Selected->Name);

		buffer[0] = 60;
		buffer[1] = 32;
		memcpy(&buffer[2], &gmUnJail, sizeof(gmUnJail));
		send(Socket, buffer, 2 + sizeof(gmUnJail), 0);

		sprintf(str, "You request: UnJail Ä%sÅ.", GMTool->Selected->Name);
		MB.NewMessage(str, 3); 

		break;

	case 3:

		GMTool->ToolMode = 0;

		break;

	case 4:

		GMTool->ToolMode = 1;

		break;

	case 5:

		GMTool->ToolMode = 2;
		GMTool->itemAm = 0;
		GMTool->itemID = 0;
		GMTool->itemType = 0;

		break;

	case 6:
	case 7:
	case 8:
	case 9:
	case 10:

		if (GMTool->Selected == NULL) return;

		short amm;

		switch (action)
		{
		case 6: amm = 5; break;
		case 7: amm = 10; break;
		case 8: amm = 20; break;
		case 9: amm = 30; break;
		case 10: amm = 60; break;
		default: amm = 1; break;
		}


		buffer[0] = 60;
		buffer[1] = 30;
		buffer[2] = amm;

		memcpy(&buffer[3], &GMTool->Selected->ID, sizeof(GMTool->Selected->ID));

		send(Socket, buffer, 3 + sizeof(GMTool->Selected->ID), 0);

		sprintf(str, "You request: Add %d minutes for Ä%sÅ.", amm, GMTool->Selected->Name);
		MB.NewMessage(str, 3); 

		break;

	case 11: // Spawn the monsters

		PACKET_SPAWNMONSTERS Spawnit;
		memset(&Spawnit, 0, sizeof(Spawnit));

		memcpy(&Spawnit.AM, &GMTool->AM, sizeof(GMTool->AM));
		memcpy(&Spawnit.ID, &GMTool->ID, sizeof(GMTool->ID));

		buffer[0] = 60;
		buffer[1] = 28;
		memcpy(&buffer[2], &Spawnit, sizeof(Spawnit));
		send(Socket, buffer, 2 + sizeof(Spawnit), 0);

		break;

	case 12: // Spawn the items

		buffer[0] = 60;
		buffer[1] = 25;

		memcpy(&buffer[2], &GMTool->itemID, sizeof(GMTool->itemID));
		memcpy(&buffer[2 + sizeof(GMTool->itemID)], &GMTool->itemType, sizeof(GMTool->itemType));
		memcpy(&buffer[2 + sizeof(GMTool->itemID) + sizeof(GMTool->itemType)], &GMTool->itemAm, sizeof(GMTool->itemAm));
        send(Socket, buffer, 2 + sizeof(GMTool->itemID) + sizeof(GMTool->itemType) + sizeof(GMTool->itemAm), 0);
	
		sprintf(str, "You request -  Create item: Type[%d], ID[%d], Amount [%lu]", GMTool->itemType, GMTool->itemID, GMTool->itemAm);
		MB.NewMessage(str, 3); 

		break;

	case 13: // Close Game

		CleanUp();

		break;

	case 14:

		if (GMTool->Selected == NULL) return;

		GMTool->On = false;

		break;

	case 15: // Change Avatar

		if (GMTool->Selected == NULL) return;
		
		PACKET_CHANGEAVATAR newAv;
		memset(&newAv, 0, sizeof(newAv));

		memcpy(&newAv.Avatar, &GMTool->ID, sizeof(GMTool->ID));
	/*
		buffer[0] = 60;
		buffer[1] = 43283;
		memcpy(&buffer[2], &newAv, sizeof(newAv));
		send(Socket, buffer, 2 + sizeof(newAv), 0);
	*/
		break;

	case 101:		
		
		// 0 = human, 2 = orc
		if (Race->chosenRace == 0){
			Race->chosenRace = 2;
		} else {
			Race->chosenRace = 0;
		}

		// 0 = male, 1 = female
		Race->chosenAvatar = 0;

		break;

	case 102:

		if (Race->chosenRace == 0){
			Race->chosenRace = 2;
		} else {
			Race->chosenRace = 0;
		}

		Race->chosenAvatar = 0;

		break;

	}
}	

textlink::textlink(class textlinkcontrol *base2, char txt2[120], short x2, short y2, short h, short w, short action2, long id)
{
	strcpy(txt, txt2);
	base = base2;
	x = x2;
	y = y2;
	height = h;
	width = w;
	action = action2;
	ID = id;
	inv = true;
}

textlink::~textlink()
{

}


bool textlink::ison(short x2, short y2)
{
	if (((x - x2) * -1) <= width && ((x - x2) * -1) >= 0 &&
		((y - y2) * -1) <= height && ((y - y2) * -1) >= 0)
		return true;
	else return false;
}


void textlink::draw()
{
	short pic2;
	if (ison(MX, MY))
		pic2 = 1;
	else pic2 = 0;

	sdl.text(txt, x, y, pic2 == 0 ? col_grey : col_green);
	
	//	Surfaces->Text(txt, x, y, pic2 == 0 ? colGreen : colGrey);

	if (pic2 == 1)
		info();
}

void textlink::info()
{
	switch (action)
	{
		default:
		break;
	}
}

void textlink::addto()
{
	if (base->textlinks == NULL)
	{
		next = NULL;
		prev = NULL;
		base->textlinks = this;
	}
	else
	{
		next = base->textlinks;
		prev = NULL;
		base->textlinks->prev = this;
		base->textlinks = this;
	}	
}

void textlink::remove()
{
	if (base->textlinks == this)
	{
		base->textlinks = base->textlinks->next;
		base->textlinks->prev = NULL;
	}
	else
	{
		if (prev != NULL)
			prev->next = next;
		if (next != NULL)
			next->prev = prev;			
	}
	delete this;
}

void textlinkcontrol::clear()
{
	textlink *tmp;
	while (textlinks)
	{
		tmp = textlinks->next;
		delete textlinks;
		textlinks = tmp;	
	}
}

void textlinkcontrol::mouseclick()
{
	textlink *but;
	but = textlinks;
	while (but)
	{
		if (but->ison(MX, MY) && but->inv)
		{
			but->pressed();
			
			but = NULL;
		}
		if (but) but = but->next;
	}
}

void textlinkcontrol::draw()
{
	textlink *tmp;
	tmp = textlinks;
	while (tmp)
	{
		if (tmp->inv)
			tmp->draw();

		tmp = tmp->next;
	}
}

textlinkcontrol::textlinkcontrol()
{
	textlinks	= NULL;
}

textlinkcontrol::~textlinkcontrol()
{
	clear();
}

void textlinkcontrol::NewTextlink(char txt2[120], short x2, short y2, short h, short w, short action2, long id)
{
	//fprintf(stderr, "bu");
	textlink *New;
	//fprintf(stderr, "be");
	New = new textlink(this, txt2, x2, y2, h, w, action2, id);
	//fprintf(stderr, "bi");

	New->addto();
	//fprintf(stderr, "ba");
}


void textlinkcontrol::switchinv(long id, bool state)
{
	textlink *tmp = getbyid(id);

	tmp->inv = state;
}

textlink *textlinkcontrol::getbyid(long id)
{
	textlink *tmp;
	tmp = textlinks;
	while (tmp)
	{
		if (tmp->ID == id) return tmp;
	
		if (tmp) tmp = tmp->next;
	}
	return NULL;
}
