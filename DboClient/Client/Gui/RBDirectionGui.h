////////////////////////////////////////////////////////////////////////////////
// File: RBDirectionGui.h
// Desc: Rank Battle 연출 Gui
//
// 2008.03.14 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_Direction_GUI_H__
#define __RB_Direction_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "TBCommonDirectionGui.h"

class CRBDirectionGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Define
	enum { MAX_TEAM_MEMBER = 5 };

	//! Constructor and Destructor
	CRBDirectionGui( const RwChar* pName );
	~CRBDirectionGui(VOID);

	//! Operation
	VOID	Init(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	SizePosCalculation( gui::CComponent* pComponent, CRectangle rtOriginal, RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
	
	//! CallBack
	VOID	OnTitleMovieEnd(gui::CComponent* pComponent);
	VOID	OnKameTeamMovieEnd( gui::CComponent* pComponent );
	VOID	OnTsuruTeamMovieEnd( gui::CComponent* pComponent );
	VOID	OnKameTeamMemberMovieEnd( gui::CComponent* pComponent );
	VOID	OnTsuruTeamMemberMovieEnd( gui::CComponent* pComponent );

private:
	
	gui::CSlot			m_slotTitleMovieEnd;
	gui::CSlot			m_slotKameTeamMovieEnd;
	gui::CSlot			m_slotTsuruTeamMovieEnd;
	gui::CSlot			m_slotKameTeamMemberMovieEnd[MAX_TEAM_MEMBER];
	gui::CSlot			m_slotTsuruTeamMemberMovieEnd[MAX_TEAM_MEMBER];
	
	//! Variables
	gui::CDialog*		m_pdlgDirection;	// Direction용 부모객체.
	gui::CFlash*		m_pflaTitle;
	gui::CFlash*		m_pflaKameTeam;
	gui::CFlash*		m_pflaTsuruTeam;
	gui::CFlash*		m_pflaKameTeamMember[MAX_TEAM_MEMBER];
	gui::CFlash*		m_pflaTsuruTeamMember[MAX_TEAM_MEMBER];
	gui::CFlash*		m_pflaVS;

	stSCOREINFO			m_stScore;			// 점수.

	CRectangle			m_rtOriginalTitle;
	CRectangle			m_rtOriginalKameTeam;
	CRectangle			m_rtOriginalTsuruTeam;
	CRectangle			m_rtOriginalKameTeamMember[MAX_TEAM_MEMBER];
	CRectangle			m_rtOriginalTsuruTeamMember[MAX_TEAM_MEMBER];
	CRectangle			m_rtOriginalVS;

	RwBool				m_bFirstStageReady;
};

#endif//__RB_Direction_GUI_H__