//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/MeshCut_Header.h"
#include "common/STL/vector.h"

class cConvexPoly;
class cCircuitElement;

void
ExpandExternalFace_SI(
        tFace convexExternalFace,
        const cConvexPoly& expandBy,
        std::vector<cCircuitElement*>& appendTo
        );
        
void
ExpandExternalEdges_SI(
        tEdge first, tEdge last,
        const cConvexPoly& expandBy,
        std::vector<cCircuitElement*>& appendTo
        );

//void
//ExpandCut_SI(
//        const cCircuitElement* cutStartElement,
//        const cConvexPoly& expandBy,
//        std::vector<cCircuitElement*>& cuts
//        );
