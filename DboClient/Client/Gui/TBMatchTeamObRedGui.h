/*****************************************************************************
* File			: TBMatchTeamObRedGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 24
* Abstract		: 천하제일 무도회 관전자 팀전 레드팀 GUI
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TB_MATCH_TEAM_OB_RED_GUI_H__
#define __TB_MATCH_TEAM_OB_RED_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GaugeAnimation.h"
#include "NtlSlDef.h"
#include "NtlWorldConceptTB.h"

#define dTEAMMEMBER_GUI_OFFSET_Y	83
#define dTEAMMEMBER_GUI_HEIGHT		45

/**
* \brief 각 멤버들의 GUI
*/
class CTBMatchTeamRedMemberGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchTeamRedMemberGui();
	virtual ~CTBMatchTeamRedMemberGui();

	// Create & Destroy
	RwBool		Create( const WCHAR* pCharName, SERIAL_HANDLE hMemberSerial );
	VOID		Destroy();

	// Update
	VOID		Update( RwReal fElapsed );

	// Handle Events & Switch Dialog
	VOID		HandleEvents(RWS::CMsg& msg);

	// Implements
	VOID		UpdateMemberData( SERIAL_HANDLE hSerial );

protected: 
	gui::CPanel*		m_pPnlClass;
	gui::CPanel*		m_pPnlLeader;
	gui::CStaticBox*	m_pStbLevel;
	gui::CStaticBox*	m_pStbName;
	gui::CProgressBar*	m_pPgbLp;
	gui::CProgressBar*	m_pPgbEp;

	CGaugeAnimation		m_LpGauge;
	CGaugeAnimation		m_EpGauge;

	SERIAL_HANDLE		m_hMemberSerial;
	RwBool				m_bCreate;
};

/**
* \brief Red 팀의 GUI
*/
class CTBMatchTeamObRedGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchTeamObRedGui( const RwChar* pName );
	virtual ~CTBMatchTeamObRedGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	VOID		CreateMemberGui();
	VOID		DestroyMemberGui();

	VOID		Update( RwReal fElapsed );

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg& msg);

protected:
	CTBMatchTeamRedMemberGui*	m_pMemberGui;
	RwUInt8						m_byMemberNums;
	
	RwBool						m_bCreate;
};

#endif