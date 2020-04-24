//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPAND_SECTION_INCLUDED
#define EXPAND_SECTION_INCLUDED

#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "common/STL/vector.h"

class cCircuitsBuilder;

enum eSectionType
{
    ST_PARALLEL,
    ST_RADIAL,
};

enum eExpansionState
{
    ES_UNKNOWN,
    ES_FOLLOWING_EXTERNAL,
    ES_BUILDING_CIRCUIT,
};

void ExpandSection(
                eSectionType sectionType, const tLine& sectionBoundary,
                eExpansionState endState,
                tEdge endEdgeOrFace,
                const tEdge* baseSequenceBegin,
                const tEdge* baseSequenceEnd,
                eExpansionState& currentState,
                tEdge& currentEdgeOrFace,
                std::vector<tEdge>& crossedOut,
                cCircuitsBuilder& result);

#endif
