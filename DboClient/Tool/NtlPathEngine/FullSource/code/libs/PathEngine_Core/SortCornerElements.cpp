//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SortCornerElements.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "common/STL/algorithm.h"
#include "platform_common/PointMultiplication.h"

class cCornerPredicate
{
public:

    // returns true if rhs is greater than lhs
    bool operator()(const cCircuitElement* lhs, const cCircuitElement* rhs);
};

bool
cCornerPredicate::operator()(const cCircuitElement* lhs, const cCircuitElement* rhs)
{
    if(lhs == rhs)
    {
        return false;
    }
    if(lhs->faceCrossed(0) != rhs->faceCrossed(0))
    {
        return rhs->faceCrossed(0).index() > lhs->faceCrossed(0).index(); 
    }
    tPoint lhs_p = lhs->cornerPoint();
    tPoint rhs_p = rhs->cornerPoint();
    if(lhs_p.getX() != rhs_p.getX())
    {
        return rhs_p.getX() > lhs_p.getX();
    }
    if(lhs_p.getY() != rhs_p.getY())
    {
        return rhs_p.getY() > lhs_p.getY();
    }
    tPoint lhs_axis = lhs->refLine().axis();
    tPoint rhs_axis = rhs->refLine().axis();
    tPoint zero_axis = tPoint(0, 1, 1);
    eDirection lhs_dir = zero_axis.direction(lhs_axis);
    eDirection rhs_dir = zero_axis.direction(rhs_axis);
    if(lhs_dir == rhs_dir)
    {
        eSide side = lhs_axis.side(rhs_axis);
        assertD(side != SIDE_CENTRE);
        return side == SIDE_RIGHT;
    }
    //.... assumes order of direction enum
    return rhs_dir > lhs_dir;
}

void
SortCornerElements(std::vector<const cCircuitElement*>& elements)
{
    std::sort(elements.begin(), elements.end(), cCornerPredicate());
}

