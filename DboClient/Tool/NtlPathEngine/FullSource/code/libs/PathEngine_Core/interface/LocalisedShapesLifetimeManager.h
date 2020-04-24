//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOCALISED_SHAPES_LIFETIME_MANAGER_INCLUDED
#define LOCALISED_SHAPES_LIFETIME_MANAGER_INCLUDED

#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class cLocalisedShapesLifetimeManager : public bThreadSafeHandleTarget
{
    std::vector<cHandle<cShapeCollisionPreprocess> > _heldPreprocess;
    std::vector<cLocalisedConvexShape*> _ownedShapes;
    std::vector<cHandle<cLocalisedShapesLifetimeManager> > _referencedSets;

    // prevent copying and assignment
    cLocalisedShapesLifetimeManager(const cLocalisedShapesLifetimeManager&);
    const cLocalisedShapesLifetimeManager& operator=(const cLocalisedShapesLifetimeManager&);

public:

    cLocalisedShapesLifetimeManager()
    {
    }
    ~cLocalisedShapesLifetimeManager();

    void hold(cHandle<cShapeCollisionPreprocess> scp);
    void hold(cLocalisedConvexShape* shape);
    void hold(cHandle<cLocalisedShapesLifetimeManager> set);
};

#endif
