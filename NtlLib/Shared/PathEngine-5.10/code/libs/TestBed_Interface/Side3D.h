//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

enum eSide3D
{
    S3D_PARALLEL_OR_INTERSECTS,
    S3D_CLOCKWISE,
    S3D_ANTICLOCKWISE,
};

class cVector3F;
eSide3D Side3D_Reference(const cVector3F& lhsStart, const cVector3F& lhsEnd, const cVector3F& rhsStart, const cVector3F& rhsEnd);

class cPlueckerLine;
eSide3D Side3D(const cPlueckerLine& lhs, const cPlueckerLine& rhs);
