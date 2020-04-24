//**********************************************************************
//
// Copyright (c) 2004
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_ADD_WITH_OVERFLOW_INCLUDED
#define PLATFORM_COMMON_ADD_WITH_OVERFLOW_INCLUDED

inline bool
AddWithOverflow(tUnsigned32 source1, tUnsigned32 source2, tUnsigned32 &result)
{
    result = source1 + source2;
    return result < source1;
}
inline bool
AddWithOverflow(tSigned32 source1, tSigned32 source2, tSigned32 &result)
{
    result = source1 + source2;
    if(source2 >= 0)
    {
        return result < source1;
    }
    return result > source1;
}
inline bool
SubtractWithOverflow(tSigned32 source1, tSigned32 source2, tSigned32 &result)
{
    result = source1 - source2;
    if(source2 >= 0)
    {
        return result > source1;
    }
    return result < source1;
}

#endif
