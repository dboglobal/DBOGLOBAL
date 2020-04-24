//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef VERTEX_TYPES_INCLUDED
#define VERTEX_TYPES_INCLUDED
#include <d3d9.h>

struct LINEVERTEX
{
    D3DVECTOR position; // The position
};
#define D3DFVF_LINEVERTEX (D3DFVF_XYZ)

struct TEXTVERTEX
{
    D3DVECTOR position; // The position
    FLOAT       tu, tv;   // The texture coordinates
};
#define D3DFVF_TEXTVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

struct POLYGONVERTEX
{
    D3DVECTOR position; // The position
    D3DVECTOR normal;
    FLOAT       tu, tv;   // The texture coordinates
    FLOAT       tu2, tv2;   // The texture coordinates
};
#define D3DFVF_POLYGONVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2)

#endif
