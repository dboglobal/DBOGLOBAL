#include "StdAfx.h"
#include "NtlPatchServerConfig.h"
#include "NtlXMLDoc.h"

#define NODE_SERVER_PATCH_ENABLE    "PATCH_SERVER_CONFIG/PATCH_ENABLE"
#define NODE_SERVER_FULLVER_IP      "PATCH_SERVER_CONFIG/FULLVER_SERVER_IP"
#define NODE_SERVER_FULLVER_FOLDER  "PATCH_SERVER_CONFIG/FULLVER_SERVER_FOLDER"
#define NODE_SERVER_ERR_MSG         "PATCH_SERVER_CONFIG/PATCH_LOCK_MSG"

CNtlPatchServerConfig* CNtlPatchServerConfig::m_pInstance = NULL;

CNtlPatchServerConfig::CNtlPatchServerConfig() 
{
    m_bPatchEnable = TRUE;
}
CNtlPatchServerConfig::~CNtlPatchServerConfig() 
{

}

BOOL CNtlPatchServerConfig::LoadServerConfigFile( const char* pszXMLFileName ) 
{
    CNtlXMLDoc doc;
    doc.Create();

    if(!doc.Load(const_cast<char*>(pszXMLFileName)))
    {
        // 파일이 존재하지 않음
        return FALSE;
    }

    char buf[1024] = {0,};

    // 패치 가능 유무
    if(doc.GetDataWithXPath(NODE_SERVER_PATCH_ENABLE, buf, sizeof(buf)))
    {
        m_bPatchEnable = atoi(buf);
    }

    // 에러 메시지
    if(doc.GetDataWithXPath(NODE_SERVER_ERR_MSG, buf, sizeof(buf)))
    {
        m_strErrorMsg = buf;
    }

    // 풀버전이 있는 서버의 IP
    if(doc.GetDataWithXPath(NODE_SERVER_FULLVER_IP, buf, sizeof(buf)))
    {
        m_strFullVerServerIP = buf;
    }

    // 풀버전이 있는 서버의 폴더 경로
    if(doc.GetDataWithXPath(NODE_SERVER_FULLVER_FOLDER, buf, sizeof(buf)))
    {
        m_strFullVerServerFolder = buf;
    }

    return TRUE;
}

CNtlPatchServerConfig* CNtlPatchServerConfig::GetInstance() 
{
    if(!m_pInstance)
    {
        m_pInstance = new CNtlPatchServerConfig();
    }

    return m_pInstance;
}

void CNtlPatchServerConfig::DeleteInstance() 
{
    delete m_pInstance;
    m_pInstance = NULL;
}