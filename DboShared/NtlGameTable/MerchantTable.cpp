#include "StdAfx.h"
#include "MerchantTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CMerchantTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CMerchantTable::CMerchantTable(void)
{
	Init();
}

CMerchantTable::~CMerchantTable(void)
{
	Destroy();
}

bool CMerchantTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CMerchantTable::Destroy()
{
}

void CMerchantTable::Init()
{
}

void* CMerchantTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMERCHANT_TBLDAT* pMerchant = new sMERCHANT_TBLDAT;
		if (NULL == pMerchant)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pMerchant;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pMerchant;
	}

	return NULL;
}

bool CMerchantTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMERCHANT_TBLDAT* pMerchant = (sMERCHANT_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pMerchant, sizeof(*pMerchant)))
			return false;

		delete pMerchant;

		return true;
	}

	return false;
}

bool CMerchantTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	sMERCHANT_TBLDAT * pTbldat = (sMERCHANT_TBLDAT*)pvTable;
	sMERCHANT_TBLDAT * pExistTbldat = NULL;

	if( bReload )
	{
		pExistTbldat = (sMERCHANT_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
			return true; 
		}
	}

	if( bUpdate) //for tables inside localize folder
	{
		pExistTbldat = (sMERCHANT_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{ //if exist then remove
			m_mapTableList.erase( pTbldat->tblidx );
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

bool CMerchantTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sMERCHANT_TBLDAT* pMerchant = (sMERCHANT_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pMerchant->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pMerchant->wszNameText, _countof(pMerchant->wszNameText));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sell_Type"))
		{
			pMerchant->bySell_Type = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Tab_Name"))
		{
			pMerchant->Tab_Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Mileage"))
		{
			pMerchant->dwNeedMileage = READ_DWORD( bstrData );
		}		
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Item_Tblidx_", wcslen(L"Item_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Item_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pMerchant->aitem_Tblidx[ i ] = READ_DWORD( bstrData );

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

		//new
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Need_Item_Tindex_", wcslen(L"Need_Item_Tindex_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Need_Item_Tindex_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pMerchant->aNeedItemTblidx[ i ] = READ_DWORD( bstrData );

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

		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Need_Item_Stack_", wcslen(L"Need_Item_Stack_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Need_Item_Stack_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pMerchant->abyNeedItemStack[ i ] = READ_BYTE( bstrData, L"Need_Item_Stack" );

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

		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Need_Zenny_", wcslen(L"Need_Zenny_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++ )
			{
				swprintf( szBuffer, 1024, L"Need_Zenny_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pMerchant->adwNeedZenny[ i ] = READ_DWORD( bstrData );

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


sTBLDAT* CMerchantTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

TBLIDX CMerchantTable::FindMerchantItem(sMERCHANT_TBLDAT* psTbldat, BYTE byIndex)
{
	if ( NTL_MAX_MERCHANT_COUNT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTbldat->aitem_Tblidx[byIndex];	
}

bool CMerchantTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload && bUpdate == false )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sMERCHANT_TBLDAT* pTableData = new sMERCHANT_TBLDAT;
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

bool CMerchantTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sMERCHANT_TBLDAT* pTableData = (sMERCHANT_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}