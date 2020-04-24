//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CUT_BUILDER_INCLUDED
#define CUT_BUILDER_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/EdgesTraversal.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/STL/vector.h"

class cCircuitElement;

class cCutBuilder
{
    cCircuitElement* _first;
    cCircuitElement* _last;

// current traversal
    cEdgesTraversal _currentTraversal;
    tLine _currentDirection;
    tLine _startCap;
    bool _startCapValid;

    const tLine& currentOrPendingDirection() const;
    void retireCurrentTraversal();
    void retireCurrentTraversal(const tLine& endCap);
    bool elementsAreValid() const;

public:

    cCutBuilder()
    {
        _first = 0;
        _last = 0;
    }
    ~cCutBuilder();

    void init_Entry(tEdge e, const tLine& direction);
    void init_Capped(tFace f, const tLine& capLine, const tLine& direction);
    void init_Uncapped(tFace f, const tLine& direction);

    void edgeCrossed(tEdge e);
    void changeDirection(const tLine& newDirection);
    void changeDirectionIfDifferent(const tLine& newDirection);

    void closeAndTakeElements_Exit(tEdge e, cCircuitElement*& first, cCircuitElement*& last);
    void closeAndTakeElements_Capped(const tLine& capLine, cCircuitElement*& first, cCircuitElement*& last);
    void closeAndTakeElements_Uncapped(cCircuitElement*& first, cCircuitElement*& last);
};

class cCutsBuilder
{
    std::vector<cCircuitElement*>& _addTo;
    tSigned32 _startIndex;
    cCutBuilder _builder;

public:

    cCutsBuilder(std::vector<cCircuitElement*>& addTo);

    void meshEntry(tEdge e, const tLine& direction);
    void startCapped(tFace f, const tLine& capLine, const tLine& direction);
    void startLoop(tFace f, const tLine& direction);

    void edgeCrossed(tEdge e);
    void changeDirection(const tLine& newDirection);
    void changeDirectionIfDifferent(const tLine& newDirection);

    void meshExit(tEdge e);
    void endCapped(const tLine& capLine);
    void endLoop();
};


#endif
