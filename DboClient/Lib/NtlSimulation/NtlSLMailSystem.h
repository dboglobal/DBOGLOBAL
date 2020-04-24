#ifndef __NTL_MAIL_SYSTEM_H__
#define __NTL_MAIL_SYSTEM_H__

// core
#include "ceventhandler.h"
#include "NtlMail.h"

class CNtlMailSystem : public RWS::CEventHandler
{
public:
	enum eMAILSYSTEM_ABILITY_TYPE
	{
		MAILSYSTEM_ABILITY_NONE				= 0x00000000,
		MAILSYSTEM_ABILITY_SEND				= 0x00000001,
		MAILSYSTEM_ABILITY_READ				= 0x00000002,
		MAILSYSTEM_ABILITY_ATTACH_SEND		= 0x00000004,	
		MAILSYSTEM_ABILITY_ATTACH_ACCEPT	= 0x00000008,
		MAILSYSTEM_ABILITY_ALL				= 0xFFFFFFFF,
	};

	enum eMAILSYSTEM_NOTIFY_TYPE
	{
		MAILSYSTEM_NOTIFY_TYPE_NONE		= 0x00000000,
		MAILSYSTEM_NOTIFY_TYPE_NEWMAIL	= 0x00000001,	///< 새로운 메일이 있다는 것을 알림
	};

	enum eMAILSYSTEM_NOTIFY_VALUE
	{
		MAILSYSTEM_NOTIFY_VALUE_NONE			= 0x00000000,
		MAILSYSTEM_NOTIFY_VALUE_MAIL_NORMAL		= 0x00000001,	///< 일반 메일 존재
		MAILSYSTEM_NOTIFY_VALUE_MAIL_MANAGER	= 0x00000002,	///< GM 메일 존재
		MAILSYSTEM_NOTIFY_VALUE_MAIL_FULL		= 0x00000004,	///< 메일이 가득참
	};

	typedef std::vector<MAILID>					MAIL_ID_VEC;
	typedef std::map<MAILID, sMAIL_PROFILE*>	MAIL_PROFILE_MAP;

protected:
	SERIAL_HANDLE		m_hObject;
	MAIL_ID_VEC			m_vecMailID;
	MAIL_PROFILE_MAP	m_mapMailProfile;
	
	RwUInt32			m_uiMailNum;
	
	RwBool				m_bAway;	
	RwUInt32			m_uiNotify;
	RwBool				m_bMailStart;

	RwUInt32			m_uiMailCount;
	RwUInt32			m_uiUnreadMailCountNormal;
	RwUInt32			m_uiUnreadMailCountManager;
	RwUInt32			m_uiOldUnreadMailCountNormal;
	RwUInt32			m_uiOldUnreadMailCountManager;

public:
	CNtlMailSystem();
	virtual ~CNtlMailSystem();

	virtual	RwBool		Create();
	void				Destroy();

	virtual	void		HandleEvents(RWS::CMsg &pMsg);

public:
	RwBool				GetMail(sMAIL_PROFILE& outMailProfile, const MAILID& mailID);
	RwBool				GetMail(sMAIL_PROFILE& outMailProfile, const RwUInt32& uiIndex, const RwBool& bReverse/*가장 최신은 제일 뒤*/);

	RwUInt32			GetAbility();
	SERIAL_HANDLE		GetUseObjectSerial()		{ return m_hObject; }

	RwUInt32			GetMailCount()				{ return m_uiMailCount; }
	RwUInt32			GetUnreadMailCountNormal()	{ return m_uiUnreadMailCountNormal; }
	RwUInt32			GetUnreadMailCountManager()	{ return m_uiUnreadMailCountManager; }
	
	RwBool				IsAway()					{ return m_bAway; }

protected:
	RwUInt32			AddMail(const sMAIL_PROFILE* pMailProfile);
	RwUInt32			DelMail(const MAILID& mailID);
	sMAIL_PROFILE*		GetMail(const MAILID& mailID);

	void				ClearMail();
	void				ReloadMail(RwUInt8 uiCount, MAILID* pmailID);

	void				SortMail();
	static bool			SortMailCompare(const MAILID& mailIDLeft, const MAILID& mailIDRight);

	void				CheckMailSystemNotify(RwUInt32 uiMailCount, RwUInt32 uiUnreadMailCountNormal, RwUInt32 uiUnreadMailCountManager);
};

#endif