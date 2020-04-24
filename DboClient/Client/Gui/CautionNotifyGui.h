/*****************************************************************************
 *
 * File			: TimeFixedNotifyGui.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 12. 06	
 * Abstract		: DBO time fixed notify gui.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __TIMEFIXED_NOTIFY_GUI_H__
#define __TIMEFIXED_NOTIFY_GUI_H__

// core
#include "NtlDebug.h"
#include "NtlCallbackWidget.h"
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

#define CAUTION_NOTIFY_DEFAULT_ALPHA	255
#define CAUTION_NOTIFY_LIFE_TIME		4.0f
#define CAUTION_NOTIFY_FADEOUT_TIME		2.0f
#define CAUTION_NOTIFY_YPOS_RATE		0.293f

#define dDEFAULT_NOTIFY_TIMER_INVERVAL_TIME	1.0f

#define dFREEPVP_OUTSIDE_STAGE_LIMIT_TIME	10.0f

class CCautionNotifyGui;

/**
* \ingroup client
* \brief IntervalTime 마다 Callback을 날리는 Timer
*/
class CNotifyTimer
{
public:
	CNotifyTimer();
	CNotifyTimer(RwReal fIntervalTime);
	~CNotifyTimer();

	RwReal	GetIntervalTime()						{ return m_fIntervalTime; }
	void	SetIntervalTime(RwReal fIntervalTime);
	
	void	Update(RwReal fElapsed);

	template <class Callbackclass>
	void	LinkIntervalTimer(Callbackclass *cbclass, int (Callbackclass::*callback)(RwReal fIntervalTime));
	void	UnLinkIntervalTimer(void);

protected:
	RwReal	m_fCurrentTime;
	RwReal	m_fIntervalTime;

	CNtlCallbackParam1float	*m_pCallTimer;
};

template <class Callbackclass>
void CNotifyTimer::LinkIntervalTimer(Callbackclass *cbclass, int (Callbackclass::*callback)(RwReal))
{
	NTL_PRE(m_pCallTimer == 0 );
	m_pCallTimer = NTL_NEW CNtlCallbackWidget1float<Callbackclass>(cbclass,callback);
}

/**
* \ingroup client
* \brief 매시간마다 일정하게 출력되는 Caution Notify
*/
class CCautionNotifyContinuance
{
public:
	CCautionNotifyContinuance();
	CCautionNotifyContinuance(CCautionNotifyGui* pNotify, std::string& strStringID, RwReal fLimitTime, RwReal fIntervalTime);
	~CCautionNotifyContinuance();

	void		Update(RwReal fElapsed);
	RwReal		IsRemain();
	std::string	GetStringID()				{ return m_strStringID; }

	int			OnIntervalTimer(RwReal fIntervalTime);

protected:
	std::string			m_strStringID;
	RwReal				m_fRemainTime;
	CCautionNotifyGui*	m_pNotifyGui;
	CNotifyTimer		m_Timer;
};

/**
* \ingroup client
* \brief 화면 중앙에 뜨는 경고 메시지를 띄우는 GUI, 누적되면 Caution SideIcon쪽으로 메시지를 보낸다.
*/
class CCautionNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef VOID (CCautionNotifyGui::*HandleFunc)(RwBool, std::string);
	typedef std::map<std::string, HandleFunc>			HANDLEMAP;
	typedef std::list< CCautionNotifyContinuance* >	CONTINUANCELIST;

public:
	CCautionNotifyGui();
	CCautionNotifyGui(const RwChar* pName);
	~CCautionNotifyGui();

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	VOID	HandleEvents( RWS::CMsg& msg );

	VOID	ShowCautionNotify( RwBool bActive, std::string& strStringID, const WCHAR* pwcString );

protected:
	VOID	PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	SetCurrentCautionNotify(std::string& strStringID, const WCHAR* pwcString );
	VOID	ClearCurrentCautionNotify();

	// Handler for each other string id
	VOID	FreePvpOutSideHandler(RwBool bActive, std::string strStringID);
	
protected:
	VOID	RegisterHandler();
	VOID	AddContinuance(std::string& strStringID, RwReal fLimitTime, RwReal fIntervalTime );
	VOID	DeleteContinuance(std::string& strStringID);

protected:
	HANDLEMAP			m_mapNotifyHandler;
	CONTINUANCELIST		m_listContinuance;

	gui::CStaticBox*	m_pStbOut;
	
	RwBool				m_bFinish;
	RwReal				m_fLifeTime;
	RwReal				m_fCurrentTime;

	RwBool				m_bFadeOut;
	RwReal				m_fFadeOutTime;

	std::string			m_strStringID;
};

#endif