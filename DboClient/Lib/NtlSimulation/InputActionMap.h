/*****************************************************************************
 *
 * File			: InputAction.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 26	
 * Abstract		: Input action 
 *****************************************************************************
 * Desc         : 
 *
 * 1st Refactoring(2008. 1. 15 ~ 1. 20 ) : 
 * 자료구조와 매니저를 완전 분리( InputActionMap <-> ActionMap )
 *
 *****************************************************************************/

#ifndef __INPUT_ACTION_MAP_H__
#define __INPUT_ACTION_MAP_H__

#include "NtlCallbackWidget.h"
#include "InputHandler.h"
#include "ceventhandler.h"
#include "NtlSLDef.h"

// ActionMap
#include "ActionMap.h"

// DashMap Struct
typedef struct _SInputDashMap
{
	RwUInt8 byDown;
	RwReal fTime;
	RwUInt32 uiMoveFlags;
}SInputDashMap;

#define NTL_KEYBOARD_DBCLICK_TIME		0.15f

/**
* \ingroup client
* ActionMap Manager
*/
class CInputActionMap : public RWS::CEventHandler
{
public:
	CInputActionMap();
	~CInputActionMap();

	static CInputActionMap* GetInstance(void);

	RwBool Create(void); 
	void Destroy(void);
	void Update(RwReal fElapsed);
	void HandleEvents(RWS::CMsg &pMsg);
	void HandleEventActionmapLoadInfo(RWS::CMsg& msg);
	void HandleEventsActionMapUpdateRes(RWS::CMsg& msg);
	void Reset(void);
	void ResetMoveFlags(void);

	int KeyDownHandler(unsigned int pKeyData);
	int KeyUpHandler(unsigned int pKeyData);

	int MouseDownHandler(unsigned int pMouseData);
	int MouseUpHandler(unsigned int pMouseData);

	void SetActive(RwBool bActive);

public:
	// CActionMap 인터페이스
	void			InitInputMode();					///< 입력 모드 해제
	RwUInt32		GetInputMode();						///< 어떤 ACTION이 INPUTMODE인지
	void			SetInputMode( RwUInt32 nAction );	///< nAction에 입력 모드 지정

	std::wstring	GetKeyName( RwUInt32 nAction );		///< 액션이 가지고 있는 키의 이름
	unsigned short	FindKey( RwUInt32 nAction );

	void			InitDefaultActionMap();				///< 디폴트 액션맵으로 셋팅

	RwBool			ApplyActionMap();					///< 액션맵의 적용(서버로 패킷 전송)
	void			CancleActionMap();					///< 적용 취소

	// Ignore Key
	RwBool			RegisterIgnoreKey( RwUInt8 byKey, int nRefCount );
	RwBool			RemoveIgnoreKey( RwUInt8 byKey );
	RwBool			IsIgnoreKey( RwUInt8 byKey, RwBool bRef = FALSE );
	void			ClearIgnoreKey();

	// ActionMap Mode
	RwUInt8			GetActionMapMode();

protected:
	RwInt32 GetKeyDownCount(RwUInt32 uiFlags);
	RwUInt32 ConvertServerFlags(RwUInt32 uiFlags);

	void HitTestDownDbClickDashMap(SInputDashMap& sDashMap);
	void HitTestUpDbClickDashMap(SInputDashMap& sDashMap);
	void CallDashMove(RwUInt32 uiServerDashMoveFlags);

	// 액션ID를 넘겨 처리하게 변경된 함수들
	void ActionDownMoveHandler(RwUInt32 uiAction);		///< 액션에 의한 이동 액션 핸들러
	void ActionDownDashMoveHandler(RwUInt32 uiAction);	///< 액션에 의한 대쉬 액션 핸들러
	void ActionDownBehaviorHandler(RwUInt32 uiAction);	///< 액션에 의한 행동 액션 핸들러
	void ActionDownGuiHandler(RwUInt32 uiAction);		///< 액션에 의한 GUI 액션 핸들러

	void ActionUpMoveHandler(RwUInt32 uiAction);			
	void ActionUpDashMoveHandler(RwUInt32 uiAction);		
	void ActionUpBehaviorHandler(RwUInt32 uiAction);		
	void ActionUpGuiHandler(RwUInt32 uiAction);			

	void RegisterFlagMap();
	RwBool IsDownAction(RwUInt32 uiAction);
	RwBool SetFlagAction(RwUInt32 uiAction, RwBool bDown);

	RwBool KeyReference( RwUInt8 byChar, RwBool bDown = TRUE);

private:
	static CInputActionMap* m_pInstance;

	INPUT_HANDLE m_hKeyDown;
	INPUT_HANDLE m_hKeyUp;
	INPUT_HANDLE m_hMouseDown;
	INPUT_HANDLE m_hMouseUp;

	CNtlCallbackParam1 *m_pCallKeyboardMove;		///< 키보드 이동
	CNtlCallbackParam1 *m_pCallKeyboardDashMove;	///< 키보드 대쉬 이동
	CNtlCallbackParam0 *m_pCallAvatarSelect;		///< 아바타 선택
	CNtlCallbackParam1 *m_pCallJump;				///< 점프
	CNtlCallbackParam1 *m_pCallCharging;			///< 기 모으기
	CNtlCallbackParam1 *m_pCallBlocking;			///< 블록 모드
	CNtlCallbackParam1 *m_pCallSitAndStand;			///< 앉아 일어서
	CNtlCallbackParam0 *m_pCallAutoRun;				///< 자동 달리기
	CNtlCallbackParam1 *m_pCallSkillQuickSlotDown;	///< 퀵슬롯 다운
	CNtlCallbackParam1 *m_pCallSkillQuickSlotUp;	///< 퀵슬롯 업
	CNtlCallbackParam1 *m_pCallSkillQuickSlotExDown;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotExUp;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotEx2Down;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotEx2Up;
	CNtlCallbackParam1 *m_pCallSkillQuickSlotChange;///< 퀵슬롯 PREV, NEXT
	CNtlCallbackParam0 *m_pCallExit;				///< Exit
	CNtlCallbackParam0 *m_pCallLooting;				///< 아이템 줍기
	CNtlCallbackParam1 *m_pCallDialogAction;		///< Dialog에 보낼 액션
	CNtlCallbackParam0 *m_pCallTabKey;				///< 탭의 액션
	CNtlCallbackParam0 *m_pCallAutoTarget;			///< 가까운 적 선택
	CNtlCallbackParam0 *m_pCallAutoAttack;			///< 자동 공격
	CNtlCallbackParam0 *m_pCallAutoFollow;			///< 자동 따라가기
	CNtlCallbackParam1 *m_pCallPartySelect;			///< 파티원 선택
	CNtlCallbackParam0 *m_pCallPartyAutoSelect;		///< 파티원 자동 선택
    CNtlCallbackParam0 *m_pCallScouterUse;          ///< 스카우터 사용 (by agebreak)
	CNtlCallbackParam1 *m_pCallTargetMarking;		///< 타겟 마킹
	CNtlCallbackParam1 *m_pCallTargetSelecting;		///< 타겟 셀렉팅
	CNtlCallbackParam1 *m_pCallChatPageChange;			///< 채팅 페이지 (0 UP 1 DOWN)
	

	RwUInt32	m_uiKey1MoveValidFlags;
	RwUInt32	m_uiKey2MoveValidFlags;
	RwUInt32	m_uiMoveFlags;						///< 클라이언트에서 사용되는 Move Flags
	RwBool		m_bRBtnDown;
	RwUInt32	m_uiServerMoveFlags;				///< 서버에 보내는 Move Flags
	RwBool		m_bActive;							///< InputActionMap이 작동중인지의 여부
	RwBool		m_bAcceptServer;					///< 서버에 적용이 가능한 상태

	typedef std::map<RwUInt32, RwBool> FLAGMAP;		///< 상태를 회복시켜줘야 할 액션들
	FLAGMAP m_mapFlag;

	typedef std::map<RwUInt8, RwUInt8>	IGNOREMAP;	///< 무시되는 키를 모아놓은 맵
	IGNOREMAP m_mapIgnore;

	typedef std::map<RwUInt16, RwUInt8> SPECMAP;	///< 특정한 동작을 해야하는 액션
	SPECMAP	m_mapSpecialAction;

	typedef std::map<RwUInt8, RwUInt8> UPDOWNREF;	///< 키가 업/다운 되는 횟수를 체크한다.
	UPDOWNREF m_mapUpdownRef;

	SInputDashMap m_sFrontDashMap;
	SInputDashMap m_sLeftDashMap;
	SInputDashMap m_sRightDashMap;
	SInputDashMap m_sBackDashMap;

	// 액션맵 클래스(ActionMap.h) by Kell
	CActionMapManager m_ActionMapManager;

public:

	template <class Callbackclass>	
	void LinkKeyboardMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkKeyboardMove(void);

	template <class Callbackclass>
	void LinkKeyboardDashMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkKeyboardDashMove(void);

	template <class Callbackclass>
	void LinkAvatarSelect(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAvatarSelect(void);

	template <class Callbackclass>
	void LinkJump(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs));
	void UnLinkJump(void);

	template <class Callbackclass>
	void LinkCharging(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiCharging));
	void UnLinkCharging(void);

	template <class Callbackclass>
	void LinkBlocking(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiBlocking));
	void UnLinkBlocking(void);

	template <class Callbackclass>
	void LinkSitAndStand(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int bKeyDown));
	void UnLinkSitAndStand(void);

	template <class Callbackclass>
	void LinkAutoRun(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoRun(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotDown(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotUp(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotExDown(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotExDown(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotExUp(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotExUp(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotEx2Down(Callbackclass* cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotEx2Down(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotEx2Up(Callbackclass* cbclass, int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx));
	void UnLinkSkillQuickSlotEx2Up(void);

	template <class Callbackclass>
	void LinkSkillQuickSlotChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotChange));
	void UnLinkSkillQuickSlotChange(void);

	template <class Callbackclass>
	void LinkGameExit(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkGameExit(void);

	template <class Callbackclass>
	void LinkLooting(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkLooting(void);

	template <class Callbackclass>
	void LinkDialogHotKey(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKey));
	void UnLinkDialogHotKey(void);

	template <class Callbackclass>
	void LinkTabKey(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkTabKey(void);

	template <class Callbackclass>
	void LinkAutoTarget(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoTarget(void);

	template <class Callbackclass>
	void LinkAutoAttack(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoAttack(void);

	template <class Callbackclass>
	void LinkAutoFollow(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
	void UnLinkAutoFollow(void);

	template <class Callbackclass>
	void LinkPartySelect(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkPartySelect(void);

	template <class Callbackclass>
	void LinkPartyAutoSelect(Callbackclass *cbclass, int (Callbackclass::*callback)(void));
	void UnLinkPartyAutoSelect(void);

    template <class Callbackclass>
    void LinkScouterUse(Callbackclass *cbclass,int (Callbackclass::*callback)(void));
    void UnLinkScouterUse(void);

	template <class Callbackclass>
	void LinkActionMapUpdate(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiAction, unsigned int iChange));
	void UnLinkActionMapUpdate(void);

	template <class Callbackclass>
	void LinkTargetMarking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkTargetMarking(void);

	template <class Callbackclass>
	void LinkTargetSelecting(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx));
	void UnLinkTargetSelecting(void);

	template <class Callbackclass>
	void LinkChatPageChange(Callbackclass *cbclass,int ( Callbackclass::*callback)(unsigned int uiOption));
	void UnLinkChatPageChane(void);
};

static CInputActionMap* GetInputActionMap(void)
{
	return CInputActionMap::GetInstance(); 
}


template <class Callbackclass>	
void CInputActionMap::LinkKeyboardMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int))
{
	NTL_PRE(m_pCallKeyboardMove == 0);
	m_pCallKeyboardMove = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkKeyboardDashMove(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs))
{
	NTL_PRE(m_pCallKeyboardDashMove == 0);
	m_pCallKeyboardDashMove = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAvatarSelect(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallAvatarSelect == 0);
	m_pCallAvatarSelect = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}


template <class Callbackclass>
void CInputActionMap::LinkJump(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiMoveFalgs))
{
	NTL_PRE(m_pCallJump == 0);
	m_pCallJump = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkCharging(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiCharging))
{
	NTL_PRE(m_pCallCharging == 0);
	m_pCallCharging = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkBlocking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiBlocking))
{
	NTL_PRE(m_pCallBlocking == 0);
	m_pCallBlocking = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSitAndStand(Callbackclass *cbclass, int (Callbackclass::*callback)(unsigned int bKeyDown))
{
	NTL_PRE(m_pCallSitAndStand == 0);
	m_pCallSitAndStand = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoRun(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallAutoRun == 0);
	m_pCallAutoRun = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotDown == 0);
	m_pCallSkillQuickSlotDown = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotUp == 0);
	m_pCallSkillQuickSlotUp = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotExDown(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotExDown == 0);
	m_pCallSkillQuickSlotExDown = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotExUp(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotExUp == 0);
	m_pCallSkillQuickSlotExUp = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotEx2Down(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotEx2Down == 0);
	m_pCallSkillQuickSlotEx2Down = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotEx2Up(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotIdx))
{
	NTL_PRE(m_pCallSkillQuickSlotEx2Up == 0);
	m_pCallSkillQuickSlotEx2Up = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkSkillQuickSlotChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiQuickSlotChange))
{
	NTL_PRE(m_pCallSkillQuickSlotChange == 0 );
	m_pCallSkillQuickSlotChange = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass, callback);
}

template <class Callbackclass>
void CInputActionMap::LinkGameExit(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE(m_pCallExit == 0);
	m_pCallExit = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkLooting(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallLooting == 0 );
	m_pCallLooting = NTL_NEW CNtlCallbackWidget0<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkDialogHotKey(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiKey))
{
	NTL_PRE( m_pCallDialogAction == 0 );
	m_pCallDialogAction = NTL_NEW CNtlCallbackWidget1<Callbackclass> (cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTabKey(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallTabKey == 0 );
	m_pCallTabKey = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoTarget(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoTarget == 0 );
	m_pCallAutoTarget = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoAttack(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoAttack == 0 );
	m_pCallAutoAttack = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkAutoFollow(Callbackclass *cbclass,int (Callbackclass::*callback)(void))
{
	NTL_PRE( m_pCallAutoFollow == 0 );
	m_pCallAutoFollow = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkPartySelect(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallPartySelect == 0 );
	m_pCallPartySelect = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkPartyAutoSelect(Callbackclass *cbclass, int (Callbackclass::*callback)(void))
{
	NTL_PRE( 0 == m_pCallPartyAutoSelect );
	m_pCallPartyAutoSelect = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkScouterUse(Callbackclass *cbclass, int (Callbackclass::*callback)(void))
{
    NTL_PRE( 0 == m_pCallScouterUse );
    m_pCallScouterUse = NTL_NEW CNtlCallbackWidget0<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTargetMarking(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallTargetMarking == 0 );
	m_pCallTargetMarking = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkTargetSelecting(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiIdx))
{
	NTL_PRE( m_pCallTargetSelecting == 0 );
	m_pCallTargetSelecting = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

template <class Callbackclass>
void CInputActionMap::LinkChatPageChange(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiOption))
{
	NTL_PRE( m_pCallChatPageChange == 0 );
	m_pCallChatPageChange = NTL_NEW CNtlCallbackWidget1<Callbackclass>(cbclass,callback);
}

#endif
