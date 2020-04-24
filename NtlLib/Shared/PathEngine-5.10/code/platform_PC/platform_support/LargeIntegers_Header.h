//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LARGE_INTEGERS_HEADER_INCLUDED
#define LARGE_INTEGERS_HEADER_INCLUDED

class cLargeInteger2
{
public:

    tUnsigned32 digits[3];

    cLargeInteger2()
    {
    }
    explicit inline cLargeInteger2(tSigned32 value);
};

class cLargeInteger4
{
public:

    tUnsigned32 digits[5];

    cLargeInteger4()
    {
    }
    explicit inline cLargeInteger4(const cLargeInteger2& value);
    explicit inline cLargeInteger4(tSigned32 value);
};

#endif
