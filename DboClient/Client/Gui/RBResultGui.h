////////////////////////////////////////////////////////////////////////////////
// File: RBResultGui.h
// Desc: Rank Battle °á°ú Gui
//
// 2007.08.23 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_RESULT_GUI_H__
#define __RB_RESULT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NumberGui.h"

class CRBResultGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! enum
	enum FLASHRESULT { LOSE, WIN, DRAW, NONE };
	enum TEAM { BLUE, RED, TEAM_COUNT };
	//! Constructor and Destructor
	CRBResultGui( const RwChar* pName );
	~CRBResultGui(VOID);

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
	VOID	CalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	SetFlashResult( FLASHRESULT eResult );

	VOID	OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );

	gui::CSlot			m_slotFSCallBack;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;	

	//! Variable
	CNumberGui			m_numResult[TEAM_COUNT];
	CNumberGui			m_numBonus[TEAM_COUNT];
	CNumberGui			m_numScore[TEAM_COUNT];
	gui::CStaticBox*	m_pstbName[TEAM_COUNT];
	gui::CStaticBox*	m_pstbInfo1[TEAM_COUNT];
	gui::CStaticBox*	m_pstbInfo2[TEAM_COUNT];
	gui::CFlash*		m_pflaResult;
	gui::CFlash*		m_pflaBar;

	RwReal				m_fWaitTime;	
	FLASHRESULT			m_eResult;
};

//class CRBResultGui : public CNtlPLGui, public RWS::CEventHandler
//{
//public:
//	//! Constructor and Destructor
//	CRBResultGui(VOID);
//	CRBResultGui( const RwChar* pName );
//	~CRBResultGui(VOID);
//
//	//! Operation
//	VOID	Init(VOID);
//
//	RwBool	Create(VOID);
//	VOID	Destroy(VOID);
//	VOID	Update( RwReal fElapsed );
//
//	VOID	Show( bool bOpen );
//
//	//! Attibutes
//
//	//! Event
//	VOID	HandleEvents( RWS::CMsg& msg );
//
//private:
//	VOID	SetResultData( VOID* pData );
//	
//	VOID	OnFlashMovieEnd( gui::CComponent* pComponent );
//	VOID	OnPaint(VOID);
//	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
//	VOID	OnClickOK( gui::CComponent* pComponent );
//	RwInt32	OnKameScoreEffectEnd(VOID);
//	RwInt32	OnColonEffectEnd(VOID);
//	RwInt32	OnTsuruScoreEffectEnd(VOID);
//	RwInt32	OnKamePointEffectEnd(VOID);
//	RwInt32	OnKameBonusPointEffectEnd(VOID);
//	RwInt32	OnTsuruPointEffectEnd(VOID);
//	RwInt32	OnTsuruBonusPointEffectEnd(VOID);
//
//	gui::CSlot			m_slotFlashMovieEnd;
//	gui::CSlot			m_slotPaint;
//	gui::CSlot			m_slotMove;
//	gui::CSlot			m_slotClickOK;
//	
//	//! Variable
//	gui::CStaticBox*	m_pstbKameName;
//	gui::CStaticBox*	m_pstbTsuruName;
//	CNumberGui			m_numKameStageScore;
//	CNumberGui			m_numTsuruStageScore;
//	CNumberGui			m_numKameScore;
//	CNumberGui			m_numKameBonusScore;
//	CNumberGui			m_numTsuruScore;
//	CNumberGui			m_numTsuruBonusScore;
//	CNumberGui			m_numColon;
//	gui::CStaticBox*	m_pstbKameRecord;
//	gui::CStaticBox*	m_pstbTsuruRecord;
//	gui::CFlash*		m_pflaGrade;
//	gui::CStaticBox*	m_pstbWaitTime;
//	
//	gui::CStaticBox*	m_pstbResultText;
//	gui::CStaticBox*	m_pstbWaitText;	
//	
//	gui::CButton*		m_pbtnOK;
//
//	RwReal				m_fWaitTime;	
//};


#endif//__RB_RESULT_GUI_H__