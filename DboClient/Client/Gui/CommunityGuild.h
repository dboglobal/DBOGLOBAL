/******************************************************************************
* File			: CommunityGuild.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 4. 19
* Abstract		: 
*****************************************************************************
* Desc			: 길드 커뮤니티 창
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// shared
#include "NtlSharedType.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "CommunityBase.h"
#include "SurfaceGui.h"
#include "Windowby3.h"
#include "RwTextureGui.h"

struct sGuildMember;
class CGuildEmblemMakerGui;
class CGuildNoticeGui;
class CNtlGuild;

class CGuildMemberGui
{
public:
	CGuildMemberGui(CNtlPLGui* pParent);
	virtual ~CGuildMemberGui();

	VOID	Destory();
	VOID	Clear();

	VOID	SetPosition(RwInt32 iParentiX, RwInt32 iParentiY, RwInt32 iX, RwInt32 iY);
	VOID	GetPosition(CRectangle& rect);

	VOID	SetMember(sGuildMember* pGuildMember);

	CHARACTERID GetCharID();

	VOID	Show(bool bShow);
	RwBool	PtInRect(RwInt32 iXPos, RwInt32 iYPos);

protected:
	VOID	SetMasterColor();
	VOID	SetSecondMasterColor();
	VOID	SetNormalColor();
	VOID	SetOffLineColor();

	VOID	ClickedMemberButton(gui::CComponent* pComponent);

public:
	gui::CSlot			m_slotMemberMenuButton;

	CNtlPLGui*			m_pParent;

	gui::CPanel*		pTributarySimbol;///< 종족과 직업 그림
	gui::CStaticBox*	m_pName;
	gui::CStaticBox*	m_pLevel;
	gui::CStaticBox*	m_pArea;
	gui::CButton*		m_pMemberMenuButton;

	CRectangle			m_rtPos;

	CHARACTERID			m_charID;
};

#define dVISIBLE_LIST_COUNT					8

class CCommunityGuild : public CCommunityBase
{
public:
	CCommunityGuild();
	virtual ~CCommunityGuild();

	virtual RwBool		Create(CNtlPLGui* pParent);
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
	VOID				OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);
	VOID				OnCaptureMouseDown(const CKey& key);

	virtual VOID		OnPaint();

	SERIAL_HANDLE		GetSeletedID();

protected:
	VOID		GuildLeave();

	VOID		AddMember(CHARACTERID charID);
	VOID		DelMember(CHARACTERID charID);

	VOID		SetOnlineMemberCount(CNtlGuild* pGuild);
	VOID		RefreshList(RwInt32 iOffSet);

	VOID		EmblemButton_Change();
	VOID		EmblemButton_Reset();

	VOID		ClearEmblemMaker();
	VOID		ClearNoticeGui();
	VOID		CheckInfoWindow();

	// Sig
	VOID		OnScrollChanged(RwInt32 iOffset);

	VOID		ClickedEmblemButton(gui::CComponent* pComponent);
	VOID		ClickedLeaveButton(gui::CComponent* pComponent);
	VOID		ClickedGhymButton(gui::CComponent* pComponent);

	VOID		ClickedNameButton(gui::CComponent* pComponent);
	VOID		ClickedLevelButton(gui::CComponent* pComponent);
	VOID		ClickedAreaButton(gui::CComponent* pComponent);

	VOID		ClickedNoticeButton(gui::CComponent* pComponent);

protected:	
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;
	gui::CSlot			m_slotEmblemButton;
	gui::CSlot			m_slotEmblemModifyButton;
	gui::CSlot			m_slotLeaveButton;
	gui::CSlot			m_slotGhymButton;
	gui::CSlot			m_slotNameButton;
	gui::CSlot			m_slotLevelButton;
	gui::CSlot			m_slotAreaButton;	
	gui::CSlot			m_slotNoticeButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;

	CNtlPLGui*			m_pParent;					///< Owner class	

	gui::CButton*		m_pEmblemButton;			///< 길드 문장 버튼
	gui::CButton*		m_pEmblemModifyButton;		///< 길드 문장 수정 버튼
	gui::CButton*		m_pLeaveButton;				///< 길드 탈퇴 버튼
	gui::CButton*		m_pGhymButton;				///< 길드 도장 버튼
	gui::CButton*		m_pNameButton;				///< 이름 정렬 버튼
	gui::CButton*		m_pLevelButton;				///< 레벨 정렬 버튼
	gui::CButton*		m_pAreaButton;				///< 지역 정렬 버튼
	gui::CButton*		m_pNoticeButton;			///< 공지 버튼

	gui::CStaticBox*	m_pGuildName;				///< 길드 이름

	gui::CStaticBox*	m_pCurGuildPointText;		///< Current guild points
	gui::CStaticBox*	m_pMaxGuildPointText;		///< Max guild points

	gui::CStaticBox*	m_pCurGuildPoint;			///< Current guild points
	gui::CStaticBox*	m_pMaxGuildPoint;			///< Max guild points


	gui::CStaticBox*	m_pGuildNum;				///< 길드인원
	gui::CStaticBox*	m_pGuildNoticeStatic;		///< '길드 공지'
	gui::CStaticBox*	m_pGuildNoticeTime;			///< 공지 날짜
	gui::CStaticBox*	m_pGuildNoticer;			///< 공지자
	gui::COutputBox*	m_pNotice;					///< 공지내용

	gui::CScrollBar*	m_pScrollbar;

	CRwTextureGui*		m_pEmblemButtonImage;

	CWindowby3			m_GuildMemBackSurface;		///< 길드 맴버 배경
	CWindowby3			m_NoticeSurface;			///< 길드 공지 배경

	CSurfaceGui			m_srfHumanShapeSurface;		///< 사람 모양
	CSurfaceGui			m_srfNoticeBar;				///< 길드 공지 바	

	RwBool				m_bNameAscendingSort;		///< 이름의 오름차순인지 여부
	RwBool				m_bLevelAscendingSort;		///< 레벨의 오름차순인지 여부
	RwBool				m_bAreaAscendingSort;		///< 지역의 오름차순이지 여부	

	RwInt32				m_iOnlineMember;			///< 현재 접속중인 길드 맴버수

	CHARACTERID			m_SelectedCharID;			///< 현재 선택된 길드원 ID

	RwInt32				m_iVisibleStart;			///< 화면에 보이는 MemGUI의 최초 인덱스
	RwInt8				m_byInfoMemberIndex;	

	CGuildMemberGui*	m_aGuildMemberGui[dVISIBLE_LIST_COUNT];

	CGuildEmblemMakerGui*	m_pEmblemMakerGui;
	CGuildNoticeGui*		m_pNoticeGui;
};