#include "StdAfx.h"
#include "ExpTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CExpTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CExpTable::CExpTable(void)
{
	Init();
}

CExpTable::~CExpTable(void)
{
	Destroy();
}

bool CExpTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CExpTable::Destroy()
{
}

void CExpTable::Init()
{
}

void* CExpTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sEXP_TBLDAT* pNewExp = new sEXP_TBLDAT;
		if (NULL == pNewExp)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewExp;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewExp;
	}

	return NULL;
}

bool CExpTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sEXP_TBLDAT* pExp = (sEXP_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pExp, sizeof(*pExp)))
			return false;

		delete pExp;

		return true;
	}

	return false;
}



bool CExpTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sEXP_TBLDAT * pTbldat = (sEXP_TBLDAT*) pvTable;
	sEXP_TBLDAT * pExistTbldat = NULL;

	// Reload인경우 Data를 찾아 Update해준다
	if( bReload )
	{
		pExistTbldat = (sEXP_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
		}
	}
	
	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		printf("[File] : %ws\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}
	
	return true;
}

bool CExpTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{

	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sEXP_TBLDAT* pExp = (sEXP_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Level"))
		{
			pExp->tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"EXP"))
		{
			pExp->dwExp = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_EXP"))
		{
			pExp->dwNeed_Exp = READ_DWORD(bstrData);
		}
		//new
		else if (0 == wcscmp(pstrDataName->c_str(), L"Normal_Race"))
		{
			pExp->wNormal_Race = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Super_Race"))
		{
			pExp->wSuperRace = READ_WORD(bstrData, pstrDataName->c_str());
		}

		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_Exp"))
		{
			pExp->dwMobExp = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Phy_Defence_Ref"))
		{
			pExp->dwPhyDefenceRef = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Eng_Defence_Ref"))
		{
			pExp->dwEngDefenceRef = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Mob_Zenny"))
		{
			pExp->dwMobZenny = READ_DWORD( bstrData );
		}
		else
		{
			printf("[File] : %ws\n[Error] : Unknown field name found!(Field Name = %ls)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CExpTable::FindData(TBLIDX tblidx)
{
	if (tblidx == 0)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (iter == End())
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CExpTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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

		sEXP_TBLDAT* pTableData = new sEXP_TBLDAT;

		if (pTableData == NULL)
		{
			printf("sEXP_TBLDAT* pTableData = new sEXP_TBLDAT is empty - destroy! \n");
			Destroy();
			return false;
		}

		/*This function adds data into pTableData*/
		if (pTableData->LoadFromBinary(serializer) == false)
		{
			delete pTableData;
			bLoop = false;
			break;
		}

	//	printf("EXP TABLE: pTableData->tblidx %d \n", pTableData->tblidx);
		if( AddTable(pTableData, bReload, bUpdate) == false)
			delete pTableData;


	} while (bLoop != false);

	return true;
}

bool CExpTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sEXP_TBLDAT* pTableData = (sEXP_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}