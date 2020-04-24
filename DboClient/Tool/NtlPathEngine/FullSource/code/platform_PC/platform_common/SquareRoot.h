//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SQUARE_ROOT_INCLUDED
#define SQUARE_ROOT_INCLUDED

inline __declspec(naked) float __fastcall
SquareRoot(float value)
{
    __asm
    {
        fld   DWORD PTR [esp+4]
        fsqrt
        ret 4
    }
}

#endif
