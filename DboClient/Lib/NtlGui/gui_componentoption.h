#ifndef __GUI_COMPONENTOPTION_H__
#define __GUI_COMPONENTOPTION_H__

#include "gui_define.h"

START_GUI

//: Sets the options for components
class CComponentOptions
{
public:
//! Attributes:

	typedef std::multimap<std::string, std::string> stlOPTIONS;
	typedef stlOPTIONS::iterator iterator;
	typedef stlOPTIONS::const_iterator const_iterator;

	//: Exists
	bool Exists(const std::string &option_name) const
	{
		return m_stlOptions.find(option_name) != m_stlOptions.end();
	}

	//: Count
	int Count(const std::string &option_name) const
	{
		return (int)m_stlOptions.count(option_name);
	}

	VOID RemoveOption(const std::string &option_name)
	{
		std::multimap<std::string, std::string>::iterator it = m_stlOptions.find(option_name);
		while (it != m_stlOptions.end())
		{
			if (it->first != option_name) break;
			std::multimap<std::string, std::string>::iterator it2 = it;
			++it;
			m_stlOptions.erase(it2);
		}
	}

	//: Getting the value of a unexisting option is undefined, so use the 'exists' function
	//: first to check if the option exists
	const std::string &GetValue(const std::string &option_name, int offset=0) const
	{ 
		std::multimap<std::string, std::string>::const_iterator it = m_stlOptions.find(option_name);
		if (it == m_stlOptions.end())
		{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			char buf[100];
			sprintf_s(buf, 100, "Missing component option '%s'", option_name.c_str());
			assert(0 && "Missing component option");
#else 
			char buf[100];
			sprintf(buf, "Missing component option '%s'", option_name.c_str());
			assert(0 && "Missing component option");
#endif
		}
		for (;offset>0;offset--)
		{
			it++;
			if (it == m_stlOptions.end())
			{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				
				char buf[100];
				sprintf_s(buf, 100, "Missing component option '%s'", option_name.c_str());
				assert(0 && "Missing component option");
#else
				char buf[100];
				sprintf(buf, "Missing component option '%s'", option_name.c_str());
				assert(0 && "Missing component option");
#endif
			}
		}
		return (*it).second; 
	}

	//: option_name에 해당하는 data factor를 수정하기 위하여 쓴다... (조심)
	std::string& GetValueFactor(const std::string& option_name,int offset=0) 
	{
		std::multimap<std::string, std::string>::iterator it = m_stlOptions.find(option_name);
		if (it == m_stlOptions.end())
		{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
			char buf[100];
			sprintf_s(buf, 100, "Missing component option '%s'", option_name.c_str());
			assert(0 && "Missing component option");
#else
			char buf[100];
			sprintf(buf, "Missing component option '%s'", option_name.c_str());
			assert(0 && "Missing component option");
#endif
		}
		for (;offset>0;offset--)
		{
			it++;
			if (it == m_stlOptions.end())
			{
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
				char buf[100];
				sprintf_s(buf, 100, "Missing component option '%s'", option_name.c_str());
				assert(0 && "Missing component option");
#else
				char buf[100];
				sprintf(buf, "Missing component option '%s'", option_name.c_str());
				assert(0 && "Missing component option");
#endif
			}
		}
		return (*it).second; 
	}
	
	const std::string &operator[](const std::string &option_name) const
	{
		return GetValue(option_name, 0);
	}

	//: Get value as int
	int GetValueAsInt(const std::string &option_name, int offset=0) const
	{
		const std::string &val = GetValue(option_name, offset);
		return atoi(val.c_str()); 
	}

	//: Get value as float
	float GetValueAsFloat(const std::string &option_name, int offset=0) const
	{
		const std::string &val = GetValue(option_name, offset);
		return (float)atof(val.c_str());
	}

	//: Get value as bool
	bool GetValueAsBool(const std::string &option_name, int offset=0) const
	{
		const std::string &val = GetValue(option_name, offset);
	
		if(val == "yes" ||
			val == "true" ||
			val == "on" ||
			val == "1")
			return true;
		else if(val == "no" ||
			val == "false" ||
			val == "off" ||
			val == "0")
			return false;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		char buf[200];
		sprintf_s(buf, 200, "Invalid bool value '%s' in component option '%s'", val.c_str(), option_name.c_str());
#else 
		char buf[200];
		sprintf(buf, "Invalid bool value '%s' in component option '%s'", val.c_str(), option_name.c_str());
#endif
		return false;
	}
	
//! Operations:
	//: Add option
	VOID AddOption(const std::string &option_name, const std::string &option_value)
	{
		m_stlOptions.insert(stlOPTIONS::value_type(option_name, option_value));
	}

	VOID AddOption( const CHAR* option_name, const INT option_value )
	{
		std::string options, value;
		char buf[256];

		options = option_name;
#if defined(_MSC_VER) && (_MSC_VER >= 1400)
		sprintf_s( buf, 256, "%d", option_value );
#else
		sprintf( buf, "%d", option_value );
#endif
		value = buf;

		AddOption( options, value );		
	}
		
	//: Clear
	VOID Clear()
	{
		m_stlOptions.clear();
	}

	iterator Begin(VOID) { return m_stlOptions.begin(); }
	iterator End(VOID) { return m_stlOptions.end(); }

	const_iterator Begin(VOID) const { return m_stlOptions.begin(); }
	const_iterator End(VOID) const { return m_stlOptions.end(); }

	//: Options
	stlOPTIONS	m_stlOptions;
};

END_GUI

#endif
