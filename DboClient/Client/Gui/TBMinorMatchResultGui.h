////////////////////////////////////////////////////////////////////////////////
// File: TBMInorMatchResultGui.h
// Desc: 천하제일 무도회 예선 결과 Gui
//
// 2008.08.11 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MINOR_MATCH_RESULT_GUI_H__
#define __TB_MINOR_MATCH_RESULT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NumberGui.h"
#include "NtlMatch.h"

#define MAX_BUDOKAI_REWARD_ITEM 1

class sITEM_TBLDAT;
class CResultNarrationGui;

class CTBMinorMatchResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! enum
	enum FLASHRESULT { LOSE, WIN, NONE, UNEARNED_WIN, DOJO_RECOMMENDED, DRAW };
	//! Constructor and Destructor
	CTBMinorMatchResultGui( const RwChar* pName );
	~CTBMinorMatchResultGui(VOID);

	//! Operation
	VOID	Init(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	VOID	Show( bool bOpen );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	RwBool	SetResultData( VOID* pData );
	VOID	SetRewardItem( RwInt32 nSlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount );
	VOID	DeleteRewardItem( RwInt32 nSlotIdx );
	VOID	DeleteAllRewardItem(VOID);
	RwBool	CreateStackNumber( RwInt32 nSlotIdx, RwUInt8 uiStackCount );
	VOID	DeleteStackNumber( RwInt32 nSlotIdx );
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

	RwInt32	OnTotalPointEnd(VOID);	

	gui::CSlot			m_slotFSCallBack;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;

	//! Variable
	CNumberGui			m_numBattlePoint;
	CNumberGui			m_numKOBonus;
	CNumberGui			m_numWinBonus;
	CNumberGui			m_numTotalPoint;
	gui::CFlash*		m_pflaResult;
	gui::CFlash*		m_pflaBar;
	gui::CStaticBox*	m_pstbWinner;

	sITEM_TBLDAT*		m_pRewardItem[MAX_BUDOKAI_REWARD_ITEM];
	CRectangle			m_rtRewardItem[MAX_BUDOKAI_REWARD_ITEM];
	CSurfaceGui			m_surIcon[MAX_BUDOKAI_REWARD_ITEM];	
	CSurfaceGui			m_surIconBack[MAX_BUDOKAI_REWARD_ITEM];
	gui::CStaticBox*	m_pstbStackCount[MAX_BUDOKAI_REWARD_ITEM];
	CSurfaceGui			m_surFocusSlot;

	RwReal				m_fWaitTime;	
	
	RwInt32				m_nMouseOnIndex;

	FLASHRESULT			m_eResult;
};


#endif//__TBMatch_RESULT_GUI_H__