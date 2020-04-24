/*****************************************************************************
 *
 * File			: DisplayStringManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 22	
 * Abstract		: DBO display string manager.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DISPLAY_STRING_MANAGER_H__
#define __DISPLAY_STRING_MANAGER_H__

#include "DisplayParser.h"

struct string_comparer
{
public:
	bool operator()(const std::string& a, const std::string& b) const
	{
		return a.compare(b)<0;
	}
};

class CDisplayStringManager
{
public:

	enum EDispType
	{
		CLIENT_ONLY_MSG,
        SERVER_SYNC_RESULT_MSG
	};


private:

	std::map<std::string, std::wstring, string_comparer> m_mapClientString;
	std::map<std::string, std::wstring, string_comparer> m_mapServerString;

	void AddString(RwUInt32 eType, const std::string& wKey, const std::wstring& wstr);

public:

	static CDisplayStringManager* GetInstance(void);
	
	const WCHAR* GetString(std::string key, RwUInt32 eType = CLIENT_ONLY_MSG);

	void InsertOptions(const CDisplayParser& Parser, RwUInt32 eType = CLIENT_ONLY_MSG);
};

static CDisplayStringManager* GetDisplayStringManager(void)
{
	return CDisplayStringManager::GetInstance(); 
}

#endif
