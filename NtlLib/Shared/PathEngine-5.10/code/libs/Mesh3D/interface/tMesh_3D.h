//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TMESH_3D_INCLUDED
#define TMESH_3D_INCLUDED

#include "libs/Mesh3D/interface/tMesh_3D_Header.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/Vector2F.h"
#include "libs/Geometry/interface/Vector3F.h"
#include "common/TemplateMesh/VectorMesh.h"
#include "platform_common/CoordinateRanges.h"

class cE3D
{
public:

    float _z;

    cE3D()
    {
        _z = 0.f;
    }
    cE3D(float z) :
     _z(z)
    {
    }

    void copyFrom(const cE3D& rhs)
    {
        _z = rhs._z;
    }

    bool operator==(const cE3D &rhs) const;
    bool operator!=(const cE3D &rhs) const
    {
        return !((*this) == rhs);
    }

    float getZ() const
    {
        return _z;
    }
};
class cV3D
{
public:

    tSigned32 _x, _y;
    
    cV3D()
    {
        _x = 0;
        _y = 0;
    }
    cV3D(tSigned32 x, tSigned32 y) :
        _x(x), _y(y)
    {
    }
    explicit cV3D(const tPoint& p) :
        _x(p.getX()), _y(p.getY())
    {
    }

    bool operator==(const cV3D &rhs) const;
    bool operator!=(const cV3D &rhs) const
    {
        return !((*this) == rhs);
    }

    tSigned32 getX() const
    {
        return _x;
    }
    tSigned32 getY() const
    {
        return _y;
    }
    tPoint getPoint() const
    {
        return tPoint(_x, _y, WORLD_RANGE);
    }
};

class cF3D
{
    bool _external;

public:

    float _heightAtOrigin;
    float _heightForX;
    float _heightForY;

    cF3D()
    {
        _heightAtOrigin = 0.0f;
        _heightForX = 0.0f;
        _heightForY = 0.0f;
        _external = false;
    }

    void copyFrom(const cF3D& rhs)
    {
        _external = rhs._external;
        _heightAtOrigin = rhs._heightAtOrigin;
        _heightForX = rhs._heightForX;
        _heightForY = rhs._heightForY;
    }

    void setPlaneEquation(float heightAtOrigin, float heightForX, float heightForY)
    {
        _heightAtOrigin = heightAtOrigin;
        _heightForX = heightForX;
        _heightForY = heightForY;
    }

    bool operator==(const cF3D &rhs) const;
    bool operator!=(const cF3D &rhs) const
    {
        return !((*this) == rhs);
    }

    bool isExternal() const
    {
        return _external;
    }

    float heightForF(const tPoint &p) const
    {
        return _heightAtOrigin
                + _heightForX * p.getX()
                + _heightForY * p.getY();
    }
    float heightForF(const cVector2F& p) const
    {
        return 
            _heightAtOrigin
            + _heightForX * p.x
            + _heightForY * p.y;
    }
    float heightForF(float x, float y) const
    {
        return 
            _heightAtOrigin
            + _heightForX * x
            + _heightForY * y;
    }

    void getNormal(float& x, float& y, float& z) const;

    static cF3D externalFace()
    {
        cF3D result;
        result._external = true;
        return result;
    }
};

class cF3D_FaceTracked : public cF3D
{
public:

    tSigned32 _originalIndex;

    cF3D_FaceTracked()
    {
        _originalIndex = -1;
    }
    cF3D_FaceTracked(const cF3D& rhs)
    {
//        static_cast<cF3D>(*this) = rhs;         //...  why doesnt this work as expected?
        cF3D::copyFrom(rhs);
        _originalIndex = -1;
    }
    
    bool operator==(const cF3D_FaceTracked &rhs) const
    {
        return _originalIndex == rhs._originalIndex && cF3D::operator==(rhs);
    }
    bool operator!=(const cF3D_FaceTracked &rhs) const
    {
        return !((*this) == rhs);
    }
};

inline tPoint GetVertexPoint(const tMesh_3D::edge e)
{
    return e.vertex()->getPoint();
}
inline tPoint3 GetVertexPoint3(const tMesh_3D::edge e)
{
    return tPoint3(e.vertex()->_x, e.vertex()->_y, static_cast<tSigned32>(e->_z), WORLD_RANGE);
}
inline cVector3F GetVertexPoint3F(const tMesh_3D::edge e)
{
    return cVector3F(
            static_cast<float>(e.vertex()->_x),
            static_cast<float>(e.vertex()->_y),
            e->_z
            );
}
inline tPoint GetVertexPoint(const tMesh_3D::vertex v)
{
    return v->getPoint();
}
inline tLine GetEdgeLine(tMesh_3D::edge e)
{
    return tLine(GetVertexPoint(e),GetVertexPoint(e.next()));
}
inline tPoint GetEdgeAxis(tMesh_3D::edge e)
{
    return GetVertexPoint(e.next())-GetVertexPoint(e);
}
inline tPoint3 GetEdgeAxis3(tMesh_3D::edge e)
{
    return GetVertexPoint3(e.next())-GetVertexPoint3(e);
}

inline tPoint GetVertexPoint(const tMesh_3D_FaceTracked::edge e)
{
    return e.vertex()->getPoint();
}
inline tPoint GetVertexPoint(const tMesh_3D_FaceTracked::vertex v)
{
    return v->getPoint();
}
inline tLine GetEdgeLine(tMesh_3D_FaceTracked::edge e)
{
    return tLine(GetVertexPoint(e),GetVertexPoint(e.next()));
}
inline tPoint GetEdgeAxis(tMesh_3D_FaceTracked::edge e)
{
    return GetVertexPoint(e.next())-GetVertexPoint(e);
}


#endif // TMESH_INCLUDED
