//**********************************************************************
//
// Copyright (c) 2002-2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTEXT_PATHFIND_PREPROCESS_INCLUDED
#define CONTEXT_PATHFIND_PREPROCESS_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/PathEngine_Core/interface/CombinedCostCalculator.h"
#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class iLineCollisionTester;
class cCornerProvider;
class cCollisionPreprocess;
class cPathfindPreprocess;
class cInternalPosition;
class cFlagVector;
class cQueryContext;
class cLocalisedConvexShape;
class cLocalisedConvexShapeSet;
class cShapesCostCalculator;
class iMovementCostCalculator;
class cShapePathfindPreprocess;

class cContextPathfindPreprocess : public bThreadSafeHandleTarget
{
    friend class cGraph_DynamicComponent;

    std::vector<cHandle<cShapeCollisionPreprocess> > _heldPreprocess;

    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;

    cCornerProvider* _cornerProvider;
    cCornerProvider* _softCorners; // only valid during construction
    tSigned32 _hardCorners;

    cLocalisedConvexShapeSet* _obstaclesAndSmallConvexOverlapped;

    cShapesCostCalculator* _costCalculator;
    std::vector<iMovementCostCalculator*> _mesh3DCostCalculators;

    cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator> _combinedCostCalculator;

    std::vector<tSigned32> _obstacleFirstCorner;

    std::vector<tSigned32> _smallConvexAdded;

    // called by graph dynamic component

    const cCornerProvider& refCornerProvider() const
    {
        return *_cornerProvider;
    }

    const cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator>* getCostCalculator() const;

    tSigned32
    numberOfObstacles() const
    {
        return _obstaclesAndSmallConvexOverlapped->size();
    }

public:

    cContextPathfindPreprocess(cQueryContext& qc);
    ~cContextPathfindPreprocess();

    void clear();

    void setQueryBounds(const tPoint& boundsMin, const tPoint& boundsMax);

    // ownership is taken
    void setMesh3DCostCalculator(iMovementCostCalculator* calculator);

    void addAgentExpansion(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        const cPathfindPreprocess& pp,
        cHandle<cShapeCollisionPreprocess> scp,
        cHandle<cShapePathfindPreprocess> spp);

    void addAgentExpansion_SoftObstacle(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        cHandle<cShapeCollisionPreprocess> scp,
        float costToTraverse, const tPoint& directionForCostToTraverse
        );
    
    void addMesh3DSoftCornersAndFinalise(const cCornerProvider& mesh3DSoftCorners);
    void finalise();

  // following used for prechecks before path away

    const tPoint& boundsMin() const
    {
        return _boundsMin;
    }
    const tPoint& boundsMax() const
    {
        return _boundsMax;
    }
    bool boundsValid() const
    {
        return _boundsValid;
    }

  // following used for push path around small convex

    const cLocalisedConvexShapeSet& refObstaclesAndSmallConvexOverlapped() const
    {
        return *_obstaclesAndSmallConvexOverlapped;
    }
};

#endif
