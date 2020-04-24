//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ManagedSoftObstacleExpansions.h"
#include "ShapeCorners.h"
#include "interface/ObstacleInfo.h"
#include "interface/QueryContext.h"
#include "interface/ShapesCostCalculator.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/ConvexPoly.h"

cManagedSoftObstacleExpansions::cManagedSoftObstacleExpansions()
{
    _costCalculator = 0;
}
cManagedSoftObstacleExpansions::~cManagedSoftObstacleExpansions()
{
    delete _costCalculator;
}

void
cManagedSoftObstacleExpansions::add(
        cQueryContext& qc,
        const std::vector<cObstacleInfo>& baseObstacles,
        const std::vector<cConvexPoly>& baseObstacleShapes,
        const cConvexPoly& expansionShape
        )
{
    tSigned32 i;
    for(i = 0; i < SizeL(baseObstacles); ++i)
    {
        const cObstacleInfo& info = baseObstacles[i];
        if(info._costToTraverse == -1.f)
        {
            continue;
        }
        if(!_costCalculator)
        {
            _costCalculator = new cShapesCostCalculator(qc.refMesh().faces());
        }
        cLocalisedConvexShape* lcs;
        cConvexPoly placedShape;
        baseObstacleShapes[i].offset(info._position.p, placedShape);
        cConvexPoly combinedShape;
        placedShape.minkowskiExpand(expansionShape, combinedShape);
        lcs = new cLocalisedConvexShape(qc, combinedShape, info._position);
        _expansions.push_back(lcs);
        _costCalculator->push_back(lcs, info._costToTraverse, info._directionForCostToTraverse);
        _expansionsLifetimeManager.hold(lcs);
    }
}

void
cManagedSoftObstacleExpansions::add(
        cQueryContext& qc,
        cHandle<cShapeCollisionPreprocess> scp,
        float traverseCost,
        const tPoint& directionForTraverseCost
        )
{
    if(!_costCalculator)
    {
        _costCalculator = new cShapesCostCalculator(qc.refMesh().faces());
    }
    _expansions.push_back(scp->getExpansion2D());
    _costCalculator->push_back(scp->getExpansion2D(), traverseCost, directionForTraverseCost);
    _expansionsLifetimeManager.hold(scp);
}

void
cManagedSoftObstacleExpansions::setCompleted()
{
    if(_costCalculator)
    {
        _costCalculator->setCompleted();
    }
}

void
cManagedSoftObstacleExpansions::addCornersTo(cQueryContext& qc, cCornerProvider& provider) const
{
    tSigned32 i;
    for(i = 0; i < SizeL(_expansions); ++i)
    {
        AddShapeCorners(qc, *_expansions[i], provider);
    }
}

