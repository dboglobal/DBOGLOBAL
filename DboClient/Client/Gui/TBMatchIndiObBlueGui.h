/*****************************************************************************
* File			: TBMatchIndiObBlueGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 11. 24
* Abstract		: 천하제일 무도회 관전자 개인전 블루팀 GUI
*****************************************************************************
* Desc         : 
*****************************************************************************/

#ifndef __TB_MATCH_INDI_OB_BLUE_GUI_H__
#define __TB_MATCH_INDI_OB_BLUE_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "GaugeAnimation.h"
#include "NtlSlDef.h"
#include "NtlWorldConceptTB.h"

class CTBMatchIndiObBlueGui : public CNtlPLGui, RWS::CEventHandler
{
public:
	// Construction & Destruction
	CTBMatchIndiObBlueGui( const RwChar* pName );
	virtual ~CTBMatchIndiObBlueGui();

	// Create & Destroy
	RwBool		Create();
	VOID		Destroy();

	VOID		Update( RwReal fElapsed );

	// HandleEvents & SwitchDialog
	VOID		HandleEvents(RWS::CMsg& msg);

	// Implements
	VOID		SetUserData();
	VOID		UpdateUserData(SERIAL_HANDLE hSerial);

protected:
	stTBudokaiMember*	GetBlueTeamUser();

protected:
	gui::CStaticBox*	m_pStbName;
	gui::CProgressBar*	m_pPgbLp;
	gui::CProgressBar*	m_pPgbEp;
	gui::CStaticBox*	m_pStbLevel;
	gui::CStaticBox*	m_pStbLp;
	gui::CStaticBox*	m_pStbEp;

	CGaugeAnimation		m_LpGauge;
	CGaugeAnimation		m_EpGauge;

	RwBool				m_bSetData;
};

#endif//__TB_MATCH_INDI_OB_BLUE_GUI_H__