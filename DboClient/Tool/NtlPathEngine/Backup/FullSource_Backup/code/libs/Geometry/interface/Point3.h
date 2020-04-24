//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

//... TODO - figure out a *nice* way to eliminate repetition of code

#ifndef POINT3_INCLUDED
#define POINT3_INCLUDED

#include "common/interface/Assert.h"
#include "libs/Geometry/interface/SideEnum.h"

template <class TRAITS>
class cUncheckedPoint3
{
public:

    typedef typename TRAITS::tMultiplier tMultiplier;
    typedef typename TRAITS::tDistance tDistance;
    typedef typename TRAITS::tMultipliedDistance tMultipliedDistance;
    typedef typename TRAITS::T T;

protected:

    T data[3];
    typedef cUncheckedPoint3<TRAITS> tThis;

public:

// can be constructed uninitialised
    cUncheckedPoint3()
    {
    }

// can be constructed from x,y,range
    cUncheckedPoint3(const T& x, const T& y, const T& z, const T& range)
    {
        data[0]=x;
        data[1]=y;
        data[2]=z;
    }

// can be copy constructed
    cUncheckedPoint3(const tThis& rhs)
    {
        data[0]=rhs.getX();
        data[1]=rhs.getY();
        data[2]=rhs.getZ();
    }

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

//    template <class R2>
//    static tThis static_cast_from(const cPoint3<R2>& rhs)
//    {
//        return tThis(static_cast<T>(rhs.getX()),static_cast<T>(rhs.getY()),0);
//    }

    const T& getX() const
    {
        return data[0];
    }
    const T& getY() const
    {
        return data[1];
    }
    const T& getZ() const
    {
        return data[2];
    }
    const T* getPtr() const
    {
        return data;
    }

// operators

    bool operator==(const tThis& rhs) const
    {
        return getX()==rhs.getX() && getY()==rhs.getY() && getZ() == rhs.getZ();
    }
    void operator-=(const tThis& rhs)
    {
        data[0]-=rhs.getX();
        data[1]-=rhs.getY();
        data[2]-=rhs.getZ();
    }
    void operator+=(const tThis& rhs)
    {
        data[0]+=rhs.getX();
        data[1]+=rhs.getY();
        data[2]+=rhs.getZ();
    }

    void operator*=(tSigned32 rhs)
    {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
    }
//    template <class SCALAR>
//    void operator*=(const SCALAR& rhs)
//    {
//        data[0]*=rhs;
//        data[1]*=rhs;
//        data[2]*=rhs;
//    }
//    template <class SCALAR>
//    void operator/=(const SCALAR& rhs)
//    {
//        data[0]/=rhs;
//        data[1]/=rhs;
//        data[2]/=rhs;
//    }

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
    template <class T>
    tThis operator*(const T& rhs) const
    {
        tThis result(*this);
        result*=rhs;
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
        data[2]=-data[2];
    }

    tThis operator-() const
    {
        tThis result=*this;
        result.reverse();
        return result;
    }


//------------------------

// distance stuff


    template <class P2>
    tDistance dotProduct(const P2& rhs) const
    {
        tDistance result1,result2;
        tMultiplier::multiply(getX(), rhs.getX(), result1);
        tMultiplier::multiply(getY(), rhs.getY(), result2);
        result1 += result2;
        tMultiplier::multiply(getZ(), rhs.getZ(), result2);
        result1 += result2;
        return result1;
    }
    tDistance lengthSquared() const
    {
        return dotProduct(*this);
    }

    tThis crossProduct(const tThis& rhs) const
    {
        return tThis(
            getY()*rhs.getZ() - getZ()*rhs.getY(),
            getZ()*rhs.getX() - getX()*rhs.getZ(),
            getX()*rhs.getY() - getY()*rhs.getX(),
            getRange()*rhs.getRange()*2
            );
    }

    bool inLine(const tThis& rhs) const
    {
        tDistance d1,d2;
        tMultiplier::multiply(getX(), rhs.getY(), d1);
        tMultiplier::multiply(rhs.getX(), getY(), d2);
        if(d1!=d2)
            return false;
        tMultiplier::multiply(getX(), rhs.getZ(), d1);
        tMultiplier::multiply(rhs.getX(), getZ(), d2);
        if(d1!=d2)
            return false;
        tMultiplier::multiply(getY(), rhs.getZ(), d1);
        tMultiplier::multiply(rhs.getY(), getZ(), d2);
        if(d1!=d2)
            return false;
        return true;
    }

    eDirection direction_KnownToBeInline(const tThis& rhs) const
    {
        if(getX())
        {
            assertD(rhs.getX());
            if((getX()>0)==(rhs.getX()>0))
                return DIRECTION_FORWARD;
            return DIRECTION_BACK;
        }
        if(getY())
        {
            assertD(rhs.getY());
            if((getY()>0)==(rhs.getY()>0))
                return DIRECTION_FORWARD;
            return DIRECTION_BACK;
        }
        assertD(getZ());
        assertD(rhs.getZ());
        if((getZ()>0)==(rhs.getZ()>0))
            return DIRECTION_FORWARD;
        return DIRECTION_BACK;
    }

//    void applyRatio(const tDistance& n, const tDistance& d)
//    {
//        tMultipliedDistance multiplied;
//        multiplied=n;
//        multiplied*=x;
//        x=multiplied/d;
//        multiplied=n;
//        multiplied*=y;
//        y=multiplied/d;
//        multiplied=n;
//        multiplied*=z;
//        z=multiplied/d;
//    }

    void updateAsMinimumBound(const tThis& rhs)
    {
        if(rhs.getX()<getX())
            data[0]=rhs.getX();
        if(rhs.getY()<getY())
            data[1]=rhs.getY();
        if(rhs.getZ()<getZ())
            data[2]=rhs.getZ();
    }
    void updateAsMaximumBound(const tThis& rhs)
    {
        if(rhs.getX()>getX())
            data[0]=rhs.getX();
        if(rhs.getY()>getY())
            data[1]=rhs.getY();
        if(rhs.getZ()>getZ())
            data[2]=rhs.getZ();
    }

//    bool insideOrOnBounds(const tThis& min, const tThis& max) const
//    {
//        if(x<min.x)
//            return false;
//        if(x>max.x)
//            return false;
//        if(y<min.y)
//            return false;
//        if(y>max.y)
//            return false;
//        if(z<min.z)
//            return false;
//        if(z>max.z)
//            return false;
//        return true;
//    }

    T getRangeUsed()
    {
        T largest=getX();
        if(getX()<0)
            largest=-largest;
        T candidate=getY();
        if(getY()<0)
            candidate=-candidate;
        if(candidate>largest)
            largest=candidate;
        candidate=getZ();
        if(getZ()<0)
            candidate=-candidate;
        if(candidate>largest)
            largest=candidate;
        return largest;
    }

    // touching is not considered to be overlapping
    // so 0,0,0->1,1,1 does not overlap 1,0,0->2,1,1
    static bool boundsOverlap(const tThis *bounds1, const tThis *bounds2)
    {
    // if the two volumes can be seperated on any axis then they do not overlap
        if(bounds2[0].getX()>=bounds1[1].getX())
            return false;
        if(bounds2[0].getY()>=bounds1[1].getY())
            return false;
        if(bounds2[0].getZ()>=bounds1[1].getZ())
            return false;
        if(bounds1[0].getX()>=bounds2[1].getX())
            return false;
        if(bounds1[0].getY()>=bounds2[1].getY())
            return false;
        if(bounds1[0].getZ()>=bounds2[1].getZ())
            return false;
    // otherwise they do
        return true;
    }

    bool
    operator<(const tThis& rhs) const
    {
        if(data[0] < rhs.data[0])
        {
            return true;
        }
        if(data[0] > rhs.data[0])
        {
            return false;
        }
        if(data[1] < rhs.data[1])
        {
            return true;
        }
        if(data[1] > rhs.data[1])
        {
            return false;
        }
        if(data[2] < rhs.data[2])
        {
            return true;
        }
        return false;
    }
};



template <class TRAITS>
class cCheckedPoint3 : public cUncheckedPoint3<TRAITS>
{
public:

    typedef cUncheckedPoint3<TRAITS> tParent;
    typedef typename TRAITS::tMultiplier tMultiplier;
    typedef typename TRAITS::tDistance tDistance;
    typedef typename TRAITS::tMultipliedDistance tMultipliedDistance;
    typedef typename TRAITS::T T;

private:

    T range;
    typedef cCheckedPoint3<TRAITS> tThis;
    void setRangeToMax(const tThis& rhs)
    {
        if(rhs.range > range)
        {
            range = rhs.range;
        }
    }

public:

    cCheckedPoint3()
    {
    }
    cCheckedPoint3(const T& x, const T& y, const T& z, const T& range) :
        cUncheckedPoint3<TRAITS>(x, y, z, 0),
        range(range)
    {
        TRAITS::checkRange(range);
        TRAITS::checkInRange(range, this->getX());
        TRAITS::checkInRange(range, this->getY());
        TRAITS::checkInRange(range, this->getZ());
    }
    cCheckedPoint3(const tThis& rhs) :
     cUncheckedPoint3<TRAITS>(rhs),
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
        TRAITS::checkInRange(range, this->getZ());
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
//    template <class SCALAR>
//    void operator*=(const SCALAR& rhs)
//    {
//        TRAITS::multiplyRange(range, rhs);
//        tParent::operator*=(rhs);
//    }
//    template <class SCALAR>
//    void operator/=(const SCALAR& rhs)
//    {
//        TRAITS::divideRange(range, rhs);
//        tParent::operator/=(rhs);
//    }

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
    template <class T>
    tThis operator*(const T& rhs) const
    {
        tThis result(*this);
        result*=rhs;
        return result;
    }
    template <class T>
    tThis operator/(const T& rhs) const
    {
        tThis result(*this);
        result/=rhs;
        return result;
    }

    tThis crossProduct(const tThis& rhs) const
    {
        TRAITS::checkRangeMultiply(getRange(),rhs.getRange());
        TRAITS::checkRangeMultiply(getRange()*rhs.getRange(),2);
        return tThis(
            this->getY()*rhs.getZ() - this->getZ()*rhs.getY(),
            this->getZ()*rhs.getX() - this->getX()*rhs.getZ(),
            this->getX()*rhs.getY() - this->getY()*rhs.getX(),
            getRange()*rhs.getRange()*2
            );
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
};


#endif
