//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SAVE_SIMPLE_3D_MESH_INCLUDED
#define SAVE_SIMPLE_3D_MESH_INCLUDED

#include "tMesh_Simple3D.h"
#include "common/STL/sstream.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/TemplateMesh/Helper.h"

inline void
SaveSimple3DMesh(
        tMesh_Simple3D& mesh, 
        iOutputStream& os
        )
{
    std::ostringstream oss;
    oss << mesh.faces() << '\n';
    tFace_Simple3D f = mesh.beginFace();
    while(f.valid())
    {
        tEdge_Simple3D e = tMesh_Simple3D::beginEdge(f);
        oss << CountEdgesAround(e) << '\n';
        do
        {
            const tPoint3 p = e.vertex()->getPoint();
            oss << p.getX() << ' ' << p.getY() << ' ' << p.getZ() << '\n';
        }
        while(e.toNextInFace());
        ++f;
    }
    os << oss.str();
}

#endif
