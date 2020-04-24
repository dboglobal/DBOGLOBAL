//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MANAGED_SOFT_OBSTACLE_EXPANSIONS_INCLUDED
#define MANAGED_SOFT_OBSTACLE_EXPANSIONS_INCLUDED

#include "libs/PathEngine_Core/interface/LocalisedShapesLifetimeManager.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class cQueryContext;
class cLocalisedConvexShape;
class cShapesCostCalculator;
class cObstacleInfo;
class cConvexPoly;
class cCornerProvider;
class cAgent;

class cManagedSoftObstacleExpansions : public bThreadSafeHandleTarget
{
    std::vector<const cLocalisedConvexShape*> _expansions;
    cShapesCostCalculator* _costCalculator;
    cLocalisedShapesLifetimeManager _expansionsLifetimeManager;

    // prevent copying and assignment
    cManagedSoftObstacleExpansions(const cManagedSoftObstacleExpansions&);
    const cManagedSoftObstacleExpansions& operator=(const cManagedSoftObstacleExpansions&);

public:

    cManagedSoftObstacleExpansions();
	~cManagedSoftObstacleExpansions();

    void add(
            cQueryContext& qc,
            const std::vector<cObstacleInfo>& baseObstacles,
            const std::vector<cConvexPoly>& baseObstacleShapes,
            const cConvexPoly& expansionShape
            );
    void add(
            cQueryContext& qc,
            cHandle<cShapeCollisionPreprocess> scp,
            float traverseCost,
            const tPoint& directionForTraverseCost
            );
    void setCompleted();

    cShapesCostCalculator& refCostCalculator()
    {
        return *_costCalculator;
    }

    void addCornersTo(cQueryContext& qc, cCornerProvider& provider) const;
};

#endif
