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
#include "RemapEntry.h"
#include "libs/Mesh_Common/interface/iMeshTranslator.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/InfinitessimallyOffset_PointLike.h"
#include "libs/Geometry/interface/StandardIotas.h"
#include "platform_common/PointMultiplication.h"

void
RemapEntry(const iMeshTranslator<tMesh>& translator,
                tEdge entry,
                const tLine& entryLine,
                tEdge& remappedEntry)
{
    tLine edgeLine = GetEdgeLine(entry);
    tIntersection intersection(entryLine, edgeLine);
    tPoint offset;
    tPoint normal = edgeLine.axis().right90();
    if(SideOfAxis_SI(normal) == SIDE_LEFT)
    {
        offset = -edgeLine.axis();
    }
    else
    {
        offset = edgeLine.axis();
    }
    cInfinitessimallyOffset_PointLike<tIntersection> pl(intersection, offset);
    cMeshElement<tMesh> el = entry;
    cMeshElement<tMesh> remappedEl = translator.translate(el, pl);
    remappedEntry = remappedEl.asEdge();
}
