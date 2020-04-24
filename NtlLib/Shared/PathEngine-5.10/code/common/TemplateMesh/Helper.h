//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TEMPLATE_MESH_HELPER_INCLUDED
#define TEMPLATE_MESH_HELPER_INCLUDED

#include "common/interface/Assert.h"

template <class tEdge> tSigned32
CountEdgesAround(tEdge startEdge)
{
    tSigned32 result = 1;
    tEdge e = startEdge.next();
    while(e != startEdge)
    {
        result++;
        ++e;
    }
    return result;
}

template <class tMesh, class tFace> tSigned32
CountEdgesInFace(tMesh& mesh, tFace f)
{
    return CountEdgesAround(mesh.beginEdge(f));
}

template <class tEdge1, class tEdge2> tEdge2
GetEdgeAtSameIndexAround(tEdge1 firstEdge_Source, tEdge1 e_Source, tEdge2 firstEdge)
{
    assertD(firstEdge_Source.face() == e_Source.face());
    while(firstEdge_Source != e_Source)
    {
        ++firstEdge_Source;
        ++firstEdge;
    }
    return firstEdge;
}

#endif
