//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "Side3D.h"
#include "PlueckerLine.h"

//eSide3D
//Side3D(const cVector3F& start1, const cVector3F& axis1, const cVector3F& start2, const cVector3F& axis2)
//{
//    cVector3F offset = start2;
//    offset -= start1;
//    cVector3F field = axis1.crossProduct(offset);
//    {
//        float l = field.lengthSquared();
//        if(l == 0.0f)
//        {
//            return S3D_INTERSECTS;
//        }
//    }
//    float direction = field.dotProduct(axis2);
//    if(direction < 0.0f)
//    {
//        return S3D_ANTICLOCKWISE;
//    }
//    if(direction > 0.0f)
//    {
//        return S3D_CLOCKWISE;
//    }
//    return S3D_PARALLEL;
//}

eSide3D
Side3D_Reference(const cVector3F& lhsStart, const cVector3F& lhsEnd, const cVector3F& rhsStart, const cVector3F& rhsEnd)
{
    cVector3F lhsAxis = lhsEnd - lhsStart;
    cVector3F rhsAxis = rhsEnd - rhsStart;
    cVector3F offset = rhsStart;
    offset -= lhsStart;
    cVector3F field = lhsAxis.crossProduct(offset);
    {
        float l = field.lengthSquared();
        if(l == 0.0f)
        {
            return S3D_PARALLEL_OR_INTERSECTS;
        }
    }
    float direction = field.dotProduct(rhsAxis);
    if(direction < 0.0f)
    {
        return S3D_ANTICLOCKWISE;
    }
    if(direction > 0.0f)
    {
        return S3D_CLOCKWISE;
    }
    return S3D_PARALLEL_OR_INTERSECTS;
}

eSide3D
Side3D(const cPlueckerLine& lhs, const cPlueckerLine& rhs)
{
    float direction = rhs.product.dotProduct(lhs.axis) + lhs.product.dotProduct(rhs.axis);
    if(direction > 0.0f)
    {
        return S3D_ANTICLOCKWISE;
    }
    if(direction < 0.0f)
    {
        return S3D_CLOCKWISE;
    }
    return S3D_PARALLEL_OR_INTERSECTS;
}
