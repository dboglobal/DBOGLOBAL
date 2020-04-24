//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DRAW3D_INCLUDED
#define DRAW3D_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/Vector3F.h"

class iGraphicsInterface;
class cConvexPoly;

void Draw3DLine(iGraphicsInterface* gip, const cVector3F& start, const cVector3F& end);
void Draw3DCross(iGraphicsInterface* gip, const cVector3F& p, float size);

class cPosition;
class iShape;

void
DrawAgentWithPrecision(
        iGraphicsInterface* gip,
        const cConvexPoly& placedPoly,
        tSigned32 zAsLong,
        tSigned32 height,
        float precisionX, float precisionY
        );

void DrawAgentArrowWithPrecision(iGraphicsInterface* gip, const cPosition& p, float direction, tSigned32 z, float size, tSigned32 height, float precisionX, float precisionY);

#endif
