#include "StdAfx.h"
#include "PortalTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CPortalTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CPortalTable::CPortalTable(void)
{
	Init();
}

CPortalTable::~CPortalTable(void)
{
	Destroy();
}

bool CPortalTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CPortalTable::Destroy()
{
}

void CPortalTable::Init()
{
}

void* CPortalTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sPORTAL_TBLDAT* pPortal = new sPORTAL_TBLDAT;
		if (NULL == pPortal)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pPortal;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pPortal;
	}

	return NULL;
}

bool CPortalTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sPORTAL_TBLDAT* pPortal = (sPORTAL_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pPortal, sizeof(*pPortal)))
			return false;

		delete pPortal;

		return true;
	}

	return false;
}

bool CPortalTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sPORTAL_TBLDAT* pTbldat = (sPORTAL_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CPortalTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sPORTAL_TBLDAT* pPortal = (sPORTAL_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pPortal->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Point_Name"))
		{
			pPortal->dwPointName = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Point_Name_Text"))
		{
			READ_STRING(bstrData, pPortal->szPointNameText, _countof(pPortal->szPointNameText));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"World"))
		{
			pPortal->worldId = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Grade"))
		{
			pPortal->byGrade = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Loc_X"))
		{
			pPortal->vLoc.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Loc_Y"))
		{
			pPortal->vLoc.y = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Loc_Z"))
		{
			pPortal->vLoc.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dir_X"))
		{
			pPortal->vDir.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Dir_Z"))
		{
			pPortal->vDir.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Map_X"))
		{
			pPortal->vMap.x = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Map_Y"))
		{
			pPortal->vMap.z = READ_FLOAT( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Point_", wcslen(L"Point_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_POINT_PORTAL; i++ )
			{
				swprintf( szBuffer, 1024, L"Point_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pPortal->aPoint[ i ] = READ_BYTE( bstrData, pstrDataName->c_str() );

					bFound = true;
					break;
				}
			}
			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Zeny_", wcslen(L"Zeny_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_POINT_PORTAL; i++ )
			{
				swprintf( szBuffer, 1024, L"Zeny_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pPortal->adwPointZenny[ i ] = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}
			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
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


sTBLDAT* CPortalTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CPortalTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sPORTAL_TBLDAT* pTableData = new sPORTAL_TBLDAT;
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

	//	printf("pTableData->tblidx %d %d\n", pTableData->tblidx, pTableData->dwUnknown);
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CPortalTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sPORTAL_TBLDAT* pTableData = (sPORTAL_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}