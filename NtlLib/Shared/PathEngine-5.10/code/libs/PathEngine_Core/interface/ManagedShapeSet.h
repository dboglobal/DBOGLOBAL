//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MANAGED_OBSTACLE_SET_INCLUDED
#define MANAGED_OBSTACLE_SET_INCLUDED

#include "libs/PathEngine_Core/interface/LocalisedConvexShapeSet.h"
#include "libs/PathEngine_Core/interface/ShapeCollisionPreprocess.h"
#include "platform_common/ThreadSafeHandleTarget.h"
#include "common/Handle.h"
#include "common/STL/vector.h"

class cManagedShapeSet : public bThreadSafeHandleTarget
{
    cLocalisedConvexShapeSet _shapeSet;
    std::vector<cHandle<cShapeCollisionPreprocess> > _heldPreprocess;
    std::vector<cLocalisedConvexShape*> _ownedShapes;
    std::vector<cHandle<cManagedShapeSet> > _referencedShapeSets;

public:

    cManagedShapeSet(tSigned32 facesInMesh) :
     _shapeSet(facesInMesh)
    {
    }
    ~cManagedShapeSet();

    void clear();

    void push_back(cHandle<cShapeCollisionPreprocess> scp);
    void push_back(cLocalisedConvexShape* shape);
    void push_back(cHandle<cManagedShapeSet> set);

    void setCompleted();

    const cLocalisedConvexShapeSet& refShapeSet() const;
};

#endif
