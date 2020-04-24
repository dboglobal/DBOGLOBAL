#ifndef __INC_DBOG_OBJECTMSG_H__
#define __INC_DBOG_OBJECTMSG_H__

#include "NtlLinkList.h"
#include "NtlObject.h"


enum eOBJMSG_ID
{
	OBJMSG_YOU_HIT,
	OBJMSG_YOU_HIT_BY_SKILL,
	OBJMSG_YOU_HIT_HTB,
	OBJMSG_YOU_HELP_ME,
	OBJMSG_YOU_KEEP_AGGRO,
	OBJMSG_TRIGGER_EVENT,
	OBJMSG_TQS_EVENT,
	OBJMSG_YOU_FACED,
	OBJMSG_YOU_ALLIANCE_HELP_ME,
	OBJMSG_WPS_EVENT,
	OBJMSG_SEND_WPS_EVENT_TO_SPS,
	OBJMSG_SEND_ITEM_EVENT_TO_SPS,
	OBJMSG_SEND_WPS_EVENT_TO_AIS,
	OBJMSG_SEND_TQS_EVENT_TO_AIS,
	OBJMSG_CHAR_TURNING_END,
	OBJMSG_START_ESCORT,

	
	OBJMSG_SEND_TS_EVENT_TO_WPS,
	OBJMSG_SEND_SPS_EVENT_TO_WPS,
	OBJMSG_SEND_AI_EVENT_TO_WPS,
	OBJMSG_SEND_ITEM_EVENT_TO_WPS,
	OBJMSG_SEND_TQS_EVENT_TO_WPS,
	OBJMSG_SEND_WPS_EVENT_TO_WPS,
	OBJMSG_SEND_SPS_EVENT,

	OBJMSG_COUNT,
};

//--------------------------------------------------------------------------------------//
//	OBJECT MESSAGT
//--------------------------------------------------------------------------------------//
class CObjectMsg
{
	friend class CObjectMsgQueue;

public:

	CObjectMsg(int msgID);
	virtual ~CObjectMsg();

public:

	DWORD					GetTime() { return m_dwTime; }

	int						GetID() { return m_msgID; }

	int						GetRefCount() { return m_refCount; }

	void					AddRef(int nCount) { m_refCount += nCount; }

public:

	static int				Release(CObjectMsg* pObjMsg);

private:

	int						m_msgID;

	DWORD					m_dwTime;

	int						m_refCount;
};


//--------------------------------------------------------------------------------------//
//	OBJECT MESSAGE LINGER
//--------------------------------------------------------------------------------------//
class CObjectMsgLinker : public CNtlLinkObject
{
	friend class CObjectMsgQueue;

public:

	CObjectMsgLinker()
		:m_pObjectMsg(NULL) {}

	virtual ~CObjectMsgLinker() {}

private:

	CObjectMsg *		m_pObjectMsg;

};


//--------------------------------------------------------------------------------------//
//	OBJECT MESSAGT QUEUE
//--------------------------------------------------------------------------------------//
class CObjectMsgQueue
{

public:

	CObjectMsgQueue();
	virtual ~CObjectMsgQueue();

public:

	void				Clear();

	void				Push(CObjectMsg *pObjectMsg, DWORD dwDelayTime);

	CObjectMsg*			Pop(DWORD dwProcessTime);

	void				UpdateTick(DWORD dwTickDiff);

private:

	CNtlLinkList		m_queue;
};



//--------------------------------------------------------------------------------------//
//	OBJECT MESSAGE CLASSES
//--------------------------------------------------------------------------------------//
class CObjMsg_YouHit : public CObjectMsg
{
public:

	CObjMsg_YouHit()
		:CObjectMsg(OBJMSG_YOU_HIT) {}

	virtual ~CObjMsg_YouHit() {}

public:

	HOBJECT		hSource;
	BYTE		byAttackResult;
	int			attackResultValue;
	BYTE		byAttackSequence;
	CNtlVector	vShift;
};


class CObjMsg_YouHitBySkill : public CObjectMsg
{
public:

	CObjMsg_YouHitBySkill()
		:CObjectMsg(OBJMSG_YOU_HIT_BY_SKILL) {}

	virtual ~CObjMsg_YouHitBySkill() {}

public:

	HOBJECT						hSource;
	TBLIDX						skillId;
	BYTE						byAttackResult;
	sDBO_SYSTEM_EFFECT_RESULT	aEffectResult[NTL_MAX_EFFECT_IN_SKILL];
	CNtlVector					vShift;
};


class CObjMsg_YouHitByHTB : public CObjectMsg
{
public:

	CObjMsg_YouHitByHTB()
		:CObjectMsg(OBJMSG_YOU_HIT_HTB) {}

	virtual ~CObjMsg_YouHitByHTB() {}

public:

	HOBJECT		hSource;
	WORD		wResultValue;
};


class CObjMsg_YouHelpMe : public CObjectMsg
{
public:

	CObjMsg_YouHelpMe()
		:CObjectMsg(OBJMSG_YOU_HELP_ME) {}

	virtual ~CObjMsg_YouHelpMe() {}

public:

	HOBJECT		hSource;
	HOBJECT		hTarget;
	DWORD		dwAggroPoint;
	PARTYID		partyID;
};


class CObjMsg_YouKeepAggro : public CObjectMsg
{
public:

	CObjMsg_YouKeepAggro()
		:CObjectMsg(OBJMSG_YOU_KEEP_AGGRO) {}

	virtual ~CObjMsg_YouKeepAggro() {}

public:

	HOBJECT		hSource;
	HOBJECT		hProvoker;
	DWORD		dwAggroPoint;
};


class CObjMsg_TriggerEvent : public CObjectMsg
{
public:

	CObjMsg_TriggerEvent()
		:CObjectMsg(OBJMSG_TRIGGER_EVENT) {}

	virtual ~CObjMsg_TriggerEvent() {}

public:

	HOBJECT			hSource;
	BYTE			byTriggerType;
	NTL_TS_EVENT_ID	teid;
};


class CObjMsg_TqsEvent : public CObjectMsg
{
public:

	CObjMsg_TqsEvent()
		:CObjectMsg(OBJMSG_TQS_EVENT) {}

	virtual ~CObjMsg_TqsEvent() {}

public:

	DWORD			eventID;
};


class CObjMsg_YouFaced : public CObjectMsg
{
public:

	CObjMsg_YouFaced()
		:CObjectMsg(OBJMSG_YOU_FACED) {}

	virtual ~CObjMsg_YouFaced() {}

public:

	HOBJECT			hSource;
};


class CObjMsg_AllianceYouHelpMe : public CObjectMsg
{
public:

	CObjMsg_AllianceYouHelpMe()
		:CObjectMsg(OBJMSG_YOU_ALLIANCE_HELP_ME) {}

	virtual ~CObjMsg_AllianceYouHelpMe() {}

public:

	HOBJECT			hRequester;
};


class CObjMsg_WpsEvent : public CObjectMsg
{
public:

	CObjMsg_WpsEvent()
		:CObjectMsg(OBJMSG_WPS_EVENT) {}

	virtual ~CObjMsg_WpsEvent() {}

public:

	DWORD			eventID;
};


class CObjMsg_SendWpsEventToSps : public CObjectMsg
{
public:

	CObjMsg_SendWpsEventToSps()
		:CObjectMsg(OBJMSG_SEND_WPS_EVENT_TO_SPS) {}

	virtual ~CObjMsg_SendWpsEventToSps() {}

public:

	DWORD			eventID;
};


class CObjMsg_SendItemEventToSps : public CObjectMsg
{
public:

	CObjMsg_SendItemEventToSps()
		:CObjectMsg(OBJMSG_SEND_ITEM_EVENT_TO_SPS) {}

	virtual ~CObjMsg_SendItemEventToSps() {}

public:

	TBLIDX			itemTblidx;
};


class CObjMsg_SendWpsEventToAis : public CObjectMsg
{
public:

	CObjMsg_SendWpsEventToAis()
		:CObjectMsg(OBJMSG_SEND_WPS_EVENT_TO_AIS) {}

	virtual ~CObjMsg_SendWpsEventToAis() {}

public:

	DWORD			eventId;
};


class CObjMsg_SendTqsEventToAis : public CObjectMsg
{
public:

	CObjMsg_SendTqsEventToAis()
		:CObjectMsg(OBJMSG_SEND_TQS_EVENT_TO_AIS) {}

	virtual ~CObjMsg_SendTqsEventToAis() {}

public:

	DWORD			eventId;
};


class CObjMsg_CharTurningEnd : public CObjectMsg
{
public:

	CObjMsg_CharTurningEnd()
		:CObjectMsg(OBJMSG_CHAR_TURNING_END) {}

	virtual ~CObjMsg_CharTurningEnd() {}

public:

	TBLIDX			directTblidx;
};


class CObjMsg_StartEscort : public CObjectMsg
{
public:

	CObjMsg_StartEscort()
		:CObjectMsg(OBJMSG_START_ESCORT) {}

	virtual ~CObjMsg_StartEscort() {}

public:

	BYTE			m_byEscortType;	//eESCORT_TYPE
};


class CObjMsg_SendTsEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendTsEventToWps()
		:CObjectMsg(OBJMSG_SEND_TS_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendTsEventToWps() {}

public:

	unsigned int	teId;
	BYTE			byTriggerType;


};

class CObjMsg_SendSpsEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendSpsEventToWps()
		:CObjectMsg(OBJMSG_SEND_SPS_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendSpsEventToWps() {}

public:

	BYTE				eObjType;

	TBLIDX				objectTblidx;

	DWORD				eventId;
	
};


class CObjMsg_SendAisEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendAisEventToWps()
		:CObjectMsg(OBJMSG_SEND_AI_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendAisEventToWps() {}

public:

	BYTE				eObjType;

	TBLIDX				objectTblidx;

	DWORD				eventId;

};

class CObjMsg_SendItemEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendItemEventToWps()
		:CObjectMsg(OBJMSG_SEND_ITEM_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendItemEventToWps() {}

public:

	TBLIDX				itemTblidx;

};

class CObjMsg_SendTqsEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendTqsEventToWps()
		:CObjectMsg(OBJMSG_SEND_TQS_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendTqsEventToWps() {}

public:

	DWORD				eventId;

};

class CObjMsg_SendWpsEventToWps : public CObjectMsg
{
public:

	CObjMsg_SendWpsEventToWps()
		:CObjectMsg(OBJMSG_SEND_WPS_EVENT_TO_WPS) {}

	virtual ~CObjMsg_SendWpsEventToWps() {}

public:

	DWORD				eventId;

	TBLIDX				wpsTblidx;

};


class CObjMsg_SendSpsEvent : public CObjectMsg
{
public:

	CObjMsg_SendSpsEvent()
		:CObjectMsg(OBJMSG_SEND_SPS_EVENT) {}

	virtual ~CObjMsg_SendSpsEvent() {}

public:


	TBLIDX				targetTblidx;

	DWORD				eventId;

};



#endif