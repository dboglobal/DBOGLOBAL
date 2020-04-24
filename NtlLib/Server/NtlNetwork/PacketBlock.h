#pragma once

#include "MemStream.h"
#include "NtlMutex.h"


class CTmpAttachBuffer
{
public:

	CTmpAttachBuffer()
	{
		memset(m_buffer, 0, sizeof(m_buffer));
	}

public:

	BYTE*		GetBuffer() { return m_buffer; }

private:

	BYTE		m_buffer[16380];
};


class CPacketBlock : public CMemStream
{
public:

	CPacketBlock();
	virtual ~CPacketBlock();

public:

	void				CreateBlock(DWORD dwSize);

	bool				IsShared();

	void				SetReference();

	bool				Release();

	bool				ReleaseRecyclic();


private:

	CNtlMutex			m_Access;
	
	long				m_nRefCount;

	CTmpAttachBuffer*	m_pTmpBuffer;

};