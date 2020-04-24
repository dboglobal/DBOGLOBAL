/*****************************************************************************
 *
 * File			: NtlEventQueue.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 08. 09	
 * Abstract		: Simulation layer event queue.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_EVENT_QUEUE_H__
#define __NTL_EVENT_QUEUE_H__

#include <list>
#include "ceventhandler.h"

/**
 * \ingroup Client
 * simulation layer event queue slot.
 *
 */
class CNtlEventQueueSlot
{
private:
	
	enum { MAX_EVENT_DATA_LEN = 4096 };

public:

	RwReal m_fDelayTime;					/**< event delay time */
	RwReal m_fCurrTime;						/**< event current time */
	RWS::CMsg m_Msg;						/**< event message */
	RwInt32 m_iDataLen;						/**< event data length */
	RwChar m_chData[MAX_EVENT_DATA_LEN];	/**< event data buffer */

	static RwFreeList *m_pFreeList;

public:

	CNtlEventQueueSlot(RWS::CMsg &Msg, void *pData, RwInt32 iDataLen, RwReal fDelayTime);
	/**
	*  memory pool을 사용하여 SEventData 메모리를 할당하는 함수.
	*  renerware의 RwFreeList를 사용하고 있다.
	*/
	static RwBool AllocFreeList(void);
	static void FreeFreeList(void);

	void* operator new(size_t size);
	void operator delete(void *pObj);
};

/**
 * \ingroup Client
 * simulation layer event queue.
 *
 */

class CNtlEventQueue
{
public:
	
	enum { MAX_EVENT_QUEUE_FILTER = 10 };
	
private:
	
	std::list<CNtlEventQueueSlot*> m_listBuffer;	/**< event list */
	RWS::CMsg m_msgFileter[MAX_EVENT_QUEUE_FILTER];
	RwUInt32 m_iFilterNum;

	static RwFreeList *m_pFreeList;			/**< event data memory pool */
	
public:

	/**
    *  Default constructor for event queue
    *
    */
	CNtlEventQueue();

	/**
    *  Virtual destructor for event queue
    *
    */
	~CNtlEventQueue();

	/**
	*  class의 멤버 접근은 이 함수를 사용한다.
    *
    */
	static CNtlEventQueue* GetInstance(void);

	/**
    *  create 함수.
	*  \return create에 성공하면 TRUE, 실패하면 FALSE
    *  \see Destroy
    */
	RwBool Create(void);

	/**
    *  destroy 함수.
	*  \see Create
    */
	void Destroy(void);

	/**
    *  event를 queue에 push하는 함수.
	*  \return push에 성공하면 TRUE, 실패하면 FALSE
	*  \param &Msg event message data
	*  \param *pData event data data
	*  \param iDataLen event data length
	*  \param fDelayTime event process delay time
    */
	RwBool Push(RWS::CMsg &Msg, void *pData, RwInt32 iDataLen, RwReal fDelayTime);

	/**
    *  queue에 쌓인 event를 처리하는 함수.
	*  \param fElapsed delta time
    */
	void Process(RwReal fElapsed);

	/**
    *  queue에 쌓인 event를 filtering 한다.
	*  \param fElapsed delta time
    */
	void CurrentProcessFilterMsg(RWS::CEventId& id);

	/**
    *  queue에 쌓인 event를 filtering 한다.
	*  \param fElapsed delta time
    */
	RwBool IsFilterMsg(RWS::CMsg& msg);
};


void PostMsg(RWS::CMsg &Msg, void *pData, RwInt32 iDataLen, RwReal fDelayTime = 0.0f); 
void EventQueueCurrentProcessFilterMsg(RWS::CEventId& id);

#endif