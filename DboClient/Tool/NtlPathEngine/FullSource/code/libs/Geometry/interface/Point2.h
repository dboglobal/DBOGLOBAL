//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef POINT2_INCLUDED
#define POINT2_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Geometry/interface/tPoint_Header.h"
#include "platform_common/AssemblyGeometry.h"

template <class TRAITS>
class cUncheckedPoint2
{
public:

    typedef typename TRAITS::tMultiplier tMultiplier;
    typedef typename TRAITS::tDistance tDistance;
    typedef typename TRAITS::tMultipliedDistance tMultipliedDistance;
    typedef typename TRAITS::T T;

protected:

    T data[2];
    typedef cUncheckedPoint2<TRAITS> tThis;

public:

// can be constructed uninitialised
    cUncheckedPoint2()
    {
    }

// can be constructed from x,y,range
    cUncheckedPoint2(const T& x, const T& y, const T& range)
    {
        data[0]=x;
        data[1]=y;
    }

// can be copy constructed
    cUncheckedPoint2(const tThis& rhs)
    {
        data[0]=rhs.getX();
        data[1]=rhs.getY();
    }

//    template <class R2>
//    static tThis static_cast_from(const cPoint2<R2>& rhs)
//    {
//        return tThis(static_cast<T>(rhs.getX()),static_cast<T>(rhs.getY()),0);
//    }

    T getRange() const
    {
        return T();
    }
    void setRange(const T& value)
    {
    }

    static T getRangeSupported()
    {
        return TRAITS::getMaximumSupported();
    }

    const T& getX() const
    {
        return data[0];
    }
    const T& getY() const
    {
        return data[1];
    }
    const T* getPtr() const
    {
        return data;
    }

// operators

    bool operator==(const tThis& rhs) const
    {
        return getX()==rhs.getX() && getY()==rhs.getY();
    }
    void operator-=(const tThis& rhs)
    {
        data[0]-=rhs.getX();
        data[1]-=rhs.getY();
    }
    void operator+=(const tThis& rhs)
    {
        data[0]+=rhs.getX();
        data[1]+=rhs.getY();
    }
    
    void operator*=(tSigned32 rhs)
    {
        data[0]*=rhs;
        data[1]*=rhs;
    }
//    void operator*=(float rhs)
//    {
//        data[0]*=rhs;
//        data[1]*=rhs;
//    }

    void
    multiplyByFloat(float rhs)
    {
        data[0] = static_cast<T>(data[0] * rhs);
        data[1] = static_cast<T>(data[1] * rhs);
    }

    template <class SCALAR>
    void operator/=(const SCALAR& rhs)
    {
        data[0]/=rhs;
        data[1]/=rhs;
    }

    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }
    template <class T>
    tThis operator+(const T& rhs) const
    {
        tThis result(*this);
        result+=rhs;
        return result;
    }
    template <class T>
    tThis operator-(const T& rhs) const
    {
        tThis result(*this);
        result-=rhs;
        return result;
    }
    tThis operator*(tSigned32 rhs) const
    {
        tThis result(*this);
        result*=rhs;
        return result;
    }
//    tThis operator*(float rhs) const
//    {
//        tThis result(*this);
//        result*=rhs;
//        return result;
//    }
    tThis multipliedByFloat(float rhs) const
    {
        tThis result(*this);
        result.multiplyByFloat(rhs);
        return result;
    }
    template <class T>
    tThis operator/(const T& rhs) const
    {
        tThis result(*this);
        result/=rhs;
        return result;
    }

    void reverse()
    {
        data[0]=-data[0];
        data[1]=-data[1];
    }

    tThis operator-() const
    {
        tThis result=*this;
        result.reverse();
        return result;
    }

    void rotateRight90()
    {
        T tmp = data[0];
        data[0] = data[1];
        data[1] = -tmp;
    }
    void rotateLeft90()
    {
        T tmp = data[0];
        data[0] = -data[1];
        data[1] = tmp;
    }
    void rotateRight45()
    {
        T tmp = data[1] - data[0];
        data[0] = data[0] + data[1];
        data[1] = tmp;
    }
    void rotateLeft45()
    {
        T tmp = data[0] + data[1];
        data[0] = data[0] - data[1];
        data[1] = tmp;
    }

    tThis left90() const
    {
        tThis result = *this;
        result.rotateLeft90();
        return result;
    }
    tThis right90() const
    {
        tThis result = *this;
        result.rotateRight90();
        return result;
    }

// distance stuff

    tDistance dotProduct(const tThis& rhs) const
    {
        tDistance result1,result2;
        tMultiplier::multiply(getX(),rhs.getX(),result1);
        tMultiplier::multiply(getY(),rhs.getY(),result2);
        result1 += result2;
        return result1;
    }
    tDistance lengthSquared() const
    {
        tDistance result1,result2;
        tMultiplier::multiply(getX(),getX(),result1);
        tMultiplier::multiply(getY(),getY(),result2);
        result1 += result2;
        return result1;
    }

// side stuff

    eSide side(const tThis& test) const
    {
//        const tThis& base=(*this);
//        tDistance lhs,rhs;
//        tMultiplier::multiply(test.getX(),base.getY(),lhs);
//        tMultiplier::multiply(test.getY(),base.getX(),rhs);
//        if(lhs==rhs)
//            return SIDE_CENTRE;
//        if(lhs<rhs)
//            return SIDE_LEFT;
//        return SIDE_RIGHT;
        return nAssemblyGeometry::side(getX(), getY(), test.getX(), test.getY());
    }

    eSide side_WithInfinitesimals(const tThis& offset, const tThis& offset_iota, const tThis& offset_iota2) const
    {
        const tThis& base=(*this);
        tDistance lhs,rhs;
        tMultiplier::multiply(offset.getX(),base.getY(),lhs);
        tMultiplier::multiply(offset.getY(),base.getX(),rhs);
        if(lhs==rhs)
        {
            tMultiplier::multiply(offset_iota.getX(),base.getY(),lhs);
            tMultiplier::multiply(offset_iota.getY(),base.getX(),rhs);
            if(lhs==rhs)
            {
                tMultiplier::multiply(offset_iota2.getX(),base.getY(),lhs);
                tMultiplier::multiply(offset_iota2.getY(),base.getX(),rhs);
            }
        }
        if(lhs<rhs)
            return SIDE_LEFT;
        return SIDE_RIGHT;
    }

    eSide side_StdIotas(const tThis& offset) const
    {
        const tThis& base=(*this);
        tDistance lhs,rhs;
        tMultiplier::multiply(offset.getX(),base.getY(),lhs);
        tMultiplier::multiply(offset.getY(),base.getX(),rhs);
        if(lhs==rhs)
        {
            // iota1.x = 1, iota1.y = 0
            if(base.getY()>0)
                return SIDE_RIGHT;
            if(base.getY()<0)
                return SIDE_LEFT;
            // iota2.x = 0, iota2.y = 1
            if(base.getX()>0)
                return SIDE_LEFT;
            if(base.getX()<0)
                return SIDE_RIGHT;
            return SIDE_CENTRE;
        }
        if(lhs<rhs)
            return SIDE_LEFT;
        return SIDE_RIGHT;
    }

    eDirection direction(const tThis& test) const
    {
        const tThis& base=(*this);
        tDistance lhs,rhs;
        if(base.getX()==0 && base.getY()==0)
            return DIRECTION_NONE;
        if(test.getX()==0 && test.getY()==0)
            return DIRECTION_NONE;
        tMultiplier::multiply(base.getY(),test.getX(),lhs);
        tMultiplier::multiply(base.getX(),test.getY(),rhs);
        if(lhs==rhs)
        {
            if(base.getX())
            {
                if(base.getX()>0 == test.getX()>0)
                    return DIRECTION_FORWARD;
                return DIRECTION_BACK;
            }
            if(base.getY()>0 == test.getY()>0)
                return DIRECTION_FORWARD;
            return DIRECTION_BACK;
        }
        if(lhs<rhs)
            return DIRECTION_LEFT;
        return DIRECTION_RIGHT;
    }

    // returns side of intersection with test relative to intersection with base
    // both lines must be to the right of this
    // side returned is therefore in the sense of those lines
    // both lines and this vector must all be non zero length
    eSide intersectionOrder_Clockwise(
        const tThis& base_start, const tThis& base_axis,
        const tThis& test_start, const tThis& test_axis) const
    {
        assertD(side(base_axis)==SIDE_RIGHT);
        assertD(side(test_axis)==SIDE_RIGHT);
        tDistance to_base_intersect;
        tDistance base_total;
        tDistance to_test_intersect;
        tDistance test_total;
        tMultipliedDistance to_base_intersect_normalised;
        tMultipliedDistance to_test_intersect_normalised;

// calulate distances for where axis crosses base

        to_base_intersect=base_axis.left90().dotProduct(base_start);
        base_total=base_axis.left90().dotProduct(*this);

// calulate distances for where axis crosses test

        to_test_intersect=test_axis.left90().dotProduct(test_start);
        test_total=test_axis.left90().dotProduct(*this);

// cross multiply to common metric

        tMultiplier::multiply(to_base_intersect,test_total,to_base_intersect_normalised);
        tMultiplier::multiply(to_test_intersect,base_total,to_test_intersect_normalised);

// determine which is first
// return corresponding code

        if(to_test_intersect_normalised==to_base_intersect_normalised)
            return SIDE_CENTRE;
        if(to_test_intersect_normalised<to_base_intersect_normalised)
            return SIDE_RIGHT;
        return SIDE_LEFT;
    }

    void applyRatio(const tDistance& n, const tDistance& d)
    {
        assertD(absoluteValueOf(n)<=absoluteValueOf(d)); // otherwise range should be modified
        tMultipliedDistance multiplied;
        multiplied=n;
        multiplied*=data[0];
        data[0]=multiplied/d;
        multiplied=n;
        multiplied*=data[1];
        data[1]=multiplied/d;
    }
    static tThis applyRatio(const tThis& start, const tThis& axis,
                        const tDistance& n, const tDistance& d,
                        T result_range)
    {
        T x,y;
        tMultipliedDistance multiplied;
        tMultiplier::multiply(n, axis.getX(), multiplied);
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(n, axis.getY(), multiplied);
        tMultiplier::divide(multiplied, d, y);
        return tThis(start.getX()+x,start.getY()+y,result_range);
    }

    tSigned32 longestAxis()
    {
        T abs[2];
        abs[0]=data[0];
        if(abs[0]<0)
            abs[0]=-abs[0];
        abs[1]=data[1];
        if(abs[1]<0)
            abs[1]=-abs[1];
        if(abs[0]>=abs[1])
            return 0;
        return 1;
    }

    static tThis roundedAtRatioAlong(
            const tThis& start, const tThis& end,
            const tDistance& n,
            const tDistance& d,
            const T& result_range,
            bool& x_rounded, bool& y_rounded
            )
    {
        assertD(!isZero(d));
        T axis_x,axis_y,x,y;
        tMultipliedDistance multiplied,remultiplied;

    //.... optimise - check remainder when dividing
        axis_x = end.data[0]-start.data[0];
        tMultiplier::multiply(n, axis_x, multiplied);
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(d, x, remultiplied);
        x_rounded=(remultiplied!=multiplied);

        axis_y = end.data[1]-start.data[1];
        tMultiplier::multiply(n, axis_y, multiplied);
        tMultiplier::divide(multiplied, d, y);
        tMultiplier::multiply(d, y, remultiplied);
        y_rounded = (remultiplied != multiplied);

        if(x_rounded && axis_x<0)
            x--;
        if(y_rounded && axis_y<0)
            y--;

        return tThis(start.data[0]+x,start.data[1]+y,result_range);
    }

// returns 1,2, or 4 points depending on where the point to approximate falls
// if 4 points are returned then these are guaranteed to go around clockwise
    void approximatePointsAtRatioAlong(
            const tDistance& n,
            const tDistance& d,
            tSigned32& result_points,
            tThis *result_buffer,
            const T& result_range
            ) const
    {
        assertD(!tMultiplier::isZero(d));
        T x,y;
        bool x_rounded,y_rounded;
        tMultipliedDistance multiplied,remultiplied;

    //.... optimise - check remainder when dividing

        tSigned32 x_direction=1;
        tSigned32 y_direction=1;

        tMultiplier::multiply(n, data[0], multiplied);
        if(tMultiplier::isNegative(multiplied))
        {
            x_direction = -1;
        }
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(d, x, remultiplied);
        x_rounded = (remultiplied != multiplied);

        tMultiplier::multiply(n, data[1], multiplied);
        if(tMultiplier::isNegative(multiplied))
        {
            y_direction = -1;
        }
        tMultiplier::divide(multiplied,d,y);
        tMultiplier::multiply(d, y, remultiplied);
        y_rounded=(remultiplied!=multiplied);

        result_buffer[0]=tThis(x,y,result_range);
        if(x_rounded && y_rounded)
        {
            result_points=4;
            if(x_direction == y_direction)
            {
                result_buffer[1]=tThis(x,y+y_direction,result_range);
                result_buffer[2]=tThis(x+x_direction,y+y_direction,result_range);
                result_buffer[3]=tThis(x+x_direction,y,result_range);
            }
            else
            {
                result_buffer[1]=tThis(x+x_direction,y,result_range);
                result_buffer[2]=tThis(x+x_direction,y+y_direction,result_range);
                result_buffer[3]=tThis(x,y+y_direction,result_range);
            }
            return;
        }
        if(x_rounded)
        {
            result_points=2;
            result_buffer[1]=tThis(x+x_direction,y,result_range);
            return;
        }
        if(y_rounded)
        {
            result_points=2;
            result_buffer[1]=tThis(x,y+y_direction,result_range);
            return;
        }
        result_points=1;
    }

    bool insideOrOnBounds(const tThis& min, const tThis& max) const
    {
        if(data[0]<min.data[0])
            return false;
        if(data[0]>max.data[0])
            return false;
        if(data[1]<min.data[1])
            return false;
        if(data[1]>max.data[1])
            return false;
        return true;
    }
    bool insideBounds(const tThis& min, const tThis& max) const
    {
        if(data[0]<=min.data[0])
            return false;
        if(data[0]>=max.data[0])
            return false;
        if(data[1]<=min.data[1])
            return false;
        if(data[1]>=max.data[1])
            return false;
        return true;
    }

    T getRangeUsed() const
    {
        T abs[2];
        abs[0]=data[0];
        if(abs[0]<0)
            abs[0]=-abs[0];
        abs[1]=data[1];
        if(abs[1]<0)
            abs[1]=-abs[1];
        if(abs[0]>=abs[1])
            return abs[0];
        return abs[1];

    }

    void updateAsMinimumBound(const tThis& rhs)
    {
        if(rhs.data[0]<data[0])
            data[0]=rhs.data[0];
        if(rhs.data[1]<data[1])
            data[1]=rhs.data[1];
    }
    void updateAsMaximumBound(const tThis& rhs)
    {
        if(rhs.data[0]>data[0])
            data[0]=rhs.data[0];
        if(rhs.data[1]>data[1])
            data[1]=rhs.data[1];
    }

    void setAsTopLeft(const tThis& bottomLeft, const tThis& topRight)
    {
        data[0] = bottomLeft.data[0];
        data[1] = topRight.data[1];
    }
    void setAsBottomRight(const tThis& bottomLeft, const tThis& topRight)
    {
        data[0] = topRight.data[0];
        data[1] = bottomLeft.data[1];
    }

    eDirection direction_KnownToBeInline(const tThis& rhs) const
    {
        if(data[0])
        {
            assertD(rhs.data[0]);
            if((data[0] > 0) == (rhs.data[0] > 0))
                return DIRECTION_FORWARD;
            return DIRECTION_BACK;
        }
        assertD(data[1]);
        assertD(rhs.data[1]);
        if((data[1] > 0) == (rhs.data[1] > 0))
            return DIRECTION_FORWARD;
        return DIRECTION_BACK;
    }

// these methods are just here to make points look like intersections
// so that code expecting intersections can be compiled with points
    tThis point() const
    {
        return *this;
    }
    tThis approximatePoint() const
    {
        return *this;
    }
    bool isPoint() const
    {
        return true;
    }

// help make points look like iPointLike
    bool isEquivalentTo(const tPoint& p) const
    {
        return *this == p;
    }
    template <class tLine> eSide
    sideOfLine(const tLine& l) const
    {
        return l.side(*this);
    }
};



template <class TRAITS>
class cCheckedPoint2 : public cUncheckedPoint2<TRAITS>
{
public:

    typedef cUncheckedPoint2<TRAITS> tParent;
    typedef typename TRAITS::tMultiplier tMultiplier;
    typedef typename TRAITS::tDistance tDistance;
    typedef typename TRAITS::tMultipliedDistance tMultipliedDistance;
    typedef typename TRAITS::T T;

private:

    T range;
    typedef cCheckedPoint2<TRAITS> tThis;
    void setRangeToMax(const tThis& rhs)
    {
        if(rhs.range > range)
        {
            range = rhs.range;
        }
    }

public:

// can be constructed uninitialised
    cCheckedPoint2()
    {
    }

// can be constructed from x,y,range
    cCheckedPoint2(const T& x, const T& y, const T& range) :
        cUncheckedPoint2<TRAITS>(x, y, 0),
        range(range)
    {
        TRAITS::checkRange(range);
        TRAITS::checkInRange(range, x);
        TRAITS::checkInRange(range, y);
    }

// can be copy constructed
    cCheckedPoint2(const tThis& rhs) :
     cUncheckedPoint2<TRAITS>(rhs),
     range(rhs.range)
    {
    }

    T getRange() const
    {
        return range;
    }
    void setRange(const T& value)
    {
        range = value;
        TRAITS::checkRange(range);
        TRAITS::checkInRange(range, this->getX());
        TRAITS::checkInRange(range, this->getY());
    }

// operators

    void operator-=(const tThis& rhs)
    {
        TRAITS::addRange(rhs.range, range);
        tParent::operator-=(rhs);
    }
    void operator+=(const tThis& rhs)
    {
        TRAITS::addRange(rhs.range, range);
        tParent::operator+=(rhs);
    }
    
    void operator*=(tSigned32 rhs)
    {
        TRAITS::multiplyRange(range, rhs);
        tParent::operator*=(rhs);
    }
    void
    multiplyByFloat(float rhs)
    {
        TRAITS::multiplyRange(range, rhs);
        tParent::multiplyByFloat(rhs);
    }
    void operator/=(tSigned32 rhs)
    {
        TRAITS::divideRange(range, rhs);
        tParent::operator/=(rhs);
    }

    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }
    template <class T>
    tThis operator+(const T& rhs) const
    {
        tThis result(*this);
        result+=rhs;
        return result;
    }
    template <class T>
    tThis operator-(const T& rhs) const
    {
        tThis result(*this);
        result-=rhs;
        return result;
    }
    tThis operator*(const tSigned32 rhs) const
    {
        tThis result(*this);
        result*=rhs;
        return result;
    }

//    tThis operator*(const float rhs) const
//    {
//        tThis result(*this);
//        result*=rhs;
//        return result;
//    }
    tThis multipliedByFloat(const float rhs) const
    {
        tThis result(*this);
        result.multiplyByFloat(rhs);
        return result;
    }

    template <class T>
    tThis operator/(const T& rhs) const
    {
        tThis result(*this);
        result/=rhs;
        return result;
    }

    tThis operator-() const
    {
        tThis result=*this;
        result.reverse();
        return result;
    }

    tThis left90() const
    {
        tThis result = *this;
        result.rotateLeft90();
        return result;
    }
    tThis right90() const
    {
        tThis result = *this;
        result.rotateRight90();
        return result;
    }
    void rotateRight45()
    {
        TRAITS::multiplyRange(range, 2L);
        tParent::rotateRight45();
//        T tmp = data[1] - data[0];
//        data[0] = data[0] + data[1];
//        data[1] = tmp;
    }
    void rotateLeft45()
    {
        TRAITS::multiplyRange(range, 2L);
        tParent::rotateLeft45();
//        T tmp = data[0] + data[1];
//        data[0] = data[0] - data[1];
//        data[1] = tmp;
    }

    static tThis applyRatio(const tThis& start, const tThis& axis,
                        const tDistance& n, const tDistance& d,
                        T result_range)
    {
        T x,y;
        tMultipliedDistance multiplied;
        tMultiplier::multiply(n, axis.getX(), multiplied);
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(n, axis.getY(), multiplied);
        tMultiplier::divide(multiplied, d, y);
        return tThis(start.getX() + x, start.getY() + y, result_range);
    }

    static tThis roundedAtRatioAlong(
            const tThis& start, const tThis& end,
            const tDistance& n,
            const tDistance& d,
            const T& result_range,
            bool& x_rounded, bool& y_rounded
            )
    {
        assertD(!tMultiplier::isZero(d));
        T axis_x,axis_y,x,y;
        tMultipliedDistance multiplied,remultiplied;

    //.... optimise - check remainder when dividing
        axis_x = end.data[0]-start.data[0];
        tMultiplier::multiply(n, axis_x, multiplied);
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(d, x, remultiplied);
        x_rounded=(remultiplied!=multiplied);

        axis_y = end.data[1]-start.data[1];
        tMultiplier::multiply(n, axis_y, multiplied);
        tMultiplier::divide(multiplied, d, y);
        tMultiplier::multiply(d, y, remultiplied);
        y_rounded = (remultiplied != multiplied);

        if(x_rounded && axis_x<0)
            x--;
        if(y_rounded && axis_y<0)
            y--;

        return tThis(start.data[0]+x,start.data[1]+y,result_range);
    }

// returns 1,2, or 4 points depending on where the point to approximate falls
// if 4 points are returned then these are guaranteed to go around clockwise
    void approximatePointsAtRatioAlong(
            const tDistance& n,
            const tDistance& d,
            tSigned32& result_points,
            tThis *result_buffer,
            const T& result_range
            ) const
    {
        assertD(!tMultiplier::isZero(d));
        T x,y;
        bool x_rounded,y_rounded;
        tMultipliedDistance multiplied,remultiplied;

    //.... optimise - check remainder when dividing

        tSigned32 x_direction = 1;
        tSigned32 y_direction = 1;

        tMultiplier::multiply(n, this->data[0], multiplied);
        if(tMultiplier::isNegative(multiplied))
        {
            x_direction = -1;
        }
        tMultiplier::divide(multiplied, d, x);
        tMultiplier::multiply(d, x, remultiplied);
        x_rounded = (remultiplied != multiplied);

        tMultiplier::multiply(n, this->data[1], multiplied);
        if(tMultiplier::isNegative(multiplied))
        {
            y_direction = -1;
        }
        tMultiplier::divide(multiplied,d,y);
        tMultiplier::multiply(d, y, remultiplied);
        y_rounded=(remultiplied!=multiplied);

        result_buffer[0]=tThis(x,y,result_range);
        if(x_rounded && y_rounded)
        {
            result_points=4;
            if(x_direction == y_direction)
            {
                result_buffer[1]=tThis(x,y+y_direction,result_range);
                result_buffer[2]=tThis(x+x_direction,y+y_direction,result_range);
                result_buffer[3]=tThis(x+x_direction,y,result_range);
            }
            else
            {
                result_buffer[1]=tThis(x+x_direction,y,result_range);
                result_buffer[2]=tThis(x+x_direction,y+y_direction,result_range);
                result_buffer[3]=tThis(x,y+y_direction,result_range);
            }
            return;
        }
        if(x_rounded)
        {
            result_points=2;
            result_buffer[1]=tThis(x+x_direction,y,result_range);
            return;
        }
        if(y_rounded)
        {
            result_points=2;
            result_buffer[1]=tThis(x,y+y_direction,result_range);
            return;
        }
        result_points=1;
    }

    void updateAsMinimumBound(const tThis& rhs)
    {
        setRangeToMax(rhs);
        tParent::updateAsMinimumBound(rhs);
    }
    void updateAsMaximumBound(const tThis& rhs)
    {
        setRangeToMax(rhs);
        tParent::updateAsMaximumBound(rhs);
    }

    void setAsTopLeft(const tThis& bottomLeft, const tThis& topRight)
    {
        range = bottomLeft.getRange();
        setRangeToMax(topRight.getRange());
        tParent::setAsTopLeft(bottomLeft, topRight);
    }
    void setAsBottomRight(const tThis& bottomLeft, const tThis& topRight)
    {
        range = bottomLeft.getRange();
        setRangeToMax(topRight.getRange());
        tParent::setAsBottomRight(bottomLeft, topRight);
    }

// these methods are just here to make points look like intersections
// so that code expecting intersections can be compiled with points
    tThis point() const
    {
        return *this;
    }
    tThis approximatePoint() const
    {
        return *this;
    }

// help make points look like iPointLike
    bool isEquivalentTo(const tPoint& p) const
    {
        return *this == p;
    }
    template <class tLine> eSide
    sideOfLine(const tLine& l) const
    {
        return l.side(*this);
    }
};


#endif // POINT2_INCLUDED

