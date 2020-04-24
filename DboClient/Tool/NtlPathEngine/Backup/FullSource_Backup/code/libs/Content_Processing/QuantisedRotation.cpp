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
#include "QuantisedRotation.h"
#include "common/interface/Assert.h"

inline tSigned32
CosOf_90Degrees(tSigned32 rotation)
{
    assertD(rotation >= 0 && rotation <= 16);
    static const tSigned32 table[] = {4096, 4076, 4017, 3920, 3784, 3612, 3406, 3166, 2896, 2598, 2276, 1931, 1567, 1189, 799, 401, 0};
    return table[rotation];
}

inline tSigned32
CosOf_360Degrees(tSigned32 rotation)
{
    switch(rotation >> 4)
    {
    default:
        invalid();
    case 0:
        return CosOf_90Degrees(rotation);
    case 1:
        return -CosOf_90Degrees(32 - rotation);
    case 2:
        return -CosOf_90Degrees(rotation - 32);
    case 3:
        return CosOf_90Degrees(64 - rotation);
    }
}

inline tSigned32
SinOf_360Degrees(tSigned32 rotation)
{
    return CosOf_360Degrees((rotation + 48) & 63);
}

void
QuantisedRotationCoefficients(tSigned32 rotation, tSigned32& sinOf, tSigned32& cosOf)
{
    assertD(rotation >= 0 && rotation < 64);
    sinOf = SinOf_360Degrees(rotation);
    cosOf = CosOf_360Degrees(rotation);
}

