//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "VertexPair.h"

class iFaceVertexMesh;

class cFaceVertexEdge
{
public:

    cFaceVertexEdge()
    {
        _face = -1;
    }
    cFaceVertexEdge(tSigned32 face, tSigned32 indexInFace) :
     _face(face),
     _indexInFace(indexInFace)
    {
        assertD(_face >= 0);
        assertD(_indexInFace >= 0 && _indexInFace < 3);
    }

    const cFaceVertexEdge& operator=(const cFaceVertexEdge& rhs)
    {
        _face = rhs._face;
        _indexInFace = rhs._indexInFace;
        return *this;
    }

    bool operator==(const cFaceVertexEdge& rhs) const
    {
        return _face == rhs._face && _indexInFace == rhs._indexInFace;
    }
    bool operator!=(const cFaceVertexEdge& rhs) const
    {
        return !(*this == rhs);
    }

    void clear()
    {
        _face = -1;
    }
    bool valid() const
    {
        return _face != -1;
    }

    cFaceVertexEdge next() const
    {
        if(_indexInFace == 2)
        {
            return cFaceVertexEdge(_face, 0);
        }
        return cFaceVertexEdge(_face, _indexInFace + 1);
    }
    cFaceVertexEdge prev() const
    {
        if(_indexInFace == 0)
        {
            return cFaceVertexEdge(_face, 2);
        }
        return cFaceVertexEdge(_face, _indexInFace - 1);
    }

    cFaceVertexEdge operator++(int)        // postfix
    {
        cFaceVertexEdge temporary = *this;
        ++*this;
        return temporary;
    }
    cFaceVertexEdge& operator++()            // prefix
    {
        ++_indexInFace;
        if(_indexInFace == 3)
        {
            _indexInFace = 0;
        }
        return *this;
    }
    cFaceVertexEdge operator--(int)        // postfix
    {
        cFaceVertexEdge temporary = *this;
        --*this;
        return temporary;
    }
    cFaceVertexEdge& operator--()            // prefix
    {
        ++_indexInFace;
        if(_indexInFace == -1)
        {
            _indexInFace = 2;
        }
        return *this;
    }

    tSigned32 vertexIndex(const iFaceVertexMesh& mesh) const;

    cVertexPair vp(const iFaceVertexMesh& mesh) const
    {
        return cVertexPair(vertexIndex(mesh), next().vertexIndex(mesh));
    }

    tSigned32 index() const
    {
        return _face * 3 + _indexInFace;
    }
    bool ownsVP(const iFaceVertexMesh& mesh) const
    {
        return vp(mesh)._v1 == vertexIndex(mesh);
    }

    tSigned32 _face, _indexInFace;
};

