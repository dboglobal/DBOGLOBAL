/*****************************************************************************
 *
 * File			: NtlTokenOption.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 22	
 * Abstract		: tokenizer token option
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_TOKENOPTION_H__
#define __NTL_TOKENOPTION_H__

#include <map>
#include <string>
#include <stdio.h>
#include <rwcore.h>
#include "NtlDebug.h"

class CNtlTokenOptions
{
public:
//! Attributes:

	typedef std::multimap<std::string, std::string> MMapOPTIONS;
	typedef MMapOPTIONS::iterator Iterator;
	typedef MMapOPTIONS::const_iterator Const_Iterator;
	
	MMapOPTIONS	m_mmapOptions;

public:


	RwBool Exists(const std::string &strOptionName) const
	{
		return m_mmapOptions.find(strOptionName) != m_mmapOptions.end();
	}

	RwInt32 Count(const std::string &strOptionName) const
	{
		return (int)m_mmapOptions.count(strOptionName);
	}

	void RemoveOption(const std::string &strOptionName)
	{
		std::multimap<std::string, std::string>::iterator it = m_mmapOptions.find(strOptionName);
		while (it != m_mmapOptions.end())
		{
			if (it->first != strOptionName) break;
			std::multimap<std::string, std::string>::iterator it2 = it;
			++it;
			m_mmapOptions.erase(it2);
		}
	}

	const std::string& GetValue(const std::string &strOptionName, RwInt32 iOffset=0) const
	{ 
		std::multimap<std::string, std::string>::const_iterator it = m_mmapOptions.find(strOptionName);
		if (it == m_mmapOptions.end())
		{
			RwChar buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			sprintf_s(buf, 1024, "Missing component option '%s'", strOptionName.c_str());
#else
			sprintf(buf, "Missing component option '%s'", strOptionName.c_str());
#endif
			NTL_ASSERTFAIL(buf);
		}
		for (;iOffset>0;iOffset--)
		{
			it++;
			if (it == m_mmapOptions.end())
			{
				RwChar buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				sprintf_s(buf, 1024, "Missing component option '%s'", strOptionName.c_str());
#else
				sprintf(buf, "Missing component option '%s'", strOptionName.c_str());
#endif
				NTL_ASSERTFAIL(buf);
			}
		}
		return (*it).second; 
	}

	std::string& GetValueFactor(const std::string& strOptionName,RwInt32 iOffset=0) 
	{
		std::multimap<std::string, std::string>::iterator it = m_mmapOptions.find(strOptionName);
		if (it == m_mmapOptions.end())
		{
			char buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			sprintf_s(buf, 1024, "Missing component option '%s'", strOptionName.c_str());
#else
			sprintf(buf, "Missing component option '%s'", strOptionName.c_str());
#endif
			NTL_ASSERTFAIL(buf);
		}
		for (;iOffset>0;iOffset--)
		{
			it++;
			if (it == m_mmapOptions.end())
			{
				char buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				sprintf_s(buf, 1024, "Missing component option '%s'", strOptionName.c_str());
#else
				sprintf(buf, "Missing component option '%s'", strOptionName.c_str());
#endif
				NTL_ASSERTFAIL(buf);
			}
		}
		return (*it).second; 
	}

	const std::string &operator[](const std::string &strOptionName) const
	{
		return GetValue(strOptionName, 0);
	}

	//: Get value as int
	int GetValueAsInt(const std::string &strOptionName, int iOffset=0) const
	{
		const std::string &val = GetValue(strOptionName, iOffset);
		return atoi(val.c_str()); 
	}

	//: Get value as bool
	RwBool GetValueAsBool(const std::string &strOptionName, int iOffset=0) const
	{
		const std::string &val = GetValue(strOptionName, iOffset);
	
		if(val == "yes" || val == "true" ||	val == "on" || val == "1")
			return TRUE;
		else if(val == "no" || val == "false" || val == "off" || val == "0")
			return FALSE;

		return FALSE;
	}

	RwInt32 GetCount(void) const
	{
		return (RwInt32)m_mmapOptions.size(); 
	}
	
	void AddOption(const std::string &strOptionName, const std::string &strOptionValue)
	{
		m_mmapOptions.insert(MMapOPTIONS::value_type(strOptionName, strOptionValue));
	}
		
	//: Clear
	void Clear()
	{
		m_mmapOptions.clear();
	}

	Iterator Begin(void) { return m_mmapOptions.begin(); }
	Iterator End(void) { return m_mmapOptions.end(); }

	Const_Iterator Begin(void) const { return m_mmapOptions.begin(); }
	Const_Iterator End(void) const { return m_mmapOptions.end(); }
};

class CNtlTokenOptionsW
{
public:
	//! Attributes:

	typedef std::multimap<std::wstring, std::wstring> MMapOPTIONS;
	typedef MMapOPTIONS::iterator Iterator;
	typedef MMapOPTIONS::const_iterator Const_Iterator;

	MMapOPTIONS	m_mmapOptions;

public:


	RwBool Exists(const std::wstring &wstrOptionName) const
	{
		return m_mmapOptions.find(wstrOptionName) != m_mmapOptions.end();
	}

	RwInt32 Count(const std::wstring &wstrOptionName) const
	{
		return (int)m_mmapOptions.count(wstrOptionName);
	}

	void RemoveOption(const std::wstring &wstrOptionName)
	{
		std::multimap<std::wstring, std::wstring>::iterator it = m_mmapOptions.find(wstrOptionName);
		while (it != m_mmapOptions.end())
		{
			if (it->first != wstrOptionName) break;
			std::multimap<std::wstring, std::wstring>::iterator it2 = it;
			++it;
			m_mmapOptions.erase(it2);
		}
	}

	const std::wstring& GetValue(const std::wstring &wstrOptionName, RwInt32 iOffset=0) const
	{ 
		std::multimap<std::wstring, std::wstring>::const_iterator it = m_mmapOptions.find(wstrOptionName);
		if (it == m_mmapOptions.end())
		{
			wchar_t buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			swprintf_s(buf, 1024, L"Missing component option '%s'", wstrOptionName.c_str());
#else
			swprintf(buf, L"Missing component option '%s'", wstrOptionName.c_str());
#endif
			NTL_ASSERTFAIL(buf);
		}
		for (;iOffset>0;iOffset--)
		{
			it++;
			if (it == m_mmapOptions.end())
			{
				wchar_t buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				swprintf_s(buf, 1024, L"Missing component option '%s'", wstrOptionName.c_str());
#else
				swprintf(buf, L"Missing component option '%s'", wstrOptionName.c_str());
#endif
				NTL_ASSERTFAIL(buf);
			}
		}
		return (*it).second; 
	}

	std::wstring& GetValueFactor(const std::wstring& wstrOptionName,RwInt32 iOffset=0) 
	{
		std::multimap<std::wstring, std::wstring>::iterator it = m_mmapOptions.find(wstrOptionName);
		if (it == m_mmapOptions.end())
		{
			wchar_t buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			swprintf_s(buf, 1024, L"Missing component option '%s'", wstrOptionName.c_str());
#else
			swprintf(buf, L"Missing component option '%s'", wstrOptionName.c_str());
#endif
			NTL_ASSERTFAIL(buf);
		}
		for (;iOffset>0;iOffset--)
		{
			it++;
			if (it == m_mmapOptions.end())
			{
				wchar_t buf[1024];
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				swprintf_s(buf, 1024, L"Missing component option '%s'", wstrOptionName.c_str());
#else
				swprintf(buf, L"Missing component option '%s'", wstrOptionName.c_str());
#endif
				NTL_ASSERTFAIL(buf);
			}
		}
		return (*it).second; 
	}

	const std::wstring &operator[](const std::wstring &wstrOptionName) const
	{
		return GetValue(wstrOptionName, 0);
	}

	//: Get value as int
	int GetValueAsInt(const std::wstring &wstrOptionName, int iOffset=0) const
	{
		const std::wstring &val = GetValue(wstrOptionName, iOffset);
		return _wtoi(val.c_str()); 
	}

	//: Get value as bool
	RwBool GetValueAsBool(const std::wstring &wstrOptionName, int iOffset=0) const
	{
		const std::wstring &val = GetValue(wstrOptionName, iOffset);

		if(val == L"yes" || val == L"true" ||	val == L"on" || val == L"1")
			return TRUE;
		else if(val == L"no" || val == L"false" || val == L"off" || val == L"0")
			return FALSE;

		return FALSE;
	}

	RwInt32 GetCount(void) const
	{
		return (RwInt32)m_mmapOptions.size(); 
	}

	void AddOption(const std::wstring &wstrOptionName, const std::wstring &wstrOptionValue)
	{
		m_mmapOptions.insert(MMapOPTIONS::value_type(wstrOptionName, wstrOptionValue));
	}

	//: Clear
	void Clear()
	{
		m_mmapOptions.clear();
	}

	Iterator Begin(void) { return m_mmapOptions.begin(); }
	Iterator End(void) { return m_mmapOptions.end(); }

	Const_Iterator Begin(void) const { return m_mmapOptions.begin(); }
	Const_Iterator End(void) const { return m_mmapOptions.end(); }
};

#endif