#ifndef secure_trade_h
#define secure_trade_h


#include "main.h"





class SecureTradeClass
{
public:

	bool TradeInProgress, TradeInPending;

	bool Lock[2];

	short Traders[2];

	short tradeitems[2][16];

	unsigned long LastAction;

	void do_trade();

	void timeCheck();
	void Abort();
	void AbortbyID(short ID);
};


#endif