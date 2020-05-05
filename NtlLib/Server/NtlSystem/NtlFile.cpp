//***********************************************************************************
//
//	File		:	NtlFile.cpp
//
//	Begin		:	2007-03-19
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlFile.h"

#include "NtlError.h"

#include <io.h>
#include <tchar.h>
#include <filesystem>


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlFile::CNtlFile(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlFile::~CNtlFile(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFile::Init()
{
	m_bAutoClose = false;

	m_hFile = HFILE_ERROR;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFile::Destroy()
{
	Close();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlFile::Create(LPCTSTR lpszFileName, int nOperationFlag /* = _O_CREAT | _O_APPEND | _O_RDWR */, int nSharingFlag /* = _SH_DENYNO */, int nPermissionMode /* = _S_IREAD | _S_IWRITE */, bool bAutoClose /* = false */)
{
	if ((nOperationFlag & _O_CREAT) == _O_CREAT)		//if the file should be created
	{
		auto path = std::filesystem::path(lpszFileName).remove_filename();

		if (!std::filesystem::exists(path))				//check if the path exists
			std::filesystem::create_directories(path);  //and create directories
	}

	int rc = _tsopen_s( &m_hFile, lpszFileName, nOperationFlag, nSharingFlag, nPermissionMode );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}

	m_strFileName = lpszFileName;

	m_bAutoClose = bAutoClose;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFile::Close()
{
	if( IsOpened() && m_bAutoClose )
	{
		_close( m_hFile );
		m_hFile = HFILE_ERROR;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlFileStream::CNtlFileStream(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlFileStream::~CNtlFileStream(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFileStream::Init()
{
	m_pFilePtr = NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFileStream::Destroy()
{
	Close();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlFileStream::Create(LPCTSTR lpszFileName, int nOperationFlag, int nSharingFlag, int nPermissionMode)
{
	CNtlFile file;

	int rc = file.Create( lpszFileName, nOperationFlag, nSharingFlag, nPermissionMode, false );
	if( NTL_SUCCESS != rc )
	{
		return rc;
	}

	rc = Attach( file );
	if(  NTL_SUCCESS != rc )
	{
		return rc;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlFileStream::Attach(CNtlFile & rFile, LPCTSTR lpszMode /* = TEXT */)
{
	if( m_pFilePtr )
	{
		return NTL_ERR_SYS_OBJECT_ALREADY_CREATED;
	}


	m_pFilePtr = _fdopen( rFile.GetFileHandle(), lpszMode );
	if( NULL == m_pFilePtr )
	{
		return GetLastError();
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlFileStream::Close()
{
	if( IsOpened() )
	{
		fclose( m_pFilePtr );
		m_pFilePtr = NULL;
	}	
}
