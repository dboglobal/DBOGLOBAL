#ifndef _DBO_TSCTCTRL_H_
#define _DBO_TSCTCTRL_H_


#include "DboTSCore.h"


/**
	Client trigger controller
*/


class CDboTSCTCtrl : public CDboTSTCtrl
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	CDboTSCTCtrl( void );
	virtual ~CDboTSCTCtrl( void );

// Methods
public:
	// Update trigger controller
	virtual void						Update( void );

	virtual unsigned int				GetCurTime( void );

// Implementations
protected:
	NTL_TSRESULT						Cont_GCond( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_GAct( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_UserSel( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_Reward( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_Start( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_End( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_Narration( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_Proposal( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_Switch( CNtlTSCont* pCont );
	NTL_TSRESULT						Cont_UnifiedNarration( CNtlTSCont* pCont );

	virtual void						UpdateTSStep( void );

	virtual void						ChangeTSState( unsigned int uiChangFlag );

	virtual bool						IfInterestingDoExceptionGroup( NTL_TS_TG_ID tgId ) const;

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
	void								UT_Party( void );

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

	// NPC 대화
	void								TU_ShowNPCConv( sTS_KEY& sKey, CDboTSActNPCConv* pAct );
	void								UT_ShowNPCConv( sTS_KEY& sKey );

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
};


#endif