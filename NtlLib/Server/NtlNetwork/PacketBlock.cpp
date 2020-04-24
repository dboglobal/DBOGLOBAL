#include "stdafx.h"
#include "PacketBlock.h"




CPacketBlock::CPacketBlock()
{
	m_nRefCount = 0;
	m_pTmpBuffer = NULL;
}

CPacketBlock::~CPacketBlock()
{
}
#include <iostream>
void CPacketBlock::CreateBlock(DWORD dwSize)
{
	CNtlLock lock(&m_Access);

	++m_nRefCount;

	if (dwSize > 16380)
	{
		BYTE* pBlock = new BYTE[dwSize];
		Attach(pBlock, dwSize);
	}
	else
	{
		if (m_pTmpBuffer == NULL)
		{
			m_pTmpBuffer = new CTmpAttachBuffer;
		}

		Attach(m_pTmpBuffer->GetBuffer(), dwSize);
	}
}

bool CPacketBlock::IsShared()
{
	return m_nRefCount > 1;
}

void CPacketBlock::SetReference()
{
	CNtlLock lock(&m_Access);

	++m_nRefCount;
}

bool CPacketBlock::Release()
{
	CNtlLock lock(&m_Access);

	assert(m_nRefCount > 0);

	if (--m_nRefCount == 0)
	{
		BYTE* pBuffer = Detach();

		if (m_pTmpBuffer)
		{
			SAFE_DELETE(m_pTmpBuffer);
		}
		else if (pBuffer)
		{
			delete pBuffer;
		}
	}

	return m_nRefCount == 0;
}

bool CPacketBlock::ReleaseRecyclic()
{
	CNtlLock lock(&m_Access);

	assert(m_nRefCount > 0);

	if (m_nRefCount == 1)
	{
		Reset();

		return true;
	}

	--m_nRefCount;
	return false;
}
