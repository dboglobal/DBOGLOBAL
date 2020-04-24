//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/InternalPosition.h"

class iRandomSequence;

cInternalPosition
RandomPosition(tMesh& mesh, tSigned32 internalFacesInMesh, iRandomSequence& randomSequence, tSigned32 tries);

cInternalPosition
RandomPosition(tMesh& mesh, tSigned32 internalFacesInMesh, iRandomSequence& randomSequence);
