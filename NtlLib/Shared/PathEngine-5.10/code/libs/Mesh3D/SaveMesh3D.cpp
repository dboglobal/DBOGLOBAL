//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/SaveMesh3D.h"
#include "interface/tMesh_3D.h"
#include "platform_common/FloatPersistence.h"
#include "common/interface/iXMLOutputStream.h"

void
SaveMesh3D(tMesh_3D& mesh, iXMLOutputStream& os)
{
    assertD(!mesh.hasDeadEntries());
    tSigned32 i;
    os.openElement("verts");
    for(i = 0; i != mesh.vertices(); ++i)
    {
        tVertex_3D v = mesh.vertAt(i);
        os.openElement("vert");
        os.addAttribute("x", v->getX());
        os.addAttribute("y", v->getY());
        tEdge_3D e = tMesh_3D::firstEdge(v);
        if(e->getZ() != 0.f)
        {
            std::string persistent = FloatToPersistent(e->getZ());
            os.addAttribute("z", persistent.c_str());
        }
        os.closeElement("vert");
    }
    os.closeElement("verts");
    os.openElement("tris");
    for(i = 0; i != mesh.faces(); ++i)
    {
        tFace_3D f = mesh.faceAt(i);
        if(f->isExternal())
        {
            break;
        }
        os.openElement("tri");
        tEdge_3D e = tMesh_3D::beginEdge(f);

        tEdge_3D vertexFirstEdge;

        os.addAttribute("edge0StartVert", e.vertex().index());
        vertexFirstEdge = tMesh_3D::firstEdge(e.vertex());
        if(e->getZ() != vertexFirstEdge->getZ())
        {
            std::string persistent = FloatToPersistent(e->getZ());
            os.addAttribute("edge0StartZ", persistent.c_str());
        }
        if(!e.twin().face()->isExternal() && e.twin().face().index() < i)
        {
            os.addAttribute("edge0Connection", e.twin().face().index());
        }

        ++e;

        os.addAttribute("edge1StartVert", e.vertex().index());
        vertexFirstEdge = tMesh_3D::firstEdge(e.vertex());
        if(e->getZ() != vertexFirstEdge->getZ())
        {
            std::string persistent = FloatToPersistent(e->getZ());
            os.addAttribute("edge1StartZ", persistent.c_str());
        }
        if(!e.twin().face()->isExternal() && e.twin().face().index() < i)
        {
            os.addAttribute("edge1Connection", e.twin().face().index());
        }

        ++e;

        os.addAttribute("edge2StartVert", e.vertex().index());
        vertexFirstEdge = tMesh_3D::firstEdge(e.vertex());
        if(e->getZ() != vertexFirstEdge->getZ())
        {
            std::string persistent = FloatToPersistent(e->getZ());
            os.addAttribute("edge2StartZ", persistent.c_str());
        }
        if(!e.twin().face()->isExternal() && e.twin().face().index() < i)
        {
            os.addAttribute("edge2Connection", e.twin().face().index());
        }

        os.closeElement("tri");
    }
    os.closeElement("tris");
}
