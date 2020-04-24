////////////////////////////////////////////////////////////////////////////////
// File: ResultNarrationGui.h
// Desc: ResultNarration
//
// 2008.11.14 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __RESULT_NARRATION_GUI_H__
#define __RESULT_NARRATION_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

class CResultNarrationGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	
	//! Constructor & Destructor
	CResultNarrationGui( const RwChar* pName );
	~CResultNarrationGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsed );

	VOID	Show( bool bShow );

	VOID	SetData( const RwChar* szFileName, const WCHAR* szMessage );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation
	VOID	CalcSize( RwInt32 nScreenWidth, RwInt32 nScreenHeight );

	//! Callback
	VOID	OnFSCallBack( const RwChar* szCommand, const RwChar* szArgs );

	gui::CSlot			m_slotFSCallBack;
	
	//! Variables
	gui::CHtmlBox*		m_phbxMessage;
	gui::CFlash*		m_pflaDirection;
	std::string			m_strFileName;
};

#endif