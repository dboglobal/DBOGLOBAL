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
#include "interface/MeshCheckSum.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/interface/iOutputStream_Operators.h"

void
GenerateMeshCheckSum(const tMesh& mesh_const, iOutputStream& os)
{
    tMesh& mesh = const_cast<tMesh&>(mesh_const); //... support const meshes
    tEdge e = mesh.beginEdge();
    os << mesh.faces();
    os << mesh.edges();
    os << mesh.vertices();
    do
    {
        os << e.index();
        if(e.twin().valid())
        {
            os << e.twin().index();
        }
        os << GetVertexPoint(e).getX();
        os << GetVertexPoint(e).getY();
    }
    while(e.toNextInMesh());
}
