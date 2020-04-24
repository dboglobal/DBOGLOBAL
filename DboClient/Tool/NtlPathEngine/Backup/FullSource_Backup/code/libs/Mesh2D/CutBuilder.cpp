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
#include "interface/CutBuilder.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"

void
cCutBuilder::retireCurrentTraversal()
{
    assertD(!_currentTraversal.empty());
    cCircuitElement* el = new cCircuitElement();
    if(_first || !_startCapValid)
    {
        el->init(_currentDirection, _currentTraversal);
    }
    else
    {
        el->init_WithStartCap(_currentDirection, _startCap, _currentTraversal);
    }
    if(_first)
    {
        _last->connectTo(*el);
    }
    else
    {
        _first = el;
    }
    _last = el;
    assertD(_currentTraversal.empty());
}
void
cCutBuilder::retireCurrentTraversal(const tLine& endCap)
{
    assertD(!_currentTraversal.empty());
    cCircuitElement* el = new cCircuitElement();
    if(_first || !_startCapValid)
    {
        el->init_WithEndCap(_currentDirection, endCap, _currentTraversal);
    }
    else
    {
        el->init_WithStartAndEndCaps(_currentDirection, _startCap, endCap, _currentTraversal);
    }
    if(_first)
    {
        _last->connectTo(*el);
    }
    else
    {
        _first = el;
    }
    _last = el;
    assertD(_currentTraversal.empty());
}

bool
cCutBuilder::elementsAreValid() const
{
    assertD(_first);
    const cCircuitElement* el = _first;
    do
    {
        if(!el->isValid_SI())
        {
            return false;
        }
        el = el->getNext();
    }
    while(el);
    return true;
}

cCutBuilder::~cCutBuilder()
{
    assertD(!_first);
}

void
cCutBuilder::init_Entry(tEdge e, const tLine& direction)
{
    _currentTraversal.init_Entry(e);
    _currentDirection = direction;
    _startCapValid = false;
}
void
cCutBuilder::init_Capped(tFace f, const tLine& capLine, const tLine& direction)
{
    assertD(!f->isExternal());
    assertD(_currentTraversal.empty());
    _currentTraversal.init_InFace(f);
    _currentDirection = direction;
    _startCap = capLine;
    _startCapValid = true;
}
void
cCutBuilder::init_Uncapped(tFace f, const tLine& direction)
{
    assertD(!f->isExternal());
    assertD(_currentTraversal.empty());
    _currentTraversal.init_InFace(f);
    _currentDirection = direction;
    _startCapValid = false;
}

void
cCutBuilder::edgeCrossed(tEdge e)
{
    assertD(!_currentTraversal.empty());
    assertD(e.face() == _currentTraversal.lastFaceCrossed());
    assertD(!e.twin().face()->isExternal());
    _currentTraversal.pushEdgeCrossing(e);
}
void
cCutBuilder::changeDirection(const tLine& newDirection)
{
    assertD(_currentDirection.side(newDirection) != SIDE_CENTRE);
    if(_first || _startCapValid)
    {
        tLine l;
        if(_first)
        {
            l = _last->refLine();
        }
        else
        {
            l = _startCap;
        }
        tIntersection p(l, _currentDirection);
        if(newDirection.side(p) == SIDE_CENTRE)
        {
            assertD(l.side(newDirection) != SIDE_CENTRE);
            _currentDirection = newDirection;
            return;
        }
    }
    retireCurrentTraversal();
    _currentTraversal.init_InFace(_last->lastFaceCrossed());
    _currentDirection = newDirection;
}
void
cCutBuilder::changeDirectionIfDifferent(const tLine& newDirection)
{
    assertD(!_currentTraversal.empty());
    if(_currentDirection.side(newDirection) == SIDE_CENTRE)
    {
        assertD(_currentDirection.direction(newDirection) == DIRECTION_FORWARD);
        assertD(_currentDirection.side(newDirection.start()) == SIDE_CENTRE);
    }
    else
    {
        changeDirection(newDirection);
    }
}

void
cCutBuilder::closeAndTakeElements_Exit(tEdge e, cCircuitElement*& first, cCircuitElement*& last)
{
    assertD(!_currentTraversal.empty());
    assertD(e.face() == _currentTraversal.lastFaceCrossed());
    _currentTraversal.pushExit(e);
    retireCurrentTraversal();
//    assertF(elementsAreValid());  //........... reinstate this and run against flatshead_dungeon.tok
    assertD(_first);
    first = _first;
    last = _last;
    _first = 0;
    _last = 0;
}
void
cCutBuilder::closeAndTakeElements_Capped(const tLine& capLine, cCircuitElement*& first, cCircuitElement*& last)
{
    if(_first)
    {
        tLine l = _last->refLine();
        tIntersection p(l, _currentDirection);
        if(capLine.side(p) == SIDE_CENTRE)
        {
            assertD(l.side(capLine) != SIDE_CENTRE);
            _currentTraversal.clear();
            _last->capEnd(capLine);

            assertF(elementsAreValid());
            assertD(_first);
            first = _first;
            last = _last;
            _first = 0;
            _last = 0;

            return;
        }
    }

    assertD(!_currentTraversal.empty());
    retireCurrentTraversal(capLine);
    assertF(elementsAreValid());
    assertD(_first);
    first = _first;
    last = _last;
    _first = 0;
    _last = 0;
}
void
cCutBuilder::closeAndTakeElements_Uncapped(cCircuitElement*& first, cCircuitElement*& last)
{
    assertD(!_currentTraversal.empty());
    retireCurrentTraversal();
    assertF(elementsAreValid());
    assertD(_first);
    first = _first;
    last = _last;
    _first = 0;
    _last = 0;
}

cCutsBuilder::cCutsBuilder(std::vector<cCircuitElement*>& addTo) :
 _addTo(addTo)
{
    _startIndex = SizeL(addTo);
}

void
cCutsBuilder::meshEntry(tEdge e, const tLine& direction)
{
    _builder.init_Entry(e, direction);
}
void
cCutsBuilder::startCapped(tFace f, const tLine& capLine, const tLine& direction)
{
    _builder.init_Capped(f, capLine, direction);
}
void
cCutsBuilder::startLoop(tFace f, const tLine& direction)
{
    _builder.init_Uncapped(f, direction);
}

void
cCutsBuilder::edgeCrossed(tEdge e)
{
    _builder.edgeCrossed(e);
}
void
cCutsBuilder::changeDirection(const tLine& newDirection)
{
    _builder.changeDirection(newDirection);
}
void
cCutsBuilder::changeDirectionIfDifferent(const tLine& newDirection)
{
    _builder.changeDirectionIfDifferent(newDirection);
}

void
cCutsBuilder::meshExit(tEdge e)
{
    cCircuitElement* first;
    cCircuitElement* last;
    _builder.closeAndTakeElements_Exit(e, first, last);
    _addTo.push_back(first);
    _addTo.push_back(last);
}
void
cCutsBuilder::endCapped(const tLine& capLine)
{
    cCircuitElement* first;
    cCircuitElement* last;
    _builder.closeAndTakeElements_Capped(capLine, first, last);
    _addTo.push_back(first);
    _addTo.push_back(last);
}
void
cCutsBuilder::endLoop()
{
    cCircuitElement* first;
    cCircuitElement* last;
    _builder.closeAndTakeElements_Uncapped(first, last);
    if(SizeL(_addTo) == _startIndex)
    {
        last->connectTo(first);
        _addTo.push_back(first);
        _addTo.push_back(0);
    }
    else
    {
        last->connectTo(_addTo[_startIndex]);
        _addTo[_startIndex] = first;
    }
}

