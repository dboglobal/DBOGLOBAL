//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "ExpandSweep.h"
#include "ExpansionFrame_SI.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"

void
DoTraverseOutJustToTheRight(
        tFace f,
        const tLine& traverseLine, const tLine& boundary,
        std::list<tEdge>& appendTo)
{
    do
    {
        tEdge e = tMesh::beginEdge(f);
        tEdge firstFacing, lastFacing;
        FacingRange_EntryUnknown(traverseLine, e, firstFacing, lastFacing);
        bool crossesVertex;
        tEdge exitEdge = ExitEdge_CrossesVertex(traverseLine, firstFacing, lastFacing, crossesVertex);
        eSide side;
        if(crossesVertex)
        {
            side = boundary.side(GetVertexPoint(exitEdge));
        }
        else
        {
            side = traverseLine.intersectionOrder_Clockwise(boundary, GetEdgeLine(exitEdge));
        }
        if(side == SIDE_CENTRE)
        {
            side = SideOfLine_SI(boundary);
        }
        if(side == SIDE_LEFT)
        {
            return;
        }
        appendTo.push_back(exitEdge);
        f = exitEdge.twin().face();
    }
    while(!f->isExternal());
}
void
DoTraverseOutJustToTheLeft(
        tFace f,
        const tLine& traverseLine, const tLine& boundary,
        std::list<tEdge>& appendTo)
{
    do
    {
        tEdge e = tMesh::beginEdge(f);
        tEdge firstFacing, lastFacing;
        FacingRange_EntryUnknown(traverseLine, e, firstFacing, lastFacing);
        bool crossesVertex;
        tEdge exitEdge = ExitEdge_CrossesVertex(traverseLine, firstFacing, lastFacing, crossesVertex);
        eSide side;
        if(crossesVertex)
        {
            side = boundary.side(GetVertexPoint(exitEdge));
            --exitEdge;
        }
        else
        {
            side = traverseLine.intersectionOrder_Clockwise(boundary, GetEdgeLine(exitEdge));
        }
        if(side == SIDE_CENTRE)
        {
            side = SideOfLine_SI(boundary);
        }
        if(side == SIDE_LEFT)
        {
            return;
        }
        appendTo.push_back(exitEdge);
        f = exitEdge.twin().face();
    }
    while(!f->isExternal());
}

tEdge
cExpandSweep::getNextInTraversal()
{
    tLine traverseLine = _frame.boundary(_section);
    tFace f = faceAtBoundary();
    tEdge result;
    TraverseToExit_SI<tMesh>(f, traverseLine, result);
    return result;
}

tEdge
cExpandSweep::nextInBaseSequence()
{
    if(!_addedToBaseSequence.empty())
    {
        return _addedToBaseSequence.back();
    }
    return _frame.baseSequenceEdge(_section, _baseSequencePosition);
}
void
cExpandSweep::advancePastNextInBaseSequence()
{
    if(!_addedToBaseSequence.empty())
    {
        _addedToBaseSequence.pop_back();
    }
    else
    {
        _baseSequencePosition++;
    }
}

void
cExpandSweep::advanceBaseSequencePosition()
{
    while(!_crossedOut.empty() && nextInBaseSequence() == _crossedOut.front())
    {
        _startFace = _crossedOut.front().twin().face();
        _crossedOut.pop_front();
        advancePastNextInBaseSequence();
    }
    tEdge e = nextInBaseSequence();
    while(e.valid() && _frame.sense(_section, e) == SIDE_LEFT)
    {
        _crossedOut.push_front(e.twin());
        advancePastNextInBaseSequence();
        _startFace = _crossedOut.front().face();
        e = nextInBaseSequence();
    }
}

tEdge
cExpandSweep::getFirstVertex() const
{
    tEdge result;
    if(_crossedOut.empty())
    {
        result.clear();
        return result;
    }
    std::list<tEdge>::const_iterator i = _crossedOut.begin();
    result = i->next();
    ++i;
    while(i != _crossedOut.end())
    {
        tEdge candidate = i->next();
        if(_frame.isBefore(_section, GetVertexPoint(result), GetVertexPoint(candidate)))
        {
            result = candidate;
        }
        ++i;
    }
    return result;
}

void
cExpandSweep::setNextEvent()
{
    _nextEvent = EXPAND_SECTION_ENDS;

  // check if the next edge event is before this

    eEvent candidate = UNSET;
    tEdge candidateEdge;
    if(!exitsMesh())
    {
        candidateEdge = getNextInTraversal();
        candidate = INTERNAL_EDGE_CROSSED;
        if(candidateEdge.twin().face()->isExternal())
        {
            candidate = MESH_EXIT;
        }
    }
    else
    if(SideOfLine_SI(_frame.boundary(_section), _crossedOut.back().next().vertex()) == SIDE_LEFT)
    {
      // the boundary line crosses back in through this external edge
        candidateEdge = _crossedOut.back().twin();
        candidate = MESH_ENTRY;
    }

    if(candidate != UNSET && SideOfEdge_SI(candidateEdge, _frame.sectionEnd(_section).end()) == SIDE_LEFT)
    {
        _nextEvent = candidate;
        _nextEventEdge = candidateEdge;
    }

    tEdge firstVertex = getFirstVertex();
    if(firstVertex.valid() && SideOfLine_SI(_frame.boundary(_section), firstVertex.vertex()) != SIDE_RIGHT)
    {
        firstVertex.clear();
    }
    if(firstVertex.valid())
    {
        bool before = (SideOfLine_SI(_frame.sectionEnd(_section), firstVertex.vertex()) == SIDE_LEFT);
        if(before && _nextEvent != EXPAND_SECTION_ENDS)
        {
            tLine vertexIntersector = _frame.expandLine(_section, GetVertexPoint(firstVertex)).reversed();
            tLine edgeLine = GetEdgeLine(_nextEventEdge);
            eSide side = _frame.boundary(_section).intersectionOrder_Clockwise(edgeLine, vertexIntersector);
            // if both events are exactly inline then the vertex event gets precedence
            before = (side != SIDE_LEFT);
        }
        if(before)
        {
            if(VertexIsExternal(firstVertex))
            {
                _nextEvent = EXTERNAL_VERTEX;
            }
            else
            {
                invalid();
                _nextEvent = INTERNAL_VERTEX;
            }
            _nextEventEdge = firstVertex;
        }
    }
}

cExpandSweep::cExpandSweep(
        const cExpansionFrame_SI& frame,
        tFace startFace, std::list<tEdge>& crossedOut) :
 _frame(frame)
{
    _section = 0;
    _baseSequencePosition = 0;
    _startFace = startFace;
    _crossedOut.swap(crossedOut);
    advanceBaseSequencePosition();
    setNextEvent();
}

bool
cExpandSweep::exitsMesh() const
{
    return !_crossedOut.empty() && _crossedOut.back().twin().face()->isExternal();
}
tEdge
cExpandSweep::exitEdge() const
{
    assertD(exitsMesh());
    return _crossedOut.back();
}
tFace
cExpandSweep::startFace() const
{
    return _startFace;
}
tFace
cExpandSweep::firstFaceTouchingVertex(tVertex v) const
{
    tFace f = _startFace;
    std::list<tEdge>::const_iterator i = _crossedOut.begin();
    while(1)
    {
        if(EdgeInFaceForVertex<tMesh>(f, v).valid())
        {
            return f;
        }
        assertD(i != _crossedOut.end());
        f = i->twin().face();
        assertD(!f->isExternal());
    }
}
tFace
cExpandSweep::faceAtBoundary() const
{
    if(_crossedOut.empty())
    {
        return _startFace;
    }
    return _crossedOut.back().twin().face();
}

void
cExpandSweep::advancePastEvent()
{
    if(_nextEvent == EXPAND_SECTION_ENDS)
    {
        assertD(!nextInBaseSequence().valid());
        _section++;
        assertD(_section < _frame.size());
        _baseSequencePosition = 0;
        advanceBaseSequencePosition();
        setNextEvent();
        return;
    }

    if(_nextEvent == MESH_ENTRY)
    {
        assertD(!_crossedOut.empty() && _nextEventEdge == _crossedOut.back().twin());
        _crossedOut.pop_back();
        advanceBaseSequencePosition();
        setNextEvent();
        return;
    }
    if(_nextEvent == INTERNAL_EDGE_CROSSED
     || _nextEvent == MESH_EXIT
     )
    {
        if(_nextEventEdge == nextInBaseSequence())
        {
            assertD(_frame.sense(_section, _nextEventEdge) != SIDE_LEFT);
            advancePastNextInBaseSequence();
            _startFace = _nextEventEdge.twin().face();
        }
        else
        if(!_crossedOut.empty() && _nextEventEdge == _crossedOut.back().twin())
        {
            assertD(_frame.sense(_section, _nextEventEdge) == SIDE_LEFT);
            _crossedOut.pop_back();
        }
        else
        {
            assertD(_frame.sense(_section, _nextEventEdge) == SIDE_RIGHT);
            _crossedOut.push_back(_nextEventEdge);
        }

        advanceBaseSequencePosition();
        setNextEvent();
        return;
    }

    if(_nextEvent == INTERNAL_VERTEX)
    {
//    // in this case, the sweep does not actually advance forward
//    // but instead just extends the base sequence around the internal vertex
//        tEdge e = _nextEventEdge;
//        assertD(_nextEventEdge.face() == _startFace);
//        tVertex v = _nextEventEdge.vertex();
//        while(!_crossedOut.empty() && _crossedOut.front().next().vertex() == v)
//        {
//            _startFace = _crossedOut.front().twin().face();
//            _crossedOut.pop_front();
//        }
//        tEdge next = nextInBaseSequence();
//        while(next.valid() && next.vertex() == v)
//        {
//            e = next.twin().next();
//            advancePastNextInBaseSequence();
//            next = nextInBaseSequence();
//        }
//        while(e.face() != _startFace)
//        {
//            _addedToBaseSequence.push_back(e.twin());
//            e = e.twin().next();
//        }
        invalid();
        return;
    }

    assertD(_nextEvent == EXTERNAL_VERTEX);

    assertD(!_crossedOut.empty());
    const tLine& eventLine = _frame.expandLine(_section, GetVertexPoint(_nextEventEdge));

    {
        tEdge next = nextInBaseSequence();
        if(next.valid() && next == _nextEventEdge && _frame.sense(_section, _nextEventEdge) == SIDE_CENTRE)
        {
        // next base sequence edge is exactly in expand direction
            advancePastNextInBaseSequence();
            _startFace = next.twin().face();
            assertD(!_startFace->isExternal());
        }
    }

    _crossedOut.clear();
    DoTraverseOutJustToTheRight(_startFace, eventLine, _frame.boundary(_section), _crossedOut);

    advanceBaseSequencePosition();
    setNextEvent();
}
