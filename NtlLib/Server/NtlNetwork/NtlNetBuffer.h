//***********************************************************************************
//
//	File		:	NtlNetBuffer.h
//
//	Begin		:	2006-08-25
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlPacket.h"
#include "NtlPacketEncoder.h"

#include "NtlCircularQueue.h"
#include "NtlError.h"
#include "NtlQueue.h"
#include "NtlDebug.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const unsigned int	DEF_PACKET_MAX_COUNT = 2000;
//-----------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
class CNtlNetBuffer : public CNtlCircularQueue<BYTE>
{
public:

	CNtlNetBuffer()
		:m_nMaxPacketSize(PACKET_MAX_SIZE) {}

public:

	bool			Create(int nPacketCapacity = DEF_PACKET_MAX_COUNT, int nMaxPacketSize = PACKET_MAX_SIZE) 
	{ 
		if( false == CNtlCircularQueue<BYTE>::Create( nMaxPacketSize *  nPacketCapacity, nMaxPacketSize ) )
		{
			return false;
		}
		m_nMaxPacketSize = nMaxPacketSize;

		return true;
	}

	bool			Resize(int nPacketCapacity = DEF_PACKET_MAX_COUNT, int nMaxPacketSize = PACKET_MAX_SIZE) 
	{
		if( false == CNtlCircularQueue<BYTE>::Realloc( nMaxPacketSize *  nPacketCapacity, nMaxPacketSize ) )
		{
			return false;
		}
		m_nMaxPacketSize = nMaxPacketSize;

		return true;
	}

public:
	BYTE *			InGetQueueBufferPtr() { return GetQueueBufferPtr(); }

	BYTE *			InGetQueueExtraPtr() { return GetQueueExtraPtr(); }

	DWORD			GetMaxPacketCount() { return GetQueueSize() / m_nMaxPacketSize; }

	int				GetMaxPacketSize() { return m_nMaxPacketSize; }
 
private:

	int				m_nMaxPacketSize;
};







class CNtlNetQueue : public CNtlQueue<CNtlPacket*>
{
public:

	CNtlNetQueue():m_nMaxCount(0) {}

	virtual ~CNtlNetQueue()
	{
		Destroy();
	}

	void Destroy()
	{
		CNtlPacket * pPacket = PopPacket();
		while( pPacket )
		{
			SAFE_DELETE( pPacket );

			pPacket = PopPacket();
		}
	}

	bool PushPacket(CNtlPacket* pPacket)
	{
		if( false == CNtlQueue<CNtlPacket*>::Push(pPacket) )
		{
			return false;
		}

		int nSize = GetSize();
		if( nSize > m_nMaxCount )
		{
			m_nMaxCount = nSize;
		}

		return true;
	}


	CNtlPacket* PeekPacket()
	{
		return CNtlQueue<CNtlPacket*>::Peek();
	}

	CNtlPacket* PopPacket()
	{
		return CNtlQueue<CNtlPacket*>::Pop();
	}


private:

	int				m_nMaxCount;

};

