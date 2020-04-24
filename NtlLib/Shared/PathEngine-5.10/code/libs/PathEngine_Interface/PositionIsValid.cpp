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
#include "PositionIsValid.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "platform_common/CoordinateRanges.h"
#include "i_pathengine.h"


bool
PositionIsValid(tMesh& mesh, const cPosition& p)
{
    if(p.x < -WORLD_RANGE || p.x > WORLD_RANGE || p.y < -WORLD_RANGE || p.y > WORLD_RANGE)
    {
        return false;
    }
    return p.cell >= 0
        && p.cell < mesh.faces()
        && PointIsInFace<tMesh>(mesh.faceAt(p.cell), tPoint(p.x, p.y, WORLD_RANGE));
}
