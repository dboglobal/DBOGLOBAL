/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 */
//**********************************************************************
//
// adaptation from stlport's stl/_algo.c
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPLACE_STL
#include <algorithm>
#else
#ifndef ALGORITHM_INCLUDED
#define ALGORITHM_INCLUDED

#include "common/STL/utility.h"

namespace std
{

template <class T> void
swap(T& lhs, T& rhs)
{
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

template <class tIt, class tDistance> void
advance(tIt& i, tDistance distance)
{
    i += distance;
}

template <class tIt>
tSigned32 distance(tIt i1, tIt i2)
{
    return i2 - i1;
}

namespace nInternal
{
    template <class tIt, class T, class tPredicate, class tDistance>
    tIt lower_bound(tIt first, tIt last,
			       const T& val, tPredicate comp, tDistance*)
    {
      tDistance len = distance(first, last);
      tDistance half;
      tIt middle;
      while (len > 0)
      {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, val))
        {
          first = middle;
          ++first;
          len = len - half - 1;
        }
        else
          len = half;
      }
      return first;
    }

    template <class tIt, class T, class tPredicate, class tDistance>
    tIt upper_bound(tIt first, tIt last,
                               const T& val, tPredicate comp, tDistance*)
    {
      tDistance len = distance(first, last);
      tDistance half;
      while (len > 0)
      {
        half = len >> 1;
        tIt middle = first;
        advance(middle, half);
        if (comp(val, *middle))
          len = half;
        else
        {
          first = middle;
          ++first;
          len = len - half - 1;
        }
      }
      return first;
    }

    template <class tIt, class tDistance>
    tIt rotate(tIt first, tIt middle,
                          tIt last,
                          tDistance*)
    {
      if (first == middle)
        return last;
      if (last  == middle)
        return first;

      tIt first2 = middle;
      do {
        swap(*first++, *first2++);
        if (first == middle)
          middle = first2;
      } while (first2 != last);

      tIt new_middle = first;

      first2 = middle;

      while (first2 != last) {
        swap (*first++, *first2++);
        if (first == middle)
          middle = first2;
        else if (first2 == last)
          first2 = middle;
      }

      return new_middle;
    }

    template <class tIt, class T, class tPredicate>
    void unguarded_linear_insert(tIt last, T val, tPredicate comp)
    {
        tIt next = last;
        --next;  
        while (comp(val, *next))
        {
            *last = *next;
            last = next;
            --next;
        }
        *last = val;
    }

    template <class tIt, class T, class tPredicate> inline void
    unguarded_insertion_sort(tIt first, tIt last, T*, tPredicate comp)
    {
        for (tIt i = first; i != last; ++i)
        {
            unguarded_linear_insert(i, *i, comp);
        }
    }

    template <class tIt> inline void
    copy_backward(tIt first, tIt last, tIt result)
    {
      while(first != last)
        *--result = *--last;
    }

    template <class tIt, class T, class tPredicate> inline void
    linear_insert(tIt first, tIt last, T val, tPredicate comp)
    {		
        if (comp(val, *first))
        {
            copy_backward(first, last, last + 1);
            *first = val;
        }
        else
        {
            unguarded_linear_insert(last, val, comp);
        }
    }

    template <class tIt, class tPredicate> inline void
    insertion_sort(tIt first, tIt last, tPredicate comp)
    {
        if (first == last)
        {
            return;
        }
        for (tIt i = first + 1; i != last; ++i)
        {
            linear_insert(first, i, *i, comp);
        }
    }

    const tSigned32 stl_threshold = 16;

    template <class tIt, class tPredicate> inline void
    final_insertion_sort(tIt first, tIt last, tPredicate comp)
    {
        if (last - first > stl_threshold)
        {
            insertion_sort(first, first + stl_threshold, comp);
            unguarded_insertion_sort(first + stl_threshold, last, &*first, comp);
        }
        else
        {
           insertion_sort(first, last, comp);
        }
    }

    template <class T, class tPredicate> inline const T&
    median(const T& a, const T& b, const T& c, tPredicate comp)
    {
      if (comp(a, b))
        if (comp(b, c))
          return b;
        else if (comp(a, c))
          return c;
        else
          return a;
      else if (comp(a, c))
        return a;
      else if (comp(b, c))
        return c;
      else
        return b;
    }

    template <class tIt, class T, class tPredicate> inline tIt
    unguarded_partition(tIt first, tIt last, T pivot, tPredicate comp) 
    {
      while (true)
      {
        while (comp(*first, pivot))
          ++first;
        --last;
        while (comp(pivot, *last))
          --last;
        if (!(first < last))
          return first;
        swap(first, last);
        ++first;
      }
    }

    template <class tIt, class T, class tSize, class tPredicate> inline void
    introsort_loop(tIt first, tIt last, T*, tSize depthLimit, tPredicate comp)
    {
        while (last - first > stl_threshold)
        {
            if(depthLimit == 0)
            {
                partialSort(first, last, last, comp);
            }
            --depthLimit;
            tIt cut = unguarded_partition(first, last,
                                T(median(*first, *(first + (last - first)/2), *(last - 1), comp)),
                                             comp);
            introsort_loop(cut, last, (T*)0, depthLimit, comp);
            last = cut;
        }
    }

    template <class _Size> inline _Size
    lg(_Size n)
    {
        _Size k;
        for (k = 0; n != 1; n >>= 1)
        {
            ++k;
        }
        return k;
    }

    class cLessThanPredicate
    {
    public:
        template <class T>
        bool operator()(const T& lhs, const T& rhs) const
        {
            return lhs < rhs;
        }
    };

    template <class tIt, class tDistance, class tPredicate>
    void merge_without_buffer(tIt first,
                                tIt middle,
                                tIt last,
                                tDistance len1, tDistance len2,
                                tPredicate comp)
    {
      if (len1 == 0 || len2 == 0)
        return;
      if (len1 + len2 == 2) {
        if (comp(*middle, *first))
        {
          swap(*first, *middle);
        }
        return;
      }
      tIt first_cut = first;
      tIt second_cut = middle;
      tDistance len11 = 0;
      tDistance len22 = 0;
      if (len1 > len2)
      {
        len11 = len1 / 2;
        advance(first_cut, len11);
        second_cut = lower_bound(middle, last, *first_cut, comp, &len11);
        len22 += distance(middle, second_cut);
      }
      else
      {
        len22 = len2 / 2;
        advance(second_cut, len22);
        first_cut = upper_bound(first, middle, *second_cut, comp, &len11);
        len11 +=distance(first, first_cut);
      }
      tIt new_middle
        = rotate(first_cut, middle, second_cut, &len11);
      merge_without_buffer(first, first_cut, new_middle, len11, len22,
                             comp);
      merge_without_buffer(new_middle, second_cut, last, len1 - len11,
                             len2 - len22, comp);
    }
}

//template <class tIt, class tPredicate> inline void
//sort(tIt first, tIt last, tPredicate comp)
//{
//    if(first != last)
//    {
//        nInternal::introsort_loop(first, last, &*first,
//                     lg(last - first) * 2,
//                                comp);
//        nInternal::final_insertion_sort(first, last, comp);
//    }
//}

// from stlport's __inplace_stable_sort
template <class tIt, class tPredicate> inline void
sort(tIt first, tIt last, tPredicate comp)
{
  if(last - first < 15)
  {
    nInternal::insertion_sort(first, last, comp);
    return;
  }
  tIt middle = first + (last - first) / 2;
  sort(first, middle, comp);
  sort(middle, last, comp);
  nInternal::merge_without_buffer(first, middle, last,
                         middle - first,
                         last - middle,
                         comp);
}


template <class tIt> inline void
sort(tIt first, tIt last)
{
    sort(first, last, nInternal::cLessThanPredicate());
}

template <class tIt, class T> inline tIt
find(tIt begin, tIt end, const T& value)
{
    while(begin != end)
    {
        if(*begin == value)
        {
            return begin;
        }
        ++begin;
    }
    return end;
}

template <class tIt, class T> void
fill(tIt begin, tIt end, const T& value)
{
    while(begin != end)
    {
        *begin = value;
        ++begin;
    }
}

template <class tSourceIt, class tTargetIt> void
copy(tSourceIt begin, tSourceIt end, tTargetIt targetBegin)
{
    while(begin != end)
    {
        *targetBegin = *begin;
        ++begin;
        ++targetBegin;
    }
}

}

#endif
#endif
