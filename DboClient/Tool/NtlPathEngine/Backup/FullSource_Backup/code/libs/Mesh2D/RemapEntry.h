//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"

template <class T> class iMeshTranslator;

void
RemapEntry(const iMeshTranslator<tMesh>& translator,
                tEdge entry,
                const tLine& entryLine,
                tEdge& remappedEntry);
