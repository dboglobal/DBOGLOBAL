/*****************************************************************************
*
* File			: LocalConfig.h
* Author		: 
* Copyright		: (аж) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CLocalConfig
//
//////////////////////////////////////////////////////////////////////////


class CLocalConfig
{
protected:
	CString					m_strPatchFileName;
	CString					m_strPatchFileVer;
	CString					m_strPatchFileDate;

public:
	CLocalConfig( void ) { return; }
	~CLocalConfig( void ) { return; }

public:
	bool					Load( CString strPath );

	const CString&			GetPatchFileName( void ) const;
	const CString&			GetPatchFileVer( void ) const;
	const CString&			GetPatchFileDate( void ) const;
};
