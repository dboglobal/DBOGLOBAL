//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SEGMENT_TREE_INCLUDED
#define SEGMENT_TREE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class cSegmentTreePoint;

class cSegmentTreeElement
{
public:

    tPoint::T start,end;
    tSigned32 key;
};

class cSegmentTree
{
    tSigned32 m_total_segments;
    tPoint::T m_range_start,m_range_end;
    tSigned32 m_root_index;

    // each non leaf node has a split, 2 children, and a set of keys
    std::vector<tPoint::T> m_splits;
    std::vector<tSigned32> m_child_indices;
    std::vector<std::vector<tSigned32> > m_segment_sets;

    // each leaf node has just a set of keys
    std::vector<std::vector<tSigned32> > m_leaf_segment_sets;

    tSigned32 buildSubTree(const tPoint::T* unique_points_ptr, tSigned32 unique_points);
    void insertSegment(const cSegmentTreeElement& segment, tSigned32 node,
                                const tPoint::T& range_start, const tPoint::T& range_end);
    bool removeEmptySubtrees(tSigned32 node);

    tSigned32 querySubTree(  tSigned32 node,
                        const tPoint::T& stabbing_value, 
                        tSigned32* results_buffer) const;

public:

    cSegmentTree(const std::vector<cSegmentTreeElement>& segments);

    tSigned32 totalNumberOfSegments() const;
    tSigned32 query(const tPoint::T& stabbing_value, tSigned32* resultsBuffer) const;
};

#endif
