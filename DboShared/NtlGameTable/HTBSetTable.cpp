#include "StdAfx.h"
#include "NtlDebug.h"

#include "HTBSetTable.h"
#include "NtlCharacter.h"
#include "NtlBattle.h"

#include "NtlSerializer.h"

WCHAR* CHTBSetTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CHTBSetTable::CHTBSetTable(void)
{
	Init();
}

CHTBSetTable::~CHTBSetTable(void)
{
	Destroy();
}

bool CHTBSetTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CHTBSetTable::Destroy()
{
}

void CHTBSetTable::Init()
{
}

void* CHTBSetTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHTB_SET_TBLDAT* pNewHTBSet = new sHTB_SET_TBLDAT;
		if (NULL == pNewHTBSet)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewHTBSet;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewHTBSet;
	}
	
	return NULL;
}

bool CHTBSetTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHTB_SET_TBLDAT* pHTBSet = (sHTB_SET_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHTBSet, sizeof(*pHTBSet)))
			return false;

		delete pHTBSet;

		return true;
	}
	
	return false;
}

bool CHTBSetTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sHTB_SET_TBLDAT* pTbldat = (sHTB_SET_TBLDAT*)pvTable;
		
	if ( false == pTbldat->bValidity_Able )
	{
		return false;
	}

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CHTBSetTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHTB_SET_TBLDAT * pHTBSet = (sHTB_SET_TBLDAT*) pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHTBSet->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pHTBSet->wszNameText, _countof(pHTBSet->wszNameText));
		}		
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			pHTBSet->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"PC_Class_Bit_Flag"))
		{
			pHTBSet->dwPC_Class_Bit_Flag = READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Slot_Index"))
		{
			pHTBSet->bySlot_Index = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Skill_Grade"))
		{
			pHTBSet->bySkill_Grade = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"HTB_Skill_Name"))
		{
			pHTBSet->HTB_Skill_Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Icon_Name"))
		{
			READ_STRING(bstrData, pHTBSet->szIcon_Name, _countof(pHTBSet->szIcon_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_EP"))
		{
			pHTBSet->wNeed_EP = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Train_Level"))
		{
			pHTBSet->byRequire_Train_Level = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_Zenny"))
		{
			pHTBSet->dwRequire_Zenny = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Next_Skill_Train_Exp"))
		{
			pHTBSet->wNext_Skill_Train_Exp = READ_WORD( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cool_Time"))
		{
			pHTBSet->wCool_Time = READ_WORD( bstrData, pstrDataName->c_str() );
			pHTBSet->dwCoolTimeInMilliSecs = pHTBSet->wCool_Time * 1000;
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pHTBSet->Note = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Set_Count"))
		{
			pHTBSet->bySetCount = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Stop_Point"))
		{
			pHTBSet->byStop_Point = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"HTB_Type_", wcslen(L"HTB_Type_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET; i++ )
			{
				swprintf( szBuffer, 1024, L"HTB_Type_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pHTBSet->aHTBAction[ i ].bySkillType = READ_BYTE( bstrData, pstrDataName->c_str() );

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
		else if( 0 == wcsncmp(pstrDataName->c_str(), L"Skill_Tblidx_", wcslen(L"Skill_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET; i++ )
			{
				swprintf( szBuffer, 1024, L"Skill_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pHTBSet->aHTBAction[ i ].skillTblidx = READ_DWORD( bstrData );

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
		else if (0 == wcscmp(pstrDataName->c_str(), L"Require_SP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pHTBSet->wRequireSP = READ_WORD(bstrData, pstrDataName->c_str());
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


sTBLDAT* CHTBSetTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CHTBSetTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sHTB_SET_TBLDAT* pTableData = new sHTB_SET_TBLDAT;
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

bool CHTBSetTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sHTB_SET_TBLDAT* pTableData = (sHTB_SET_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}