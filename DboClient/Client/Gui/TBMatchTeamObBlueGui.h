/*****************************************************************************
* File			: TBMatchTeamObBlueGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 24
* Abstract		: 천하제일 무도회 관전자 팀전 블루팀 GUI
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TB_MATCH_TEAM_OB_BLUE_GUI_H__
#define __TB_MATCH_TEAM_OB_BLUE_GUI_H__

#define dTEAMMEMBER_GUI_OFFSET_Y	83
#define dTEAMMEMBER_GUI_HEIGHT		45
#define dTEAMMEMBER_GUI_WIDTH		127

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GaugeAnimation.h"
#include "NtlSlDef.h"
#include "NtlWorldConceptTB.h"


/**
* \brief 각 멤버들의 GUI
*/
class CTBMatchTeamBlueMemberGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchTeamBlueMemberGui();
	virtual ~CTBMatchTeamBlueMemberGui();

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
* \brief Blue 팀의 GUI
*/
class CTBMatchTeamObBlueGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchTeamObBlueGui( const RwChar* pName );
	virtual ~CTBMatchTeamObBlueGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	VOID		CreateMemberGui();
	VOID		DestroyMemberGui();

	VOID		Update( RwReal fElapsed );

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg& msg);

protected:
	CTBMatchTeamBlueMemberGui*	m_pMemberGui;
	RwUInt8						m_byMemberNums;

	RwBool						m_bCreate;
};

#endif