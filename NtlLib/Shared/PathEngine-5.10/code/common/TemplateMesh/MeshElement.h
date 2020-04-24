//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_ELEMENT_INCLUDED
#define MESH_ELEMENT_INCLUDED

#include "common/interface/Assert.h"
#include "common/TemplateMesh/External.h"

class bMeshElement
{
public:

    enum eType
    {
        FACE,
        EDGE,
        VERTEX,
        UNSET // for internal use only
    };
};

template <class T>
class cMeshElement : public bMeshElement
{
public:
    
    typedef cMeshElement<T> tThis;
    typedef T tMesh;
    typedef typename T::edge tEdge;
    typedef typename T::vertex tVertex;
    typedef typename T::face tFace;

private:

    union
    {
        tEdge _edge;
        tVertex _vertex;
        tFace _face;
    } _it;
    eType _type;

public:

// allow construction uninitialised
    cMeshElement()
    {
    }

// straightforward construct as element
    cMeshElement(tFace face)
    {
        _it._face = face;
        _type = FACE;
    }
    cMeshElement(tEdge edge)
    {
        _it._edge = edge;
        _type = EDGE;
    }
    cMeshElement(tVertex vertex)
    {
        _it._vertex = vertex;
        _type = VERTEX;
    }

    void setInvalid() {_type = UNSET;}
    void clear() {_type = UNSET;}
    bool valid() const {return _type != UNSET;}

    eType type() const
    {
        assertD(_type != UNSET);
        return _type;
    }

    bool operator==(const cMeshElement<T> &rhs) const
    {
        if(type() != rhs.type())
        {
            return false;
        }
        switch(type())
        {
        default:
            invalid();
        case UNSET:
            return true;
        case FACE:
            return _it._face == rhs._it._face;
        case EDGE:
            return _it._edge == rhs._it._edge;
        case VERTEX:
            return _it._vertex == rhs._it._vertex;
        }
    }
    bool operator!=(const cMeshElement<T> &rhs) const
    {
        return !(*this==rhs);
    }

    bool isVertex() const
    {
        return _type == VERTEX;
    }
    bool isEdge() const
    {
        return _type == EDGE;
    }
    bool isFace() const
    {
        return _type == FACE;
    }

    bool isExternalVertex() const
    {
        return type() == VERTEX && VertexIsExternal(tMesh::firstEdge(_it._vertex));
    }
    bool isExternalEdge() const
    {
        return type() == EDGE && (_it._edge.face()->isExternal() || _it._edge.twin().face()->isExternal());
    }
    bool isExternalFace() const
    {
        return type() == FACE && _it._face->isExternal();
    }


    tFace asFace() const
    {
        assertD(_type == FACE);
        return _it._face;
    }
    tEdge asEdge() const
    {
        assertD(_type == EDGE);
        return _it._edge;
    }
    tVertex asVertex() const
    {
        assertD(_type == VERTEX);
        return _it._vertex;
    }

    tEdge asEdgeInFace(tFace f) const
    {
        assertD(_type == EDGE);
        if(_it._edge.face() == f)
        {
            return _it._edge;
        }
        assertD(_it._edge.twin().face() == f);
        return _it._edge.twin();
    }
    tEdge asEdgeFromVertex(tVertex v) const
    {
        assertD(_type == EDGE);
        if(_it._edge.vertex() == v)
        {
            return _it._edge;
        }
        assertD(_it._edge.twin().vertex() == v);
        return _it._edge.twin();
    }
    tEdge asEdgeFromVertex(const tThis& v) const
    {
        return asEdgeFromVertex(v.asVertex());
    }

    tFace getOverlappingFace() const
    {
        switch(type())
        {
        default:
            invalid();
        case FACE:
            return _it._face;
        case EDGE:
            if(_it._edge.face()->isExternal())
            {
                return _it._edge.twin().face();
            }
            return _it._edge.face();            
        case VERTEX:
            tEdge e = tMesh::firstEdge(_it._vertex);
            if(e.face()->isExternal())
            {
                return e.twin().face();
            }
            return e.face();
        }
    }

    bool isPositionallyEquivalent(const cMeshElement<T>& rhs) const
    {
        if(type() != rhs.type())
            return false;
        switch(type())
        {
        default:
            invalid();
        case FACE:
            return _it._face == rhs._it._face;
        case EDGE:
            return _it._edge == rhs._it._edge || (_it._edge.twin().valid() && _it._edge.twin() == rhs._it._edge);
        case VERTEX:
            return _it._vertex == rhs._it._vertex;
        }
    }
};


#endif
