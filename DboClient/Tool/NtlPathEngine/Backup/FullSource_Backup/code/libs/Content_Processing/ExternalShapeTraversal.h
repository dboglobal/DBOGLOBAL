// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Mesh_Common/interface/GeometryOverlay.h"
#include "common/TemplateMesh/MeshElement.h"

class cExternalShapeTraversal
{
    const cGeometryOverlay<tMesh_3D_FaceTracked>& _overlay;
    cMeshElement<tMesh_3D_FaceTracked> _previousEl, _currentEl;
    tEdge_3D_FaceTracked _shapeEdge;
    bool _atVertex;

    void followExternalEdges();

public:

    cExternalShapeTraversal(
        const cGeometryOverlay<tMesh_3D_FaceTracked>& overlay,
        const cMeshElement<tMesh_3D_FaceTracked>& previousEl,
        const cMeshElement<tMesh_3D_FaceTracked>& currentEl,
        tEdge_3D_FaceTracked shapeEdge
        ) :
     _overlay(overlay),
     _previousEl(previousEl),
     _currentEl(currentEl),
     _shapeEdge(shapeEdge)
    {
        _atVertex = false;
    }

    void meshModified(
        const cMeshElement<tMesh_3D_FaceTracked>& newPreviousEl,
        const cMeshElement<tMesh_3D_FaceTracked>& newCurrentEl);

    cMeshElement<tMesh_3D_FaceTracked> advance();
    bool atShapeVertex(tPoint& vertex) const;
    tLine getCurrentLine() const;
};
