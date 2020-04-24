//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CompoundContextCollisionPreprocess.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/tLine.h"

cHandle<cContextCollisionPreprocess>*
cCompoundContextCollisionPreprocess::preprocessPtr()
{
    if(_preprocessVector)
    {
        return &_preprocessVector->front();
    }
    return _preprocessArray;
}
const cHandle<cContextCollisionPreprocess>*
cCompoundContextCollisionPreprocess::preprocessPtr() const
{
    if(_preprocessVector)
    {
        return &_preprocessVector->front();
    }
    return _preprocessArray;
}
tSigned32*
cCompoundContextCollisionPreprocess::firstIndexPtr()
{
    if(_firstIndexVector)
    {
        return &_firstIndexVector->front();
    }
    return _firstIndexArray;
}
const tSigned32*
cCompoundContextCollisionPreprocess::firstIndexPtr() const
{
    if(_firstIndexVector)
    {
        return &_firstIndexVector->front();
    }
    return _firstIndexArray;
}

cCompoundContextCollisionPreprocess::cCompoundContextCollisionPreprocess() :
 _numberOfPreprocesses(0),
 _preprocessVector(0),
 _firstIndexVector(0)
{
    _firstIndexArray[0] = 0;
}
cCompoundContextCollisionPreprocess::~cCompoundContextCollisionPreprocess()
{
    delete _preprocessVector;
    delete _firstIndexVector;
}

void
cCompoundContextCollisionPreprocess::addPreprocess(cHandle<cContextCollisionPreprocess> preprocess)
{
    if(_numberOfPreprocesses < 4)
    {
        _preprocessArray[_numberOfPreprocesses] = preprocess;
        _firstIndexArray[1 + _numberOfPreprocesses] = _firstIndexArray[_numberOfPreprocesses] + preprocess->refShapeSet().size();
        _numberOfPreprocesses++;
        return;
    }
    if(_numberOfPreprocesses == 4)
    {
        _preprocessVector = new std::vector<cHandle<cContextCollisionPreprocess> >(_numberOfPreprocesses + 1);
        _firstIndexVector = new std::vector<tSigned32>(1 + _numberOfPreprocesses + 1);
        (*_firstIndexVector)[0] = 0;
        for(tSigned32 i = 0; i != _numberOfPreprocesses; ++i)
        {
            (*_preprocessVector)[i] = _preprocessArray[i];
            (*_firstIndexVector)[1 + i] = _firstIndexArray[1 + i];
        }
    }
    _preprocessVector->push_back(preprocess);
    _firstIndexVector->push_back(_firstIndexVector->back() + preprocess->refShapeSet().size());
    _numberOfPreprocesses++;
}

tSigned32
cCompoundContextCollisionPreprocess::size() const
{
    return firstIndexPtr()[_numberOfPreprocesses];
}

void
cCompoundContextCollisionPreprocess::initShapesInFaceIterator(tSigned32 faceIndex, tShapesInFaceIterator& it)
{
    it._faceIndex = faceIndex;
    it._preprocessIndex = 0;
    it._indexInFace = 0;
}
bool
cCompoundContextCollisionPreprocess::nextShapeInFace(tShapesInFaceIterator& it, tSigned32& index, const cLocalisedConvexShape*& shapePtr) const
{
    while(it._indexInFace == preprocessPtr()[it._preprocessIndex]->refShapeSet().shapesInFace(it._faceIndex))
    {
        it._indexInFace = 0;
        ++it._preprocessIndex;
        if(it._preprocessIndex == _numberOfPreprocesses)
        {
            return false;
        }
    }
    const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[it._preprocessIndex]->refShapeSet();
    tSigned32 shapeSetIndex = shapeSet.indexForShapeInFace(it._faceIndex, it._indexInFace);
    index = firstIndexPtr()[it._preprocessIndex] + shapeSetIndex;
    shapePtr = &shapeSet[shapeSetIndex];
    ++it._indexInFace;
    return true;
}
bool
cCompoundContextCollisionPreprocess::nextShapeInFace(tShapesInFaceIterator& it, const cLocalisedConvexShape*& shapePtr) const
{
    while(it._indexInFace == preprocessPtr()[it._preprocessIndex]->refShapeSet().shapesInFace(it._faceIndex))
    {
        it._indexInFace = 0;
        ++it._preprocessIndex;
        if(it._preprocessIndex == _numberOfPreprocesses)
        {
            return false;
        }
    }
    const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[it._preprocessIndex]->refShapeSet();
    tSigned32 shapeSetIndex = shapeSet.indexForShapeInFace(it._faceIndex, it._indexInFace);
    shapePtr = &shapeSet[shapeSetIndex];
    ++it._indexInFace;
    return true;
}
bool
cCompoundContextCollisionPreprocess::nextShapeInFace(tShapesInFaceIterator& it, tSigned32& index, const cLocalisedConvexShape*& shapePtr, cAgent*& agentPtr) const
{
    while(it._indexInFace == preprocessPtr()[it._preprocessIndex]->refShapeSet().shapesInFace(it._faceIndex))
    {
        it._indexInFace = 0;
        ++it._preprocessIndex;
        if(it._preprocessIndex == _numberOfPreprocesses)
        {
            return false;
        }
    }
    const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[it._preprocessIndex]->refShapeSet();
    tSigned32 shapeSetIndex = shapeSet.indexForShapeInFace(it._faceIndex, it._indexInFace);
    index = firstIndexPtr()[it._preprocessIndex] + shapeSetIndex;
    shapePtr = &shapeSet[shapeSetIndex];
    agentPtr = preprocessPtr()[it._preprocessIndex]->getOwnerAgent(shapeSetIndex);
    ++it._indexInFace;
    return true;
}

bool
cCompoundContextCollisionPreprocess::testPointCollision_IgnoringOneAgent(const cInternalPosition& p, const cAgent* agentToIgnore) const
{
    tSigned32 faceIndex = p.cell();
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[i]->refShapeSet();
        tSigned32 j;
        for(j = 0; j != shapeSet.shapesInFace(faceIndex); j++)
        {
            tSigned32 k = shapeSet.indexForShapeInFace(faceIndex, j);
            if(preprocessPtr()[i]->getOwnerAgent(k) == agentToIgnore)
            {
                continue;
            }
            if(shapeSet[k].testPointCollision(p))
            {
                return true;
            }
        }
    }
    return false;
}

tSigned32
cCompoundContextCollisionPreprocess::getAllAgentsOverlapped(const cInternalPosition& p, cAgent** resultsBuffer) const
{
    tSigned32 faceIndex = p.cell();
    tSigned32 numberOverlapped = 0;
    tSigned32 i;
    cAgent** previousPreprocessesStart = resultsBuffer;
    cAgent** previousPreprocessesEnd;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        previousPreprocessesEnd = resultsBuffer;
        const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[i]->refShapeSet();
        tSigned32 j;
        for(j = 0; j != shapeSet.shapesInFace(faceIndex); j++)
        {
            tSigned32 k = shapeSet.indexForShapeInFace(faceIndex, j);
            if(shapeSet[k].testPointCollision(p))
            {
                cAgent* ownerAgent = preprocessPtr()[i]->getOwnerAgent(k);
                if(std::find(previousPreprocessesStart, previousPreprocessesEnd, ownerAgent) == previousPreprocessesEnd)
                {
                    *resultsBuffer++ = ownerAgent;
                    ++numberOverlapped;
                }
            }
        }
    }
    return numberOverlapped;
}

void
cCompoundContextCollisionPreprocess::firstCollision(
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        cAgent*& collidingAgent
        )
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[i]->refShapeSet();
        tFace endF;
        cAxisFraction candidateDistanceAlong;
        tSigned32 candidateLineIndex;
        tSigned32 candidateShape;
        if(LineCollision_AgainstShapeSet(startF, line, shapeSet, candidateShape, candidateLineIndex, candidateDistanceAlong, endF))
        {
            if(!collides || candidateDistanceAlong < distanceAlong)
            {
                collides = true;
                distanceAlong = candidateDistanceAlong;
                collidingAgent = preprocessPtr()[i]->getOwnerAgent(candidateShape);
                collidingLine = shapeSet[candidateShape].poly().getBoundaryLine(candidateLineIndex);
            }
        }
    }
}

void
cCompoundContextCollisionPreprocess::firstCollision(
        tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        cAgent*& collidingAgent
        )
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        const cLocalisedConvexShapeSet& shapeSet = preprocessPtr()[i]->refShapeSet();
        cAxisFraction candidateDistanceAlong;
        tSigned32 candidateLineIndex;
        tSigned32 candidateShape;
        if(LineCollision_AgainstShapeSet(startF, line, startFraction, endFraction, shapeSet, candidateShape, candidateLineIndex, candidateDistanceAlong))
        {
            if(!collides || candidateDistanceAlong < distanceAlong)
            {
                collides = true;
                distanceAlong = candidateDistanceAlong;
                collidingAgent = preprocessPtr()[i]->getOwnerAgent(candidateShape);
                collidingLine = shapeSet[candidateShape].poly().getBoundaryLine(candidateLineIndex);
            }
        }
    }
}

// interface for iPointCollisionTester

bool
cCompoundContextCollisionPreprocess::testPointCollision(cQueryContext& qc, tFace f, const tPoint& p) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testPointCollision(qc, f, p))
        {
            return true;
        }
    }
    return false;
}
bool
cCompoundContextCollisionPreprocess::testPointCollision(cQueryContext& qc, tFace f, const tIntersection& p) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testPointCollision(qc, f, p))
        {
            return true;
        }
    }
    return false;
}

// interface for iLineCollisionTester

bool
cCompoundContextCollisionPreprocess::testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testCollision(qc, from, to))
        {
            return true;
        }
    }
    return false;
}
bool
cCompoundContextCollisionPreprocess::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testCollisionToXY(qc, startF, line))
        {
            return true;
        }
    }
    return false;
}
bool
cCompoundContextCollisionPreprocess::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testCollisionToXY(qc, startF, line, endF))
        {
            return true;
        }
    }
    return false;
}

bool
cCompoundContextCollisionPreprocess::testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
{
    tSigned32 i;
    for(i = 0; i != _numberOfPreprocesses; ++i)
    {
        if(preprocessPtr()[i]->refShapeSet().testForSnagsToSegmentEnd(qc, segmentLine, approximation))
        {
            return true;
        }
    }
    return false;
}
