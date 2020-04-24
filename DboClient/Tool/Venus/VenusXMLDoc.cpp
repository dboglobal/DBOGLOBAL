#include "StdAfx.h"
#include <comdef.h>
#include <atlbase.h>
#include "VenusXMLDoc.h"

DWORD CVenusXMLDoc::m_dwNumberOfCreatedInstances = 0;

CVenusXMLDoc::CVenusXMLDoc(void)
{
	Init();
}

CVenusXMLDoc::~CVenusXMLDoc(void)
{
	Destroy();
}

bool CVenusXMLDoc::Create()
{
	if (NULL != m_pXMLDocument)
		return false;

	::CoInitialize(NULL);

	HRESULT hResult;

	hResult = ::CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&m_pXMLDocument);
	if (FAILED(hResult))
	{
		Destroy();
		return false;
	}

	if (NULL == m_pXMLDocument)
	{
		Destroy();
		return false;
	}

	CVenusXMLDoc::m_dwNumberOfCreatedInstances++;

	hResult = m_pXMLDocument->put_async(VARIANT_FALSE);
	if (FAILED(hResult))
	{
		Destroy();
		return false;
	}

	hResult = m_pXMLDocument->put_validateOnParse(VARIANT_FALSE);
	if (FAILED(hResult))
	{
		Destroy();
		return false;
	}

	hResult = m_pXMLDocument->put_resolveExternals(VARIANT_FALSE);
	if (FAILED(hResult))
	{
		Destroy();
		return false;
	}

	return true;
}

bool CVenusXMLDoc::Destroy()
{
	if (NULL != m_pXMLDocument)
	{
		m_pXMLDocument->Release();
		m_pXMLDocument = NULL;

		CVenusXMLDoc::m_dwNumberOfCreatedInstances--;
	}

	m_bIsFileLoaded = false;

	if (0 == CVenusXMLDoc::m_dwNumberOfCreatedInstances)
	{
		::CoUninitialize();
	}

	return true;
}

void CVenusXMLDoc::Init()
{
	m_pXMLDocument = NULL;
	m_bIsFileLoaded = false;
}

bool CVenusXMLDoc::Load(WCHAR* pwszFileName)
{
	if (false != m_bIsFileLoaded)
		return false;

	VARIANT_BOOL status = VARIANT_FALSE;
	HRESULT hResult = m_pXMLDocument->load((_variant_t)pwszFileName, &status);
	if (FAILED(hResult))
	{
		return false;
	}

	if (VARIANT_FALSE == status)
	{
		IXMLDOMParseError* pXMLError = NULL;
		m_pXMLDocument->get_parseError(&pXMLError);

		BSTR bstrErrorReasonString;
		BSTR bstrSrcText;
		LONG lLineNumber = 0;
		LONG lLinePosition = 0;

		pXMLError->get_srcText(&bstrSrcText);
		pXMLError->get_reason(&bstrErrorReasonString);
		pXMLError->get_line(&lLineNumber);
		pXMLError->get_linepos(&lLinePosition);

		::SysFreeString(bstrErrorReasonString);

		pXMLError->Release();
		
		return false;
	}

	m_bIsFileLoaded = true;
	
	return true;
}

bool CVenusXMLDoc::Load(char* pszFileName)
{
	if (false != m_bIsFileLoaded)
		return false;

	WCHAR wszUnicodeFileName[MAX_UNICODE_FILE_NAME_LENGTH + 1];
	ZeroMemory(wszUnicodeFileName, sizeof(WCHAR) * (MAX_UNICODE_FILE_NAME_LENGTH + 1));

	int iWrittenChars = ::MultiByteToWideChar(GetACP(), 0, pszFileName, -1, wszUnicodeFileName, MAX_UNICODE_FILE_NAME_LENGTH);
	wszUnicodeFileName[MAX_UNICODE_FILE_NAME_LENGTH] = L'\0';

	if (0 == iWrittenChars)
		return false;

	return Load(wszUnicodeFileName);
}

bool CVenusXMLDoc::LoadXML( char* szXMLBuffer ) 
{
	USES_CONVERSION;

	if(false != m_bIsFileLoaded)
		return false;
	
	return LoadXML(A2W(szXMLBuffer));
}

bool CVenusXMLDoc::LoadXML( WCHAR* wszXMLBuffer ) 
{
	if (false != m_bIsFileLoaded)
		return false;

	VARIANT_BOOL status;	
	HRESULT hResult = m_pXMLDocument->loadXML(wszXMLBuffer, &status);

	if (FAILED(hResult))
	{
		return false;
	}

	if (VARIANT_FALSE == status)
	{
		IXMLDOMParseError* pXMLError = NULL;
		m_pXMLDocument->get_parseError(&pXMLError);

		BSTR bstrErrorReasonString;
		pXMLError->get_reason(&bstrErrorReasonString);
		::SysFreeString(bstrErrorReasonString);

		pXMLError->Release();
		
		return false;
	}

	m_bIsFileLoaded = true;
	
	return true;
}


IXMLDOMNode* CVenusXMLDoc::SelectSingleNode(WCHAR* pwszXPath)
{
	if (false == m_bIsFileLoaded)
		return NULL;
	if (NULL == pwszXPath)
		return NULL;

	IXMLDOMNode* pXMLNode = NULL;

	BSTR bstrXPath = ::SysAllocString(pwszXPath);
	HRESULT hResult = m_pXMLDocument->selectSingleNode(bstrXPath, &pXMLNode);
	if (FAILED(hResult))
	{
		::SysFreeString(bstrXPath);
		return NULL;
	}

	::SysFreeString(bstrXPath);
	return pXMLNode;
}

IXMLDOMNode* CVenusXMLDoc::SelectSingleNode(char* pszXPath)
{
	if (false == m_bIsFileLoaded)
		return NULL;

	WCHAR wszUnicodeXPath[MAX_UNICODE_XPATH_LENGTH + 1];
	ZeroMemory(wszUnicodeXPath, sizeof(WCHAR) * (MAX_UNICODE_XPATH_LENGTH + 1));

	int iWrittenChars = ::MultiByteToWideChar(GetACP(), 0, pszXPath, -1, wszUnicodeXPath, MAX_UNICODE_XPATH_LENGTH);
	wszUnicodeXPath[MAX_UNICODE_XPATH_LENGTH] = L'\0';

	if (0 == iWrittenChars)
		return NULL;

	return SelectSingleNode(wszUnicodeXPath);
}

IXMLDOMNodeList* CVenusXMLDoc::SelectNodeList(WCHAR* pwszXPath)
{
	if (false == m_bIsFileLoaded)
		return NULL;
	if (NULL == pwszXPath)
		return NULL;

	IXMLDOMNodeList* pXMLNodeList = NULL;

	BSTR bstrXPath = ::SysAllocString(pwszXPath);
	HRESULT hResult = m_pXMLDocument->selectNodes(bstrXPath, &pXMLNodeList);
	if (FAILED(hResult))
	{
		::SysFreeString(bstrXPath);
		return NULL;
	}

	::SysFreeString(bstrXPath);
	return pXMLNodeList;
}

IXMLDOMNodeList* CVenusXMLDoc::SelectNodeList(char* pszXPath)
{
	if (false == m_bIsFileLoaded)
		return NULL;

	WCHAR wszUnicodeXPath[MAX_UNICODE_XPATH_LENGTH + 1];
	ZeroMemory(wszUnicodeXPath, sizeof(WCHAR) * (MAX_UNICODE_XPATH_LENGTH + 1));

	int iWrittenChars = ::MultiByteToWideChar(GetACP(), 0, pszXPath, -1, wszUnicodeXPath, MAX_UNICODE_XPATH_LENGTH);
	wszUnicodeXPath[MAX_UNICODE_XPATH_LENGTH] = L'\0';

	if (0 == iWrittenChars)
		return NULL;

	return SelectNodeList(wszUnicodeXPath);
}

bool CVenusXMLDoc::GetTextWithAttributeName(IXMLDOMNode* pNode, WCHAR* pwszAttributeName, WCHAR* pwszResultText, int nBufferSizeInWChars)
{
	if (NULL == pNode || NULL == pwszAttributeName || NULL == pwszResultText)
	{
//		NtlAssertFail("NULL == pNode || NULL == pwszAttributeName || NULL == pwszResultText");
		return false;
	}
	if (0 >= nBufferSizeInWChars)
	{
//		NtlAssertFail("0 >= nBufferSizeInWChars");
		return false;
	}

	IXMLDOMNamedNodeMap* pMap = NULL;
	pNode->get_attributes(&pMap);
	if (NULL == pMap)
	{
//		NtlAssertFail("Couldn't get the attribute list from the given IXMLDOMNode.");
		return false;
	}

	IXMLDOMNode* pVirtualNode = NULL;
	pMap->getNamedItem(pwszAttributeName, &pVirtualNode);
	if (NULL == pVirtualNode)
	{
//		NtlAssertFail("Couldn't find the given attribute name.");
		return false;
	}

	VARIANT var;
	VariantInit(&var);
	pVirtualNode->get_nodeValue(&var);

	if (wcslen(V_BSTR(&var)) >= (size_t)nBufferSizeInWChars)
	{
//		NtlAssertFail("The buffer size is not enough to take the whole attribute value.");
		return false;
	}

	wcscpy_s(pwszResultText, nBufferSizeInWChars, V_BSTR(&var));

	return true;
}

bool CVenusXMLDoc::GetTextWithAttributeName(IXMLDOMNode* pNode, char* pszAttributeName, char* pszResultText, int nBufferSizeInBytes)
{
	if (NULL == pNode || NULL == pszAttributeName || NULL == pszResultText)
	{
//		NtlAssertFail("NULL == pNode || NULL == pszAttributeName || NULL == pszResultText");
		return false;
	}
	if (0 >= nBufferSizeInBytes)
	{
//		NtlAssertFail("0 >= nBufferSizeInBytes");
		return false;
	}

	int nRequiredBytes = 0;
	nRequiredBytes = MultiByteToWideChar(GetACP(), 0, pszAttributeName, -1, NULL, 0);
	if (0 == nRequiredBytes)
	{
//		NtlAssertFail("The given attribute name can't be converted into WCHAR type for some reason.");
		return false;
	}
	if (nRequiredBytes > (CVenusXMLDoc::MAX_ATTRIBUTE_NAME_IN_WCHAR + 1))
	{
//		NtlAssertFail("The given attribute name is too long.");
		return false;
	}

	WCHAR pwszAttributeNameInWChar[CVenusXMLDoc::MAX_ATTRIBUTE_NAME_IN_WCHAR + 1];

	int nUsedBufferSize = MultiByteToWideChar(GetACP(), 0, pszAttributeName, -1, pwszAttributeNameInWChar, (CVenusXMLDoc::MAX_ATTRIBUTE_NAME_IN_WCHAR + 1));
	if (0 == nUsedBufferSize)
	{
//		NtlAssertFail("The given attribute name couldn't be converted into WCHAR type for some reason.");
		return false;
	}

	IXMLDOMNamedNodeMap* pMap = NULL;
	pNode->get_attributes(&pMap);
	if (NULL == pMap)
	{
//		NtlAssertFail("Couldn't get the attribute list from the given IXMLDOMNode.");
		return false;
	}

	IXMLDOMNode* pVirtualNode = NULL;
	pMap->getNamedItem(pwszAttributeNameInWChar, &pVirtualNode);
	if (NULL == pVirtualNode)
	{
//		NtlAssertFail("Couldn't find the given attribute name.");
		return false;
	}

	VARIANT var;
	VariantInit(&var);
	pVirtualNode->get_nodeValue(&var);

	nRequiredBytes = WideCharToMultiByte(::GetACP(), 0, V_BSTR(&var), -1, pszResultText, 0, NULL, NULL);
	if (nRequiredBytes > nBufferSizeInBytes)
	{
//		NtlAssertFail("The buffer size is not enough to take the whole attribute value.");
		return false;
	}

	WideCharToMultiByte(GetACP(), 0, V_BSTR(&var), -1, pszResultText, nBufferSizeInBytes, NULL, NULL);
	return true;
}

bool CVenusXMLDoc::GetDataWithXPath(WCHAR* pwszXPath, WCHAR* pwszResultData, int nBufferSizeInWChars)
{
	if (NULL == pwszXPath || NULL == pwszResultData || 0 >= nBufferSizeInWChars)
		return false;

    IXMLDOMNode* pNode = NULL;    
    m_pXMLDocument->selectSingleNode(pwszXPath, &pNode);

    if(!pNode)
        return false;

	BSTR bstr = NULL;
	if (FAILED(pNode->get_text(&bstr)))
	{
		::SysFreeString(bstr);
        pNode->Release();			
		return false;
	}

	wcscpy_s(pwszResultData, nBufferSizeInWChars, bstr);

	::SysFreeString(bstr);
    pNode->Release();

	return true;
}

bool CVenusXMLDoc::GetDataWithXPath(char* pszXPath, char* pszResultData, int nBufferSizeInBytes)
{
	if (NULL == pszXPath || NULL == pszResultData || 0 >= nBufferSizeInBytes)
		return false;

	WCHAR wszUnicodeXPath[1024 + 1];

	int iRequiredChars = ::MultiByteToWideChar(GetACP(), 0, pszXPath, -1, NULL, 0);
	if (_countof(wszUnicodeXPath) < iRequiredChars)
		return false;

	int iWrittenChars = ::MultiByteToWideChar(GetACP(), 0, pszXPath, -1, wszUnicodeXPath, _countof(wszUnicodeXPath));
	if (0 == iWrittenChars)
		return false;
	wszUnicodeXPath[_countof(wszUnicodeXPath) - 1] = L'\0';

	WCHAR wszUnicodeResultData[1024 + 1];
	if (false == GetDataWithXPath(wszUnicodeXPath, wszUnicodeResultData, _countof(wszUnicodeResultData)))
		return false;

	iRequiredChars = ::WideCharToMultiByte(GetACP(), 0, wszUnicodeResultData, -1, NULL, 0, NULL, NULL);
	if (nBufferSizeInBytes < iRequiredChars)
		return false;

	iWrittenChars = ::WideCharToMultiByte(GetACP(), 0, wszUnicodeResultData, -1, pszResultData, nBufferSizeInBytes, NULL, NULL);
	if (0 == iWrittenChars)
		return false;
	pszResultData[nBufferSizeInBytes - 1] = '\0';

	return true;
}

/**
* XML 파일의 형태를 Tab을 이용해서 보기 좋게 정렬한다.
* \param szIndentFileName 정렬에 사용할 스타일 시트 파일명
* return 성공 유무
*/
bool CVenusXMLDoc::SetIndent(WCHAR* szIndentFileName)
{
    IXMLDOMDocument* pXSL = NULL;
    CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pXSL);
    if(!pXSL)
        return false;

    VARIANT_BOOL vBool;
    pXSL->put_async(VARIANT_FALSE);
    pXSL->load((_variant_t)szIndentFileName, &vBool);

    VARIANT vObject;
    VariantInit(&vObject);
    vObject.vt = VT_DISPATCH;
    vObject.pdispVal = m_pXMLDocument;

    m_pXMLDocument->transformNodeToObject(pXSL, vObject);

    if(pXSL)
    {
        pXSL->Release();
        pXSL = NULL;
    }

    return TRUE;
}
