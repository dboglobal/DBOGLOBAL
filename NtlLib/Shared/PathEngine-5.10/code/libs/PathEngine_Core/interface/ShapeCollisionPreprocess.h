//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SHAPE_COLLISION_PREPROCESS_INCLUDED
#define SHAPE_COLLISION_PREPROCESS_INCLUDED

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class cConvexPoly;
class cLocalisedConvexShape;
class cQueryContext;

class cShapeCollisionPreprocess : public bThreadSafeHandleTarget
{
    cInternalPosition _preprocessedPosition;
    cLocalisedConvexShape* _expansion2D;

public:

    cShapeCollisionPreprocess(); // must be initialised directly after construction
    ~cShapeCollisionPreprocess();

    void initOrReinit(
        cQueryContext& qc,
        const cConvexPoly& placedBaseShape,
        const cInternalPosition& position
        );

    void initOrReinit(
        cQueryContext& qc,
        const cConvexPoly& placedBaseShape,
        const cInternalPosition& position,
        const cConvexPoly& expandShape
        );

    bool isValidFor(const cInternalPosition& baseShapePosition) const;

    const cLocalisedConvexShape* getExpansion2D() const
    {
        return _expansion2D;
    }
};

#endif
