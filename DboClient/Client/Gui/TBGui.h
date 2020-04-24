////////////////////////////////////////////////////////////////////////////////
// File: TBGui.h
// Desc: Tenkaichi Budokai Gui Group
//       천하제일 무도회 관련 Gui. 시즌, 매치에 맞게 생성 삭제된다.
//
// 2007.08.23 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_GUI_H__
#define __TB_GUI_H__

#include "ceventhandler.h"

class CTBMinorMatchInfoGui;
class CTBMinorDirectionGui;
class CTBMajorMatchInfoGui;
class CTBMajorDirectionGui;
class CTBFinalMatchInfoGui;
class CTBFinalDirectionGui;
class CTBTournamentMatchInfoGui;
class CTBTournamentGui;
class CTBNewsGui;
class CTBRequestGui;
class CTBMinorMatchResultGui;
class CTBMatchResultGui;
class CTBAwardResultGui;
class CResultNarrationGui;
class CCounterGui;
class CTBMatchObserverVersusGui;
class CTBMatchIndiObRedGui;
class CTBMatchIndiObBlueGui;
class CTBMatchTeamObRedGui;
class CTBMatchTeamObBlueGui;

#define TB_MATCH_RESULT_REMAIN_TIME		10.99f

class CTBGui : public RWS::CEventHandler
{
public:
	//! Destructor
	~CTBGui(VOID) {}

	//! Operation
	static	VOID CreateInstance(VOID);
	static	VOID DeleteInstance(VOID);

	VOID	Create( RwUInt8 byMatchType );
	VOID	Destroy(VOID);

	// 소식지 생성 및 삭제
	VOID	CreateBudokaiNewsOnce();	///< 로딩 후 한번만 실행
	VOID	CreateBudokaiNews();
	VOID	DestroyBudokaiNews();
	
	//! Access
	static CTBGui*	GetInstance(VOID);

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	// 각 Match State의 Update 때 참가자와 관전자를 구분하여 처리한다.
	VOID	HandleEventMinorMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventMinorMatchStateUpdateObserver( RWS::CMsg& msg );
	VOID	HandleEventMajorMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventMajorMatchStateUpdateObserver( RWS::CMsg& msg );
	VOID	HandleEventFinalMatchStateUpdateEntrance( RWS::CMsg& msg );
	VOID	HandleEventFinalMatchStateUpdateObserver( RWS::CMsg& msg );
	
	// 객체가 중간에 생성 되었을 때 SetGauge를 해주기 위해
	VOID	HandleEventSobCreate( RWS::CMsg& msg );

	//! Implements
	VOID	SetPlayerState( RwUInt32 hSerial );
	VOID	SetBattleGauge(VOID);
	VOID	UnsetBattleGauge(VOID);
	VOID	ProgressMessageNotify( RwUInt8 byMsgId );
	VOID	DirectionMode( RwBool bDirection );
	VOID	SetCounterPosition( RwInt32 nScreenHeight );
	VOID	SetCounterTime( RwReal fTime );
	VOID	HideCounterTime(VOID);

	//! Variable
	CTBMinorMatchInfoGui*		m_pMinorMatchInfoGui;
	CTBMinorDirectionGui*		m_pMinorDirectionGui;
	CTBMajorMatchInfoGui*		m_pMajorMatchInfoGui;
	CTBMajorDirectionGui*		m_pMajorDirectionGui;
	CTBFinalMatchInfoGui*		m_pFinalMatchInfoGui;
	CTBFinalDirectionGui*		m_pFinalDirectionGui;	
	CTBTournamentGui*			m_pTournamentGui;
	CTBTournamentMatchInfoGui*	m_pTournamentMatchInfoGui;
	CTBMatchResultGui*			m_pMatchResultGui;
	CTBAwardResultGui*			m_pAwardResultGui;
	CTBMinorMatchResultGui*		m_pMinorMatchResultGui;
	CResultNarrationGui*		m_pResultNarration;
	CCounterGui*				m_pCounter;

	// 관전자 GUI
	CTBMatchObserverVersusGui*	m_pObserverVersusGui;
	CTBMatchIndiObRedGui*		m_pMatchIndiObRedGui;
	CTBMatchIndiObBlueGui*		m_pMatchIndiObBlueGui;
	CTBMatchTeamObRedGui*		m_pMatchTeamObRedGui;
	CTBMatchTeamObBlueGui*		m_pMatchTeamObBlueGui;

	// 천하제일 무도회 소식지
	CTBNewsGui*					m_pNewsGui;
	CTBRequestGui*				m_pRequestGui;

	RwBool						m_bCreate;
	
	static CTBGui*				m_pInstance;

	//! Prevented default constructor
	CTBGui(VOID);
};

static CTBGui* GetTBGuiInstance(VOID)
{
	return CTBGui::GetInstance();
}

#endif//__TB_GUI_H__