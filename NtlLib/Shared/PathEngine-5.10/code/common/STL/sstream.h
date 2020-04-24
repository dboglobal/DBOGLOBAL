//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef REPLACE_STL
#include <sstream>
#else
#ifndef STL_SSTREAM_INCLUDED
#define STL_SSTREAM_INCLUDED

#include "common/STL/string.h"
#include "common/interface/Assert.h"

namespace std
{

class ostringstream
{
    typedef ostringstream tThis;

    string _str;

    tThis& put(const char* ptr)
    {
        while(*ptr)
        {
            put(*ptr++);
        }
        return *this;
    } 

public:

    string str() const
    {
        return _str;
    }

    tThis& put(char value)
    {
        _str.push_back(value);
        return *this;
    } 

    tThis& operator<<(short value) {return operator<<(static_cast<tSigned32>(value));}
    tThis& operator<<(unsigned short value) {return operator<<(static_cast<tUnsigned32>(value));}
    tThis& operator<<(int value) {return operator<<(static_cast<tSigned32>(value));}
    tThis& operator<<(unsigned int value) {return operator<<(static_cast<tUnsigned32>(value));}
    tThis& operator<<(float value) {return operator<<(static_cast<double>(value));}

    tThis& operator<<(tSigned32 value)
    {
        if(value == -2147483647L - 1)
        {
            put("-2147483648");
            return *this;
        }
        if(value < 0)
        {
            put('-');
            value = -value;
        }
        return operator<<(static_cast<tUnsigned32>(value));
    }
    tThis& operator<<(tUnsigned32 value)
    {
        if(value >= 10)
        {
            operator<<(value / 10);
        }
        char digit = static_cast<char>(value % 10);
        put('0' + digit);
        return *this;
    }
    tThis& operator<<(double value)
    {
        if(value == 0.0)
        {
            put('0');
            return *this;
        }

        union
        {
            tUnsigned32 ul;
            float f;
        } floatOrInt;

        floatOrInt.f = static_cast<float>(value);
        tUnsigned32 exponentUL = (floatOrInt.ul & 0x7f800000);
        exponentUL >>= 23;
        if(exponentUL == 255)
        {
            put("specialnumber");
            return *this;
        }
        if(exponentUL == 0)
        {
            put("denormalised");
            return *this;
        }

        if(value < 0.0)
        {
            put('-');
            value = -value;
        }

        int powerOfTen = 0;
        while(value >= 10.0)
        {
            value /= 10.0;
            powerOfTen++;
        }
        while(value < 1.0)
        {
            value *= 10.0;
            powerOfTen--;
        }

        floatOrInt.f = static_cast<float>(value);
        tUnsigned32 mantissa = (floatOrInt.ul & 0x007fffff);
        exponentUL = (floatOrInt.ul & 0x7f800000);
        exponentUL >>= 23;
        tSigned32 exponent = static_cast<tSigned32>(exponentUL);
        exponent -= 127;

        mantissa += 0x800000;
        tUnsigned32 digitMask = 0x7800000;
        int digitShift = 23;
        assertD(exponent >= 0 && exponent <= 3);
        digitMask >>= exponent;
        digitShift -= exponent;

        int digit = ((mantissa & digitMask) >> digitShift);
        assertD(digit >= 1 && digit < 10);
        put('0' + digit);
        mantissa -= (mantissa & digitMask);
        if(mantissa == 0)
        {
            return *this;
        }
        put('.');
        do
        {
            mantissa *= 10;
            int digit = ((mantissa & digitMask) >> digitShift);
            assertD(digit >= 0 && digit < 10);
            put('0' + digit);
            mantissa -= (mantissa & digitMask);
        }
        while(mantissa != 0);
        if(powerOfTen)
        {
            put('e');
            operator<<(powerOfTen);
        }
        return *this;
    }
//    tThis& operator<<(const void* value)
//    {
//    }
//    tThis& operator<<(bool value)
//    {
//    }

    tThis& operator<<(char value)
    {
        put(value);
        return *this;
    }

    tThis& operator<<(const string& value)
    {
        tUnsigned32 i;
        for(i = 0; i < value.size(); i++)
        {
            put(value[i]);
        }
        return *this;
    }
    tThis& operator<<(const char* ptr)
    {
        tUnsigned32 length = strlen(ptr);
        tUnsigned32 i;
        for(i = 0; i < length; i++)
        {
            put(ptr[i]);
        }
        return *this;
    }
};


}

#endif
#endif
