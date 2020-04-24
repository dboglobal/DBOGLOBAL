/*****************************************************************************
 *
 * File			: DLIntegritySys.h
 * Author		:
 * Copyright	: (аж)NTL
 * Date			:
 * Abstract		: 
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/


#pragma once


#include "DLEventDef.h"
#include "NtlMD5CheckSum.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLIntegritySys
//
//////////////////////////////////////////////////////////////////////////


class CDLIntegritySys : public CNtlMD5CheckSum
{
public:
	struct sIT_ERROR_FILE_INFO
	{
		CString					strPath;
		CString					strFileName;
	};

	typedef std::list< sIT_ERROR_FILE_INFO > listdef_ErrorFileList;

protected:
	bool						m_bPendingDestroy;
	listdef_ErrorFileList		m_defErrorFileList;

public:
	CDLIntegritySys( void );
	~CDLIntegritySys( void );

public:
	bool						Create( void );
	void						Delete( void );

	void						DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

	listdef_ErrorFileList*		GetErrorFileList( void ) { return &m_defErrorFileList; }

protected:
	bool						LoadSkipFileList( void );

	virtual bool				OnEvent_ChecksumTest( const char* szFileName, int returnCode );
};
