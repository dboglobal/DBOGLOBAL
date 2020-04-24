//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/SideEnum.h"

eSide
Side_Directions(eDirection base, eDirection test);
eSide
Side_Directions(const tPoint& base, const tPoint& lhs, const tPoint& rhs);

eSide
Side_Directions_BaseAtEnd(eDirection base, eDirection test);
eSide
Side_Directions_BaseAtEnd(const tPoint& base, const tPoint& lhs, const tPoint& rhs);
