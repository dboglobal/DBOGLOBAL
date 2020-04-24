/*****************************************************************************
 *
 * File			: IntegritySys.h
 * Author		:
 * Copyright	: (аж)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/


#pragma once


#include "NtlMD5CheckSum.h"


//////////////////////////////////////////////////////////////////////////
//
// CIntegritySys
//
//////////////////////////////////////////////////////////////////////////


class CIntegritySys : public CNtlMD5CheckSum
{
public:
	CIntegritySys( void );
	~CIntegritySys( void );

protected:
	virtual bool				IsSkipFileOnChecksumBuild( char* szFileName, bool bFolder );
	virtual bool				OnEvent_ChecksumBuild( const char* szFileName, int returnCode );
};
