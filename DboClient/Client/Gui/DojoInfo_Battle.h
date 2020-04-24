/******************************************************************************
* File			: DojoInfo_Battle.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: 도장 정보의 전투탭
*****************************************************************************/

#pragma once

// cleint
#include "SurfaceGui.h"
#include "Windowby3.h"
#include "DojoInfo_Base.h"


class CDojoInfo_Battle : public CDojoInfo_Base
{
public:
#define dDUEL_PLAYER_NUM			(5)

	struct sDUEL_DEFENCE_PLAYER
	{
		CSurfaceGui				srfClass;

		gui::CStaticBox*		pName;
		gui::CStaticBox*		pLevel;
		gui::CStaticBox*		Race;

		gui::CButton*			pButton;

		gui::CSlot				sloButton;
	};


public:
	CDojoInfo_Battle();
	virtual ~CDojoInfo_Battle(VOID);

	virtual RwBool		Create(CNtlPLGui* pParent);
	virtual VOID		Update(RwReal fElapsed) {}
	virtual VOID		Destroy();	

	virtual VOID		Clear();
	virtual VOID		ActiveTab(bool bActive);

	VOID				OnClicked_DefencePlayerButton(gui::CComponent* pComponent);
	VOID				OnClicked_AskDuelButton(gui::CComponent* pComponent);
	VOID				OnClicked_AskScrambleButton(gui::CComponent* pComponent);

	virtual VOID		OnMouseDown(const CKey& key) {}
	virtual VOID		OnMouseUp(const CKey& key) {}
	virtual VOID		OnMove(RwInt32 nX, RwInt32 nY);
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY) {}
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent) {}

	virtual VOID		OnPaint();

	virtual VOID		SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex = INVALID_TBLIDX);
	virtual VOID		HandleEvents( RWS::CMsg &msg );

protected:
	CNtlPLGui*			m_pParent;

	gui::CSlot			m_slotAskDuelButton;
	gui::CSlot			m_slotAskScrambleButton;

	gui::CButton*		m_pAskDuelButton;
	gui::CButton*		m_pAskScrambleButton;

	gui::CStaticBox*	m_pDuelDefence_PlayerStatic;
	gui::CStaticBox*	m_pDuelDefence_NameStatic;
	gui::CStaticBox*	m_pDuelDefence_LevelStatic;
	gui::CStaticBox*	m_pDuelDefence_RaceStatic;

	gui::CStaticBox*	m_pDuelOffenceGuild_Static;
	gui::CStaticBox*	m_pDuelOffenceGuild_NameStatic;
	gui::CStaticBox*	m_pDuelOffenceGuild_Name;
	gui::CStaticBox*	m_pDuelOffenceGuild_MasterStatic;
	gui::CStaticBox*	m_pDuelOffenceGuild_Master;

	gui::CStaticBox*	m_pScrambleOffenceGuild_Static;
	gui::CStaticBox*	m_pScrambleOffenceGuild_NameStatic;
	gui::CStaticBox*	m_pScrambleOffenceGuild_Name;
	gui::CStaticBox*	m_pScrambleOffenceGuild_MasterStatic;
	gui::CStaticBox*	m_pScrambleOffenceGuild_Master;

	sDUEL_DEFENCE_PLAYER m_DuelDefencePlayerGui[dDUEL_PLAYER_NUM];

	CWindowby3			m_srf3DuelListBack;

	CSurfaceGui			m_srfDuelDefencePlayerBar;
	CSurfaceGui			m_srfDuelOffenceGuildBar;
	CSurfaceGui			m_srfScrambleOffenceGuildBar;

	RwUInt32			m_uiDojoTableIndex;
};