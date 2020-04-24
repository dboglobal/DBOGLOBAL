#ifndef _NTL_NAVI_SYNC_H_
#define _NTL_NAVI_SYNC_H_


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviCS
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviCS
{
protected:
	CRITICAL_SECTION					m_sCS;

public:
	CNtlNaviCS( void )					{ InitializeCriticalSection( &m_sCS ); }
	~CNtlNaviCS( void )					{ DeleteCriticalSection( &m_sCS ); }

public:
	void								Lock( void )		{ EnterCriticalSection( &m_sCS ); }
	void								Unlock( void )		{ LeaveCriticalSection( &m_sCS ); }
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviAutoCS
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviAutoCS
{
protected:
	CNtlNaviCS*							m_pCS;

public:
	CNtlNaviAutoCS( const CNtlNaviCS* pCS )
	{
		m_pCS = (CNtlNaviCS*)pCS;

		m_pCS->Lock();
	}

	~CNtlNaviAutoCS( void )
	{
		m_pCS->Unlock();
	}
};



#endif