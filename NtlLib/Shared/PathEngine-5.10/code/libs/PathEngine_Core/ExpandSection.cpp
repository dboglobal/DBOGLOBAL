//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ExpandSection.h"

void ExpandSection(
                eSectionType sectionType, const tLine& sectionBoundary,
                eExpansionState endState,
                tEdge endEdgeOrFace,
                const tEdge* baseSequenceBegin,
                const tEdge* baseSequenceEnd,
                eExpansionState& currentState,
                tEdge& currentEdgeOrFace,
                std::vector<tEdge>& crossedOut,
                cCircuitsBuilder& result)
{
    while(1)
    {
        if(currentState == ES_FOLLOWING_EXTERNAL)
        {
        }
        else
        {
            assertD(currentState == ES_BUILDING_CIRCUIT);
        }
    }
}
