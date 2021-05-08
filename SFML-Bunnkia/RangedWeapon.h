#ifndef RANGEDWEAPON_H
#define RANGEWEAPON_H

#include "Item.h"

class Item;

class RangedWeapon :
    public Item
{
private:

public:
    RangedWeapon();
    virtual ~RangedWeapon();

    //Functions
    virtual RangedWeapon* clone() = 0;
};

#endif //RANGEDWEAPON_H