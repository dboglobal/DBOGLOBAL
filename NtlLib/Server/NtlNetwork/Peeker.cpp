#include "stdafx.h"
#include "Peeker.h"
#include "PacketBlock.h"


CPeeker::CPeeker()
{
	m_pBlock = NULL;
	m_pStart = NULL;
	m_pEnd = NULL;
}

CPeeker::CPeeker(CPacketBlock * pBlock)
{
	m_pBlock = pBlock;
	m_pStart = m_pBlock->GetFilledPos();
	m_pEnd = m_pStart;
}

CPeeker::~CPeeker()
{
}

void CPeeker::SetReference()
{
	m_pBlock->SetReference();
}

bool CPeeker::IsEmpty()
{
	assert(m_pBlock != NULL);

	return m_pBlock->IsEmptyBlock();
}

bool CPeeker::Release(DWORD & rdwBytesRemoved)
{
	DWORD dwSending = (DWORD)(m_pEnd - m_pStart);

	if (dwSending <= rdwBytesRemoved)
	{
		rdwBytesRemoved -= dwSending;
		m_pStart = m_pEnd;

		if (m_pBlock->GetEndPos() == m_pEnd)
			return Release();
	}
	else
	{
		m_pStart += rdwBytesRemoved;
		rdwBytesRemoved = 0;
	}

	return false;
}

bool CPeeker::Release()
{
	if (m_pBlock->Release() == false)
		return false;

	SAFE_DELETE(m_pBlock);

	return true;
}

bool CPeeker::ReleaseRecyclic(DWORD & rdwBytesRemoved)
{
	DWORD dwSending = (DWORD)(m_pEnd - m_pStart);

	if (dwSending <= rdwBytesRemoved)
	{
		rdwBytesRemoved -= dwSending;
		m_pStart = m_pEnd;

		if (m_pBlock->GetEndPos() == m_pEnd)
			return ReleaseRecyclic();
	}
	else
	{
		m_pStart += rdwBytesRemoved;
		rdwBytesRemoved = 0;
	}

	return false;
}

bool CPeeker::ReleaseRecyclic()
{
	if (m_pBlock->ReleaseRecyclic())
	{
		m_pStart = m_pBlock->GetFilledPos();
		m_pEnd = m_pStart;

		return false;
	}

	return true;
}

CPacketBlock * CPeeker::GetPacketBlock(DWORD dwRequiredSize)
{
	if (m_pBlock)
	{
		if (m_pBlock->IsShared() == false)
		{
			if (m_pBlock->IsEnough(dwRequiredSize))
				return m_pBlock;
		}
	}

	return NULL;
}

CPacketBlock * CPeeker::CreatePacketBlock(DWORD dwRequiredSize)
{
	m_pBlock = new CPacketBlock;

	m_pBlock->CreateBlock(dwRequiredSize);

	m_pStart = m_pBlock->GetFilledPos();
	m_pEnd = m_pStart;

	return m_pBlock;
}
//#include <iostream>
bool CPeeker::ReadyForSending(LPWSABUF wb)
{
	wb->buf = (char*)m_pStart;
	m_pEnd = m_pBlock->GetEndPos();
	wb->len = (ULONG)(m_pEnd - m_pStart);
	//printf("m_pEnd %I64u, m_pStart %i64u, m_pEnd %i,  m_pStart %i\n", sizeof(m_pEnd), sizeof(m_pStart), m_pEnd, m_pStart);

	return wb->len != 0;
}
