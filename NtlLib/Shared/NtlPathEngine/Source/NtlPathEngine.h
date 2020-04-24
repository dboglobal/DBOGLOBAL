/*****************************************************************************
* File			: NtlPathEngine.h
* Author		: Haesung, Cho
* Copyright		: (林)NTL
* Date			: 2009. 03. 10
* Abstract		: Pathengine wrapping project ( for client )
*****************************************************************************
* Desc:
* 
* This project is created for the client extension of the current PathEngine.
* It is a static library project. It is used in conjunction with the TestPathEngine project on the Tool side.
*
* Note1: When all work is stabilized, this project should be integrated with NtlNavi project
* Note2: When integration with NtlNavi is finished, delete TestPathEngine project from Tool side
* Note3: As a result of client's Pathengine R & D, SKIP (2009.04)
* 
* Worklist
* (￠) 1瞒 : Library interface 积己
* (￠) 2瞒 : PathEngine data create module 积己  ( Federation tile 积己 )
* (￠) 3瞒 : PathEngine data seamless loading module 积己
* (≮) Fourth: Interlocking with Client and Library
* () 5th: stabilization work
*****************************************************************************/

#ifndef _NTL_PATHENGINE_H_
#define _NTL_PATHENGINE_H_

#pragma once

#include <Windows.h>

class iPathEngine;
class iErrorHandler;
class CNtlPathEngineLog;

#include "NtlPathSeamlessManager.h"

class CNtlPathEngine
{
public:
	CNtlPathEngine(void);
	~CNtlPathEngine(void);

	typedef iPathEngine* (__stdcall* tGetInterfaceFunction)( iErrorHandler* );

	// SingleTon
	static CNtlPathEngine*	GetInstance();

	virtual bool	Create( const char* pPathEngineDllName, BOOL bSeamless = TRUE );
	virtual void	Update( float fElapsed );
	virtual void	Destroy( void );

	iPathEngine*	GetPathEngine();

	// Seamless
	void	SeamlessAddAgentSize( int nSize );

	void	SeamlessChangeWorld( int nWorldIndex );
	void	SeamlessUpdatePosition( int nPositionX, int nPositionY, int nPositionZ );

	void	SeamlessPathFinding( 
		int nAgentSize, int nSrcPositionX, int nSrcPositionY, int nSrcPositionZ,
		int nDestPositionX, int nDestPositionY, int nDestPositionZ, std::list< cPosition >& vec );


protected:
	HINSTANCE		m_hInstance;
	iPathEngine*	m_pPathEngine;

	CNtlPathSeamlessManager			m_seamManager;
	BOOL							m_bSeamless;

	// Currently Position
	int								m_nPositionX;
	int								m_nPositionY;
	int								m_nPositionZ;
};

static CNtlPathEngine* GetNtlPathEngine()
{
	return CNtlPathEngine::GetInstance();
}

#endif