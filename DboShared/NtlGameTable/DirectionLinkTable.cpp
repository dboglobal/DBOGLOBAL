//***********************************************************************************
//
//	File		:	DirectionLinkTable.cpp
//
//	Begin		:	2007-06-01
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "DirectionLinkTable.h"

#include "NtlDebug.h"
#include "NtlSerializer.h"
#include "NtlBitFlag.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
WCHAR* CDirectionLinkTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CDirectionLinkTable::CDirectionLinkTable( void )
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CDirectionLinkTable::~CDirectionLinkTable( void )
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CDirectionLinkTable::Init( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::Create(DWORD dwCodePage)
{
	if( false == CTable::Create(dwCodePage) )
	{
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CDirectionLinkTable::Destroy( void )
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CDirectionLinkTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDIRECTION_LINK_TBLDAT* pNewObj = new sDIRECTION_LINK_TBLDAT;
		if ( NULL == pNewObj )
		{
			return NULL;
		}

		CPINFO cpInfo;
		if ( !GetCPInfo( dwCodePage, &cpInfo ) )
		{
			delete pNewObj;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewObj;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::DeallocNewTable( void* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDIRECTION_LINK_TBLDAT* pObj = (sDIRECTION_LINK_TBLDAT*)pvTable;
		if ( IsBadReadPtr( pObj, sizeof(*pObj) ) ) return false;

		delete pObj;

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bUpdate);

	sDIRECTION_LINK_TBLDAT * pTbldat = (sDIRECTION_LINK_TBLDAT*)pvTable;
	sDIRECTION_LINK_TBLDAT * pExistTbldat = NULL;

	if( bReload )
	{
		pExistTbldat = (sDIRECTION_LINK_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// return false for release reloaded table data
			return true; 
		}
	}

	if( false == CheckData( pTbldat) )
	{
		_ASSERTE( 0 );
		return false;
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	static char szTemp[1024] = { 0x00, };

	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sDIRECTION_LINK_TBLDAT * pTbldat = (sDIRECTION_LINK_TBLDAT*) pvTable;

		if ( 0 == wcscmp( pstrDataName->data(), L"Tblidx" ) )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pTbldat->tblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->data(), L"Function_Name"))
		{
			READ_STRING( bstrData, pTbldat->szFunctionName, _countof(pTbldat->szFunctionName) );			
		}
		else if (0 == wcscmp(pstrDataName->data(), L"Note"))
		{
			READ_STRING( bstrData, pTbldat->szNote, _countof(pTbldat->szNote) );
		}
		else if ( 0 == wcscmp( pstrDataName->data(), L"Type" ) )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pTbldat->byType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcscmp( pstrDataName->data(), L"Animation_ID" ) )
		{
			pTbldat->dwAnimationID = READ_DWORD( bstrData );
		}
		else if ( 0 == wcscmp( pstrDataName->data(), L"Direction_Func_Flag" ) )
		{
			pTbldat->byFuncFlag = (BYTE) READ_BITFLAG( bstrData );
		}
		else
		{
			_ASSERT( 0 );
			return false;
		}
	}
	else
	{
		_ASSERT( 0 );
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sTBLDAT* CDirectionLinkTable::FindData( TBLIDX tblidx )
{
	TABLEIT iter = m_mapTableList.find( tblidx );
	if ( m_mapTableList.end() == iter )
	{
		return NULL;
	}

	return (sTBLDAT*)(iter->second); 
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
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
		sDIRECTION_LINK_TBLDAT* pTableData = new sDIRECTION_LINK_TBLDAT;
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sDIRECTION_LINK_TBLDAT* pTableData = (sDIRECTION_LINK_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CDirectionLinkTable::CheckData(sDIRECTION_LINK_TBLDAT * pTbldat)
{
	if( BIT_FLAG_TEST( pTbldat->byFuncFlag, DIRECTION_FUNC_FLAG_TIMEOUT ) &&
		BIT_FLAG_TEST( pTbldat->byFuncFlag, DIRECTION_FUNC_FLAG_KEEPUP ) )
	{
		_ASSERT( 0 );
		CTable::CallErrorCallbackFunction( L"file[%s] index[%u] : The value error.( byFuncFlag can't set DIRECTION_FUNC_FLAG_TIMEOUT and DIRECTION_FUNC_FLAG_KEEPUP simultaneously)", m_wszXmlFileName, pTbldat->tblidx );
		return false;
	}

	return true;
}