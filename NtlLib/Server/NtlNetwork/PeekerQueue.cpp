#include "stdafx.h"
#include "PeekerQueue.h"
#include "assert.h"



CPeekerQueue::CPeekerQueue(UINT64 nBufferLimit)
{
	m_nBufferLimit = nBufferLimit;
}

CPeekerQueue::~CPeekerQueue()
{
	Clear();
}

void CPeekerQueue::Clear()
{
	CPeeker peeker;

	while (this->size())
	{
		peeker = this->front();
		peeker.Release();

		this->pop_front();
	}

	this->clear();
}

bool CPeekerQueue::IsEmpty()
{
	if (this->size() == 0)
		return true;

	if (this->size() == 1)
	{
		return this->front().IsEmpty();
	}

	return false;
}

void CPeekerQueue::SetBufferLimit(UINT64 nLimit)
{
	m_nBufferLimit = nLimit;
}

void CPeekerQueue::Add(CPacketBlock * pBlock)
{
	CPeeker peeker(pBlock);

	this->push_back(peeker);
}

void CPeekerQueue::Add(CPeekerQueue * peek)
{
	if (peek->IsEmpty() == false)
	{
		for (std::deque<CPeeker>::iterator it = this->begin(); it != this->end(); it++)
		{
			CPeeker& peeker = *it;

			peek->push_back(peeker);

			peeker.SetReference();
		}
	}
}

CPacketBlock * CPeekerQueue::GetPacketBlock(DWORD dwRequiredSize)
{
	CPeeker peeker;
	CPacketBlock* pBlock = NULL;

	if (this->size())
	{
		peeker = this->back();
		pBlock = peeker.GetPacketBlock(dwRequiredSize);
	}

	if (pBlock == NULL)
	{
		if (m_nBufferLimit && this->size() > m_nBufferLimit)
		{
			return NULL;
		}

		if (dwRequiredSize < c_nDefaultLimit)
			dwRequiredSize = c_nDefaultLimit;

		pBlock = peeker.CreatePacketBlock(dwRequiredSize);

		this->push_back(peeker);
	}

	return pBlock;
}

void CPeekerQueue::RemoveMsg(DWORD dwBytesRemoved)
{
	while (dwBytesRemoved > 0)
	{
		assert(size() > 0);

		CPeeker& peeker = this->front();

		if (this->size() == 1)
		{
			if (peeker.ReleaseRecyclic(dwBytesRemoved))
				this->pop_front();

			return;
		}

		if (peeker.Release(dwBytesRemoved))
			this->pop_front();
	}
}

int CPeekerQueue::PeekBuf(LPWSABUF pWsaBuf, int nWsaBufCount, int& rnSendBufCount)
{
	rnSendBufCount = 0;

	if (IsEmpty())
	{
		return 1;
	}

	for (std::deque<CPeeker>::iterator it = this->begin(); it != this->end(); it++)
	{
		CPeeker& peeker = *it;

		if (nWsaBufCount > rnSendBufCount)
		{
			if (peeker.ReadyForSending(pWsaBuf))
				++rnSendBufCount;
		}
	}

	return rnSendBufCount;
}
