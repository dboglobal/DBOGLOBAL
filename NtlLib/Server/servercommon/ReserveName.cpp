#include "stdafx.h"
#include "ReserveName.h"
#include "NtlStringHandler.h"
#include "NtlXMLLoader\NtlXMLDoc.h"
#include <codecvt>

CReserveName::CReserveName()
{
	LoadFile();
}

CReserveName::~CReserveName()
{
}


void CReserveName::LoadFile()
{
	WCHAR* path = L".\\resource\\server_data\\reserve_name.xml";

	CNtlXMLDoc doc;
	doc.Create();

	if (doc.Load(path) == false)
	{
		printf("Failed loading reserve_name.xml");
		return;
	}

	BSTR rowXPathFormat = L"/RECORDS";

	IXMLDOMNode* pAttributeNameNode = doc.SelectSingleNode(rowXPathFormat);
	if (NULL == pAttributeNameNode)
	{
		doc.Destroy();
		return;
	}
	IXMLDOMNodeList* pAttributeNameNodeList = NULL;
	HRESULT hResult = pAttributeNameNode->get_childNodes(&pAttributeNameNodeList);
	if (S_OK != hResult)
	{
		pAttributeNameNode->Release();

		doc.Destroy();
		return;
	}

	long length = 0;
	if (FAILED(pAttributeNameNodeList->get_length(&length)))
	{
		pAttributeNameNodeList->Release();
		pAttributeNameNode->Release();

		doc.Destroy();

		return;
	}

	BSTR bstr = NULL;
	BSTR bstr2 = NULL;

	//get records
	// Retrieves field names.
	for (long i = 0; i<length; i++)
	{
		IXMLDOMNode* pCellNode = NULL;
		IXMLDOMNode* pDataNode = NULL;

		if (FAILED(pAttributeNameNodeList->get_item(i, &pCellNode)))
		{
			pAttributeNameNode->Release();
			pAttributeNameNodeList->Release();
			::SysFreeString(rowXPathFormat);
			doc.Destroy();
			printf("d \n");
			return;
		}
		if (NULL == pCellNode)
			break;

		//get name
		{
			if (FAILED(pCellNode->get_firstChild(&pDataNode)))
			{
				pCellNode->Release();
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				printf("e \n");
				return;
			}
			if (NULL == pDataNode)
			{

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
				printf("f \n");
				return;
			}
		}

		//get account id
		{
			if (FAILED(pCellNode->get_lastChild(&pDataNode)))
			{
				pCellNode->Release();
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				printf("e \n");
				return;
			}
			if (NULL == pDataNode)
			{

				pCellNode->Release();
				break;
			}

			if (FAILED(pDataNode->get_text(&bstr2)))
			{
				::SysFreeString(bstr2);
				pDataNode->Release();
				pCellNode->Release();
				pAttributeNameNode->Release();
				pAttributeNameNodeList->Release();
				::SysFreeString(rowXPathFormat);
				doc.Destroy();
				printf("f \n");
				return;
			}
		}

		std::string text = ws2s(std::wstring(bstr));
		std::string text2 = ws2s(std::wstring(bstr2));

		m_mapNames.insert({ text.c_str(), std::stoi(text2) });

		::SysFreeString(bstr);
		::SysFreeString(bstr2);
		pDataNode->Release();
		pCellNode->Release();
	}

/*	for (std::map<const std::string, unsigned int>::iterator it = m_mapNames.begin(); it != m_mapNames.end(); it++)
	{
		printf("%s %u \n", it->first.c_str(), it->second);
	}*/
}

void CReserveName::ReloadFile()
{
	m_mapNames.clear();

	LoadFile();
}

bool CReserveName::IsReserved(std::string & strName, unsigned int accountid)
{
	std::map<const std::string, unsigned int>::iterator it = m_mapNames.find(strName);
	if (it != m_mapNames.end())
	{
		if (it->second != accountid)
			return true;
	}

	return false;
}


bool CReserveName::IsReserved(WCHAR* wchName, unsigned int accountid)
{
	std::string strName = ws2s(wchName);

	std::map<const std::string, unsigned int>::iterator it = m_mapNames.find(strName);
	if (it != m_mapNames.end())
	{
		if (it->second != accountid)
			return true;
	}

	return false;
}