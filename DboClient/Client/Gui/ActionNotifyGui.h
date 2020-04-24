////////////////////////////////////////////////////////////////////////////////
// File: ActionNotifyGui.h
// Desc: Action Notify. 3มู
//
// 2007.02.14 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ACTION_NOTIFY_GUI_H__
#define __ACTION_NOTIFY_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"

class CActionNotifyItem
{
public:
	//! Enum 
	enum FADESTATE { NONE, FADE_IN, SHOW, FADE_OUT };
	enum MOVESTATE { STOP, MOVE };
	
	CActionNotifyItem(VOID);
	~CActionNotifyItem(VOID);

	VOID SetStaticBox( gui::CStaticBox* pStaticBox );
	RwInt32 GetLine(VOID) { return m_nLine; }
	RwInt32 GetFadeState(VOID) { return m_eFadeState; }

	RwBool Update( RwReal fElapsedTime );

	VOID SetText( WCHAR* pText, INT nLine );
	VOID Move( RwUInt8 ucLine );
		
private:
	//! Implements
	VOID SetFadeState( RwInt32 nFadeState );
	RwInt32 GetYPos( RwInt32 nLine );

	//! Variable
	gui::CStaticBox* m_pstbActionText;

	RwReal	m_fCurrentFadeTime;
	RwReal	m_fCurrentMoveTime;
	RwInt32	m_nLine;
	RwInt32 m_nNewYPos;
	RwInt32 m_nOldYPos;
	RwInt32 m_eFadeState;
	RwInt32 m_eMoveState;
};

class CActionNotifyGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! enum
	enum { MAXLINE = 3 };
	
	//! Constructor & Desturctor
	CActionNotifyGui( const RwChar* pName );
	virtual ~CActionNotifyGui(VOID);

	//! Operation
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

private:
	//! Implementation	
	VOID	PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight );
		
	//! Variables
	CActionNotifyItem	m_ActionNotifyItem[MAXLINE];		
};

#endif//__ACTION_NOTIFY_GUI_H__

