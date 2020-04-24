//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH2D_LOAD_CIRCUITS_XML_INCLUDED
#define MESH2D_LOAD_CIRCUITS_XML_INCLUDED

#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"

class iXMLInputStream;

void
LoadCutSequence_XML(tMesh& mesh, iXMLInputStream& is, tMeshCutSequence& sequence);

void
LoadCircuits_XML(tMesh& mesh, iXMLInputStream& is, tMeshCircuits& circuits);

#endif
