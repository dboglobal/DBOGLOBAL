//***********************************************************************************
//
//	File		:	NtlConnection.cpp
//
//	Begin		:	2005-12-19
//
//	Copyright	:	ㄏ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network Connection Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlConnection.h"

#include "NtlNetwork.h"
#include "NtlAcceptor.h"
#include "NtlConnector.h"

#include "NtlLog.h"	
#include "NtlError.h"
#include "NtlThread.h"

#include "Utils.h"

#include "PacketBlock.h"


//-----------------------------------------------------------------------------------
// TODO List
//-----------------------------------------------------------------------------------
// @ PostRecv and the size when it all Post by PostSend are accumulated in the current buffer size
// This will consume a lot of kernel resource. Let's set the size to be transmitted and received at once put to use.
// In order to demonstrate the performance of each session will also consider placing a virtual function in the session
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CFunctionHelper
{
public:
	CFunctionHelper(const char * lpszFunctionName, void * pointer)
		:m_lpszFunctionName(lpszFunctionName), m_pointer(pointer)
	{ 
	//	NTL_PRINT( PRINT_SYSTEM, "Session[%X]\t%s\tBegin\t%s", m_pointer, m_lpszFunctionName, CNtlThread::GetCurrentThread()->GetName() );
	}

	~CFunctionHelper()
	{
	//	NTL_PRINT( PRINT_SYSTEM, "Session[%X]\t%s\tEnd\t%s", m_pointer, m_lpszFunctionName, CNtlThread::GetCurrentThread()->GetName() );
	}

public:

	const char * m_lpszFunctionName;

	void * m_pointer;
};

#define FUNCTION_BEGIN() CFunctionHelper __functionHelper(__FUNCTION__, this)
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const DWORD CONNECTION_KEEP_ALIVE_TIME			= 5 * 1000; // 家南 可记 KEEP ALIVE 俊 荤侩
const DWORD CONNECTION_KEEP_ALIVE_INTERVAL		= 1 * 1000; // 家南 可记 KEEP ALIVE 俊 荤侩
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// connection STATUS string
//-----------------------------------------------------------------------------------
char * s_connection_status_string[CNtlConnection::MAX_STATUS] = 
{
	"STATUS_INIT",
	"STATUS_CREATE",
	"STATUS_ACCEPT",
	"STATUS_CONNECT",
	"STATUS_ACTIVE",
	"STATUS_CLOSE",
	"STATUS_SHUTDOWN",
	"STATUS_DESTROY",
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * CNtlConnection::GetStatusString()
{
	if( m_status >= MAX_STATUS )
		return "Unknown STATUS";

	return s_connection_status_string[ m_status ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnection::CNtlConnection()
	:m_queSending(20)
{
	FUNCTION_BEGIN();

	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlConnection::~CNtlConnection()
{
	FUNCTION_BEGIN();

	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnection::Init()
{
	FUNCTION_BEGIN();

	m_status = STATUS_INIT;

	m_pNetworkRef	= NULL;
	m_pAcceptorRef	= NULL;
	m_pConnectorRef	= NULL;
	m_dwIoPostCount = 0;

	m_recvBuffer.Clear();
	m_sendBuffer.Clear();
	m_recvContext.Clear();
	m_sendContext.Clear();
	m_recvContext.param = this;
	m_sendContext.param = this;
	m_pPacketEncoder = NULL;

	m_dwControlFlag = 0;
	m_dwAliveTime = 0;

	m_bSending = false;
	m_bDisconnect = false;
	m_bConnected = false;

	m_dwConnectTime = 0;
	m_dwBytesRecvSize = 0;
	m_dwBytesSendSize = 0;
	m_dwBytesRecvSizeMax = 0;
	m_dwBytesSendSizeMax = 0;
	m_dwPacketRecvCount = 0;
	m_dwPacketSendCount = 0;

	m_dwReceivedBytesForTrafficControl = 0;
	m_dwTimeTrafficControlStarted = GetTickCount();
	m_bIsTrafficHeavy = false;
	m_bHasWrittenHeavyTrafficLog = false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Acceptor TH, Connector TH
//-----------------------------------------------------------------------------------
int CNtlConnection::Create(CNtlNetwork * pNetwork)
{
	FUNCTION_BEGIN();


	if( IsStatus( STATUS_CREATE ) )
	{
		NTL_PRINT(PRINT_SYSTEM, "true == IsStatus( STATUS_CREATE )");
		return NTL_ERR_NET_CONNECTION_STATUS_WRONG;
	}


	if( NULL == pNetwork )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL == pNetwork");
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	if( NULL != m_pNetworkRef )
	{
		NTL_PRINT(PRINT_SYSTEM, "NULL != m_pNetworkRef, m_pNetworkRef = %016x", m_pNetworkRef);
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}

	if (false == m_recvBuffer.Create(GetMaxRecvPacketCount(), GetMaxRecvPacketSize()))
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}

	//new send buffer
	m_queSending.SetBufferLimit(GetMaxSendPacketCount());

	/*if (false == m_sendBuffer.Create(1000, GetMaxSendPacketSize()))
	{
		return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
	}*/

	int rc = m_socket.Create( CNtlSocket::eSOCKET_TCP );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = m_socket.SetKeepAlive( CONNECTION_KEEP_ALIVE_TIME, CONNECTION_KEEP_ALIVE_INTERVAL );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = m_socket.SetLinger( true, 0 );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}

	m_pPacketEncoder = GetPacketEncoder();
	m_pNetworkRef = pNetwork;


	SetStatus( STATUS_CREATE );

	return NTL_SUCCESS;
}

bool CNtlConnection::Create(CNtlSocket & rSocket, CNtlSockAddr & rSockAddr, int nPacketCount, int nPacketSize)
{
	if (nPacketCount <= 0)
		nPacketCount = DEF_PACKET_MAX_COUNT;

	if (nPacketSize <= 0)
		nPacketSize = PACKET_MAX_SIZE;

	if (m_recvBuffer.Create(nPacketCount, nPacketSize))
	{
		m_queSending.SetBufferLimit(nPacketCount);

		m_socket = rSocket;
		m_remoteAddr = rSockAddr;

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Dispatcher TH, Application TH, Acceptor TH, Connector TH, 
//-----------------------------------------------------------------------------------
void CNtlConnection::Destroy()
{
	FUNCTION_BEGIN();

	m_socket.Shutdown( SD_BOTH );

	m_socket.Close();


	if( m_pAcceptorRef )
	{
		m_pAcceptorRef->OnDisconnected( m_bConnected );
	}

	if( m_pConnectorRef )
	{
		m_pConnectorRef->OnDisconnected( m_bConnected );
	}

	m_dwReceivedBytesForTrafficControl = 0;
	m_dwTimeTrafficControlStarted = 0;
	m_bIsTrafficHeavy = false;
	m_bHasWrittenHeavyTrafficLog = false;
}

int CNtlConnection::CheckForSending(CNtlPacket * pPacket)
{
	if (IsStatus(STATUS_ACTIVE) == false)
		return NTL_ERR_NET_CONNECTION_STATUS_WRONG;

	if(pPacket->IsValidPacket() == false)
		return NTL_ERR_NET_PACKET_INVALID;

	if(pPacket->GetPacketLen() > GetMaxSendPacketSize())
		return NTL_ERR_NET_PACKET_EXCEED_ALLOWED_SIZE;

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH, Application TH ( PostRecv/PostSend/IOCP transfered 0 )
//-----------------------------------------------------------------------------------
void CNtlConnection::Close(bool bForce)
{
	UNREFERENCED_PARAMETER(bForce);
	FUNCTION_BEGIN();

	if( false == ExchangeStatus( STATUS_CLOSE, false, STATUS_CLOSE ) )
	{
		return;
	}

	// The bottom, it is impossible in progress. Processor extinction due to the memory dwaem
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Application TH, Monitor TH
//-----------------------------------------------------------------------------------
int CNtlConnection::Disconnect(bool bGraceful)
{
	FUNCTION_BEGIN();

	if( bGraceful )
	{
		m_socket.Shutdown( SD_SEND );
	}
	else
	{
		m_socket.Shutdown( SD_BOTH );
		m_socket.Close();
	}

	m_bDisconnect = true;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnection::CheckDisconnect(bool bGraceful)
{
	FUNCTION_BEGIN();

	if (!m_bDisconnect)
	{
		if (bGraceful)
		{
			m_socket.Shutdown(SD_SEND);
		}
		else
		{
			m_socket.Shutdown(SD_BOTH);
			m_socket.Close();
		}

		m_bDisconnect = true;

		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Application TH, Monitor TH
//-----------------------------------------------------------------------------------
int CNtlConnection::Shutdown()
{
	FUNCTION_BEGIN();


	SetStatus( STATUS_SHUTDOWN );


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnection::RecvPackets(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();

	IncreaseBytesRecv(dwTransferedBytes);

	if (!m_recvBuffer.IncreasePushPos(dwTransferedBytes))
	{
		NTL_PRINT(PRINT_SYSTEM, "Session[%X] Recv Buffer OverFlow : BufferCur[%d] BufferMax[%d] TransferedBytes[%u]", this, m_recvBuffer.GetCurSize(), m_recvBuffer.GetQueueSize(), dwTransferedBytes);
		return NTL_ERR_NET_SESSION_RECV_BUFFER_OVERFLOW;
	}

	int rc = NTL_SUCCESS;
	WORD wPacketLength = 0;

	CNtlPacket packet;
	do
	{
		rc = MakeSureCompletedPacket(&m_recvBuffer, &wPacketLength);
		if (NTL_SUCCESS != rc)
		{
			if (NTL_ERR_NET_PACKET_PENDING_HEADER == rc || NTL_ERR_NET_PACKET_PENDING_DATA == rc)
			{
				break;
			}
			NTL_PRINT(PRINT_SYSTEM, "Session[%X] MakeSureCompletedPacket Error[%d:%s]", this, rc, NtlGetErrorMessage(rc));
			return rc;
		}

		if (true != IsValidPacket(m_recvBuffer.GetQueueWorkPtr(), wPacketLength))
		{
			NTL_PRINT(PRINT_SYSTEM, "true != IsValidPacket()");
		}

		packet.AttachData(m_recvBuffer.GetQueueWorkPtr(), wPacketLength);

		m_recvBuffer.IncreaseWorkPos(wPacketLength);

		m_pNetworkRef->PostNetEventMessage((WPARAM)NETEVENT_RECV, (LPARAM)this);

	} while (NTL_SUCCESS == rc);


	return NTL_SUCCESS;
}

int CNtlConnection::PostSend_Ex()
{
	FUNCTION_BEGIN();

	if (false == IsStatus(STATUS_ACTIVE))
	{
		Disconnect(false);
		return NTL_ERR_NET_CONNECTION_STATUS_WRONG;
	}


	DWORD dwFlags = 0;
	DWORD dwSendBytes = 0;
	int nSendBufCount = 0;

	m_sendContext.Reset();
	m_sendContext.iomode = IOMODE_SEND_EX;

	m_queSending.PeekBuf(&m_sendContext.wsabuf, 1, nSendBufCount);

	IncreasePostIoCount();
	//printf("nSendBufCount %i, len %lu \n", nSendBufCount, m_sendContext.wsabuf.len);
	int rc = m_socket.SendEx(&m_sendContext.wsabuf,
		(DWORD)nSendBufCount,
		&dwSendBytes,
		dwFlags,
		&m_sendContext);

	if (NTL_SUCCESS != rc)
	{
		DecreasePostIoCount();
		//NTL_PRINT(PRINT_SYSTEM, "Session[%X] PostSend_Ex Function Failed: (%d)%s", this, rc, NtlGetErrorMessage(rc));
		return rc;
	}

	return NTL_SUCCESS;
}

int CNtlConnection::AddToSendBuffer(CNtlPacket * pPacket)
{
	if (CPacketBlock* pBlock = m_queSending.GetPacketBlock(pPacket->GetUsedSize()))
	{
		pPacket->GetPacketHeader()->packetSequence = WORD(m_dwPacketSendCount & PACKET_MAX_SEQUENCE);

		pBlock->Push(pPacket->GetPacketBuffer(), pPacket->GetUsedSize());

		IncreasePacketSend();

		return NTL_SUCCESS;
	}

	return NTL_ERR_NET_SESSION_SEND_BUFFER_OVERFLOW;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( IOCP )
//-----------------------------------------------------------------------------------
int CNtlConnection::CompleteIO(sIOCONTEXT * pIOContext, DWORD dwParam)
{
	switch( pIOContext->iomode )
	{
	case IOMODE_ACCEPT:
		return CompleteAccept( dwParam );

	case IOMODE_CONNECT:
		return CompleteConnect( dwParam );

	case IOMODE_RECV:
		return CompleteRecv( dwParam );

	case IOMODE_SEND:
		return CompleteSend( dwParam );

	case IOMODE_SEND_EX:
		return CompleteSend_Ex(dwParam);

	default:
		return NTL_ERR_NET_INVALID_COMPLETE_IO_MODE;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( CompleteRecv, CompleteAccept )
//-----------------------------------------------------------------------------------
int CNtlConnection::PostRecv()
{
	FUNCTION_BEGIN();

	if( false == IsStatus( STATUS_ACTIVE ) )
	{
		Disconnect( false );
		return NTL_ERR_NET_SESSION_CLOSED; //if we do here NTL_SUCCESS then connection will stay forever active..
	}

	if (m_bIsTrafficHeavy == true)
	{
		Disconnect(false);
		return NTL_ERR_NET_SESSION_CLOSED; //if we do here NTL_SUCCESS then connection will stay forever active..
	}

	DWORD dwFlags = 0;
	DWORD dwTransferedBytes	= 0;

	m_recvContext.Reset();
	m_recvContext.iomode = IOMODE_RECV;
	m_recvContext.wsabuf.buf = (char*) m_recvBuffer.GetQueuePushPtr();
	m_recvContext.wsabuf.len = m_recvBuffer.GetPushAvailableSize();


	if (IsSetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE))
	{
		DWORD dwLen = 0;
		dwLen = m_bIsTrafficHeavy ? GetMaxRecvPacketCount() / DEF_PACKET_MAX_COUNT : GetMaxRecvPacketCount();
		if (dwLen < m_recvContext.wsabuf.len)
			m_recvContext.wsabuf.len = dwLen;
	}

	
	//  [1/20/2007 zeroera] : Description: Considering that you adjust the appropriate buff size takes place, but the recv buffer overflow in case the client will send a Fake Packet appropriate deal
	if( m_recvBuffer.GetPushAvailableSize() <= 0 )
	{
		Disconnect( false );

		NTL_PRINT(PRINT_SYSTEM, "Session[%X] m_recvBuffer.GetPushAvailableSize() <= 0, m_recvBuffer.GetPushAvailableSize() = %d", this, m_recvBuffer.GetPushAvailableSize() );
		return NTL_ERR_NET_SESSION_RECV_BUFFER_OVERFLOW;
	}

	IncreasePostIoCount();

	int rc = m_socket.RecvEx(	&m_recvContext.wsabuf,
		1,
		&dwTransferedBytes,
		&dwFlags,
		&m_recvContext);

	if( NTL_SUCCESS != rc )
	{
		DecreasePostIoCount();

		NTL_PRINT(PRINT_SYSTEM, "Session[%X] RecvEx Function Failed (%d)%s", this, rc, NtlGetErrorMessage( rc ) );
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by App TH, Worker TH ( CompleteSend, PushPacket )
//-----------------------------------------------------------------------------------
int CNtlConnection::PostSend()
{
	FUNCTION_BEGIN();

	if( false == IsStatus( STATUS_ACTIVE ) )
	{
		Disconnect( false );
		return NTL_ERR_NET_CONNECTION_STATUS_WRONG;
	}


	DWORD dwFlags		= 0;
	DWORD dwSendBytes	= 0;

	m_sendContext.Reset();
	m_sendContext.iomode = IOMODE_SEND;
	m_sendContext.wsabuf.buf = (char*) m_sendBuffer.GetQueuePopPtr();
	
	if( m_sendBuffer.GetPopPos() + m_sendBuffer.GetCurSize() >= m_sendBuffer.GetQueueSize() )
	{

		//  [9/13/2007 zeroera] : EDIT: part modified to improve performance
		m_sendContext.wsabuf.len = m_sendBuffer.GetQueueSize() - m_sendBuffer.GetPopPos();

		//if( m_sendBuffer.GetQueueSize() <= m_sendBuffer.GetPopPos() )
		//{
		//	NTL_PRINT(PRINT_SYSTEM, "m_sendBuffer.GetQueueSize() <= m_sendBuffer.GetPopPos(), m_sendBuffer.GetQueueSize() = %d, m_sendBuffer.GetPopPos() = %d", m_sendBuffer.GetQueueSize(), m_sendBuffer.GetPopPos());
		//	Disconnect( false );
		//	return NTL_SUCCESS;
		//}
	}
	else
	{
		m_sendContext.wsabuf.len = m_sendBuffer.GetCurSize();

		//if( m_sendBuffer.GetCurSize() <= 0 )
		//{
		//	NTL_PRINT(PRINT_SYSTEM, "m_sendBuffer.GetCurSize() <= 0, m_sendBuffer.GetCurSize() = %d", m_sendBuffer.GetCurSize());
		//	Disconnect( false );
		//	return NTL_SUCCESS;
		//}
	}
	//printf("nSendBufCount 1, len %lu \n", m_sendContext.wsabuf.len);
	IncreasePostIoCount();

	int rc = m_socket.SendEx(	&m_sendContext.wsabuf,
							 1,
							 &dwSendBytes,
							 dwFlags,
							 &m_sendContext);

	if( NTL_SUCCESS != rc )
	{
		DecreasePostIoCount();

		NTL_PRINT(PRINT_SYSTEM,"Session[%X] SendEx Function Failed: (%d)%s", this, rc, NtlGetErrorMessage( rc ) );
		return rc;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Acceptor TH ( Run )
//-----------------------------------------------------------------------------------
int CNtlConnection::PostAccept(CNtlAcceptor* pAcceptor)
{
	FUNCTION_BEGIN();

	if( NULL == pAcceptor )
	{
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}


	SetStatus( STATUS_ACCEPT );


	m_pAcceptorRef = pAcceptor;


	DWORD dwBytes = 0;

	m_recvContext.Reset();
	m_recvContext.iomode = IOMODE_ACCEPT;
	m_recvContext.wsabuf.buf = (char*) m_recvBuffer.GetQueuePushPtr();
	m_recvContext.wsabuf.len = 0;

	IncreasePostIoCount();
	m_pAcceptorRef->IncreaseCurAcceptingCount();


	int rc = pAcceptor->GetListenSocket().AcceptEx(	m_socket,
		m_recvContext.wsabuf.buf,
		0,
		sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16,
		&dwBytes,
		&m_recvContext );

	if( NTL_SUCCESS != rc )
	{
		DecreasePostIoCount();
		m_pAcceptorRef->DecreaseCurAcceptingCount();

		NTL_PRINT(PRINT_SYSTEM, "Session[%X] AcceptEx Function Failed: (%d)%s", this, rc, NtlGetErrorMessage( rc ) );
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Connector TH ( Run )
//-----------------------------------------------------------------------------------
int CNtlConnection::PostConnect(CNtlConnector* pConnector)
{
	FUNCTION_BEGIN();

	if( NULL == pConnector )
	{
		return NTL_ERR_SYS_INPUT_PARAMETER_WRONG;
	}

	SetStatus( STATUS_CONNECT );

	m_pConnectorRef = pConnector;


	int rc = m_socket.Connect( pConnector->GetConnectAddr() );
	if( NTL_SUCCESS != rc )
	{
		ERR_LOG(LOG_NETWORK, "Session[%X] Connect Function Failed: (%d)%s", this, rc, NtlGetErrorMessage(rc));
		//printf( "Session[%X] Connect Function Failed: (%d)%s", this, rc, NtlGetErrorMessage( rc ) );
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( IOCP )
//-----------------------------------------------------------------------------------
int CNtlConnection::CompleteRecv(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();

	// Remote close
	if( 0 == dwTransferedBytes )
	{
		return NTL_ERR_NET_SESSION_CLOSED;
	}


	UpdateTrafficInfo(dwTransferedBytes);


	int rc = RecvPackets(dwTransferedBytes);
	if (rc != NTL_SUCCESS)
		return rc;

	rc = PostRecv();
	if (rc != NTL_SUCCESS)
		return rc;

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( IOCP )
//-----------------------------------------------------------------------------------
int CNtlConnection::CompleteSend(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();


	IncreaseBytesSend( dwTransferedBytes );


	CNtlLock lock(&m_mutexSend);

	m_sendBuffer.IncreasePopPos( dwTransferedBytes );


	int rc = 0;

	//if (IsSetControlFlag(CONTROL_FLAG_USE_SEND_QUEUE))
	{
		CNtlPacket * pSendPacket = m_sendQueue.PeekPacket();
		while (pSendPacket)
		{
			if (m_sendBuffer.GetPushAvailableSize() <= pSendPacket->GetUsedSize())
			{
				break;
			}

			memcpy(m_sendBuffer.GetQueuePushPtr(), pSendPacket->GetPacketBuffer(), pSendPacket->GetUsedSize());
			m_sendBuffer.IncreasePushPos(pSendPacket->GetUsedSize());
			IncreasePacketSend();

			m_sendQueue.PopPacket();
			SAFE_DELETE(pSendPacket);

			pSendPacket = m_sendQueue.PeekPacket();
		}
	}

	if( m_sendBuffer.GetCurSize() )
	{
		rc = PostSend();
		if( NTL_SUCCESS != rc )
		{
			m_bSending = false;

			Disconnect( false );

			ERR_LOG(LOG_NETWORK, "Session[%X] PostSend Error[%d:%s]", this, rc, NtlGetErrorMessage(rc));
			return rc;
		}
		else
		{
			m_bSending = true;
		}
	}
	else
	{
		m_bSending = false;
	}

	return NTL_SUCCESS;
}

int CNtlConnection::CompleteSend_Ex(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();

	IncreaseBytesSend(dwTransferedBytes);

	CNtlLock lock(&m_mutexSend);

	m_queSending.RemoveMsg(dwTransferedBytes);

	if (m_queSending.IsEmpty())
	{
		m_bSending = false;
	}
	else
	{
		int rc = PostSend_Ex();
		if (rc != NTL_SUCCESS)
		{
			m_bSending = false;

			Disconnect(false);

			ERR_LOG(LOG_NETWORK, "Session[%X] CompleteSend_Ex Error[%d:%s]", this, rc, NtlGetErrorMessage(rc));

			return rc;
		}
		else
		{
			m_bSending = true;
		}
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( IOCP )
//-----------------------------------------------------------------------------------
int CNtlConnection::CompleteAccept(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();

	UpdateTrafficInfo(dwTransferedBytes);

	SOCKADDR_IN * pLocalAddr = NULL;
	SOCKADDR_IN * pRemoteAddr = NULL;
	int nLocalAddrLen = 0;
	int nRemoteAddrLen = 0;

	m_socket.GetAcceptExSockaddrs(	m_recvContext.wsabuf.buf,
									0,
									sizeof(SOCKADDR_IN) + 16,
									sizeof(SOCKADDR_IN) + 16,
									(SOCKADDR**) &pLocalAddr,
									&nLocalAddrLen,
									(SOCKADDR**) &pRemoteAddr,
									&nRemoteAddrLen);

	SetAddress( pLocalAddr, pRemoteAddr );
	ZeroMemory( m_recvContext.wsabuf.buf, sizeof(SOCKADDR_IN) + 16 + sizeof(SOCKADDR_IN) + 16 );
	m_bConnected = true;

	int rc = m_pNetworkRef->Associate( this, true );
	if( NTL_SUCCESS != rc )
	{
		ERR_LOG(LOG_NETWORK, "Session[%X] Associate Error[%d:%s]", this, rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	if (m_pNetworkRef->IsBlockedIp(m_remoteAddr.GetAddr()))
	{
		ERR_LOG(LOG_NETWORK, "Session[%X] Connection from Blocked IP(%s).", this, m_remoteAddr.GetDottedAddr());
		return NTL_ERR_NET_CONNECTION_FROM_BLOCKED_IP;
	}

	SetStatus( STATUS_ACTIVE );
	m_dwConnectTime = GetTickCount();

	m_pAcceptorRef->OnAccepted(this);

	m_pNetworkRef->PostNetEventMessage( (WPARAM)NETEVENT_ACCEPT, (LPARAM)this );


	rc = PostRecv();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}


	//NTL_PRINT(PRINT_SYSTEM, "Session[%X]\tCompleteAccept Called Local[%s:%u] Remote[%s:%u]", this, GetLocalIP(), GetLocalPort(), GetRemoteIP(), GetRemotePort());

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( IOCP )
//-----------------------------------------------------------------------------------
int CNtlConnection::CompleteConnect(DWORD dwTransferedBytes)
{
	FUNCTION_BEGIN();

	UpdateTrafficInfo(dwTransferedBytes);

	m_socket.GetPeerAddr( m_remoteAddr );
	m_socket.GetLocalAddr( m_localAddr );
	m_bConnected = true;

	int rc = m_pNetworkRef->Associate( this, true );
	if( NTL_SUCCESS != rc )
	{
		NTL_PRINT(PRINT_SYSTEM, "Session[%X] CompleteConnect Error[%d:%s]", this, rc, NtlGetErrorMessage(rc) );
		return rc;
	}

	SetStatus( STATUS_ACTIVE );
	m_dwConnectTime = GetTickCount();


	m_pConnectorRef->OnConnected();


	m_pNetworkRef->PostNetEventMessage( (WPARAM)NETEVENT_CONNECT, (LPARAM)this );	

	rc = PostRecv();
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}



	NTL_PRINT(PRINT_SYSTEM, "Session[%X]\tCompleteConnect Called Local[%s:%u] Remote[%s:%u]", this, GetLocalIP(), GetLocalPort(),	GetRemoteIP(), GetRemotePort() );


	return NTL_SUCCESS;
}

int CNtlConnection::SendPacket(CNtlPacket * pPacket)
{
	FUNCTION_BEGIN();

	int nRes = CheckForSending(pPacket);

	if (nRes != NTL_SUCCESS)
	{
		return nRes;
	}

	CNtlLock lock(&m_mutexSend);

	//if (NtlWrapper::CEncoderEx::CheckForAuth((NtlWrapper::CEncoderEx *)&pointer->m_pEncoderRecv, packet))
	//{

		nRes = AddToSendBuffer(pPacket);
		if (nRes != NTL_SUCCESS)
		{
			return nRes;
		}

		if (m_bSending)
		{
			return NTL_SUCCESS;
		}

		nRes = PostSend_Ex();
		if (nRes != NTL_SUCCESS)
		{
			return nRes;
		}

		m_bSending = true;

	//}

	return NTL_SUCCESS;
}

int CNtlConnection::PostPacket(CNtlPacket * pPacket)
{
	FUNCTION_BEGIN();

	int nRes = CheckForSending(pPacket);

	if (nRes != NTL_SUCCESS)
	{
		return nRes;
	}

	CNtlLock lock(&m_mutexSend);

	//if (NtlWrapper::CEncoderEx::CheckForAuth((NtlWrapper::CEncoderEx *)&pointer->m_pEncoderRecv, packet))
	//{

		nRes = AddToSendBuffer(pPacket);

	//}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( CompleteAccept )
//-----------------------------------------------------------------------------------
void CNtlConnection::SetAddress(SOCKADDR_IN * pLocalAddr, SOCKADDR_IN * pRemoteAddr)
{
	FUNCTION_BEGIN();


	m_localAddr	= *pLocalAddr;
	m_remoteAddr = *pRemoteAddr;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Worker TH ( CompleteRecv )
//-----------------------------------------------------------------------------------
bool CNtlConnection::PopPacket(CNtlPacket * pPacket)
{
	FUNCTION_BEGIN();

	if( NULL == pPacket )
	{
		NTL_PRINT(PRINT_SYSTEM,"NULL == pPacket");
		return false;
	}


	int nPacketLen = GetPacketLen( m_recvBuffer.GetQueuePopPtr() );

	pPacket->AttachData( m_recvBuffer.GetQueuePopPtr(), (WORD) (GetHeaderSize() + nPacketLen) );

	if( false == pPacket->IsValidPacket() )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlConnection::IsValidPacket(void * pvPacketHeader, WORD wPacketLength)
{
	if (GetHeaderSize() > wPacketLength)
	{
		return false;
	}

	CNtlPacket packet;
	packet.AttachData((BYTE*)pvPacketHeader, wPacketLength);

	return packet.IsValidPacket();
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//WORD CNtlConnection::GetSequence(void * pvPacketData)
//{
//	LPPACKETDATA pData = (LPPACKETDATA)pvPacketData;

//	return pData->packetSequence;
//}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by App TH, Worker TH ( Send )
//-----------------------------------------------------------------------------------
int CNtlConnection::PushPacket(CNtlPacket * pPacket)
{
	FUNCTION_BEGIN();


	if( false == IsStatus( STATUS_ACTIVE ) )
	{
		return NTL_ERR_NET_CONNECTION_STATUS_WRONG;
	}

	if( false == pPacket->IsValidPacket() )
	{
		return NTL_ERR_NET_PACKET_INVALID;
	}

	CNtlLock lock(&m_mutexSend);

	if( pPacket->GetPacketLen() > GetMaxSendPacketSize() )
	{
		return NTL_ERR_NET_PACKET_EXCEED_ALLOWED_SIZE;
	}


	if (m_sendBuffer.GetPushAvailableSize() <= pPacket->GetUsedSize())
	{
		//printf("m_sendBuffer.GetPushAvailableSize() %u <= pPacket->GetUsedSize() %u \n", m_sendBuffer.GetPushAvailableSize(), pPacket->GetUsedSize());
		CNtlPacket * pQueuedPacket = new CNtlPacket(*pPacket);
		if (NULL == pQueuedPacket)
		{
			return NTL_ERR_SYS_MEMORY_ALLOC_FAIL;
		}

		if (false == m_sendQueue.PushPacket(pQueuedPacket))
		{
			SAFE_DELETE(pQueuedPacket);

			return NTL_ERR_NET_SESSION_SEND_BUFFER_OVERFLOW;
		}
		
		return NTL_SUCCESS;
	}

	memcpy( m_sendBuffer.GetQueuePushPtr(), pPacket->GetPacketBuffer(), pPacket->GetUsedSize() );

	int rc = NTL_SUCCESS;

	m_sendBuffer.IncreasePushPos( pPacket->GetUsedSize() );
	IncreasePacketSend();


	if( false == m_bSending )
	{
		rc = PostSend();
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}

		m_bSending = true;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
// Called by Monitor TH ( Valid Check )
//-----------------------------------------------------------------------------------
bool CNtlConnection::ValidCheck(DWORD dwTickDiff)
{
	if( IsSetControlFlag( CONTROL_FLAG_CHECK_ALIVE ) )
	{
		m_dwAliveTime += dwTickDiff;

		if( m_dwAliveTime >= GetAliveCheckTime() )
		{
			return false;
		}
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlConnection::PacketLogTime(DWORD dwTickDiff)
{
	m_dwLogTime += dwTickDiff;
	if (m_dwLogTime >= c_ConnectionFlooding)
	{
		m_dwLogTime = 0;
		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlConnection::IsShutdownable()
{
	if( IsStatus( STATUS_CLOSE ) )
	{
		if( 0 == InterlockedCompareExchange( (LONG*)&m_dwIoPostCount, 0, 0 )  )
		{
			return true;
		}
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlConnection::GetAddressInfo(CNtlString * pString, WORD * pPort, bool bRemote /* = true */)
{
	if( bRemote )
	{
		pString->GetString().assign( GetRemoteAddr().GetDottedAddr() );
		*pPort = GetRemoteAddr().GetPort();
	}
	else
	{
		pString->GetString().assign( GetLocalAddr().GetDottedAddr() );
		*pPort = GetLocalAddr().GetPort();
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlConnection::GetPacketLen(BYTE* pHeaderPointer)
{	
	STHeaderBase* pHeader = (STHeaderBase*)pHeaderPointer;
	return pHeader->wPacketLen;	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//void CNtlConnection::SetSequence(void* pvPacketData, WORD bySequence)
//{
//	LPPACKETDATA pData = (LPPACKETDATA)pvPacketData;
//
//	pData->packetSequence = bySequence;
//}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
//bool CNtlConnection::HasValidSequence(void* pvPacketData, WORD wSequence)
//{
//	LPPACKETDATA pData = (LPPACKETDATA)pvPacketData;
//
//	return wSequence == pData->packetSequence;
//	return false;
//}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlConnection::UpdateTrafficInfo(DWORD dwTransferedBytes)
{
	if (!IsSetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE))
		return;

	DWORD dwNow = GetTickCount();

	m_dwReceivedBytesForTrafficControl = UnsignedSafeIncrease<DWORD>(m_dwReceivedBytesForTrafficControl, dwTransferedBytes);

	if (!m_bIsTrafficHeavy && HEAVY_TRAFFIC_TRANSFERRED_DATA_IN_BYTES < m_dwReceivedBytesForTrafficControl)
	{
		//ERR_LOG(LOG_NETWORK, "Traffic Info.(Remote IP : %s, Local Port : %u) [%u] bytes, [%u] millisecs", GetRemoteIP(), GetLocalPort(), m_dwReceivedBytesForTrafficControl, dwNow - m_dwTimeTrafficControlStarted);

		if (HEAVY_TRAFFIC_INTERVAL_IN_MILLISECS <= dwNow - m_dwTimeTrafficControlStarted)
		{
			m_dwReceivedBytesForTrafficControl = 0;
			m_dwTimeTrafficControlStarted = dwNow;
		}
		else
		{
			m_bIsTrafficHeavy = true;

			m_pNetworkRef->RegisterBlockedIp(GetRemoteAddr().GetAddr());

			if (!m_bHasWrittenHeavyTrafficLog)
			{
				ERR_LOG(LOG_NETWORK, "Heavy Traffic Mode.(Remote IP : %s, Local Port : %u) [%u] bytes, [%u] millisecs, [%u] bytes/secs", GetRemoteIP(), GetLocalPort(), m_dwReceivedBytesForTrafficControl, dwNow - m_dwTimeTrafficControlStarted, m_dwReceivedBytesForTrafficControl / dwNow - m_dwTimeTrafficControlStarted);
				m_bHasWrittenHeavyTrafficLog = true;
			}
		}
	}
}


