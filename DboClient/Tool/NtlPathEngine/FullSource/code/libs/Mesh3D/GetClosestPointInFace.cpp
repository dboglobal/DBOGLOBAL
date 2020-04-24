//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/GetClosestPointInFace.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

static bool
GetClosestPointInFace_Rooted(tFace_3D f, const tPoint& root, tPoint& result)
{
    for(tSigned32 x = 0; x != 5; ++x)
    {
        for(tSigned32 y = 0; y != 5; ++y)
        {
            result = root + tPoint(-2 + x, -2 + y, 2);
            if(!PointIsInFace<tMesh_3D>(f, result))
            {
                continue;
            }
            return true;
        }
    }
    return false;
}

bool
GetClosestPointInFace(tFace_3D f3D, const tPoint& p, tPoint& closestP)
{
    tEdge_3D e = tMesh_3D::beginEdge(f3D);
    do
    {
        tLine edgeLine = GetEdgeLine(e);
        tLine startLine = edgeLine.left90();
        tLine prevEndLine = GetEdgeLine(e.prev()).reversed().right90();

        if(prevEndLine.side(p) != SIDE_LEFT
        && startLine.side(p) != SIDE_RIGHT)
        {
            return GetClosestPointInFace_Rooted(f3D, GetVertexPoint(e), closestP);
        }

        tLine endLine = edgeLine.reversed().right90();

        if(edgeLine.side(p) != SIDE_RIGHT
        && startLine.side(p)  ==  SIDE_RIGHT
        && endLine.side(p)  ==  SIDE_LEFT)
        {
            tLine l1 = GetEdgeLine(e);
            tLine l2(p, p + l1.axis().right90());
            tIntersection intersection(l1, l2);
            return GetClosestPointInFace_Rooted(f3D, intersection.approximatePoint(), closestP);
        }
    }
    while(e.toNextInFace());
    invalid();
    return false;
}

