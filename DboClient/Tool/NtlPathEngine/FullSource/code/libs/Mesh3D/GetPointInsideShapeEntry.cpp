//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/GetPointInsideShapeEntry.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/ConvexPoly.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"

bool
GetPointInsideShapeEntry(tEdge_3D e3D, const tPoint& origin, const cConvexPoly& cs, tPoint& result)
{
    tLine edgeLine = GetEdgeLine(e3D);

  // does the shape enter through the edge?

    tSigned32 i;
    tLine shapeLine;
    for(i = 0; i != cs.sides(); ++i)
    {
        shapeLine = tLine(origin + cs.vertex(i), origin + cs.vertex(cs.nextVertex(i)));
        if(SideOfLine_SI(shapeLine, e3D.vertex()) == SIDE_RIGHT
         && SideOfLine_SI(shapeLine, e3D.next().vertex()) == SIDE_LEFT
         && SideOfEdge_SI(e3D, shapeLine.start()) == SIDE_LEFT
         && SideOfEdge_SI(e3D, shapeLine.end()) == SIDE_RIGHT
         )
        {
          // this line enters through the edge
            break;
        }
    }
    if(i == cs.sides())
    {
        return false; // doesn't enter through this edge
    }

    tIntersection intersection(shapeLine, edgeLine);
    tPoint centreP = intersection.approximatePoint();

    for(tSigned32 x = 0; x != 5; ++x)
    {
        for(tSigned32 y = 0; y != 5; ++y)
        {
            result = centreP + tPoint(-2 + x, -2 + y, 2);
            if(shapeLine.side(result) != SIDE_RIGHT) // early out
            {
                continue;
            }
            if(!PointIsInFace<tMesh_3D>(e3D.face(), result))
            {
                continue;
            }
            if(!cs.pointCollides2D(result - origin))
            {
                continue;
            }
            return true;
        }
    }
    return false;
}
