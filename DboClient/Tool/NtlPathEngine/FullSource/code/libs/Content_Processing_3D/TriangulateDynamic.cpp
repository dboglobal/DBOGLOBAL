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
#include "TriangulateDynamic.h"
#include "Plane.h"
#include "SideOfProjectedTo2D.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "i_pathengine.h"

class cMeshDynamicSideChecker
{
    const cBSPVertices& _vertices;
    int _projectionType;

public:

    cMeshDynamicSideChecker(const cBSPVertices& vertices, int projectionType) :
      _vertices(vertices),
      _projectionType(projectionType)
    {
    }

    bool
    quadSplitIsValid(tEdge_Dynamic eSplitStart, tEdge_Dynamic eSplitEnd, tEdge_Dynamic mustBeOnLeft, tEdge_Dynamic mustBeOnRight)
    {
        const cDynamicPoint3& startP = eSplitStart.vertex()->dynamicPoint(_vertices);
        const cDynamicPoint3& endP = eSplitEnd.vertex()->dynamicPoint(_vertices);
        eSide side;
        side = SideOfProjectedTo2D(_projectionType, startP, endP, mustBeOnLeft.vertex()->dynamicPoint(_vertices));
        if(side != SIDE_LEFT)
        {
            return false;
        }
        side = SideOfProjectedTo2D(_projectionType, startP, endP, mustBeOnRight.vertex()->dynamicPoint(_vertices));
        if(side != SIDE_RIGHT)
        {
            return false;
        }
        return true;
    }

    eSide
    sideOfLine(tEdge_Dynamic e1, tEdge_Dynamic e2, tEdge_Dynamic eTest)
    {
        const cDynamicPoint3& p1 = e1.vertex()->dynamicPoint(_vertices);
        const cDynamicPoint3& p2 = e2.vertex()->dynamicPoint(_vertices);
        const cDynamicPoint3& pTest = eTest.vertex()->dynamicPoint(_vertices);
        return SideOfProjectedTo2D(_projectionType, p1, p2, pTest);
    }
};


void
RemoveInternalVertex_Dynamic(tMesh_Dynamic& mesh, tVertex_Dynamic v, const cBSPVertices& vertices, int projectionType)
{
    cMeshDynamicSideChecker sideChecker(vertices, projectionType);
    RemoveInternalVertex(mesh, v, sideChecker);
}

void
RemoveInlineEdgeVertex_NoExternalFaces_Dynamic(tMesh_Dynamic& mesh, tVertex_Dynamic v, const cBSPVertices& vertices, int projectionType)
{
    cMeshDynamicSideChecker sideChecker(vertices, projectionType);
    RemoveInlineEdgeVertex_NoExternalFaces(mesh, v, sideChecker);
}

void
RemoveInlineBoundaryVertex_Dynamic(tMesh_Dynamic& mesh, tEdge_Dynamic eBoundary1, tEdge_Dynamic eBoundary2, const cBSPVertices& vertices, int projectionType, int projectionType2)
{
    cMeshDynamicSideChecker sideChecker(vertices, projectionType);
    cMeshDynamicSideChecker sideChecker2(vertices, projectionType2);
    RemoveInlineBoundaryVertex(mesh, eBoundary1, eBoundary2, sideChecker, sideChecker2);
}

void
TriangulateFace_ConvexWithInlineEdges_Dynamic(
        tMesh_Dynamic& mesh,
        tFace_Dynamic& f,
        const cBSPVertices& vertices, int projectionType
        )
{
    cMeshDynamicSideChecker sideChecker(vertices, projectionType);
    TriangulateFace_ConvexWithInlineEdges(mesh, f, sideChecker);
}

void
TriangulateMesh_Dynamic(tMesh_Dynamic& mesh, const cBSPVertices& vertices, const std::vector<int>& projectionTypes, iProgressCallBack* progressCallBack)
{
    // we loop down through face indices, to ensure that faces added during triangulation are not reprocessed
    mesh.consolidate();
    tSigned32 i = mesh.faces();
    tSigned32 facesToProcess = i;
    while(i)
    {
        --i;
        tFace_Dynamic f = mesh.faceAt(i);
        if(progressCallBack && (i & 7) == 0)
        {
            progressCallBack->updateProgress("Triangulating faces", static_cast<float>(facesToProcess - i) / static_cast<float>(facesToProcess));
        }
        if(f->wasDiscarded())
        {
            continue;
        }
        TriangulateFace_ConvexWithInlineEdges_Dynamic(mesh, f, vertices, projectionTypes[f->_splitterIndex]);
    }
}
