//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef EXPAND_SWEEP_INCLUDED
#define EXPAND_SWEEP_INCLUDED

#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "common/STL/list.h"
#include "common/STL/vector.h"

void
DoTraverseOutJustToTheRight(
        tFace f,
        const tLine& traverseLine, const tLine& boundary,
        std::list<tEdge>& appendTo);
void
DoTraverseOutJustToTheLeft(
        tFace f,
        const tLine& traverseLine, const tLine& boundary,
        std::list<tEdge>& appendTo);

class cExpansionFrame_SI;

class cExpandSweep
{
public:
    enum eEvent
    {
        UNSET,

      // edge crossing events
        INTERNAL_EDGE_CROSSED,
        MESH_ENTRY,
        MESH_EXIT,
      // vertex swept events
        INTERNAL_VERTEX,
        EXTERNAL_VERTEX,

      // section end
        EXPAND_SECTION_ENDS,
    };
private:

    const cExpansionFrame_SI& _frame;
    tSigned32 _section;

  // state within current expansion section
    tSigned32 _baseSequencePosition;
    std::vector<tEdge> _addedToBaseSequence;
    tFace _startFace;
    std::list<tEdge> _crossedOut;

  // next event
    eEvent _nextEvent;
    tEdge _nextEventEdge;

    tEdge getNextInTraversal();
    tEdge nextInBaseSequence();
    void advancePastNextInBaseSequence();
    void advanceBaseSequencePosition();
    tEdge getFirstVertex() const;
    void setNextEvent();

public:

    // crossedOut is swapped in
    cExpandSweep(const cExpansionFrame_SI& frame, tFace startFace, std::list<tEdge>& crossedOut);

    tSigned32 expandSection() const {return _section;}
    bool exitsMesh() const;
    tEdge exitEdge() const;
    tFace startFace() const;
    tFace firstFaceTouchingVertex(tVertex v) const;
    tFace faceAtBoundary() const;
    bool crossedOutEquals(const std::list<tEdge>& rhs) const
    {
        return _crossedOut == rhs;
    }

    eEvent nextEvent() const
    {
        return _nextEvent;
    }
    tEdge nextEventEdge()
    {
        return _nextEventEdge;
    }

    void advancePastEvent();
};

#endif
