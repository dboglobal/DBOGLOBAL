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
#include "interface/LoadFaceVertexMeshSet.h"
#include "interface/LoadedFaceVertexMesh.h"
#include "libs/ParseXML/interface/XMLInputStreamToHandler.h"
#include "common/interface/iXMLInputStream.h"
#include "i_pathengine.h"
#include "common/CLib/string.h"

cFaceVertexMeshSetLoader::~cFaceVertexMeshSetLoader()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_result); ++i)
    {
        delete _result[i];
    }
}

iXMLHandler*
cFaceVertexMeshSetLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "mesh") == 0)
    {
        cLoadedFaceVertexMesh* m = new cLoadedFaceVertexMesh;
        _result.push_back(m);
        return m;
    }
    return 0;
}

static void
LoadFaceVertexMeshSet(iXMLInputStream& is, std::vector<iFaceVertexMesh*>& appendTo)
{
    const char* nextElement = is.peekNextElement();
    while(nextElement)
    {
        if(strcmp(nextElement, "mesh") == 0)
        {
            const char *const* attributes = is.openNextElement();
            cLoadedFaceVertexMesh* mesh = new cLoadedFaceVertexMesh();
            bool ok = XMLInputStreamToHandler(is, attributes, *mesh);
            assertD(ok);
            is.closeElement();
            appendTo.push_back(mesh);
        }
        else
        {
            is.skipNextElement();
        }
        nextElement = is.peekNextElement();
    }
}

void
LoadMeshesFromContentSnapshot(iXMLInputStream& is, std::vector<iFaceVertexMesh*>& appendTo)
{
    const char *const* attributes = is.openElement("content");
    LoadFaceVertexMeshSet(is, appendTo);
    is.closeElement();
}
