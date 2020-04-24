//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//**** This header is for use by the 3d content processing demo, for visualisation purposes
//**** but is not part of the supported API

#ifndef CONTENT_PROCESSING_3D__GENERATE_HULLS_FOR_VISUALISATION_INCLUDED
#define CONTENT_PROCESSING_3D__GENERATE_HULLS_FOR_VISUALISATION_INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

class iSolidObjects;

void
GenerateHullsForVisualisation(
        const iSolidObjects& solidObjects,
        tMesh_3D& result
        );

#endif
