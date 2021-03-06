#ifndef ITEM_H
#define ITEM_H

enum ItemTypes { IT_DEFAULT = 0, IT_RANGEDWEAPON, IT_MELEEWEAPON };
enum ItemRarities { COMMON = 0, UNCOMMON, RARE, EPIC, LEGENDARY, MYTHICAL };

class Item
{
private:
	void initVariables();

protected:

public:
	//Variables
	short unsigned type;
	unsigned level;
	unsigned value;

	Item(unsigned level, unsigned value);
	virtual ~Item();

	//Accessors
	const short unsigned& getType() const { return this->type; }
	const unsigned& getLevel() const { return this->level; }
	const unsigned& getValue() const { return this->value; }

	//Functions
	virtual Item* clone() = 0;
};

#endif //Item.h