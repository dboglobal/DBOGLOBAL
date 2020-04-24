//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "iAgent_Wrapper.h"
#include "Logger.h"
#include "WrapperManagement.h"
#include "AllWrappers.h"
void
iAgent_Wrapper::destroy()
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 0, (void*)this);
    gLogger.methodEntry_Completed();
    _target->destroy();
    gLogger.methodExit(3, 0, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

iMesh*
iAgent_Wrapper::getMesh() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 1, (void*)this);
    gLogger.methodEntry_Completed();
    iMesh* result = _target->getMesh();
    ExtractWrapper<iMesh_Wrapper>(result);
    gLogger.methodExit(3, 1, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iShape*
iAgent_Wrapper::getShape() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 2, (void*)this);
    gLogger.methodEntry_Completed();
    iShape* result = _target->getShape();
    ExtractWrapper<iShape_Wrapper>(result);
    gLogger.methodExit(3, 2, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iAgent_Wrapper::setUserData(void* voidPointer)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 3, (void*)this);
    gLogger.log(voidPointer);
    gLogger.methodEntry_Completed();
    _target->setUserData(voidPointer);
    gLogger.methodExit(3, 3, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void*
iAgent_Wrapper::getUserData() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 4, (void*)this);
    gLogger.methodEntry_Completed();
    void* result = _target->getUserData();
    gLogger.methodExit(3, 4, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iAgent_Wrapper::getPosition() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 5, (void*)this);
    gLogger.methodEntry_Completed();
    cPosition result = _target->getPosition();
    gLogger.methodExit(3, 5, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iAgent_Wrapper::moveTo(const cPosition& newPosition)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 6, (void*)this);
    gLogger.log(newPosition);
    gLogger.methodEntry_Completed();
    _target->moveTo(newPosition);
    gLogger.methodExit(3, 6, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

bool
iAgent_Wrapper::canCollide() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 7, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->canCollide();
    gLogger.methodExit(3, 7, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iAgent_Wrapper::canPathfind() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 8, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->canPathfind();
    gLogger.methodExit(3, 8, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iAgent_Wrapper::testCollisionAt(iCollisionContext* context, const cPosition& newPosition) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 9, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(newPosition);
    gLogger.methodEntry_Completed();
    bool result = _target->testCollisionAt(ExtractWrapped<iCollisionContext_Wrapper>(context), newPosition);
    gLogger.methodExit(3, 9, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iAgent_Wrapper::testCollisionTo(iCollisionContext* context, const cPosition& newPosition) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 10, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(newPosition);
    gLogger.methodEntry_Completed();
    bool result = _target->testCollisionTo(ExtractWrapped<iCollisionContext_Wrapper>(context), newPosition);
    gLogger.methodExit(3, 10, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iAgent_Wrapper::testCollisionTo_XY(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 11, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    bool result = _target->testCollisionTo_XY(ExtractWrapped<iCollisionContext_Wrapper>(context), x, y, cell);
    gLogger.methodExit(3, 11, queryCount, (void*)this);
    gLogger.log(cell);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionInfo*
iAgent_Wrapper::firstCollisionTo(iCollisionContext* context, tSigned32 x, tSigned32 y, tSigned32& cell) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 12, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(x);
    gLogger.log(y);
    gLogger.methodEntry_Completed();
    iCollisionInfo* result = _target->firstCollisionTo(ExtractWrapped<iCollisionContext_Wrapper>(context), x, y, cell);
    result = WrapNew<iCollisionInfo_Wrapper>(result);
    gLogger.methodExit(3, 12, queryCount, (void*)this);
    gLogger.log(cell);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

cPosition
iAgent_Wrapper::findClosestUnobstructedPosition(iCollisionContext* context, tSigned32 maximumDistance) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 13, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(maximumDistance);
    gLogger.methodEntry_Completed();
    cPosition result = _target->findClosestUnobstructedPosition(ExtractWrapped<iCollisionContext_Wrapper>(context), maximumDistance);
    gLogger.methodExit(3, 13, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iAgent_Wrapper::getAllAgentsOverlapped(iCollisionContext* context, iAgent** resultsBuffer, tSigned32& numberOverlapped) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 14, (void*)this);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    _target->getAllAgentsOverlapped(ExtractWrapped<iCollisionContext_Wrapper>(context), resultsBuffer, numberOverlapped);
    ExtractWrappers(resultsBuffer, numberOverlapped);
    gLogger.methodExit(3, 14, queryCount, (void*)this);
    gLogger.logAgentVector(resultsBuffer, numberOverlapped);
    gLogger.methodExit_Completed();
}

bool
iAgent_Wrapper::testCollisionDirectlyAgainstPlacedShape(iShape* shape, const cPosition& shapePosition) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 15, (void*)this);
    gLogger.log((void*)shape);
    gLogger.log(shapePosition);
    gLogger.methodEntry_Completed();
    bool result = _target->testCollisionDirectlyAgainstPlacedShape(ExtractWrapped<iShape_Wrapper>(shape), shapePosition);
    gLogger.methodExit(3, 15, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iAgent_Wrapper::findShortestPathTo(iCollisionContext* context, const cPosition& target) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 16, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(target);
    gLogger.methodEntry_Completed();
    iPath* result = _target->findShortestPathTo(ExtractWrapped<iCollisionContext_Wrapper>(context), target);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 16, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iAgent_Wrapper::findShortestPathTo_WithQueryCallBack(iCollisionContext* context, const cPosition& target, iQueryCallBack* queryCallBack) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 17, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(target);
    gLogger.log((void*)queryCallBack);
    gLogger.methodEntry_Completed();
    iQueryCallBack_Wrapper queryCallBack_Wrapper;
    queryCallBack_Wrapper._target = queryCallBack;
    iPath* result = _target->findShortestPathTo_WithQueryCallBack(ExtractWrapped<iCollisionContext_Wrapper>(context), target, &queryCallBack_Wrapper);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 17, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iAgent_Wrapper::findPathAway(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 18, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(awayFrom);
    gLogger.log(distanceAway);
    gLogger.methodEntry_Completed();
    iPath* result = _target->findPathAway(ExtractWrapped<iCollisionContext_Wrapper>(context), awayFrom, distanceAway);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 18, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iAgent_Wrapper::findPathAway_WithQueryCallBack(iCollisionContext* context, const cPosition& awayFrom, tSigned32 distanceAway, iQueryCallBack* queryCallBack) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 19, (void*)this);
    gLogger.log((void*)context);
    gLogger.log(awayFrom);
    gLogger.log(distanceAway);
    gLogger.log((void*)queryCallBack);
    gLogger.methodEntry_Completed();
    iQueryCallBack_Wrapper queryCallBack_Wrapper;
    queryCallBack_Wrapper._target = queryCallBack;
    iPath* result = _target->findPathAway_WithQueryCallBack(ExtractWrapped<iCollisionContext_Wrapper>(context), awayFrom, distanceAway, &queryCallBack_Wrapper);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 19, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionInfo*
iAgent_Wrapper::advanceAlongPath(iPath* path, float distance, iCollisionContext* context)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 20, (void*)this);
    gLogger.log((void*)path);
    gLogger.log(distance);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    iCollisionInfo* result = _target->advanceAlongPath(ExtractWrapped<iPath_Wrapper>(path), distance, ExtractWrapped<iCollisionContext_Wrapper>(context));
    result = WrapNew<iCollisionInfo_Wrapper>(result);
    gLogger.methodExit(3, 20, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iCollisionInfo*
iAgent_Wrapper::advanceAlongPathWithPrecision(iPath* path, float distance, iCollisionContext* context, float& precisionX, float& precisionY)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 21, (void*)this);
    gLogger.log((void*)path);
    gLogger.log(distance);
    gLogger.log((void*)context);
    gLogger.methodEntry_Completed();
    iCollisionInfo* result = _target->advanceAlongPathWithPrecision(ExtractWrapped<iPath_Wrapper>(path), distance, ExtractWrapped<iCollisionContext_Wrapper>(context), precisionX, precisionY);
    result = WrapNew<iCollisionInfo_Wrapper>(result);
    gLogger.methodExit(3, 21, queryCount, (void*)this);
    gLogger.log(precisionX);
    gLogger.log(precisionY);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

iPath*
iAgent_Wrapper::generateCurvedPath(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 22, (void*)this);
    gLogger.log((void*)basePath);
    gLogger.log((void*)context);
    gLogger.log(startVectorX);
    gLogger.log(startVectorY);
    gLogger.log(sectionLength);
    gLogger.log(turnRatio1);
    gLogger.log(turnRatio2);
    gLogger.methodEntry_Completed();
    iPath* result = _target->generateCurvedPath(ExtractWrapped<iPath_Wrapper>(basePath), ExtractWrapped<iCollisionContext_Wrapper>(context), startVectorX, startVectorY, sectionLength, turnRatio1, turnRatio2);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 22, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

void
iAgent_Wrapper::setTraverseCost(float cost)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 23, (void*)this);
    gLogger.log(cost);
    gLogger.methodEntry_Completed();
    _target->setTraverseCost(cost);
    gLogger.methodExit(3, 23, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

void
iAgent_Wrapper::setTraverseCostDirection(tSigned32 directionVectorX, tSigned32 directionVectorY)
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 24, (void*)this);
    gLogger.log(directionVectorX);
    gLogger.log(directionVectorY);
    gLogger.methodEntry_Completed();
    _target->setTraverseCostDirection(directionVectorX, directionVectorY);
    gLogger.methodExit(3, 24, queryCount, (void*)this);
    gLogger.methodExit_Completed();
}

float
iAgent_Wrapper::getTraverseCost() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 25, (void*)this);
    gLogger.methodEntry_Completed();
    float result = _target->getTraverseCost();
    gLogger.methodExit(3, 25, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}

void
iAgent_Wrapper::getTraverseCostDirection(tSigned32& directionVectorX, tSigned32& directionVectorY) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 26, (void*)this);
    gLogger.methodEntry_Completed();
    _target->getTraverseCostDirection(directionVectorX, directionVectorY);
    gLogger.methodExit(3, 26, queryCount, (void*)this);
    gLogger.log(directionVectorX);
    gLogger.log(directionVectorY);
    gLogger.methodExit_Completed();
}

iPath*
iAgent_Wrapper::generateCurvedPath_WithEndVector(iPath* basePath, iCollisionContext* context, tSigned32 startVectorX, tSigned32 startVectorY, tSigned32 endVectorX, tSigned32 endVectorY, tSigned32 sectionLength, float turnRatio1, float turnRatio2) const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 27, (void*)this);
    gLogger.log((void*)basePath);
    gLogger.log((void*)context);
    gLogger.log(startVectorX);
    gLogger.log(startVectorY);
    gLogger.log(endVectorX);
    gLogger.log(endVectorY);
    gLogger.log(sectionLength);
    gLogger.log(turnRatio1);
    gLogger.log(turnRatio2);
    gLogger.methodEntry_Completed();
    iPath* result = _target->generateCurvedPath_WithEndVector(ExtractWrapped<iPath_Wrapper>(basePath), ExtractWrapped<iCollisionContext_Wrapper>(context), startVectorX, startVectorY, endVectorX, endVectorY, sectionLength, turnRatio1, turnRatio2);
    result = WrapNew<iPath_Wrapper>(result);
    gLogger.methodExit(3, 27, queryCount, (void*)this);
    gLogger.log((void*)result);
    gLogger.methodExit_Completed();
    return result;
}

bool
iAgent_Wrapper::isMoveable() const
{
    tUnsigned32 queryCount = gLogger.methodEntry(3, 28, (void*)this);
    gLogger.methodEntry_Completed();
    bool result = _target->isMoveable();
    gLogger.methodExit(3, 28, queryCount, (void*)this);
    gLogger.log(result);
    gLogger.methodExit_Completed();
    return result;
}
void iAgent_Wrapper::destroy_APIObjectWrapper()
{
    delete this;
}
