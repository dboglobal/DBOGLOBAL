//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BSP3DManager.h"
#include "BSP3DOperand.h"
#include "BSPTree.h"
#include "SideOfPlane.h"
#include "SidesForSplit.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "common/Containers/SwapOut.h"
#include "i_pathengine.h"

static tSigned32
CountUnassignedBounds(const std::vector<cBSP3DOperand*>& operands)
{
    tSigned32 result = 0;
    tSigned32 i;
    for(i = 0; i != SizeL(operands); ++i)
    {
        result += operands[i]->numberOfUnassignedBounds();
    }
    return result;
}
static tSigned32
SplitForUnassignedBoundIndex(const std::vector<cBSP3DOperand*>& operands, tSigned32 boundIndex)
{
    tSigned32 i;
    for(i = 0; i != SizeL(operands); ++i)
    {
        tSigned32 bounds = operands[i]->numberOfUnassignedBounds();
        if(boundIndex < bounds)
        {
            return operands[i]->splitForUnassignedBoundIndex(boundIndex);
        }
        boundIndex -= bounds;
    }
    invalid();
    return 0;
}

static void
SendFragmentsDownFrontSubTree(
        cBSP3DManager& manager,
        const cBSPTree<cBSP3DManager>& tree,
        iProgressCallBack* progressCallBack
        )
{
    // we loop down through face indices, to ensure that fragments added by splitting are not reprocessed
    manager._result.consolidateFaces();
    tSigned32 i = manager._result.faces();
    tSigned32 facesToProcess = i;
    while(i)
    {
        --i;
        tFace_Dynamic toSplit = manager._result.faceAt(i);
        if(toSplit->wasDiscarded())
        {
            continue;
        }
        if(progressCallBack && (i & 7) == 0)
        {
            progressCallBack->updateProgress("Splitting elements", static_cast<float>(facesToProcess - i) / static_cast<float>(facesToProcess));
        }
        bool facesOppositeDirection;
        tSigned32 node = toSplit->getSplitNode(facesOppositeDirection);;
        if(facesOppositeDirection)
        {
            tree.splitFragmentByBackSubTree(node, toSplit);
        }
        else
        {
            tree.splitFragmentByFrontSubTree(node, toSplit);
        }
    }
}

void
cBSP3DManager::extractFacesFromContainingOperand(cBSP3DOperand* operand)
{
    tMesh_Dynamic operandMesh;
    operand->takeMeshAndInvalidate(operandMesh);
    tFace_Dynamic f = operandMesh.beginFace();
    do
    {
        tFace_Dynamic fToProcess = f;
        ++f;
//        fToProcess->_clippedOperand = _nextClippedOperand;
        if(fToProcess->wasDiscarded())
        {
            if(_keepDiscardedFaces)
            {
                tEdge_Dynamic e = tMesh_Dynamic::beginEdge(fToProcess);
                do
                {
                    if(e.twin().valid())
                    {
                        operandMesh.breakEdge(e);
                    }
                }
                while(e.toNextInFace());
            } 
            else
            {
                operandMesh.eraseFace(fToProcess);
            }
        }
    }
    while(f.valid());
    operandMesh.consolidate();
    _nextClippedOperand++;
    _result.append(operandMesh);
}

void
cBSP3DManager::extractFacesFromNonContainingOperand(cBSP3DOperand* operand)
{
    assertD(_keepDiscardedFaces);
    tMesh_Dynamic operandMesh;
    operand->takeMeshAndInvalidate(operandMesh);
    tFace_Dynamic f = operandMesh.beginFace();
    do
    {
        tFace_Dynamic fToProcess = f;
        ++f;
//        fToProcess->_clippedOperand = _nextClippedOperand;
        if(!fToProcess->wasDiscarded())
        {
            fToProcess->discard();
        }
    }
    while(f.valid());
    operandMesh.consolidate();
    _nextClippedOperand++;
    _result.append(operandMesh);
}

void
cBSP3DManager::considerSplit(
        std::vector<cBSP3DOperand*>& operands,
        tSigned32 splitterIndex
        )
{
    PROFILER_SCOPE("cBSP3DManager::considerSplit()");
    assertD(splitterIndex >= 0);
    assertD(splitterIndex != _bestSplit); // calling code needs to ensure that the same split is not considered twice

    const cPlane& splitter = _splitters[splitterIndex];
    _candidateSplitPreprocess.resize(0);
    SidesForSplit(_vertices, operands, _splitters[splitterIndex], _candidateSplitPreprocess);

    tSigned32 preprocessPosition = 0;
    tSigned32 frontUnassigned = 0;
    tSigned32 backUnassigned = 0;
//    tSigned32 totalCost = 0;
    bool frontIsContained = false;
    bool backIsContained = false;
    for(tSigned32 i = 0; i != SizeL(operands); i++)
    {
        tSigned32 frontUnassignedInOperand, backUnassignedInOperand;
        bool frontIsContainedByOperand, backIsContainedByOperand;
        operands[i]->predict_UnassignedBoundsSplit(_candidateSplitPreprocess, preprocessPosition, frontUnassignedInOperand, backUnassignedInOperand, frontIsContainedByOperand, backIsContainedByOperand);
        preprocessPosition += operands[i]->numberOfVertices() * 2;
        frontUnassigned += frontUnassignedInOperand;
        backUnassigned += backUnassignedInOperand;
        if(frontIsContainedByOperand)
        {
            frontIsContained = true;
        }
        if(backIsContainedByOperand)
        {
            backIsContained = true;
        }
//        totalCost += unassignedInFront;
//        totalCost += unassignedBehind;
//        if(_bestSplit != -1 && totalCost >= _bestSplitCost)
//        {
//            return;
//        }
    }

    tSigned32 totalCost = 0;
    if(!frontIsContained)
    {
        totalCost += frontUnassigned;
    }
    if(!backIsContained)
    {
        totalCost += backUnassigned;
    }

    if(_bestSplit != -1 && totalCost >= _bestSplitCost)
//    if(_bestSplit != -1 && totalCost <= _bestSplitCost)//.......... choose worst split
    {
        return;
    }

    _candidateSplitPreprocess.swap(_bestSplitPreprocess);
    _bestSplit = splitterIndex;
    _bestSplitCost = totalCost;
}

tSigned32
cBSP3DManager::doBestSplit(
        std::vector<cBSP3DOperand*>& operands,
        tSigned32 splitNode,
        std::vector<cBSP3DOperand*>& backOperands,
        bool& frontContained, bool& backContained
        )
{
    assertD(backOperands.empty());
    assertD(_bestSplit != -1);
    tSigned32 splitterSelected = _bestSplit;
    _bestSplit = -1;

    frontContained = false;
    backContained = false;
    tSigned32 operandContainingFront;
    tSigned32 operandContainingBack;

#ifdef ASSERTIONS_ON
    tSigned32 unassignedBoundsElsewhere = _totalUnassignedBounds;
#endif

    tSigned32 bufferPosition = 0;
    tSigned32 i;
    for(i = 0; i != SizeL(operands); i++)
    {
        _totalUnassignedBounds -= operands[i]->numberOfUnassignedBounds();
#ifdef ASSERTIONS_ON
        unassignedBoundsElsewhere -= operands[i]->numberOfUnassignedBounds();
#endif
        tClippedOperand *front;
        tClippedOperand *back;
        tSigned32 nextBufferPosition = bufferPosition + operands[i]->numberOfVertices() * 2;
        operands[i]->split(
                _vertices,
                _splitters[splitterSelected],
                _bestSplitPreprocess, bufferPosition,
                splitNode, front, back, _nextEdgePair
                );
        bufferPosition = nextBufferPosition;
        if(front && back)
        {
            _stats._operandSplits++;
        }
        if(back)
        {
            _totalUnassignedBounds += back->numberOfUnassignedBounds();
            if(back->numberOfUnassignedBounds() == 0)
            {
                backContained = true;
                operandContainingBack = SizeL(backOperands);
                //... optimise - dont generate both halves for further splits
                //... optimise - flag this clipped operand during considerSplit() and process it first
            }
            backOperands.push_back(back);
        }
        if(front)
        {
            _totalUnassignedBounds += front->numberOfUnassignedBounds();
        }
        operands[i] = front;
    }
    for(i = 0; i != SizeL(operands);)
    {
        if(operands[i])
        {
            if(operands[i]->numberOfUnassignedBounds() == 0)
            {
                frontContained = true;
                operandContainingFront = i;
                //... optimise - dont generate both halves for further splits
                //... optimise - flag this clipped operand during considerSplit() and process it first
            }
            i++;
        }
        else
        {
            SwapOut(operands, i);
        }
    }

    std::vector<tClippedOperand*>& frontOperands = operands;

    if(backContained)
    {
        for(i = 0; i != SizeL(backOperands); ++i)
        {
            _totalUnassignedBounds -= backOperands[i]->numberOfUnassignedBounds();
            if(i == operandContainingBack)
            {
                extractFacesFromContainingOperand(backOperands[i]);
            }
            else
            if(_keepDiscardedFaces)
            {
                extractFacesFromNonContainingOperand(backOperands[i]);
            }
            delete backOperands[i];
        }
        backOperands.clear();
    }
    if(frontContained)
    {
        for(i = 0; i != SizeL(frontOperands); ++i)
        {
            _totalUnassignedBounds -= frontOperands[i]->numberOfUnassignedBounds();
            if(i == operandContainingFront)
            {
                extractFacesFromContainingOperand(frontOperands[i]);
            }
            else
            if(_keepDiscardedFaces)
            {
                extractFacesFromNonContainingOperand(frontOperands[i]);
            }
            delete frontOperands[i];
        }
        frontOperands.clear();
    }

    assertD(unassignedBoundsElsewhere + _bestSplitCost == _totalUnassignedBounds); //... only in the case of unassigned bounds heuristic

    if(_progressCallBack)
    {
        if(_totalUnassignedBounds < _bestUnassignedBounds)
        {
            _bestUnassignedBounds = _totalUnassignedBounds;
        }
        tSigned32 dispatched = _totalStartingUnassignedBounds - _bestUnassignedBounds;
        _progressCallBack->updateProgressL("Building BSP tree", dispatched, _totalStartingUnassignedBounds);
    }

    return splitterSelected;
}

cBSP3DManager::cBSP3DManager(
        iProgressCallBack* progressCallBack,
        bool keepDiscardedFaces,
        bool keepTree
        )
{
    _nextEdgePair = 0;
    _bestSplit = -1;
    _nextClippedOperand = 0;
    _totalUnassignedBounds = 0;
    _progressCallBack = progressCallBack;
    _keepDiscardedFaces = keepDiscardedFaces;
    _keepTree = keepTree;
    _tree = 0;
}

cBSP3DManager::~cBSP3DManager()
{
    delete _tree;
}

void
cBSP3DManager::addOperand(tMesh_Simple3D& primitive)
{
    tSigned32 operandIndex = SizeL(_initialOperands);
    _initialOperands.push_back(new cBSP3DOperand(_vertices, primitive, operandIndex, _splitters));
    _totalUnassignedBounds += _initialOperands.back()->numberOfUnassignedBounds();
}

void
cBSP3DManager::buildTreeAndSplitFragments()
{
    _totalStartingUnassignedBounds = _totalUnassignedBounds;
    _bestUnassignedBounds = _totalUnassignedBounds;
    _tree = new cBSPTree<cBSP3DManager>(*this, _initialOperands);
    _stats._splitNodes = _tree->getNumberOfSplitNodes();
    _stats._obstructedLeaves = _tree->getNumberOfObstructedLeaves();
    _stats._maximumDepth = _tree->getDepth();
    SendFragmentsDownFrontSubTree(*this, *_tree, _progressCallBack);
    if(!_keepTree)
    {
        delete _tree;
        _tree = 0;
    }
}

void
cBSP3DManager::splitOperands(
        std::vector<tClippedOperand*>& operands,
        tSigned32 splitNode, 
        std::vector<tClippedOperand*>& backOperands,
        tSigned32& splitterSelected, 
        bool& frontContained, bool& backContained
        )
{
    assertD(backOperands.empty());

    tSigned32 totalSplits = CountUnassignedBounds(operands);
    assertD(totalSplits > 0);
    tSigned32 splitsToTry = 3;
    if(splitsToTry > totalSplits)
    {
        splitsToTry = totalSplits;
    }

//    splitsToTry = totalSplits; //.......
//    splitsToTry = 1; //.......

    std::vector<tSigned32> tried(splitsToTry);

    tSigned32 i;
    for(i = 0; i != splitsToTry; ++i)
    {
        tSigned32 splitAsBoundIndex = _sequence.random31() % (totalSplits - i);

        // eliminate repeats (without biasing result!)
        tSigned32 j;
        for(j = 0; j != i; ++j)
        {
            if(splitAsBoundIndex == tried[j])
            {
                splitAsBoundIndex++;
            }
        }
#ifdef ASSERTIONS_ON
        for(j = 0; j != i; ++j)
        {
            assertD(splitAsBoundIndex != tried[j]);
        }
        assertD(splitAsBoundIndex < totalSplits);
#endif
        tried[i] = splitAsBoundIndex;
        std::sort(&tried[0], &tried[i] + 1);

        tSigned32 splitterIndex = SplitForUnassignedBoundIndex(operands, splitAsBoundIndex);
        considerSplit(operands, splitterIndex);
    }
    
    splitterSelected = doBestSplit(operands, splitNode, backOperands, frontContained, backContained);
}


void
cBSP3DManager::splitFragment(
        tFace_Dynamic toSplit, 
        tSigned32 splitterIndex, 
        bool& extendsInFront, tFace_Dynamic& inFront,
        bool& extendsBehind, tFace_Dynamic& behind
        )
{
    const cPlane& splitter = _splitters[splitterIndex];
    std::vector<eSideOfPlane> sides;
    tEdge_Dynamic e;

    extendsInFront = false;
    extendsBehind = false;
    e = tMesh_Dynamic::beginEdge(toSplit);
    do
    {
        eSideOfPlane side = SideOfPlane(splitter, e.vertex()->dynamicPoint(_vertices));
        if(side == IN_FRONT)
        {
            extendsInFront = true;
        }
        if(side == BEHIND)
        {
            extendsBehind = true;
        }
        sides.push_back(side);
    }
    while(e.toNextInFace());
    sides.push_back(sides.front());

    if(!(extendsInFront && extendsBehind))
    {
      // doesnt cross plane
        if(extendsInFront)
        {
            inFront = toSplit;
        }
        else
        {
            behind = toSplit;
        }
        return;
    }

    tEdge_Dynamic e1, e2;
    bool e1AtVertex, e2AtVertex;
    e1.clear();
    e2.clear();
    e = tMesh_Dynamic::beginEdge(toSplit);
    tSigned32 i = 0;
    do
    {
        eSideOfPlane sideForStart = sides[i];
        eSideOfPlane sideForEnd = sides[i + 1];
        if(sideForStart != IN_FRONT && sideForEnd == IN_FRONT)
        {
            assertD(!e1.valid());
            e1 = e;
            e1AtVertex = (sideForStart == ON_PLANE);
        }
        if(sideForStart != BEHIND && sideForEnd == BEHIND)
        {
            assertD(!e2.valid());
            e2 = e;
            e2AtVertex = (sideForStart == ON_PLANE);
        }
        ++i;
    }
    while(e.toNextInFace());

    assertD(e1.valid() && e2.valid());

    if(!e1AtVertex)
    {
        if(e1.twin().valid())
        {
            if(e1->_pairIndex == -1)
            {
                e1->_pairIndex = _nextEdgePair++;
                e1.twin()->_pairIndex = _nextEdgePair++;
            }
            else
            {
                assertD(e1.twin().face()->wasDiscarded());
            }
            _result.breakEdge(e1);
        }
        _result.splitEdge(e1);
        e1.next()->_pairIndex = e1->_pairIndex;
        ++e1;
        cDynamicPoint3 axis(e1.next().vertex()->dynamicPoint(_vertices));
        axis -= e1.prev().vertex()->dynamicPoint(_vertices);
        cDynamicPoint3 splitPoint;
        Intersection(
                splitter,
                e1.prev().vertex()->dynamicPoint(_vertices), axis,
                splitPoint
                );
        e1.vertex()->initForSplitPoint(_vertices, splitPoint);
    }

    if(!e2AtVertex)
    {
        if(e2.twin().valid())
        {
            if(e2->_pairIndex == -1)
            {
                e2->_pairIndex = _nextEdgePair++;
                e2.twin()->_pairIndex = _nextEdgePair++;
            }
            else
            {
                assertD(e2.twin().face()->wasDiscarded());
            }
            _result.breakEdge(e2);
        }
        _result.splitEdge(e2);
        e2.next()->_pairIndex = e2->_pairIndex;
        ++e2;
        cDynamicPoint3 axis(e2.next().vertex()->dynamicPoint(_vertices));
        axis -= e2.prev().vertex()->dynamicPoint(_vertices);
        cDynamicPoint3 splitPoint;
        Intersection(
                splitter,
                e2.prev().vertex()->dynamicPoint(_vertices), axis,
                splitPoint
                );
        e2.vertex()->initForSplitPoint(_vertices, splitPoint);
    }

    _result.splitFace(e1, e2);
    assertD(e1.prev().twin() == e2.prev()); // e1 and e2 are after each of the verts for the split

    _result.breakEdge(e1.prev());

    e1.prev()->_pairIndex = _nextEdgePair++;
    e2.prev()->_pairIndex = _nextEdgePair++;

    inFront = e1.face();
    behind = e2.face();
}

void
cBSP3DManager::fragmentIsInside(tFace_Dynamic fragment)
{
    if(_keepDiscardedFaces)
    {
        fragment->discard();
        tEdge_Dynamic e = tMesh_Dynamic::beginEdge(fragment);
        do
        {
            if(e.twin().valid())
            {
                _result.breakEdge(e);
            }
        }
        while(e.toNextInFace());
    }
    else
    {
        _result.eraseFace(fragment);
    }
}
