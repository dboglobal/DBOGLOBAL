//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SAVE_DYNAMIC_MESH_INCLUDED
#define SAVE_DYNAMIC_MESH_INCLUDED

#include "tMesh_Dynamic.h"
#include "common/STL/sstream.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/TemplateMesh/Helper.h"

inline void
SaveDynamicMesh(
        const cBSPVertices& vertices,
        tMesh_Dynamic& mesh, 
        iOutputStream& os
        )
{
    std::ostringstream oss;
    oss << mesh.faces() << '\n';
    tFace_Dynamic f = mesh.beginFace();
    while(f.valid())
    {
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(f);
        oss << CountEdgesAround(e) << '\n';
        do
        {
            const cDynamicPoint3& p = e.vertex()->dynamicPoint(vertices);
            oss << p.x().asFloat() << ' ' << p.y().asFloat() << ' ' << p.z().asFloat() << '\n';
        }
        while(e.toNextInFace());
        ++f;
    }
    os << oss.str();
}

#endif
