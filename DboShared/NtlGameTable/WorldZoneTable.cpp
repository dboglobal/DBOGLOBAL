//***********************************************************************************
//
//	File		:	WorldZoneTable.cpp
//
//	Begin		:	2007-08-14
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "WorldZoneTable.h"

#include "NtlSerializer.h"

WCHAR* CWorldZoneTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CWorldZoneTable::CWorldZoneTable(void)
{
	Init();
}

CWorldZoneTable::~CWorldZoneTable(void)
{
	Destroy();
}

bool CWorldZoneTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CWorldZoneTable::Destroy()
{
}

void CWorldZoneTable::Init()
{
}

void* CWorldZoneTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_ZONE_TBLDAT* pNewWorldZone = new sWORLD_ZONE_TBLDAT;
		if (NULL == pNewWorldZone)
		{
			return NULL;
		}

		CPINFO cpInfo;
		if (false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewWorldZone;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewWorldZone;
	}
	
	return NULL;
}

bool CWorldZoneTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_ZONE_TBLDAT* pWorldZone = (sWORLD_ZONE_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pWorldZone, sizeof(*pWorldZone)))
		{
			return false;
		}

		delete pWorldZone;

		return true;
	}
	
	return false;
}

bool CWorldZoneTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sWORLD_ZONE_TBLDAT* pTbldat = (sWORLD_ZONE_TBLDAT*)pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CWorldZoneTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_ZONE_TBLDAT* pWorldZone = (sWORLD_ZONE_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorldZone->tblidx = READ_DWORD( bstrData );
		}		
		else if ( 0 == wcscmp( pstrDataName->c_str(), L"Function_Bit_Flag" ) )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorldZone->wFunctionBitFlag = (WORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"World"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorldZone->worldTblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorldZone->nameTblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRINGW(bstrData, pWorldZone->wszName_Text, _countof(pWorldZone->wszName_Text));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Forbidden_Vehicle"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorldZone->bForbidden_Vehicle = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CWorldZoneTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
	{
		return NULL;
	}

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CWorldZoneTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sWORLD_ZONE_TBLDAT* pTableData = new sWORLD_ZONE_TBLDAT;
		if (NULL == pTableData)
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

bool CWorldZoneTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sWORLD_ZONE_TBLDAT* pTableData = (sWORLD_ZONE_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}