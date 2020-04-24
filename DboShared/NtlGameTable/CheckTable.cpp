//***********************************************************************************
//
//	File		:	CheckTable.cpp
//
//	Begin		:	2006-08-03
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Desc		:	
//
//************************************************************************************

#include "stdafx.h"
#include "CheckTable.h"
#include "NtlXMLDoc.h"

CCheckTable::CCheckTable(void)
{
	Init();
}

CCheckTable::~CCheckTable(void)
{
	Destroy();
}

bool CCheckTable::Create(WCHAR* pwszFileName, DWORD dwCodePage)
{
	if (NULL == pwszFileName)
	{
		Destroy();
		return false;
	}

	CNtlXMLDoc doc;
	doc.Create();

	if (false == doc.Load(pwszFileName))
	{
		doc.Destroy();
		Destroy();
		return false;
	}

	// Stores the XML file name.
	{
		wcsncpy_s<_countof(m_wszXmlFileName)>(m_wszXmlFileName, pwszFileName, _countof(m_wszXmlFileName) - 1);
		m_wszXmlFileName[_countof(m_wszXmlFileName) - 1] = L'\0';
	}

	BSTR rowXPathFormat;
	rowXPathFormat = ::SysAllocString(L"/dataroot/%s[%d]");
	WCHAR wszXPath[CNtlXMLDoc::MAX_UNICODE_XPATH_LENGTH + 1];

	DWORD dwSheetIndex = 0;
	WCHAR** ppwszSheetList = GetSheetListInWChar();

	while (NULL != ppwszSheetList[dwSheetIndex])
	{
		swprintf_s<_countof(wszXPath)>(wszXPath, rowXPathFormat, ppwszSheetList[dwSheetIndex], 0);

		IXMLDOMNode* pAttributeNameNode = NULL;
		pAttributeNameNode = doc.SelectSingleNode(wszXPath);
		if (NULL == pAttributeNameNode)
		{
			::SysFreeString(rowXPathFormat);
			doc.Destroy();
			Destroy();
			return false;
		}
		IXMLDOMNodeList* pAttributeNameNodeList = NULL;
		HRESULT hResult = pAttributeNameNode->get_childNodes(&pAttributeNameNodeList);
		if (S_OK != hResult)
		{
			pAttributeNameNode->Release();
			::SysFreeString(rowXPathFormat);
			doc.Destroy();
			Destroy();
			return false;
		}

		long length = 0;
		if (FAILED(pAttributeNameNodeList->get_length(&length)))
		{
			pAttributeNameNodeList->Release();
			pAttributeNameNode->Release();
			::SysFreeString(rowXPathFormat);
			doc.Destroy();
			Destroy();
			return false;
		}

		BSTR bstr = NULL;
		std::map<long, std::wstring> mapAttributeMapping;

		// Retrieves field names.
		for (long i=0 ; i<length ; i++)
		{
			IXMLDOMNode* pCellNode = NULL;
			IXMLDOMNode* pDataNode = NULL;
			if (FAILED(pAttributeNameNodeList->get_item(i, &pCellNode)))
			{
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				Destroy();
				return false;
			}
			if (NULL == pCellNode)
				break;

			if (FAILED(pCellNode->get_firstChild(&pDataNode)))
			{
				pCellNode->Release();
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				Destroy();
				return false;
			}
			if (NULL == pDataNode)
			{
				CTable::CallErrorCallbackFunction("[File] : %S\n[Error] : A field name may be null.(Index : %d)", pwszFileName, i);

				pCellNode->Release();
				break;
			}

			if (FAILED(pDataNode->get_text(&bstr)))
			{
				::SysFreeString(bstr);
				pDataNode->Release();
				pCellNode->Release();
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				Destroy();
				return false;
			}

			mapAttributeMapping.insert(std::pair<long, std::wstring>(i, std::wstring(bstr)));

			::SysFreeString(bstr);
			pDataNode->Release();
			pCellNode->Release();
		}
		pAttributeNameNode->Release();
		pAttributeNameNodeList->Release();

		int iRowIndex = 1;

		// Retrieves data.
		bool bLoop = true;
		do
		{
			IXMLDOMNode* pRoot = NULL;
			IXMLDOMNodeList* pRow = NULL;
			swprintf_s<_countof(wszXPath)>(wszXPath, rowXPathFormat, ppwszSheetList[dwSheetIndex], iRowIndex);

			pRoot = doc.SelectSingleNode(wszXPath);
			if (NULL == pRoot)
			{
				::SysFreeString(rowXPathFormat);
				break;
			}

			HRESULT hResult = pRoot->get_childNodes(&pRow);
			if (S_OK != hResult)
			{
				pRoot->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				Destroy();
				return false;
			}

			void* pvTable = AllocNewTable(ppwszSheetList[dwSheetIndex], dwCodePage);
			if (NULL == pvTable)
			{
				pRow->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				Destroy();

				return false;
			}

			bool bIsEndOfSheet = false;

			for (long i=0 ; i<length ; i++)
			{
				IXMLDOMNode* pCellNode = NULL;
				IXMLDOMNode* pDataNode = NULL;

				if (FAILED(pRow->get_item(i, &pCellNode)))
				{
					DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
					pRow->Release();
					::SysFreeString(rowXPathFormat);
					doc.Destroy();
					Destroy();

					return false;
				}

				if (NULL == pCellNode)
				{
					if (0 == i)
					{
						bIsEndOfSheet = true;
						break;
					}
					else
					{
						continue;
					}
				}

				std::map<long, std::wstring>::iterator mapIt;
				mapIt = mapAttributeMapping.find(i);
				if (mapAttributeMapping.end() == mapIt)
				{
					DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
					pCellNode->Release();
					pRow->Release();
					::SysFreeString(rowXPathFormat);
					doc.Destroy();
					Destroy();

					return false;
				}

				if (FAILED(pCellNode->get_firstChild(&pDataNode)))
				{
					CTable::CallErrorCallbackFunction("[File] : %S\n[Error] : Table data is null.(Row Index : %d, Field Name = %S)", pwszFileName, iRowIndex - 1, (mapIt->second).c_str());

					DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
					pCellNode->Release();
					pRow->Release();
					::SysFreeString(rowXPathFormat);
					doc.Destroy();
					Destroy();

					return false;
				}

				if (NULL == pDataNode)
				{
					CTable::CallErrorCallbackFunction("[File] : %S\n[Error] : Table data is null.(Row Index : %d, Field Name = %S)", pwszFileName, iRowIndex - 1, (mapIt->second).c_str());

					pCellNode->Release();

					if (0 == i)
					{
						DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
						bIsEndOfSheet = true;
						break;
					}
					else
					{
						continue;
					}
				}

				if (FAILED(pDataNode->get_text(&bstr)))
				{
					pDataNode->Release();
					pCellNode->Release();
					DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
					pRow->Release();
					::SysFreeString(rowXPathFormat);
					doc.Destroy();
					Destroy();

					return false;
				}

				SetTableData(pvTable, ppwszSheetList[dwSheetIndex], &(mapIt->second), bstr);

				::SysFreeString(bstr);
				pDataNode->Release();
				pCellNode->Release();
			}
			pRow->Release();

			if (false == bIsEndOfSheet)
			{
				if (false == AddTable(pvTable, ppwszSheetList[dwSheetIndex]))
				{
					DeallocNewTable(pvTable, ppwszSheetList[dwSheetIndex]);
				}

				iRowIndex++;
			}
			else
			{
				bLoop = false;
			}
		} while (false != bLoop);

		dwSheetIndex++;
	}

	::SysFreeString(rowXPathFormat);

	doc.Destroy();

	return true;
}

bool CCheckTable::Create(char* pszFileName, DWORD dwCodePage)
{
	if (NULL == pszFileName)
		return false;

	WCHAR wszUnicodeFileName[CNtlXMLDoc::MAX_UNICODE_FILE_NAME_LENGTH + 1];
	ZeroMemory(wszUnicodeFileName, sizeof(wszUnicodeFileName));

	int iWrittenChars = ::MultiByteToWideChar(::GetACP(), 0, pszFileName, -1, wszUnicodeFileName, CNtlXMLDoc::MAX_UNICODE_FILE_NAME_LENGTH);
	wszUnicodeFileName[CNtlXMLDoc::MAX_UNICODE_FILE_NAME_LENGTH] = L'\0';

	if (0 == iWrittenChars)
		return false;

	return Create(wszUnicodeFileName, dwCodePage);
}

void CCheckTable::Destroy()
{
	for( CHTABLEIT it = m_mapTableList.begin(); it != m_mapTableList.end(); it++ )
	{
		delete it->second;
	}

	m_mapTableList.clear();

	::ZeroMemory(m_wszXmlFileName, sizeof(m_wszXmlFileName));
}

void CCheckTable::Init()
{
	m_mapTableList.clear();

	::ZeroMemory(m_wszXmlFileName, sizeof(m_wszXmlFileName));
}