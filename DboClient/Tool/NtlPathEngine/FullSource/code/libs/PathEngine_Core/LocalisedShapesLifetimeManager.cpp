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
#include "interface/LocalisedShapesLifetimeManager.h"
#include "interface/LocalisedConvexShape.h"

cLocalisedShapesLifetimeManager::~cLocalisedShapesLifetimeManager()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_ownedShapes); ++i)
    {
        delete _ownedShapes[i];
    }
}

void
cLocalisedShapesLifetimeManager::hold(cHandle<cShapeCollisionPreprocess> scp)
{
    _heldPreprocess.push_back(scp);
}
void
cLocalisedShapesLifetimeManager::hold(cLocalisedConvexShape* shape)
{
    _ownedShapes.push_back(shape);
}
void
cLocalisedShapesLifetimeManager::hold(cHandle<cLocalisedShapesLifetimeManager> set)
{
    _referencedSets.push_back(set);
}
