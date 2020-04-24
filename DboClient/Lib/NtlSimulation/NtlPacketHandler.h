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

#ifndef __NTL_PACKET_HANDLER_H__
#define __NTL_PACKET_HANDLER_H__

typedef void (*NotifyPacketHander)(void *pPacket);

class CNtlPacketHandler
{
private:

	typedef std::map<WORD, NotifyPacketHander> MapHandler;
	static MapHandler m_mapHandler;

public:

	static void Register(WORD wProtocolOp, NotifyPacketHander fp);
	static void UnRegister(WORD wProtocolOp);

	static void DetectPacketHeader(void *pPacket);
};


#endif 
