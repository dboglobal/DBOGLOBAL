#include "StdAfx.h"
#include "NtlPatchXmlSave.h"
#include "comutil.h"

CNtlPatchXmlSave::CNtlPatchXmlSave()
{
	m_pElemRoot = NULL;
}

CNtlPatchXmlSave::~CNtlPatchXmlSave()
{
}

BOOL CNtlPatchXmlSave::SaveCurVersion(const char *pszClientVersion, const char* pszDate, const char *pszLauncherVersion, const char *pszFullPathFileName)
{
	//읽기 전용이면 쓰기로 바꾼다.
	SetFileAttributes(pszFullPathFileName, 4);
	
	Create();
	
	IXMLDOMProcessingInstruction* pPI    = NULL;        

    m_pXMLDocument->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
	
	if(!pPI)
		return FALSE;

	m_pXMLDocument->appendChild(pPI, NULL);
    
    m_pXMLDocument->createElement(L"Version", &m_pElemRoot);
    if(!m_pElemRoot)
        return FALSE;

	IXMLDOMElement* pElemClientVersion = NULL;
    IXMLDOMElement* pElemLauncherVersion = NULL;

	m_pXMLDocument->createElement(L"Client",   &pElemClientVersion);
	m_pXMLDocument->createElement(L"Launcher", &pElemLauncherVersion);
	
	pElemClientVersion->setAttribute(L"Ver", (_variant_t)pszClientVersion);
    pElemClientVersion->setAttribute(L"Date", (_variant_t)pszDate);

	pElemLauncherVersion->setAttribute(L"Ver", (_variant_t)pszLauncherVersion);

	m_pElemRoot->appendChild(pElemClientVersion, NULL);
	m_pElemRoot->appendChild(pElemLauncherVersion, NULL);
	
	m_pXMLDocument->appendChild(m_pElemRoot, NULL);
	
	HRESULT hr = m_pXMLDocument->save((_variant_t)pszFullPathFileName);

    if(hr != S_OK)
		return FALSE;

	return TRUE;
}
