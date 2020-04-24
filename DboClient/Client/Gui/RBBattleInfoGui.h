////////////////////////////////////////////////////////////////////////////////
// File: RBBattleInfoGui.h
// Desc: Rank Battle Info Gui
//
// 2008.02.11 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_BATTLE_ENEMY_PARTY_INFO_GUI_H__
#define __RB_BATTLE_ENEMY_PARTY_INFO_GUI_H__

#include "NtlParty.h"
#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NumberGui.h"

class CRBBattleEnemyInfoItem
{
public:
	CRBBattleEnemyInfoItem(VOID);
	~CRBBattleEnemyInfoItem(VOID);

	//! Attribute
	RwUInt32 GetSerial(VOID) { return m_hSerial; }

	//! Operation
	VOID	SetUI( gui::CStaticBox* pName );
	VOID	SetData( RwUInt32 hSerial, RwUInt8 byClass, std::wstring& wstrName );
	VOID	SetState( RwUInt8 byState );
	VOID	SetEnable( RwBool bEnable );
	VOID	InitData(VOID);

	VOID	Show( RwBool bShow );
	
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );

private:
	//! Variable
	gui::CStaticBox*	m_pName;
	CSurfaceGui			m_surClass;
	
	RwUInt32			m_hSerial;
};

class CRBBattleInfoGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Constructor and Destructor
	CRBBattleInfoGui( const RwChar* pName );
	~CRBBattleInfoGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

	//! Attibutes


	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	CRBBattleEnemyInfoItem* FindEnemyInfo( RwUInt32 hSerial );
	VOID	SetStraightWinScore( RwInt32 nStraightWinScore );
	VOID	SetPerfectWinScore( RwInt32 nPerfectWinScore );
	//! Callback	
	VOID	OnPaint(VOID);
	VOID	OnPostPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnClickMinimize( gui::CComponent* pComponent );

	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotPostPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot	m_slotClickMinimize;

	//! Variable Enemy Info
	CRBBattleEnemyInfoItem m_EnemyInfoArray[NTL_MAX_MEMBER_IN_PARTY];

	//! Variable Battle Info
	gui::CStaticBox*	m_pstbKameTeamName;
	gui::CStaticBox*	m_pstbTsuruTeamName;
	gui::CStaticBox*	m_pstbVS;
	gui::CStaticBox*	m_pstbEnemyMember;
	gui::CStaticBox*	m_pstbTitle;
	gui::CMDStaticBox*	m_pmdbStraightWinCount;
	gui::CMDStaticBox*	m_pmdbPerfectWinCount;
	gui::CButton*		m_pbtnMinimize;
	CNumberGui			m_numKameWinCount;
	CNumberGui			m_numTsuruWinCount;
};


#endif//__RB_BATTLE_ENEMY_PARTY_INFO_GUI_H__