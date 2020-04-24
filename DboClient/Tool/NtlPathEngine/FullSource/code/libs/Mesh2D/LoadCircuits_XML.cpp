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
#include "interface/LoadCircuits_XML.h"
#include "CircuitLoaders.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/ParseXML/interface/XMLInputStreamToHandler.h"
#include "common/interface/iXMLInputStream.h"

void
LoadCutSequence_XML(tMesh& mesh, iXMLInputStream& is, tMeshCutSequence& sequence)
{
    is.openElement("circuit");
    const char* nextElement = is.peekNextElement();
    while(nextElement)
    {
        if(strcmp("cut", nextElement) == 0)
        {
            const char *const* attributes = is.openNextElement();

            cCutLoader loader(mesh);
            XMLInputStreamToHandler(is, attributes, loader);
            sequence.push_back(loader.getResult());

            is.closeElement();
        }
        else
        {
            is.skipNextElement();
        }
        nextElement = is.peekNextElement();
    }
    is.closeElement();
}

void
LoadCircuits_XML(tMesh& mesh, iXMLInputStream& is, tMeshCircuits& circuits)
{
    is.openElement("circuits");
    const char* nextElement = is.peekNextElement();
    while(nextElement)
    {
        if(strcmp("circuit", nextElement) == 0)
        {
            circuits.push_back(new tMeshCutSequence);
            LoadCutSequence_XML(mesh, is, *circuits.back());
        }
        else
        {
            is.skipNextElement();
        }
        nextElement = is.peekNextElement();
    }
    is.closeElement();
}

