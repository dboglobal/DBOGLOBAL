/*****************************************************************************
 *
 * File			: Utility.h
 * Author		:
 * Copyright	: (аж)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// Path
//
//////////////////////////////////////////////////////////////////////////


extern void AttachBackSlash( CString& strString );


extern CString GetSplitPath( CString strFullPath );


extern CString GetSplitFileName( CString strFullPath );


//////////////////////////////////////////////////////////////////////////
//
// Directory
//
//////////////////////////////////////////////////////////////////////////


extern bool MakeDirectory( const CString& strPath );


extern bool DeleteDirectory( const CString& strDir );


extern bool CopyDirectory( const CString& strSrc, const CString& strDest );


extern bool MoveDirectory( const CString& strSrc, const CString& strDest );
