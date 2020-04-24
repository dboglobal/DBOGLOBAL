//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_WITH_TRACKED_TRANSFORM__OPERATIONS
#define MESH_WITH_TRACKED_TRANSFORM__OPERATIONS

#include "libs/Geometry/interface/iPointLike.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/TransformTree.h"
#include "platform_common/PointMultiplication.h"

class cSplit3Way : public nTransformTree::iOperation
{
    tLine _separatingLine;
public:

    cSplit3Way(const tLine& separatingLine) :
     _separatingLine(separatingLine)
    {
    }

    int resultFor(const iPointLike& p) const
    {
        switch(p.sideOfLine(_separatingLine))
        {
            default:
                invalid();
            case SIDE_LEFT:
                return 0;
            case SIDE_CENTRE:
                return 1;
            case SIDE_RIGHT:
                return 2;
        }
    }
    int sourceFor(const iPointLike& p) const
    {
        return 0;
    }
};

class cJoin3Way : public nTransformTree::iOperation
{
    tLine _separatingLine;
public:

    cJoin3Way(const tLine& separatingLine) :
     _separatingLine(separatingLine)
    {
    }

    int resultFor(const iPointLike& p) const
    {
        return 0;
    }
    int sourceFor(const iPointLike& p) const
    {
        switch(p.sideOfLine(_separatingLine))
        {
        default:
            invalid();
        case SIDE_LEFT:
            return 0;
        case SIDE_CENTRE:
            return 1;
        case SIDE_RIGHT:
            return 2;
        }
    }
};

class cFlip3Way : public nTransformTree::iOperation
{
    tLine _sourceSeparator;
    tLine _resultSeparator;
public:

    cFlip3Way(const tLine& sourceSeparator, const tLine& resultSeparator) :
     _sourceSeparator(sourceSeparator),
     _resultSeparator(resultSeparator)
    {
    }

    int resultFor(const iPointLike& p) const
    {
        switch(p.sideOfLine(_resultSeparator))
        {
        default:
            invalid();
        case SIDE_LEFT:
            return 0;
        case SIDE_CENTRE:
            return 1;
        case SIDE_RIGHT:
            return 2;
        }
    }
    int sourceFor(const iPointLike& p) const
    {
        switch(p.sideOfLine(_sourceSeparator))
        {
        default:
            invalid();
        case SIDE_LEFT:
            return 0;
        case SIDE_CENTRE:
            return 1;
        case SIDE_RIGHT:
            return 2;
        }
    }
};

class cSplitRadial : public nTransformTree::iOperation
{
    tPoint _centre;
    std::vector<tPoint> _spokes;

    void checkConvex() const
    {
        size_t i;
        for(i = 0; i + 1 < _spokes.size(); i++)
        {
            assertD(_spokes[i].side(_spokes[i + 1]) == SIDE_RIGHT);
        }
        assertD(_spokes[i].side(_spokes[0]) == SIDE_RIGHT);
    }

public:

    // vector is swapped in
    cSplitRadial(const tPoint centre, std::vector<tPoint>& spokes) :
     _centre(centre)
    {
        spokes.swap(_spokes);
//        _spokes.push_back(_spokes.front());
        checkConvex();
    }

    int resultFor(const iPointLike& p) const
    {
        if(p.isEquivalentTo(_centre))
        {
            return 0;
        }
        eSide sideLast = p.sideOfLine(tLine(_centre, _centre + _spokes[0]));
        tSigned32 i;
        for(i = 0; i < SizeL(_spokes); i++)
        {
            tSigned32 nextI = (i + 1) % SizeL(_spokes);
            eSide sideThis = p.sideOfLine(tLine(_centre, _centre + _spokes[nextI]));
            if(sideThis == SIDE_LEFT)
            {
                if(sideLast == SIDE_CENTRE)
                {
                    return 1 + i * 2;
                }
                if(sideLast == SIDE_RIGHT)
                {
                    return 1 + i * 2 + 1;
                }
            }
            sideLast = sideThis;
        }
        invalid();
        return 0;
    }
    int sourceFor(const iPointLike& p) const
    {
        return 0;
    }
};

class cJoinRadial : public nTransformTree::iOperation
{
    tPoint _centre;
    std::vector<tPoint> _spokes;

    void checkConvex() const
    {
        size_t i;
        for(i = 0; i + 1 < _spokes.size(); i++)
        {
            assertD(_spokes[i].side(_spokes[i + 1]) == SIDE_RIGHT);
        }
    }

public:

    // vector is swapped in
    cJoinRadial(const tPoint centre, std::vector<tPoint>& spokes) :
     _centre(centre)
    {
        spokes.swap(_spokes);
        _spokes.push_back(_spokes.front());
        checkConvex();
    }

    int resultFor(const iPointLike& p) const
    {
        return 0;
    }
    int sourceFor(const iPointLike& p) const
    {
        if(p.isEquivalentTo(_centre))
        {
            return 0;
        }
        eSide sideLast = p.sideOfLine(tLine(_centre, _centre + _spokes[0]));
        tSigned32 i;
        for(i = 0; i < SizeL(_spokes); i++)
        {
            eSide sideThis = p.sideOfLine(tLine(_centre, _centre + _spokes[i + 1]));
            if(sideThis == SIDE_LEFT)
            {
                if(sideLast == SIDE_CENTRE)
                {
                    return 1 + i * 2;
                }
                if(sideLast == SIDE_RIGHT)
                {
                    return 1 + i * 2 + 1;
                }
            }
            sideLast = sideThis;
        }
        invalid();
        return 0;
    }
};

class cFixedTwoToOne : public nTransformTree::iOperation
{
public:

    cFixedTwoToOne() {}

    int resultFor(const iPointLike& p) const
    {
        return 0;
    }
    int sourceFor(const iPointLike& p) const
    {
        return 0;
    }
};

#endif
