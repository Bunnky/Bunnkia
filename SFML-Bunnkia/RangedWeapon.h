#ifndef RANGEDWEAPON_H
#define RANGEWEAPON_H

#include "Item.h"

class Item;

class RangedWeapon :
    public Item
{
private:

public:
    RangedWeapon(unsigned value);
    virtual ~RangedWeapon();

    //Functions
    virtual RangedWeapon* clone() = 0;
};

#endif //RANGEDWEAPON_H