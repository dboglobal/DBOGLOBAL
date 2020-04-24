////////////////////////////////////////////////////////////////////////////////
// File: TBMatchResultGui.h
// Desc: 천하제일 무도회 결과 Gui
//
// 2008.08.11 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MATCH_RESULT_GUI_H__
#define __TB_MATCH_RESULT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NumberGui.h"
#include "NtlMatch.h"

#define MAX_BUDOKAI_REWARD_ITEM 1

class sITEM_TBLDAT;
class CResultNarrationGui;

class CTBMatchResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! enum
	enum FLASHRESULT { LOSE, WIN, DRAW, NONE, UNEARNED_WIN };
	enum TEAM { BLUE, RED, TEAM_COUNT };
	//! Constructor and Destructor
	CTBMatchResultGui( const RwChar* pName );
	~CTBMatchResultGui(VOID);

	//! Operation
	VOID	Init(VOID);

	RwBool	Create( eMATCHWORLD_TYPE eMatchType );
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	VOID	Show( bool bOpen );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	RwBool	SetResultData( VOID* pData );
	VOID	SetRewardItem( RwInt32 eTeam, RwInt32 nSlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount );
	VOID	DeleteRewardItem( RwInt32 eTeam, RwInt32 nSlotIdx );
	VOID	DeleteAllRewardItem(VOID);
	RwBool	CreateStackNumber( RwInt32 eTeam, RwInt32 nSlotIdx, RwUInt8 uiStackCount );
	VOID	DeleteStackNumber( RwInt32 eTeam, RwInt32 nSlotIdx );
	VOID	HideResultComopnent(VOID);
	VOID	SetMouseOnIndex( RwInt32 nIndex );
	RwInt32	GetRewardSlotIndex( RwInt32 nX, RwInt32 nY );
	VOID	CalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	SetFlashResult( FLASHRESULT eResult );
	
	VOID	OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );
	VOID	OnMouseLeave( gui::CComponent* pComponent );

	gui::CSlot			m_slotFSCallBack;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	
	//! Variable
	CNumberGui			m_numResult[TEAM_COUNT];
	CNumberGui			m_numBonus[TEAM_COUNT];
	CNumberGui			m_numScore[TEAM_COUNT];
	gui::CStaticBox*	m_pstbName[TEAM_COUNT];
	gui::CStaticBox*	m_pstbInfo1[TEAM_COUNT];
	gui::CStaticBox*	m_pstbInfo2[TEAM_COUNT];
	gui::CFlash*		m_pflaResult;
	gui::CFlash*		m_pflaBar;
	
	sITEM_TBLDAT*		m_pRewardItem[TEAM_COUNT][MAX_BUDOKAI_REWARD_ITEM];
	CRectangle			m_rtRewardItem[TEAM_COUNT][MAX_BUDOKAI_REWARD_ITEM];
	CSurfaceGui			m_surIcon[TEAM_COUNT][MAX_BUDOKAI_REWARD_ITEM];	
	CSurfaceGui			m_surIconBack[TEAM_COUNT][MAX_BUDOKAI_REWARD_ITEM];
	gui::CStaticBox*	m_pstbStackCount[TEAM_COUNT][MAX_BUDOKAI_REWARD_ITEM];
	CSurfaceGui			m_surFocusSlot;
	
	RwReal				m_fWaitTime;	
	RwUInt8				m_eMatchType;

	RwInt32				m_nMouseOnIndex;

	RwBool				m_bTournamentTrigger;
	FLASHRESULT			m_eResult;
	RwBool				m_bPresent[TEAM_COUNT];
};


#endif//__TBMatch_RESULT_GUI_H__