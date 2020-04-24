//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/ContextPathfindPreprocess.h"
#include "interface/ShapesCostCalculator.h"
#include "ShapeCorners.h"
#include "libs/PathEngine_Core/interface/CollisionPreprocess.h"
#include "libs/PathEngine_Core/interface/PathfindPreprocess.h"
#include "libs/PathEngine_Core/interface/CornerProvider.h"
#include "libs/PathEngine_Core/interface/QueryContext.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/PathEngine_Core/interface/LineCollision.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/PathEngine_Core/interface/ShapePathfindPreprocess.h"
#include "common/Containers/FlagVector.h"
#include "common/STL/algorithm.h"

cContextPathfindPreprocess::cContextPathfindPreprocess(cQueryContext& qc)
{
    _boundsValid = false;

    _cornerProvider = new cCornerProvider(&qc.refMesh());
    _obstacleFirstCorner.push_back(0);

    _obstaclesAndSmallConvexOverlapped = new cLocalisedConvexShapeSet(qc.getNumberOfInternalFaces());

    _costCalculator = new cShapesCostCalculator(qc.getNumberOfInternalFaces());
    _mesh3DCostCalculators.push_back(0);

    _softCorners = 0;
}
cContextPathfindPreprocess::~cContextPathfindPreprocess()
{
    assertD(_softCorners == 0);
    delete _cornerProvider;
    delete _obstaclesAndSmallConvexOverlapped;
    delete _costCalculator;
    delete _mesh3DCostCalculators[0];
}

void
cContextPathfindPreprocess::clear()
{
    assertD(_softCorners == 0);
    _boundsValid = false;

    _cornerProvider->clearWithoutFreeingBuffers();
    _obstacleFirstCorner.resize(0);
    _obstacleFirstCorner.push_back(0);

    _obstaclesAndSmallConvexOverlapped->clear();

    _costCalculator->clear();

    delete _mesh3DCostCalculators[0];
    _mesh3DCostCalculators[0] = 0;

    _combinedCostCalculator.clear();

    _smallConvexAdded.clear();
    _heldPreprocess.clear();
}

void
cContextPathfindPreprocess::setQueryBounds(const tPoint& boundsMin, const tPoint& boundsMax)
{
    _boundsMin = boundsMin;
    _boundsMax = boundsMax;
    _boundsValid = true;
}

void
cContextPathfindPreprocess::setMesh3DCostCalculator(iMovementCostCalculator* calculator)
{
    assertD(_mesh3DCostCalculators[0] == 0);
    _mesh3DCostCalculators[0] = calculator;
    _combinedCostCalculator.setTarget2Vector(&_mesh3DCostCalculators);
}

void
cContextPathfindPreprocess::addAgentExpansion(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        const cPathfindPreprocess& pp,
        cHandle<cShapeCollisionPreprocess> scp,
        cHandle<cShapePathfindPreprocess> spp
        )
{
    _heldPreprocess.push_back(scp);
    const cLocalisedConvexShape* expansion = scp->getExpansion2D();
    const std::vector<tSigned32>& smallConvexOverlapped = spp->refSmallConvexOverlapped();

    if(_boundsValid)
    {
        AddShapeCorners(qc, *expansion, _boundsMin, _boundsMax, *_cornerProvider);
    }
    else
    {
        AddShapeCorners(qc, *expansion, *_cornerProvider);
    }

    _obstaclesAndSmallConvexOverlapped->push_back(expansion);
    _obstacleFirstCorner.push_back(_cornerProvider->size());

    size_t i;
    for(i = 0; i < smallConvexOverlapped.size(); i++)
    {
        tSigned32 toAdd = smallConvexOverlapped[i];
        if(std::find(_smallConvexAdded.begin(), _smallConvexAdded.end(), toAdd) != _smallConvexAdded.end())
        {
        // already added this one
            continue;
        }

        // add corners and circuits for this small convex obstacle

        const cLocalisedConvexShape* smallConvexShape = &(*pp.getSmallConvexOnOriginal())[toAdd];

        if(_boundsValid)
        {
            AddShapeCorners(qc, *smallConvexShape, _boundsMin, _boundsMax, *_cornerProvider);
        }
        else
        {
            AddShapeCorners(qc, *smallConvexShape, *_cornerProvider);
        }

        _obstaclesAndSmallConvexOverlapped->push_back(smallConvexShape);
        _obstacleFirstCorner.push_back(_cornerProvider->size());

        _smallConvexAdded.push_back(toAdd);
    }
}

void
cContextPathfindPreprocess::addAgentExpansion_SoftObstacle(
        cQueryContext& qc,
        const cCollisionPreprocess& cp,
        cHandle<cShapeCollisionPreprocess> scp,
        float costToTraverse, const tPoint& directionForCostToTraverse
        )
{
    _heldPreprocess.push_back(scp);
    const cLocalisedConvexShape* expansion = scp->getExpansion2D();

    assertD(costToTraverse > 0.f);

    if(!_softCorners)
    {
        _softCorners = new cCornerProvider(&qc.refMesh());
    }

    if(_boundsValid)
    {
        AddShapeCorners(qc, *expansion, _boundsMin, _boundsMax, *_softCorners);
    }
    else
    {
        AddShapeCorners(qc, *expansion, *_softCorners);
    }

    _costCalculator->push_back(expansion, costToTraverse, directionForCostToTraverse);
    _combinedCostCalculator.setTarget1(_costCalculator);
}

void
cContextPathfindPreprocess::addMesh3DSoftCornersAndFinalise(const cCornerProvider& mesh3DSoftCorners)
{
    _hardCorners = _cornerProvider->size();
    if(_softCorners)
    {
        _cornerProvider->add(*_softCorners);
        delete _softCorners;
        _softCorners = 0;
    }
    if(_boundsValid)
    {
        _cornerProvider->addFiltered(mesh3DSoftCorners, _boundsMin, _boundsMax);
    }
    else
    {
        _cornerProvider->add(mesh3DSoftCorners);
    }
    _obstaclesAndSmallConvexOverlapped->setCompleted();
    _costCalculator->setCompleted();
}
void
cContextPathfindPreprocess::finalise()
{
    _hardCorners = _cornerProvider->size();
    if(_softCorners)
    {
        _cornerProvider->add(*_softCorners);
        delete _softCorners;
        _softCorners = 0;
    }
    _obstaclesAndSmallConvexOverlapped->setCompleted();
    _costCalculator->setCompleted();
}

const cCombinedCostCalculator<cShapesCostCalculator, iMovementCostCalculator>*
cContextPathfindPreprocess::getCostCalculator() const
{
    if(_combinedCostCalculator.empty())
    {
        return 0;
    }
    return &_combinedCostCalculator;
}
