////////////////////////////////////////////////////////////////////////////////
// File: TBMajorDirectionGui.h
// Desc: 천하제일 무도회 본선 연출 Gui
//
// 2008.07.04 Peessi@hitel.net   
// 2008.09.18 Peessi@hitel.net 거의 엎음.
////////////////////////////////////////////////////////////////////////////////

#ifndef __TB_MAJOR_DIRECTION_GUI_H__
#define __TB_MAJOR_DIRECTION_GUI_H__

#include "TBCommonDirectionGui.h"

class CTBMajorDirectionGui : public CTBMainCommonDirectionGui
{
public:
	//! Enum
	enum FLASHID { WAIT, TITLE, ROUNDSTART, WINLOSE };

	//! Constructor and Destructor
	CTBMajorDirectionGui( const RwChar* pName );
	~CTBMajorDirectionGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	//! Attibutes

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	ReCalcPosition( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! CallBack
	//VOID	OnTitleMovieEnd( gui::CComponent* pComponent );
	//VOID	OnWinLoseMovieEnd( gui::CComponent* pComponent );
	
	//gui::CSlot			m_slotTitleMovieEnd;	
	gui::CSlot			m_slotWinLoseMovieEnd;
};

#endif //__TB_MAJOR_DIRECTION_GUI_H__