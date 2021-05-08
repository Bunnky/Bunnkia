#include "stdafx.h"
#include "Bow.h"

Bow::Bow(unsigned value, std::string texture_file)
    : RangedWeapon(value, texture_file)
{

}

Bow::~Bow()
{

}

Bow* Bow::clone()
{
    return new Bow(*this);
}
