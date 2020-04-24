/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 9. 13S
* Abstract		: 
*****************************************************************************
* Desc			: 커뮤니티 관련 다이얼로그 형틀
*				  각각의 페이지 내용은 PageType에 따라 세팅되고
*				  메세지와 이벤트도 활성화된 Page에 전달된다.
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// shared
#include "NtlParty.h"

// gui

#include "gui_panel.h"

// simulation
#include "NtlSLParty.h"

// dbo
#include "CommunityBase.h"
#include "SlotGui.h"
#include "Windowby3.h"

class CCommunityParty : public CCommunityBase
{
public:
	enum eZennyDivType
	{
		ZDT_PICKUP_PERSON,		///< 주은 사람이 임자
		ZDT_EQUAL,				///< 공동 분배

		NUM_ZDT
	};

	enum eItemDivType
	{
		IDT_PICKUP_PERSON,		///< 주은 사람이 임자
		IDT_ORDER,				///< 순서되로
		IDT_GRADE_ITEM,			///< 아이템 등급에 따라 기여도 분배
		IDT_CONTIBUTION,		///< 기여도 분배

		NUM_IDT
	};

	struct PartyMemberUI
	{
		SERIAL_HANDLE		hSerial;	///< 맴버의 시리얼
		gui::CStaticBox*	pName;		///< 파티원 이름
		gui::CStaticBox*	pScore;		///< 파티원 기여점수
		gui::CPanel*		pTributarySimbol;///< 종족과 직업 그림

		PartyMemberUI()
		:pName(NULL)
		,pScore(NULL)
		{
		}
	};

	///< 좌/중/우로 분리되어 각각 확대되는 3등분 윈도우
	struct Windowby3						
	{
		CSurfaceGui			Left;
		CSurfaceGui			Center;
		CSurfaceGui			Right;

		VOID	Render()
		{
			Left.Render();
			Center.Render();
			Right.Render();
		}
	};

	CCommunityParty();
	virtual ~CCommunityParty();

	virtual RwBool		Create(CNtlPLGui* pParent);
	virtual VOID		Update(RwReal fElapsed);
	virtual VOID		Destroy();

	virtual VOID		SwitchDialog(bool bOpen);

	virtual VOID		Clear();
	virtual VOID		ActiveTab(bool bActive);

	virtual VOID		HandleEvents( RWS::CMsg &msg );	
	
	virtual VOID		OnMouseDown(const CKey& key);
	virtual VOID		OnMouseUp(const CKey& key);
	virtual VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent);

	virtual VOID		OnPaint();

protected:	
	VOID		PartyLeave();

	VOID		SetZennyDivision(RwUInt8 byIndex);
	VOID		SetItemDivision(RwUInt8 byIndex);

	VOID		LockZennyButton(bool bLock);
	VOID		LockItemButton(bool bLock);

	VOID		SetMember(sPartyMember* pMember, RwUInt8 byIndex);
	VOID		DelMember(RwUInt8 byIndex);

	VOID		CheckInfoWindow();

	VOID		OnGotFocus();
	VOID		OnLostFocus();	

	VOID		OnReturn_PartyNameInput();

	VOID		ClickedPartyCreateButton(gui::CComponent* pComponent);
	VOID		ClickedSelectDivButton(gui::CComponent* pComponent);
	VOID		ClickedLockButton(gui::CComponent* pComponent);

	VOID		OnClicked_InvenButton(gui::CComponent* pComponent);
	VOID		OnClicked_CharmButton(gui::CComponent* pComponent);
	VOID		OnClicked_CPButton(gui::CComponent* pComponent);
	VOID		OnClicked_ItemtoCPButton(gui::CComponent* pComponent);

	VOID		LocateLeaderMark(RwInt32 iIndex);	///< 리더의 위치에 마크를 표시한다. iIndex <= 0 이면 표시하지 않는다.

	VOID		CalcSurfacePosition();

	VOID		AdjustSurface(gui::CSurface& surface);	///< 버튼에서 쓸 좌표로 surface를 이동시킨다

protected:	
	gui::CSlot			m_slotReturnInput;
	gui::CSlot			m_slotSelectZennyDivButton;
	gui::CSlot			m_slotSelectItemDivButton;
	gui::CSlot			m_slotZennyLockButton;
	gui::CSlot			m_slotItemLockButton;
	gui::CSlot			m_slotPartyCreateButton;
	gui::CSlot			m_slotInvenButton;
	gui::CSlot			m_slotCharmButton;
	gui::CSlot			m_slotCPButton;
	gui::CSlot			m_slotItemtoCPButton;
	gui::CSlot			m_slotGotFocus;
	gui::CSlot			m_slotLostFocus;

	CNtlPLGui*			m_pParent;					///< Owner class	

	gui::CStaticBox*	m_pPartyName;				///< '파티이름'
	CSurfaceGui			m_PartyNameBack;			///< 파티 이름 백보드

	gui::CStaticBox*	m_pMyPartyNameStatic;		///< 현재 파티 이름
	gui::CInputBox*		m_pPartNameInput;			///< 파티 이름 입력란
	gui::CButton*		m_pPartyButtonCreate;		///< 파티 생성



	CSurfaceGui			m_ZennySurface;				///< 'Zenny' 서페이스
	CSurfaceGui			m_ZennyDivSurface[NUM_ZDT];	///< 제니 분배 방식
	gui::CButton*		m_pSelectZennyDivButton;	///< 제니 분배 방식 변경 버튼
	CSurfaceGui			m_ZennyExplainPanel;		///< 제니 분배 설명 배경
	gui::CStaticBox*	m_pZennyDivExplain;			///< 제니 분배 설명
	gui::CButton*		m_pZennyLockButton;			///< 제니 분배 잠금 버튼

	CSurfaceGui			m_ItemSurface;				///< 'Item' 서페이스
	CSurfaceGui			m_ItemDivSurface[NUM_IDT];	///< 아이템 분배 방식
	gui::CButton*		m_pSelectItemDivButton;		///< 아이템 분배 방식 변경 버튼
	CSurfaceGui			m_ItemExplainPanel;			///< 아이템 분배 설명 배경
	gui::CStaticBox*	m_pItemDivExplain;			///< 아이템 분배 설명
	gui::CButton*		m_pItemLockButton;			///< 아이템 분배 잠금 버튼


	CSurfaceGui			m_MemberHeadLine;			///< 파티 맴버 해드라인
	gui::CStaticBox*	m_MemberStatic;				///< '파티 맴버'
	gui::CStaticBox*	m_ScoreStatic;				///< '기여점수'
	CSurfaceGui			m_LeaderMark;				///< 파티장 마크
	CSurfaceGui			m_srfMemberboard;			///< 맴버 리스트 배경
	PartyMemberUI		m_PartyMemberInfo[NTL_MAX_MEMBER_IN_PARTY];	///< 파티원 정보	


	gui::CButton*		m_PartyInvenButton;			///< 파티 인벤 버튼		
	gui::CButton*		m_pCharmButton;				///< 부적 버튼
	CSlotGui			m_PartyCharmSlot[NTL_PARTY_CHARM_INVENTORY_COUNT];	///< 파티 부적 슬롯
	CSurfaceGui			m_CharmBackpanel;			///< 부적 패널
	
	CSurfaceGui			m_CPHeadLine;				///< CP 해드라인
	gui::CStaticBox*	m_CPStatic;					///< 'CP 변환'
	gui::CStaticBox*	m_CPPoint;					///< 실제 보유 CP
	gui::CStaticBox*	m_CPExplain;				///< CP 설명
	gui::CButton*		m_pCPButton;				///< CP 버튼
	gui::CButton*		m_pItemtoCPButton;			///< 아이템을 CP로 전환 버튼
	
	RwInt32				m_iLeaderIndex;				///< 파티장 인덱스
	RwInt8				m_byInfoWindowIndex;

	RwUInt8				m_byItemDivisionType;		///< 현재 그려지고 있는 아이템 분배 방식
	RwUInt8				m_byZennyDivisionType;		///< 현재 그려지고 있는 제니 분배 방식

	RwUInt8				m_bySelectedItemDivisionType;	///< 현재 아이템 분배 방식
	RwUInt8				m_bySelectedZennyDivisionType;	///< 현재 제니 분배 방식
};