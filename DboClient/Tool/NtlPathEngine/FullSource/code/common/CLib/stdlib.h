//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//#ifndef CLIB_STDLIB_INCLUDED
//#define CLIB_STDLIB_INCLUDED
//
//inline tSigned32
//strtol(const char* ptr, char** endPtr, int base)
//{
////    assertD(base == 10);
//    tSigned32 result = 0;
//    while(*ptr == ' ')
//    {
//        ptr++;
//    }
//    bool negate = false;
//    if(*ptr == '-')
//    {
//        negate = true;
//        ptr++;
//    }
//    if(*ptr == '+')
//    {
//        ptr++;
//    }
//
//    if(!(*ptr >= '0' && *ptr <= '9'))
//    {
//        if(endPtr)
//        {
//            const char *const* endPtr2 = const_cast<const char *const*>(endPtr);
//            *endPtr2 = ptr;
//        }
//        return 0;
//    }
//
//    do
//    {
//        int digit = *ptr++;
//        digit -= '0';
//
//        //.... assumes no overflow
//        result *= 10;
//        result += digit;
//    }
//    while(*ptr >= '0' && *ptr <= '9');
//
//    if(endPtr)
//    {
//        const char *const* endPtr2 = const_cast<const char *const*>(endPtr);
//        *endPtr2 = ptr;
//    }
//    if(negate)
//    {
//        result = -result;
//    }
//    return result;
//}
//
//inline tUnsigned32
//strtoul(const char* ptr, char** endPtr, int base)
//{
////    assertD(base == 10);
//    tUnsigned32 result = 0;
//    while(*ptr == ' ')
//    {
//        ptr++;
//    }
//    if(*ptr == '+')
//    {
//        ptr++;
//    }
//
//    if(!(*ptr >= '0' && *ptr <= '9'))
//    {
//        if(endPtr)
//        {
//            const char *const* endPtr2 = const_cast<const char *const*>(endPtr);
//            *endPtr2 = ptr;
//        }
//        return 0;
//    }
//
//    do
//    {
//        int digit = *ptr++;
//        digit -= '0';
//
//        //.... assumes no overflow
//        result *= 10;
//        result += digit;
//    }
//    while(*ptr >= '0' && *ptr <= '9');
//
//    if(endPtr)
//    {
//        const char *const* endPtr2 = const_cast<const char *const*>(endPtr);
//        *endPtr2 = ptr;
//    }
//    return result;
//}
//
//#endif

#include <stdlib.h>
