//**********************************************************************
//
// Copyright (c) 2002-2007
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/cPath.h"
#include "CurvedPath.h"
#include "Pool.h"
#include "interface/cCollisionContext.h"
#include "interface/cCollisionInfo.h"
#include "interface/Singletons.h"
#include "interface/cMesh.h"
#include "interface/cShape.h"
#include "interface/cPathEngine.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "platform_common/CoordinateRanges.h"
#include "platform_common/FloatPersistence.h"
#include "common/MutexLock.h"
#include "common/interface/Attributes.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/STL/algorithm.h"
#include "iAPIObjectWrapper.h"

using namespace std;

//..... TODO put this somewhere sensible
static tPoint
PointForPosition(const cPosition& position)
{
    return tPoint(position.x, position.y, WORLD_RANGE);
}

void
cPath::init_common()
{
    if(_size > 1)
    {
        _length = 0;
        tSigned32 i;
        for(i = 0; i + 1 < _size; i++)
        {
            _length += Length(_internalPositions[i + 1].p - _internalPositions[i].p);
            if(_connectionIndices && (*_connectionIndices)[i] != -1)
            {
                assertD(_mesh);
                _length += _mesh->lookupPenaltyForOffMeshConnection((*_connectionIndices)[i]);
            }
        }
//        _originalStart = _points.front();
        _distanceFromOriginalStart = 0.f;
//        _toggleApproximationDirection = false;
        _segmentInterpolator.initOrReinit(_internalPositions[0].p, _internalPositions[1].p);
    }
}

float
cPath::getAdvanceInfo(
        float distance,
        bool& reachesNextPoint,
        bool& exactlyOnLine,
        tPoint& point1, tPoint& point2
        )
{
    assertD(_size > 1);

    distance += _distanceFromOriginalStart;
    if(static_cast<tSigned32>(distance) >= _segmentInterpolator.segmentLength())
    {
        reachesNextPoint = true;
        distance -= static_cast<float>(_segmentInterpolator.segmentLength());
        if(distance < 0.f)
        {
            distance = 0.f;
        }
        return distance;
    }
    reachesNextPoint = false;

    tSigned32 interpolatorValue = static_cast<tSigned32>(distance * _segmentInterpolator.interpolatorMax()) / _segmentInterpolator.segmentLength();
//    assertD(interpolatorValue < _segmentInterpolator.interpolatorMax());
    if(interpolatorValue == _segmentInterpolator.interpolatorMax())
    {
        interpolatorValue--;
    }
    _segmentInterpolator.interpolate(PointForPosition(position(0)), interpolatorValue, exactlyOnLine, point1, point2);
    return 0.f;

//    tLine originalLine(PointForPosition(_originalStart), PointForPosition(position(1)));
//    tSigned32 originalLineLength = Length(originalLine.axis());
//    float updatedDistance = distance + _distanceFromOriginalStart;
//    if(static_cast<tSigned32>(updatedDistance) >= originalLineLength)
//    {
//        reachesNextPoint = true;
//        updatedDistance -= static_cast<float>(originalLineLength);
//        if(updatedDistance < 0.f)
//        {
//            updatedDistance = 0.f;
//        }
//        return updatedDistance;
//    }
//    reachesNextPoint = false;
//
//    tLine currentLine(PointForPosition(position(0)), PointForPosition(position(1)));
//    tSigned32 currentLineLength = Length(currentLine.axis());
//    if(static_cast<tSigned32>(distance) >= currentLineLength)
//    {
//        distance = static_cast<float>(currentLineLength - 1);
//    }
//    ApproximatePointsAlongLine(currentLine, currentLineLength, distance, exactlyOnLine, point1, point2);
//
//    if(!exactlyOnLine)
//    {
//        assertD(currentLine.side(point1) == SIDE_LEFT);
//        assertD(currentLine.side(point2) == SIDE_RIGHT);
//        eSide sideForOriginalStart = currentLine.side(originalLine.start());
//        if(sideForOriginalStart == SIDE_LEFT)
//        {
//            // preferred approximation is set to negate current drift from original segment line
//            std::swap(point1, point2);
//        }
//    }
//
//    return 0.f;
}

void
cPath::pop_front()
{
    assertD(_size > 1);
    if(connectionIndex(0) != -1)
    {
        _length -= _mesh->lookupPenaltyForOffMeshConnection(connectionIndex(0));
	    _length -= Length(_internalPositions[_startOffset + 1].p - _internalPositions[_startOffset].p);
    }
    else
    {
        _length -= _segmentInterpolator.segmentLength();
    }
    _startOffset++;
//...... TODO if this gets above a certain value then copy down to new vectors
    _size--;
    _distanceFromOriginalStart = 0;
    if(_size > 1)
    {
        assertD(_length > 0);
        _segmentInterpolator.initOrReinit(_internalPositions[_startOffset].p, _internalPositions[_startOffset + 1].p);
    }
    else
    {
        assertD(_length == 0);
    }
}

void
//cPath::advancePoint0To(const cInternalPosition& p, float distanceAdvanced, bool exactlyOnPath, const tPoint& p2)
cPath::advancePoint0To(const cInternalPosition& p, float distanceAdvanced)
{
    assertD(size() > 1);
    _internalPositions[_startOffset] = p;
    _points[_startOffset].x = p.p.getX();
    _points[_startOffset].y = p.p.getY();
    _points[_startOffset].cell = p.cell();
    _distanceFromOriginalStart += distanceAdvanced;
//    _isExactlyOnPath = exactlyOnPath;
//    _approximationOnOtherSide = p2;
}

void
cPath::finaliseAdvance(
        const cInternalPosition& newPosition,
        float distance,
//        bool exactlyOnLine,
//        const tPoint& point2,
        float& precisionX, float& precisionY
        )
{
    if(newPosition.f->isExternal())
    {
      // this should only happen if the path crosses through space obstructed by the edge of the mesh
      // which should only happen in the case of calling pathfinding queries for obstructed start position
      // skipping to the end of the segment is chosen because this is relatively straightforward and enables the agent to continue advancing
        pop_front();
        precisionX = 0.0f;
        precisionY = 0.0f;
    }
    else
    {
        assertD(newPosition != internalPosition(1));
        advancePoint0To(newPosition, distance);
        getPrecisionXY(precisionX, precisionY);
    }
}

void
cPath::finaliseAdvance(
        const cPosition& newPosition,
        float distance,
        float& precisionX, float& precisionY
        )
{
    if(newPosition.cell == -1)
    {
      // this should only happen if the path crosses through space obstructed by the edge of the mesh
      // which should only happen in the case of calling pathfinding queries for obstructed start position
      // skipping to the end of the segment is chosen because this is relatively straightforward and enables the agent to continue advancing
        pop_front();
        precisionX = 0.0f;
        precisionY = 0.0f;
    }
    else
    {
        assertD(newPosition != position(1));
        cInternalPosition ip;
        _mesh->convertPosition(newPosition, ip);
        advancePoint0To(ip, distance);
        getPrecisionXY(precisionX, precisionY);
    }
}

cPath::cPath() :
 _internalPositions(nSingletons::pathEngine().getPathReserveSize()),
 _points(_internalPositions.size())
{
    _wrapper = 0; // if logging is on then this will always be reset after init
    _poolIndex = -1;
    _heldNode.setInvalid();
    _mesh = 0;
}

cPath::~cPath()
{
    assertD(_poolIndex == -1); // assert not held by pool
    assertD(!_heldNode.valid()); // assert that an external reference is not held
    assertD(_mesh == 0);
}

// initialisation (or reinitialisation)

void
cPath::init(const cInternalPosition& singlePoint)
{
    _connectionIndices = 0;
    _size = 1;
    _startOffset = 0;
    _internalPositions.resize(1);
    _internalPositions[0] = singlePoint;
    _points.resize(1);

    _points[0].x = singlePoint.p.getX();
    _points[0].y = singlePoint.p.getY();
    _points[0].cell = singlePoint.cell();
}

void
cPath::init(std::vector<cInternalPosition>& points, std::vector<tSigned32>* connectionIndices)
{
    assertD(!points.empty());
    _connectionIndices = connectionIndices;
    _size = SizeL(points);
    _startOffset = 0;
    _internalPositions.swap(points);
    _points.resize(_size);
    tSigned32 i;
    for(i = 0; i < _size; i++)
    {
        const cInternalPosition& p = _internalPositions[i];
        _points[i].x = p.p.getX();
        _points[i].y = p.p.getY();
        _points[i].cell = p.cell();
        assertD(_points[i].cell >= 0);
    }
    init_common();
}

void
cPath::init(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathSize)
{
    assertD(pathSize > 0);
    assertD(_mesh);
    if(connectionIndicesBuffer)
    {
        assertD(pathSize > 1);
        _connectionIndices = new std::vector<tSigned32>(pathSize - 1);
        tSigned32 i;
        for(i = 0; i + 1 < pathSize; ++i)
        {
            (*_connectionIndices)[i] = connectionIndicesBuffer[i];
        }
    }
    else
    {
        _connectionIndices = 0;
    }
    _size = pathSize;
    _startOffset = 0;
    _internalPositions.resize(_size);
    _points.resize(_size);
    tSigned32 i;
    for(i = 0; i < _size; i++)
    {
        _points[i] = positionsBuffer[i];
        const cInternalPosition p; 
        _mesh->convertPosition(positionsBuffer[i], _internalPositions[i]);
        assertD(_points[i].cell >= 0);
    }
    init_common();
}
void
cPath::init_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathSize)
{
    assertD(pathSize > 0);
    assertD(_mesh);
    if(connectionIndicesBuffer)
    {
        assertD(pathSize > 1);
        _connectionIndices = new std::vector<tSigned32>(pathSize - 1);
        tSigned32 i;
        for(i = 0; i + 1 < pathSize; ++i)
        {
            (*_connectionIndices)[pathSize - 2 - i] = connectionIndicesBuffer[i];
        }
    }
    else
    {
        _connectionIndices = 0;
    }
    _size = pathSize;
    _startOffset = 0;
    _internalPositions.resize(_size);
    _points.resize(_size);
    tSigned32 i;
    for(i = 0; i < _size; i++)
    {
        _points[i] = positionsBuffer[pathSize - 1 - i];
        const cInternalPosition p;
        _mesh->convertPosition(positionsBuffer[pathSize - 1 - i], _internalPositions[i]);
        assertD(_points[i].cell >= 0);
    }
    init_common();
}

void
cPath::load(iXMLInputStream& is, const char *const* rootAttributes)
{
    assertD(_mesh);
    _size = ValueAsLong(rootAttributes, "size");
    assertD(_size > 0);
    tPoint originalStart;
    if(_size > 1)
    {
        _length = ValueAsUnsignedLong(rootAttributes, "length");
//        _originalStart.cell = ValueAsLong(rootAttributes, "startCell");
//        _originalStart.x = ValueAsLong(rootAttributes, "startX");
//        _originalStart.y = ValueAsLong(rootAttributes, "startY");
        originalStart = tPoint(ValueAsLong(rootAttributes, "startX"), ValueAsLong(rootAttributes, "startY"), WORLD_RANGE);
        const char* value = GetValueForAttribute(rootAttributes, "distFromStart");
        assertD(value);
        _distanceFromOriginalStart = FloatFromPersistent(value);
//        _toggleApproximationDirection = ValueAsBoolWithDefault(rootAttributes, "toggleApproximation", false);
    }
    _points.resize(_size);
    _startOffset = 0;
    _internalPositions.resize(_size);
    tSigned32 i;
    for(i = 0; i < _size; ++i)
    {
        const char *const* attributes = is.openElement("position");
        _points[i].cell = ValueAsLong(attributes, "cell");
        _points[i].x = ValueAsLong(attributes, "x");
        _points[i].y = ValueAsLong(attributes, "y");
        is.closeElement();
        _mesh->convertPosition(_points[i], _internalPositions[i]);
    }
    _connectionIndices = 0;
    const char* nextElement = is.peekNextElement();
    if(nextElement && strcmp(nextElement, "connectionIndex") == 0)
    {
        _connectionIndices = new std::vector<tSigned32>(_size - 1);
        for(i = 0; i + 1 < _size; ++i)
        {
            const char *const* attributes = is.openElement("connectionIndex");
            (*_connectionIndices)[i] = ValueAsLong(attributes, "value");
            is.closeElement();
        }
    }
    if(_size > 1)
    {
        _segmentInterpolator.initOrReinit(originalStart, _internalPositions[1].p);
    }
}

// interface

void cPath::destroy()
{
    if(_wrapper) {_wrapper->destroy_APIObjectWrapper();}
    _wrapper = 0;
    if(_mesh)
    {
        _mesh->detachPath(_heldNode);
        _heldNode.setInvalid();
        _mesh = 0;
    }
    else
    {
        assertD(!_heldNode.valid());
    }
    delete _connectionIndices;
    nSingletons::pathEngine().refPathPool().release(this);
}

tSigned32
cPath::size() const
{
    cMutexLock lock(_mutex);
    return _size;
}
cPosition
cPath::position(tSigned32 pointIndex) const
{
    cMutexLock lock(_mutex);
    return _points[_startOffset + pointIndex];
}
tSigned32
cPath::connectionIndex(tSigned32 segmentIndex) const
{
    cMutexLock lock(_mutex);
    if(_connectionIndices)
    {
        return (*_connectionIndices)[_startOffset + segmentIndex];
    }
    return -1;
}
const cPosition*
cPath::getPositionArray() const
{
    cMutexLock lock(_mutex);
    return &_points[_startOffset];
}    
const tSigned32*
cPath::getConnectionIndexArray() const
{
    cMutexLock lock(_mutex);
    if(!_connectionIndices)
    {
        return 0;
    }
    return &(*_connectionIndices)[_startOffset];
}    
tUnsigned32
cPath::getLength() const
{
    cMutexLock lock(_mutex);
    if(_size <= 1)
    {
        return 0;
    }
    return static_cast<tUnsigned32>(_length - _distanceFromOriginalStart);
}

iCollisionInfo*
cPath::advanceAlong(
        iShape* shape, float distance, iCollisionContext* context,
        float& precisionX, float& precisionY
        )
{
    assertD(_mesh);
    cMutexLock lock(_mutex);
    iCollisionInfo* info;
    tSigned32 points = size();
    assertD(points > 0);
    if(points < 2)
    {
        precisionX = 0.0f;
        precisionY = 0.0f;
        return 0;
    }

    if(connectionIndex(0) != -1)
    {
        pop_front();
        return 0;
    }

    bool reachesEnd;
    bool exactlyOnLine;
    tPoint point1, point2;
    float distanceLeft = getAdvanceInfo(distance, reachesEnd, exactlyOnLine, point1, point2);

    cPosition startPosition = position(0);

    while(reachesEnd)
    {
        cPosition p = position(1);
        tSigned32 cell;
        info = _mesh->firstCollision(shape, context, startPosition, p.x, p.y, cell);
        assertD(info || cell == p.cell);
        if(info)
        {
            return info;
        }

        pop_front();

        tSigned32 points = size();
        if(points < 2 || connectionIndex(0) != -1) // stop if the source endpoint for an off-mesh connection is reached
        {
            precisionX = 0.0f;
            precisionY = 0.0f;
            return 0;
        }

        distance = distanceLeft;

        getAdvanceInfo(distance, reachesEnd, exactlyOnLine, point1, point2);
        startPosition = position(0);
    }

//    if(!exactlyOnLine && _toggleApproximationDirection)
//    {
//        std::swap(point1, point2);
//    }

    tLine segmentLine(internalPosition(0).p, internalPosition(1).p);

    cPosition p;
    info = _mesh->firstCollision(shape, context, startPosition, point1.getX(), point1.getY(), p.cell);
    if(info == 0)
    {
        p.x = point1.getX();
        p.y = point1.getY();
        if(p.cell == -1 || // this condition added for graceful failure in the case the start of a path is inside obstructed space for external edges
            exactlyOnLine || !_mesh->testForSnagsToSegmentEnd(shape, context, segmentLine, p))
        {
            finaliseAdvance(p, distance, precisionX, precisionY);
            return 0;
        }
    }
    if(!exactlyOnLine)
    {
        delete info;
        info = _mesh->firstCollision(shape, context, startPosition, point2.getX(), point2.getY(), p.cell);
        if(info == 0)
        {
            p.x = point2.getX();
            p.y = point2.getY();
            if(p.cell == -1 || // this condition added for graceful failure in the case the start of a path is inside obstructed space for external edges
                !_mesh->testForSnagsToSegmentEnd(shape, context, segmentLine, p))
            {
//                _toggleApproximationDirection = !_toggleApproximationDirection;
                finaliseAdvance(p, distance, precisionX, precisionY);
                return 0;
            }
        }
    }

    if(exactlyOnLine)
    {
        assertD(info); // line from candidate to segment end was not checked in this case
        return info;
    }

  // collision may be due to approximation
  // so test collision for line direct to segment end
  // if this succeeds then this indicates a pathological sub-integer gap in the surrounding geometry
  // and in this case agent simply jumps directly to section end

    delete info;
    info = _mesh->firstCollision(shape, context, startPosition, position(1).x, position(1).y, p.cell);
    if(info)
    {
        return info;
    }
    pop_front();
    precisionX = 0.0f;
    precisionY = 0.0f;
    return 0;
}

void
cPath::renderOnGround(tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const
{
    cMutexLock lock(_mutex);
    assertD(_mesh);
    tSigned32 i;
    for(i = 0; i + 1 < size(); ++i)
    {
        _mesh->renderLineOnGround(position(i), position(i + 1).x, position(i + 1).y, originX, originY, callBack);
    }
}

//=========

void
cPath::getPrecisionXY(float& x, float& y)
{
    assertD(_size > 1);
//    tPoint p(_originalStart.x, _originalStart.y, WORLD_RANGE);
    const tPoint& p = _segmentInterpolator.refStart();
    tPoint axis = _internalPositions[_startOffset + 1].p - p;
    double fractionAlong = _distanceFromOriginalStart;
    fractionAlong /= Length_AsDouble(axis);
    double targetX = axis.getX() * fractionAlong + p.getX();
    x = static_cast<float>(targetX - _internalPositions[_startOffset].p.getX());
    double targetY = axis.getY() * fractionAlong + p.getY();
    y = static_cast<float>(targetY - _internalPositions[_startOffset].p.getY());
}

bool
cPath::operator==(cPath& rhs)
{
    if(size() != rhs.size())
    {
        return false;
    }
    if(size() == 0)
    {
        return true;
    }
    const cPosition* pointArray1 = getPositionArray();
    const cPosition* pointArray2 = rhs.getPositionArray();
    tSigned32 i;
    for(i = 0; i < size(); ++i)
    {
        if(*pointArray1 != *pointArray2)
        {
            return false;
        }
        ++pointArray1;
        ++pointArray2;
    }
    for(i = 0; i + 1 < size(); ++i)
    {
        if(connectionIndex(i) != rhs.connectionIndex(i))
        {
            return false;
        }
    }
    return true;
}

//void
//cPath::append(const cInternalPosition& p)
//{
//    assertD(size() > 0);
//    if(size() == 1)
//    {
//        _length = 0;
//        _originalStart = position(0);
//        _distanceFromOriginalStart = 0.f;
//        _isExactlyOnPath = true;
//        _toggleApproximationDirection = false;
//    }
//    _length += Length(p.p - _internalPositions.back().p);
//    _internalPositions.push_back(p);
//    _points.push_back(cPosition());
//    _points.back().x = p.p.getX();
//    _points.back().y = p.p.getY();
//    _points.back().cell = p.cell();
//    _size++;
////    _connectionIndices.push_back(-1);
//}

//void
//cPath::replaceEnd(tSigned32 replaceFromIndex, const std::list<std::pair<tFace, tPoint> >& pathToAppend)
//{
//    assertD(replaceFromIndex > 0);
//    assertD(pathToAppend.size() >= 2);
//    _size = _startOffset + replaceFromIndex + pathToAppend.size() - 1;
//    if(replaceFromIndex == 1)
//    {
//        // the start section will be affected
//        // so throw away original start info
//        _originalStart = position(0);
//        _distanceFromOriginalStart = 0.f;
//        _isExactlyOnPath = true;
//    }
//    _internalPositions.resize(_size);
//    _points.resize(_size);
//    std::list<std::pair<tFace, tPoint> >::const_iterator i;
//    i = pathToAppend.begin();
//    ++i;
//    for(; i != pathToAppend.end(); ++i)
//    {
//        _internalPositions[_startOffset + replaceFromIndex].f = i->first;
//        _internalPositions[_startOffset + replaceFromIndex].p = i->second;
//        const cInternalPosition& p = _internalPositions[_startOffset + replaceFromIndex];        
//        _points[_startOffset + replaceFromIndex].x = p.p.getX();
//        _points[_startOffset + replaceFromIndex].y = p.p.getY();
//        _points[_startOffset + replaceFromIndex].cell = p.cell();
//        ++replaceFromIndex;
//    }
//    {
//        tSigned32 i;
//        for(i = 0; i + 1 < _size; i++)
//        {
//            _length += Length(_internalPositions[_startOffset + i + 1].p - _internalPositions[_startOffset + i].p);
//        }
//    }
//}

//void
//cPath::replaceStart(tSigned32 replaceToIndex, const std::list<std::pair<tFace, tPoint> >& pathToPrepend)
//{
//    assertD(pathToPrepend.size() >= 2);
//    tSigned32 newSize = _size;
//    newSize -= replaceToIndex + 1;
//    newSize += pathToPrepend.size() - 1;
//    if(SizeL(pathToPrepend) <= _startOffset + replaceToIndex + 1)
//    {
//    // there is space to insert the new points at the start of the existing vectors
//        _startOffset = pathToPrepend.size() - replaceToIndex - 1;
//        std::list<std::pair<tFace, tPoint> >::const_iterator i;
//        i = pathToPrepend.begin();
//        tSigned32 j;
//        for(j = 0; j < SizeL(pathToPrepend); ++j)
//        {
//            _internalPositions[_startOffset + j].f = i->first;
//            _internalPositions[_startOffset + j].p = i->second;
//            const cInternalPosition& p = _internalPositions[_startOffset + j];
//            _points[_startOffset + j].x = p.p.getX();
//            _points[_startOffset + j].y = p.p.getY();
//            _points[_startOffset + j].cell = p.cell();
//            ++i;
//        }
//    }
//    else
//    {
//        std::vector<cInternalPosition> newInternalPositions(newSize);
//        std::vector<cPosition> newPoints(newSize);
//
//        std::list<std::pair<tFace, tPoint> >::const_iterator i;
//        i = pathToPrepend.begin();
//        tSigned32 j;
//        for(j = 0; j < SizeL(pathToPrepend); ++j)
//        {
//            newInternalPositions[j].f = i->first;
//            newInternalPositions[j].p = i->second;
//            const cInternalPosition& p = newInternalPositions[j];
//            newPoints[j].x = p.p.getX();
//            newPoints[j].y = p.p.getY();
//            newPoints[j].cell = p.cell();
//            ++i;
//        }
//        tSigned32 k;
//        for(k = replaceToIndex + 1; k < _size; ++k)
//        {
//            newInternalPositions[j] = _internalPositions[k];
//            newPoints[j] = _points[k];
//            ++j;
//        }
//        _startOffset = 0;
//        newInternalPositions.swap(_internalPositions);
//        newPoints.swap(_points);
//    }
//    _size = newSize;
//
//    _distanceFromOriginalStart = 0;
//    _originalStart = _points[_startOffset];
//    _isExactlyOnPath = true;
//
//    {
//        tSigned32 i;
//        for(i = 0; i + 1 < _size; i++)
//        {
//            _length += Length(_internalPositions[_startOffset + i + 1].p - _internalPositions[_startOffset + i].p);
//        }
//    }
//}

//tSigned32
//cPath::getIndexForEndUnwind(const tPoint& newEndPoint)
//{
//    assertD(size() >= 2);
//    tSigned32 result = size() - 2;
//    tSigned32 index = _startOffset + result;
//    while(result > 0)
//    {
//        tLine lineBeforeCorner(_internalPositions[index - 1].p, _internalPositions[index].p);
//        eSide side = SideOfLine(lineBeforeCorner, _internalPositions[index + 1].p);
//        assertD(side != SIDE_CENTRE);
//        if(SideOfLine(lineBeforeCorner, newEndPoint) == side)
//        {
//        // point still winds this corner
//            break;
//        }
//        --result;
//        --index;
//    }
//    return result;
//}
//
//tSigned32
//cPath::getIndexForStartUnwind(const tPoint& newStartPoint)
//{
//    assertD(size() >= 2);
//    tSigned32 result = 1;
//    tSigned32 index = _startOffset + result;
//    while(result < size() - 1)
//    {
//        tLine lineBeforeCorner(_internalPositions[index + 1].p, _internalPositions[index].p);
//        eSide side = SideOfLine(lineBeforeCorner, _internalPositions[index - 1].p);
//        assertD(side != SIDE_CENTRE);
//        if(SideOfLine(lineBeforeCorner, newStartPoint) == side)
//        {
//        // point still winds this corner
//            break;
//        }
//        ++result;
//        ++index;
//    }
//    return result;
//}

void
cPath::curved(
            cShape* shape,
            cCollisionContext* context,
            tSigned32 startVectorX, tSigned32 startVectorY,
            tSigned32 endVectorX, tSigned32 endVectorY,
            tSigned32 sectionLength,
            float turnRatio1, float turnRatio2,
            cPath* result
            )
{
    assertD(_mesh);
    vector<cInternalPosition> pathResult;
    vector<tSigned32>* connectionIndicesResult = 0;
    GenerateCurvedPath(
            _mesh, shape, context,
            _internalPositions, _connectionIndices, _startOffset,
            startVectorX, startVectorY,
            endVectorX, endVectorY,
            sectionLength, turnRatio1, turnRatio2,
            pathResult, connectionIndicesResult
            );
    result->init(pathResult, connectionIndicesResult);
}

void
cPath::convert(std::list<std::pair<tFace, tPoint> >& appendTo)
{
    tSigned32 i;
    for(i = 0; i < size(); i++)
    {
        const cInternalPosition& p = _internalPositions[_startOffset + i];
        appendTo.push_back(std::pair<tFace, tPoint>(p.f, p.p));
    }
}

void
cPath::save(iXMLOutputStream& os)
{
    os.addAttribute("size", size());
    if(size() > 1)
    {
        os.addAttribute("length", _length);
//        os.addAttribute("startCell", _originalStart.cell);
//        os.addAttribute("startX", _originalStart.x);
//        os.addAttribute("startY", _originalStart.y);
        os.addAttribute("startX", _segmentInterpolator.refStart().getX());
        os.addAttribute("startY", _segmentInterpolator.refStart().getY());
        std::string persistent = FloatToPersistent(_distanceFromOriginalStart);
        os.addAttribute("distFromStart", persistent.c_str());
//        if(_toggleApproximationDirection)
//        {
//            os.addAttribute("toggleApproximation", "true");
//        }
    }
    tSigned32 i;
    for(i = 0; i < size(); ++i)
    {
        cPosition p = position(i);
        os.openElement("position");
        os.addAttribute("cell", p.cell);
        os.addAttribute("x", p.x);
        os.addAttribute("y", p.y);
        os.closeElement("position");
    }
    if(_connectionIndices)
    {
        for(i = 0; i + 1 < size(); ++i)
        {
            os.openElement("connectionIndex");
            os.addAttribute("value", (*_connectionIndices)[i]);
            os.closeElement("connectionIndex");
        }
    }
}

tSigned32
cPath::majorVersion()
{
    return 2;
}
tSigned32
cPath::minorVersion()
{
    return 0;
}
