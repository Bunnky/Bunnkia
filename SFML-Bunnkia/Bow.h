#ifndef BOW_H
#define BOW_H

#include "RangedWeapon.h"

class RangedWeapon;

class Bow :
    public RangedWeapon
{
private:

public:
    Bow(unsigned value, std::string texture_file);
    virtual ~Bow();

    virtual Bow* clone();
};

#endif //BOW_H