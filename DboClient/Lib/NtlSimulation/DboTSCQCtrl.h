#ifndef _DBO_TSCQCTRL_H_
#define _DBO_TSCQCTRL_H_


#include "DboTSCore.h"


/**
	Client quest controller
*/


class CDboTSCQCtrl : public CDboTSQCtrl
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:

	#define QMI_PROG_TYPE_EVENTER			(1)
	#define QMI_PROG_TYPE_MARK_POSITION		(2)
	#define QMI_PROG_TYPE_VISIT_SVR_EVENTER	(3)

	struct SQuestMarkInfo
	{
		eQMI_TARGET_TYPE				eTargetType;			// 타겟 마크 종류
		unsigned int					uiTargetWorldTblIdx;	// 타겟이 소속된 월드 테이블 인덱스
		unsigned int					uiTargetTableIdx;		// 타겟의 테이블 인덱스
		float							fTargetPosX;			// 타겟의 위치 ( x, y, z )
		float							fTargetPosY;
		float							fTargetPosZ;
		unsigned int					uiTooltipTblIdx;		// 툴팁에 출력할 테이블 인덱스
	};

	typedef std::multimap<QM_KEY, SQuestMarkInfo> mapdef_QUESTMARKINFO_LIST;

	enum eNPC_CAMERA_DIR_TYPE
	{
		eNPC_CAMERA_DIR_TYPE_CONTINUE	= 0x01,
		eNPC_CAMERA_DIR_TYPE_STOP		= 0x02,
	};

	typedef std::map< eREG_C_TIMING, void* > mapdef_CLIENT_TIMING_DATA_LIST;

	typedef std::multimap< unsigned int, sTS_KEY > mmapdef_SVR_EVENT_ID_LIST;

	struct sSERVER_EVENT_ID_DATA
	{
		unsigned int					uiEventID;
		sTS_KEY							sTSKey;
	};

	typedef std::map< NTL_TS_TC_ID, sSERVER_EVENT_ID_DATA > mapdef_REGISTED_SVR_EVENT_ID_LIST;

// Member variables
protected:
	RwUInt32							m_uiTitle;
	RwUInt32							m_uiArea;
	RwUInt32							m_uiGoal;
	RwUInt32							m_uiGrade;
	eGRADE_TYPE							m_eGradeType;
	eQUEST_SORT_TYPE					m_eSort;
	RwUInt32							m_uiSort;
	RwUInt32							m_uiContents;
	sREWARD_INFO*						m_pDefRwd;
	sREWARD_INFO*						m_pSelRwd;

	//new
	unsigned int						m_uiRewardZenny;
	unsigned int						m_uiRewardExp;

	unsigned int						m_uiUpdateTimeCnt;

	bool								m_bQuestShare;

	mapdef_QUESTMARKINFO_LIST			m_defQuestMarkInfoList;

	bool								m_bNPCCameraStopEvt;

	std::map< unsigned int, unsigned int >	m_mapNPCIdxList;

	mapdef_CLIENT_TIMING_DATA_LIST		m_defCTimingDataList;
	eREG_C_TIMING_EXCUTE_TYPE			m_eCTimingExcType;
	uREG_C_TIMING_EXCUTE_DATA			m_uCTimingExcData;

	mmapdef_SVR_EVENT_ID_LIST			m_defSvrEventIDList;
	mapdef_REGISTED_SVR_EVENT_ID_LIST	m_defRegSvrEvtIDList;

	RwBool								m_bTransCameraCtrlRight;

// Constructions and Destructions
public:
	CDboTSCQCtrl();
	virtual ~CDboTSCQCtrl();

// Methods
public:
	// Update quest controller
	virtual void						Update( void );

	virtual RwUInt32					GetTitle();
	virtual RwUInt32					GetArea( void )			{ return m_uiArea; }
	virtual RwUInt32					GetGoal( void )			{ return m_uiGoal; }
	virtual RwUInt32					GetGrade( void )		{ return m_uiGrade; }
	virtual eGRADE_TYPE					GetGradeType( void )	{ return m_eGradeType; }
	virtual eQUEST_SORT_TYPE			GetSortType( void )		{ return m_eSort; }
	virtual RwUInt32					GetSort( void )			{ return m_uiSort; }
	virtual RwUInt32					GetContents( void )		{ return m_uiContents; }
	virtual sREWARD_INFO*				GetDefRwd( void )		{ return m_pDefRwd; }
	virtual sREWARD_INFO*				GetSelRwd( void )		{ return m_pSelRwd; }

	virtual unsigned int				GetRewardExp(void) { return m_uiRewardExp; }
	virtual unsigned int				GetRewardZeny(void) { return m_uiRewardZenny; }

	int									GetLimitTime( void );

	virtual unsigned int				GetCurTime( void );

	bool								IsQuestShareMode( void )	{ return m_bQuestShare; }
	void								SetQuestShareMode( bool bShare ) { m_bQuestShare = bShare; }

	bool								IsCompletedServerEvt( void );

	bool								HasEventNPCInProgressQuest( unsigned int uiNPCIdx );

	void								RegNPCCameraStopEvt( void );
	void								UnregNPCCameraStopEvt( void );

	void								AddClientTimingData( eREG_C_TIMING eCTimingType, void* pData );
	void								ClearAllClientTimingData( void );
	void								SetTimingExecutionData( eREG_C_TIMING_EXCUTE_TYPE eType, uREG_C_TIMING_EXCUTE_DATA& uData );
	void								MatchTimingData_MobClick( unsigned int uiMobGroupIdx );
	void								MatchTimingData_NpcClick( unsigned int uiNPCIdx );
	void								MatchTimingData_ObjClick( unsigned int uiWorldIdx, unsigned int uiObjIdx );
	void								MatchTimingData_EnterRgn( unsigned int uiWorldIdx, float fAvatarPosX, float fAvatarPosZ );
	void								MatchTimingData_GetQItem( unsigned int uiQItemIdx );
	void								MatchTimingData_QTitleClick( void );
	void								MatchTimingData_AcceptClick( void );
	void								MatchTimingData_SuccessQuest( void );
	void								MatchTimingData_PreReward( void );
	void								MatchTimingData_PostReward( void );

	void								SkipContainer( NTL_TS_TC_ID tcID );

	void								Build( void );

	void								AttachSvrEventID( NTL_TS_EVENT_ID eID );
	void								UpdateSvrEventID( void );

	void								SetTransCameraCtrlRight( RwBool bTransCameraCtrlRight );

// Implementations
protected:
	NTL_TSRESULT						Cont_GCond( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_GAct( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_UserSel( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_Reward( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_Start( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_End( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_Narration( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_Proposal( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_Switch( CNtlTSCont* pCont, bool bDoAction = true );
	NTL_TSRESULT						Cont_UnifiedNarration( CNtlTSCont* pCont, bool bDoAction = true );

	void								UpdateLimitTime( void );

	void								UpdateSuccessState( void );
	void								UpdateClearState( void );

	virtual void						UpdateTSStep( void );

	virtual void						ChangeTSState( unsigned int uiChangFlag );

	virtual bool						IfInterestingDoExceptionGroup( NTL_TS_TG_ID tgId ) const;

	virtual void						LoadQuestProgressInfo_V0( const sPROGRESS_QUEST_INFO::uDATA& uData );

	void								UpdateQuestMark( void );

	bool								HasQuestMark( QM_KEY Key );

	void								RegisterQuestMark_Eventer( QM_KEY Key,
																   eQMI_TARGET_TYPE eTargetType,
																   unsigned int uiTargetWorldTbldx,
																   unsigned int uiTargetTableIdx,
																   float fTargetPosX,
																   float fTargetPosY,
																   float fTargetPosZ,
																   unsigned int uiTooltipTblIdx );

	void								RegisterQuestMark_Position( QM_KEY Key,
																	eQMI_TARGET_TYPE eTargetType,
																	unsigned int uiTargetWorldTbldx,
																	float fTargetPosX,
																	float fTargetPosY,
																	float fTargetPosZ,
																	unsigned int uiTooltipTblIdx );

	void								UnregisterQuestMark( QM_KEY Key );

	// Return 값 :	true	-> 카메라 스톱 함
	//				false	-> 카메라 스톱 안함
	unsigned char						CheckUnregNPCCameraStop( CNtlTSCont* pNextCont, bool bFirstCall = true );

	bool								IsRemovingTMQQuest( void ) const;
	void								SetRemovingTMQQuest( bool bRemoving );

	void								BuildEventNPCInProgressQuest( std::map< unsigned int, unsigned int >& mapNPCIdxList );

	void								DoClientTimingAction( void );

// Message
public:
	//////////////////////////////////////////////////////////////////////////
	//
	//	Client <-> Server 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

	// 트리거 진행 흐름에 관계된 메시지들
	void								UG_Avatar_TS_Confirm_Step( NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData );
	void								GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId );

	// 퀘스트 포기
	void								UG_Avatar_TS_GiveUp_Quest( void );
	void								GU_Avatar_TS_GiveUp_Quest( WORD wResultCode );

	// 서버 이벤트
	void								GU_Avatar_TS_SToC_Event_Start_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								GU_Avatar_TS_SToC_Event_End_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								GU_Avatar_TS_SToC_Event_Update_NFY( NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, BYTE bySvrEvtType, BYTE bySlot, uSTOC_EVT_UPDATE_DATA& uEvtData );

	// TS 상태 업데이트 메시지
	void								GU_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );
	void								UG_TS_Update_State( unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );

	//////////////////////////////////////////////////////////////////////////
	//
	// 클라이언트 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//
		//	Golbal
		//
		//////////////////////////////////////////////////////////////////////////

	void								TU_AcceptProposal( NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId );

	void								TU_AcceptReward( NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId, unsigned int uiQTitle, bool bOutStateMsg );

	void								TU_AcceptGiveUp( NTL_TS_TG_ID tgId, unsigned int uiQTitle );

	void								UT_GiveUpQuest( void );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Event
		//
		//////////////////////////////////////////////////////////////////////////

	void								UT_ClickObject( unsigned int uiObjId );
	void								UT_CollisionObject( unsigned int uiObjId );
	void								UT_ClickNPC( unsigned int uiNPCId );
	void								UT_ItemUse( unsigned int uiItemIdx, vecdef_ItemTargetList* pdefItemTargetList );
	void								UT_ItemGet( unsigned int uiItemIdx );
	void								UT_ItemEquip( unsigned int uiItemIdx );
	void								UT_ScoutUse( unsigned int uiItemIdx, SScouterUseTargetInfo* pTargetInfo );
	void								UT_RcvSvrEvt( unsigned int uiEvtID );
	void								UT_SkillUse( unsigned int uiSkillIdx, SSkillUseInfo* psSkillUseInfo );
	void								UT_ColRgn( sCOL_RGN_DATA* pColRgnData );
	void								UT_RB( unsigned int uiWorldIdx, SRB_DATA* pRBData );
	void								UT_ClickMob( unsigned int uiMobGroupID, SMobTargetInfo* pTargetInfo );
	void								UT_BindStone( void );
	void								UT_SearchQuest( void );
	void								UT_ItemUpgrade( void );
	void								UT_Teleport( void );
	void								UT_Budokai( void );
	void								UT_SlotMachine( void );
	void								UT_HoipoiMix( void );
	void								UT_PrivateShop( void );
	void								UT_FreeBattle( void );
	void								UT_ItemIdentity( void );
	void								UT_UseMail( void );
	void								UT_Party();

	void								UT_ClickSideIcon();
	void								UT_LevelCheck();
	void								UT_Quest();
	void								UT_DialogOpen();

		//////////////////////////////////////////////////////////////////////////
		//
		//	Container
		//
		//////////////////////////////////////////////////////////////////////////

	// 제안 대화 상자
	void								TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pContRwd );
	void								UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept );

	// 사용자 보상 선택 대화 상자
	void								TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSContReward* pCont );
	void								UT_ShowRewardDialog( sTS_KEY& sKey, int nSelRwdIdx, bool bCancel );

	// 사용자 선택 대화 상자
	void								TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSContUsrSel* pCont );
	void								UT_ShowUserSelectionDialog( sTS_KEY& sKey, NTL_TS_TC_ID tcSelId, bool bCancel );

	// 나래이션 대화 상자
	void								TU_ShowNarrationDialog( sTS_KEY& sKey, CDboTSContNarration* pCont );
	void								UT_ShowNarrationDialog( sTS_KEY& sKey, bool bCancel );

	// 통합 나래이션 대화 상자
	void								TU_ShowUnifiedNarrationDialog( sTS_KEY& sKey, CDboTSContUnifiedNarration* pCont );
	void								UT_ShowUnifiedNarrationDialog( sTS_KEY& sKey, bool bCancel );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Action
		//
		//////////////////////////////////////////////////////////////////////////

	// 예외 타이머
	void								TG_Avatar_TS_ExceptTimerStartNfy( sTS_KEY& sKey, CDboTSActETimerS* pAct );
	void								TG_Avatar_TS_ExceptTimerEndNfy( sTS_KEY& sKey, CDboTSActETimerE* pAct );

	// 서버 이벤트
	void								TU_ServerEvent( sTS_KEY& sKey, CDboTSActSToCEvt* pAct );
	void								UT_ServerEvent( sTS_KEY& sKey );

	// NPC 대화
	void								TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct );
	void								UT_ShowNPCConv( sTS_KEY& sKey );

	// 퀘스트 정보 등록
	void								TU_RegisterQuestInfo( sTS_KEY& sKey, CDboTSActRegQInfo* pAct );
	void								UT_RegisterQuestInfo( sTS_KEY& sKey );

	// 카메라 연출
	void								TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct );
	void								UT_CameraDirection( sTS_KEY& sKey );

	// TS 진행 대기
	void								TG_WaitTSNfy( sTS_KEY& sKey, CDboTSActTWaitTS* pAct );

	// SSM 업데이트
	void								TG_InSSMNfy( sTS_KEY& sKey, CDboTSActInSSM* pAct );

	// 상태 업데이트
	void								TG_TSStateNfy( sTS_KEY& sKey, CDboTSActTSState* pAct );

	// Object 대화
	void								TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct );
	void								UT_ShowObjConv( sTS_KEY& sKey );

	// Hint
	void								TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct );
	void								UT_Hint( sTS_KEY& sKey );

	// Open Window
	void								TU_Open_Window( sTS_KEY& sKey, CDboTSActOpenWindow* pAct );
	void								UT_Open_Window( sTS_KEY& sKey );

	// PC conversation
	void								TU_PC_ConvNfy( sTS_KEY& sKey, CDboTSActPCConv* pAct );

	// Teleport Mudosa
	void								TU_TelMudosa( sTS_KEY& sKey, CDboTSActTelMudosa* pAct );
	void								UT_TelMudosa( sTS_KEY& sKey, RwUInt8 byMudosaIndex, RwBool bCancel );

	// Register client timing
	void								TU_RegCTiming( sTS_KEY& sKey, CDboTSActRegCTiming* pAct );
	void								UT_RegCTiming( sTS_KEY& sKey );

	// Execute client group
	void								TU_ExcCGroup( sTS_KEY& sKey, CDboTSActExcCGroup* pAct );
	void								UT_ExcCGroup( sTS_KEY& sKey );
};


inline int CDboTSCQCtrl::GetLimitTime( void )
{
	sEXCEPTION_TIMER* pTimeLimitInfo = GetTimeLimitInfo();
	if ( pTimeLimitInfo )
	{
		return 0xffffffff == pTimeLimitInfo->uiRemainTime ? 0 : pTimeLimitInfo->uiRemainTime;
	}

	return 0xffffffff;
}


#endif