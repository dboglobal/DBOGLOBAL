//***********************************************************************************
//
//	File		:	NtlIniFile.cpp
//
//	Begin		:	2006-01-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Myoung Jin, Choi		( yoshiki@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlIniFile.h"

const unsigned int MAX_BUFFER = 256;

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlIniFile::CNtlIniFile()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlIniFile::~CNtlIniFile()
{
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIniFile::Create(const char * lpszFullName)
{
	WIN32_FIND_DATA FindFileData;

	HANDLE hFind = FindFirstFile( lpszFullName, &FindFileData );
	if ( INVALID_HANDLE_VALUE == hFind ) 
	{
		return GetLastError();
	} 


	FindClose( hFind );

	m_strConfigFileName = lpszFullName;
	m_strLastReadGroup.GetString().clear();
	m_strLastReadKey.GetString().clear();


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlIniFile::Create(const char * lpszPathName, const char * lpszFileName)
{
	m_strConfigFileName.Format("%s/%s", lpszPathName, lpszFileName);

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, CNtlString &val)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	char buffer[MAX_BUFFER + 1] = { 0x00, };
	if( 0 == GetPrivateProfileString(group, key, NULL, buffer, MAX_BUFFER, m_strConfigFileName.c_str() ) )
	{
		return false;
	}

	val = buffer;
	
	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlString CNtlIniFile::Read(const char *group, const char *key)
{
	CNtlString strTemp;

	if( NULL == group || NULL == key )
	{
		return strTemp;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	if( !Read(group, key, strTemp) )
	{
		strTemp = "";
	}

	return strTemp;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, bool &bFlag)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	if( 0 == strTemp.GetString().compare("true") || 0 == strTemp.GetString().compare("TRUE") )
	{
		bFlag = true;
	}
	else
	{
		bFlag = false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, char &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (char) atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, short &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (short) atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, int &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, float &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (float) atof( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, unsigned char &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (unsigned char) atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, unsigned short &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (unsigned short) atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *group, const char *key, unsigned int &num)
{
	if( NULL == group || NULL == key )
	{
		return false;
	}

	m_strLastReadGroup = group;
	m_strLastReadKey = key;

	CNtlString strTemp;
	if( !Read(group, key, strTemp) )
	{
		return false;
	}

	num = (unsigned int) atoi( strTemp.c_str() );

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlIniFile::Read(const char *pszGroup, const char *pszKey, DWORD &dwNumber)
{
	if( NULL == pszGroup || NULL == pszKey )
	{
		return false;
	}

	m_strLastReadGroup = pszGroup;
	m_strLastReadKey = pszKey;

	CNtlString strTemp;
	if ( false == Read(pszGroup, pszKey, strTemp) )
	{
		return false;
	}

	dwNumber = (DWORD)(atoi(strTemp.c_str()));

	return true;
}