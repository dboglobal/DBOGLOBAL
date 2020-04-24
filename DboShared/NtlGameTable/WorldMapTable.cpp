#include "StdAfx.h"
#include "WorldMapTable.h"
#include "NtlDebug.h"

#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CWorldMapTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CWorldMapTable::CWorldMapTable(void)
{
	Init();
}

CWorldMapTable::~CWorldMapTable(void)
{
	Destroy();
}

bool CWorldMapTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CWorldMapTable::Destroy()
{
}

void CWorldMapTable::Init()
{
}

void* CWorldMapTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_MAP_TBLDAT* pWorld = new sWORLD_MAP_TBLDAT;
		if (NULL == pWorld)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pWorld;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pWorld;
	}

	return NULL;
}

bool CWorldMapTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_MAP_TBLDAT* pWorld = (sWORLD_MAP_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pWorld, sizeof(*pWorld)))
			return false;

		delete pWorld;

		return true;
	}

	return false;
}

bool CWorldMapTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sWORLD_MAP_TBLDAT* pTbldat = (sWORLD_MAP_TBLDAT*)pvTable;
	sWORLD_MAP_TBLDAT * pExistTbldat = NULL;

	if( bReload )
	{
		pExistTbldat = (sWORLD_MAP_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
		}
	}
	
	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CWorldMapTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sWORLD_MAP_TBLDAT* pWorld = (sWORLD_MAP_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"World_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->World_Tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Zone_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->Zone_Tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Worldmap_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->Worldmap_Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			READ_STRINGW(bstrData, pWorld->wszNameText, _countof(pWorld->wszNameText));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Validity_Able"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->bValidityAble = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Map_Type"))
		{
			pWorld->byMapType = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Standard_Loc_X"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->vStandardLoc.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Standard_Loc_Z"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->vStandardLoc.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Worldmap_Scale"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->fWorldmapScale = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Link_Map_Idx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->dwLinkMapIdx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Combobox_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pWorld->dwComboBoxType = READ_DWORD( bstrData );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Warfog_", wcslen(L"Warfog_") ) )
		{
			bool bFound = false;
			WCHAR szBuffer[1024] = { 0x00, };

			for( int i = 0; i < DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG; i++ )
			{
				swprintf( szBuffer, 1024, L"Warfog_%d", i + 1 );

				if( 0 == wcscmp( pstrDataName->c_str(), szBuffer ) )
				{
					pWorld->wWarfog[i] = READ_WORD(bstrData, pstrDataName->c_str());
					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}


		else if (0 == wcscmp(pstrDataName->c_str(), L"Recomm_Min_Level"))
		{
			pWorld->byRecomm_Min_Level = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Recomm_Max_Level"))
		{
			pWorld->byRecomm_Max_Level = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}

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


sTBLDAT* CWorldMapTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CWorldMapTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sWORLD_MAP_TBLDAT* pTableData = new sWORLD_MAP_TBLDAT;
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

	//	printf("pTableData->tblidx %u \n", pTableData->tblidx);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CWorldMapTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sWORLD_MAP_TBLDAT* pTableData = (sWORLD_MAP_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}