////////////////////////////////////////////////////////////////////////////////
// File: TBTournamentMatchInfoGui.h
// Desc: Tournament Info Gui
//
// 2008.09.4 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_TOURNAMENT_MATCH_INFO_GUI_H__
#define __TB_TOURNAMENT_MATCH_INFO_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlBudokai.h"
#include "NumberGui.h"

class CTBTournamentMatchInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enumaration
	enum MATCHINFO { CLASS, LEVEL, BATTLESCORE, RANKPOINT, GUILDNAME, NUM_INFO };
	
	//! Structure
	struct stTEAM
	{
		gui::CStaticBox*	m_pstbName;
		gui::CStaticBox*	m_ppnlDojoEmblem;
		gui::CStaticBox*	m_pstbLine1[NUM_INFO];
		gui::CStaticBox*	m_pstbLine2[NUM_INFO];	
		CNumberGui			m_numScore;
	};
	
	//! Constructor and Destructor
	CTBTournamentMatchInfoGui( const RwChar* pName );
	~CTBTournamentMatchInfoGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	VOID	SetIndividualData( sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA* pData, RwUInt16 wTeamType );	// pData = NULL 이면 그냥 클리어.
	VOID	SetTeamData( sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA* pData, RwUInt16 wTeamType );	
	VOID	SetMatchData( RwUInt8 byDepth, RwUInt8 byIndex, RwUInt8 byLeftScore, RwUInt8 byRightScore );// Score = INVALID_BYTE 아예 표시 안함.

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation	
	VOID	SetCenterPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	ClearData( RwUInt16 wTeamType );

	//! Callback	
	VOID	OnClickClose( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnPaint(VOID);

	gui::CSlot	m_slotClickClose;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotPaint;

	//! Variable Battle Info
	gui::CStaticBox*	m_pstbTitle;
	gui::CStaticBox*	m_pstbMatch;
	gui::CButton*		m_pbtnClose;

	stTEAM				m_stTeam[2];
};


#endif//__TB_TOURNAMENT_MATCH_INFO_GUI_H__