//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CLOSEST_UNOBSTRUCTED_POINT_INCLUDED
#define CLOSEST_UNOBSTRUCTED_POINT_INCLUDED

#include "libs/Geometry/interface/tIntersection_Header.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"

//class cElementInFaceEntry;
class cLocalisedConvexShapeSet;
class cCompoundContextCollisionPreprocess;
class cFlagVector;
template <class T> class iMeshTranslator;
class cMeshCut;
class cQueryContext;
class cEdgeExpansion;

namespace nClosestUnobstructedPoint
{

cInternalPosition
find(cQueryContext& qc,
        const cInternalPosition& centre,
        tSigned32 radius,
//        const std::vector<std::vector<cElementInFaceEntry> >& staticCircuitsLookup,
        const cEdgeExpansion& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        const cCompoundContextCollisionPreprocess* dynamicExpansions // zero can be passed in to indicate no dynamic expansions
        );

cInternalPosition
find_Reference(cQueryContext& qc,
        const cInternalPosition& centre,
        tSigned32 radius,
//        const std::vector<std::vector<cElementInFaceEntry> >& staticCircuitsLookup,
        const cEdgeExpansion& edgeExpansion,
        const cLocalisedConvexShapeSet& staticExpansions,
        const cCompoundContextCollisionPreprocess* dynamicExpansions // zero can be passed in to indicate no dynamic expansions
        );

void checkResultAgainstReference(const tPoint& centre, const tPoint& reference, const tPoint& result);

}

#endif
