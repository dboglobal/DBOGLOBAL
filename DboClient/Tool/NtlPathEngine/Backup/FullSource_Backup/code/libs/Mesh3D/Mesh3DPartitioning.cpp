//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/Mesh3DPartitioning.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/RangeTree.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"

void
cMesh3DPartitioning::initRangeTree(const std::vector<cSegmentTreeElement>& segments)
{
    std::vector<std::pair<tPoint::T, tSigned32> > pairs(segments.size());
    size_t i;
    for(i = 0; i < segments.size(); i++)
    {
        pairs[i].first = segments[i].start;
        pairs[i].second = segments[i].key;
    }
    _rangeTree = new cRangeTree(pairs);
}

cMesh3DPartitioning::~cMesh3DPartitioning()
{
    delete _segmentTree;
    delete _rangeTree;
}

void
cMesh3DPartitioning::filterByYRange(tSigned32 startY, tSigned32 endY, tSigned32* resultsBuffer, tSigned32& results) const
{
    tSigned32 i;
    for(i = 0; i < results;)
    {
        tSigned32 f = resultsBuffer[i];
        if(_yRanges[f * 2] > endY || _yRanges[f * 2 + 1] < startY)
        {
            resultsBuffer[i] = resultsBuffer[results - 1];
            results--;
        }
        else
        {
            i++;
        }
    }
}

tSigned32
cMesh3DPartitioning::maximumResultsSize()
{
    return _rangeTree->totalNumberOfElements();
}
tSigned32
cMesh3DPartitioning::queryFacesPotentiallyStabbed(const tPoint& p, tSigned32* resultsBuffer) const
{
    tSigned32 results = _segmentTree->query(p.getX(), resultsBuffer);
    filterByYRange(p.getY(), p.getY(), resultsBuffer, results);
    return results;
}
tSigned32
cMesh3DPartitioning::queryFacesPotentiallyOverlapped(const tPoint& bottomLeft, const tPoint& topRight, tSigned32* resultsBuffer) const
{
    tSigned32 results = _rangeTree->query(bottomLeft.getX(), topRight.getX(), resultsBuffer);
    results += _segmentTree->query(bottomLeft.getX(), resultsBuffer + results);
    filterByYRange(bottomLeft.getY(), topRight.getY(), resultsBuffer, results);
    return results;
}
