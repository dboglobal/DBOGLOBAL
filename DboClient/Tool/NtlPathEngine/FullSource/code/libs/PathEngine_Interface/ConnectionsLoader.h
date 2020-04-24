//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONNECTIONS_LOADER_INCLUDED
#define CONNECTIONS_LOADER_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/ParseXML/interface/iXMLHandler.h"

class cOffMeshConnections;

class cConnectionsLoader : public iXMLHandler
{
    tMesh& _mesh;
    cOffMeshConnections& _loadInto;
    bool _loadingEndPoints;

public:

    cConnectionsLoader(tMesh& mesh, cOffMeshConnections& loadInto) :
      _mesh(mesh),
      _loadInto(loadInto)
    {
    }

// interface for iXMLHandler

    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
