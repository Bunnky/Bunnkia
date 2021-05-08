#ifndef ITEM_H
#define ITEM_H

enum ItemTypes {IT_DEFAULT = 0, IT_RANGEDWEAPON, IT_MELEEWEAPON};
enum ItemRarities {COMMON = 0, UNCOMMON, RARE, EPIC, LEGENDARY, MYTHICAL};

class Item
{
private:
	void initVariables();

protected:

public:
	//Variables
	short unsigned type;
	unsigned value;

	Item(unsigned value);
	virtual ~Item();

	//Functions
	virtual Item* clone() = 0;
};



#endif //Item.h