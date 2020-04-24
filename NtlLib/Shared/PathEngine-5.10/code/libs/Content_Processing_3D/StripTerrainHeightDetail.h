//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "tMesh_Dynamic_Header.h"

class cSourceDataLookup;
class cBSPVertices;

void
StripTerrainHeightDetail(tMesh_Dynamic& mesh, const cBSPVertices& vertices, tSigned32 numberOfTerrainLayers, const cSourceDataLookup& lookup);
