//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/SideEnum.h"

class cPointTraits;

#ifdef RANGE_CHECKING

template <class T> class cCheckedPoint2;
typedef cCheckedPoint2<cPointTraits> tPoint;

#else

template <class T> class cUncheckedPoint2;
typedef cUncheckedPoint2<cPointTraits> tPoint;

#endif

tSigned32 Length(const tPoint& vector);
double Length_AsDouble(const tPoint& vector);
void SetLength(tPoint& vector, tSigned32 new_length);

eSide Side_Vectors(const tPoint& base, const tPoint& test);
eDirection Direction_Vectors(const tPoint& base, const tPoint& test);
