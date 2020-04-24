//***********************************************************************************
//
//	File		:	NtlSession.cpp
//
//	Begin		:	2005-12-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Network Session Class
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSession.h"

#include "NtlPacketSYS.h"
#include "NtlLog.h"
#include "NtlError.h"

#include "NtlAcceptor.h"
#include "NtlNetwork.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession::CNtlSession(SESSIONTYPE sessionType)
:
m_sessionType( sessionType )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSession::~CNtlSession()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Init()
{
	m_hSession = INVALID_HSESSION;

	m_hAcceptingSession = INVALID_HSESSION;

	m_dwReferenceCount = 1;

	m_nDecryptionFailureCount = 0;

	m_uniqueHandle = INVALID_HSESSION;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Destroy()
{
	m_uniqueHandle = INVALID_HSESSION;

	if( IsConnected() )
	{
		/*NTL_LOG( LOG_TRAFFIC, "%u,%s,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d",
									GetSessionType(), GetRemoteIP(), GetRemotePort(),
									GetTickCount() - GetConnectTime(),
									GetBytesTotalSize(),
									GetBytesRecvSize(),
									GetBytesRecvSizeMax(),
									GetBytesSendSize(),
									GetBytesSendSizeMax(),
									GetPacktTotalCount(),
									GetPacketRecvCount(),
									GetPacketSendCount(),
									GetRecvQueueMaxUseSize(),
									GetSendQueueMaxUseSize(),
									GetRecvBuffer()->GetQueueLoopCount(),
									GetSendBuffer()->GetQueueLoopCount() );*/
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Acquire()
{
	InterlockedIncrement( (LONG*)&m_dwReferenceCount );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSession::Release()
{
	if( 0 == InterlockedDecrement( (LONG*)&m_dwReferenceCount ) )
	{
		delete this;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlSession::IsInternalConnection(char* pIp)
{
#if defined(NTL_INTERNAL_ADDRESS_PREFIX)
	#undef NTL_INTERNAL_ADDRESS_PREFIX
#endif

#define NTL_INTERNAL_ADDRESS_PREFIX		"10.0.0."
	if (NULL == pIp)
	{
		return false;
	}

	if (0 == strncmp(pIp, NTL_INTERNAL_ADDRESS_PREFIX, strlen(NTL_INTERNAL_ADDRESS_PREFIX)))
	{
		return true;
	}
	else
	{
		return false;
	}
#undef NTL_INTERNAL_ADDRESS_PREFIX
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSession::OnConnect()
{
	m_uniqueHandle = rand() % INVALID_HSESSION - 1;

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSession::OnAccept()
{
	m_uniqueHandle = rand() % INVALID_HSESSION - 1;

	return NTL_SUCCESS;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSession::OnDispatch(CNtlPacket * pPacket)
{
	PACKETDATA * pPacketData = (PACKETDATA *) pPacket->GetPacketData();
	//printf("CNtlSession::OnDispatch | pPacketData->wOpCode: %u \n", pPacketData->wOpCode);
	switch( pPacketData->wOpCode )
	{
		case SYS_ALIVE:
		{
			ResetAliveTime();
		}
		break;

	default:
		ERR_LOG( LOG_NETWORK, "SessionType[%u] Send Wrong Packet[%u]", m_sessionType, pPacketData->wOpCode );
		return NTL_ERR_NET_PACKET_OPCODE_WRONG;
	}

	return NTL_SUCCESS;
}

int CNtlSession::ProcessPacket()
{
	int rc = NTL_SUCCESS;

	CNtlPacket packet;
	if (PopPacket(&packet))
	{
		int nPacketLen = GetPacketLen((BYTE*)(packet.GetPacketHeader()));

		if(m_pPacketEncoder && m_pPacketEncoder->RxDecrypt(packet, GetPacketRecvCount() & PACKET_MAX_SEQUENCE) != NTL_SUCCESS)
		{
			if (ALLOWED_DECRYPTION_FAILURE_COUNT < ++m_nDecryptionFailureCount)
			{
				m_pNetworkRef->RegisterBlockedIp(GetRemoteAddr().GetAddr()); 
				SetStatus(STATUS_CLOSE);

				if (CheckDisconnect(false))
				{
					ERR_LOG(LOG_NETWORK,"Session[%X] : Diconnecting due to too many decryption failure. Local Port[%u], Remote IP[%s]", this, GetLocalPort(), GetRemoteIP());
				}

				m_nDecryptionFailureCount = 0;
			}
		}
		else
		{
			if (packet.GetPacketHeader()->packetSequence != WORD(GetPacketRecvCount() & PACKET_MAX_SEQUENCE))
			{
				ERR_LOG(LOG_NETWORK, "Session[%X] Sequence Error PacketSEQ[%u] CurrentSEQ[%u]", this, packet.GetPacketHeader()->packetSequence, (GetPacketRecvCount() & PACKET_MAX_SEQUENCE));
				return NTL_ERR_NET_PACKET_SEQUENCE_FAIL;
			}

			IncreasePacketRecv();

			rc = OnDispatch(&packet); //dispatch
		}

		GetRecvBuffer()->IncreasePopPos(GetHeaderSize() + nPacketLen);
	}
	else
	{
		ERR_LOG(LOG_NETWORK, "Session[%X] : Diconnecting due to sending invalid packet. Local Port[%u], Remote IP[%s]", this, GetLocalPort(), GetRemoteIP());
		Disconnect(false);
		rc = NTL_ERR_NET_PACKET_INVALID;
	}

	return rc;
}
