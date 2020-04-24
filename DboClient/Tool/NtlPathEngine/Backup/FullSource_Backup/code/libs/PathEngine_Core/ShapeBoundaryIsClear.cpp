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
#include "ShapeBoundaryIsClear.h"
#include "interface/iCollisionTester.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/tLine.h"

bool
ShapeBoundaryIsClear(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32 indexForShapeToTest,
        const iPointCollisionTester& basePointCollision,
        const iLineCollisionTester& baseLineCollision
        )
{
    const cLocalisedConvexShape& shape = shapeSet[indexForShapeToTest];
    assertD(!shape.exitsMesh());
    tFace f = shape.centre().f;
    const cConvexPoly& poly = shape.poly();
    tLine line(shape.centre().p, poly.vertex(0));
    f = FaceAtEndOfLine<tMesh>(f, line);
    assertD(f.valid());

    if(basePointCollision.testPointCollision(qc, f, poly.vertex(0)))
    {
        return false;
    }
    if(PointCollision_AgainstShapeSet_IgnoringOneShape(shapeSet, cInternalPosition(f, poly.vertex(0)), indexForShapeToTest))
    {
        return false;
    }

    tSigned32 i;
    for(i = 0; i != poly.sides(); ++i)
    {
        tLine line = poly.getBoundaryLine(i);
        tFace endF;
        if(baseLineCollision.testCollisionToXY(qc, f, line, endF))
        {
            return false;
        }
        if(shapeSet.testCollisionToXY(qc, f, line))
        {
            return false;
        }
        f = endF;
    }
    return true;
}
