/*****************************************************************************
 *
 * File			: NtlLoadingController.h
 * Author		: JeongHo,Rho
 * Copyright	: (аж)NTL
 * Date			: 2007. 2. 13	
 * Abstract		: 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/
#ifndef _NTL_LOADINGCONTROLLER_H_
#define _NTL_LOADINGCONTROLLER_H_

#include "NtlDebug.h"

class CNtlLoadingController
{
// Member variables
protected:
	CRITICAL_SECTION*					m_pCS;

// Construction and Destructions
public:
	CNtlLoadingController( void )
	{
		m_pCS = NULL;
	}

	virtual ~CNtlLoadingController( void )
	{
		Delete();
	}

// Methods
public:
	static CNtlLoadingController*		GetInstance( void );

	bool								Create( void );
	void								Delete( void );

	void								Lock( void );
	void								Unlock( void );
};


inline bool CNtlLoadingController::Create( void )
{
	m_pCS = NTL_NEW CRITICAL_SECTION;
	InitializeCriticalSection( m_pCS );
	return true;
}

inline void CNtlLoadingController::Delete( void )
{
	if( m_pCS )
	{
		DeleteCriticalSection( m_pCS );

		NTL_DELETE( m_pCS );
	}
}

inline void CNtlLoadingController::Lock( void )
{
	if ( m_pCS )
	{
		EnterCriticalSection( m_pCS );
	}
}

inline void CNtlLoadingController::Unlock( void )
{
	if ( m_pCS )
	{
		LeaveCriticalSection( m_pCS );
	}
}


#endif
