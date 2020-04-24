//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//#ifndef CLIB_STRING_INCLUDED
//#define CLIB_STRING_INCLUDED
//
//inline tSigned32 strlen(const char* ptr)
//{
//    tSigned32 result = 0;
//    while(*ptr++)
//    {
//        result++;
//    }
//    return result;
//}
//inline char* strcpy(char* dest, const char* source)
//{
//    char* result = dest;
//    do
//    {
//        *dest++ = *source;
//    }
//    while(*source++);
//    return result;
//}
//inline int strcmp(const char* lhs, const char* rhs)
//{
//    int difference;
//    while(1)
//    {
//        difference = static_cast<unsigned char>(*lhs);
//        difference -= static_cast<unsigned char>(*rhs);
//        if(difference != 0)
//        {
//            if(difference > 0)
//            {
//                return 1;
//            }
//            return -1;
//        }
//        if(*lhs == 0)
//        {
//            if(difference < 0)
//            {
//                return -1;
//            }
//            return 0;
//        }
//        ++lhs;
//        ++rhs;
//    }
//}
//
//#endif

#include <string.h>
