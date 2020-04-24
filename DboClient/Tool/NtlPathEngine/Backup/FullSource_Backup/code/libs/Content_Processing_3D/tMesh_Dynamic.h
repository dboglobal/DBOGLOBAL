//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef T_MESH_DYNAMIC_INCLUDED
#define T_MESH_DYNAMIC_INCLUDED

#include "DynamicPoint3.h"
#include "tMesh_Dynamic_Header.h"
#include "BSPVertices.h"
#include "common/TemplateMesh/VectorMesh.h"
#include "common/interface/Assert.h"

class cEDynamic
{
public:

    union
    {
        tSigned32 _pairIndex;
        tSigned32 _originalIndex;
        tSigned32 _approximateZ;
    };

    cEDynamic()
    {
    }
    cEDynamic(const cEDynamic& rhs)
    {
        _pairIndex = rhs._pairIndex;
    }
};
class cVDynamic
{
    tSigned32 _pointIndex;

public:

    cVDynamic()
    {
    }

    void
    initForOriginalPoint(cBSPVertices& vertices, const tPoint3& originalPoint)
    {
        _pointIndex = vertices.addOriginal(originalPoint);
    }
    void
    initForSplitPoint(cBSPVertices& vertices, const cDynamicPoint3& splitPoint)
    {
        _pointIndex = vertices.addDynamic(splitPoint);
    }

    bool
    isOriginalVertex(const cBSPVertices& vertices) const
    {
        assertD(_pointIndex >= 0);
        return vertices.isOriginal(_pointIndex);
    }
    const tPoint3&
    originalPoint(const cBSPVertices& vertices) const
    {
        assertD(_pointIndex >= 0);
        return vertices.lookupOriginal(_pointIndex);
    }
    const cDynamicPoint3&
    dynamicPoint(const cBSPVertices& vertices) const
    {
        assertD(_pointIndex >= 0);
        return vertices.lookupDynamic(_pointIndex);
    }
    tPoint
    approximatedPoint2D(const cBSPVertices& vertices) const
    {
        assertD(_pointIndex >= 0);
        return vertices.lookupApproximated2D(_pointIndex);
    }

//    cVector3F getPoint3F(const cBSPVertices& vertices) const;
    tPoint3 approximatedPoint3(const cBSPVertices& vertices, tSigned32 range) const;
};

class cFDynamic
{
  // boolean specific
    tSigned32 _splitNode;
    //.. the following is here only for visualisation
    bool _discarded; // can be replaced by _splitNode == -1, but _splitNode is currently retained for visualisation

public:

  // boolean specific
    tSigned32 _splitterIndex;
    //.. the following is here only for visualisation
    tSigned32 _operandIndex;
//    tSigned32 _clippedOperand; //.. lose this and look up from _splitterIndex instead

    cFDynamic()
    {
    }
    cFDynamic(const cFDynamic& rhs)
    {
        _splitNode = rhs._splitNode;
        _discarded = rhs._discarded;
        _splitterIndex = rhs._splitterIndex;
        _operandIndex = rhs._operandIndex;
//        _clippedOperand = rhs._clippedOperand;
    }

    void
    initAsUnassigned(tSigned32 splitterIndex, tSigned32 operandIndex)
    {
        _splitNode = -1;
        _discarded = false;
        _splitterIndex = splitterIndex;
        _operandIndex = operandIndex;
    }

    void
    initAsInternalFace(tSigned32 splitNode, bool facesOppositeDirection)
    {
        assertD(splitNode >= 0);
        if(facesOppositeDirection)
        {
            _splitNode = -2 - splitNode;
        }
        else
        {
            _splitNode = splitNode;
        }
        _discarded = true;
        _splitterIndex = -1;
        _operandIndex = -1;
    }

    void assignSplitNode(tSigned32 splitNode, bool facesOppositeDirection)
    {
        assertD(splitNode >= 0);
        assertD(_splitNode == -1);
        assertD(!_discarded);
        if(facesOppositeDirection)
        {
            _splitNode = -2 - splitNode;
        }
        else
        {
            _splitNode = splitNode;
        }
    }

    void discard()
    {
        assertD(!_discarded);
        assertD(_splitNode != -1);
        _discarded = true;
    }

    bool isUnassigned() const
    {
        return _splitNode == -1;
    }
    bool wasDiscarded() const // to be called after initial tree construction
    {
        return isUnassigned() || _discarded;
    }

    tSigned32 getSplitNode(bool& facesOppositeDirection) const
    {
        if(_splitNode >= -1)
        {
            facesOppositeDirection = false;
            return _splitNode;
        }
        facesOppositeDirection = true;
        return -2 - _splitNode;
    }

    bool isExternal() const
    {
        return false;
    }
};

cVector3F GetVertexPoint3F(const cBSPVertices& vertices, const tEdge_Dynamic e);
cDynamicPoint3 GetEdgeAxis3(const cBSPVertices& vertices, tEdge_Dynamic e);

#endif
