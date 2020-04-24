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
#include "interface/ManagedShapeSet.h"
#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"

cManagedShapeSet::~cManagedShapeSet()
{
    tSigned32 i;
    for(i = 0; i != SizeL(_ownedShapes); ++i)
    {
        delete _ownedShapes[i];
    }
}

void
cManagedShapeSet::clear()
{
    _shapeSet.clear();
    _heldPreprocess.resize(0);
    _ownedShapes.resize(0);
}

void
cManagedShapeSet::push_back(cHandle<cShapeCollisionPreprocess> scp)
{
    _shapeSet.push_back(scp->getExpansion2D());
    _heldPreprocess.push_back(scp);
}
void
cManagedShapeSet::push_back(cLocalisedConvexShape* shape)
{
    _shapeSet.push_back(shape);
    _ownedShapes.push_back(shape);
}
void
cManagedShapeSet::push_back(cHandle<cManagedShapeSet> set)
{
    tSigned32 i;
    for(i = 0; i != set->refShapeSet().size(); ++i)
    {
        _shapeSet.push_back(&set->refShapeSet()[i]);
    }
    _referencedShapeSets.push_back(set);
}

void
cManagedShapeSet::setCompleted()
{
    _shapeSet.setCompleted();
}

const cLocalisedConvexShapeSet&
cManagedShapeSet::refShapeSet() const
{
    return _shapeSet;
}

