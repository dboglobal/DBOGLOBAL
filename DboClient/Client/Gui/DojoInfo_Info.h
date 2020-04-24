/******************************************************************************
* File			: DojoInfo_Info.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: 도장 정보의 정보탭
*****************************************************************************/

#pragma once

// simulation
#include "NtlSLDef.h"

// cleint
#include "SurfaceGui.h"
#include "DojoInfo_Base.h"
#include "Windowby3.h"

class sDOJO_INFO;
class CGuildNoticeGui;
struct sDBO_DOJO_NPC_INFO;


class CDojoInfo_Info : public CDojoInfo_Base
{
public:
	CDojoInfo_Info();
	virtual ~CDojoInfo_Info(VOID);

	virtual RwBool		Create(CNtlPLGui* pParent);
	virtual VOID		Update(RwReal fElapsed);
	virtual VOID		Destroy();	
	
	virtual VOID		ActiveTab(bool bActive);

	virtual VOID		OnMouseDown(const CKey& key);
	virtual VOID		OnMouseUp(const CKey& key);
	virtual VOID		OnMove(RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent);

	virtual VOID		OnPaint();

	virtual VOID		SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex = INVALID_TBLIDX);
	virtual VOID		HandleEvents( RWS::CMsg &msg );

protected:
	virtual VOID		Clear();

	VOID				SetDojoInfo(sDBO_DOJO_NPC_INFO* pDBO_DOJO_NPC_INFO);

	VOID				ClearNoticeGui();

	VOID				OnClicked_TenkaichiPlayerButton(gui::CComponent* pComponent);
	VOID				OnClicked_NoticeButton(gui::CComponent* pComponent);

protected:
	CNtlPLGui*			m_pParent;

	gui::CSlot			m_slotTenkaichiPlayerButton;
	gui::CSlot			m_slotNoticeButton;

	gui::CButton*		m_pTenkaichiPlayerButton;
	gui::CButton*		m_pNoticeButton;

	gui::CStaticBox*	m_pDojoInfoStaticBar;
	gui::CStaticBox*	m_pDojoNameStatic;
	gui::CStaticBox*	m_pDojoLevelStatic;
	gui::CStaticBox*	m_pDojoAreaNameStatic;
	gui::CStaticBox*	m_pGuildNameStatic;
	gui::CStaticBox*	m_pGuildPointStatic;
	gui::CStaticBox*	m_pPublicPeaceStatic;
	gui::CStaticBox*	m_pTenkaichiPlayerStatic;
	gui::CStaticBox*	m_pDojoName;
	gui::CStaticBox*	m_pDojoLevel;
	gui::CStaticBox*	m_pDojoAreaName;
	gui::CStaticBox*	m_pGuildName;
	gui::CStaticBox*	m_pGuildPoint;
	gui::CStaticBox*	m_pPublicPeace;
	gui::CStaticBox*	m_pTenkaichiPlayer;

	gui::CStaticBox*	m_pNoticeBarStatic;
	gui::CStaticBox*	m_pNoticeCharName;

	gui::COutputBox*	m_pNoticeOutput;

	CGuildNoticeGui*	m_pNoticeGui;

	CSurfaceGui			m_srfDojoInfoBar;
	CSurfaceGui			m_srfNotifyBar;

	CWindowby3			m_NoticeBackground;

	RwUInt32			m_uiDojoTableIndex;
};