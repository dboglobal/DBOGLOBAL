//***********************************************************************************
//
//	File		:	NtlPacket.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network Packet Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlPacket.h"

#include "NtlLog.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket()
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket(CNtlPacket & rhs)
{
	Init();

	(*this) = rhs;
}

/*
///-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket(BYTE * pBuffer, int nSize)
{
InitUseInternalBuffer(pBuffer, nSize);
}
*/


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket(BYTE * pPacketData, WORD wPacketBodySize)
{
	Init();

	InitUseInternalBuffer( NULL, (WORD)GetHeaderSize() + wPacketBodySize );
	SetPacket( pPacketData, wPacketBodySize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket(WORD wPacketBodySize)
{
	Init();

	InitUseInternalBuffer( NULL, static_cast<WORD>(GetHeaderSize()) + wPacketBodySize );
	SetPacketLen( wPacketBodySize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::CNtlPacket(BYTE * pAttachBuffer)
{
	Init();

	Attach( pAttachBuffer );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket::~CNtlPacket()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::Init()
{
	m_pPacketBufferPtr		= NULL;
	m_pAllocateBuffer		= NULL;
	m_pPacketHeader			= NULL;
	m_pPacketData			= NULL;
	m_pEndPos				= NULL;
	m_pReadDataPos			= NULL;
	m_pWriteDataPos			= NULL;
	m_wBufferUsedSize		= 0;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::InitUseInternalBuffer(const BYTE * pPacketBuffer, WORD wBufferUsedSize)
{
	Realloc( wBufferUsedSize );

	m_pPacketBufferPtr = m_pAllocateBuffer;
	if( m_pPacketBufferPtr )
	{
		if( pPacketBuffer )
		{
			memcpy( m_pPacketBufferPtr, pPacketBuffer, wBufferUsedSize );
		}

		m_pPacketHeader			= (LPSTHeaderBase) &m_pPacketBufferPtr[0];
		m_pPacketData			= (BYTE*) &m_pPacketBufferPtr[GetHeaderSize()];
		m_pEndPos				= ( wBufferUsedSize >= 1 ) ? &m_pPacketBufferPtr[wBufferUsedSize - 1] : m_pPacketBufferPtr;
		m_pReadDataPos			= m_pWriteDataPos = m_pPacketData;
		m_wBufferUsedSize		= wBufferUsedSize;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::Destroy()
{
	m_pReadDataPos			= NULL;
	m_pWriteDataPos			= NULL;
	m_pEndPos				= NULL;
	m_pPacketData			= NULL;
	m_pPacketHeader			= NULL;
	m_pPacketBufferPtr		= NULL;
	m_wBufferUsedSize		= 0;

	SAFE_DELETE(m_pAllocateBuffer);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::InitUseExternalBuffer(BYTE * pPacketBuffer, WORD wBufferUsedSize)
{
	m_pPacketBufferPtr		= pPacketBuffer;

	m_pPacketHeader			= (LPSTHeaderBase) &m_pPacketBufferPtr[0];
	m_pPacketData			= (BYTE*) &m_pPacketBufferPtr[GetHeaderSize()];
	m_pEndPos				= ( wBufferUsedSize >= 1 ) ? &m_pPacketBufferPtr[wBufferUsedSize - 1] : m_pPacketBufferPtr;

	m_pReadDataPos			= m_pWriteDataPos = m_pPacketData;	
	m_wBufferUsedSize		= wBufferUsedSize;	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::AttachData(BYTE * pPacketBuffer, WORD wBufferUsedSize)
{
	InitUseExternalBuffer( pPacketBuffer, wBufferUsedSize );
}


//-----------------------------------------------------------------------------------
//		Purpose	: 완전한 패킷 버퍼를 인자로 호출할 것
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::Attach(BYTE * pPacketBuffer)
{
	LPSTHeaderBase pPacketHeader = (LPSTHeaderBase) &pPacketBuffer[0];

	WORD wPacketTotalSize = pPacketHeader->wPacketLen + (WORD)GetHeaderSize();

	InitUseExternalBuffer( pPacketBuffer, wPacketTotalSize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::Realloc(int nSize)
{
	SAFE_FREE( m_pAllocateBuffer );

	m_pAllocateBuffer = (BYTE*) calloc( nSize, sizeof(BYTE) );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlPacket::IsValidHeader()
{
	if( NULL == m_pPacketBufferPtr )
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return false;
	}


	if( GetUsedSize() < GetHeaderSize() )
	{
		_ASSERT( GetUsedSize() >= GetHeaderSize() );
		return false;
	}
	

	return true;	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlPacket::IsValidPacket()
{
	if( NULL == m_pPacketBufferPtr )
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return false;
	}

	if( false == IsValidHeader() )
	{
		_ASSERT( true == IsValidHeader() );
		return false;
	}

	if( GetUsedSize() < GetPacketLen() )
	{
		_ASSERT( GetUsedSize() >= GetPacketLen() );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::CopyToBuffer(BYTE * pDestBuffer, int nSize)
{
	if( NULL == m_pPacketBufferPtr )
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return;
	}

	memcpy( pDestBuffer, m_pPacketBufferPtr, nSize) ;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::CopyFromBuffer(BYTE * pSrcBuffer, int nSize)
{
	InitUseInternalBuffer( pSrcBuffer, (BYTE)nSize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::ReadData(void * pDest, int nSize)
{
	if( NULL == m_pPacketBufferPtr )
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return;
	}

	if( m_pReadDataPos + nSize > m_pEndPos || 
		m_pReadDataPos + nSize > m_pPacketData + GetPacketDataSize() )
	{
		return;
	}

	memcpy( pDest, m_pReadDataPos, nSize );
	m_pReadDataPos += nSize;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::WriteData(void * pSrc, int nSize)
{
	if( NULL == m_pPacketBufferPtr )
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return;
	}

	if( m_pWriteDataPos + nSize > m_pEndPos )
	{
		return;
	}

	memcpy( m_pWriteDataPos, pSrc, nSize );

	m_pWriteDataPos += nSize;
	m_wBufferUsedSize = m_wBufferUsedSize + (WORD)nSize;
	SetPacketLen( GetPacketLen() + (WORD)nSize );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::SetPacket(BYTE * pPacketData, WORD wPacketBodySize)
{
	if( NULL == m_pPacketBufferPtr)
	{
		_ASSERT( NULL != m_pPacketBufferPtr );
		return;
	}

	if( wPacketBodySize > GetUsedSize() - GetHeaderSize() )
	{
		return;
	}

	SetPacketLen( wPacketBodySize );
	memcpy( m_pPacketData, pPacketData, wPacketBodySize );

	m_pWriteDataPos		= m_pPacketData + wPacketBodySize;
	m_wBufferUsedSize	= wPacketBodySize + (WORD)GetHeaderSize();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPacket::SetPacketLen(WORD wPacketLen)
{
	if( wPacketLen > PACKET_LIMIT_SIZE )
	{
		_ASSERT( wPacketLen <= PACKET_LIMIT_SIZE );
		m_pPacketHeader->wPacketLen = PACKET_LIMIT_SIZE;
		return;
	}

	m_pPacketHeader->wPacketLen = wPacketLen;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator =( CNtlPacket & rhs)
{
	InitUseInternalBuffer( rhs.m_pPacketBufferPtr, rhs.GetUsedSize() );

	size_t wReadPos = rhs.m_pReadDataPos - rhs.m_pPacketData;
	size_t wWritePos = rhs.m_pWriteDataPos - rhs.m_pPacketData;

	m_pReadDataPos = m_pPacketData + wReadPos;
	m_pWriteDataPos = m_pPacketData + wWritePos;

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator <<( CNtlPacket& rhs )
{
	*this = rhs;
	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator >>( CNtlPacket& rhs )
{
	rhs = *this;
	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator <<( LPTSTR arg )
{
	WriteData( (void *) arg, (int) strlen( arg ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator >>( LPTSTR arg )
{
	ReadData( (void*) arg, (int) strlen( ( LPTSTR ) m_pReadDataPos ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator <<( bool arg )
{
	WriteData( &arg, sizeof( bool ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator >>( bool& arg )
{
	ReadData( &arg, sizeof( bool ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator <<( int arg )
{
	WriteData( &arg, sizeof( int ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator >>( int& arg )
{
	ReadData( &arg, sizeof( int ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator <<( long arg )
{
	WriteData( &arg, sizeof( long ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket& CNtlPacket::operator >>( long& arg )
{
	ReadData( &arg, sizeof( long ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket& CNtlPacket::operator <<( DWORD arg )
{
	WriteData( &arg, sizeof( DWORD ) );

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacket & CNtlPacket::operator >>( DWORD& arg )
{
	ReadData( &arg, sizeof( DWORD ) );

	return *this;
}
