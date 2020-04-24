//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CORE_FIRST_COLLISION_TESTER_INCLUDED
#define CORE_FIRST_COLLISION_TESTER_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tLine_Header.h"

class cElementCollisionInfo;
class cQueryContext;
class iFirstCollisionTester
{
public:

    virtual ~iFirstCollisionTester()
    {
    }

    virtual void firstCollision(cQueryContext& qc, tFace startF, const tLine& line, cElementCollisionInfo& info) const = 0;
};

#endif
