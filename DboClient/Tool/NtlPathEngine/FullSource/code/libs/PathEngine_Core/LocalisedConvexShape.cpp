//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LocalisedConvexShape.h"
#include "ResolveFaceForIntersection.h"
#include "interface/OverlappingPolygon.h"
#include "interface/QueryContext.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "common/Containers/FlagVector.h"
#include "common/Containers/CollapsedVectorVector.h"


cLocalisedConvexShape::cLocalisedConvexShape(cQueryContext& qc, const cConvexPoly& poly, const cInternalPosition& centre) :
 _poly(poly),
 _centre(centre)
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();

    FacesOverlappedByOverlappingPolygon_SI(_centre.f, poly, openQueueBuffer, resultBuffer, _exitsMesh);
    _facesOverlapped.resize(resultBuffer.numberSet());
    {
        tSigned32 i;
        for(i = 0; i < resultBuffer.numberSet(); i++)
        {
            _facesOverlapped[i] = resultBuffer.indexSet(i);
        }
    }

    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
}

cLocalisedConvexShape::cLocalisedConvexShape(const cConvexPoly& poly, const cInternalPosition& centre, tFace* facesBuffer, cFlagVector& faceFlagVector) :
 _poly(poly),
 _centre(centre)
{
    FacesOverlappedByOverlappingPolygon_SI(_centre.f, poly, facesBuffer, faceFlagVector, _exitsMesh);
    _facesOverlapped.resize(faceFlagVector.numberSet());
    {
        tSigned32 i;
        for(i = 0; i < faceFlagVector.numberSet(); i++)
        {
            _facesOverlapped[i] = faceFlagVector.indexSet(i);
        }
    }
}

bool
cLocalisedConvexShape::overlapsFace(tSigned32 f) const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_facesOverlapped); i++)
    {
        if(_facesOverlapped[i] == f)
        {
            return true;
        }
    }
    return false;
}

cInternalPosition
cLocalisedConvexShape::centre() const
{
    return _centre;
}

void
cLocalisedConvexShape::getDataForSideElement(tSigned32 side, tPoint* buffer) const
{
    buffer[0] = _poly.vertex(side);
    buffer[1] = _poly.vertex_WrappedAround(side + 1);
    buffer[2] = _poly.vertex_WrappedAround(side + 2);
}

bool
cLocalisedConvexShape::testPointCollision(const cInternalPosition& p) const
{
    if(!_poly.pointCollides2D(p.p))
    {
        return false;
    }
    return overlapsFace(p.f.index());
}
bool
cLocalisedConvexShape::testPointCollision(tFace f, const tIntersection& p) const
{
    if(!_poly.pointCollides2D(p))
    {
        return false;
    }
    return overlapsFace(f.index());
}
bool
cLocalisedConvexShape::testPointCollision_Inverted(const cInternalPosition& p) const
{
    if(_poly.pointCollides2D_Inverted(p.p))
    {
        return true;
    }
    return !overlapsFace(p.f.index());
}
bool
cLocalisedConvexShape::testPointCollision_Inverted(tFace f, const tIntersection& p) const
{
    if(_poly.pointCollides2D_Inverted(p))
    {
        return true;
    }
    return !overlapsFace(f.index());
}

bool
cLocalisedConvexShape::testPointIsOnBoundary(tFace f, const tIntersection& p) const
{
    if(!_poly.pointIsOnBoundary2D(p))
    {
        return false;
    }
    return overlapsFace(f.index());
}
