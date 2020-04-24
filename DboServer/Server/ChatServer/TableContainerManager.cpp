#include "stdafx.h"
#include "TableContainerManager.h"
#include "NtlBitFlagManager.h"
#include "TableFileNameList.h"

CTableContainerManager::CTableContainerManager()
{
}

CTableContainerManager::~CTableContainerManager()
{
}



//-----------------------------------------------------------------------------------
//		Loading Tables
//-----------------------------------------------------------------------------------
bool CTableContainerManager::CreateTableContainer(BYTE LoadTableFormat, char* pszPath)
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


	flagManager.Set(CTableContainer::TABLE_HLS_SLOT_MACHINE); //WORK
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_SLOT_MACHINE, "table_slot_machine_data");

	flagManager.Set(CTableContainer::TABLE_HLS_SLOT_MACHINE_ITEM); //WORK
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_SLOT_MACHINE_ITEM, "table_slot_machine_item_data");

	flagManager.Set(CTableContainer::TABLE_HLS_ITEM); //WORK
	fileNameList.SetFileName(CTableContainer::TABLE_HLS_ITEM, "Table_HLS_Item_Data");


	//set load table method/format
	CTable::eLOADING_METHOD eLoadMethod = (CTable::eLOADING_METHOD)LoadTableFormat;

	//load tables
	Create(flagManager, pszPath, &fileNameList, eLoadMethod, GetACP(), NULL);


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		LOAD LOCALIZED TABLE (update tables)
//-----------------------------------------------------------------------------------
bool CTableContainerManager::LoadLocalizedTable()
{
	//CNtlBitFlagManager flagManager;
	//if (false == flagManager.Create(CTableContainer::TABLE_COUNT))
	//{
	//	return false;
	//}

	//CTableFileNameList fileNameList;
	//if (false == fileNameList.Create())
	//{
	//	return false;
	//}

	//flagManager.Set(CTableContainer::TABLE_MOB);
	//fileNameList.SetFileName(CTableContainer::TABLE_MOB, "o_Table_MOB_Data");


	////update tables
	//Update(flagManager, &fileNameList);


	return TRUE;
}
