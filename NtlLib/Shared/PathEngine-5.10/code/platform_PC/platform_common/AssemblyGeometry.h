//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef ASSEMBLY_GEOMETRY_INCLUDED
#define ASSEMBLY_GEOMETRY_INCLUDED

#include "libs/Geometry/interface/SideEnum.h"
#include "platform_support/LargeIntegers.h"

namespace nAssemblyGeometry
{

inline eSide
side(tSigned32 baseX, tSigned32 baseY, tSigned32 testX, tSigned32 testY)
{
//    assertC(SIDE_CENTRE == 0);
//    assertC(SIDE_LEFT == 1);
//    assertC(SIDE_RIGHT == 2);
    tSigned32 lhs, rhs;
    _asm
    {
        mov eax, baseY
        imul testX
        jc asmlabel_multipliedoutof32bits
        mov lhs, eax
        mov eax, baseX
        imul testY
        jc asmlabel_multipliedoutof32bits
        mov rhs, eax
    }
    if(lhs < rhs)
    {
        return SIDE_LEFT;
    }
    if(lhs == rhs)
    {
        return SIDE_CENTRE;
    }
    return SIDE_RIGHT;
asmlabel_multipliedoutof32bits:
    cLargeInteger2 lhs64, rhs64;
    nLargeIntegers::multiply(testX, baseY, lhs64);
    nLargeIntegers::multiply(testY, baseX, rhs64);
    if(lhs64 < rhs64)
    {
        return SIDE_LEFT;
    }
    if(lhs64 == rhs64)
    {
        return SIDE_CENTRE;
    }
    return SIDE_RIGHT;
}

}

#endif
