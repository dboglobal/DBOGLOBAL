#pragma once

#include "NtlSingleton.h"
#include <map>
#include <string>

class CReserveName : public CNtlSingleton<CReserveName>
{
public:

	CReserveName();
	virtual ~CReserveName();

public:

	void					LoadFile();
	void					ReloadFile();

	bool					IsReserved(std::string& strName, unsigned int accountid);

	bool					IsReserved(WCHAR* wchName, unsigned int accountid);

private:


	struct char_cmp {
		bool operator () (const std::string& a, const std::string& b) const
		{
			return _stricmp(a.c_str(), b.c_str()) < 0;
		}
	};

	std::map<const std::string, unsigned int, char_cmp>	m_mapNames; //first = name, second = accountid

};

#define GetReserveNameSystem()		CReserveName::GetInstance()
#define g_pReserveName				GetReserveNameSystem()