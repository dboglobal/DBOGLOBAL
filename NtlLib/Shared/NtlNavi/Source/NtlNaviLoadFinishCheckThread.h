#ifndef _NTL_NAVI_LOAD_FINISH_CHECK_THREAD_H_
#define _NTL_NAVI_LOAD_FINISH_CHECK_THREAD_H_


#include "NtlNaviSync.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviLoadFinishCheckThread
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviLoadFinishCheckThread
{
protected:
	HANDLE							m_hLoadingFinish;

	CNtlNaviCS						m_clExitCS;
	bool							m_bExit;

public:
	CNtlNaviLoadFinishCheckThread( void );
	~CNtlNaviLoadFinishCheckThread( void );


public:
	bool							Create( void );

	void							Delete( void );

	bool							Begin( void );

	void							End( void );

	HANDLE							GetLoadingFinishEvent( void );

	bool							IsExit( void );

	void							SetExit( bool bExit );

	virtual unsigned int			ThreadCallBackFunc( void ) = 0;


protected:
	static unsigned int __stdcall	ThreaFuncCB( void* pParam );
};



#endif