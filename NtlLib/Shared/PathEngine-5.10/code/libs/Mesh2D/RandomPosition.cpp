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
#include "interface/RandomPosition.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/FaceBounds.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/iRandomSequence.h"

cInternalPosition
RandomPosition(tMesh& mesh, tSigned32 internalFacesInMesh, iRandomSequence& randomSequence, tSigned32 tries)
{
    tSigned32 i;
    for(i = 0; i < tries; i++)
    {
        tSigned32 r = randomSequence.random31();
        tSigned32 rX = randomSequence.random31();
        tSigned32 rY = randomSequence.random31();

        tSigned32 faceIndex = r % internalFacesInMesh;

        tFace f = mesh.faceAt(faceIndex);

        tPoint bottomLeft, topRight;
        GetFaceBounds<tMesh>(f, bottomLeft, topRight);

        tPoint range = topRight - bottomLeft;

        tPoint p(bottomLeft.getX() + rX % range.getX(),
                 bottomLeft.getY() + rY % range.getY(),
                 WORLD_RANGE);

        if(PointIsInFace<tMesh>(f, p))
        {
            return cInternalPosition(f, p);
        }
    }
    cInternalPosition result;
    result.setInvalid();
    return result;
}

cInternalPosition
RandomPosition(tMesh& mesh, tSigned32 internalFacesInMesh, iRandomSequence& randomSequence)
{
    while(1)
    {
        tSigned32 r = randomSequence.random31();
        tSigned32 rX = randomSequence.random31();
        tSigned32 rY = randomSequence.random31();

        tSigned32 faceIndex = r % internalFacesInMesh;

        tFace f = mesh.faceAt(faceIndex);

        tPoint bottomLeft, topRight;
        GetFaceBounds<tMesh>(f, bottomLeft, topRight);

        tPoint range = topRight - bottomLeft;

        tPoint p(bottomLeft.getX() + rX % range.getX(),
                 bottomLeft.getY() + rY % range.getY(),
                 WORLD_RANGE);

        if(PointIsInFace<tMesh>(f, p))
        {
            return cInternalPosition(f, p);
        }
    }
}
