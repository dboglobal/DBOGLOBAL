//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONNECT_NEW_CUTS_INCLUDED
#define CONNECT_NEW_CUTS_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"

class cElementInFaceEntry;
class cCutRegistry;
class cLocalisedConvexShape;
class cFlagVector;

void SortOutSelfCrossings(tMeshCutSequence& cuts);

void ConnectExternalEdgeExpansion(cCutRegistry& registry, tMeshCutSequence& toAdd);
//void ConnectBaseCircuitExpansion(cCutRegistry& registry, tMeshCutSequence& toAdd, cFlagVector& faceFlagsBuffer);
void ConnectBaseObstacleExpansion(cCutRegistry& registry, tMeshCircuits& toAdd, const cLocalisedConvexShape& lcs, cFlagVector& faceFlagsBuffer);

// for debugging
void CheckNoSelfIntersections(cCutRegistry& registry);
void CheckNoSelfIntersections(tSigned32 facesInMesh, tSigned32 edgesInMesh, const tMeshCutSequence& cuts);


#endif
