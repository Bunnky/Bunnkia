#ifndef drag_h
#define drag_h

#include "main.h"


//----------------------------------------------------------------------------
// This class contains all possible information of item we are dragging.
struct DraggingClass
{
	// Is dragging on?
	bool On; 
	
	// Are we going to split some item?
	bool Split; 
	// Amount of the split
	unsigned long SplitAmount; 

	short ItemID;

	unsigned char ToList;
	unsigned char FromList;

	char Txt[20];

	int Mode; // Drag: 1) Backpack, 2) Minimap, 3) some Item

		
	short int x,y; // In case of Backpack/Minimap/Bank/Identify, we need to take x,y of dragging position to make dragging look better.
	short int spx, spy; // Coordinates for new pile of splitted item.

	ItemClass *TargetItem; // Target Item to drag
	class identify *iddrag;

	void Start (int MODE); // Start Dragging.

	void Stop () // Stop Dragging.
	{ 
		if (!Split)
		TargetItem = NULL;
		On = false; 
		x = 0; 
		y = 0;
	}
	
	void Release();
};

extern DraggingClass Dragging;
//------------------------------------------------------------------


#endif