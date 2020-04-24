//**********************************************************************
//
// Copyright (c) 2003-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PUSH_PATH_AROUND_INCLUDED
#define PUSH_PATH_AROUND_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

//class cElementInFaceEntry;
template <class T> class iMeshTranslator;
class cLocalisedConvexShapeSet;
class cInternalPosition;
class cEdgeExpansion;

namespace nPushPathAroundObstacles
{

void pushPathAroundSmallConvex(
//        const std::vector<std::vector<cElementInFaceEntry> >& baseElementsLookup,
        const cEdgeExpansion& edgeExpansion, // for convex corner lookup by face
        const cLocalisedConvexShapeSet& nonSmallConvexExpansions,
        const iMeshTranslator<tMesh>* translatorToSmallConvex, // can be zero to indicate that no partitioning is being used
        const cLocalisedConvexShapeSet& smallConvexExpansions,
        const cLocalisedConvexShapeSet* dynamicExpansions,
        const std::vector<cInternalPosition>& path,
        const std::vector<tSigned32>* connectionIndices,
        std::vector<cInternalPosition>& pathResult,
        std::vector<tSigned32>* connectionIndicesResult
        );

}

#endif
