//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"

// returns true if the test point is inside this corner
bool InsideCorner(const tLine &before, const tLine &after, const tPoint &test);

// returns true if the test vector goes inside the corner, no matter where from
bool InsideCorner_Vector(const tLine &before, const tLine &after, const tPoint &test);

// returns true if the test line goes inside the corner, no matter where from
bool InsideCorner(const tLine &before, const tLine &after, const tLine &test);

// returns true if the line comes from outside the corner and goes inside
bool EntersCorner(const tLine &before, const tLine &after, const tLine &test);
