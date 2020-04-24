//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLUECKER_LINE_INCLUDED
#define PLUECKER_LINE_INCLUDED

#include "libs/Geometry/interface/Vector3F.h"

class cPlueckerLine
{
public:

    cPlueckerLine()
    {
    }
    cPlueckerLine(const cVector3F& start, const cVector3F& end);

    cVector3F axis;
    cVector3F product;
};

#endif
