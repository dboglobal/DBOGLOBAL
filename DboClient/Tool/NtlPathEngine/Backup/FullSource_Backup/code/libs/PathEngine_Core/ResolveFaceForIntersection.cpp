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
#include "ResolveFaceForIntersection.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tLine.h"

tFace
ResolveFaceForIntersection(
                tFace traverseFrom,
                const tLine& traverseLine,
                const tLine& intersectingLine)
{
    assertD(traverseLine.side(intersectingLine) == SIDE_LEFT);
    tEdge edgeCrossed;

    //.... TODO optimise - lose the point like for these performance sensitive collision functions
    cPoint_PointLike p(traverseLine.end());

    tFace intersectionF = traverseFrom;
    while(TraverseTowardsTarget_SI<tMesh>(traverseFrom, traverseLine, p, edgeCrossed) || traverseFrom->isExternal())
    {
      // check if the intersection occurs before edge crossing or exit
        eSide side = traverseLine.intersectionOrder_Anticlockwise(intersectingLine, GetEdgeLine(edgeCrossed.twin()));
        if(EdgeHasPriority_SI(edgeCrossed))
        {
            if(side != SIDE_RIGHT)
            {
                break;
            }
        }
        else
        {
            if(side == SIDE_LEFT)
            {
                break;
            }
        }
        if(traverseFrom->isExternal())
        {
            return tMesh::endFace(); // intersection occurs after exit
        }
        intersectionF = traverseFrom;
    }
    return intersectionF;
}
