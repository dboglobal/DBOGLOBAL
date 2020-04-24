//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef QUERY_LOGGING_WRAPPER_IAGENT_WRAPPER_INCLUDED
#define QUERY_LOGGING_WRAPPER_IAGENT_WRAPPER_INCLUDED

#include "i_pathengine.h"
#include "iAPIObjectWrapper.h"

class iAgent_Wrapper : public iAgent, public iAPIObjectWrapper
{
public:

    typedef iAgent tWrapped;

	iAgent* _target;

    static tSigned32 interfaceIndex()
    {
        return 3;
    }

    void
    destroy();

    iMesh*
    getMesh() const;

    iShape*
    getShape() const;

    void
    setUserData(void* voidPointer);

    void*
    getUserData() const;

    cPosition
    getPosition() const;

    void
    moveTo(const cPosition& newPosition);

    bool
    canCollide() const;

    bool
    canPathfind() const;

    bool
    testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const;

    bool
    testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const;

    bool
    testCollisionTo_XY(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;

    iCollisionInfo*
    firstCollisionTo(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const;

    cPosition
    findClosestUnobstructedPosition(iCollisionContext* context, tSigned32 maximumDistance) const;

    void
    getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, tSigned32& numberOverlapped) const;

    bool
    testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const;

    iPath*
    findShortestPathTo(iCollisionContext* context, const cPosition& target) const;

    iPath*
    findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const;

    iPath*
    findPathAway(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway) const;

    iPath*
    findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const;

    iCollisionInfo*
    advanceAlongPath(iPath* path, float distance, iCollisionContext* context);

    iCollisionInfo*
    advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY);

    iPath*
    generateCurvedPath(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;

    void
    setTraverseCost(float cost);

    void
    setTraverseCostDirection(tSigned32 directionVectorX, tSigned32 directionVectorY);

    float
    getTraverseCost() const;

    void
    getTraverseCostDirection(tSigned32& directionVectorX, tSigned32& directionVectorY) const;

    iPath*
    generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const;

    bool
    isMoveable() const;

    void destroy_APIObjectWrapper();

    void operator delete(void* p) {::operator delete(p);}
};

#endif
