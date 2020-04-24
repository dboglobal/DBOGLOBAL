////////////////////////////////////////////////////////////////////////////////
// File: TBMinorMatchInfoGui.h
// Desc: Tenkaichi Budokai Minormatch Info Gui
//
// 2008.06.26 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MINOR_MATCH_INFO_GUI_H__
#define __TB_MINOR_MATCH_INFO_GUI_H__

#include "NtlParty.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NumberGui.h"
#include "NtlWorldConceptTB.h"

#define MAX_TB_MINOR_ENEMYINFO	5

/**
* \brief Eneny Info
* \ingroup client
*/
class CTBMinorMatchEnemyInfoItem
{
public:
	CTBMinorMatchEnemyInfoItem(VOID);
	~CTBMinorMatchEnemyInfoItem(VOID);
	
	//! Attributes
	RwUInt16 GetTeamType(VOID) { return m_wTeamType; }

	//! Operation
	VOID	Clear(VOID);
	VOID	SetUI( gui::CStaticBox* pName, gui::CStaticBox* pKillCount, gui::CProgressBar* pTeamMember, gui::CProgressBar* pKilledTeamMember );
	VOID	SetData( RwUInt16 wTeamType, const WCHAR* szName, RwInt8 byKillCount );
	VOID	SetKillCount( RwUInt8 byKillCount );
	VOID	SetMemberState( RwUInt16 wTeamType );
		
	VOID	Show( RwBool bShow );	

private:
	//! Variable
	gui::CStaticBox*	m_pName;
	gui::CStaticBox*	m_pKillCount;
	gui::CProgressBar*	m_pTeamMember;
	gui::CProgressBar*	m_pKilledTeamMember;	

	RwUInt16			m_wTeamType;
};

class CTBMinorMatchInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Info GUI Type
	enum eInfoType { INFO_ENTRANCE, INFO_OBSERVER, INFO_NUMS, INFO_INVALID = 0xFF };

	//! Constructor and Destructor
	CTBMinorMatchInfoGui( const RwChar* pName );
	~CTBMinorMatchInfoGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	VOID	HandleEventEntranceMinorMatchPlayerState( RWS::CMsg& msg );
	VOID	HandleEventObserverMinorMatchPlayerState( RWS::CMsg& msg );
	VOID	HandleEventEntranceUpdateScore( RWS::CMsg& msg );
	VOID	HandleEventObserverUpdateScore( RWS::CMsg& msg );
	VOID	HandleEventEntranceMatchStateUpdate( RWS::CMsg& msg );
	VOID	HandleEventObserverMatchStateUpdate( RWS::CMsg& msg );

private:
	//! Implementation
	CTBMinorMatchEnemyInfoItem* FindEnemyInfoItem( RwUInt16 wTeamType );
	RwUInt32	GetTotalMemberCount(VOID);
	RwUInt32	GetAliveEnemyCount(VOID);
	RwUInt32	GetAliveCount(VOID);
	RwBool		IsEntrance();
	
	VOID		UpdateInfoType();
	VOID		SetInfoFrame();
	VOID		SetInfoOnce();
		
	//! Callback	
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnClickMinimize( gui::CComponent* pComponent );

	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotClickMinimize;

	//! Variable Enemy Info
	CTBMinorMatchEnemyInfoItem m_EnemyInfoArray[MAX_TB_MINOR_ENEMYINFO];
	std::list<stTBudokaiTeam*> m_listTeamSortByKillCount;
	
	//! Variable Battle Info
	gui::CStaticBox*	m_pstbNameTitle;
	gui::CStaticBox*	m_pstbKOTitle;
	gui::CStaticBox*	m_pstbTitle;
	gui::CButton*		m_pbtnMinimize;
	
	gui::CPanel*		m_pPnlSurvivedEnemy;
	gui::CPanel*		m_pPnlKilledEnemy;
	gui::CStaticBox*	m_pStbSurvivedPlayer;
	gui::CStaticBox*	m_pStbAllPlayer;

	CNumberGui			m_numAliveCount;
	CNumberGui			m_numKillCount;

	RwUInt8				m_byInfoType;
};

bool EnemyTeamSortByKillCount( const stTBudokaiTeam* lhs, const stTBudokaiTeam* rhs );


#endif//__TB_MINOR_MATCH_INFO_GUI_H__