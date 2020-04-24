//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/SquareRoot.h"

class cPoint3
{
    float m_data[3];
public:
    cPoint3()
    {
    }
    template <class T>
    cPoint3(const T* ptr)
    {
        m_data[0]=ptr[0];
        m_data[1]=ptr[1];
        m_data[2]=ptr[2];
    }
    cPoint3(float x, float y, float z)
    {
        m_data[0]=x;
        m_data[1]=y;
        m_data[2]=z;
    }

    float x() const
    {
        return m_data[0];
    }
    float y() const
    {
        return m_data[1];
    }
    float z() const
    {
        return m_data[2];
    }

    float& x()
    {
        return m_data[0];
    }
    float& y()
    {
        return m_data[1];
    }
    float& z()
    {
        return m_data[2];
    }

//    void setX(float value)
//    {
//        m_data[0]=value;
//    }
//    void setY(float value)
//    {
//        m_data[1]=value;
//    }
//    void setZ(float value)
//    {
//        m_data[2]=value;
//    }


    void clear()
    {
        m_data[0]=0;
        m_data[1]=0;
        m_data[2]=0;
    }

    template <class T>
    void operator=(const T* ptr)
    {
        m_data[0]=ptr[0];
        m_data[1]=ptr[1];
        m_data[2]=ptr[2];
    }
    cPoint3 operator-(const cPoint3 &rhs) const
    {
        return cPoint3(
            m_data[0]-rhs.m_data[0],
            m_data[1]-rhs.m_data[1],
            m_data[2]-rhs.m_data[2]
            );
    }
    cPoint3 operator+(const cPoint3 &rhs) const
    {
        return cPoint3(
            m_data[0]+rhs.m_data[0],
            m_data[1]+rhs.m_data[1],
            m_data[2]+rhs.m_data[2]
            );
    }
    cPoint3 operator*(float rhs) const
    {
        return cPoint3(
            m_data[0]*rhs,
            m_data[1]*rhs,
            m_data[2]*rhs
            );
    }
    cPoint3 operator/(float rhs) const
    {
        return cPoint3(
            m_data[0]/rhs,
            m_data[1]/rhs,
            m_data[2]/rhs
            );
    }
    void operator-=(const cPoint3 &rhs)
    {
        m_data[0]-=rhs.m_data[0];
        m_data[1]-=rhs.m_data[1];
        m_data[2]-=rhs.m_data[2];
    }
    void operator+=(const cPoint3 &rhs)
    {
        m_data[0]+=rhs.m_data[0];
        m_data[1]+=rhs.m_data[1];
        m_data[2]+=rhs.m_data[2];
    }
    void operator*=(float rhs)
    {
        m_data[0]*=rhs;
        m_data[1]*=rhs;
        m_data[2]*=rhs;
    }
    void operator/=(float rhs)
    {
        m_data[0]/=rhs;
        m_data[1]/=rhs;
        m_data[2]/=rhs;
    }
    float lengthSquared() const
    {
        return
            m_data[0]*m_data[0]
            +
            m_data[1]*m_data[1]
            +
            m_data[2]*m_data[2]
            ;
    }
    float dotProduct(const cPoint3 &rhs) const
    {
        return
            m_data[0]*rhs.m_data[0]
            +
            m_data[1]*rhs.m_data[1]
            +
            m_data[2]*rhs.m_data[2]
            ;
    }
    void makeNormalTo(const cPoint3 &rhs)
    {
        float distalongrhs=dotProduct(rhs);
        float totaldist=rhs.lengthSquared();
        float ratio=distalongrhs/totaldist;
        m_data[0]-=rhs.m_data[0]*ratio;
        m_data[1]-=rhs.m_data[1]*ratio;
        m_data[2]-=rhs.m_data[2]*ratio;
    }
    void setLength(float length)
    {
        float ratio = length / SquareRoot(lengthSquared());
        m_data[0]*=ratio;
        m_data[1]*=ratio;
        m_data[2]*=ratio;
    }

    cPoint3 crossProduct(const cPoint3 &rhs)
    {
        return cPoint3(
            m_data[1]*rhs.m_data[2] - m_data[2]*rhs.m_data[1],
            m_data[2]*rhs.m_data[0] - m_data[0]*rhs.m_data[2],
            m_data[0]*rhs.m_data[1] - m_data[1]*rhs.m_data[0]
            );
    }
};

