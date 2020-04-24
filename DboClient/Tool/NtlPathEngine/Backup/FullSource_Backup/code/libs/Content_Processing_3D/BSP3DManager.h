//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_3D_OPERAND_MANAGER_INCLUDED
#define BSP_3D_OPERAND_MANAGER_INCLUDED

#include "libs/PathEngine_Interface/RandomSequence.h" //.... move to a lower level module
#include "Plane.h"
#include "tMesh_Dynamic.h"
#include "tMesh_Simple3D_Header.h"
#include "BSPStats.h"
#include "common/STL/vector.h"
#include "common/Containers/BitVector.h"

class cBSP3DOperand;
class iProgressCallBack;
template <class T> class cBSPTree;

class cBSP3DManager
{
    cRandomSequence _sequence;
    tSigned32 _nextEdgePair;
    tSigned32 _bestSplit;
    tSigned32 _bestSplitCost;
    cBitVector _bestSplitPreprocess;
    cBitVector _candidateSplitPreprocess;
    tSigned32 _nextClippedOperand;
    tSigned32 _totalUnassignedBounds;
    tSigned32 _totalStartingUnassignedBounds;
    tSigned32 _bestUnassignedBounds;
    iProgressCallBack* _progressCallBack;
    bool _keepDiscardedFaces;
    bool _keepTree;
    std::vector<cBSP3DOperand*> _initialOperands;
    cBSPTree<cBSP3DManager>* _tree;

    void
    extractFacesFromContainingOperand(cBSP3DOperand* operand);
    void
    extractFacesFromNonContainingOperand(cBSP3DOperand* operand);

    void
    considerSplit(
            std::vector<cBSP3DOperand*>& operands,
            tSigned32 splitterIndex
            );

    tSigned32
    doBestSplit(
            std::vector<cBSP3DOperand*>& operands,
            tSigned32 splitNode,
            std::vector<cBSP3DOperand*>& backOperands,
            bool& frontContained, bool& backContained
            );

    // prevent copying and assignment
    cBSP3DManager(const cBSP3DManager&);
    const cBSP3DManager& operator=(const cBSP3DManager&);

public:

    std::vector<cPlane> _splitters;
    cBSPVertices _vertices;
    tMesh_Dynamic _result;
    cBSPStats _stats;

    cBSP3DManager(
            iProgressCallBack* progressCallBack,
            bool keepDiscardedFaces,
            bool keepTree = false
            );
    ~cBSP3DManager();

    void addOperand(tMesh_Simple3D& primitive);
    void buildTreeAndSplitFragments();

    const cBSPTree<cBSP3DManager>& refTree() const
    {
        assertD(_keepTree);
        assertD(_tree);
        return *_tree;
    }

// ---- interface for cBSPTree

    typedef cBSP3DOperand tClippedOperand;
    typedef cPlane tSplitter;

    void
    splitOperands(
            std::vector<tClippedOperand*>& operands,
            tSigned32 splitNode, 
            std::vector<tClippedOperand*>& backOperands,
            tSigned32& splitterSelected, 
            bool& frontContained, bool& backContained
            );

    const tSplitter&
    refSplitter(tSigned32 index) const
    {
        return _splitters[index];
    }

    void
    splitFragment(tFace_Dynamic toSplit, tSigned32 splitterIndex, bool& extendsInFront, tFace_Dynamic& inFront, bool& extendsBehind, tFace_Dynamic& behind);
    
    void
    fragmentIsInside(tFace_Dynamic fragment);
};

#endif
