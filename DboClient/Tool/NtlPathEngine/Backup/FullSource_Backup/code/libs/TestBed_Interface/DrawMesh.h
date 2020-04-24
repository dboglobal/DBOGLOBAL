//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DRAWMESH_INCLUDED
#define DRAWMESH_INCLUDED

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh3D/interface/tMesh_3D_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"

class iGraphicsInterface;
class cMeshTraversal;
class iRenderGeometry;
class cTranslationToOverlapped;
class cCircuitElement;

void
DrawLineOnGround(iGraphicsInterface* gip, tFace_3D f, const tLine& line, eSide spinThroughSide);

void DrawInternalEdgesOnOverlapped(
            iGraphicsInterface* gip,
            tMesh &mesh);

void DrawMeshCut(const cMeshCut& cut, iGraphicsInterface* gip,
                 const cTranslationToOverlapped& translation
                 );
void DrawMeshCutSequence(const tMeshCutSequence& cut_sequence, iGraphicsInterface* gip,
                 const cTranslationToOverlapped& translation
                         );
void DrawMeshCircuits(const tMeshCircuits& circuits, iGraphicsInterface* gip,
                 const cTranslationToOverlapped& translation
                      );

#endif
