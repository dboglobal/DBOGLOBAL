#include "StdAfx.h"
#include "DwcTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CDwcTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CDwcTable::CDwcTable(void)
{
	Init();
}

CDwcTable::~CDwcTable(void)
{
	Destroy();
}

bool CDwcTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CDwcTable::Destroy()
{
}

void CDwcTable::Init()
{
}

void* CDwcTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDWC_TBLDAT* pNewHelp = new sDWC_TBLDAT;
		if (NULL == pNewHelp)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewHelp;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewHelp;
	}

	return NULL;
}

bool CDwcTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDWC_TBLDAT* pHelp = (sDWC_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CDwcTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sDWC_TBLDAT* pTbldat = (sDWC_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CDwcTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDWC_TBLDAT* pHelp = (sDWC_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHelp->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			pHelp->tblNameIndex = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Level_Min"))
		{
			pHelp->byLevel_Min = READ_BYTE( bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Level_Max"))
		{
			pHelp->byLevel_Max = READ_BYTE( bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Admission_Bit_Flag"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pHelp->wAdmission_Bit_Flag = (WORD)READ_BITFLAG( bstrData, 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Admission_Num_Min"))
		{
			pHelp->byAdmission_Num_Min = READ_BYTE( bstrData, pstrDataName->c_str(), 1);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Admission_Num_Max"))
		{
			pHelp->byAdmission_Num_Max = READ_BYTE( bstrData, pstrDataName->c_str());
		}

		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Condition_Tblidx", wcslen(L"Condition_Tblidx") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < MAX_DWC_ADMISSION_CONDITION_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Condition_Tblidx%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pHelp->aConditionTblidx[ i ] = READ_DWORD( bstrData );

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

		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Mission_Tblidx", wcslen(L"Mission_Tblidx") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; i++ )
			{
				swprintf( szBuffer, 1024, L"Mission_Tblidx%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pHelp->aMissionTblidx[ i ] = READ_DWORD( bstrData );

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

		else if (0 == wcscmp(pstrDataName->c_str(), L"Prologue_Cinematic_Tblidx"))
		{
			pHelp->prologueCinematicTblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Prologue_Text"))
		{
			pHelp->prologueTblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"World_Tblidx"))
		{
			pHelp->worldTblidx = READ_DWORD( bstrData );
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


sTBLDAT* CDwcTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CDwcTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sDWC_TBLDAT* pTableData = new sDWC_TBLDAT;
		if (NULL == pTableData)
		{
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

bool CDwcTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sDWC_TBLDAT* pTableData = (sDWC_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}