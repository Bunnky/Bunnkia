
#ifndef quickslots_h
#define quickslots_h


#include "main.h"


//----------------------------------------------------------
// Quick key slots class
class QuickSlotClass
{
public:

	bool ShowQuickSlots;

	short QuickSlotTypes[10]; // Useable item, spell, ..
	short QuickSlotTargets[10];

	bool is_slotitem(short id, short type, short family);

	void FreeSlot( short Slot );
	void SetSlot( short Slot, short Type, short Target );
	void CheckQuickSlots( int key );

	void Draw();
};

extern QuickSlotClass QK;
//----------------------------------------------------------


#endif