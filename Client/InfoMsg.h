#pragma once



class InfoMsg
{
public:

	InfoMsg *Next, *Prev;
	class InfoMsgClass *base;

	unsigned long iTime;
	char iMsg[256];
	
	void addTo();
	void remove();
};


class InfoMsgClass
{
public:

	InfoMsgClass();
	~InfoMsgClass();

	InfoMsg *Msg;	

	void newInfoMsg(char const *msg, ...);
	void draw();
};
