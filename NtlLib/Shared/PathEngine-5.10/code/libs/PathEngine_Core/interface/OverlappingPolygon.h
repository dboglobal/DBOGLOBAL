//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

class cQueryContext;
class cConvexPoly;
class cInternalPosition;
class cFlagVector;

void
CircuitsForOverlappingPolygon(
        tFace centreF,
        const cConvexPoly& polygon,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        );

void
FacesOverlappedByOverlappingPolygon_SI(
        tFace centreF,
        const cConvexPoly& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );

void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF,
        const cConvexPoly& polygon,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );
void
FacesOverlappedByOverlappingPolygon_OF(
        tFace centreF, const tIntersection& centreP,
        const cConvexPoly& offsetPoly,
        tFace* openQueueBuffer,
        cFlagVector& resultBuffer,
        bool& exitsMesh
        );

bool
ShapeCollidesWithMeshEdge(
        cQueryContext& qc,
        tFace centreF,
        const cConvexPoly& polygon
        );

void
CircuitsForInfiniteCylinder(
        tMesh& mesh,
        const cConvexPoly& polygon,
        std::list<tFace>& containedExternalFaces,
        tMeshCircuits& appendTo
        );

//void
//GetSquareBounds(
//        const cInternalPosition& centre,
//        tSigned32 radius,
//        std::list<tFace>& containedExternalFaces,
//        tMeshCircuits& appendTo
//        );
