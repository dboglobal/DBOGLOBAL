//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPLACE_STL
#include <utility>
#else
#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED

namespace std
{

template <class tFirst, class tSecond>
class pair
{
public:

    pair() :
     first(tFirst()),
     second(tSecond())
    {
    }
    pair(const tFirst& first, const tSecond& second) :
     first(first),
     second(second)
    {
    }
    template <class tRHSFirst, class tRHSSecond>
    pair(const pair<tRHSFirst, tRHSSecond>& rhs) :
     first(rhs.first),
     second(rhs.second)
    {
    }
    tFirst first;
    tSecond second;
};

template <class tFirst, class tSecond> inline bool
operator==(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}
template <class tFirst, class tSecond> inline bool
operator!=(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return !(lhs == rhs);
}
template <class tFirst, class tSecond> inline bool
operator<(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return lhs.first < rhs.first || !(rhs.first < lhs.first) && lhs.second < rhs.second;
}
template <class tFirst, class tSecond> inline bool
operator>(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return rhs < lhs;
}
template<class tFirst, class tSecond> inline bool
operator<=(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return !(rhs < lhs);
}
template<class tFirst, class tSecond> inline bool
operator>=(const pair<tFirst, tSecond>& lhs, const pair<tFirst, tSecond>& rhs)
{
    return !(lhs < rhs);
}

}

#endif
#endif
