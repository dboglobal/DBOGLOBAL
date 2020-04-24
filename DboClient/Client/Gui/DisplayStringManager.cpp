#include "precomp_dboclient.h"
#include "DisplayStringManager.h"


CDisplayStringManager* CDisplayStringManager::GetInstance(void)
{
	static CDisplayStringManager Instance;
	return &Instance;
}

void CDisplayStringManager::AddString(RwUInt32 eType, const std::string& wKey, const std::wstring& wstr )
{
	if (eType == CLIENT_ONLY_MSG)
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapClientString.find(wKey);
		if (it != m_mapClientString.end())
		{
			NTL_ASSERT(false, "DisplayString Error: Duplicate ID - " << wKey.c_str());
			return;
		}

		m_mapClientString[wKey] = wstr;
	}
	else
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapServerString.find(wKey);
		if (it != m_mapServerString.end())
		{
			NTL_ASSERT(false, "DisplayString Error: Duplicate ID - " << wKey.c_str());
			return;
		}

		m_mapServerString[wKey] = wstr;
	}

}

const WCHAR * CDisplayStringManager::GetString(std::string key, RwUInt32 eType)
{
	if (eType == CLIENT_ONLY_MSG)
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapClientString.find(key);
		if (it != m_mapClientString.end())
			return it->second.c_str();
	}
	else
	{
		std::map<std::string, std::wstring, string_comparer>::iterator it = m_mapServerString.find(key);
		if (it != m_mapServerString.end())
			return it->second.c_str();
	}

	static WCHAR awcBuffer[] = L"String not found";

	return awcBuffer;
}

void CDisplayStringManager::InsertOptions(const CDisplayParser& Parser, RwUInt32 eType /*= CLIENT_ONLY_MSG*/)
{
	NTL_FUNCTION("CDisplayStringManager::InsertOptions");


	const CDisplayParser::MapDataOption& DataOption = Parser.GetDataOptions();
	CDisplayParser::MapDataOption::const_iterator it;

	for(it = DataOption.begin(); it != DataOption.end(); ++it)
	{
		AddString(eType, it->first, it->second);
	}

	NTL_RETURNVOID();
}
