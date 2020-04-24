//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LINE_INCLUDED
#define LINE_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/SideEnum.h"
#include "libs/Geometry/interface/Point2.h"

template <class RangePolicy> class cIntersection2;

template <class tPoint>
class cLine
{
    typedef tPoint T;
    T m_start;
    T m_end;
    typedef cLine<tPoint> tThis;
    typedef cLine<tPoint> tCopy;
public:
    cLine()
    {
    }
    template <class T2, class T3>
    cLine(const T2& start, const T3& end)
    :m_start(start),m_end(end)
    {
    }

    const T& start() const
    {
        return m_start;
    }
    const T& end() const
    {
        return m_end;
    }

// operators

    template <class R2>
    bool operator==(const cLine<R2>& rhs) const
    {
        return m_start==rhs.m_start && m_end==rhs.m_end;
    }
    void operator-=(const tPoint& rhs)
    {
        m_start-=rhs;
        m_end-=rhs;
    }
    void operator+=(const tPoint& rhs)
    {
        m_start+=rhs;
        m_end+=rhs;
    }
    template <class SCALAR>
    void operator*=(const SCALAR& rhs)
    {
        m_start*=rhs;
        m_end*=rhs;
    }
    template <class SCALAR>
    void operator/=(const SCALAR& rhs)
    {
        m_start/=rhs;
        m_end/=rhs;
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
    tThis operator-(const tPoint& rhs) const
    {
        tThis result(*this);
        result-=rhs;
        return result;
    }
    template <class SCALAR>
    tThis operator*(const SCALAR& rhs) const
    {
        tThis result(*this);
        result*=rhs;
        return result;
    }
    template <class SCALAR>
    tThis operator/(const SCALAR& rhs) const
    {
        tThis result(*this);
        result/=rhs;
        return result;
    }

    tCopy operator-() const
    {
        return tCopy(-start(),-end());
    }

//------------------------

    tCopy left90() const
    {
        return tCopy(start(),start()+(axis().left90()));
    }
    tCopy right90() const
    {
        return tCopy(start(),start()+(axis().right90()));
    }
    void rotateLeft90()
    {
        m_end = start() + axis().left90();
    }
    void rotateRight90()
    {
        m_end = start() + axis().right90();
    }

    void reverse()
    {
        T temp=m_start;
        m_start=m_end;
        m_end=temp;
    }

    tCopy reversed() const
    {
        return tCopy(end(),start());
    }

    T axis() const
    {
        return end() - start();
    }
    T axisReversed() const
    {
        return start() - end();
    }

    eSide side(const tPoint& testpoint) const
    {
        return axis().side(testpoint-start());
    }
    eDirection direction(const tPoint& testpoint) const
    {
        return axis().direction(testpoint-start());
    }
    eSide side_WithInfinitesimals(const tPoint& testpoint, const tPoint& testpoint_iota, const tPoint& testpoint_iota2) const
    {
        return axis().side_WithInfinitesimals(testpoint-start(),testpoint_iota,testpoint_iota2);
    }
    eSide side_StdIotas(const tPoint& testpoint) const
    {
        return axis().side_StdIotas(testpoint-start());
    }
    eSide side_NegativeStdIotas(const tPoint& testpoint) const
    {
        return OtherSide((testpoint-start()).side_StdIotas(axis()));
    }

    eSide side(const tThis& testline) const
    {
        return axis().side(testline.axis());
    }
    eDirection direction(const tThis& testline) const
    {
        return axis().direction(testline.axis());
    }
    eDirection direction_KnownToBeInline(const tThis& rhs) const
    {
        return axis().direction_KnownToBeInline(rhs.axis());
    }

    // returns side of intersection with test relative to intersection with base
    // both lines must be to the right of this
    // all three lines must be non zero length
    // side returned is therefore in the sense of those lines
    eSide intersectionOrder_Clockwise(const tThis& base, const tThis& test) const
    {
        return axis().intersectionOrder_Clockwise(
            base.start()-start(), base.axis(),
            test.start()-start(), test.axis());
    }

    eSide intersectionOrder_Anticlockwise(const tThis& base, const tThis& test) const
    {
        return axis().intersectionOrder_Clockwise(
            base.end()-start(), base.axisReversed(),
            test.end()-start(), test.axisReversed());
    }

    eSide intersectionOrder_Clockwise_Anticlockwise(const tThis& base, const tThis& test) const
    {
        return axis().intersectionOrder_Clockwise(
            base.start()-start(), base.axis(),
            test.end()-start(), test.axisReversed());
    }

    T approximateIntersection(const tThis& rhs) const
    {
        assertD(side(rhs)!=SIDE_CENTRE); // lines must be non-parallel, also both non-zero

        T normal=axis().right90();
        typename T::tDistance sdist,edist;

        sdist=normal.dotProduct(rhs.start()-start());
        edist=normal.dotProduct(rhs.end()-start());

        // if sdist==edist then lines are parallel or one is zero length
        // so it should be impossible to trigger this assert
        assertD(sdist!=edist);

        typename T::T range=0;
#ifdef RANGE_CHECKING
        range=start().getRange();
        if(end().getRange()>range)
            range=end().getRange();
        if(rhs.start().getRange()>range)
            range=rhs.start().getRange();
        if(rhs.end().getRange()>range)
            range=rhs.end().getRange();
#endif
        typename T::tDistance diff(edist);
        diff -= sdist;
        return T::applyRatio(rhs.start(), rhs.axis(), -sdist, diff, range);
    }


    void intersectionFraction(
        const tThis& rhs,
        typename T::tDistance& numerator,
        typename T::tDistance& denominator
        ) const
    {
        assertD(side(rhs)!=SIDE_CENTRE); // lines must be non-parallel, also both non-zero

        T normal=rhs.axis().right90();
        typename T::tDistance sdist,edist;

        sdist=normal.dotProduct(start()-rhs.start());
        edist=normal.dotProduct(end()-rhs.start());

        // if sdist==edist then lines are parallel or one is zero length
        // so it should be impossible to trigger this assert
        assertD(sdist!=edist);

        numerator=-sdist;
        denominator=edist-sdist;
    }


    eSide intersectionOrder_Clockwise_WithInfinitesimals(
        const tThis& base,
        const tPoint& base_iota,
        const tPoint& base_iota2,
        const tThis& test) const
    {
        assertD(side(base)==SIDE_RIGHT);
        assertD(side(test)==SIDE_RIGHT);
        typename T::tDistance to_base_intersect;
        typename T::tDistance base_total;
        typename T::tDistance to_test_intersect;
        typename T::tDistance test_total;
        typename T::tMultipliedDistance to_base_intersect_normalised;
        typename T::tMultipliedDistance to_test_intersect_normalised;

// calulate distances for where axis crosses base

        to_base_intersect=base.axis().left90().dotProduct(base.start()-start());
        base_total=base.axis().left90().dotProduct(axis());

// calulate distances for where axis crosses test

        to_test_intersect=test.axis().left90().dotProduct(test.start()-start());
        test_total=test.axis().left90().dotProduct(axis());

// cross multiply to common metric

        to_base_intersect_normalised=to_base_intersect;
        to_base_intersect_normalised*=test_total;
        to_test_intersect_normalised=to_test_intersect;
        to_test_intersect_normalised*=base_total;

// determine which is first
// return corresponding code

        if(to_test_intersect_normalised==to_base_intersect_normalised)
        {
            typename T::tDistance iota_offset;
            iota_offset=base.axis().left90().dotProduct(base_iota);
            if(iota_offset==0)
                iota_offset=base.axis().left90().dotProduct(base_iota2);
            if(iota_offset==0)
                return SIDE_CENTRE;
            if(iota_offset>0)
                return SIDE_RIGHT;
            return SIDE_LEFT;
        }
        if(to_test_intersect_normalised<to_base_intersect_normalised)
            return SIDE_RIGHT;
        return SIDE_LEFT;
    }


    eSide side_Intersection(const tThis& line1, const tThis& line2) const
    {
        assertD(line1.side(line2)!=SIDE_CENTRE);

        eSide side_line1=side(line1);

        if(side_line1==SIDE_CENTRE)
            return side(line1.start());

        eSide side_line2=side(line2);

        if(side_line2==SIDE_CENTRE)
            return side(line2.start());

        tThis line1Copy(line1);
        if(side_line1!=SIDE_RIGHT)
        {
            line1Copy.reverse();
        }
        tThis line2Copy(line2);
        if(side_line2!=SIDE_RIGHT)
        {
            line2Copy.reverse();
        }

        eSide result = intersectionOrder_Clockwise(line1Copy, line2Copy);

        if(line1Copy.side(line2Copy) == SIDE_RIGHT)
        {
            result = OtherSide(result);
        }

        return result;
    }


    eSide side(const cIntersection2<tPoint>& testpoint) const
    {
//    //.. optimise - intersection guarantees that line2 is on right of line1
//        return side_Intersection(testpoint.refLine0(),testpoint.refLine1());
        return testpoint.sideOfLine(*this);
    }

    eDirection direction(const cIntersection2<tPoint>& p) const
    {
        eSide s = side(p);
        if(s == SIDE_LEFT)
        {
            return DIRECTION_LEFT;
        }
        if(s == SIDE_RIGHT)
        {
            return DIRECTION_RIGHT;
        }
        s = right90().side(p);
        if(s == SIDE_LEFT)
        {
            return DIRECTION_FORWARD;
        }
        if(s == SIDE_CENTRE)
        {
            return DIRECTION_NONE;
        }
        return DIRECTION_BACK;
    }
};


#endif
