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
#include "VertexBufferManager.h"
#include "VertexTypes.h"
#include "D3D.h"
#include "D3DErrors.h"
#include <windows.h>

//#include "globals.h"

cVertexBufferManager::cVertexBufferManager()
{
    tSigned32 tris=MAX_TRIS;
    tSigned32 lines=MAX_LINES;
    HRESULT result;

// allocate tri vertex buffer

    result = cD3D::device()->CreateVertexBuffer(
            sizeof(POLYGONVERTEX)*tris*3,
            D3DUSAGE_WRITEONLY,
            D3DFVF_POLYGONVERTEX,
            D3DPOOL_DEFAULT,
            &m_tris_buffer,
			0
			);

    // in the case of insufficient memory half the number of tris until success
    while(result==D3DERR_OUTOFVIDEOMEMORY || result==E_OUTOFMEMORY && tris/2>0)
    {
        tris/=2;
        result = cD3D::device()->CreateVertexBuffer(
                sizeof(POLYGONVERTEX)*tris*3,
                D3DUSAGE_WRITEONLY,
                D3DFVF_POLYGONVERTEX,
                D3DPOOL_DEFAULT,
                &m_tris_buffer,
				0
				);
    }
    if(D3D_OK!=result)
    {
        MessageBox (NULL, "Failed to create vertex buffer for tris", "Warning: render dll.", MB_OK);
        MessageBox (NULL, d3derrstring(result), "D3D error:", MB_OK);
        m_tris_buffer=0;
        tris=0;
    }

// allocate lines vertex buffer

    result = cD3D::device()->CreateVertexBuffer(
            sizeof(LINEVERTEX)*lines*2,
            D3DUSAGE_WRITEONLY,
            D3DFVF_LINEVERTEX,
            D3DPOOL_DEFAULT,
            &m_lines_buffer,
			0
			);

    // in the case of insufficient memory half the number of lines until success
    while(result==D3DERR_OUTOFVIDEOMEMORY || result==E_OUTOFMEMORY && lines/2>0)
    {
        lines/=2;
        result = cD3D::device()->CreateVertexBuffer(
                sizeof(LINEVERTEX)*lines*2,
                D3DUSAGE_WRITEONLY,
                D3DFVF_LINEVERTEX,
                D3DPOOL_DEFAULT,
                &m_lines_buffer,
				0
				);
    }
    if(D3D_OK!=result)
    {
        MessageBox (NULL, "Failed to create vertex buffer for lines", "Warning: render dll.", MB_OK);
        MessageBox (NULL, d3derrstring(result), "D3D error:", MB_OK);
        m_lines_buffer=0;
        lines=0;
    }

// initialise next free tables

    m_next_free_tri.resize(tris);
    m_next_free_line.resize(lines);
    {
        tSigned32 i;
        for(i=0;i<tris;i++)
            m_next_free_tri[i]=i+1;
        for(i=0;i<lines;i++)
            m_next_free_line[i]=i+1;
    }
    if(tris==0)
        m_first_free_tri=-1;
    else
    {
        m_first_free_tri=0;
        m_next_free_tri[tris-1]=-1;
    }
    if(lines==0)
        m_first_free_line=-1;
    else
    {
        m_first_free_line=0;
        m_next_free_line[lines-1]=-1;
    }

    _trisAllocated = 0;
    _linesAllocated = 0;
}

cVertexBufferManager::~cVertexBufferManager()
{
//..... sort out DX initialisation and deinitialisation so that PathEngine is always shutdown before this stuff is deleted
//    assertD(_trisAllocated == 0);
//    assertD(_linesAllocated == 0);
    if(m_tris_buffer)
        m_tris_buffer->Release();
    if(m_lines_buffer)
        m_lines_buffer->Release();
}

