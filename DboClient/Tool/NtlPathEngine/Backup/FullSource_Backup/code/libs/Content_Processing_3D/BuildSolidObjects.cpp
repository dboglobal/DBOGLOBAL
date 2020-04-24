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
#include "BuildSolidObjects.h"
#include "ConvexHull3D.h"
#include "tMesh_Simple3D.h"
#include "ValidateVolume.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "common/interface/Error.h"
#include "common/interface/Assert.h"
#include "platform_common/CoordinateRanges.h"
#include "i_pathengine.h"

void
BuildSolidObjects(
        const iSolidObjects& solidObjects,
        std::vector<tMesh_Simple3D*>& operands
        )
{
    bool emittedOutOfRangeError = false;
    std::vector<tPoint3> points;
    tSigned32 i;
    for(i = 0; i != solidObjects.numberOfConvexSolids(); ++i)
    {
        points.resize(solidObjects.convexSolid_Points(i));
        tSigned32 j;
        for(j = 0; j != solidObjects.convexSolid_Points(i); ++j)
        {
            tSigned32 x, y, z;
            solidObjects.convexSolid_Point(i, j, x, y, z);
            if(x < -WORLD_RANGE || x > WORLD_RANGE
             || y < -WORLD_RANGE || y > WORLD_RANGE
             || z < -WORLD_RANGE || z > WORLD_RANGE
                )
            {
                if(!emittedOutOfRangeError)
                {
                    Error("NonFatal", "BuildMeshFrom3DContent() - Out of range coordinates encountered in solid object data. Objects with out of range coordinates will be skipped.");
                    emittedOutOfRangeError = true;
                }
                break;
            }
            points[j] = tPoint3(x, y, z, WORLD_RANGE);
        }
        if(j != solidObjects.convexSolid_Points(i))
        {
            continue;
        }
        operands.push_back(new tMesh_Simple3D);
        ConvexHull3D(SizeL(points), &points.front(), *operands.back());
        operands.back()->consolidate();
		assertD(ValidateVolume(*operands.back()));
    }
}
