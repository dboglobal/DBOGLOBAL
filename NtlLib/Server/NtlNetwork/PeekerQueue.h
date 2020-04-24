#pragma once

#include <deque>

#include "Peeker.h"

class CPeekerQueue : public std::deque<CPeeker>
{

	static const int c_DefaultBlockSize = 4096;
	static const int c_nDefaultLimit = 16380;

public:

	CPeekerQueue(UINT64 nBufferLimit);
	virtual ~CPeekerQueue();

public:

	void						Clear();

	bool						IsEmpty();

	void						SetBufferLimit(UINT64 nLimit);

	//GetMaxUsedSize;

	void						Add(CPacketBlock* pBlock);

	void						Add(CPeekerQueue* peek);

	CPacketBlock*				GetPacketBlock(DWORD dwRequiredSize);

	void						RemoveMsg(DWORD dwBytesRemoved);

	int							PeekBuf(LPWSABUF pWsaBuf, int nWsaBufCount, int& rnSendBufCount);

private:

	UINT64						m_nBufferLimit;

};