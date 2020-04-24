#include "stdafx.h"
#include "MemStream.h"
#include "assert.h"


CMemStream::CMemStream()
{
	m_pMemBlock = NULL;
	m_pMemBlockEnd = NULL;
	m_pDataPosStart = NULL;
	m_pDataPosEnd = NULL;
}

void CMemStream::Attach(BYTE * pBlock, int nBlockSize)
{
	m_pMemBlock = pBlock;
	m_pDataPosStart = pBlock;
	m_pDataPosEnd = pBlock;
	m_pMemBlockEnd = &m_pMemBlock[nBlockSize];
}

BYTE * CMemStream::Detach()
{
	BYTE* pMemBlock = m_pMemBlock;

	m_pMemBlockEnd = NULL;
	m_pMemBlock = NULL;
	m_pDataPosEnd = NULL;
	m_pDataPosStart = NULL;

	return pMemBlock;
}

void CMemStream::Reset()
{
	assert(m_pMemBlock != NULL);

	m_pDataPosStart = m_pMemBlock;
	m_pDataPosEnd = m_pDataPosStart;
}

BYTE * CMemStream::GetFilledPos()
{
	return m_pDataPosStart;
}

BYTE * CMemStream::GetEndPos()
{
	return m_pDataPosEnd;
}

bool CMemStream::IsEmptyBlock()
{
	return m_pDataPosEnd == m_pDataPosStart;
}
//#include <iostream>
CMemStream & CMemStream::Push(BYTE * p, DWORD dwSize)
{
	memcpy(m_pDataPosEnd, p, dwSize);

	m_pDataPosEnd += dwSize;
//	printf("CMemStream::Push: %I64u, %i %lu \n", sizeof(m_pDataPosEnd), m_pDataPosEnd, dwSize);
	return *this;
}

bool CMemStream::IsEnough(DWORD dwSize)
{
	return &m_pDataPosEnd[dwSize] <= m_pMemBlockEnd;
}
