////////////////////////////////////////////////////////////////////////////////
// File: TBAwardGui.h
// Desc: 천하제일 시상식 Gui
//
// 2008.10.24 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_AWARD_RESULT_GUI_H__
#define __TB_AWARD_RESULT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NumberGui.h"
#include "NtlMatch.h"

#define MAX_BUDOKAI_REWARD_ITEM 1

class sITEM_TBLDAT;

class CTBAwardResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! typedef
	typedef struct _stAwardItem
	{
		gui::CStaticBox*	m_pName;
		CNumberGui			m_numPoint;
		sITEM_TBLDAT*		m_pRewardItem;
		CRectangle			m_rtRewardItem;
		CSurfaceGui			m_surIcon;			
		gui::CStaticBox*	m_pstbStackCount;
	}stAWARDITEM;

	//! Constructor and Destructor
	CTBAwardResultGui( const RwChar* pName );
	~CTBAwardResultGui(VOID);

	//! Operation
	VOID	Init(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	VOID	Show( bool bOpen );

	VOID	CalcResize( RwUInt32 uiWidth, RwUInt32 uiHeight );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	VOID	SetResultData( RwUInt16* paTeanNameIndex, VOID* pData );
	VOID	SetRewardItem( RwUInt8 bySlotIdx, RwUInt32 tblidx, RwUInt8 byStackCount );
	VOID	DeleteRewardItem( RwUInt8 bySlotIdx );
	VOID	DeleteAllRewardItem(VOID);
	RwBool	CreateStackNumber( RwUInt8 bySlotIdx, RwUInt8 uiStackCount );
	VOID	DeleteStackNumber( RwUInt8 bySlotIdx );
	VOID	SetMouseOnIndex( RwInt32 nIndex );
	RwInt32	GetRewardSlotIndex( RwInt32 nX, RwInt32 nY );
	
	VOID	ShowAwardItem( RwUInt8 bySlotIdx );
	
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
	gui::CFlash*		m_pflaAward;
	gui::CFlash*		m_pflaEffect;
	stAWARDITEM			m_stAwardItem[MAX_FINALMATCH_RESULT];
	
	CSurfaceGui			m_surFocusSlot;
	RwInt32				m_nMouseOnIndex;

	RwReal				m_fWaitTime;	
};
#endif//__TBAward_RESULT_GUI_H__