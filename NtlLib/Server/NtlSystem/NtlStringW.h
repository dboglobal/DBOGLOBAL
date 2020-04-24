#pragma once


#include <string>

class CNtlStringW
{
public:
	CNtlStringW(void);
	CNtlStringW(const char * pszString);
	CNtlStringW(const WCHAR* pwszString);
	virtual ~CNtlStringW(void);

public:
	std::wstring &		GetString(void) { return m_str; }
	int					Format(const WCHAR *format, ...);
	CNtlStringW  &		operator=(const char * pszString);
	CNtlStringW  &		operator=(const WCHAR* pwszString);
	bool				operator==(CNtlStringW& string);
	const wchar_t* c_str(void) const { return m_str.c_str(); }

	bool operator<(const CNtlStringW &right) const
	{
		if ( m_str<right.m_str ) return true;
		else return false;
	}

private:
	std::wstring		m_str;
};
