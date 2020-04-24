//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/SquareRoot.h"
#include "platform_common/PointMultiplication.h"

tSigned32 Length(const tPoint3& vector)
{
    return static_cast<tSigned32>(SquareRoot(tPoint3::tMultiplier::asFloat(vector.lengthSquared())));
}
