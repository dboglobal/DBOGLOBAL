#include "precomp_dboclient.h"
#include "DBOEncrypter.h"
#include "NtlXMLDoc.h"
#include "NtlFileSerializer.h"
#include "NtlResourceFileManager.h"
#include "NtlLuaState.h"

#define CONFIG_CRYPT_PASSWORD "$$$$$$%%%%%%&&&&&(((()))))===="

CDBOEncrypter::CDBOEncrypter() 
{

}

CDBOEncrypter::~CDBOEncrypter() 
{

}

RwBool CDBOEncrypter::EncryptConfig( const RwChar* szFilePath ) 
{
    // Open
    SConfigData data;
    if(!LoadConfigOptions(&data, szFilePath))
        return FALSE;

    // File Path
    char szDrive[128]   = {0,};
    char szdir[128]     = {0,};
    char szfName[128]   = {0,};
    char szExt[128]     = {0,};
    _splitpath_s(szFilePath, szDrive, szdir, szfName, szExt);

    std::string strDestFile = szDrive;
    strDestFile + szdir;
    strDestFile += szfName;
    strDestFile += ".dbo";

    if(!SaveConfigOptions(&data, strDestFile.c_str(), CONFIG_CRYPT_PASSWORD))
        return FALSE;

    return TRUE;
}

RwBool CDBOEncrypter::LoadConfigOptions(OUT SConfigData* pConfigData, const RwChar* szFilePath)
{
    CNtlXMLDoc doc;
    doc.Create();

    // xml doc load
    if(doc.Load( (char*)szFilePath ) == false)
    {
        NTL_RETURN(FALSE);
    }

    char chBuffer[1024];

    IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/config_options/op");

    if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    if(!doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->strAddr = chBuffer;

    if(!doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        NTL_RETURN(FALSE);
    }

    pConfigData->dwPort = (DWORD)atoi(chBuffer);

    pNode->Release(); 

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

	// localize
	pNode = doc.SelectSingleNode("/config_options/localize");
	if (doc.GetTextWithAttributeName(pNode, "folder", chBuffer, 1024))
	{
		pConfigData->strLocalize = chBuffer;
	}

    pNode->Release(); 

    return TRUE;
}

RwBool CDBOEncrypter::SaveConfigOptions(SConfigData* pConfigData, const RwChar* szDestPath, const RwChar* szCryptPassword)
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);

    nsl<<pConfigData->strAddr;
    nsl<<pConfigData->dwPort;
    nsl<<pConfigData->strBugTrapServerIP;
    nsl<<pConfigData->dwBugTrapServerPort;
	nsl << pConfigData->strLocalize;

    return nsl.SaveFile((RwChar*)szDestPath, TRUE, (RwChar*)szCryptPassword);
}

RwBool CDBOEncrypter::DecryptConfig(OUT SConfigData* pConfigData, const RwChar* szFilePath)
{
    CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);
    if(!nsl.LoadFile((char*)szFilePath, TRUE, CONFIG_CRYPT_PASSWORD))
        return FALSE;

    nsl>>pConfigData->strAddr;
    nsl>>pConfigData->dwPort;
    nsl>>pConfigData->strBugTrapServerIP;
    nsl>>pConfigData->dwBugTrapServerPort;
	nsl >> pConfigData->strLocalize;

    return TRUE;
}

RwBool CDBOEncrypter::CompileLuaFiles() 
{
    // Compile all the Lua files in the Script folder.
    std::string strPath = ".\\script\\";

    CNtlResourceFileManager fmLua;
    fmLua.AddExt(".lua");
    fmLua.LoadPath(strPath.c_str());


    CNtlResourceFileManager::smdef_Entity::iterator it;
    for(it = fmLua.m_smEntity.begin(); it != fmLua.m_smEntity.end(); ++it)
    {
        std::string strFileName = (*it).second;
        if(!CNtlLuaState::CompileLua(strFileName.c_str()))
            return FALSE;
    }

    return TRUE;
}
