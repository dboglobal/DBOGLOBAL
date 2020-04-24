//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/SideEnum.h"
#include "common/Containers/BitVector.h"

//inline void
//StoreSideOfPlane(cBitVector& bitVector, tSigned32 startPosition, tSigned32 index, eSideOfPlane side)
//{
//    bitVector.setBit(startPosition + index * 2, side == IN_FRONT);
//    bitVector.setBit(startPosition + index * 2 + 1, side == BEHIND);
//}
inline void
StoreSideOfPlane(cBitVector& bitVector, eSideOfPlane side)
{
    bitVector.push_back(side == IN_FRONT);
    bitVector.push_back(side == BEHIND);
}
inline eSideOfPlane
RetrieveSideOfPlane(const cBitVector& bitVector, tSigned32 startPosition, tSigned32 index)
{
    if(bitVector[startPosition + index * 2])
    {
        return IN_FRONT;
    }
    if(bitVector[startPosition + index * 2 + 1])
    {
        return BEHIND;
    }
    return ON_PLANE;
}
