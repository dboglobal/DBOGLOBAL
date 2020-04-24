//***********************************************************************************
//
//	File		:	VehicleTable.cpp
//
//	Begin		:	2008-11-04
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//***********************************************************************************

#include "stdafx.h"
#include "VehicleTable.h"
#include "NtlDebug.h"
#include "NtlBattle.h"
#include "NtlSerializer.h"

WCHAR* CVehicleTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CVehicleTable::CVehicleTable(void)
{
	Init();
}

CVehicleTable::~CVehicleTable(void)
{
	Destroy();
}

bool CVehicleTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CVehicleTable::Destroy()
{
}

void CVehicleTable::Init()
{
}

void* CVehicleTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sVEHICLE_TBLDAT* pNewItem = new sVEHICLE_TBLDAT;
		if (NULL == pNewItem)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewItem;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewItem;
	}

	return NULL;
}

bool CVehicleTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sVEHICLE_TBLDAT* pItem = (sVEHICLE_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;
		return true;
	}

	return false;
}

bool CVehicleTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sVEHICLE_TBLDAT * pTbldat = (sVEHICLE_TBLDAT*) pvTable;
	sVEHICLE_TBLDAT * pExistTbldat = NULL;

	if( true == bReload )
	{
		pExistTbldat = (sVEHICLE_TBLDAT*)FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			return true; 
		}
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CVehicleTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{

	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sVEHICLE_TBLDAT* pItem = (sVEHICLE_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			READ_STRING( bstrData, pItem->szModelName, _countof(pItem->szModelName) );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"SRP_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bySRPType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Speed"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bySpeed = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		//else if (0 == wcscmp(pstrDataName->c_str(), L"Fuel_Efficiency"))
		//{
		//	CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
		//	pItem->byFuelEfficiency = READ_BYTE( bstrData, pstrDataName->c_str() );
		//}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Vehicle_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byVehicleType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Run_Height") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->wRunHeight = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Personnel"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byPersonnel = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		//else if ( 0 == wcscmp(pstrDataName->c_str(), L"Name") )
		//{
		//	CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
		//	pItem->dwName = READ_DWORD( bstrData );
		//}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CVehicleTable::FindData(TBLIDX tblidx)
{
	if ( 0 == tblidx )
	{
		return NULL;
	}

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if ( End() == iter )
	{
		return NULL;
	}

	return (sTBLDAT*)(iter->second); 
}

bool CVehicleTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sVEHICLE_TBLDAT* pTableData = new sVEHICLE_TBLDAT;
		if ( NULL == pTableData )
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		if (false == pTableData->LoadFromBinary(serializer))
		{
			delete pTableData;
			bLoop = false;
			break;
		}

	//	printf("pTableData->tblidx %d \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CVehicleTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sVEHICLE_TBLDAT* pTableData = (sVEHICLE_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}