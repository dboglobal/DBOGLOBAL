//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOCALISED_CONVEX_SHAPE_INCLUDED
#define LOCALISED_CONVEX_SHAPE_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/ConvexPoly.h"

class cFlagVector;
class cQueryContext;
template <class T> class cCollapsedVectorVector;
class cAgent;

class cLocalisedConvexShape
{
    cConvexPoly _poly;
    cInternalPosition _centre;
    std::vector<tSigned32> _facesOverlapped;
    bool _exitsMesh;

public:

    cLocalisedConvexShape(cQueryContext& qc, const cConvexPoly& poly, const cInternalPosition& centre);
    cLocalisedConvexShape(const cConvexPoly& poly, const cInternalPosition& centre, tFace* facesBuffer, cFlagVector& faceFlagVector);

    tSigned32
    sides() const
    {
        return _poly.sides();
    }

    const cConvexPoly&
    poly() const
    {
        return _poly;
    }

    bool exitsMesh() const
    {
        return _exitsMesh;
    }

    bool overlapsFace(tSigned32 f) const;
    cInternalPosition centre() const;

    void getDataForSideElement(tSigned32 side, tPoint* buffer) const;

    const std::vector<tSigned32>& refFacesOverlapped() const
    {
        return _facesOverlapped;
    }

    bool testPointCollision(const cInternalPosition& p) const;
    bool testPointCollision(tFace f, const tPoint& p) const
    {
        return testPointCollision(cInternalPosition(f, p));
    }
    // the intersection must be on its axis
    bool testPointCollision(tFace f, const tIntersection& p) const;

    bool testPointCollision_Inverted(const cInternalPosition& p) const;
    // the intersection must be on its axis
    bool testPointCollision_Inverted(tFace f, const tIntersection& p) const;

    bool testPointIsOnBoundary(tFace f, const tIntersection& p) const;
};

#endif
