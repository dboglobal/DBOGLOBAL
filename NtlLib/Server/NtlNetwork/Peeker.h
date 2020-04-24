#pragma once


class CPacketBlock;

class CPeeker
{

	CPacketBlock*		m_pBlock;
	BYTE*				m_pStart;
	BYTE*				m_pEnd;

public:

	CPeeker();
	CPeeker(CPacketBlock* pBlock);
	virtual ~CPeeker();

public:

	void					SetReference();

	bool					IsEmpty();

	bool					Release(DWORD & rdwBytesRemoved);

	bool					Release();

	bool					ReleaseRecyclic(DWORD & rdwBytesRemoved);

	bool					ReleaseRecyclic();

	CPacketBlock*			GetPacketBlock(DWORD dwRequiredSize);

	CPacketBlock*			CreatePacketBlock(DWORD dwRequiredSize);

	bool					ReadyForSending(LPWSABUF wb);
};