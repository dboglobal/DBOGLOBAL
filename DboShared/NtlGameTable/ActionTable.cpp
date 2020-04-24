//***********************************************************************************
//
//	File		:	ActionTable.cpp
//
//	Begin		:	2006-08-29
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlDebug.h"

#include "ActionTable.h"

#include "NtlSerializer.h"

WCHAR* CActionTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CActionTable::CActionTable(void)
{
	Init();
}

CActionTable::~CActionTable(void)
{
	Destroy();
}

bool CActionTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CActionTable::Destroy()
{
}

void CActionTable::Init()
{
}

void* CActionTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sACTION_TBLDAT* pNewAction = new sACTION_TBLDAT;
		if (NULL == pNewAction)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewAction;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewAction;
	}

	return NULL;
}

bool CActionTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sACTION_TBLDAT* pAction = (sACTION_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pAction, sizeof(*pAction)))
			return false;

		delete pAction;

		return true;
	}

	return false;
}

bool CActionTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sACTION_TBLDAT* pTbldat = (sACTION_TBLDAT*)pvTable;
		
	if ( false == pTbldat->bValidity_Able)
	{
		return false;
	}

	if( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CActionTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sACTION_TBLDAT* pAction = (sACTION_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pAction->tblidx = READ_DWORD(bstrData);
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			pAction->bValidity_Able = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_Type"))
		{
			pAction->byAction_Type = READ_BYTE(bstrData, pstrDataName->c_str());

			if (ACTION_TYPE_FIRST > pAction->byAction_Type || ACTION_TYPE_LAST < pAction->byAction_Type)
			{
				_ASSERT(0);
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Action_Name"))
		{
			pAction->Action_Name = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Icon_Name"))
		{
			READ_STRING(bstrData, pAction->szIcon_Name, _countof(pAction->szIcon_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pAction->Note = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Chat_Command_Index"))
		{
			pAction->chat_Command_Index = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"ETC_Action_Type"))
		{
			pAction->byETC_Action_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}		
		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		_ASSERT(0);
		return false;
	}

	return true;
}


sTBLDAT* CActionTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CActionTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sACTION_TBLDAT* pTableData = new sACTION_TBLDAT;
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

bool CActionTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sACTION_TBLDAT* pTableData = (sACTION_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}