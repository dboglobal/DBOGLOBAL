#ifndef __SERVER_STRING_SEPERATOR__
#define __SERVER_STRING_SEPERATOR__


#include <vector>
#include <list>
#include "ControlScriptGlobal.h"

class CStringSeparator
{

public:

	typedef sSSD_MESSAGE::eDEFINE CStringSeparator::eDEFINE;

	typedef std::vector<std::string> SEPARATOR_VEC;
	typedef SEPARATOR_VEC::iterator SEPARATOR_IT;
	typedef SEPARATOR_VEC::value_type SEPARATOR_VAL;

	typedef std::list<std::string> TOKEN_LIST;
	typedef TOKEN_LIST::iterator TOKEN_IT;
	typedef TOKEN_LIST::value_type TOKEN_VAL;

	enum eFLOAT_STATE
	{
		FLOAT_STATE_OPEN,
		FLOAT_STATE_LEFT,
		FLOAT_STATE_POINT,
		FLOAT_STATE_RIGHT,
		INVALID_FLOAT_STATE = 0xFF,
	};

public:
	CStringSeparator();
	virtual ~CStringSeparator();


public:

	void					ClearToken();

	bool					AddSeparator(std::string strSeparator);

	bool					AddRemover(std::string strSeparator);

	bool					Separate(std::string& strBuffer);

	static bool				IsFloat(std::string & rNumber);

	static bool				IsInteger(std::string & rNumber);

	static bool				IsUnsignedInteger(std::string & rNumber);


	TOKEN_IT				Begin() { return m_listToken.begin(); }
	TOKEN_IT				End() { return m_listToken.end(); }

	DWORD					GetCount() { return (DWORD)m_listToken.size(); }



protected:

	SEPARATOR_VEC			m_vecSeparator;

	SEPARATOR_VEC			m_vecRemover;

	TOKEN_LIST				m_listToken;


};


#endif