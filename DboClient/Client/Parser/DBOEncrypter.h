#pragma once

#include "DboGlobal.h"

/// DBO 관련 컨피그및 스크립트 파일들을 암호화/복호화 하는 클래스
class CDBOEncrypter
{
public:
    CDBOEncrypter();
    ~CDBOEncrypter();    

    // ConfigOptions.Xml 
    RwBool  EncryptConfig(const RwChar* szFilePath);                 ///< Config 파일을 Encrypt한다.
    RwBool  DecryptConfig(OUT SConfigData* pConfigData, const RwChar* szFilePath);                 ///< Config 파일을 Decrypt한다.

    // Lua 컴파일 
    RwBool  CompileLuaFiles();                                      ///< Script 폴더안에 있는 모든 Lua 파일들을 컴파일한다.

protected:    
    

    // ConfigOptions.Xml 
    RwBool LoadConfigOptions(OUT SConfigData* pConfigData, const RwChar* szFilePath);             ///< Config 파일을 Load 한다.
    RwBool SaveConfigOptions(SConfigData* pConfigData, const RwChar* szDestPath, const RwChar* szCryptPassword);

protected:

};

static CDBOEncrypter* GetDBOEncrypter()
{
    static CDBOEncrypter dboEnctyper;

    return &dboEnctyper;
}