//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef RANGE_TREE_INCLUDED
#define RANGE_TREE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"

class cRangeTree
{
    tSigned32 m_total_elements;
    tPoint::T m_range_start, m_range_end;
    std::vector<tPoint::T> m_values;
    std::vector<tSigned32> m_child_nodes;
    std::vector<tSigned32> m_element_indices;
    std::vector<tSigned32> m_element_data;

    static void storeSubset(const std::pair<tPoint::T, tSigned32>* subset_begin,
                        const std::pair<tPoint::T, tSigned32>* subset_end,
                        std::vector<tSigned32>& indices, std::vector<tSigned32>& data);
    tSigned32 buildSubTree(const std::vector<std::pair<tPoint::T, tSigned32> >& pairs,
                        const tSigned32* unique_points_ptr, tSigned32 unique_points
                        );
    tSigned32 querySubTree(  tSigned32 node,
                        bool extends_left, bool extends_right,
                        const tPoint::T& start, const tPoint::T& end,
                        tSigned32* results_buffer) const;
public:

    cRangeTree(const std::vector<std::pair<tPoint::T, tSigned32> >& pairs);

    tSigned32 totalNumberOfElements() const;
    tSigned32 query(const tPoint::T& start, const tPoint::T& end, tSigned32* resultsBuffer) const;
};

#endif
