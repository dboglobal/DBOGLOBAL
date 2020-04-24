//***********************************************************************************
//
//	File		:	NtlConnection.h
//
//	Begin		:	2005-12-19
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network IO Connection Class
//
//***********************************************************************************

#pragma once


#include "NtlBitFlag.h"
#include "NtlIOCP.h"
#include "NtlSocket.h"
#include "NtlNetBuffer.h"
#include "NtlPacketEncoder.h"

#include "NtlMutex.h"

#include "PeekerQueue.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const DWORD CONNECTION_KEEP_ALIVE_CHECK_TIME	= 15 * 1000; // 
//-----------------------------------------------------------------------------------


class CNtlNetwork;
class CNtlSession;
class CNtlAcceptor;
class CNtlConnector;


class CNtlConnection
{
	friend class CIocpWorkerThread;
	friend class CNtlSession;

	const DWORD c_Auth_TimeOver = 600000;
	const DWORD c_ConnectionFlooding = 60000;

public:

	enum eCONTROLFLAG
	{
		CONTROL_FLAG_DO_ALIVE = 0x01 << 0,
		CONTROL_FLAG_CHECK_ALIVE = 0x01 << 1,
		CONTROL_FLAG_CHECK_OPCODE = 0x01 << 2,
		CONTROL_FLAG_USE_AUTH = 0x01 << 3,
		CONTROL_FLAG_LIMITED_RECV_SIZE = 0x01 << 4,

		MAX_CONTROL_FLAG
	};


	enum eSTATUS					
	{
		STATUS_INIT = 0,			// 초기상태
		STATUS_CREATE,				// 생성상태
		STATUS_ACCEPT,				// Accept 대기 상태
		STATUS_CONNECT,				// Connect 대기 상태 ( AcceptEx, ConnectEx 에서 사용 )
		STATUS_ACTIVE,				// 활동상태
		STATUS_CLOSE,				// 닫힌 상태
		STATUS_SHUTDOWN,			// 종료 상태
		STATUS_DESTROY,				// 소멸 상태
		STATUS_CHANGE_PORT,

		MAX_STATUS
	};


public:

	CNtlConnection();

	virtual ~CNtlConnection();

public:

	int									Create(CNtlNetwork * pNetwork);

	bool								Create(CNtlSocket& rSocket, CNtlSockAddr& rSockAddr, int nPacketCount, int nPacketSize);

public:


	int									CompleteIO(sIOCONTEXT * pIOContext, DWORD dwParam);

	void								Close(bool bForce);




public:

	virtual DWORD						GetMaxRecvPacketSize() { return PACKET_MAX_SIZE; }

	virtual DWORD						GetMaxSendPacketSize() { return PACKET_MAX_SIZE; }

	virtual DWORD						GetMaxRecvPacketCount() { return DEF_PACKET_MAX_COUNT; }

	virtual DWORD						GetMaxSendPacketCount() { return DEF_PACKET_MAX_COUNT; }

	virtual DWORD						GetAliveCheckTime() { return CONNECTION_KEEP_ALIVE_CHECK_TIME; }

	void								ResetAliveTime() { m_dwAliveTime = 0; }


public:

	int									Disconnect(bool bGraceful);

	int									CheckDisconnect(bool bGraceful);

	bool								ValidCheck(DWORD dwTickDiff);

	bool								PacketLogTime(DWORD dwTickDiff);

	bool								IsShutdownable();

	bool								CanbeDestroy() { return IsStatus(STATUS_DESTROY); }

	int									Shutdown();


public:

	int									RecvPackets(DWORD dwTransferedBytes);

private:

	int									PostSend_Ex();

	int									AddToSendBuffer(CNtlPacket* pPacket);

public:


	int									PostRecv();

	int									PostSend();	

	int									PostAccept(CNtlAcceptor* pAcceptor);

	int									PostConnect(CNtlConnector* pConnector);


	int									CompleteRecv(DWORD dwTransferedBytes);

	int									CompleteSend(DWORD dwTransferedBytes);

	int									CompleteSend_Ex(DWORD dwTransferedBytes);

	int									CompleteAccept(DWORD dwTransferedBytes);

	int									CompleteConnect(DWORD dwTransferedBytes);

	int									SendPacket(CNtlPacket * pPacket);

	int									PostPacket(CNtlPacket * pPacket);

	//int									Push(void* pData, DWORD dwSize);

	int									PushPacket(CNtlPacket * pPacket);

	bool								PopPacket(CNtlPacket * pPacket);

	virtual bool						IsValidPacket(void * pvPacketHeader, WORD wPacketLength);

	//virtual WORD						GetSequence(void * pvPacketData);


	


public:


	sIOCONTEXT *						GetRecvContext() { return &m_recvContext; }

	sIOCONTEXT *						GetSendContext() { return &m_sendContext; }

	CNtlNetBuffer *						GetRecvBuffer() { return &m_recvBuffer; }

	CNtlNetBuffer *						GetSendBuffer() { return &m_sendBuffer; }

	CNtlSocket &						GetSocket() { return m_socket; }

	CNtlSockAddr &						GetRemoteAddr() { return m_remoteAddr; }

	CNtlSockAddr &						GetLocalAddr() { return m_localAddr; }

	void								SetAddress(SOCKADDR_IN * pLocalAddr, SOCKADDR_IN * pRemoteAddr);

	const char *						GetRemoteIP() { return m_remoteAddr.GetDottedAddr(); }

	WORD								GetRemotePort() { return m_remoteAddr.GetPort(); }

	const char *						GetLocalIP() { return m_localAddr.GetDottedAddr(); }

	WORD								GetLocalPort() { return m_localAddr.GetPort(); }

	bool								GetAddressInfo(CNtlString * pString, WORD * pPort, bool bRemote /* = true */);

	CNtlConnector *						GetConnector() { return m_pConnectorRef; }

	CNtlAcceptor *						GetAcceptor() { return m_pAcceptorRef; }

	int									MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength);
										
public:

	void								SetControlFlag(DWORD dwControlFlag) { BIT_FLAG_SET( m_dwControlFlag, dwControlFlag); }

	void								UnsetControlFlag(DWORD dwControlFlag) { BIT_FLAG_UNSET( m_dwControlFlag, dwControlFlag); }

	bool								IsSetControlFlag(DWORD dwControlFlag) { return BIT_FLAG_TEST( m_dwControlFlag, dwControlFlag); }

	void								SetPacketEncoder(CNtlPacketEncoder * pPacketEncoder) { m_pPacketEncoder = pPacketEncoder; }

	CNtlPacketEncoder *					GetPacketEncoder() { return m_pPacketEncoder; }


public:

	eSTATUS								GetStatus();

	void								SetStatus(eSTATUS status);

	bool								IsStatus(eSTATUS status);

	bool								ExchangeStatus(const eSTATUS prevStatus, bool bResult, const eSTATUS newStatus);

	const char *						GetStatusString();

	bool								IsConnected() { return m_bConnected; }


public:

	DWORD								GetConnectTime() { return m_dwConnectTime; }

	DWORD								GetBytesTotalSize() { return GetBytesRecvSize() + GetBytesSendSize(); }
	DWORD								GetBytesRecvSize() { return m_dwBytesRecvSize; }
	DWORD								GetBytesRecvSizeMax() { return m_dwBytesRecvSizeMax; }
	DWORD								GetBytesSendSize() { return m_dwBytesSendSize; }
	DWORD								GetBytesSendSizeMax() { return m_dwBytesSendSizeMax; }
	void								IncreaseBytesRecv(DWORD dwSize);
	void								IncreaseBytesSend(DWORD dwSize);

	DWORD								GetPacktTotalCount() { return GetPacketRecvCount() + GetPacketSendCount(); }
	DWORD								GetPacketRecvCount() { return m_dwPacketRecvCount; }
	DWORD								GetPacketSendCount() { return m_dwPacketSendCount; }
	void								IncreasePacketRecv();
	void								IncreasePacketSend();

	DWORD								GetRecvQueueMaxUseSize() { return m_recvQueue.GetMaxUsedSize(); }
	DWORD								GetSendQueueMaxUseSize() { return m_sendQueue.GetMaxUsedSize(); }

	virtual int							GetHeaderSize() {return PACKET_HEADSIZE;}
	virtual int							GetPacketLen(BYTE* pHeaderPointer);

	//virtual void						SetSequence(void* pvPacketData, WORD bySequence); //TODO


protected:

	void								UpdateTrafficInfo(DWORD dwTransferedBytes);


private:

	void								Init();

	void								Destroy();

private:

	int									CheckForSending(CNtlPacket* pPacket);

private:

	void								IncreasePostIoCount() { InterlockedIncrement( (LONG*)&m_dwIoPostCount); }

	void								DecreasePostIoCount() { InterlockedDecrement( (LONG*)&m_dwIoPostCount); }


private:

	eSTATUS								m_status;

	CNtlNetwork *						m_pNetworkRef;

	CNtlAcceptor *						m_pAcceptorRef;

	CNtlConnector *						m_pConnectorRef;

	DWORD								m_dwIoPostCount;


	CNtlSocket							m_socket;

	CNtlSockAddr						m_remoteAddr;

	CNtlSockAddr						m_localAddr;

	sIOCONTEXT							m_recvContext;

	sIOCONTEXT							m_sendContext;

	DWORD								m_dwAliveTime;	

	DWORD								m_dwLogTime;


private:

	CNtlNetBuffer 						m_recvBuffer;

	CNtlNetBuffer 						m_sendBuffer;

	CNtlNetQueue						m_recvQueue;

	CNtlNetQueue						m_sendQueue;

	CPeekerQueue						m_queSending;


protected:

	CNtlPacketEncoder *					m_pPacketEncoder;

private:

	DWORD								HEAVY_TRAFFIC_TRANSFERRED_DATA_IN_BYTES = 20475;
	DWORD								HEAVY_TRAFFIC_INTERVAL_IN_MILLISECS = 4000;

private:

	DWORD								m_dwControlFlag;

	bool								m_bSending;

	bool								m_bDisconnect;

	bool								m_bConnected;

	CNtlMutex							m_mutex;

	CNtlMutex							m_mutexSend;


private:

	DWORD								m_dwConnectTime;

	DWORD								m_dwBytesRecvSize;
	DWORD								m_dwBytesSendSize;
	DWORD								m_dwBytesRecvSizeMax;
	DWORD								m_dwBytesSendSizeMax;

	DWORD								m_dwPacketRecvCount;
	DWORD								m_dwPacketSendCount;

	DWORD								m_dwReceivedBytesForTrafficControl;
	DWORD								m_dwTimeTrafficControlStarted;
	bool								m_bIsTrafficHeavy;
	bool								m_bHasWrittenHeavyTrafficLog;

};



//-----------------------------------------------------------------------------------
// inline function
//-----------------------------------------------------------------------------------
inline void CNtlConnection::IncreaseBytesRecv(DWORD dwSize)
{
	InterlockedExchangeAdd( (LONG*)&m_dwBytesRecvSize, dwSize );

	if( dwSize > m_dwBytesRecvSizeMax )
	{
		m_dwBytesRecvSizeMax = dwSize;
	}
}

inline void CNtlConnection::IncreaseBytesSend(DWORD dwSize)
{
	InterlockedExchangeAdd( (LONG*)&m_dwBytesSendSize, dwSize );

	if( dwSize > m_dwBytesSendSizeMax )
	{
		m_dwBytesSendSizeMax = dwSize;
	}
}

inline void CNtlConnection::IncreasePacketRecv()
{
	InterlockedIncrement( (LONG*)&m_dwPacketRecvCount );
}

inline void CNtlConnection::IncreasePacketSend()
{
	InterlockedIncrement( (LONG*)&m_dwPacketSendCount );
}

inline CNtlConnection::eSTATUS CNtlConnection::GetStatus()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	eSTATUS status = m_status;

	return status;
}

inline void	CNtlConnection::SetStatus(eSTATUS status)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_status = status;
}

inline bool CNtlConnection::IsStatus(eSTATUS status)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	return m_status == status;

}

inline bool CNtlConnection::ExchangeStatus(const eSTATUS prevStatus, const bool bResult, const eSTATUS newStatus)
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( bResult )
	{
		if( prevStatus != m_status )
			return false;
	}
	else
	{
		if( prevStatus == m_status )
			return false;
	}

	m_status = newStatus;

	return true;
}

//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
inline int CNtlConnection::MakeSureCompletedPacket(CNtlNetBuffer* pBuffer, WORD* pwPacketLength)
{
	// If the header has not been completed
	if ( pBuffer->GetWorkRemainSize() < GetHeaderSize() )
	{
		return NTL_ERR_NET_PACKET_PENDING_HEADER;
	}


	int nLinearSize = (int) ( pBuffer->InGetQueueExtraPtr() - pBuffer->GetQueueWorkPtr() );

	// If the header is cut off from the buffer
	if ( nLinearSize < GetHeaderSize() )
	{
		::CopyMemory( pBuffer->InGetQueueExtraPtr(), pBuffer->InGetQueueBufferPtr(), GetHeaderSize() - nLinearSize );
	}

	if( m_pPacketEncoder )
	{
	//	int rc = m_pPacketEncoder->RxDecrypt( pBuffer->GetQueueWorkPtr() );
	//	if ( NTL_SUCCESS != rc )
	//	{
	//		return rc;
	//	}
	}

	int wPacketLen = GetPacketLen( pBuffer->GetQueueWorkPtr() );

	int nPacketSize = GetHeaderSize() + wPacketLen;
	if( nPacketSize >= pBuffer->GetMaxPacketSize() )
	{
		NTL_PRINT(1, "ALLOWED PACKET SIZE %d - CUR PACKET SIZE %d, GetHeaderSize %u, wPacketLen %u, GetWorkRemainSize %u, nLinearSize %u\n",
			GetMaxSendPacketSize(), nPacketSize, GetHeaderSize(), wPacketLen, pBuffer->GetWorkRemainSize(), nLinearSize);
		return NTL_ERR_NET_PACKET_EXCEED_ALLOWED_SIZE;
	}


	// If the packet is not completed
	if ( pBuffer->GetWorkRemainSize() < nPacketSize )
	{
		return NTL_ERR_NET_PACKET_PENDING_DATA;
	}


	//If the packet is truncated in the buffer
	if ( nLinearSize < nPacketSize )
	{
		if ( nLinearSize < GetHeaderSize() )
		{
			::CopyMemory(	pBuffer->GetQueueWorkPtr() + GetHeaderSize(),
				pBuffer->InGetQueueBufferPtr() + GetHeaderSize() - nLinearSize,
				wPacketLen );
		}
		else
		{
			::CopyMemory(	pBuffer->InGetQueueExtraPtr(),
				pBuffer->InGetQueueBufferPtr(),
				nPacketSize - nLinearSize );
		}
	}

	*pwPacketLength = (WORD) nPacketSize;

	return NTL_SUCCESS;
}