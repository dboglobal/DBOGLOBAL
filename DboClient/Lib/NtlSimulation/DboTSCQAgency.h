#ifndef _DBO_TSCQAGENCY_H_
#define _DBO_TSCQAGENCY_H_


#include "DboTSCore.h"
#include "ceventhandler.h"


class CDboTSCQCtrl;
class CDboUnifiedQuestNarration;


/** 
	Client quest agency
*/


class CDboTSCQAgency : public CDboTSQAgency, public RWS::CEventHandler
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	struct sAvarPosInfo
	{
		unsigned						uiWorldIx;
		float							fPosX, fPosZ;

		sAvarPosInfo( void )
		{
			Clear();
		}

		void Clear( void )
		{
			uiWorldIx = 0xffffffff;
			fPosX = FLT_MAX;
			fPosZ = FLT_MAX;
		}
	};

	struct sCOL_TRIG_OBJ_INFO
	{
		sCOL_TRIG_OBJ_INFO( void ) : uiEraseTime( 500 ), hHandle( 0xffffffff ), uiStandTime( 0 ) { return; }

		unsigned int					uiEraseTime;

		unsigned int					hHandle;
		unsigned int					uiStandTime;
	};

	typedef std::map< unsigned int, sCOL_TRIG_OBJ_INFO > mapdef_COL_TRIG_OBJ_LIST;

// Member variables
protected:
	SGET_QUEST_INFO						m_QuestInfo;

	sAvarPosInfo						m_sAvatarCurPos;

	mapdef_COL_TRIG_OBJ_LIST			m_defColTrigObjCache;

	CDboUnifiedQuestNarration*			m_pUnifiedQuestNarration;

// Constructions and Destructions
public:
	CDboTSCQAgency( void );
	virtual ~CDboTSCQAgency( void );

// Methods
public:
	SGET_QUEST_INFO*					GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter = NULL );

	SGET_QUEST_INFO*					GetQuestInfoList_ForQuestSerarch( unsigned int uiWorldTblIdx, RwReal fX, RwReal fZ, RwReal fRadius );

	bool								HasEventNPCInProgressQuest( unsigned int uiNPCIdx );

	virtual void						HandleEvents( RWS::CMsg& Msg );

	virtual void						Update( void );

	bool								IsSameKeyTSKeyWithQMKey( const sTS_KEY& sKey, const QM_KEY& QMKey );
	NTL_TS_T_ID							GetTIDFromQuestMarkKey( const QM_KEY& Key );
	const QM_KEY&						MakeQuestMarkKey( unsigned char byQProgType, NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, unsigned char byType = 0xff, unsigned int uiUserData1 = 0xffffffff, unsigned int uiUserData2 = 0xffffffff );
	void								SplitQuestMarkKey( const QM_KEY& Key, unsigned char& byQProgType, NTL_TS_T_ID& tId, NTL_TS_TC_ID& tcId, unsigned char& byType, unsigned int& uiUserData1, unsigned int& uiUserData2 );

	const wchar_t*						GetQuestText( NTL_TS_T_ID tID );

// Implementations
protected:
	virtual CDboTSTCtrl*				MakeTriggerController( CNtlTSTrigger* pTrig );

	sCOL_TRIG_OBJ_INFO*					FindColTrigObj( unsigned int hHandle );
	void								AttachColTrigObj( unsigned int hHandle );

	void								GetStarterOrRewardEventInfo( NTL_TS_T_ID tID, RwUInt32& uiEventType, RwUInt32& uiEventID, RwBool& bIsStarter );

// Message
public:
	//////////////////////////////////////////////////////////////////////////
	//
	//	Client <-> Server 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

	// 퀘스트 공유
	void								UG_Quest_Share( NTL_TS_T_ID tId );
	void								GU_Quest_Share_Nfy( WORD wResultCode, HOBJECT hActor, NTL_TS_T_ID tId );

	// 초기 게임 진입 시 처리 되는 메시지들
	// 퀘스트 전용
	void								GU_Avatar_TS_Completed_Info( const sCOMPLETE_QUEST_INFO& sInfo );
	void								GU_Avatar_TS_Progress_Info( const sPROGRESS_QUEST_INFO& sInfo );

	// 트리거 시스템 흐름 진행 관련 메시지들
	void								UG_Avatar_TS_Confirm_Step( NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData );
	void								GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId );

	// 퀘스트 포기
	void								UG_Avatar_TS_GiveUp_Quest( NTL_TS_T_ID tId );
	void								GU_Avatar_TS_GiveUp_Quest( WORD wResultCode, NTL_TS_T_ID tId );

	// 서버 이벤트
	void								GU_Avatar_TS_SToC_Event_Start_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								GU_Avatar_TS_SToC_Event_End_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId );
	void								GU_Avatar_TS_SToC_Event_Update_NFY( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, BYTE bySvrEvtType, BYTE bySlot, uSTOC_EVT_UPDATE_DATA& uEvtData );

	// TS 상태 업데이트 메시지
	void								GU_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );
	void								UG_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );

	// 서버로부터 받는 TS 진행 이벤트
	void								GU_TS_Update_Event_Nfy( NTL_TS_EVENT_ID eID );

	// 서버로부터 받는 TS 강제 실행 명령
	void								GU_TS_Excute_Trigger( NTL_TS_T_ID tID );

	// 서버로부터 받는 TMQ 관련 완료 및 진행중 퀘스트 제거 이벤트
	void								GU_TS_RemoveTMQQuest( NTL_TS_T_ID tMin, NTL_TS_T_ID tMax );

	void								GU_Qeust_Force_Completion( NTL_TS_T_ID tID );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Client <-> Client TS 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//
		//	Global
		//
		//////////////////////////////////////////////////////////////////////////

	void								TU_RegQuestInfoNfy( NTL_TS_T_ID tId, NTL_TS_TC_ID tcId, NTL_TS_TA_ID taId, bool bNewRegister, bool bQuestShare, unsigned int uiQAreaName, unsigned int uiQState, unsigned int uiQTitle, unsigned int uiQGoal, eQUEST_SORT_TYPE eQSortType );

	void								TU_UnregQuestInfoNfy( NTL_TS_T_ID tId );

	void								TU_ShowQuestWindowNfy( NTL_TS_T_ID tId );

	void								TU_ShowQuestIndicatorNfy( NTL_TS_T_ID tId, bool bAuto );

	void								TU_UpdateQuestStateNfy( NTL_TS_T_ID tId, bool bOutStateMsg, unsigned int uiUpdatedQState, unsigned int uiQState, unsigned int uiQuestTitle, eSTOC_EVT_DATA_TYPE eEvtInfoType, const uSTOC_EVT_DATA& uEvtInfoData );

	void								TU_UpdateQuestProgressInfoNfy( NTL_TS_T_ID tId, eSTOC_EVT_DATA_TYPE eEvtInfoType, uSTOC_EVT_DATA& uEvtInfoData, RwUInt32 uiTimeLimit );

	void								TU_AcceptProposal( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId );

	void								TU_AcceptReward( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, NTL_TS_TC_ID tcId, unsigned int uiQTitle, bool bOutStateMsg );

	void								TU_AcceptGiveUp( NTL_TS_T_ID tId, NTL_TS_TG_ID tgId, unsigned int uiQTitle );

	void								UT_GiveUpQuest( NTL_TS_T_ID tId );

	void								UT_UpdateAvatarPos( unsigned int uiWorldIdx, float fPosX, float fPosZ );

	void								TU_FinishQuest( unsigned char byTSType, NTL_TS_T_ID tID );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Event
		//
		//////////////////////////////////////////////////////////////////////////

	void								UT_EventDoQuest( sQUEST_INFO& sQuestInfo, eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter = NULL, RwBool bTransCameraCtrlRight = FALSE );

	void								UT_DoTutorialQuest( NTL_TS_T_ID tId );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Container
		//
		//////////////////////////////////////////////////////////////////////////

	// 제안 대화 상자
	void								TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pContRwd );
	void								UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept );

	// 사용자 보상 선택 대화 상자
	void								TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl, CDboTSContReward* pContRwd );
	void								UT_ShowRewardDialog( sTS_KEY& sKey, int nSelIdx, bool bCancel );

	// 사용자 선택 대화 상자
	void								TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl, CDboTSContUsrSel* pCont );
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

	// 서버 이벤트
	void								TU_ServerEvent( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl );
	void								UT_ServerEvent( sTS_KEY& sKey );

	// NPC 대화
	void								TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct );
	void								UT_ShowNPCConv( sTS_KEY& sKey );

	// 퀘스트 정보 등록
	void								TU_RegisterQuestInfo( sTS_KEY& sKey, CDboTSCQCtrl* pQCtrl );
	void								UT_RegisterQuestInfo( sTS_KEY& sKey );

	// 카메라 연출
	void								TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct );
	void								UT_CameraDirection( sTS_KEY& sKey );

	// Object 대화
	void								TU_ShowObjConv( sTS_KEY& sKey, CDboTSActObjConv* pAct );
	void								UT_ShowObjConv( sTS_KEY& sKey );

	// Hint
	void								TU_Hint( sTS_KEY& sKey, CDboTSActHint* pAct );
	void								UT_Hint( sTS_KEY& sKey );

	// Open inventory
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


#endif