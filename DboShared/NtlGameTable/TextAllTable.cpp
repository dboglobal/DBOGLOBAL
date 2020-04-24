#include "stdafx.h"
#include "TextAllTable.h"

#include "NtlXMLDoc.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"
#include "NtlStringHandler.h"

WCHAR* CTextTable::m_pwszSheetList[] = 
{
	L"Table_Data_KOR",
	NULL
};

CTextTable::CTextTable(VOID)
{
	Init();
}

CTextTable::~CTextTable(VOID)
{
	Destroy();
}

BOOL CTextTable::Create(DWORD dwCodePage, INT nField, INT nType )
{
	m_nField = nField;
	m_nType = nType;

	if (false == CTable::Create(dwCodePage))
	{
		return FALSE;
	}

	return TRUE;
}

VOID* CTextTable::AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTEXT_TBLDAT* pNewText = new sTEXT_TBLDAT;
		if ( NULL == pNewText )
			return NULL;

		CPINFO cpInfo;
		if( !GetCPInfo( dwCodePage, &cpInfo ) )
		{
			delete pNewText;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pNewText;
	}

	return NULL;
}

bool CTextTable::DeallocNewTable( VOID* pvTable, WCHAR* pwszSheetName )
{
	if ( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTEXT_TBLDAT* pText = (sTEXT_TBLDAT*)pvTable;
		if ( IsBadReadPtr( pText, sizeof( *pText ) ) )
			return false;

		delete pText;

		return true;
	}

	return false;
}

bool CTextTable::AddTable( VOID* pvTable, bool bReload, bool bUpdate )
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sTEXT_TBLDAT* pTbldat = (sTEXT_TBLDAT*)pvTable;

	if( !m_mapTableList.insert( std::pair<TBLIDX,sTBLDAT*>( pTbldat->tblidx, pTbldat ) ).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CTextTable::SetTableData( VOID* pvTable, WCHAR* pwszSheetName, TYPE eType, BSTR bstrData )
{
	if( 0 == wcscmp( pwszSheetName, L"Table_Data_KOR" ) )
	{
		sTEXT_TBLDAT* pChatCommand = (sTEXT_TBLDAT*)pvTable;

		if( eType == INDEX )
		{
			pChatCommand->tblidx = READ_DWORD( bstrData );
		}
		else if( eType == TEXT )
		{
			if ( false == READ_STR( pChatCommand->wstrText, bstrData) )
			{
				return false;
			}
		}
		else
		{
			_ASSERT( 0 );
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

bool CTextTable::SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData )
{
	pvTable;
	pwszSheetName;
	pstrDataName;
	bstrData;

	return true;
}

bool CTextTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if (false == bReload && bUpdate == false)
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sTEXT_TBLDAT* pTableData = new sTEXT_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		WORD wTextLength = 0;
		if (serializer.GetDataSize() < sizeof(pTableData->tblidx) + sizeof(wTextLength))
		{
			//- yoshiki : To log system!

			delete pTableData;
			Destroy();
			return false;
		}
		serializer >> pTableData->tblidx;
		serializer >> wTextLength;

		if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
		{
			//- yoshiki : To log system!
			delete pTableData;

			Destroy();
			return false;
		}

		WCHAR* pwszText = new WCHAR[wTextLength + 1];
		if (NULL == pwszText)
		{
			//- yoshiki : To log system!
			delete pTableData;

			Destroy();
			return false;
		}
		serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
		pwszText[wTextLength] = L'\0';

		pTableData->wstrText = pwszText;

		delete [] pwszText;

		//  [4/26/2008 zeroera] : 설명 : 실패하더라도 Load의 종료여부는 File Loading에서 결정한다
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	};

	return true;
}

bool CTextTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sTEXT_TBLDAT* pTableData = (sTEXT_TBLDAT*)(iter->second);

		serializer << pTableData->tblidx;
		serializer << (WORD)((pTableData->wstrText).size());
		serializer.In((pTableData->wstrText).c_str(), (int)((pTableData->wstrText).size() * sizeof(WCHAR)));
	}

	return true;
}

bool CTextTable::InitializeFromXmlDoc(CNtlXMLDoc* pXmlDoc, WCHAR* pwszFileName, bool bReload, bool bUpdate)
{
	if( NULL == pXmlDoc )
	{
		return false;
	}

	BSTR rowXPathFormat;
	rowXPathFormat = ::SysAllocString( L"/dataroot/%s/F%d" );
	WCHAR wszXPath[CNtlXMLDoc::MAX_UNICODE_XPATH_LENGTH+1];

	DWORD dwSheetIndex = 0;
	WCHAR** ppwszSheetList = GetSheetListInWChar();

	BSTR bstr = NULL;

	while( ppwszSheetList[dwSheetIndex] )
	{
		swprintf_s<_countof(wszXPath)>( wszXPath, rowXPathFormat, ppwszSheetList[dwSheetIndex], m_nField );

		IXMLDOMNodeList* pIndexNodeList = NULL;
		pIndexNodeList = pXmlDoc->SelectNodeList( wszXPath );
		if( !pIndexNodeList )
		{
			::SysFreeString( rowXPathFormat );

			return false;
		}

		LONG nIndexLength = 0;
		if( FAILED( pIndexNodeList->get_length( &nIndexLength ) ) )
		{
			pIndexNodeList->Release();
			::SysFreeString( rowXPathFormat );

			return false;
		}

		swprintf_s<_countof(wszXPath)>( wszXPath, rowXPathFormat, ppwszSheetList[dwSheetIndex], m_nField + 1 );

		IXMLDOMNodeList* pTextNodeList = NULL;
		pTextNodeList = pXmlDoc->SelectNodeList( wszXPath );
		if( !pTextNodeList )
		{
			pIndexNodeList->Release();
			::SysFreeString( rowXPathFormat );

			return false;
		}

		LONG nTextLength = 0;
		if( FAILED( pTextNodeList->get_length( &nTextLength ) ) )
		{
			pIndexNodeList->Release();
			pTextNodeList->Release();
			::SysFreeString( rowXPathFormat );

			return false;
		}

		if( nTextLength != nIndexLength )
		{
			pIndexNodeList->Release();
			pTextNodeList->Release();
			::SysFreeString( rowXPathFormat );

			return false;
		}

		// 제목은 버린다.
		for( INT j = 0 ; j < nIndexLength ; ++j )
		{
			VOID* pvTable = AllocNewTable( ppwszSheetList[dwSheetIndex], m_dwCodePage );
			if ( !pvTable )
			{
				pIndexNodeList->Release();
				pTextNodeList->Release();
				::SysFreeString( rowXPathFormat );

				return false;				
			}	

			IXMLDOMNode* pCellNode = NULL;
			IXMLDOMNode* pDataNode = NULL;

			if( FAILED( pIndexNodeList->get_item( j, &pCellNode ) ) )
			{
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString(rowXPathFormat);

				return false;
			}
			
			if( !pCellNode )
			{
				//DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				//break;
				bstr = L"";
				SetTableData(pvTable, ppwszSheetList[dwSheetIndex], INDEX, bstr);
				
				continue;
			}

			if( FAILED( pCellNode->get_firstChild( &pDataNode ) ) )
			{
				pCellNode->Release();
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString( rowXPathFormat );

				return false;
			}

			if ( !pDataNode )
			{
				//pCellNode->Release();
				//DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				//break;
				bstr = L"";
				SetTableData(pvTable, ppwszSheetList[dwSheetIndex], INDEX, bstr);
				
				continue;
			}

			if ( FAILED( pDataNode->get_text( &bstr ) ) )
			{
				::SysFreeString(bstr);
				pDataNode->Release();
				pCellNode->Release();
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString( rowXPathFormat );

				return false;
			}

			SetTableData( pvTable, ppwszSheetList[dwSheetIndex], INDEX, bstr );

			::SysFreeString(bstr);
			pCellNode->Release();
			pDataNode->Release();

			pCellNode = NULL;
			pDataNode = NULL;

			if( FAILED( pTextNodeList->get_item( j, &pCellNode ) ) )
			{
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString(rowXPathFormat);

				return false;
			}

			if( !pCellNode )
			{
				//DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				//break;

				bstr = L"";
				SetTableData(pvTable, ppwszSheetList[dwSheetIndex], INDEX, bstr);
				
				continue;
			}

			if( FAILED( pCellNode->get_firstChild( &pDataNode ) ) )
			{
				//- yoshiki : TODO!(Get the field name)
				CTable::CallErrorCallbackFunction("[File] : %S\n[Error] : Table data is null.(Row Index : %d, Field Name = %S)", pwszFileName, j - 1, L"FIELD_NAME!");

				pCellNode->Release();
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString( rowXPathFormat );

				return false;
			}

			if ( !pDataNode )
			{
				#if 0
				//- yoshiki : TODO!(Get the field name)
				CTable::CallErrorCallbackFunction("[File] : %S\n[Error] : Table data is null.(Row Index : %d, Field Name = %S)", pwszFileName, j - 1, L"FIELD_NAME!");

				pCellNode->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				_ASSERT( 0 );		// Index는 있고 Data는 공란인경우.
				break;
				#endif

				bstr = L"";
				SetTableData(pvTable, ppwszSheetList[dwSheetIndex], INDEX, bstr);
				
				continue;
			}

			if ( FAILED( pDataNode->get_text( &bstr ) ) )
			{
				::SysFreeString(bstr);
				pDataNode->Release();
				pCellNode->Release();
				pIndexNodeList->Release();
				pTextNodeList->Release();
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
				::SysFreeString( rowXPathFormat );

				return false;
			}

			SetTableData( pvTable, ppwszSheetList[dwSheetIndex], TEXT, bstr );

			::SysFreeString(bstr);
			pCellNode->Release();
			pDataNode->Release();

			if( !AddTable( pvTable, bReload, bUpdate ) )
			{
				DeallocNewTable( pvTable, ppwszSheetList[dwSheetIndex] );
			}
		}

		pIndexNodeList->Release();
		pTextNodeList->Release();
		++dwSheetIndex;
	}

	::SysFreeString( rowXPathFormat );

	return true;
}

VOID CTextTable::Destroy(VOID)
{
	m_nField = -1;
	m_nType = -1;
}

VOID CTextTable::Init(VOID)
{
	m_nField = -1;
	m_nType = -1;
}

sTBLDAT* CTextTable::FindData( TBLIDX tblidx )
{
	if( !tblidx )
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find( tblidx );
	if( End() == iter )
		return NULL;

	return (sTBLDAT*)( iter->second );
}

std::wstring& CTextTable::GetText( TBLIDX tblidx )
{
	static std::wstring wstrBuf;

	GetText( tblidx, &wstrBuf );
	
	return wstrBuf;
}

BOOL CTextTable::GetText( TBLIDX tblidx, std::wstring* pwstr )
{
	sTEXT_TBLDAT* pData = reinterpret_cast<sTEXT_TBLDAT*>( FindData( tblidx ) );

	// WCHAR buf[256];

	if( !pData )
	{
		//  swprintf_s( buf, 256, L"%d Tbl %u Index is Invalid", m_nType, tblidx );
		//  *pwstr = buf;
		*pwstr = L" ";

		return FALSE;
	}
	else if( !wcscmp( pData->wstrText.c_str(), L"" ) )
	{
		//  swprintf_s( buf, 256, L"%d Tbl %u Index is Invalid", m_nType, tblidx );
		//  *pwstr = buf;
		*pwstr = L" ";

		return FALSE;
	}

	*pwstr = pData->wstrText;

	return TRUE;
}

BOOL CMapNameTextTable::GetZoneName( TBLIDX tblidx, std::wstring* pwstr )
{
	GetText( tblidx, pwstr );
	std::wstring::size_type pos;

	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		GetErrorText( tblidx, pwstr );
		return FALSE;
	}
	pwstr->erase( pos );

	return TRUE;
}

std::wstring CMapNameTextTable::GetZoneName( TBLIDX tblidx )
{
	std::wstring wstrText;

	GetZoneName( tblidx, &wstrText );

	return wstrText;
}

BOOL CMapNameTextTable::GetSectorName( TBLIDX tblidx, std::wstring* pwstr )
{
	GetText( tblidx, pwstr );

	std::wstring::size_type pos;
	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		GetErrorText( tblidx, pwstr );
		return FALSE;
	}
	pwstr->erase( 0, pos + 1 );

	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		GetErrorText( tblidx, pwstr );
		return FALSE;
	}
	pwstr->erase( pos );

	return TRUE;
}

std::wstring CMapNameTextTable::GetSectorName( TBLIDX tblidx )
{
	std::wstring wstrText;
	
	GetSectorName( tblidx, &wstrText );

	return wstrText;
}

BOOL CMapNameTextTable::GetAreaName( TBLIDX tblidx, std::wstring* pwstr )
{
	GetText( tblidx, pwstr );
	std::wstring::size_type pos;

	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		GetErrorText( tblidx, pwstr );
		return FALSE;
	}
	pwstr->erase( 0, pos + 1 );

	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		GetErrorText( tblidx, pwstr );
		return FALSE;
	}
	pwstr->erase( 0, pos + 1 );

	pos = pwstr->find( L";" );
	if( pos == std::wstring::npos )
	{
		*pwstr = L"Last Semicolon";
		return FALSE;	
	}
	pwstr->erase( pos );

	return TRUE;
}

std::wstring CMapNameTextTable::GetAreaName( TBLIDX tblidx )
{
	std::wstring wstrText;

	GetAreaName( tblidx, &wstrText );

	return wstrText;
}

VOID CMapNameTextTable::GetErrorText( TBLIDX tblidx, std::wstring* pwString )
{
	WCHAR buf[256];
	swprintf_s( buf, 256, L"TextTable::MapName Error : %u Index is Wrong", tblidx );
	(*pwString) = buf;
}

CTextAllTable::CTextAllTable(VOID)
{
	ZeroMemory( m_pTextTable, sizeof( m_pTextTable ) );

	m_dwCodePage = 0;
}

CTextAllTable::~CTextAllTable(VOID)
{
	Destroy();
}

BOOL CTextAllTable::Create( DWORD dwCodePage )
{
	m_dwCodePage = dwCodePage;

	return TRUE;
}

VOID CTextAllTable::Destroy(VOID)
{
	Reset();

	m_eTableLoadType = TABLE_LOAD_TYPE_NORMAL;

	m_dwCodePage = 0;
}

INT CTextAllTable::TypeToField( INT eType )
{
	return 2 * eType + 1;
}

void CTextAllTable::Reset()
{
	for (int nTableType = 0 ; nTableType < TABLE_COUNT ; nTableType++)
	{
		if (NULL != m_pTextTable[nTableType])
		{
			delete m_pTextTable[nTableType];
			m_pTextTable[nTableType] = NULL;
		}
	}
}

bool CTextAllTable::LoadFromXml(char* pszFileName)
{
	WCHAR* pwszFileName = Ntl_MB2WC(pszFileName);
	if (NULL == pwszFileName)
	{
		return false;
	}

	bool bResult = LoadFromXml(pwszFileName);

	Ntl_CleanUpHeapStringW(pwszFileName);

	return bResult;
}

bool CTextAllTable::LoadFromXml(WCHAR* pwszFileName)
{
	Reset();

	for (int nTableType = 0 ; nTableType < TABLE_COUNT ; nTableType++)
	{
		CTextTable* pTable = new CTextTable;
		if (NULL == pTable)
		{
			//- yoshiki : To log system!

			Destroy();
			return false;
		}

		if (FALSE == pTable->Create(m_dwCodePage, TypeToField(nTableType), nTableType))
		{
			//- yoshiki : To log system!

			delete pTable;

			Destroy();
			return false;
		}

		if ( FALSE == pTable->LoadFromXml( pwszFileName, false, false ) )
		{
			//- yoshiki : To log system!

			delete pTable;

			Destroy();
			return false;
		}

		m_pTextTable[nTableType] = pTable;
	}

	return true;
}

bool CTextAllTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if (false == bReload && bUpdate == false)
	{
		Reset();
	}
	
	//printf("serializer.GetDataSize() %u \n", serializer.GetDataSize());
	while (0 < serializer.GetDataSize())
	{
		int nTableType = 0;
		int nPayloadLength = 0;
		if (serializer.GetDataSize() < sizeof(nTableType) + sizeof(nPayloadLength))
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}
		serializer >> nTableType;
		serializer >> nPayloadLength;

		if (TABLE_COUNT <= nTableType)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		CTextTable* pTable = NULL;
		if (GetTableLoadType() == TABLE_LOAD_TYPE_OVERRIDING)
		{
			pTable = GetTextTbl((CTextAllTable::TABLETYPE)nTableType);
			if (!pTable)
			{
				Destroy();
				return false;
			}

			SetTableLoadType(TABLE_LOAD_TYPE_OVERRIDING);
		}
		else
		{
			pTable = new CTextTable;
			if (NULL == pTable)
			{
				//- yoshiki : To log system!
				Destroy();
				return false;
			}

			if (FALSE == pTable->Create(m_dwCodePage, TypeToField(nTableType), nTableType))
			{
				//- yoshiki : To log system!
				delete pTable;

				Destroy();
				return false;
			}
		}

		CNtlSerializer partialSerializer;
		serializer.Out(partialSerializer, nPayloadLength);

		if ( false == pTable->LoadFromBinary(partialSerializer, bReload, bUpdate ) )
		{
			//- yoshiki : To log system!
			printf("if ( false == pTable->LoadFromBinary(partialSerializer, false, false ) ) \n");

			delete pTable;

			Destroy();
			return false;
		}

		m_pTextTable[nTableType] = pTable;

	};

	return true;
}

bool CTextAllTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	CNtlSerializer partialSerializer;

	for (int nIndex = 0 ; nIndex < TABLE_COUNT ; nIndex++)
	{
		if (NULL != m_pTextTable[nIndex])
		{
			int nTableType = nIndex;

			m_pTextTable[nIndex]->SaveToBinary(partialSerializer);

			serializer << nTableType;
			serializer << partialSerializer.GetDataSize();

			serializer.In(partialSerializer.GetData(), partialSerializer.GetDataSize());
		}
	}

	return true;
}