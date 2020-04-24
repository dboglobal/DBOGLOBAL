#ifndef __INC_DBOG_PACKET_EVENT_H__
#define __INC_DBOG_PACKET_EVENT_H__


#include "PacketEventObj.h"
#include "NtlMutex.h"


class CPacketEvent
{

public:

	CPacketEvent();
	virtual ~CPacketEvent();

public:

	void						Init();

	void						Destroy();

	void						ProcessEventQueue();

	void						PostEvent(CPacketEventObj* pEvent);


protected:

	void						ReleaseEvent();

	void						ProcessEvent();

private:

	CNtlMutex					m_eventQueueMutex;

	CNtlLinkList				m_eventQueue;

	CNtlLinkList				m_processQueue;

};


#endif