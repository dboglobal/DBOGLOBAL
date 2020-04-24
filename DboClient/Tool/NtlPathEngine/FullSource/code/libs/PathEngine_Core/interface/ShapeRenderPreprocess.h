//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "platform_common/ThreadSafeHandleTarget.h"

class cConvexPoly;

class cShapeRenderPreprocess : public bThreadSafeHandleTarget
{
    cInternalPosition _preprocessedPosition;
    tMeshCircuits _circuits;

public:

    bool isValidFor(const cInternalPosition& baseShapePosition) const;

    void initOrReinit(
            const cInternalPosition& position,
            const cConvexPoly& placedBaseShape,
            const cConvexPoly& expandShape
            );

    const tMeshCircuits& refCircuits() const
    {
        return _circuits;
    }
};
