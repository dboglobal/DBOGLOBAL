//***********************************************************************************
//
//	File		:	NtlString.h
//
//	Begin		:	2006-05-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include <string>

class CNtlString
{
public:
	CNtlString() {}
	CNtlString(const char * lpszStr):m_str(lpszStr) {}
	CNtlString(const WCHAR * pwszString);

public:
	std::string & GetString(void) { return m_str; }
	CNtlString & operator=(const char * lpszStr);
	CNtlString & operator=(const WCHAR * pwszString);
	
	int Format(const char *format, ...);
	const char* c_str(void) const { return m_str.c_str(); }
	void clear(void) { m_str.clear(); }

	bool operator<(const CNtlString &right) const
	{
		if ( m_str<right.m_str ) return true;
		else return false;
	}

private:
	 std::string m_str;
};

