#include "StdAfx.h"
#include "CommonConfigTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"


// DWORD
#define COMMONCONFIG_TBLDAT_SET_DWORD( table_loc, valuename, maxvalue)								\
	if( false == ReadDWORD( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}

WCHAR* CCommonConfigTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CCommonConfigTable::CCommonConfigTable(void)
{
	Init();
}

CCommonConfigTable::~CCommonConfigTable(void)
{
	Destroy();
}

bool CCommonConfigTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CCommonConfigTable::Destroy()
{
}

void CCommonConfigTable::Init()
{
	m_mapCommonConfigValueList.clear();
}

void* CCommonConfigTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sCOMMONCONFIG_TBLDAT* pNewHelp = new sCOMMONCONFIG_TBLDAT;
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

bool CCommonConfigTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sCOMMONCONFIG_TBLDAT* pHelp = (sCOMMONCONFIG_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CCommonConfigTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sCOMMONCONFIG_TBLDAT* pTbldat = (sCOMMONCONFIG_TBLDAT*) pvTable;


	//map
	sCOMMONCONFIG_VALUE_DATA* data = new sCOMMONCONFIG_VALUE_DATA;
	COMMONCONFIG_TBLDAT_SET_DWORD(0, data->adwValue[0], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(1, data->adwValue[1], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(2, data->adwValue[2], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(3, data->adwValue[3], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(4, data->adwValue[4], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(5, data->adwValue[5], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(6, data->adwValue[6], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(7, data->adwValue[7], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(8, data->adwValue[8], INVALID_DWORD)
	COMMONCONFIG_TBLDAT_SET_DWORD(9, data->adwValue[9], INVALID_DWORD)
	m_mapCommonConfigValueList[pTbldat->tblidx] = data;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CCommonConfigTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sCOMMONCONFIG_TBLDAT* pHelp = (sCOMMONCONFIG_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHelp->tblidx = READ_DWORD( bstrData );
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


sTBLDAT* CCommonConfigTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

sCOMMONCONFIG_VALUE_DATA* CCommonConfigTable::FindCommonConfig(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	std::map<TBLIDX, sCOMMONCONFIG_VALUE_DATA*>::iterator iter;

	iter = m_mapCommonConfigValueList.find(tblidx);
	if (m_mapCommonConfigValueList.end() == iter)
		return NULL;
	
	return (sCOMMONCONFIG_VALUE_DATA*)(iter->second); 
}

#include <iostream>
bool CCommonConfigTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sCOMMONCONFIG_TBLDAT* pTableData = new sCOMMONCONFIG_TBLDAT;
		if (NULL == pTableData)
		{
			Destroy();
			return false;
		}

		// tblidx
		if( serializer.GetDataSize() < sizeof(pTableData->tblidx) )
		{
			delete pTableData;
			Destroy();
			return false;
		}

		serializer >> pTableData->tblidx;

		// name
		if( false == GetBinaryText( pTableData->wstrName, serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}


		// value
		if( false == GetBinaryText( pTableData->wstrValue[0], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[1], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[2], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[3], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[4], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[5], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[6], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[7], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[8], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[9], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}


		/*printf("pTableData->tblidx %d \n", pTableData->tblidx);
		std::wcout << pTableData->wstrName << "\n\n";*/
		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	}

	return true;
}

bool CCommonConfigTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sCOMMONCONFIG_TBLDAT* pTableData = (sCOMMONCONFIG_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue)
{
	__int64 nTemp = INVALID_BYTE;

	if( 0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}
	
	if( 0 > nTemp || byMaxValue < nTemp)
	{
		rDest = INVALID_BYTE;
		return false;
	}

	rDest = (BYTE) nTemp;
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue)
{
	__int64 nTemp = INVALID_DWORD;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxValue < nTemp)
	{
		rDest = INVALID_DWORD;
		return false;
	}

	rDest = (DWORD)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx)
{
	__int64 nTemp = INVALID_TBLIDX;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxTblidx < nTemp)
	{
		rDest = INVALID_TBLIDX;

		return false;
	}

	rDest = (TBLIDX)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::ReadFLOAT( float &rDest, std::wstring & wstrValue, float fMaxValue)
{
	double fTemp = INVALID_FLOAT;

	if(0 != wstrValue.size())
	{
		fTemp = _wtof(wstrValue.c_str());
	}

	if( fMaxValue < fTemp)
	{
		rDest = INVALID_FLOAT;
		return false;
	}

	rDest = (float)fTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc)
{
#ifdef _DEVEL
	if (FALSE != IsBadWritePtr( pDest, sizeof(WCHAR) * dwDestLength) )
#else
	if (NULL == pDest)
#endif
	{
		//- yoshiki : To log system!
		return false;
	}

	if ( wstrSrc.c_str()[0] != L'@')
	{
		if ( dwDestLength < (wstrSrc.length() + 1))
		{
			if (NULL != m_pfnErrorCallback)
			{
				CallErrorCallbackFunction(L"[File] : %s\n[Error] : The string[%s]'s length[%u] is bigger than the max. length[%u]",
					m_wszXmlFileName, wstrSrc.c_str(), wstrSrc.length(), dwDestLength - 1);
			}

			return false;
		}
		else
		{
			wcscpy_s( pDest, dwDestLength, wstrSrc.c_str());
		}
	}
	else
	{
		pDest[0] = L'\0';
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CCommonConfigTable::GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer)
{
	// text length
	WORD wTextLength = 0;
	if(serializer.GetDataSize() < sizeof(wTextLength))
	{
		return false;
	}

	serializer >> wTextLength;


	// value
	if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
	{
		return false;
	}

	WCHAR* pwszText = new WCHAR[wTextLength + 1];
	if (NULL == pwszText)
	{
		return false;
	}

	serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
	pwszText[wTextLength] = L'\0';

	wstrValue = pwszText;

	delete [] pwszText;

	return true;
}
