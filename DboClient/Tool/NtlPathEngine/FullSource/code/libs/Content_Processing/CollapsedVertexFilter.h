//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "VerticalEdgeCollapses.h"
#include "i_pathengine.h"

class cCollapsedVertexFilter : public iFaceVertexMesh
{
    const iFaceVertexMesh& _base;
    const cVerticalEdgeCollapses& _collapses;

public:

    cCollapsedVertexFilter(const iFaceVertexMesh& base, const cVerticalEdgeCollapses& collapses) :
     _base(base),
     _collapses(collapses)
    {
    }

    tSigned32 getCollapsedIndex(tSigned32 face, tSigned32 vertexInFace) const
    {
        return _collapses.getCollapsedIndex(_base.vertexIndex(face, vertexInFace));
    }
    bool faceWasCollapsed(tSigned32 face) const
    {
        return _collapses.faceWasCollapsed(face);
    }
    void vertexZRange(tSigned32 collapsedVertex, float& minZ, float& maxZ) const
    {
        _collapses.vertexZRange(collapsedVertex, minZ, maxZ);
    }

// iFaceVertexMesh

    tSigned32 faces() const
    {
        return _base.faces();
    }
    tSigned32 vertices() const
    {
        return _base.vertices();
    }
    tSigned32 vertexIndex(tSigned32 face, tSigned32 vertexInFace) const
    {
        assertD(!_collapses.faceWasCollapsed(face));
        return _collapses.getCollapsedIndex(_base.vertexIndex(face, vertexInFace));
    }
    tSigned32 vertexX(tSigned32 vertex) const
    {
        return _base.vertexX(vertex);
    }
    tSigned32 vertexY(tSigned32 vertex) const
    {
        return _base.vertexY(vertex);
    }
    float vertexZ(tSigned32 vertex) const
    {
        return _base.vertexZ(vertex);
    }
    tSigned32 faceAttribute(tSigned32 face, tSigned32 attributeIndex) const
    {
        return _base.faceAttribute(face, attributeIndex);
    }
};
