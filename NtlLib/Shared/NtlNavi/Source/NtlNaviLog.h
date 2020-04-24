#ifndef _NTL_NAVI_LOG_H_
#define _NTL_NAVI_LOG_H_


#include "NtlNaviSync.h"


class INtlNaviLog;


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviLog
//
//////////////////////////////////////////////////////////////////////////


class CNtlNaviLog
{
// Instance
public:
	static CNtlNaviLog*				GetInstance( void );


// Declarations
public:
	enum
	{
		LOG_MAX_BUFFER				= 2048
	};


// Member variables
protected:
	static char						s_szStrBuf[LOG_MAX_BUFFER];

	CNtlNaviCS						m_clLogCS;
	INtlNaviLog*					m_pLog;

// Constructions and Destructions
public:
	CNtlNaviLog( void );
	~CNtlNaviLog( void );


// Operations
public:
	void							SetLog( INtlNaviLog* pLog );

	void							Log( const char* pFormat, ... );
};


#endif