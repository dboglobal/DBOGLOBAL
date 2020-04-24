#ifndef _DBO_TSCTAGENCY_H_
#define _DBO_TSCTAGENCY_H_


#include "DboTSCore.h"
#include "ceventhandler.h"


class CDboTSCTCtrl;
class CDboUnifiedQuestNarration;


/** 
	Client trigger agency
*/


class CDboTSCTAgency : public CDboTSTAgency, public RWS::CEventHandler
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
	SGET_QUEST_INFO						m_TriggerInfo;

	sAvarPosInfo						m_sAvatarCurPos;

	mapdef_COL_TRIG_OBJ_LIST			m_defColTrigObjCache;

	CDboUnifiedQuestNarration*			m_pUnifiedQuestNarration;

// Constructions and Destructions
public:
	CDboTSCTAgency( void );
	virtual ~CDboTSCTAgency( void );

// Methods
public:
	SGET_QUEST_INFO*					GetQuestInfoList( eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter = NULL );

	virtual void						Update( void );

	virtual void						HandleEvents( RWS::CMsg& Msg );

// Implementations
protected:
	virtual CDboTSTCtrl*				MakeTriggerController( CNtlTSTrigger* pTrig );

	sCOL_TRIG_OBJ_INFO*					FindColTrigObj( unsigned int hHandle );
	void								AttachColTrigObj( unsigned int hHandle );

// Message
public:
	//////////////////////////////////////////////////////////////////////////
	//
	//	Client <-> Server 메시지들
	//
	//////////////////////////////////////////////////////////////////////////

	// 트리거 시스템 흐름 진행 관련 메시지들
	void								UG_Avatar_TS_Confirm_Step( NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, unsigned int* uiParam, unsigned char byEventType, unsigned int uiEventData );
	void								GU_Avatar_TS_Confirm_Step( WORD wResultCode, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId );

	// TS 상태 업데이트 메시지
	void								GU_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );
	void								UG_TS_Update_State( NTL_TS_T_ID tId, unsigned char byType, unsigned short wTSState, unsigned int uiParam = 0xffffffff );

	// 서버로부터 받는 TS 진행 이벤트
	void								GU_TS_Update_Event_Nfy( NTL_TS_EVENT_ID eID );

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

	void								UT_UpdateAvatarPos( unsigned int uiWorldIdx, float fPosX, float fPosZ );

	void								TU_FinishQuest( unsigned char byTSType, NTL_TS_T_ID tID );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Event
		//
		//////////////////////////////////////////////////////////////////////////

	void								UT_EventDoTrigger( sQUEST_INFO& sQuestInfo, eEVENT_GEN_TYPE eEvtGenType, RwUInt32 uiOwnerId, void* pParameter = NULL );

		//////////////////////////////////////////////////////////////////////////
		//
		//	Container
		//
		//////////////////////////////////////////////////////////////////////////

	// 제안 대화 상자
	void								TU_ShowProposalDialog( sTS_KEY& sKey, CDboTSContProposal* pCont, CDboTSContReward* pContRwd );
	void								UT_ShowProposalDialog( sTS_KEY& sKey, bool bAccept );

	// 사용자 보상 선택 대화 상자
	void								TU_ShowRewardDialog( sTS_KEY& sKey, CDboTSCTCtrl* pTCtrl, CDboTSContReward* pContRwd );
	void								UT_ShowRewardDialog( sTS_KEY& sKey, int nSelIdx, bool bCancel );

	// 사용자 선택 대화 상자
	void								TU_ShowUserSelectionDialog( sTS_KEY& sKey, CDboTSCTCtrl* pTCtrl, CDboTSContUsrSel* pCont );
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

	// NPC 대화
	void								TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct );
	void								UT_ShowNPCConv( sTS_KEY& sKey );

	// 카메라 연출
	void								TU_CameraDirection( sTS_KEY& sKey, CDboTSActOPCam* pAct );
	void								UT_CameraDirection( sTS_KEY& sKey );

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
};


#endif