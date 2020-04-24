#include "precomp_dboclient.h"
#include "DboExtraErrorReport.h"

#include <atlbase.h>
#include <comutil.h>

CDboExtraErrorReport::CDboExtraErrorReport(void)
{
    m_pRootElem = NULL;

    Open();
}

CDboExtraErrorReport::~CDboExtraErrorReport(void)
{
    if(m_pRootElem)
    {
        m_pRootElem->Release();
        m_pRootElem = NULL;
    }    
}

RwBool CDboExtraErrorReport::Open() 
{
    Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;        

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    if(!pPI)
        return FALSE;
    m_pXMLDocument->appendChild(pPI, NULL);

    m_pXMLDocument->createElement(L"EXTRA_ERROR_INFO", &m_pRootElem);
    if(!m_pRootElem)
        return FALSE;

    m_pXMLDocument->appendChild(m_pRootElem, NULL);

    return TRUE;
}

RwBool CDboExtraErrorReport::Save(const char* szFileName)
{
    if(!szFileName)
        return FALSE;

    SetIndent();

    HRESULT hr = m_pXMLDocument->save((_variant_t)szFileName);
    if(hr != S_OK)
        return FALSE;
    
    return TRUE;
}

void CDboExtraErrorReport::WriteInfo( RwChar* szNodeName, RwChar* szInfo ) 
{
    USES_CONVERSION;   

    IXMLDOMElement* pElemTemp   = NULL;
    m_pXMLDocument->createElement(A2W(szNodeName), &pElemTemp);
    pElemTemp->put_text(A2W(szInfo));

    m_pRootElem->appendChild(pElemTemp, NULL);

}
void CDboExtraErrorReport::WriteInfo( RwChar* szNodeName, WCHAR* szInfo ) 
{
    USES_CONVERSION;   

    IXMLDOMElement* pElemTemp   = NULL;
    m_pXMLDocument->createElement(A2W(szNodeName), &pElemTemp);
    pElemTemp->put_text(szInfo);

    m_pRootElem->appendChild(pElemTemp, NULL);
}

RwBool CDboExtraErrorReport::SetIndent() 
{
    IXMLDOMDocument* pXSL = NULL;
    CoCreateInstance(__uuidof(DOMDocument30), NULL, CLSCTX_INPROC_SERVER, __uuidof(IXMLDOMDocument), (void**)&pXSL);
    if(!pXSL)
        return false;

    VARIANT_BOOL vBool;
    pXSL->put_async(VARIANT_FALSE);
    pXSL->load((_variant_t)L"indent.xsl", &vBool);

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

void CDboExtraErrorReport::SetDXDiagLog() 
{
    // Dx에서 PC관련 정보를 가져와서 txt파일을 만든다.
    SHELLEXECUTEINFO execInfo;
    ZeroMemory(&execInfo, sizeof(SHELLEXECUTEINFO));
    execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    execInfo.lpVerb = "open";
    execInfo.lpFile = "dxdiag.exe";
    execInfo.lpParameters = "/t DxLog.txt";
    execInfo.nShow = SW_HIDE;    
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT;
    ShellExecuteEx(&execInfo);    
}