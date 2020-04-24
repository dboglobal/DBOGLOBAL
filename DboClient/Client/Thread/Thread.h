/*****************************************************************************
 *
 * File			: Thread.h
 * Author		: Jeong Ho, Rho
 * Copyright	: (аж)NTL
 * Date			: 2007. 1. 3
 * Abstract		: Thread
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __THREAD_H__
#define __THREAD_H__


#include "CriticalSection.h"


#define RESULT_THREAD_SUCCESS			(0)
#define RESULT_THREAD_ERROR				(1)
#define RESULT_THREAD_FORCE_EXIT		(2)

class CWorkTreadMgr
{
public:
	typedef std::map< HANDLE, HANDLE > mapdef_ThreadList;

protected:
	mapdef_ThreadList					m_defTreadList;

public:
	void								AddThread( HANDLE hHandle );
	void								RemoveTread( HANDLE hHandle );
	int									GetThreadCnt( void );
};

extern CWorkTreadMgr					g_clWorkThreadMgr;


class CThread
{
// Member variables
protected:
	HANDLE								m_hThread;
	unsigned int						m_uiThreaID;

	CCriticalSection					m_clCSExit;
	bool								m_bExit;

// Constructions and Destructions
public:
	CThread( void );
	virtual ~CThread( void );

// Methods
public:
	bool								Create( void );
	void								Delete( void );

	void								SuspendThread( void );
	void								ResumeThread( void );

	virtual int							ThreadCallBackFunc( void* pParam );

// Implementations
protected:
	bool								IsExit( void );
	void								SetExit( bool bExit );

	static unsigned int __stdcall		ThreaFuncCB( void* pParam );
};


#endif