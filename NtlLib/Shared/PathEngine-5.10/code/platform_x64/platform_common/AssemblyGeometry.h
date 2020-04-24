//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
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
//    tSigned32 lhs, rhs;
//    _asm
//    {
//        mov eax, baseY
//        imul testX
//        jc asmlabel_multipliedoutof32bits
//        mov lhs, eax
//        mov eax, baseX
//        imul testY
//        jc asmlabel_multipliedoutof32bits
//        mov rhs, eax
//    }
//    if(lhs < rhs)
//    {
//        return SIDE_LEFT;
//    }
//    if(lhs == rhs)
//    {
//        return SIDE_CENTRE;
//    }
//    return SIDE_RIGHT;
//asmlabel_multipliedoutof32bits:
    tSigned64 lhs, rhs;
    lhs = testX;
    lhs *= baseY;
    rhs = testY;
    rhs *= baseX;
    if(lhs < rhs)
    {
        return SIDE_LEFT;
    }
    if(lhs == rhs)
    {
        return SIDE_CENTRE;
    }
    return SIDE_RIGHT;
}

}

#endif
