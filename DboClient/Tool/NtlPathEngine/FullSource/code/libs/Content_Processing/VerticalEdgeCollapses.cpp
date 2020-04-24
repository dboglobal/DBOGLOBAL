//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "VerticalEdgeCollapses.h"
#include "common/interface/Assert.h"
#include "i_pathengine.h"

bool
cVerticalEdgeCollapses::collapseEdgeIfVertical(const iFaceVertexMesh& mesh, tSigned32 v0, tSigned32 v1)
{
    if(mesh.vertexX(v0) != mesh.vertexX(v1)
    || mesh.vertexY(v0) != mesh.vertexY(v1))
    {
        return false;
    }

    while(_vertexCollapseTable[v0] != v0)
    {
        v0 = _vertexCollapseTable[v0];
    }
    while(_vertexCollapseTable[v1] != v1)
    {
        v1 = _vertexCollapseTable[v1];
    }

    if(v0 > v1)
    {
        tSigned32 tmp = v0;
        v0 = v1;
        v1 = tmp;
    }

    _vertexCollapseTable[v1] = v0;
    return true;
}

cVerticalEdgeCollapses::cVerticalEdgeCollapses(const iFaceVertexMesh& mesh, cBitVector& faceSkipFlags) :
 _vertexCollapseTable(mesh.vertices()),
 _faceWasCollapsed(mesh.faces(), false),
 _zRanges(mesh.vertices() * 2)
{
    tSigned32 i;
    for(i = 0; i < mesh.vertices(); i++)
    {
        _vertexCollapseTable[i] = i;
    }

    for(i = 0; i < mesh.faces(); i++)
    {
        if(faceSkipFlags[i])
        {
            continue;
        }
        bool wasCollapsed = false;
        if(collapseEdgeIfVertical(mesh, mesh.vertexIndex(i, 0), mesh.vertexIndex(i, 1)))
        {
            wasCollapsed = true;
        }
        if(collapseEdgeIfVertical(mesh, mesh.vertexIndex(i, 1), mesh.vertexIndex(i, 2)))
        {
            wasCollapsed = true;
        }
        if(collapseEdgeIfVertical(mesh, mesh.vertexIndex(i, 2), mesh.vertexIndex(i, 0)))
        {
            wasCollapsed = true;
        }
        _faceWasCollapsed.setBit(i, wasCollapsed); 
    }

  // propagate collapses down
    for(i = 0; i < mesh.vertices(); i++)
    {
        tSigned32 propagatedCollapse = _vertexCollapseTable[i];
        while(_vertexCollapseTable[propagatedCollapse] != propagatedCollapse)
        {
            propagatedCollapse = _vertexCollapseTable[propagatedCollapse];
        }
        _vertexCollapseTable[i] = propagatedCollapse;
    }

  // fill in z ranges
    for(i = 0; i < mesh.vertices(); i++)
    {
        tSigned32 collapsed = _vertexCollapseTable[i];
        float z = mesh.vertexZ(i);
        if(collapsed == i)
        {
            _zRanges[collapsed * 2] = z;
            _zRanges[collapsed * 2 + 1] = z;
        }
    }
    for(i = 0; i < mesh.vertices(); i++)
    {
        tSigned32 collapsed = _vertexCollapseTable[i];
        float z = mesh.vertexZ(i);
        if(z < _zRanges[collapsed * 2])
        {
            _zRanges[collapsed * 2] = z;
        }
        else
        if(z > _zRanges[collapsed * 2 + 1])
        {
            _zRanges[collapsed * 2 + 1] = z;
        }
    }
}
