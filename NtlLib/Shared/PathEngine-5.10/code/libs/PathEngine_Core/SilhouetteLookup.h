//**********************************************************************
//
// Copyright (c) 2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SILHOUETTE_LOOKUP_INCLUDED
#define SILHOUETTE_LOOKUP_INCLUDED

#include "PointsOfVisibilityState.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "common/Containers/CollapsedVectorVector.h"
#include "common/STL/vector.h"

class iXMLInputStream;
class iXMLOutputStream;
class cQueryContext;
template <class T> class cMeshWithTrackedTransform;
class cCombinedStaticCircuits;
class cGraph_StaticComponent;

class cSilhouetteLookup
{
    cCollapsedVectorVector<tUnsigned32> _lookup;

    void
    buildLookupFromReachableStateVectors(
        std::vector<std::vector<cPointsOfVisibilityState> >& silhouetteLookupVectors
        );

public:

    cSilhouetteLookup()
    {
    }

    cSilhouetteLookup(
            cQueryContext& qc,
            cMeshWithTrackedTransform<tMesh>& partitioned,
            const cCombinedStaticCircuits& combinedCircuits,
            cGraph_StaticComponent& staticGraph,
            tSigned32 numberOfHardCorners
            );

    cSilhouetteLookup(iXMLInputStream& is);

    void save(iXMLOutputStream& os) const;

    void
    swap(cSilhouetteLookup& rhs)
    {
        _lookup.swap(rhs._lookup);
    }

    tSigned32
    pointsFromFace(tSigned32 f) const
    {
        return _lookup.subVectorSize(f * 2);
    }
    tSigned32
    pointFromFace(tSigned32 f, tSigned32 i) const
    {
        return static_cast<tSigned32>(_lookup[f * 2][i]);
    }
    tSigned32
    statesFromFace(tSigned32 f) const
    {
        return _lookup.subVectorSize(f * 2 + 1);
    }
    cPointsOfVisibilityState
    stateFromFace(tSigned32 f, tSigned32 i) const
    {
        return cPointsOfVisibilityState::fromValue(_lookup[f * 2 + 1][i]);
    }
};


#endif
