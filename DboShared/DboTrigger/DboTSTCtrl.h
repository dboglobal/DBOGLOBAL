#ifndef _DBO_TSTCTRL_H_
#define _DBO_TSTCTRL_H_


#include "DboTSCoreDefine.h"


class CDboTSTAgency;
class CDboTSActETimerS;


/**
	Trigger controller
	Is responsible to process the data and that the operation routine of the trigger
*/


#define MAKE_EXCEPTION_TIMER_KEY( tcId, taId )      ((WORD)(((BYTE)((DWORD_PTR)(tcId) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(taId) & 0xff))) << 8))
#define GET_TC_ID( EKey )							((BYTE)((DWORD_PTR)(EKey) & 0xff))
#define GET_TA_ID( EKey )							((BYTE)((DWORD_PTR)(EKey) >> 8))


class CDboTSTCtrl : public CNtlTSController
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef std::deque<unsigned int> deqdef_PARAMETER;
	typedef std::map<WORD, WORD> hashdef_ENTITY_LIST;

	struct sEXCEPTION_TIMER
	{
		sEXCEPTION_TIMER( void )
		{
			tcId = NTL_TS_TC_ID_INVALID;
			taId = NTL_TS_TA_ID_INVALID;

			uiCurTime = 0;
			uiRemainTime = 0;

			pActExceptTimer = 0;
		}

		NTL_TS_TC_ID					tcId;
		NTL_TS_TA_ID					taId;

		unsigned int					uiCurTime;
		unsigned int					uiRemainTime;

		CDboTSActETimerS*				pActExceptTimer;
	};
	typedef std::map<unsigned int, sEXCEPTION_TIMER> mapdef_EXCEPT_TIMER_SLOT;

	struct sGROPU_UPDATE_INFO
	{
		CNtlTSGroup*					pCurGroup;
		CNtlTSCont*						pCurCont;
		int								nActionRef;
	};
	typedef std::map<NTL_TS_TG_ID, sGROPU_UPDATE_INFO> mapdef_GROUP_UPDATE_INFO;

	struct sCONT_SYNC_QUEUE
	{
		hashdef_ENTITY_LIST				defContSyncQueue;		// 동기화 컨테이너 액션 리스트
	};

// Member variables
protected:
	CDboTSTAgency*						m_pParent;				// 부모 ( Agency )

	unsigned int						m_uiCurState;			// 트리거 시스템의 현재 상태
	CNtlTSCont*							m_pCurTSP;				// 현재 진행중인 TSP
	mapdef_EXCEPT_TIMER_SLOT			m_defExceptTimerSlot;	// 예외 타이머
	sSTORAGE_SLOT_MEMORY				m_sSSM;					// 저장 슬롯 메모리
	sCONT_SYNC_QUEUE					m_sSyncQueue;			// 동기화 컨테이너 큐

	// TSP Waiting progress
	unsigned int						m_uiTWPeriod;
	unsigned int						m_uiTWStart;

	// Event data
	unsigned char						m_byEventType;
	unsigned int						m_uiEventData;

	// Event-related temporary variables
	eEVENT_GEN_TYPE						m_eEvtGenType;
	unsigned int						m_uiEvtGenId;
	SScouterUseTargetInfo				m_sScouterUseTargetInfo;
	SSkillUseInfo						m_sSkillUserInfo;
	vecdef_ItemTargetList				m_defItemTargetList;
	sCOL_RGN_DATA						m_sColRgnData;
	SRB_DATA							m_sRBData;
	SMobTargetInfo						m_sMobTargetData;

	// Group update Related variables
	mapdef_GROUP_UPDATE_INFO			m_defGroupUpdateList;

// Constructions
public:
	CDboTSTCtrl( void );
	virtual ~CDboTSTCtrl( void );

// Methods
public:
	// TS Renewal
	virtual void						Update( void );

	// Check whether you can proceed with the trigger
	bool								CanProgressClickObject( unsigned int uiObjId );
	bool								CanProgressCollisionObject( unsigned int uiObjId );
	bool								CanProgressClickNPC( unsigned int uiNPCId );
	bool								CanProgressItemUse( unsigned int uiItemIdx, vecdef_ItemTargetList* pdefItemTargetList );
	bool								CanProgressItemGet( unsigned int uiItemIdx );
	bool								CanProgressItemEquip( unsigned int uiItemIdx );
	bool								CanProgressScoutUse( unsigned int uiItemIdx, SScouterUseTargetInfo* pTargetInfo );
	bool								CanProgressRcvSvrEvt( unsigned int uiEvtID );
	bool								CanProgressSkillUse( unsigned int uiSkillIdx, SSkillUseInfo* psSkillUseInfo );
	bool								CanProgressColRgn( sCOL_RGN_DATA* pColRgnData );
	bool								CanProgressRB( unsigned int uiRBTblIdx, SRB_DATA* psRBData );
	bool								CanProgressClickMob( unsigned int uiMobGroupID, SMobTargetInfo* pMobTargetData );
	bool								CanProgressBindStone( void );
	bool								CanProgressSearchQuest( void );
	bool								CanProgressItemUpgrade( void );
	bool								CanProgressTeleport( void );
	bool								CanProgressBudokai( void );
	bool								CanProgressSlotMachine( void );
	bool								CanProgressHoipoiMix( void );
	bool								CanProgressPrivateShop( void );
	bool								CanProgressFreeBattle( void );
	bool								CanProgressItemIdentity( void );
	bool								CanProgressUseMail( void );
	bool								CanProgressParty( void );

	bool								CanProgressFLink(void);
	bool								CanProgressClickSideIcon(void);
	bool								CanProgressLevelCheck(void);
	bool								CanProgressQuest(void);
	bool								CanProgressDialogOpen(void);


	// Parents ( Agency )
	CDboTSTAgency*						GetParent( void );
	void								SetParent( CDboTSTAgency* pParent );

	CNtlTSCont*							GetCurTSP( void );

	// Current state of the trigger system
	unsigned int						GetCurState( void ) const;
	void								SetCurState( unsigned int uiState );

	bool								IsError( void ) const;					// TS 에러
	bool								IsFailed( void ) const;					// TS 진행 실패 ( 게임적인 실패 )
	bool								IsCleared( void ) const;				// 퀘스트 클리어 상태
	bool								IsCSComunication( void ) const;			// 클라이언트와 서버의 통신중
	bool								IsClientWait( void ) const;				// 클라이언트와 통신중
	bool								IsSvrWait( void ) const;				// 서버와 통신중
	bool								IsExitState( void ) const;				// 종료 상태
	bool								IsUIProgress( void ) const;				// UI 쪽에 진행 상태를 알려주기 위한 플래그
	bool								IsSvrComAfterClientWait( void ) const;	// 서버와 통신후 클라이언트 진행 대기
	bool								IsEscort( void ) const;					// 호위
	bool								IsSvrEvtWorldRange( void ) const;		// 서버이벤트(몹킬...)의 적용 대상이 월드 ( 이 플래그가 켜져 있지 않으면 파티 기준으로 처리됨 )
	bool								IsWPSMode( void ) const;				// World play script 액션이 동작중
	bool								IsUserOutFailedWPS( void ) const;		// World play script가 동작중 유저가 로그 아웃 한 경우 Failed 처리 되어야 하는 경우를 저장하는 플래그
	bool								IsSuccess( void ) const;				// Success
	bool								IsComplete( void ) const;				// Complete

	void								SetError( bool bOn = true, bool bUpdate = true );
	void								SetFailed( bool bOn = true, bool bUpdate = true );
	void								SetCleared( bool bOn = true, bool bUpdate = true );
	void								SetCSComunication( bool bOn = true, bool bUpdate = true );
	void								SetClientWait( bool bOn = true, bool bUpdate = true );
	void								SetSvrWait( bool bOn = true, bool bUpdate = true );
	void								SetExitState( bool bOn = true, bool bUpdate = true );
	void								SetSvrComAfterClientWait( bool bOn = true, bool bUpdate = true );
	void								SetEscort( bool bOn = true, bool bUpdate = true );
	void								SetSvrEvtWorldRange( bool bOn = true, bool bUpdate = true );
	void								SetWPSMode( bool bOn = true, bool bUpdate = true );
	void								SetUserOutFailedWPS( bool bOn = true, bool bUpdate = true );
	void								SetSuccess( bool bOn = true, bool bUpdate = true );
	void								SetComplete( bool bOn = true, bool bUpdate = true );

	// Exceptions timer
	void								AttachExceptionTimer( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, unsigned int uiCurTime, unsigned int uiRemainTime );
	void								DetachExceptionTimer( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								UpdateExceptionTimer( void );

	sEXCEPTION_TIMER*					GetTimeLimitInfo( void );

	// Storage slot Memory
	unsigned int						GetSSM( eSSM_ID eSSMId ) const;
	void								SetSSM( eSSM_ID eSSMId, unsigned int uiVal );

	// Synchronization queue container
	// Synchronization queue is a container with only the main target group
	bool								IsContSyncQueueEmpty( void ) const;
	void								AddContSyncQueue( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								DelContSyncQueue( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );

	// TSP Waiting progress
	bool								IsTimeWait( void ) const;
	void								StartTimeWait( unsigned int uiTime );

	// Event data
	unsigned char						GetEventType( void );
	unsigned int						GetEventData( void );
	void								SetEventData( unsigned char byEventType, unsigned int uiEventData );
	void								InitEventData( void );

	// Event-related temporary use functions
	eEVENT_GEN_TYPE						GetEventGenType( void ) const;
	void								SetEventGenType( eEVENT_GEN_TYPE eEvtGenType );

	unsigned int						GetEventGenId( void ) const;
	void								SetEventGenId( unsigned int uiEvtGenId );

	SScouterUseTargetInfo&				GetEventGenScouterUseTargetInfo( void );
	void								SetEventGenScouterUseTargetInfo( SScouterUseTargetInfo* psTargetInfo );

	SSkillUseInfo&						GetEventGenSkillUseInfo( void );
	void								SetEventGenSkillUseInfo( SSkillUseInfo* psSkillUseInfo );

	vecdef_ItemTargetList&				GetEventGenItemTargetInfo( void );
	void								SetEventGenItemTargetInfo( vecdef_ItemTargetList* pdefItemTargetList );

	sCOL_RGN_DATA&						GetEventGenColRgnInfo( void );
	void								SetEventGenColRgnInfo( sCOL_RGN_DATA* pColRgnData );

	SRB_DATA&							GetEventGenRBData( void );
	void								SetEventGenRBData( SRB_DATA* pRBData );

	SMobTargetInfo&						GetEventGenMobTargetData( void );
	void								SetEventGenMobTargetData( SMobTargetInfo* pMobTargetData );

	void								ClearAllEventGenInfo( void );

	virtual unsigned int				GetCurTime( void );

	virtual void						AttachUpdateExceptionGroup( NTL_TS_TG_ID tgId );

// Implementations
protected:
	// Trigger step progress update
	virtual void						UpdateTSStep( void );

	// Exception group updates
	virtual bool						IfInterestingDoExceptionGroup( NTL_TS_TG_ID ) const { return true; }
	virtual void						UpdateExceptionGroup( void );
	virtual void						AutoDetachedUpdateExceptionGroup( CNtlTSGroup* pAutoDetachedGroup );

	virtual void						BeginExceptionAction( sTS_KEY& sKey ) { UNREFERENCED_PARAMETER( sKey ); }
	virtual void						EndExceptionAction( sTS_KEY& sKey ) { UNREFERENCED_PARAMETER( sKey ); }

	virtual void						AddUpdateExceptionGroupActRef( NTL_TS_TG_ID tgId );
	virtual void						ReleaseUpdateExceptionGroupActRef( NTL_TS_TG_ID tgId );

	// Proceeding in accordance with the updated TSP waiting time
	virtual void						UpdateTimeWait( void );

	// TSP processing function in accordance with the trigger in progress
	virtual void						MoveTSP( CNtlTSCont* pCurCont, CNtlTSCont* pNextCont, bool bSave );

	virtual void						ChangeTSState( unsigned int uiChangFlag ) { UNREFERENCED_PARAMETER( uiChangFlag ); return; }
};


inline CDboTSTAgency* CDboTSTCtrl::GetParent( void )
{
	return m_pParent;
}

inline CNtlTSCont* CDboTSTCtrl::GetCurTSP( void )
{
	return m_pCurTSP;
}

inline unsigned int CDboTSTCtrl::GetCurState( void ) const
{
	return m_uiCurState;
}

inline bool CDboTSTCtrl::IsError( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_ERROR_BLOCK ? true : false;
}

inline bool CDboTSTCtrl::IsFailed( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_PROGRESS_FAILED ? true : false;
}

inline bool CDboTSTCtrl::IsCleared( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_CLEARED ? true : false;
}

inline bool CDboTSTCtrl::IsCSComunication( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_CS_COM_WAIT ? true : false;
}

inline bool CDboTSTCtrl::IsClientWait( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_CLIENT_COM_WAIT ? true : false;
}

inline bool CDboTSTCtrl::IsSvrWait( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_SVR_COM_WAIT ? true : false;
}

inline bool CDboTSTCtrl::IsExitState( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_EXIT ? true : false;
}

inline bool CDboTSTCtrl::IsUIProgress( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_UI_PROGRESS ? true : false;
}

inline bool CDboTSTCtrl::IsSvrComAfterClientWait( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT ? true : false;
}

inline bool CDboTSTCtrl::IsEscort( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_ESCORT ? true : false;
}

inline bool CDboTSTCtrl::IsSvrEvtWorldRange( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_SVR_EVT_WORLD_RANGE ? true : false;
}

inline bool CDboTSTCtrl::IsWPSMode( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_WPS_MODE ? true : false;
}

inline bool CDboTSTCtrl::IsUserOutFailedWPS( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_USER_OUT_FAILED_WPS ? true : false;
}

inline bool CDboTSTCtrl::IsSuccess( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_SUCCESS ? true : false;
}

inline bool CDboTSTCtrl::IsComplete( void ) const
{
	return m_uiCurState & eTS_PROG_STATE_COMPLETE ? true : false;
}

inline void CDboTSTCtrl::SetError( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_ERROR_BLOCK;
	else		m_uiCurState &= ~eTS_PROG_STATE_ERROR_BLOCK;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_ERROR_BLOCK );
}

inline void CDboTSTCtrl::SetFailed( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_PROGRESS_FAILED;
	else		m_uiCurState &= ~eTS_PROG_STATE_PROGRESS_FAILED;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_PROGRESS_FAILED );
}

inline void CDboTSTCtrl::SetCleared( bool bOn, bool bUpdate )
{
	if ( bOn )
	{
		m_uiCurState |= eTS_PROG_STATE_CLEARED;
		m_uiCurState &= ~eTS_PROG_STATE_UI_PROGRESS;
	}
	else
	{
		m_uiCurState &= ~eTS_PROG_STATE_CLEARED;
		m_uiCurState |= eTS_PROG_STATE_UI_PROGRESS;
	}

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_CLEARED );
}

inline void CDboTSTCtrl::SetCSComunication( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_CS_COM_WAIT;
	else		m_uiCurState &= ~eTS_PROG_STATE_CS_COM_WAIT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_CS_COM_WAIT );
}

inline void CDboTSTCtrl::SetClientWait( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_CLIENT_COM_WAIT;
	else		m_uiCurState &= ~eTS_PROG_STATE_CLIENT_COM_WAIT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_CLIENT_COM_WAIT );
}

inline void CDboTSTCtrl::SetSvrWait( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_SVR_COM_WAIT;
	else		m_uiCurState &= ~eTS_PROG_STATE_SVR_COM_WAIT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_SVR_COM_WAIT );
}

inline void CDboTSTCtrl::SetExitState( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_EXIT;
	else		m_uiCurState &= ~eTS_PROG_STATE_EXIT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_EXIT );
}

inline void CDboTSTCtrl::SetSvrComAfterClientWait( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT;
	else		m_uiCurState &= ~eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_SVR_COM_AFTER_CLIENT_WAIT );
}

inline void CDboTSTCtrl::SetEscort( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_ESCORT;
	else		m_uiCurState &= ~eTS_PROG_STATE_ESCORT;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_ESCORT );
}

inline void CDboTSTCtrl::SetSvrEvtWorldRange( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_SVR_EVT_WORLD_RANGE;
	else		m_uiCurState &= ~eTS_PROG_STATE_SVR_EVT_WORLD_RANGE;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_SVR_EVT_WORLD_RANGE );
}

inline void CDboTSTCtrl::SetWPSMode( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_WPS_MODE;
	else		m_uiCurState &= ~eTS_PROG_STATE_WPS_MODE;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_WPS_MODE );
}

inline void CDboTSTCtrl::SetUserOutFailedWPS( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_USER_OUT_FAILED_WPS;
	else		m_uiCurState &= ~eTS_PROG_STATE_USER_OUT_FAILED_WPS;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_USER_OUT_FAILED_WPS );
}

inline void CDboTSTCtrl::SetSuccess( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_SUCCESS;
	else		m_uiCurState &= ~eTS_PROG_STATE_SUCCESS;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_SUCCESS );
}

inline void CDboTSTCtrl::SetComplete( bool bOn, bool bUpdate )
{
	if ( bOn )	m_uiCurState |= eTS_PROG_STATE_COMPLETE;
	else		m_uiCurState &= ~eTS_PROG_STATE_COMPLETE;

	if ( bUpdate ) ChangeTSState( eTS_PROG_STATE_COMPLETE );
}

inline unsigned int CDboTSTCtrl::GetSSM( eSSM_ID eSSMId ) const
{
	if ( (int)eSSMId < eSSM_ID_0 || (int)eSSMId >= eSSM_ID_MAX ) return 0xffffffff;
	return m_sSSM.auiSSM[eSSMId];
}

inline bool CDboTSTCtrl::IsContSyncQueueEmpty( void ) const
{
	return m_sSyncQueue.defContSyncQueue.empty();
}

inline bool CDboTSTCtrl::IsTimeWait( void ) const
{
	return m_uiTWPeriod ? true : false;
}

inline eEVENT_GEN_TYPE CDboTSTCtrl::GetEventGenType( void ) const
{
	return m_eEvtGenType;
}

inline unsigned int CDboTSTCtrl::GetEventGenId( void ) const
{
	return m_uiEvtGenId;
}

inline SScouterUseTargetInfo& CDboTSTCtrl::GetEventGenScouterUseTargetInfo( void )
{
	return m_sScouterUseTargetInfo;
}

inline SSkillUseInfo& CDboTSTCtrl::GetEventGenSkillUseInfo( void )
{
	return m_sSkillUserInfo;
}

inline vecdef_ItemTargetList& CDboTSTCtrl::GetEventGenItemTargetInfo( void )
{
	return m_defItemTargetList;
}

inline sCOL_RGN_DATA& CDboTSTCtrl::GetEventGenColRgnInfo( void )
{
	return m_sColRgnData;
}


inline SRB_DATA& CDboTSTCtrl::GetEventGenRBData( void )
{
	return m_sRBData;
}


inline SMobTargetInfo& CDboTSTCtrl::GetEventGenMobTargetData( void )
{
	return m_sMobTargetData;
}


#endif