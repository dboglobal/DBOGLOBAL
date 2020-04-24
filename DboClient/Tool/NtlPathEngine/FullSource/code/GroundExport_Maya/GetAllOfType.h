//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef GET_ALL_OF_TYPE_INCLUDED
#define GET_ALL_OF_TYPE_INCLUDED

#include <maya/MFn.h>
#include "common/STL/vector.h"

class MDagPath;

void GetAllOfType(MFn::Type type, std::vector<MDagPath>& appendTo);
void GetAllSelectedOfType(MFn::Type type, std::vector<MDagPath>& appendTo);

#endif
