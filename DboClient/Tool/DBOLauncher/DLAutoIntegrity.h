/*****************************************************************************
*
* File			: DLAutoIntegrity.h
* Author		: 
* Copyright		: (аж)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLAutoIntegrity
//
//////////////////////////////////////////////////////////////////////////


class CDLAutoIntegrity
{
public:
	TCHAR				m_szHeader[1024];

protected:

public:
	CDLAutoIntegrity( void );
	~CDLAutoIntegrity( void );

public:
	bool				IsAutoIntegrity( void );
	void				MakeAutoIntegrity( bool bMake );
};


extern CDLAutoIntegrity g_clDLAutoIntegrity;