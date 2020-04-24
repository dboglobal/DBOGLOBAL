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
#include "interface/SaveMappingTo2D.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/ElementIndexing.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/Error.h"
#include "common/STL/sstream.h"

void
SaveMappingTo2D(tSigned32 verticesIn3D, tMesh& mesh, iXMLOutputStream& os)
{
    tSigned32 i;
    os.openElement("mappingTo2D");

    std::vector<tSigned32> targetIndex(mesh.vertices());
    {
        tSigned32 numberOfAdditionalVertices = 0;
        for(i = 0; i != mesh.vertices(); ++i)
        {
            tSigned32 elementIndex = mesh.vertAt(i)->getElementOn3DIndex();
            bMeshElement::eType type;
            tSigned32 subIndex;
            TypeAndSubIndexForElementIndex(elementIndex, type, subIndex);
            if(type == bMeshElement::VERTEX)
            {
                targetIndex[i] = subIndex;
            }
            else
            {
                if(numberOfAdditionalVertices == 0)
                {
                    os.openElement("verts");
                }
                os.openElement("vert");
                os.addAttribute("targetElement", elementIndex);
                os.addAttribute("x", mesh.vertAt(i)->getPoint().getX());
                os.addAttribute("y", mesh.vertAt(i)->getPoint().getY());
                os.closeElement("vert");
                targetIndex[i] = verticesIn3D + numberOfAdditionalVertices;
                numberOfAdditionalVertices++;
            }
        }
        if(numberOfAdditionalVertices)
        {
            os.closeElement("verts");
        }
    }

    i = 0;
    while(1)
    {
        tFace f = mesh.faceAt(i);
        if(f->isExternal())
        {
            break;
        }
        os.openElement("poly");
        tEdge e = tMesh::beginEdge(f);
        do
        {
            os.openElement("edge");
            std::ostringstream oss;
            os.addAttribute("startVert", targetIndex[e.vertex().index()]);
            tSigned32 connection = e.twin().face().index();
            if(connection < i)
            {
                os.addAttribute("connection", connection);
            }
            os.closeElement("edge");
        }
        while(e.toNextInFace());
        os.closeElement("poly");

        ++i;
    }
    os.closeElement("mappingTo2D");
}
