//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON__DIRECTORY_INCLUDED
#define PLATFORM_COMMON__DIRECTORY_INCLUDED

#include "common/STL/string.h"

inline std::string
WorkingDirectory()
{
    return "";
}
inline std::string
ResourceRootDirectory()
{
    return "..\\";
}
inline std::string
ResultsDirectory()
{
    return "..\\..\\";
}

#define STATIC_STRING_FOR_FILE_IN_WORKING_DIRECTORY(fileName) fileName

inline char
PathSeparator()
{
	return '\\';
}

#endif
