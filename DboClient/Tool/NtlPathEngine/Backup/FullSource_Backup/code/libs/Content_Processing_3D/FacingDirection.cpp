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
#include "FacingDirection.h"
#include "tMesh_Simple3D.h"
#include "libs/Geometry/interface/tPoint3.h"

// note that faces may have more than 3 edges
// and some edges may be inline
// SIDE_RIGHT = facing up
// SIDE_LEFT = facing down
// SIDE_CENTRE = vertical
eSide 
FacingDirection(tFace_Simple3D f)
{
    eSide result = SIDE_CENTRE;
    tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
    tLine l = GetEdgeLine(e);
    bool not_completed;
    not_completed = e.toNextInFace();
    assertD(not_completed);
    not_completed = e.toNextInFace();
    assertD(not_completed);
    do
    {
        eSide side = l.side(GetVertexPoint(e));
        if(side != result)
        {
            assertD(result == SIDE_CENTRE);
            result = side;
        }
    }
    while(e.toNextInFace());
    return result;
}


