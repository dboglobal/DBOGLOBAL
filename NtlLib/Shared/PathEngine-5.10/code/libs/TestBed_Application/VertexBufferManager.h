//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include <d3d9.h>
#include <vector>

const tSigned32 MAX_TRIS=40000;
const tSigned32 MAX_LINES=80000;

class cVertexBufferManager
{
    IDirect3DVertexBuffer9* m_tris_buffer;
    IDirect3DVertexBuffer9* m_lines_buffer;
    tSigned32 m_first_free_tri;
    std::vector<tSigned32> m_next_free_tri;
    tSigned32 m_first_free_line;
    std::vector<tSigned32> m_next_free_line;
    tSigned32 _trisAllocated;
    tSigned32 _linesAllocated;

public:
    cVertexBufferManager();
    ~cVertexBufferManager();

    tSigned32 allocateTri()
    {
        _trisAllocated++;
        tSigned32 result=m_first_free_tri;
        if(result!=-1)
            m_first_free_tri=m_next_free_tri[result];
        return result;
    }
    void freeTri(tSigned32 index)
    {
        _trisAllocated--;
        assertD(index >= 0 && index < SizeL(m_next_free_tri));
        m_next_free_tri[index]=m_first_free_tri;
        m_first_free_tri=index;
    }

    tSigned32 allocateLine()
    {
        _linesAllocated++;
        tSigned32 result=m_first_free_line;
        if(result!=-1)
            m_first_free_line=m_next_free_line[result];
        return result;
    }
    void freeLine(tSigned32 index)
    {
        _linesAllocated--;
        assertD(index >= 0 && index < SizeL(m_next_free_line));
        m_next_free_line[index]=m_first_free_line;
        m_first_free_line=index;
    }

    IDirect3DVertexBuffer9* getTrisBuffer()
    {
        return m_tris_buffer;
    }
    IDirect3DVertexBuffer9* getLinesBuffer()
    {
        return m_lines_buffer;
    }
};

