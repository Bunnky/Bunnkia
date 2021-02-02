#include "sdl/include/sdl.h"

#include "InfoMsg.h"
#include "main.h"


void InfoMsg::remove()
{
	if (base->Msg == this)
	{
		base->Msg = base->Msg->Next;
		if (base->Msg != NULL)
			base->Msg->Prev = NULL;
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

void InfoMsg::addTo()
{
	if (base->Msg == NULL)
	{
		Prev = NULL;
		Next = NULL;
		base->Msg = this;
	}
	else
	{
		Prev = NULL;
		Next = base->Msg;
		base->Msg->Prev = this;
		base->Msg = this;
	}
}

void InfoMsgClass::newInfoMsg(char const *Msg, ...)
{
	InfoMsg *New = new InfoMsg;

	char buf[256];
	
    va_list args;

    va_start (args,Msg);
    _vsnprintf(buf,256,Msg,args);
    va_end(args);

    buf[255] = '\0';
    strcpy(New->iMsg, buf);	

	New->iTime = SDL_GetTicks();
	New->base = this;
	New->addTo();
}

InfoMsgClass::InfoMsgClass()
{
	Msg = NULL;
}

InfoMsgClass::~InfoMsgClass()
{
	InfoMsg *tmp;
	while (Msg)
	{
		tmp = Msg->Next;
		delete Msg;
		Msg = tmp;
	}
}

void InfoMsgClass::draw()
{
	InfoMsg *tmp, *tmptmp;
	tmp = Msg;
	int count = 0;
	while (tmp)
	{
		tmptmp = tmp->Next;
		if (SDL_GetTicks() - tmp->iTime > 30000)
			tmp->remove();
		else sdl.text(tmp->iMsg, 10, 415 - count * 12, col_grey);
	
		tmp = tmptmp; count++;
	}
}