//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tPoint3_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"
#include "common/STL/string.h"

void PointFromString(const char* str, tPoint& point);
void IntersectionFromString(const char* str, tIntersection& result);
void Point3FromString(const char* str, tPoint3& point);
void LineFromString(const char* str, tLine& result);
void VerticesFromString(const char* str, std::vector<tSigned32>& result);
std::string VerticesToString(const std::vector<tSigned32>& vertices);
