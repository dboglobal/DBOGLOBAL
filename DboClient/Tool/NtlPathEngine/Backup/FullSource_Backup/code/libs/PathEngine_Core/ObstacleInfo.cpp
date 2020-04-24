//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ObstacleInfo.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "common/interface/iOutputStream_Operators.h"
#include "platform_common/FloatPersistence.h"

void
cObstacleInfo::checksum(iOutputStream& os, const cConvexPoly& shape) const
{
    os << _position.f.index();
    os << _position.p.getX();
    os << _position.p.getY();

    os << shape.sides();
    tSigned32 i;
    for(i = 0; i < shape.sides(); ++i)
    {
        os << shape.vertex(i).getX();
        os << shape.vertex(i).getY();
    }

    std::string persistent = FloatToPersistent(_costToTraverse);
    os << persistent;
    os << _directionForCostToTraverse.getX();
    os << _directionForCostToTraverse.getY();
}

