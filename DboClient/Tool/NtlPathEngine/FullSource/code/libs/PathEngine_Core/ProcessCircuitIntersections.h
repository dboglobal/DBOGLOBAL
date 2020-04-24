//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PROCESS_CIRCUIT_INTERSECTIONS_INCLUDED
#define PROCESS_CIRCUIT_INTERSECTIONS_INCLUDED

#include "libs/Mesh2D/interface/MeshCut_Header.h"
class cCutRegistry;

// returns true if there were intersections (or touch points) and the circuit elements got added to the registry (or deleted)
// otherwise, the registry and circuit are not affected and returns false
bool
ProcessCircuitIntersections(cCutRegistry& registry, tMeshCutSequence& toAdd);

#endif
