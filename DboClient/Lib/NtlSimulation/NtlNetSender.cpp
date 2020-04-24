#include "precomp_ntlsimulation.h"
#include "NtlNetSender.h"

// shared
#include "NtlClientNet.h"
#include "NtlPacketCommon.h"

// core
#include "NtlDebug.h"
#include "NtlCoreUtil.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"


NotifyNetSendPacketFunc CNtlNetSender::m_fnSendPacket = NULL;

CNtlNetSender::CNtlNetSender()
{
	m_hSocketId = INVALID_HSERVER;
	m_pNet = NULL;
}

void CNtlNetSender::CallNotifySendPacket(void *pData)
{
	if(m_fnSendPacket)
		(*m_fnSendPacket)(pData);
}

void CNtlNetSender::SetData(HSERVER hSocketId, CNtlClientNet *pNet, RwBool bAsync /*= FALSE*/)
{
	m_hSocketId = hSocketId;
	m_pNet = pNet;
}

bool CNtlNetSender::SendPacket(RwInt32 iDataLen, void *pData)
{
	if(m_pNet == NULL || m_hSocketId == INVALID_HSERVER)
		return 0;

    bool bError = m_pNet->PushPacket(m_hSocketId, pData, iDataLen); 

	CallNotifySendPacket(pData);

	CNtlSLEventGenerator::NotifySendPacket(pData); 

	if(!bError)
	{
		CNtlSLEventGenerator::NetSendError(this); 

		m_pNet = NULL;
		m_hSocketId = INVALID_HSERVER;
	}

	return bError;
}


void CNtlNetSender::LinkNotifySendPacketFunc(NotifyNetSendPacketFunc fnSendPacket)
{
	NTL_ASSERT(m_fnSendPacket == NULL, "CNtlNetSender::LinkNotifySendPacketFunc");

	m_fnSendPacket = fnSendPacket;
}

void CNtlNetSender::UnLinkNotifySendPacket(void)
{
	m_fnSendPacket = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

NtlVirtualNetSenderFunc CNtlVirtualNetSender::m_fnNetCallback;

CNtlVirtualNetSender::CNtlVirtualNetSender()
{
}

void CNtlVirtualNetSender::SetVirtualNetSenderFunction(NtlVirtualNetSenderFunc fnNetCallback)
{
	m_fnNetCallback = fnNetCallback;
}

bool CNtlVirtualNetSender::SendPacket(RwInt32 iDataLen, void *pData)
{
	CallNotifySendPacket(pData);

	if(m_fnNetCallback)
	{
		return (*m_fnNetCallback)(iDataLen, pData);
	}

	return true;
}
