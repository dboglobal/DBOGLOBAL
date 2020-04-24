/*****************************************************************************
*
* File			: GameLoadingThread.h
* Author		: Jeong Ho, Rho
* Copyright		: (аж)NTL
* Date			: 2006. 1. 3
* Abstract		: Game loading scene
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __GAME_LOADING_THREAD_H__
#define __GAME_LOADING_THREAD_H__


#include "Thread.h"

class CGameStage;


class CGameLoadingThread : public CThread
{
	// Declarations
public:
	enum eGAME_LOAD_STATE
	{
		eGAME_LOAD_STATE_WAIT,
		eGAME_LOAD_STATE_LOADING,
		eGAME_LOAD_STATE_LOADED
	};

	// Member variables
protected:

	CGameStage*							m_pGameStage;

	CCriticalSection					m_clCSLoaded;
	eGAME_LOAD_STATE					m_eGameLoadState;

	LPDIRECT3DDEVICE9					m_pDevice;

	// Constructions and Destructions
public:
	CGameLoadingThread( CGameStage* pGameStage );
	virtual ~CGameLoadingThread( void );

	// Methods
public:
	virtual int							ThreadCallBackFunc( void* pParam );

	eGAME_LOAD_STATE					GetLoadState( void );

	// Implementations
protected:

	void								SetLoadState( eGAME_LOAD_STATE eState );
};

#endif