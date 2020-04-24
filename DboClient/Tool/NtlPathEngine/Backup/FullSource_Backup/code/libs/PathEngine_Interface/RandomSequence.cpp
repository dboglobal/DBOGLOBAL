//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "RandomSequence.h"

cRandomSequence::cRandomSequence()
{
    _seed = 0;
}

tSigned32
cRandomSequence::getSeed() const
{
    return _seed;
}
void
cRandomSequence::setSeed(tSigned32 value)
{
    _seed = value;
}
tSigned32
cRandomSequence::random15()
{
    _seed = _seed * tSigned32(214013L) + tSigned32(2531011L); //... platform dependant?
    return (_seed >> 16) & 0x7fff;
}
tSigned32
cRandomSequence::random31()
{
    tSigned32 result = random15();
    result <<= 15;
    result ^= random15();
    result <<= 1;
    result ^= random15();
    return result;
}
