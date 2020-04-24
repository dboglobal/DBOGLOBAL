/*****************************************************************************
* File			: NtlPathCS.h
* Author		: Haesung, Cho
* Copyright		: (аж)NTL
* Date			: 2009. 03. 19
* Abstract		: Pathengine Critical section for multi-thread
*****************************************************************************
* Desc:
* 
*****************************************************************************/

#ifndef _NTL_PATH_CS_H_
#define _NTL_PATH_CS_H_

class CNtlPathCS
{
protected:
	CRITICAL_SECTION					m_sCS;

public:
	CNtlPathCS( void )					{ InitializeCriticalSection( &m_sCS ); }
	~CNtlPathCS( void )					{ DeleteCriticalSection( &m_sCS ); }

public:
	void								Lock( void )		{ EnterCriticalSection( &m_sCS ); }
	void								Unlock( void )		{ LeaveCriticalSection( &m_sCS ); }
};

#endif