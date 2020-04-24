//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INTERSECTION2_INCLUDED
#define INTERSECTION2_INCLUDED

#include "libs/Geometry/interface/Line.h"

template <class tPoint>
class cIntersection2
{
    typedef typename tPoint::tMultiplier tMultiplier;
    typedef typename tPoint::tDistance tDistance;
    typedef typename tPoint::tMultipliedDistance tMultipliedDistance;
    cLine<tPoint> m_line;
    tDistance m_numerator,m_denominator;
    typedef cIntersection2<tPoint> tCopy;
    typedef cIntersection2<tPoint> tThis;
public:

    cIntersection2() {}

    template <class R2>
    cIntersection2(const cIntersection2<R2>& rhs) :
      m_line(rhs.m_line),
      m_numerator(rhs.m_numerator),
      m_denominator(rhs.m_denominator)
    {
    }

    cIntersection2(const tPoint& rhs) 
    {
        m_line = cLine<tPoint>(rhs,rhs+tPoint(1,0,1));
        //.. optimise - use specific initialiser functions here
        m_numerator = tDistance(0);
        m_denominator = tDistance(1);
    }

    template <class R2, class R3>
    cIntersection2(const cLine<R2>& line0, const cLine<R3>& line1)
    {
        m_line = line0;

        tPoint axis=line0.axis();
        tPoint normal;
        
        // normalised to positive denominator
        eSide side = axis.side(line1.axis());
        assertD(side != SIDE_CENTRE);
        if(side == SIDE_RIGHT)
            normal=line1.axis().left90();
        else
            normal=line1.axis().right90();

        m_denominator=normal.dotProduct(axis);

        tPoint offset = line1.start()-line0.start();
        m_numerator=normal.dotProduct(offset);
    }

    cIntersection2(const cLine<tPoint>& line, const tDistance& n, const tDistance& d) :
      m_line(line),
      m_numerator(n),
      m_denominator(d)
    {
          if(tMultiplier::isNegative(m_denominator))
          {
              tMultiplier::negate(m_numerator);
              tMultiplier::negate(m_denominator);
          }
    }

    const cLine<tPoint>& refLine() const
    {
        return m_line;
    }

// operators

    template <class R2>
    bool operator==(const cIntersection2<R2>& rhs) const
    {
        tMultipliedDistance x,y;
        tMultipliedDistance rhs_x,rhs_y;
        getNumerators(x,y);
        rhs.getNumerators(rhs_x,rhs_y);
        x *= rhs.m_denominator;
        rhs_x *= m_denominator;
        if(x!=rhs_x)
            return false;
        y *= rhs.m_denominator;
        rhs_y *= m_denominator;
        return y==rhs_y;
    }
    bool operator==(const tPoint& rhs) const
    {
        tMultipliedDistance x,y;
        getNumerators(x,y);
        tMultipliedDistance rhsX(rhs.getX());
        rhsX *= m_denominator;
        if(x != rhsX)
        {
            return false;
        }
        tMultipliedDistance rhsY(rhs.getY());
        rhsY *= m_denominator;
        return y == rhsY;
    }
    void operator-=(const tPoint& rhs)
    {
        m_line -= rhs;
    }
    void operator+=(const tPoint& rhs)
    {
        m_line += rhs;
    }

    bool operator!=(const tThis& rhs) const
    {
        return !((*this)==rhs);
    }
    tThis operator+(const tPoint& rhs) const
    {
        tThis result(*this);
        result+=rhs;
        return result;
    }
    template <class T>
    tThis operator-(const tPoint& rhs) const
    {
        tThis result(*this);
        result-=rhs;
        return result;
    }

    tPoint approximatePoint() const
    {
        typename tPoint::T range=0;
#ifdef RANGE_CHECKING
        range=m_line.start().getRange();
        if(m_line.end().getRange()>range)
            range=m_line.end().getRange();
#endif
        return tPoint::applyRatio(m_line.start(),m_line.axis(),m_numerator,m_denominator,range);
    }
    tPoint point() const
    {
        tPoint result=approximatePoint();
        assertD(*this==result);
        return result;
    }
//    bool isPoint() const
//    {
//        return *this==approximatePoint();
//    }
    bool isOnIntegerGrid(tPoint& p) const
    {
        p = approximatePoint();
        return *this == p;
    }

    // returns distance along line 1 as a fraction
    void getAxisFraction(typename tPoint::tDistance& n, typename tPoint::tDistance& d) const
    {
        n = m_numerator;
        d = m_denominator;
    }

    tPoint rounded(const typename tPoint::T& result_range,
                            bool& x_rounded, bool& y_rounded)
    {
        return tPoint::roundedAtRatioAlong(
                m_line.start(),m_line.end(),
                m_numerator,m_denominator,
                result_range,x_rounded,y_rounded);
    }


    void approximatePoints(
            tSigned32& result_points,
            tPoint* result_buffer,
            const typename tPoint::T& result_range
            ) const
    {
        m_line.axis().approximatePointsAtRatioAlong(
            m_numerator,m_denominator,
            result_points,result_buffer,result_range);

        tSigned32 i;
        for(i=0;i<result_points;i++)
            result_buffer[i]=tPoint(
                result_buffer[i].getX()+m_line.start().getX(),
                result_buffer[i].getY()+m_line.start().getY(),
                result_range);
    }


    void approximatePoints_IsOnAxis(
            tSigned32& resultPoints,
            tPoint* resultBuffer
            ) const
    {
        typename tPoint::T resultRange = m_line.start().getRange();
        if(m_line.end().getRange() > resultRange)
        {
            resultRange = m_line.end().getRange();
        }
        approximatePoints(resultPoints, resultBuffer, resultRange);
    }


    eSide sideOfLine(const cLine<tPoint> &base) const
    {
        assertD(!tMultiplier::isNegative(m_denominator));

        eSide side = m_line.side(base);
        if(side == SIDE_CENTRE)
            return base.side(m_line.start());
        tThis base_intersect(m_line,base);
        tMultipliedDistance base_distance, test_distance;
        tMultiplier::multiply(base_intersect.m_numerator, m_denominator, base_distance);
        tMultiplier::multiply(m_numerator, base_intersect.m_denominator, test_distance);
        
        // result is set up initially as if side were SIDE_RIGHT
        eSide result;
        if(test_distance < base_distance)
            result = SIDE_RIGHT;
        else if(test_distance > base_distance)
            result = SIDE_LEFT;
        else
            result = SIDE_CENTRE;

        if(side == SIDE_LEFT)
            result = OtherSide(result);

        return result;
    }

    void getNumerators(typename tPoint::tMultipliedDistance& x, typename tPoint::tMultipliedDistance& y) const
    {
        tPoint s,a;

        s=m_line.start();
        a=m_line.axis();

        typename tPoint::tMultipliedDistance tmp;

        tMultiplier::multiply(m_numerator, a.getX(), x);
        tMultiplier::multiply(m_denominator, s.getX(), tmp);
        x += tmp;

        tMultiplier::multiply(m_numerator, a.getY(), y);
        tMultiplier::multiply(m_denominator, s.getY(), tmp);
        y += tmp;
    }

    void getXAsFraction(
                typename tPoint::tMultipliedDistance& n,
                typename tPoint::tDistance& d) const
    {
        tPoint s,a;
        s=m_line.start();
        a=m_line.axis();
        typename tPoint::tMultipliedDistance tmp;
        tMultiplier::multiply(m_numerator, a.getX(), n);
        tMultiplier::multiply(m_denominator, s.getX(), tmp);
        n += tmp;
        d = m_denominator;
    }

    void getYAsFraction(
                typename tPoint::tMultipliedDistance& n,
                typename tPoint::tDistance& d) const
    {
        tPoint s,a;
        s=m_line.start();
        a=m_line.axis();
        typename tPoint::tMultipliedDistance tmp;
        tMultiplier::multiply(m_numerator, a.getY(), n);
        tMultiplier::multiply(m_denominator, s.getY(), tmp);
        n += tmp;
        d = m_denominator;
    }

// help make intersections look like iPointLike
    bool isEquivalentTo(const tPoint& p)
    {
        return *this == approximatePoint();
    }
};

template <class tPoint>
bool operator==(const tPoint& lhs, const cIntersection2<tPoint>& rhs)
{
    return rhs==lhs;
}

#endif
