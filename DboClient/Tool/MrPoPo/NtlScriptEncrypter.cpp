#include "StdAfx.h"
#include "NtlScriptEncrypter.h"
#include "NtlXMLDoc.h"
#include "NtlFileSerializer.h"

#define CRYPT_PASSWORD "!@*&(agebreak"

CNtlScriptEncrypter::CNtlScriptEncrypter(void)
{
}

CNtlScriptEncrypter::~CNtlScriptEncrypter(void)
{
}

/**
 * 컨피그 설정 파일을 로드한다. 확장자로 XML,EDF를 구별한다.
 * \param pConfigData 로드한 데이터를 저장할 포인터
 * \param szFileName 로드할 파일 이름
 * \return 
 */
BOOL CNtlScriptEncrypter::LoadConfigOption( OUT SConfigData* pConfigData, char* szFileName ) 
{
    if(!pConfigData || !szFileName)
        return FALSE;

    char szDrive[128]   = {0,};
    char szdir[128]     = {0,};
    char szfName[128]   = {0,};
    char szExt[128]     = {0,};
    _splitpath_s(szFileName, szDrive, szdir, szfName, szExt);    
    _strlwr_s(szExt);
    
    if(strcmp(szExt, ".xml") == 0)
    {
        return LoadConfigOptionXML(pConfigData, szFileName);
    }
    else if(strcmp(szExt, ".edf") == 0)
    {
        return LoadConfigOptionENC(pConfigData, szFileName);
    }

    return FALSE;
}

BOOL CNtlScriptEncrypter::LoadConfigOptionXML( OUT SConfigData* pConfigData, char* szFileName ) 
{
    CNtlXMLDoc doc;
    doc.Create();

    // xml doc load
    if(doc.Load( (char*)szFileName ) == false)
    {
        return FALSE;
    }

    char chBuffer[1024];

    // config operation을 얻어온다.
    IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/config_options/op");

    if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
    {
        return FALSE;
    }

    if(!doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        return FALSE;
    }

    pConfigData->strAddr = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        return FALSE;
    }

    pConfigData->dwPort = (DWORD)atoi(chBuffer);

    pNode->Release(); 

    // config local을 얻어온다.

    pNode = doc.SelectSingleNode((char*)"/config_options/local");

    if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
    {
        return FALSE;
    }

    if(!doc.GetTextWithAttributeName(pNode, "local_dsp", chBuffer, 1024))
    {
        return FALSE;
    }

    pConfigData->strLocalDsp = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "local_sync_dsp", chBuffer, 1024))
    {
        return FALSE;
    }

    pConfigData->strLocalSyncDsp = chBuffer;

    // Bug Trap
    pNode = doc.SelectSingleNode("/config_options/BUGTRAP");
    if(doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        pConfigData->strBugTrapServerIP = chBuffer;
    }

    if(doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        pConfigData->dwBugTrapServerPort = atoi(chBuffer);        
    }

    pNode->Release(); 

    return TRUE;
}

BOOL CNtlScriptEncrypter::LoadConfigOptionENC( OUT SConfigData* pConfigData, char* szFileName ) 
{
    USES_CONVERSION;

    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);
    if(!nsl.LoadFile((char*)szFileName, TRUE, CRYPT_PASSWORD))
        return FALSE;

    std::string strAddr = "";
    std::string strLocalDsp = "";
    std::string strLocalSyncDsp = "";
    std::string strBugTrapServerIP ="";

    nsl>>strAddr;
    nsl>>pConfigData->dwPort;
    nsl>>strLocalDsp;
    nsl>>strLocalSyncDsp;
    nsl>>strBugTrapServerIP;
    nsl>>pConfigData->dwBugTrapServerPort;

    pConfigData->strAddr = A2W(strAddr.c_str());
    pConfigData->strLocalDsp = A2W(strLocalDsp.c_str());
    pConfigData->strLocalSyncDsp = A2W(strLocalSyncDsp.c_str());
    pConfigData->strBugTrapServerIP = A2W(strBugTrapServerIP.c_str());

    return TRUE;
}

BOOL CNtlScriptEncrypter::SaveConfigOption(SConfigData* pConfigData, char* szFileName, BOOL bEncrypt /* = FALSE */)
{
    if(!pConfigData || !szFileName)
        return FALSE;

    if(bEncrypt)
    {
        return SaveConfigOptionENC(pConfigData, szFileName, CRYPT_PASSWORD);
    }
    else
    {
        return SaveConfigOptionXML(pConfigData, szFileName);
    }    
}

BOOL CNtlScriptEncrypter::SaveConfigOptionXML( SConfigData* pConfigData, char* szFileName ) 
{
    USES_CONVERSION;

    CNtlXMLDoc doc;
    doc.Create();

    IXMLDOMProcessingInstruction* pPI    = NULL;        

    doc.GetDocument()->createProcessingInstruction(L"xml", L"version=\"1.0\" encoding = \"UTF-8\"", &pPI);
    if(!pPI)
        return FALSE;
    doc.GetDocument()->appendChild(pPI, NULL);

    IXMLDOMElement*     pElemRoot = NULL;                ///< Root Element
    IXMLDOMElement*     pElemOp = NULL;
    IXMLDOMElement*     pElemLocal = NULL;
    IXMLDOMElement*     pElemBUGTrap = NULL;

    doc.GetDocument()->createElement(L"config_options", &pElemRoot);
    doc.GetDocument()->appendChild(pElemRoot, NULL);

    WCHAR wbuf[1204] = {0,};

    doc.GetDocument()->createElement(L"op", &pElemOp);
    pElemOp->setAttribute(L"ver", (_variant_t)L"0.1");
    pElemOp->setAttribute(L"ip", (_variant_t)pConfigData->strAddr);
    swprintf_s(wbuf, L"%d", pConfigData->dwPort);
    pElemOp->setAttribute(L"port", (_variant_t)wbuf);    

    doc.GetDocument()->createElement(L"local", &pElemLocal);
    pElemLocal->setAttribute(L"ver", (_variant_t)L"0.1");
    pElemLocal->setAttribute(L"local_dsp", (_variant_t)pConfigData->strLocalDsp);
    pElemLocal->setAttribute(L"local_sync_dsp", (_variant_t)pConfigData->strLocalSyncDsp);    

    doc.GetDocument()->createElement(L"BUGTRAP", &pElemBUGTrap);
    pElemBUGTrap->setAttribute(L"ip", (_variant_t)pConfigData->strBugTrapServerIP);
    swprintf_s(wbuf, L"%d", pConfigData->dwBugTrapServerPort);
    pElemBUGTrap->setAttribute(L"port", (_variant_t)wbuf);

    pElemRoot->appendChild(pElemOp, NULL);
    pElemRoot->appendChild(pElemLocal, NULL);
    pElemRoot->appendChild(pElemBUGTrap, NULL);

    doc.SetIndent(L"indent.xsl");
    HRESULT hr = doc.GetDocument()->save((_variant_t)szFileName);
    if(hr != S_OK)
        return FALSE;

    return TRUE;
}

BOOL CNtlScriptEncrypter::SaveConfigOptionENC( SConfigData* pConfigData, char* szFileName, char* szCryptPassword ) 
{
    USES_CONVERSION;

    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);

    std::string strAddr = W2A(pConfigData->strAddr);
    std::string strLocalDsp = W2A(pConfigData->strLocalDsp);
    std::string strLocalSyncDsp = W2A(pConfigData->strLocalSyncDsp);
    std::string strBugTrapServerIP = W2A(pConfigData->strBugTrapServerIP);

    nsl<<strAddr;
    nsl<<pConfigData->dwPort;
    nsl<<strLocalDsp;
    nsl<<strLocalSyncDsp;
    nsl<<strBugTrapServerIP;
    nsl<<pConfigData->dwBugTrapServerPort;

    return nsl.SaveFile(szFileName, TRUE, szCryptPassword);

    return TRUE;
}