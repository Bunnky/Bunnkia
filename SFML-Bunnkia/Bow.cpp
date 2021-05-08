#include "stdafx.h"
#include "Bow.h"

Bow::Bow()
{

}

Bow::~Bow()
{

}

Bow* Bow::clone()
{
    return new Bow(*this);
}
