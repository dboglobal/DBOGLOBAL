//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "i_pathengine.h"
#include "libs/PathEngine_Interface/interface/PathSegmentInterpolator.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "common/Containers/ReplacementList.h"
#include "common/STL/vector.h"
#include "common/STL/list.h"
#include "common/STL/utility.h"
#include "platform_common/Mutex.h"

class cShape;
class cMesh;
class cCollisionContext;
class iCollisionInfo;
template <class T> class cPool;
class cMutex;
class iXMLOutputStream;
class iXMLInputStream;
class iAPIObjectWrapper;

class cPath : public iPath
{
    cMutex _mutex; // applied on external interface
    cMesh* _mesh;
    cList_NoSize<cPath*>::iterator _heldNode;

    tSigned32 _size;
    tSigned32 _startOffset;
    std::vector<cInternalPosition> _internalPositions;
    std::vector<cPosition> _points;
    std::vector<tSigned32>* _connectionIndices;

  // these are only kept valid when _size > 1
    tSigned32 _length;
//    cPosition _originalStart;
    cPathSegmentInterpolator _segmentInterpolator;
    float _distanceFromOriginalStart;

//    bool _toggleApproximationDirection;

    void init_common();

  // support for path pool

    tSigned32 _poolIndex;
    friend class cPool<cPath>;
    void setPoolIndex(tSigned32 value)
    {
        _poolIndex = value;
    }
    tSigned32 getPoolIndex() const
    {
        return _poolIndex;
    }

    float getAdvanceInfo(float distance,
                         bool& reachesNextPoint,
                         bool& exactlyOnLine,
                         tPoint& point1, tPoint& point2
                         );
    void pop_front();
    void advancePoint0To(const cInternalPosition& p, float distanceAdvanced);

    void
    finaliseAdvance(
            const cInternalPosition& newPosition,
            float distance,
            float& precisionX, float& precisionY
            );
    void
    finaliseAdvance(
            const cPosition& newPosition,
            float distance,
            float& precisionX, float& precisionY
            );

public:

    iAPIObjectWrapper* _wrapper;

    cPath();
    ~cPath();

// initialisation (or reinitialisation)

    void attachToMesh(cMesh* mesh, cList_NoSize<cPath*>::iterator heldNode)
    {
        assertD(_mesh == 0);
        _mesh = mesh;
        assertD(_heldNode.isInvalid());
        _heldNode = heldNode;
    }

    void init(const cInternalPosition& singlePoint);

    // the vector is swapped in by these initialisers
    void init(std::vector<cInternalPosition>& points, std::vector<tSigned32>* connectionIndices);

    void init(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathSize);
    void init_Reversed(const cPosition* positionsBuffer, const tSigned32* connectionIndicesBuffer, tSigned32 pathSize);

    void load(iXMLInputStream& is, const char *const* rootAttributes);

// iPath

    void destroy();
    tSigned32 size() const;
    cPosition position(tSigned32 pointIndex) const;
    tSigned32 connectionIndex(tSigned32 segmentIndex) const;
    const cPosition* getPositionArray() const;
    const tSigned32* getConnectionIndexArray() const;
    tUnsigned32 getLength() const;
    iCollisionInfo* advanceAlong(iShape* shape, float distance, iCollisionContext* context, float& precisionX, float& precisionY);
    void renderOnGround(tSigned32 originX, tSigned32 originY, iRender3DLinesCallBack& callBack) const;
    void operator delete(void* p) {::operator delete(p);}

//=========

    bool operator==(cPath& rhs);
    bool operator!=(cPath& rhs)
    {
        return !((*this) == rhs);
    }

//    void append(const cInternalPosition& p);
//    void replaceEnd(tSigned32 replaceFromIndex, const std::vector<cInternalPosition>& pathToAppend, tUnsigned32 pathToAppendLength);
//    void replaceEnd(tSigned32 replaceFromIndex, const std::list<std::pair<tFace, tPoint> >& pathToAppend);
//    void replaceStart(tSigned32 replaceToIndex, const std::list<std::pair<tFace, tPoint> >& pathToPrepend);

    cInternalPosition internalPosition(tSigned32 pointindex)
    {
        return _internalPositions[_startOffset+pointindex];
    }

//    tSigned32 getIndexForEndUnwind(const tPoint& newEndPoint);
//    tSigned32 getIndexForStartUnwind(const tPoint& newStartPoint);

    void curved(
            cShape* shape,
            cCollisionContext* context,
            tSigned32 startVectorX, tSigned32 startVectorY,
            tSigned32 endVectorX, tSigned32 endVectorY,
            tSigned32 sectionLength,
            float turnRatio1, float turnRatio2,
            cPath* result
            );

    void getPrecisionXY(float& x, float& y);

    void convert(std::list<std::pair<tFace, tPoint> >& appendTo);

    void save(iXMLOutputStream& os);

    static tSigned32 majorVersion();
    static tSigned32 minorVersion();
};

inline cPath* FromI(iPath* ptr)
{
    return static_cast<cPath*>(ptr);
}

inline void Destroy(cPath* ptr)
{
    ptr->destroy();
}
