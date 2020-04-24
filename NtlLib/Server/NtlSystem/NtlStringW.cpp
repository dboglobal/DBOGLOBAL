//***********************************************************************************
//
//	File		:	NtlStringW.cpp
//
//	Begin		:	2006-10-16
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlStringW.h"

const unsigned int	MAX_FORMAT_STR_BUFF = 2048;


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW::CNtlStringW(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW::~CNtlStringW(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW::CNtlStringW(const WCHAR* pwszString)//:std::wstring(pwszString)
{
	m_str.assign( pwszString );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW::CNtlStringW(const char * pszString)
{
	*this = pszString;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW & CNtlStringW::operator=(const char * pszString)
{
	int nWStrLen = MultiByteToWideChar( ::GetACP(), 0, pszString, -1, NULL, 0 );
	WCHAR * pwString = new WCHAR[ nWStrLen ];
	if( pwString )
	{
		MultiByteToWideChar( ::GetACP(), 0, pszString, -1, pwString, nWStrLen );

		m_str.assign( pwString );

		delete[] pwString;
	}

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlStringW& CNtlStringW::operator=(const WCHAR* pwszString)
{
	m_str.assign(pwszString);

	return *this;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CNtlStringW::operator==(CNtlStringW& string)
{
	if (0 == wcscmp(c_str(), string.c_str()))
		return true;

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlStringW::Format(const WCHAR *format, ...)
{
	int nRV = 0;
	WCHAR szBuf[MAX_FORMAT_STR_BUFF] = { 0x00, };

	va_list valist;

	memset(szBuf, 0x00, sizeof(szBuf));

	va_start(valist, format);

#if ( _MSC_VER >= 1400 ) // VS8+
	nRV = vswprintf_s(szBuf, MAX_FORMAT_STR_BUFF, format, valist);
#else
	nRV = vswprintf(szBuf, format, valist);
#endif

	va_end(valist);

	if( nRV > 0 )
		m_str.assign( szBuf );
	else
		m_str.clear();

	return nRV;
}