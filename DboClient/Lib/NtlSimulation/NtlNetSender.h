/*****************************************************************************
 *
 * File			: NtlPacketHander.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 27	
 * Abstract		: Input handler 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_NET_SENDER_H__
#define __NTL_NET_SENDER_H__

#include "NtlClientNet.h"

typedef void (*NotifyNetSendPacketFunc)(void *pData);

class CNtlNetSender
{
private:

	HSERVER m_hSocketId;
	CNtlClientNet *m_pNet;
	static NotifyNetSendPacketFunc m_fnSendPacket;

protected:

	void CallNotifySendPacket(void *pData);

public:

	CNtlNetSender();
	virtual ~CNtlNetSender() {}

	void		SetData(HSERVER hSocketId, CNtlClientNet *pNet, RwBool bAsync = FALSE);
	HSERVER		GetSocketHandle(void);
	virtual bool SendPacket(RwInt32 iDataLen, void *pData);

	static void LinkNotifySendPacketFunc(NotifyNetSendPacketFunc fnSendPacket);
	static void UnLinkNotifySendPacket(void);
};

inline HSERVER CNtlNetSender::GetSocketHandle(void)
{
	return m_hSocketId;
}


typedef bool (*NtlVirtualNetSenderFunc)(RwInt32 iDataLen, void *pData);


class CNtlVirtualNetSender : public CNtlNetSender
{
private:

	static NtlVirtualNetSenderFunc m_fnNetCallback;

public:

	CNtlVirtualNetSender();

	static void SetVirtualNetSenderFunction(NtlVirtualNetSenderFunc fnNetCallback);

	bool SendPacket(RwInt32 iDataLen, void *pData);
};



#endif