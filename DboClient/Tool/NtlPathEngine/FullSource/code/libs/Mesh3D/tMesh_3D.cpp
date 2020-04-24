//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "common/CLib/stdlib.h"
#include "platform_common/SquareRoot.h"

bool
cE3D::operator==(const cE3D& rhs) const
{
    return _z == rhs._z;
}

bool
cV3D::operator==(const cV3D& rhs) const
{
    return _x == rhs._x
        && _y == rhs._y
        ;
}

bool
cF3D::operator==(const cF3D& rhs) const
{
    return _external == rhs._external
        && _heightAtOrigin == rhs._heightAtOrigin
        && _heightForX == rhs._heightForX
        && _heightForY == rhs._heightForY
        ;
}

void
cF3D::getNormal(float& x, float& y, float& z) const
{
    x = -_heightForX;
    y = -_heightForY;
    float length = SquareRoot(x * x + y * y + 1.f);
    x /= length;
    y /= length;
    z = 1.f / length;
}

