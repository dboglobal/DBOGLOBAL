#include "StdAfx.h"
#include "DynamicFieldSystemTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"



WCHAR* CDynamicFieldSystemTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CDynamicFieldSystemTable::CDynamicFieldSystemTable(void)
{
	Init();
}

CDynamicFieldSystemTable::~CDynamicFieldSystemTable(void)
{
	Destroy();
}

bool CDynamicFieldSystemTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CDynamicFieldSystemTable::Destroy()
{
}

void CDynamicFieldSystemTable::Init()
{
	memset(m_abOnOff, false, sizeof(m_abOnOff));
	m_mapDynamicFieldSystem.clear();
}

void* CDynamicFieldSystemTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pNewHelp = new sDYNAMIC_FIELD_SYSTEM_TBLDAT;
		if (NULL == pNewHelp)
			return NULL;

		CPINFO cpInfo;
		if (false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewHelp;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewHelp;
	}

	return NULL;
}

bool CDynamicFieldSystemTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pHelp = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CDynamicFieldSystemTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sDYNAMIC_FIELD_SYSTEM_TBLDAT* pTbldat = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)pvTable;

	if (false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second)
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ", m_wszXmlFileName, pTbldat->tblidx);
		_ASSERTE(0);
		return false;
	}

	if (pTbldat->bOnOff == false)
		return true;

	m_abOnOff[pTbldat->byType] = true;
	m_mapDynamicFieldSystem.insert(DYNAMIC_FIELD_SYSTEM_VAL(pTbldat->byType, pTbldat));

	return true;
}

bool CDynamicFieldSystemTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pHelp = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHelp->tblidx = READ_DWORD(bstrData);
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


sTBLDAT* CDynamicFieldSystemTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second);
}

bool CDynamicFieldSystemTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if (false == bReload)
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pTableData = new sDYNAMIC_FIELD_SYSTEM_TBLDAT;
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

	//	printf("CDynamicFieldSystemTable: pTableData->tblidx %u, byType %u, tIndex %u, aIndex %u, byGroup %u\n", pTableData->tblidx, pTableData->byType, pTableData->tIndex, pTableData->aIndex, pTableData->byGroup);

		if (false == AddTable(pTableData, bReload, bUpdate))
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CDynamicFieldSystemTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin(); End() != iter; iter++)
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pTableData = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}


bool CDynamicFieldSystemTable::IsOn(eEVENT_SYSTEM_TYPE eEventSystemType)
{
	return m_abOnOff[eEventSystemType];
}


DYNAMIC_FIELD_SYSTEM_MAP::iterator CDynamicFieldSystemTable::DynamicFieldBegin()
{
	return m_mapDynamicFieldSystem.begin();
}


DYNAMIC_FIELD_SYSTEM_MAP::iterator CDynamicFieldSystemTable::DynamicFieldEnd()
{
	return m_mapDynamicFieldSystem.end();
}


bool CDynamicFieldSystemTable::IsThisDynamicFieldMob(TBLIDX tblidx)
{
	TABLE::iterator iter;
	for (iter = Begin(); End() != iter; iter++)
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pTableData = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)(iter->second);

		if (pTableData->bOnOff == true && pTableData->tIndex == tblidx)
			return true;
	}

	return false;
}
