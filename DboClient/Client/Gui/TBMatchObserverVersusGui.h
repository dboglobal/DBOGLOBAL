/*****************************************************************************
* File			: TBMatchObserverVersusGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 24
* Abstract		: 천하제일 무도회 관전자 VS GUI
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TB_MATCH_OBSERVER_VERSUS_GUI_H__
#define __TB_MATCH_OBSERVER_VERSUS_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NumberGui.h"

class CTBMatchObserverVersusGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	enum eVersusType { TYPE_INDI, TYPE_TEAM, TYPE_NUMS, INVALID_TYPE = 0xFF };

	CTBMatchObserverVersusGui(const RwChar* pName);
	virtual ~CTBMatchObserverVersusGui();

	// Create & Destroy
	RwBool		Create();
	VOID		CreateRedScoreNumber();
	VOID		CreateBlueScoreNumber();
	VOID		CreateLeftTimeNumber();
	VOID		Destroy();

	VOID		Update( RwReal fElapsed );

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg& msg);

	// Implements
	VOID		SetVersusType();
	VOID		SetInterfaceByType( RwUInt8 byType );
	VOID		SetAdjustPosition();
	VOID		SetCurrentScore();
	VOID		SetScore( RwUInt8 byRedScore, RwUInt8 byBlueScore );
	VOID		SetTime( RwUInt32 nTime );

	VOID		SetTimer( RwBool bPlay, RwReal fTime );

	// Callback
	VOID		OnPaint();
	VOID		OnMove( RwInt32 nOldX, RwInt32 nOldY );

protected:
	gui::CPanel*		m_pPnlRedPanel;
	gui::CPanel*		m_pPnlBluePanel;
	gui::CStaticBox*	m_pStbRedTeam;
	gui::CStaticBox*	m_pStbBlueTeam;
	gui::CPanel*		m_pPnlScore;

	CNumberGui			m_numRedScore;
	CNumberGui			m_numBlueScore;
	CNumberGui			m_numLeftTime;

	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;

	RwUInt8				m_byType;
	RwBool				m_bTimeOn;
	RwReal				m_fTime;
};

#endif