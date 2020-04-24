//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GENERATE_MAPPING_INCLUDED
#define GENERATE_MAPPING_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class cParsedXMLElement;

tMesh*
GenerateIdentityMapping(tMesh_3D& mesh3D);

void
OptimiseMapping(tMesh& mapping);

#endif
