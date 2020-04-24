//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_3D_OPERAND_INCLUDED
#define BSP_3D_OPERAND_INCLUDED

#include "tMesh_Dynamic.h"
#include "tMesh_Simple3D_Header.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "common/STL/vector.h"

class cPlane;
class cVerticalPlane;
class cBitVector;

class cBSP3DOperand
{
    tMesh_Dynamic _mesh;
    tSigned32 _numberOfUnassignedBounds;

    bool validate(const cBSPVertices& vertices) const;

    void
    markCoplanarBound(
            const cBitVector& sidesBuffer, tSigned32 sidesStart,
            tSigned32 splitNode, bool facesOppositeDirection
            );

    cBSP3DOperand(tMesh_Dynamic& swapIn);

    // prevent copying and assignment
    cBSP3DOperand(const cBSP3DOperand&);
    const cBSP3DOperand& operator=(const cBSP3DOperand&);

public:

    cBSP3DOperand(
            cBSPVertices& vertices,
            tMesh_Simple3D& meshSimple3D,
            tSigned32 operandIndex, std::vector<cPlane>& splitters
            );

    bool
    vertsAreConsolidated() const
    {
        return _mesh.vertices() == _mesh.vertSize();
    }
    tVertex_Dynamic
    beginVertex()
    {
        return _mesh.beginVertex();
    }

// interface for cBSP3DManager

    tSigned32
    numberOfVertices() const
    {
        return _mesh.vertices();
    }

    bool
    predict_WillBeSplit(
            const cBitVector& preprocessBuffer,
            tSigned32 bufferPosition
            );

    void
    predict_UnassignedBoundsSplit(
            const cBitVector& preprocessBuffer,
            tSigned32 bufferPosition,
            tSigned32& unassignedInFront, tSigned32& unassignedBehind,
            bool& containsFront, bool& containsBack
            );

    void
    split(
            cBSPVertices& vertices,
            const cPlane& splitter,
            const cBitVector& preprocessBuffer,
            tSigned32 bufferPosition,
            tSigned32 splitNode, 
            cBSP3DOperand*& front, cBSP3DOperand*& back,
            tSigned32& nextEdgePair
            );

    void
    takeMeshAndInvalidate(tMesh_Dynamic& swapOutTo);

    tSigned32
    numberOfUnassignedBounds() const
    {
        return _numberOfUnassignedBounds;
    }
    tSigned32
    splitForUnassignedBoundIndex(tSigned32 index) const;
};

#endif
