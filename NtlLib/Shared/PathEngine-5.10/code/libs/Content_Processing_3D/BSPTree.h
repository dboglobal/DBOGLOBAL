//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BSP_TREE_INCLUDED
#define BSP_TREE_INCLUDED

#include "libs/Geometry/interface/SideEnum.h"
#include "common/STL/vector.h"

template <class tOperandManager>
class cBSPTree
{
    typedef typename tOperandManager::tClippedOperand tClippedOperand;
    typedef typename tOperandManager::tSplitter tSplitter;

    // node data is only accessable through getters and setters to avoid repeat of
    //  a tricky sequence point bug involving the vector being reallocated during subtree creation
    class cNodes
    {
        std::vector<tSigned32> _data;
    public:

        tSigned32 pushBack()
        {
            tSigned32 index = SizeL(_data) / 3;
            _data.resize(_data.size() + 3);
            return index;
        }

        void setSplitter(tSigned32 node, tSigned32 value)
        {
            _data[node * 3 + 0] = value;
        }
        void setFront(tSigned32 node, tSigned32 value)
        {
            _data[node * 3 + 1] = value;
        }
        void setBack(tSigned32 node, tSigned32 value)
        {
            _data[node * 3 + 2] = value;
        }

        tSigned32 getSplitter(tSigned32 node) const
        {
            assertD(node >= 0);
            return _data[node * 3 + 0];
        }
        tSigned32 getFront(tSigned32 node) const
        {
            assertD(node >= 0);
            return _data[node * 3 + 1];
        }
        tSigned32 getBack(tSigned32 node) const
        {
            assertD(node >= 0);
            return _data[node * 3 + 2];
        }

        tSigned32 size() const
        {
            return SizeL(_data) / 3;
        }
    };

    tOperandManager& _manager;
    cNodes _nodes;
    tSigned32 _maximumDepth;
    tSigned32 _numberOfObstructedLeaves;

    bool
    isLeaf(tSigned32 node) const
    {
        return node < 0;
    }
    static tSigned32
    unobstructedLeaf()
    {
        return -1;
    }
    static tSigned32
    obstructedLeaf()
    {
        return -2;
    }

    const tSplitter&
    refSplitter(tSigned32 node) const
    {
        tSigned32 splitterIndex = _nodes.getSplitter(node);
        return _manager.refSplitter(splitterIndex);
    }

    tSigned32
    buildSubTreeForOperands(std::vector<tClippedOperand*>& operands, tSigned32 depth)
    {
        tSigned32 splitNode = _nodes.pushBack();
        depth++;
        if(depth > _maximumDepth)
        {
            _maximumDepth = depth;
        }
        
        tSigned32 splitterSelected;
        bool frontContained, backContained;

        {
            std::vector<tClippedOperand*> backOperands;
            _manager.splitOperands(operands, splitNode, backOperands, splitterSelected, frontContained, backContained);
            if(backContained)
            {
                assertD(backOperands.empty()); // manager takes all remaining clipped operands at end of split operation, in this case
                _nodes.setBack(splitNode, obstructedLeaf());
                _numberOfObstructedLeaves++;
            }
            else
            if(backOperands.empty())
            {
                _nodes.setBack(splitNode, unobstructedLeaf());
            }
            else
            {
                _nodes.setBack(splitNode, buildSubTreeForOperands(backOperands, depth));
                assertD(backOperands.empty());
            }
        }

        std::vector<tClippedOperand*>& frontOperands = operands;

        if(frontContained)
        {
            assertD(frontOperands.empty()); // manager takes all remaining clipped operands at end of split operation, in this case
            _nodes.setFront(splitNode, obstructedLeaf());
            _numberOfObstructedLeaves++;
        }
        else
        if(frontOperands.empty())
        {
            _nodes.setFront(splitNode, unobstructedLeaf());
        }
        else
        {
            _nodes.setFront(splitNode, buildSubTreeForOperands(frontOperands, depth));
            assertD(frontOperands.empty());
        }

        _nodes.setSplitter(splitNode, splitterSelected);

        //.. merge in case where both front and back are both contained?
        // (how often does this happen?)

        return splitNode;
    }

    template <class tFragmentRef> void
    splitFragmentBySubTree(tSigned32 splitNode, tFragmentRef fragment) const
    {
        if(splitNode == unobstructedLeaf())
        {
            return;
        }
        if(splitNode == obstructedLeaf())
        {
            _manager.fragmentIsInside(fragment);
            return;
        }
        bool extendsInFront, extendsBehind;
        tFragmentRef inFront, behind;
        _manager.splitFragment(fragment, _nodes.getSplitter(splitNode), extendsInFront, inFront, extendsBehind, behind);
        if(extendsInFront)
        {
            splitFragmentBySubTree(_nodes.getFront(splitNode), inFront);
        }
        if(extendsBehind)
        {
            splitFragmentBySubTree(_nodes.getBack(splitNode), behind);
        }
    }

    // boundaries are inclusive
    template <class tPoint> bool
    testObstructedBySubTree(tSigned32 subTree, const tPoint& p) const
    {
        if(subTree == obstructedLeaf())
        {
            return true;
        }
        if(subTree == unobstructedLeaf())
        {
            return false;
        }
        eSideOfPlane side = SideOfPlane(refSplitter(subTree), p);
        if(side == BEHIND)
        {
            return testObstructedBySubTree(_nodes.getBack(subTree), p);
        }
        if(side == IN_FRONT)
        {
            return testObstructedBySubTree(_nodes.getFront(subTree), p);
        }
        return 
            testObstructedBySubTree(_nodes.getBack(subTree), p)
            ||
            testObstructedBySubTree(_nodes.getFront(subTree), p)
            ;
    }

public:

    // manager must remain valid for the lifetime of this object
    cBSPTree(tOperandManager& manager, std::vector<tClippedOperand*>& operands) :
     _manager(manager)
    {
        _maximumDepth = 0;
        _numberOfObstructedLeaves = 0;
        assertD(!operands.empty());
        tSigned32 added = buildSubTreeForOperands(operands, 0);
        assertD(added == 0);
    }

    tSigned32 getDepth() const
    {
        return _maximumDepth;
    }
    tSigned32 getNumberOfSplitNodes() const
    {
        return _nodes.size();
    }
    tSigned32 getNumberOfObstructedLeaves() const
    {
        return _numberOfObstructedLeaves;
    }

    template <class tFragmentRef> void
    splitFragmentByFrontSubTree(tSigned32 splitNode, tFragmentRef fragment) const
    {
        splitFragmentBySubTree(_nodes.getFront(splitNode), fragment);
    }
    template <class tFragmentRef> void
    splitFragmentByBackSubTree(tSigned32 splitNode, tFragmentRef fragment) const
    {
        splitFragmentBySubTree(_nodes.getBack(splitNode), fragment);
    }

    // boundaries are inclusive
    template <class tPoint> bool
    testObstructed(const tPoint& p) const
    {
        return testObstructedBySubTree(0, p);
    }
};

#endif
