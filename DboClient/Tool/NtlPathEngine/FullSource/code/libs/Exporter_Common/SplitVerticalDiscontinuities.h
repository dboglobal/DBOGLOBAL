//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"

// the resulting mesh has disconnected edges along the vertical strips
// but this doesnt matter if it is just going to be used in a cMesh3DWrapper, for example
void
SplitVerticalDiscontinuities(tMesh_3D_FaceTracked& mesh);
