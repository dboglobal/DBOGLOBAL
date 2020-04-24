//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/RangeTree.h"
//#include "libs/Instrumentation/interface/Profiler.h"
#include "common/STL/algorithm.h"
#include "common/STL_Helpers.h"

using namespace std;

class cRangeTreePredicate
{
public:
	cRangeTreePredicate() // Stop MSVC71 complaining about the object not being 'defined'
	{
	}

    bool operator()(const pair<tPoint::T,tSigned32>& lhs, const pair<tPoint::T,tSigned32>& rhs) const
    {
        return lhs.first < rhs.first;
    }
};

void
cRangeTree::storeSubset(const std::pair<tPoint::T, tSigned32>* subset_begin,
                        const std::pair<tPoint::T, tSigned32>* subset_end,
                        std::vector<tSigned32>& indices, std::vector<tSigned32>& data)
{
    tSigned32 start_index = SizeL(data);
    indices.push_back(start_index);

    tSigned32 subset_size = static_cast<tSigned32>(subset_end - subset_begin) + 1;
    assertD(subset_size >= 0);
    data.resize(start_index + subset_size);

    while(1)
    {
        data[start_index++] = subset_begin->second;
        if(subset_begin == subset_end)
            break;
        subset_begin++;
    }
}

tSigned32
cRangeTree::buildSubTree(const std::vector<std::pair<tPoint::T, tSigned32> >& pairs,
                        const tSigned32* unique_points_ptr, tSigned32 unique_points
                        )
{
    assertD(unique_points > 0);

    tSigned32 node = SizeL(m_values);

// store canonical subset
    {
        tSigned32 subset_begin = unique_points_ptr[0];
        tSigned32 subset_end = unique_points_ptr[unique_points] - 1;

        storeSubset(    &pairs[subset_begin], &pairs[subset_end],
                        m_element_indices,
                        m_element_data);
    }

    tSigned32 split = unique_points / 2;

// store subset exactly at split
    {
        tSigned32 subset_begin = unique_points_ptr[split];
        tSigned32 subset_end = unique_points_ptr[split + 1] - 1;

        storeSubset(    &pairs[subset_begin], &pairs[subset_end],
                        m_element_indices,
                        m_element_data);
    }

    {
        tSigned32 i = unique_points_ptr[split];
        m_values.push_back(pairs[i].first);
    }

    // reserve space for child indices;
    m_child_nodes.resize(m_child_nodes.size()+2);

    tSigned32 before = -1;
    if(split > 0)
    {
        before = buildSubTree(pairs, unique_points_ptr, split);
    }
    tSigned32 after = -1;
    tSigned32 first_after_split = split + 1;
    if(first_after_split < unique_points)
    {
        after = buildSubTree(pairs, unique_points_ptr + first_after_split, unique_points - first_after_split);
    }

    m_child_nodes[node * 2 + 0] = before;
    m_child_nodes[node * 2 + 1] = after;
    
    return node;
}

tSigned32 
cRangeTree::querySubTree(   tSigned32 node,
                            bool extends_left, bool extends_right,
                            const tPoint::T& start, const tPoint::T& end,
                            tSigned32* results_buffer) const
{
    tSigned32 results = 0;

    if(extends_left && extends_right)
    {
    // report canonical subset for this node and return
        tSigned32 begin = m_element_indices[node * 2 + 0];
        tSigned32 end = m_element_indices[node * 2 + 1];
        while(begin < end)
        {
            results_buffer[results++] = m_element_data[begin++];
        }

        return results;
    }

    tPoint::T split_value = m_values[node];

    if(extends_left || start < split_value - 1)
    {
    // expand left sub tree
        tSigned32 before = m_child_nodes[node * 2 + 0];
        if(before != -1)
        {
            bool extends_right = (end >= split_value);
            results += querySubTree(before, extends_left, extends_right, start, end, results_buffer + results);
        }
    }
    if(start < split_value && end > split_value)
    {
    // report subset exactly at this node
        tSigned32 begin = m_element_indices[node * 2 + 1];
        tSigned32 end = m_element_indices[node * 2 + 2];
        while(begin < end)
        {
            results_buffer[results++] = m_element_data[begin++];
        }
    }
    if(extends_right || end > split_value + 1)
    {
    // expand right sub tree
        tSigned32 after = m_child_nodes[node * 2 + 1];
        if(after != -1)
        {
            bool extends_left = (start <= split_value);
            results += querySubTree(after, extends_left, extends_right, start, end, results_buffer + results);
        }
    }

    return results;
}

cRangeTree::cRangeTree(const std::vector<std::pair<tPoint::T, tSigned32> >& pairs)
{
    m_total_elements = SizeL(pairs);
    if(m_total_elements == 0)
    {
        return;
    }

    vector<pair<tPoint::T, tSigned32> > working_pairs = pairs;
    cRangeTreePredicate predicate;
    sort(working_pairs.begin(), working_pairs.end(), predicate);

    m_range_start = working_pairs.front().first;
    m_range_end = working_pairs.back().first;

    vector<tSigned32> unique_points(working_pairs.size() + 1);
    {
        tSigned32 actual_size = 0;
        tSigned32 i;
        for(i = 0; i < SizeL(working_pairs); i++)
        {
            if(i == 0 || predicate(working_pairs[i-1],working_pairs[i]))
            {
                unique_points[actual_size++] = i;
            }
        }
        unique_points[actual_size++] = i;
        unique_points.resize(actual_size);
    }

    tSigned32 root_index = buildSubTree(working_pairs, &unique_points.front(), SizeL(unique_points) - 1);
    assertD(root_index == 0);

    m_element_indices.push_back(SizeL(m_element_data));

    ShrinkToFit(m_values);
    ShrinkToFit(m_child_nodes);
    ShrinkToFit(m_element_indices);
    ShrinkToFit(m_element_data);
}

tSigned32
cRangeTree::totalNumberOfElements() const
{
    return m_total_elements;
}

tSigned32
cRangeTree::query(const tPoint::T& start, const tPoint::T& end, tSigned32* resultsBuffer) const
{
    if(m_total_elements == 0)
        return 0;
    if(start >= m_range_end || end <= m_range_start)
        return 0;
    bool extends_left = (start < m_range_start);
    bool extends_right = (end > m_range_end);
    return querySubTree(0, extends_left, extends_right, start, end, resultsBuffer);
}

