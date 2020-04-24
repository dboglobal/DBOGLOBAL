//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "LoadBaseMesh.h"
#include "libs/Content_Processing/interface/LoadedFaceVertexMesh.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "platform_common/LoadSaveBinary.h"

iFaceVertexMesh*
LoadBaseMesh(const char* type)
{
    std::string fileName = "..\\resource\\content3D\\";
    fileName.append(type);
    fileName.append(".tok");
    std::vector<char> buffer;
    LoadBinary(fileName.c_str(), buffer);
    if(buffer.empty())
    {
        Error("Fatal", "Failed to load buffer for a mesh object.");
        return 0;
    }
    cLoadedFaceVertexMesh* loadedMesh = new cLoadedFaceVertexMesh;
    cNestedHandler meshHandler(*loadedMesh, "mesh");
    cDocumentHandler documentHandler(meshHandler, "content");
    bool parsedOK = ParseTokenisedXML(&buffer[0], static_cast<tUnsigned32>(buffer.size()), documentHandler);
    if(!parsedOK)
    {
        Error("Fatal", "Failed to parse buffer for a mesh object.");
    }
    if(loadedMesh->faces() == 0)
    {
        Error("NonFatal", "Loaded mesh object has no faces.");
    }
    return loadedMesh;
}
