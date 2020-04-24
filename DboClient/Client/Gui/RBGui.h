////////////////////////////////////////////////////////////////////////////////
// File: RBGui.h
// Desc: Rank Battle Gui Group
//       랭크배틀에서만 쓰이는 Gui들의 모음. 동적으로 생성 삭제 된다.
//
// 2007.08.23 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_GUI_H__
#define __RB_GUI_H__

#include "ceventhandler.h"

class CRBResultGui;
class CRBBattleInfoGui;
class CRBDirectionGui;
class CCounterGui;
class CResultNarrationGui;

class CRBGui : public RWS::CEventHandler
{
public:
	//! Destructor
	~CRBGui(VOID) {}

	//! Operation
	static	VOID CreateInstance(VOID);
	static	VOID DeleteInstance(VOID);

	VOID	Create(VOID);
	VOID	Destroy(VOID);

	VOID	SetCounterPosition( RwInt32 nScreenHeight );
	VOID	SetCounterTime( RwReal fTime );
	VOID	HideCounterTime(VOID);
	VOID	DirectionMode( RwBool bDirection );
	
	//! Access
	static CRBGui*	GetInstance(VOID);

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	//! Variable
	CRBResultGui*				m_pResultGui;
	CRBBattleInfoGui*			m_pBattleInfoGui;
	CRBDirectionGui*			m_pDirectionGui;
	CCounterGui*				m_pCounter;
	CResultNarrationGui*		m_pResultNarration;

	RwBool						m_bCreate;
	
	static CRBGui*				m_pInstance;

	//! Prevented default constructor
	CRBGui(VOID);
};

static CRBGui* GetRBGuiInstance(VOID)
{
	return CRBGui::GetInstance();
}

#endif//__RB_GUI_H__