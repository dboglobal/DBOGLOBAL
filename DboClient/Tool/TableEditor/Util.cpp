#include "pch.h"
#include "Util.h"
#include "TableFileNameList.h"
#include "NtlXMLDoc.h"
#include "TableFileNameList.h"
#include "NtlBitFlagManager.h"

CTableContainer* m_pTableContainer = nullptr;

bool CreateTableContainer(const char* pPath, CTable::eLOADING_METHOD eLoadingMethod)
{
	CNtlBitFlagManager flagManager;
	if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
	{
		return false;
	}

	CTableFileNameList fileNameList;
	if (false == fileNameList.Create())
	{
		return false;
	}

	flagManager.Set(CTableContainer::TABLE_ITEM);
	flagManager.Set(CTableContainer::TABLE_NEWBIE);
	flagManager.Set(CTableContainer::TABLE_SPEECH);

	fileNameList.SetFileName(CTableContainer::TABLE_ITEM, "Table_Item_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_NEWBIE, "Table_Newbie_Data");
	fileNameList.SetFileName(CTableContainer::TABLE_SPEECH, "Table_NPC_Speech_Data");

	m_pTableContainer = new CTableContainer;

	if (!m_pTableContainer->Create(flagManager, (char*)pPath, &fileNameList, eLoadingMethod, GetACP(), nullptr))
	{
		delete m_pTableContainer;

		return false;
	}

	return true;
}

void DeleteTableContainer()
{
	if (m_pTableContainer)
	{
		delete m_pTableContainer;
		m_pTableContainer = nullptr;
	}
}

CTableContainer* GetTableContainer()
{
	return m_pTableContainer;
}
