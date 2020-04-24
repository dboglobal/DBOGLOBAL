//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SMALL_CONVEX_SPLIT_INCLUDED
#define SMALL_CONVEX_SPLIT_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "common/Containers/BitVector.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"

class cLocalisedConvexShapeSet;
class cLocalisedConvexShape;
template <class T> class cMeshWithTrackedTransform;
class cCombinedLineCollisionTester;
class cCombinedPointCollisionTester;
class cElementLookupCollisionTester;
class iXMLInputStream;
class iXMLOutputStream;
class cQueryContext;

class cSmallConvexSplit
{
    cBitVector _isSmallConvex;
    std::vector<cLocalisedConvexShape*> _ownedShapes;
    cLocalisedConvexShapeSet* _smallConvexOnOriginal;
    cLocalisedConvexShapeSet* _nonSmallConvex;
//    cMeshWithTrackedTransform<tMesh>* _smallConvexPartitioned;
//    cLocalisedConvexShapeSet* _smallConvexOnPartitioned;

    void
    constructorCommon(
            cQueryContext& qc,
            const cLocalisedConvexShapeSet& baseObstacleExpansions
            );

public:

    cSmallConvexSplit(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& baseObstacleExpansions,
        const cElementLookupCollisionTester& edgeExpansionCollisionTester,
        tSigned32 maxCircumference
        );

    cSmallConvexSplit(
        iXMLInputStream& is,
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& baseObstacleExpansions

        );

    ~cSmallConvexSplit();

    void save(iXMLOutputStream& os) const;

    // returns zero if no partitioning applies
    const cMeshWithTrackedTransform<tMesh>* getPartitioned() const;

    tFace translateToPartitioned(tFace f, const tPoint& p) const;

    const cLocalisedConvexShapeSet& refSmallConvexOnPartitioned() const;
    const cLocalisedConvexShapeSet& refSmallConvexOnOriginal() const;
    const cLocalisedConvexShapeSet& refNonSmallConvex() const;
};

#endif
